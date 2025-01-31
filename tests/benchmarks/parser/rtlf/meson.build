rtlf_parser_benchmarks_source_files = [
    'rtlf_parser_benchmarks.cpp',
    test_utils_src_files,
    rtlf_parser_source_files,
    spot_utils_source_files,
]

rtlf_parser_benchmarks_exe = executable(
    'rtlf_parser_benchmarks',
    rtlf_parser_benchmarks_source_files,
    dependencies : [catch2_dep, catch2_with_main_dep, spot_dep],
    include_directories : include_directories
)

# Copy benchmark files into build directory
configure_file(input: 'rtlf-parser-benchmark-1.txt', output: 'rtlf-parser-benchmark-1.txt', copy: true)

benchmark('rtlf-parser', rtlf_parser_benchmarks_exe, suite: ['parser'], workdir: meson.current_source_dir())