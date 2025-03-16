observables_system_tests_exe = executable(
    'observables_system_tests',
    'observables_tests.cpp',
    link_with: [test_rtl_mc_static_library],
    include_directories : include_directories
)

test('system-observables', observables_system_tests_exe, workdir: meson.current_source_dir())
