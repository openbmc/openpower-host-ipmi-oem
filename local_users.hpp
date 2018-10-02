#pragma once

#include <host-ipmid/ipmid-api.h>

namespace local
{
namespace users
{

/**
 * @brief Enable all local BMC users
 *
 * Sets the UserEnabled property on all
 * /xyz/openbmc_project/user/<user> objects.
 *
 * @return ipmi_ret_t - IPMI CC
 */
ipmi_ret_t enableUsers();


/**
 * Resets the root password back to the default one
 * using PAM.
 *
 * @return ipmi_ret_t - IPMI_CC_SUCCESS if successful
 */
ipmi_ret_t resetRootPassword();

}
}
