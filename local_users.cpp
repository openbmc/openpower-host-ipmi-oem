#include "config.h"
#include "local_users.hpp"
#include <experimental/filesystem>
#include <fstream>
#include <host-ipmid/ipmid-host-cmd.hpp>
#include <phosphor-logging/log.hpp>
#include <security/pam_misc.h>
#include <security/pam_appl.h>
#include <iostream>

namespace local
{
namespace users
{

using namespace phosphor::logging;
namespace fs = std::experimental::filesystem;

constexpr auto USER_IFACE = "xyz.openbmc_project.User.Attributes";
constexpr auto PROP_IFACE = "org.freedesktop.DBus.Properties";

using DbusObjectPath = std::string;
using DbusService = std::string;
using DbusInterface = std::string;
using ObjectTree = std::map<DbusObjectPath,
                            std::map<DbusService, std::vector<DbusInterface>>>;

static constexpr auto pamService = "resetrootpw";
static constexpr auto pamConfPath = "/etc/pam.d/";
static constexpr auto pamConfContents =
R"(password [success=1 default=bad] pam_unix.so md5
password required pam_deny.so
password required pam_permit.so
)";

pam_response* respData = NULL;

struct Password
{
    const char* newPassword;
};

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

/**
 * Creates the PAM config file in /etc/pam.d/
 *
 * @return int - 0 if successful, -1 else
 *
 */
int writePAMConfFile()
{
    std::ofstream file;
    fs::path path = fs::path(pamConfPath) / pamService;

    file.exceptions(std::ofstream::failbit | std::ofstream::badbit |
                    std::ofstream::eofbit);
    try
    {
        file.open(path);
        file << pamConfContents;
        file.close();
    }
    catch (std::exception& e)
    {
        log<level::ERR>("Failed writing pam config file",
                        entry("FILE=%s", path.c_str()),
                        entry("ERROR=%s", e.what()));
        return -1;
    }

    return 0;
}

/**
 *  Removes the PAM config file in /etc/pam.d/
 */
void removePAMConfFile()
{
    fs::remove(fs::path(pamConfPath) / pamService);
}

/**
 * PAM conversation function for changing the password.
 * This is called by the PAM modules.
 *
 * @param[in] nMsg - number of messages in next argument
 * @param[in] msg - array of pam_message pointers
 * @param[out] resp - array of response objects
 * @param[in] appPtr - application data pointer
 *
 * @return PAM_SUCCESS
 */
int changePassword(int nMsg, const pam_message** msg,
                   pam_response** resp, void* appPtr)
{
    auto appData = static_cast<Password*>(appPtr);
    respData = static_cast<pam_response*>(calloc(nMsg, sizeof(pam_response)));

    memset(respData, 0, sizeof(pam_response) * nMsg);
    *resp = respData;

    for (int i = 0; i < nMsg; i++)
    {
        if (msg[0]->msg_style == PAM_PROMPT_ECHO_OFF)
        {
            respData[i].resp = strdup(appData->newPassword);
        }
    }
    respData->resp_retcode = 0;

    return PAM_SUCCESS;
}

ipmi_ret_t resetRootPassword()
{
    ipmi_ret_t ipmiRC = IPMI_CC_OK;
    pam_conv conv;
    pam_handle_t* pamh = nullptr;
    Password appData;
    int rc;

    appData.newPassword = DEFAULT_PW;

    conv.appdata_ptr = static_cast<void*>(&appData);
    conv.conv = changePassword;

    try
    {
        rc = writePAMConfFile();
        if (rc != 0)
        {
            throw std::exception();
        }

        rc = pam_start(pamService, "root", &conv, &pamh);
        if (rc != PAM_SUCCESS)
        {
            log<level::ERR>("pam_start failed",
                    entry("RC=%d\n", rc));
            throw std::exception();
        }

        rc = pam_chauthtok(pamh, 0);
        if (rc != PAM_SUCCESS)
        {
            log<level::ERR>("pam_chauthtok failed",
                    entry("RC=%d\n", rc));
            throw std::exception();
        }
    }
    catch (std::exception& e)
    {
        ipmiRC = IPMI_CC_UNSPECIFIED_ERROR;
    }

    removePAMConfFile();

    if (pamh)
    {
        rc = pam_end(pamh, PAM_SUCCESS);
        if (rc != PAM_SUCCESS)
        {
            log<level::INFO>("pam_end failed",
                    entry("RC=%d", rc));
        }
    }

    return ipmiRC;
}

}
}
