tilextractor_benchmarks_exe = executable(
    'tilextractor_benchmarks',
    'tilextractor_benchmarks.cpp',
    link_with: [test_rtl_mc_static_library],
	dependencies: test_dependencies,
    include_directories : include_directories
)

benchmark('tilextractor', tilextractor_benchmarks_exe, workdir: meson.current_source_dir())