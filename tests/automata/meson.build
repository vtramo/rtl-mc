subdir('onthefly')

formula_denotation_map_tests_exe = executable(
    'formula_denotation_map_tests',
    'polyhedral_system_formula_denotation_map_tests.cpp',
    link_with: [test_rtl_mc_static_library],
    dependencies: test_dependencies,
    include_directories : include_directories
)

test('formula-denotation-map', formula_denotation_map_tests_exe, workdir: meson.current_source_dir())