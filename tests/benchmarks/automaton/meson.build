automaton_benchmarks_source_files = [
    discretization_source_files,
    automaton_source_files,
    polyhedral_system_parser_source_files,
    polyhedral_system_source_files,
    test_utils_src_files,
    'automaton_benchmarks.cpp',
    spot_utils_source_files,
    ppl_utils_source_files,
]

automaton_benchmarks_exe = executable(
    'automaton_benchmarks',
    automaton_benchmarks_source_files,
    dependencies : [catch2_dep, spot_dep, ppl_dep, gmp_dep, gmpxx_dep, antlr4_runtime_dep, spdlog],
    include_directories : include_directories
)

# Copy benchmark files into build directory
configure_file(input: 'automaton-polyhedral-system-benchmark-1.txt', output: 'automaton-polyhedral-system-benchmark-1.txt', copy: true)
configure_file(input: 'automaton-formula-benchmark-1.txt', output: 'automaton-formula-benchmark-1.txt', copy: true)

benchmark('automaton-construction', automaton_benchmarks_exe, suite: ['automaton'], workdir: meson.current_source_dir())