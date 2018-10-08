#include "config.h"
#include "ldap.hpp"
#include <host-ipmid/ipmid-host-cmd.hpp>
#include <phosphor-logging/log.hpp>

namespace ldap
{

static constexpr auto ldapCfgPath = "/xyz/openbmc_project/user/ldap/config";
static constexpr auto deleteIface = "xyz.openbmc_project.Object.Delete";
static constexpr auto deleteMethod = "Delete";

using namespace phosphor::logging;

ipmi_ret_t deleteConfig()
{
    auto& bus = ipmid_get_sdbus_plus_handler();
    std::string service;

    try
    {
        auto method = bus->new_method_call(MAPPER_BUS_NAME,
                                           MAPPER_OBJ,
                                           MAPPER_IFACE,
                                           "GetObject");
        method.append(ldapCfgPath, std::vector<std::string>{deleteIface});

        auto reply = bus->call(method);

        std::map<std::string, std::vector<std::string>> result;
        reply.read(result);

        if (!result.empty())
        {
            service = result.begin()->first;
        }
    }
    catch (sdbusplus::exception::SdBusError& e)
    {
        //No LDAP object, no problem
    }

    if (service.empty())
    {
        return IPMI_CC_OK;
    }

    try
    {
        auto method = bus->new_method_call(service.c_str(),
                                           ldapCfgPath,
                                           deleteIface,
                                           deleteMethod);
        auto reply = bus->call(method);
        if (reply.is_method_error())
        {
            log<level::ERR>("Method error on LDAP delete config call");
            return IPMI_CC_UNSPECIFIED_ERROR;
        }
    }
    catch (sdbusplus::exception::SdBusError& e)
    {
        log<level::ERR>("Failed deleting LDAP configuration",
                entry("ERROR=%s", e.what()));
        return IPMI_CC_UNSPECIFIED_ERROR;
    }

    return IPMI_CC_OK;
}

}
