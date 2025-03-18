tilextractor_tests_exe = executable(
    'tilextractor_tests',
    'tilextractor_tests.cpp',
    link_with: [test_rtl_mc_static_library],
	dependencies: test_dependencies,
    include_directories : include_directories
)

test('tilextractor', tilextractor_tests_exe, workdir: meson.current_source_dir())