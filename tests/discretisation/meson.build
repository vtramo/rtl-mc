discretisation_tests_src_files = [
    'discretisation_tests.cpp',
    spot_utils_source_files,
    discretisation_source_files
]

discretisation_tests_exe = executable(
    'discretisation_tests',
    discretisation_tests_src_files,
    dependencies : [spot_dep, catch2_dep, catch2_with_main_dep],
    include_directories : include_directories
)

# Copy benchmark files into build directory
configure_file(input: 'discretisation-test-1.txt', output: 'discretisation-test-1.txt', copy: true)

test('discretisation', discretisation_tests_exe, suite: ['discretisation'], workdir: meson.current_source_dir())