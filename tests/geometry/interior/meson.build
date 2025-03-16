interior_tests_exe = executable(
    'interior_tests',
    'interior_tests.cpp',
    link_with: [test_rtl_mc_static_library],
    include_directories : include_directories
)

test('interior', interior_tests_exe, workdir: meson.current_source_dir())
