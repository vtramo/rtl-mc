automaton_tests_src_files = [
    'automaton_tests.cpp',
    automaton_source_files,
    polyhedral_system_source_files,
    polyhedral_system_parser_source_files,
    spot_utils_source_files,
    ppl_utils_source_files,
    operators_source_files,
    discretisation_source_files
]

automaton_tests_exe = executable(
    'automaton_tests',
    automaton_tests_src_files,
    dependencies : [spot_dep, catch2_dep, catch2_with_main_dep, ppl_dep, gmp_dep, antlr4_runtime_dep, spdlog],
    include_directories : include_directories
)

test('automaton', automaton_tests_exe, suite: ['automaton'], workdir: meson.current_source_dir())