omnidirectional_tests_exe = executable(
    'omnidirectional_tests',
    'omnidirectional_tests.cpp',
    link_with: [test_rtl_mc_static_library],
    include_directories : include_directories
)

test('omnidirectional', omnidirectional_tests_exe, workdir: meson.current_source_dir())
