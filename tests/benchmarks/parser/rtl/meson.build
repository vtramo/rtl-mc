rtl_parser_benchmarks_exe = executable(
    'rtl_parser_benchmarks',
    'rtl_parser_benchmarks.cpp',
    link_with: [test_rtl_mc_static_library],
	dependencies: test_dependencies,
    include_directories : include_directories
)

# Copy benchmark files into build directory
configure_file(input: 'rtl-parser-benchmark-1.txt', output: 'rtl-parser-benchmark-1.txt', copy: true)

benchmark('rtl-parser', rtl_parser_benchmarks_exe, workdir: meson.current_source_dir())