discretization_tests_src_files = [
    'discretization_tests.cpp',
    spot_utils_source_files,
    discretization_source_files
]

discretization_tests_exe = executable(
    'discretization_tests',
    discretization_tests_src_files,
    dependencies : [spot_dep, catch2_dep, catch2_with_main_dep],
    include_directories : include_directories
)

# Copy benchmark files into build directory
configure_file(input: 'discretization-test-1.txt', output: 'discretization-test-1.txt', copy: true)

test('discretization', discretization_tests_exe, suite: ['discretization'], workdir: meson.current_source_dir())