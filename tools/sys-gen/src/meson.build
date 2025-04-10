sys_gen_project_source_files = [
    'main.cpp',
    'SysGenProgram.h',
    'ClosedInterval.h',
    'Tank.h',
    'tank_experiment.h',
    'tank_experiment.cpp',
]

link_args = []
if get_option('enable_profiling') == true
    link_args += ['-pg', '-pie']
endif

sys_gen_exe = executable(
    'sys-gen',
    sys_gen_project_source_files,
    link_whole: [rtl_mc_static_library],
    dependencies: [argparse_dep],
    cpp_args: '-DRTL_MC_VERSION="' + meson.project_version() + '"',
    include_directories : include_directories,
    link_args: link_args,
    install : true
)