ppl_dep = dependency('ppl', version: '>=14.0.0')
gmp_dep = dependency('gmp', version: '>=6.3.0')
gmpxx_dep = dependency('gmpxx', version: '>=6.3.0')
antlr4_runtime_dep = dependency('antlr4-runtime', version: '>=4.13.0')
spot_dep = dependency('libspot', version: '>=2.12.1')

project_dependencies = [
    ppl_dep,
    gmp_dep,
    gmpxx_dep,
    antlr4_runtime_dep,
    spot_dep
]

project_source_files = ['main.cpp']

subdir('parser')
subdir('system')
subdir('discretization')
subdir('utils')


project_exe = executable(
    meson.project_name(),
    project_source_files,
    dependencies : project_dependencies,
    include_directories : include_directories,
    install : true
)