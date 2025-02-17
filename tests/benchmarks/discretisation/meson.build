discretisation_benchmarks_source_files = [
    discretisation_source_files,
    test_utils_src_files,
    'discretisation_benchmarks.cpp',
    spot_utils_source_files,
]

discretisation_benchmarks_exe = executable(
    'discretisation_benchmarks',
    discretisation_benchmarks_source_files,
    dependencies : [catch2_dep, catch2_with_main_dep, spot_dep],
    include_directories : include_directories
)

# Copy benchmark files into build directory
configure_file(input: 'discretisation-benchmark-1.txt', output: 'discretisation-benchmark-1.txt', copy: true)

benchmark('discretisation', discretisation_benchmarks_exe, suite: ['discretisation'], workdir: meson.current_source_dir())