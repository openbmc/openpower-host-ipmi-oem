#include <arpa/inet.h>
#include <endian.h>
#include <limits.h>
#include <mapper.h>
#include <netinet/in.h>

#include <cstring>
#include <filesystem>
#include <fstream>
#include <ipmid/api.hpp>
#include <ipmid/types.hpp>
#include <ipmid/utils.hpp>
#include <map>
#include <phosphor-logging/elog-errors.hpp>
#include <phosphor-logging/log.hpp>
#include <sdbusplus/bus.hpp>
#include <sdbusplus/message/types.hpp>
#include <sdbusplus/server/object.hpp>
#include <string>
#include <xyz/openbmc_project/Common/error.hpp>
#include <xyz/openbmc_project/State/Chassis/server.hpp>
#include <xyz/openbmc_project/State/Host/server.hpp>

static void registerNetfnChassisFunctions() __attribute__((constructor));

// Phosphor Host State manager
namespace State = sdbusplus::xyz::openbmc_project::State::server;

namespace fs = std::filesystem;

using namespace phosphor::logging;
using namespace sdbusplus::xyz::openbmc_project::Common::Error;

// Various Chassis operations under a single command.
enum ipmi_chassis_control_cmds : uint8_t
{
    CMD_POWER_OFF = 0x00,
    CMD_POWER_ON = 0x01,
    CMD_POWER_CYCLE = 0x02,
    CMD_HARD_RESET = 0x03,
    CMD_PULSE_DIAGNOSTIC_INTR = 0x04,
    CMD_SOFT_OFF_VIA_OVER_TEMP = 0x05,
};

//------------------------------------------
// Calls into Host State Manager Dbus object
//------------------------------------------
static int initiateHostStateTransition(ipmi::Context::ptr& ctx,
                                       State::Host::Transition transition)
{
    // OpenBMC Host State Manager dbus framework
    constexpr auto hostStatePath = "/xyz/openbmc_project/state/host0";
    constexpr auto hostStateIntf = "xyz.openbmc_project.State.Host";

    // Convert to string equivalent of the passed in transition enum.
    auto request = State::convertForMessage(transition);

    std::string service;
    boost::system::error_code ec =
        ipmi::getService(ctx, hostStateIntf, hostStatePath, service);

    if (!ec)
    {
        ec = ipmi::setDbusProperty(ctx, service, hostStatePath, hostStateIntf,
                                   "RequestedHostTransition", request);
    }
    if (ec)
    {
        log<level::ERR>("Failed to initiate transition",
                        entry("EXCEPTION=%s, REQUEST=%s", ec.message().c_str(),
                              request.c_str()));
        return -1;
    }
    return 0;
}

//------------------------------------------
// Set Enabled property to inform NMI source
// handling to trigger a NMI_OUT BSOD.
//------------------------------------------
static int setNmiProperty(ipmi::Context::ptr& ctx, const bool value)
{
    constexpr const char* nmiSourceObjPath =
        "/xyz/openbmc_project/Chassis/Control/NMISource";
    constexpr const char* nmiSourceIntf =
        "xyz.openbmc_project.Chassis.Control.NMISource";
    std::string bmcSourceSignal = "xyz.openbmc_project.Chassis.Control."
                                  "NMISource.BMCSourceSignal.ChassisCmd";

    std::string service;
    boost::system::error_code ec =
        ipmi::getService(ctx, nmiSourceIntf, nmiSourceObjPath, service);
    if (!ec)
    {
        ec = ipmi::setDbusProperty(ctx, service, nmiSourceObjPath,
                                   nmiSourceIntf, "BMCSource", bmcSourceSignal);
    }
    if (!ec)
    {
        ec = ipmi::setDbusProperty(ctx, service, nmiSourceObjPath,
                                   nmiSourceIntf, "Enabled", value);
    }
    if (ec)
    {
        log<level::ERR>("Failed to trigger NMI_OUT",
                        entry("EXCEPTION=%s", ec.message().c_str()));
        return -1;
    }

    return 0;
}

//-------------------------------------------------------------
// Send a command to SoftPowerOff application to stop any timer
//-------------------------------------------------------------
static int stopSoftOffTimer(ipmi::Context::ptr& ctx)
{
    static constexpr auto softOffBusName =
        "xyz.openbmc_project.Ipmi.Internal.SoftPowerOff";
    static constexpr auto softOffObjPath =
        "/xyz/openbmc_project/ipmi/internal/soft_power_off";
    constexpr auto softOffIface = "xyz.openbmc_project.Ipmi.Internal."
                                  "SoftPowerOff";
    constexpr auto property = "ResponseReceived";
    constexpr auto value = "xyz.openbmc_project.Ipmi.Internal."
                           "SoftPowerOff.HostResponse.HostShutdown";

    // TODO get softOffBusName from mapper: ipmi::getService(ctx, ...);

    // No error object or reply expected.
    boost::system::error_code ec = ipmi::setDbusProperty(
        ctx, softOffBusName, softOffObjPath, softOffIface, property, value);

    if (ec)
    {
        log<level::ERR>("Failed to set property in SoftPowerOff object",
                        entry("ERROR=%s", ec.message().c_str()));
        return -1;
    }

    return 0;
}

//----------------------------------------------------------------------
// Create file to indicate there is no need for softoff notification to host
//----------------------------------------------------------------------
static void indicateNoSoftOffNeeded()
{
    static constexpr auto HOST_INBAND_REQUEST_DIR = "/run/openbmc/";
    static constexpr auto HOST_INBAND_REQUEST_FILE = "host@%u-request";
    fs::path path{HOST_INBAND_REQUEST_DIR};
    if (!fs::is_directory(path))
    {
        fs::create_directory(path);
    }

    // Add the host instance (default 0 for now) to the file name
    std::string file{HOST_INBAND_REQUEST_FILE};
    auto size = std::snprintf(nullptr, 0, file.c_str(), 0);
    size++; // null
    std::unique_ptr<char[]> buf(new char[size]);
    std::snprintf(buf.get(), size, file.c_str(), 0);

    // Append file name to directory and create it
    path /= buf.get();
    std::ofstream(path.c_str());
}

/** @brief Implementation of chassis control command
 *
 *  @param - chassisControl command byte
 *
 *  @return  Success or InvalidFieldRequest.
 */
static ipmi::RspType<> ipmiChassisControl(ipmi::Context::ptr& ctx,
                                          uint8_t chassisControl)
{
    int rc = 0;
    switch (chassisControl)
    {
        case CMD_POWER_ON:
            rc = initiateHostStateTransition(ctx, State::Host::Transition::On);
            break;
        case CMD_POWER_OFF:
            // This path would be hit in 2 conditions.
            // 1: When user asks for power off using ipmi chassis command 0x04
            // 2: Host asking for power off post shutting down.

            // If it's a host requested power off, then need to nudge Softoff
            // application that it needs to stop the watchdog timer if running.
            // If it is a user requested power off, then this is not really
            // needed. But then we need to differentiate between user and host
            // calling this same command

            // For now, we are going ahead with trying to nudge the soft off and
            // interpret the failure to do so as a non softoff case
            rc = stopSoftOffTimer(ctx);

            // Only request the Off transition if the soft power off
            // application is not running
            if (rc < 0)
            {
                // First create a file to indicate to the soft off application
                // that it should not run. Not doing this will result in State
                // manager doing a default soft power off when asked for power
                // off.
                indicateNoSoftOffNeeded();

                // Now request the shutdown
                rc = initiateHostStateTransition(ctx,
                                                 State::Host::Transition::Off);
            }
            else
            {
                log<level::INFO>("Soft off is running, so let shutdown target "
                                 "stop the host");
            }
            break;

        case CMD_HARD_RESET:
            rc = initiateHostStateTransition(
                ctx, State::Host::Transition::ForceWarmReboot);
            break;
        case CMD_POWER_CYCLE:
            // SPEC has a section that says certain implementations can trigger
            // PowerOn if power is Off when a command to power cycle is
            // requested

            // First create a file to indicate to the soft off application
            // that it should not run since this is a direct user initiated
            // power reboot request (i.e. a reboot request that is not
            // originating via a soft power off SMS request)
            indicateNoSoftOffNeeded();

            rc = initiateHostStateTransition(ctx,
                                             State::Host::Transition::Reboot);
            break;

        case CMD_SOFT_OFF_VIA_OVER_TEMP:
            // Request Host State Manager to do a soft power off
            rc = initiateHostStateTransition(ctx, State::Host::Transition::Off);
            break;

        case CMD_PULSE_DIAGNOSTIC_INTR:
            rc = setNmiProperty(ctx, true);
            break;

        default: {
            log<level::ERR>("Invalid Chassis Control command",
                            entry("CMD=0x%X", chassisControl));
            return ipmi::responseInvalidFieldRequest();
        }
    }

    return ((rc < 0) ? ipmi::responseUnspecifiedError()
                     : ipmi::responseSuccess());
}

static void registerNetfnChassisFunctions()
{

    // <Chassis Control>
    ipmi::registerHandler(ipmi::prioOemBase, ipmi::netFnChassis,
                          ipmi::chassis::cmdChassisControl,
                          ipmi::Privilege::Operator, ipmiChassisControl);
}
