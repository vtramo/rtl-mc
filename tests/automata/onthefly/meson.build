automaton_tests_exe = executable(
    'automaton_on_the_fly_tests',
    'automaton_on_the_fly_tests.cpp',
    link_with: [test_rtl_mc_static_library],
	dependencies: test_dependencies,
    include_directories : include_directories
)

formula_denotation_map_tests_exe = executable(
    'formula_denotation_map_tests',
    'polyhedral_system_formula_denotation_map_tests.cpp',
    link_with: [test_rtl_mc_static_library],
    dependencies: test_dependencies,
    include_directories : include_directories
)

test('automaton-on-the-fly', automaton_tests_exe, workdir: meson.current_source_dir())
test('formula-denotation-map', formula_denotation_map_tests_exe, workdir: meson.current_source_dir())