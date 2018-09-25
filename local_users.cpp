#include "local_users.hpp"
#include <host-ipmid/ipmid-host-cmd.hpp>
#include <phosphor-logging/log.hpp>

namespace local
{
namespace users
{

using namespace phosphor::logging;

constexpr auto MAPPER_BUS_NAME = "xyz.openbmc_project.ObjectMapper";
constexpr auto MAPPER_OBJ = "/xyz/openbmc_project/object_mapper";
constexpr auto MAPPER_IFACE = "xyz.openbmc_project.ObjectMapper";
constexpr auto USER_IFACE = "xyz.openbmc_project.User.Attributes";
constexpr auto PROP_IFACE = "org.freedesktop.DBus.Properties";

using DbusObjectPath = std::string;
using DbusService = std::string;
using DbusInterface = std::string;
using ObjectTree = std::map<DbusObjectPath,
                            std::map<DbusService, std::vector<DbusInterface>>>;

/**
 * @brief Gets a list of all local users in the form of GetSubTree
 *        results.
 *
 * @param[out] users - Filled in with the results of a
 *                     GetSubTree call that returns all users.
 */
void getUsers(ObjectTree& users)
{
    auto& bus = ipmid_get_sdbus_plus_handler();

    try
    {
        auto method = bus->new_method_call(MAPPER_BUS_NAME,
                MAPPER_OBJ,
                MAPPER_IFACE,
                "GetSubTree");
        method.append("/", 0, std::vector<std::string>{USER_IFACE});

        auto reply = bus->call(method);
        if (reply.is_method_error())
        {
            throw std::runtime_error("Method error on GetSubTree call");
        }

        reply.read(users);
    }
    catch (sdbusplus::exception::SdBusError& e)
    {
        throw std::runtime_error(e.what());
    }
}

/**
 * @brief Enables the user passed in
 *
 * @param[in] path - The user object path
 * @param[in] service - The service hosting the user
 */
void enableUser(const std::string& path, const std::string& service)
{
    auto& bus = ipmid_get_sdbus_plus_handler();

    try
    {
        auto method = bus->new_method_call(service.c_str(),
                path.c_str(),
                PROP_IFACE,
                "Set");
        sdbusplus::message::variant<bool> enabled{true};
        method.append(USER_IFACE, "UserEnabled", enabled);

        auto reply = bus->call(method);
        if (reply.is_method_error())
        {
            throw std::runtime_error("Method error on property set call");
        }
    }
    catch (sdbusplus::exception::SdBusError& e)
    {
        throw std::runtime_error(e.what());
    }
}

ipmi_ret_t enableUsers()
{
    ObjectTree users;

    try
    {
        getUsers(users);

        for (const auto& user : users)
        {
            enableUser(user.first, user.second.begin()->first);
        }
    }
    catch (std::runtime_error& e)
    {
        log<level::ERR>("Failed enabling local users",
                entry("ERROR=%s", e.what()));
        return IPMI_CC_UNSPECIFIED_ERROR;
    }

    return IPMI_CC_OK;
}

}
}
