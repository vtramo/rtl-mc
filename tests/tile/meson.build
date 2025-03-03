tilextractor_tests_src_files = [
    'tilextractor_tests.cpp',
    omnidirectional_abstraction_source_files,
    polyhedral_system_source_files,
    polyhedral_system_parser_source_files,
    spot_utils_source_files,
    ppl_utils_source_files,
]

tilextractor_tests_exe = executable(
    'tilextractor_tests',
    tilextractor_tests_src_files,
    dependencies : [ppl_dep, gmp_dep, gmpxx_dep, spot_dep, catch2_dep, catch2_with_main_dep, antlr4_runtime_dep],
    include_directories : include_directories
)

test('tile', tilextractor_tests_exe, suite: ['tile-extractors'], workdir: meson.current_source_dir())