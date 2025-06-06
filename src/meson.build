ppl_dep = dependency('ppl', version: '==1.2')
gmp_dep = dependency('gmp', version: '>=6.1.0')
gmpxx_dep = dependency('gmpxx', version: '>=6.1.0')
antlr4_runtime_dep = dependency('antlr4-runtime', version: '>=4.13.0')
spot_dep = dependency('libspot', version: '>=2.12.1')
argparse_dep = dependency('argparse', fallback : ['argparse'], version: '>=3.1')
spdlog_dep = dependency('spdlog', fallback : ['spdlog'], version: '>=1.15.0')
fmt_dep = dependency('fmt', fallback: ['fmt'], version: '>=11.1.0')

add_project_arguments('-DSPOT_WANT_STRONG_X', language: 'cpp')
add_project_arguments('-DPPL_THREAD_SAFE=1', language: 'cpp')

if get_option('debug') == false
    add_project_arguments('-DNDEBUG', language: 'cpp')
else
    add_project_arguments('-DDEBUG', language: 'cpp')
endif

project_dependencies = [
    ppl_dep,
    gmp_dep,
    gmpxx_dep,
    antlr4_runtime_dep,
    spot_dep,
    argparse_dep,
    spdlog_dep,
    fmt_dep
]

project_source_files = []

subdir('abstractions')
subdir('product')
subdir('parser')
subdir('system')
subdir('discretisation')
subdir('operators')
subdir('geometry')
subdir('utils')
subdir('automata')
subdir('mcsolvers')
subdir('cli')
subdir('logger')
subdir('stats')
subdir('formula')

rtl_mc_static_library = static_library(
    'rtl-mc',
    project_source_files,
    dependencies: project_dependencies,
    include_directories: include_directories,
    link_args : link_args
)

rtl_mc_exe = executable(
    meson.project_name(),
    'main.cpp',
    cpp_args: '-DRTL_MC_VERSION="' + meson.project_version() + '"',
    link_whole: [rtl_mc_static_library],
    dependencies: [spdlog_dep, fmt_dep, argparse_dep],
    include_directories : include_directories,
    install : true
)