denot_on_the_fly_benchmarks_source_files = [
    discretisation_source_files,
    core_source_files,
    automaton_source_files,
    polyhedral_system_parser_source_files,
    polyhedral_system_source_files,
    test_utils_src_files,
    'denot_on_the_fly_benchmarks.cpp',
    spot_utils_source_files,
    ppl_utils_source_files,
    operators_source_files,
]

denot_on_the_fly_benchmarks_exe = executable(
    'denot_on_the_fly_benchmarks',
    denot_on_the_fly_benchmarks_source_files,
    dependencies : [catch2_dep, catch2_with_main_dep, spot_dep, ppl_dep, gmp_dep, gmpxx_dep, antlr4_runtime_dep, spdlog],
    include_directories : include_directories
)


benchmark('denot-on-the-fly', denot_on_the_fly_benchmarks_exe, suite: ['denot-on-the-fly'], workdir: meson.current_source_dir())