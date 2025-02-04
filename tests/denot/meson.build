denot_tests_src_files = [
    'denot_tests.cpp',
    'denot_memoization_tests.cpp',
    core_source_files,
    automaton_source_files,
    polyhedral_system_source_files,
    polyhedral_system_parser_source_files,
    spot_utils_source_files,
    ppl_utils_source_files,
    discretization_source_files
]

denot_tests_exe = executable(
    'denot_tests',
    denot_tests_src_files,
    dependencies : [spot_dep, catch2_dep, catch2_with_main_dep, ppl_dep, gmp_dep, antlr4_runtime_dep, spdlog],
    include_directories : include_directories
)

test('denot', denot_tests_exe, suite: ['denot'], workdir: meson.current_source_dir())