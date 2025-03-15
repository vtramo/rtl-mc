parse_rtl_tests_source_files = [
    'parse_rtl_tests.cpp',
    rtl_parser_source_files,
    spot_utils_source_files,
]

parse_rtl_tests_exe = executable(
    'parse_rtl_tests',
    parse_rtl_tests_source_files,
    dependencies : [catch2_dep, catch2_with_main_dep, spot_dep],
    include_directories : include_directories
)

# Copy test files into build directory
configure_file(input: 'rtl-test-1.txt', output: 'rtl-test-1.txt', copy: true)

test('rtl-parser', parse_rtl_tests_exe, suite: ['rtl'], workdir: meson.current_source_dir())
