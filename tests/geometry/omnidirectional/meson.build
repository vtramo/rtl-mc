omnidirectional_tests_exe = executable(
    'omnidirectional_tests',
    'omnidirectional_tests.cpp',
    link_with: [test_rtl_mc_static_library], dependencies: [spdlog_dep, fmt_dep, catch2_dep, catch2_with_main_dep],
    include_directories : include_directories
)

test('omnidirectional', omnidirectional_tests_exe, workdir: meson.current_source_dir())
