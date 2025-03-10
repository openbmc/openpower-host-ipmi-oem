sdbuspp_prog = find_program('sdbus++')

domain = 'org.open_power.OCC.Metrics'
md_yaml_file = files('./Metrics.metadata.yaml')

md_cpp = custom_target(
    'error.cpp',
    output: 'error.cpp',
    input: md_yaml_file,
    capture: true,
    command: [sdbuspp_prog, '-r', root, 'error', 'exception-header', domain],
)

md_common_hpp = custom_target(
    'common.hpp',
    output: 'common.hpp',
    input: md_yaml_file,
    capture: true,
    command: [sdbuspp_prog, '-r', root, 'interface', 'common-header', domain],
    install: true,
    install_dir: get_option('includedir') / 'org/open_power/OCC/Metrics',
)

occmetrics_dbus_deps = [
    dependency('phosphor-dbus-interfaces'),
    dependency('sdbusplus'),
]

occmetrics_dbus = declare_dependency(
    dependencies: occmetrics_dbus_deps,
    sources: [md_cpp, md_common_hpp],
)