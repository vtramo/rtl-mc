discretisation_benchmarks_exe = executable(
    'discretisation_benchmarks',
    'discretisation_benchmarks.cpp',
    link_with: [test_rtl_mc_static_library],
	dependencies: test_dependencies,
    include_directories : include_directories
)

# Copy benchmark files into build directory
configure_file(input: 'discretisation-benchmark-1.txt', output: 'discretisation-benchmark-1.txt', copy: true)

benchmark('discretisation', discretisation_benchmarks_exe, workdir: meson.current_source_dir())