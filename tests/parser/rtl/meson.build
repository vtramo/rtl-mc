parse_rtl_tests_exe = executable(
    'parse_rtl_tests',
    'parse_rtl_tests.cpp',
    link_with: [test_rtl_mc_static_library], dependencies: [spdlog_dep, fmt_dep, catch2_dep, catch2_with_main_dep],
    include_directories : include_directories
)

# Copy test files into build directory
configure_file(input: 'rtl-test-1.txt', output: 'rtl-test-1.txt', copy: true)

test('rtl-parser', parse_rtl_tests_exe, workdir: meson.current_source_dir())
