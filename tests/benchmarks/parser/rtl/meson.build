rtl_parser_benchmarks_exe = executable(
    'rtl_parser_benchmarks',
    'rtl_parser_benchmarks.cpp',
    link_with: [test_rtl_mc_static_library], dependencies: [spdlog_dep, fmt_dep, catch2_dep, catch2_with_main_dep],
    include_directories : include_directories
)

# Copy benchmark files into build directory
configure_file(input: 'rtl-parser-benchmark-1.txt', output: 'rtl-parser-benchmark-1.txt', copy: true)

benchmark('rtl-parser', rtl_parser_benchmarks_exe, workdir: meson.current_source_dir())