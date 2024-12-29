parse_system_tests_source_files = [
    'parse_system_tests.cpp',
    antlr4_source_files,
    polyhedral_system_source_files,
    ppl_utils_source_files
]

parse_system_tests_exe = executable(
    'parse_system_tests',
    parse_system_tests_source_files,
    dependencies : [catch2_dep, antlr4_runtime_dep, ppl_dep, gmp_dep, gmpxx_dep],
    include_directories : include_directories
)

test('parser', parse_system_tests_exe, suite: ['parser'], workdir: meson.current_source_dir())
