rtl_gen_project_source_files = [
    'main.cpp',
    'RtlGenProgram.h',
    'FormulaFamilyVisitor.h',
    'AlternationFormulaFamilyParams.h',
]

link_args = []
if get_option('enable_profiling') == true
    link_args += ['-pg', '-pie']
endif

rtl_gen_exe = executable(
    'rtl-gen',
    rtl_gen_project_source_files,
    link_whole: [rtl_mc_static_library],
    link_args: link_args,
    dependencies: [argparse_dep],
    cpp_args: '-DRTL_MC_VERSION="' + meson.project_version() + '"',
    include_directories : include_directories,
    install : true
)