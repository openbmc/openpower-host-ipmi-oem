project(
    'openpower-host-ipmi-oem',
    'cpp',
    default_options: [
        'warning_level=3',
        'werror=true',
        'cpp_std=c++23',
        'buildtype=debugoptimized',
    ],
    meson_version: '>=1.1.1',
    license: 'Apache-2.0',
    version: '0.1',
)

conf = configuration_data()
conf.set_quoted('HOST_NAME', get_option('host-name'))

configure_file(output: 'config.h', configuration: conf)

sdbusplus = dependency('sdbusplus')
phosphorlogging = dependency('phosphor-logging')
phosphordbusinterfaces = dependency('phosphor-dbus-interfaces')
ipmid = dependency('libipmid')

root = meson.current_source_dir()

# Generate sdbus++ code (occmetrics)
subdir('org/open_power/OCC/Metrics')

# Generate inventory sensor files
inventory_gen_src = []
subdir('scripts')

lilboemhandler_sources = [
    'host-interface.cpp',
    'oemhandler.cpp',
    md_cpp,
    inventory_gen_src,
]

lilboemhandler = library(
    'oemhandler',
    lilboemhandler_sources,
    gnu_symbol_visibility: 'hidden',
    install: true,
    install_dir: get_option('libdir') / 'ipmid-providers',
    version: meson.project_version(),
    override_options: ['b_lundef=false'],
    dependencies: [
        sdbusplus,
        phosphorlogging,
        phosphordbusinterfaces,
        ipmid,
        occmetrics_dbus,
    ],
)

