parse_system_tests_source_files = [
    'parse_system_tests.cpp',
    polyhedral_system_parser_source_files,
    polyhedral_system_source_files,
    ppl_utils_source_files,
    spot_utils_source_files
]

parse_system_tests_exe = executable(
    'parse_system_tests',
    parse_system_tests_source_files,
    dependencies : [catch2_dep, spot_dep, antlr4_runtime_dep, ppl_dep, gmp_dep, gmpxx_dep],
    include_directories : include_directories
)

# Copy test files into build directory
configure_file(input: 'polyhedral-system-test-1.txt', output: 'polyhedral-system-test-1.txt', copy: true)

test('system-parser', parse_system_tests_exe, suite: ['parser'], workdir: meson.current_source_dir())
