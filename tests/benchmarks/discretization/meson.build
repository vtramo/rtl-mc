discretization_benchmarks_source_files = [
    discretization_source_files,
    test_utils_src_files,
    'discretization_benchmarks.cpp',
    spot_utils_source_files,
]

discretization_benchmarks_exe = executable(
    'discretization_benchmarks',
    discretization_benchmarks_source_files,
    dependencies : [catch2_dep, spot_dep],
    include_directories : include_directories
)

# Copy benchmark files into build directory
configure_file(input: 'discretization-benchmark-1.txt', output: 'discretization-benchmark-1.txt', copy: true)

benchmark('discretization', discretization_benchmarks_exe, suite: ['discretization'], workdir: meson.current_source_dir())