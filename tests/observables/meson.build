observables_system_tests_source_files = [
    'observables_tests.cpp',
    polyhedral_system_parser_source_files,
    polyhedral_system_source_files,
    ppl_utils_source_files,
    operators_source_files,
    observable_patch_cartesian_product_source_files,
    spot_utils_source_files
]

observables_system_tests_exe = executable(
    'observables_system_tests',
    observables_system_tests_source_files,
    dependencies : [catch2_dep, catch2_with_main_dep, spot_dep, antlr4_runtime_dep, ppl_dep, gmp_dep, gmpxx_dep],
    include_directories : include_directories
)

test('system-observables', observables_system_tests_exe, suite: ['observables'], workdir: meson.current_source_dir())
