ppl_dep = dependency('ppl', version: '==1.2')
gmp_dep = dependency('gmp', version: '>=6.3.0')
gmpxx_dep = dependency('gmpxx', version: '>=6.3.0')
antlr4_runtime_dep = dependency('antlr4-runtime', version: '>=4.13.0')
spot_dep = dependency('libspot', version: '>=2.12.1')
argparse_dep = dependency('argparse', fallback : ['argparse'], version: '>=3.1')
spdlog = dependency('spdlog', fallback : ['spdlog'], version: '>=1.15.0')

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
    spdlog
]

project_source_files = ['main.cpp']

subdir('abstractions')
subdir('product')
subdir('parser')
subdir('system')
subdir('discretisation')
subdir('operators')
subdir('utils')
subdir('automaton')
subdir('mcsolvers')
subdir('cli')
subdir('logger')
subdir('stats')

link_args = []
if get_option('enable_profiling') == true
    link_args += ['-pg']
endif

project_exe = executable(
    meson.project_name(),
    project_source_files,
    dependencies : project_dependencies,
    include_directories : include_directories,
    install : true,
    link_args : link_args
)