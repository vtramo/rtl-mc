ppl_utils_tests_exe = executable(
    'ppl-utils',
    'ppl_utils_tests.cpp',
    link_with: [test_rtl_mc_static_library],
    include_directories : include_directories
)

test('ppl-utils', ppl_utils_tests_exe, workdir: meson.current_source_dir())
