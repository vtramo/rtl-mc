discretisation_tests_exe = executable(
    'discretisation',
    'discretisation_tests.cpp',
    link_with: [test_rtl_mc_static_library],
    include_directories : include_directories
)

# Copy benchmark files into build directory
configure_file(input: 'discretisation-test-1.txt', output: 'discretisation-test-1.txt', copy: true)

test('discretisation', discretisation_tests_exe, workdir: meson.current_source_dir())