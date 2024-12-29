parse_system_benchmarks_source_files = [
    'parser.cpp',
    antlr4_source_files,
    polyhedral_system_source_files,
    ppl_utils_source_files
]

parse_system_benchmarks_exe = executable(
    'parser_benchmarks',
    parse_system_benchmarks_source_files,
    dependencies : [catch2_dep, antlr4_runtime_dep, ppl_dep, gmp_dep, gmpxx_dep],
    include_directories : include_directories
)

benchmark('parser-benchmarks', parse_system_benchmarks_exe, suite: ['parser'], workdir: meson.current_source_dir())