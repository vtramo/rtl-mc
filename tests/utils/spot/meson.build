spot_utils_tests_exe = executable(
    'spot_utils_tests',
    'spot_utils_tests.cpp',
    link_with: [test_rtl_mc_static_library],
	dependencies: test_dependencies,
    include_directories : include_directories
)

test('spot-utils', spot_utils_tests_exe, workdir: meson.current_source_dir())
