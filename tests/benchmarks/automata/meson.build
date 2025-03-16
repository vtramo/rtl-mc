automata_benchmarks_exe = executable(
    'automata_benchmarks',
    'automata_benchmarks.cpp',
    link_with: [test_rtl_mc_static_library],
    include_directories : include_directories
)

# Copy benchmark files into build directory
configure_file(input: 'automata-polyhedral-system-benchmark-1.txt', output: 'automata-polyhedral-system-benchmark-1.txt', copy: true)
configure_file(input: 'automata-formula-benchmark-1.txt', output: 'automata-formula-benchmark-1.txt', copy: true)

benchmark('automata-construction', automata_benchmarks_exe, workdir: meson.current_source_dir())