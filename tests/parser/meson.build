parse_system_tests_source_files = [
    'parse_system_tests.cpp',
    antlr4_source_files,
    polyhedral_system_source_files,
    ppl_utils_source_files
]

install_data(
    sources: [meson.current_source_dir() / 'polyhedral-system-test-1.txt'],
    install_dir: meson.project_build_root() / 'tests' / 'parser',
    install_tag: 'tests'
)
install_data(
    sources: [meson.current_source_dir() / 'polyhedral-system-test-1.txt'],
    install_dir: meson.project_build_root(),
    install_tag: 'tests'
)

parse_system_tests_exe = executable(
    'parse_system_tests',
    parse_system_tests_source_files,
    dependencies : [catch2_dep, antlr4_runtime_dep, ppl_dep, gmp_dep, gmpxx_dep],
    include_directories : include_directories
)

test('parser', parse_system_tests_exe)
