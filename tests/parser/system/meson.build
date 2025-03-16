parse_system_tests_exe = executable(
    'parse_system_tests',
    'parse_system_tests.cpp',
    link_with: [test_rtl_mc_static_library],
    include_directories : include_directories
)

# Copy test files into build directory
configure_file(input: 'polyhedral-system-test-1.txt', output: 'polyhedral-system-test-1.txt', copy: true)

test('system-parser', parse_system_tests_exe, workdir: meson.current_source_dir())