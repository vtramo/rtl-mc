parse_rtlf_tests_source_files = [
    'parse_rtlf_tests.cpp',
    rtlf_parser_source_files,
    spot_utils_source_files,
]

parse_rtlf_tests_exe = executable(
    'parse_rtlf_tests',
    parse_rtlf_tests_source_files,
    dependencies : [catch2_dep, catch2_with_main_dep, spot_dep],
    include_directories : include_directories
)

# Copy test files into build directory
configure_file(input: 'rtlf-test-1.txt', output: 'rtlf-test-1.txt', copy: true)

test('rtlf-parser', parse_rtlf_tests_exe, suite: ['rtlf'], workdir: meson.current_source_dir())
