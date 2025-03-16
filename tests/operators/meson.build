traversal_operators_system_tests_exe = executable(
    'traversal_operator_tests',
    'traversal_operator_tests.cpp',
    link_with: [test_rtl_mc_static_library],
    include_directories : include_directories
)

test('traversal-operator', traversal_operators_system_tests_exe, workdir: meson.current_source_dir())
