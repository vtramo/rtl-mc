current_dir = meson.current_source_dir()
test_utils_src_files = [current_dir / 'test_utils.h']
include_directories += [include_directories('.')]

test_rtl_mc_static_library = static_library(
    'test-rtl-mc-static-library',
    test_utils_src_files,
    link_with: [rtl_mc_static_library],
    dependencies: [catch2_dep, catch2_with_main_dep],
    include_directories: include_directories
)

subdir('automata')
subdir('discretisation')
subdir('benchmarks')
subdir('parser')
subdir('utils')
subdir('denot')
subdir('tile')
subdir('observables')
subdir('operators')