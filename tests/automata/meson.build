automaton_tests_exe = executable(
    'automaton_tests',
    'on_the_fly_tests.cpp',
    link_with: [test_rtl_mc_static_library],
    include_directories : include_directories
)

test('automata', automaton_tests_exe, workdir: meson.current_source_dir())