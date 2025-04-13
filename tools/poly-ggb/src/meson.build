poly_ggb_project_source_files = [
    'main.cpp',
    'PolyGgbProgram.h',
    'SymbolTable.h',
    'GeogebraPatch.h',
    'RGBColor.h',
    'RGBColor.cpp',
    'geogebra_xml.h',
    'geogebra_zip.h',
]

tinyxml2_dep = dependency('tinyxml2', fallback : ['tinyxml2'], version: '>=11.0.0')
minizip_ng_dep = dependency('minizip', fallback : ['minizip-ng'], version: '>=4.0.9')

subdir('parser')

link_args = []
if get_option('enable_profiling') == true
    link_args += ['-pg', '-pie']
endif

include_directories += [
    include_directories('.'),
    include_directories('parser'),
    include_directories('parser' / 'generated' / 'g4')
]

poly_ggb_exe = executable(
    'poly-ggb',
    poly_ggb_project_source_files,
    link_whole: [rtl_mc_static_library],
    link_args: link_args,
    dependencies: [argparse_dep, tinyxml2_dep, minizip_ng_dep],
    cpp_args: '-DRTL_MC_VERSION="' + meson.project_version() + '"',
    include_directories : include_directories,
    install : true
)