#include "config.h"
#include "ldap.hpp"
#include "types.hpp"
#include <host-ipmid/ipmid-host-cmd.hpp>
#include <phosphor-logging/log.hpp>

namespace ldap
{

static constexpr auto ldapCfgPath = "/xyz/openbmc_project/user/ldap/config";
static constexpr auto deleteIface = "xyz.openbmc_project.Object.Delete";
static constexpr auto deleteMethod = "Delete";

using namespace phosphor::logging;
using namespace util;

/**
 * Delete the LDAP configuration object
 *
 * @return ipmi_ret_t - IPMI_CC_SUCCESS if successful
 */
ipmi_ret_t deleteConfigObject()
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
    }
    catch (sdbusplus::exception::SdBusError& e)
    {
        log<level::ERR>("Failed deleting LDAP configuration",
                entry("ERROR=%s", e.what()));
        return IPMI_CC_UNSPECIFIED_ERROR;
    }

    return IPMI_CC_OK;
}

/**
 * Gets all LDAP users.
 *
 * @param[out] users - filled in with the LDAP user
 *                     object tree information.
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

        method.append("/xyz/openbmc_project/user/ldap", 0,
                      std::vector<std::string>{});

        auto reply = bus->call(method);

        reply.read(users);
    }
    catch (sdbusplus::exception::SdBusError& e)
    {
        // No LDAP users
    }
}

/**
 * Deletes the object path passed in.
 *
 * @param[in] path - the object path
 * @param[in] service - the service that owns the object
 */
void deleteUser(const std::string& path, const std::string& service)
{
    auto& bus = ipmid_get_sdbus_plus_handler();

    auto method = bus->new_method_call(service.c_str(),
                                       path.c_str(),
                                       deleteIface,
                                       deleteMethod);
    auto reply = bus->call(method);
}

/**
 *  Deletes all LDAP users, which are the D-Bus objects under
 *  /xyz/openbmc_project/user/ldap/.
 *
 * @return ipmi_ret_t - IPMI_CC_SUCCESS if successful
 */
ipmi_ret_t deleteUsers()
{
    ObjectTree users;

    try
    {
        getUsers(users);

        for (const auto& user : users)
        {
            deleteUser(user.first, user.second.begin()->first);
        }
    }
    catch (std::exception& e)
    {
        log<level::ERR>("Failed deleting LDAP users",
                         entry("ERROR=%s", e.what()));
        return IPMI_CC_UNSPECIFIED_ERROR;
    }

    return IPMI_CC_OK;
}

// See header file for prologue
ipmi_ret_t deleteConfig()
{
    ipmi_ret_t finalRC = IPMI_CC_OK;

    auto rc = deleteConfigObject();
    if (rc != IPMI_CC_OK)
    {
        finalRC = rc;
    }

    rc = deleteUsers();
    if (rc != IPMI_CC_OK)
    {
        finalRC = rc;
    }

    return finalRC;
}

}
