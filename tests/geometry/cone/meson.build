cone_tests_exe = executable(
    'cone_tests',
    'cone_tests.cpp',
    link_with: [test_rtl_mc_static_library],
	dependencies: test_dependencies,
    include_directories : include_directories
)

test('cone', cone_tests_exe, workdir: meson.current_source_dir())
