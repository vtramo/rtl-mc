tilextractor_tests_exe = executable(
    'tilextractor_tests',
    'tilextractor_tests.cpp',
    link_with: [test_rtl_mc_static_library],
    include_directories : include_directories
)

test('tile', tilextractor_tests_exe, workdir: meson.current_source_dir())