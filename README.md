This .so file is designed to support the OpenPOWER's BIOS OEM commands.
Documentation can be found by contacting the OpenPOWER mailing list @ https://github.com/open-power/op-build

## To Build
```
To build this package, do the following steps:

    1. ./bootstrap.sh
    2. ./configure ${CONFIGURE_FLAGS}
    3. make

To clean the repository run `./bootstrap.sh clean`.
```

## Supported Commands
- Partial Add
- Prepare for host update
- Reset BMC authentication

## Command Documentation

### Reset BMC authentication
Netfun: 0x3a
Command: 0x11

This command will reset the BMC authentication mechanism back to the defaults.

This includes:
1. Deleting the LDAP configuration and removing any LDAP user accounts.
2. Re-enabling all local users, if they were previously disabled.
3. Resetting the root password back to the default one.

This command is not allowed when the IPMI restriction mode is set to
'Whitelist'.  See [here][1] for more information.

[1]: https://github.com/openbmc/phosphor-dbus-interfaces/blob/master/xyz/openbmc_project/Control/Security/RestrictionMode.interface.yaml
