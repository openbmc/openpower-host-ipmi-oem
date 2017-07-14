#include <chrono>
#include <functional>
#include <oemhandler.hpp>
#include <host-ipmid/ipmid-host-cmd.hpp>
#include <host-ipmid/host-cmd-manager.hpp>
#include <host-ipmid/utils.hpp>
#include <phosphor-logging/log.hpp>
#include <host-interface.hpp>
#include <config.h>
namespace open_power
{
namespace host
{
namespace command
{

// Maps IPMI command to it's counterpart in interface
const std::map<uint8_t, Host::Command> Host::intfCommand = {
    {
        IPMI_CMD_OCC_RESET,
            Base::Host::Command::OCCReset
    }
};

// Called at user request
void Host::execute(Base::Host::Command command, uint8_t data)
{
    using namespace phosphor::logging;

    log<level::INFO>("Pushing cmd on to queue",
            entry("CONTROL_HOST_CMD=%s",
                  convertForMessage(command)));

    auto cmd = std::make_tuple(std::make_pair(IPMI_CMD_OCC_RESET, data),
                        std::bind(&Host::commandStatusHandler,
                            this, std::placeholders::_1,
                                std::placeholders::_2));

    return cmdManager->execute(cmd);
}

// Called into by Command Manager
void Host::commandStatusHandler(IpmiCmdData cmd, bool status)
{
    // Need to convert <cmd> to the equivalent one mentioned in spec
    auto value = status ? Result::Success : Result::Failure;

    // Fire a signal
    this->commandComplete(intfCommand.at(std::get<0>(cmd)), value);
}

} // namespace command
} // namespace host
} // namepsace open_power
