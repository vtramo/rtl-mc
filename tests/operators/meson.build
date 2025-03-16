traverse_operators_system_tests_exe = executable(
    'traverse_operator_tests',
    'traverse_operator_tests.cpp',
    link_with: [test_rtl_mc_static_library],
    include_directories : include_directories
)

test('traverse-operator', traverse_operators_system_tests_exe, workdir: meson.current_source_dir())
