ppl_benchmarks_exe = executable(
    'ppl_benchmarks',
    'ppl_benchmarks.cpp',
    link_with: [test_rtl_mc_static_library],
	dependencies: test_dependencies,
    include_directories : include_directories
)

benchmark('ppl', ppl_benchmarks_exe, workdir: meson.current_source_dir())