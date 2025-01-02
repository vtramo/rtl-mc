parse_system_benchmarks_source_files = [
    'system_parser_benchmarks.cpp',
    test_utils_src_files,
    polyhedral_system_parser_source_files,
    polyhedral_system_source_files,
    ppl_utils_source_files
]

parse_system_benchmarks_exe = executable(
    'system_parser_benchmarks',
    parse_system_benchmarks_source_files,
    dependencies : [catch2_dep, antlr4_runtime_dep, ppl_dep, gmp_dep, gmpxx_dep],
    include_directories : include_directories
)

# Copy benchmark files into build directory
configure_file(input: 'polyhedral-system-parser-benchmark-1.txt', output: 'polyhedral-system-parser-benchmark-1.txt', copy: true)

benchmark('system-parser', parse_system_benchmarks_exe, suite: ['parser'], workdir: meson.current_source_dir())