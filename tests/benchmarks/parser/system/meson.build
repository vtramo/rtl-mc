parse_system_benchmarks_exe = executable(
    'system_parser_benchmarks',
    'system_parser_benchmarks.cpp',
    link_with: [test_rtl_mc_static_library], dependencies: [spdlog_dep, fmt_dep, catch2_dep, catch2_with_main_dep],
    include_directories : include_directories
)

# Copy benchmark files into build directory
configure_file(input: 'polyhedral-system-parser-benchmark-1.txt', output: 'polyhedral-system-parser-benchmark-1.txt', copy: true)

benchmark('system-parser', parse_system_benchmarks_exe, workdir: meson.current_source_dir())