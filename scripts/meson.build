# Generate Configuration Files from Yaml
python_exe = find_program('python3', 'python')

invsensor_gen = custom_target(
    'invsensor-gen',
    output: 'inventory-sensor.cpp',
    input: ['inventory-sensor.py', get_option('invsensor-yaml-gen')],
    command: [
        python_exe,
        '@INPUT0@',
        '-i',
        '@INPUT1@',
        '-o',
        meson.current_build_dir(),
        'generate-cpp',
    ],
)
inventory_gen_src = invsensor_gen
