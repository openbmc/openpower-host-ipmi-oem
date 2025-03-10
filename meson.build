project(
    'openpower-host-ipmi-oem',
    'cpp',
    default_options: ['warning_level=3', 'werror=true', 'cpp_std=c++23'],
    meson_version: '>=1.1.1',
    license: 'Apache-2.0',
    version: '0.1',
)
cpp = meson.get_compiler('cpp')

conf = configuration_data()
conf.set_quoted('HOST_NAME', get_option('host-name'))
conf.set_quoted('CONTROL_HOST_OBJ_MGR', get_option('control-host-obj-mgr'))

configure_file(output: 'config.h', configuration: conf)

sdbusplus = dependency('sdbusplus')
phosphorlogging = dependency('phosphor-logging')
phosphordbusinterfaces = dependency('phosphor-dbus-interfaces')
ipmid = dependency('libipmid')

root = meson.current_source_dir()

# Generate sdbus++ code (occmetrics)
subdir('org/open_power/OCC/')

lilboemhandler_sources = files (
    'oemhandler.cpp',
    'host-interface.cpp',
    'org/open_power/OCC/error.cpp',
    'inventory-sensor.cpp'
)

lilboemhandler = shared_module(
    'openpower-host-ipmi-oem',
    lilboemhandler_sources,
    gnu_symbol_visibility: 'hidden',
    install: true,
    dependencies: [
        sdbusplus,
        phosphorlogging,
        phosphordbusinterfaces,
        ipmid,
        occmetrics_dbus,
    ],
)

