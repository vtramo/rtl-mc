denot_on_the_fly_exe = executable(
    'denot_on_the_fly',
    'denot_on_the_fly_tests.cpp',
    link_with: [test_rtl_mc_static_library],
    include_directories : include_directories
)

test('denot-on-the-fly', denot_on_the_fly_exe, workdir: meson.current_source_dir())
