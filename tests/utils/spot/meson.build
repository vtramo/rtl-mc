spot_utils_tests_source_files = [
    'spot_utils_tests.cpp',
    spot_utils_source_files
]

spot_utils_tests_exe = executable(
    'spot_utils_tests',
    spot_utils_tests_source_files,
    dependencies : [catch2_dep, spot_dep],
    include_directories : include_directories
)

test('spot-utils', spot_utils_tests_exe, suite: ['spot-utils'], workdir: meson.current_source_dir())
