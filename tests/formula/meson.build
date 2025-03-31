is_non_recurrent_tests_exe = executable(
    'is_non_recurrent_tests',
    'is_non_recurrent_tests.cpp',
    link_with: [test_rtl_mc_static_library],
	dependencies: test_dependencies,
    include_directories : include_directories
)

remove_sing_tests_exe = executable(
    'remove_sing_tests',
    'remove_sing_tests.cpp',
    link_with: [test_rtl_mc_static_library],
    dependencies: test_dependencies,
    include_directories : include_directories
)

test('remove-sing', remove_sing_tests_exe, workdir: meson.current_source_dir())
test('is-non-recurrent', is_non_recurrent_tests_exe, workdir: meson.current_source_dir())
