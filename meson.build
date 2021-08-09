project('obmc-yadro-snmpmgr', 'cpp',
    default_options: [
        'warning_level=3',
        'werror=true',
        'cpp_std=c++17',
    ],
    license: 'Apache-2.0',
    version: '0.1',
)

# CLI11 might not have a pkg-config. It is header only so just make
# sure we can access the needed symbols from the header.
cli11_dep = dependency('CLI11', required: false)
has_cli11 = meson.get_compiler('cpp').has_header_symbol(
    'CLI/CLI.hpp',
    'CLI::App',
    dependencies: cli11_dep,
    required: false)
if not has_cli11
    cli11_proj = subproject('cli11', required: false)
    assert(cli11_proj.found(), 'CLI11 is required')
    cli11_dep = cli11_proj.get_variable('CLI11_dep')
endif

sdbusplus = dependency('sdbusplus', fallback: ['sdbusplus', 'sdbusplus_dep'])

if get_option('remote-host').enabled()
    add_project_arguments('-DREMOTE_HOST', language: 'cpp')
endif

executable(
    'yadro-snmpmgr',
    [
        'src/main.cpp',
        'src/dbus.cpp',
    ],
    dependencies: [
        cli11_dep,
        sdbusplus,
    ],
    install: true,
    install_dir: get_option('sbindir'),
)
