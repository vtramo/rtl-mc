denot_benchmarks_source_files = [
    discretization_source_files,
    core_source_files,
    automaton_source_files,
    polyhedral_system_parser_source_files,
    polyhedral_system_source_files,
    test_utils_src_files,
    'denot_benchmarks.cpp',
    spot_utils_source_files,
    ppl_utils_source_files,
]

denot_benchmarks_exe = executable(
    'denot_benchmarks',
    denot_benchmarks_source_files,
    dependencies : [catch2_dep, spot_dep, ppl_dep, gmp_dep, gmpxx_dep, antlr4_runtime_dep, spdlog],
    include_directories : include_directories
)


benchmark('denot', denot_benchmarks_exe, suite: ['denot'], workdir: meson.current_source_dir())