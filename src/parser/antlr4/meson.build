current_source_dir = meson.current_source_dir()
generated_dir = current_source_dir / 'generated'

if get_option('generate_parser')
    message('Generating ANTLR4 PolyhedralSystem parser... ')
    is_windows = host_machine.system() == 'windows'
    generate_script = is_windows ? 'generate-parser.bat' : 'generate-parser.sh'
    run_command(generate_script, check: true)
    message('Parser generation complete! (Location: ' + generated_dir + '/)')
endif

antlr4_source_files = [
    generated_dir / 'PolyhedralSystemBaseListener.cpp',
    generated_dir / 'PolyhedralSystemBaseListener.h',
    generated_dir / 'PolyhedralSystemBaseVisitor.cpp',
    generated_dir / 'PolyhedralSystemBaseVisitor.h',
    generated_dir / 'PolyhedralSystemLexer.cpp',
    generated_dir / 'PolyhedralSystemLexer.h',
    generated_dir / 'PolyhedralSystemListener.cpp',
    generated_dir / 'PolyhedralSystemListener.h',
    generated_dir / 'PolyhedralSystemParser.h',
    generated_dir / 'PolyhedralSystemParser.cpp',
    generated_dir / 'PolyhedralSystemVisitor.h',
    generated_dir / 'PolyhedralSystemVisitor.cpp',
    current_source_dir / 'PolyhedralSystemSymbolTableListener.h',
    current_source_dir / 'PolyhedralSystemSymbolTableListener.cpp',
    current_source_dir / 'PolyhedralSystemBuilderVisitor.h',
    current_source_dir / 'PolyhedralSystemBuilderVisitor.cpp',
]

project_source_files += antlr4_source_files
