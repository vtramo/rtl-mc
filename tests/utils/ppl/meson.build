ppl_utils_tests_source_files = [
    'ppl_utils_tests.cpp',
    ppl_utils_source_files,
    operators_source_files,
    polyhedral_system_source_files,
    spot_utils_source_files,
    polyhedral_system_parser_source_files
]

ppl_utils_tests_exe = executable(
    'ppl_utils_tests',
    ppl_utils_tests_source_files,
    dependencies : [catch2_dep, catch2_with_main_dep, ppl_dep, gmp_dep, gmpxx_dep, spot_dep, antlr4_runtime_dep],
    include_directories : include_directories
)

test('ppl-utils', ppl_utils_tests_exe, suite: ['ppl-utils'], workdir: meson.current_source_dir())
