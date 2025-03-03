traversal_operators_source_files = [
    'traversal_operator_tests.cpp',
    polyhedral_system_source_files,
    polyhedral_system_parser_source_files,
    ppl_utils_source_files,
    operators_source_files,
    spot_utils_source_files
]

traversal_operators_system_tests_exe = executable(
    'traversal_operator_tests',
    traversal_operators_source_files,
    dependencies : [catch2_dep, catch2_with_main_dep, spot_dep, ppl_dep, gmp_dep, antlr4_runtime_dep, gmpxx_dep],
    include_directories : include_directories
)

test('traversal-operator', traversal_operators_system_tests_exe, suite: ['traversal-operator'], workdir: meson.current_source_dir())
