denot_on_the_fly_benchmarks_exe = executable(
    'denot_on_the_fly_benchmarks',
    'denot_on_the_fly_benchmarks.cpp',
    link_with: [test_rtl_mc_static_library], dependencies: [spdlog_dep, fmt_dep, catch2_dep, catch2_with_main_dep],
    include_directories : include_directories
)

benchmark('denot-on-the-fly', denot_on_the_fly_benchmarks_exe, workdir: meson.current_source_dir())