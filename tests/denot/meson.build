denot_on_the_fly_src_files = [
    'denot_on_the_fly_tests.cpp',
    denot_on_the_fly_source_files,
    automaton_source_files,
    polyhedral_system_source_files,
    polyhedral_system_parser_source_files,
    spot_utils_source_files,
    ppl_utils_source_files,
    operators_source_files,
    discretisation_source_files
]

denot_on_the_fly_exe = executable(
    'denot_on_the_fly',
    denot_on_the_fly_src_files,
    dependencies : [spot_dep, catch2_dep, catch2_with_main_dep, ppl_dep, gmp_dep, antlr4_runtime_dep, spdlog],
    include_directories : include_directories
)

test('denot-on-the-fly', denot_on_the_fly_exe, suite: ['denot-on-the-fly'], workdir: meson.current_source_dir())
