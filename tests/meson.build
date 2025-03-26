current_dir = meson.current_source_dir()
test_utils_src_files = [current_dir / 'test_utils.h']
include_directories += [include_directories('.')]

test_rtl_mc_static_library = static_library(
    'test-rtl-mc-static-library',
    test_utils_src_files,
    link_with: [rtl_mc_static_library],
    include_directories: include_directories
)

catch2_subproject = subproject('catch2', version: '>=3.7.1')
catch2_dep = catch2_subproject.get_variable('catch2_dep')
catch2_with_main_dep = catch2_subproject.get_variable('catch2_with_main_dep')

test_dependencies = [spdlog_dep, fmt_dep, catch2_dep, catch2_with_main_dep]

subdir('automata')
subdir('discretisation')
subdir('benchmarks')
subdir('parser')
subdir('utils')
subdir('geometry')
subdir('denot')
subdir('tile')
subdir('observables')
subdir('operators')