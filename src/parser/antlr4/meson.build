current_source_dir = meson.current_source_dir()
generated_dir = current_source_dir / 'generated' / 'g4'

if get_option('generate_parser')
    grammar_relative_path = 'g4' / 'PolyhedralSystem.g4'
    generated_dir_relative_path = 'generated'

    message('Generating ANTLR4 PolyhedralSystem parser... ')
    run_command('antlr4',
                '-listener',
                '-visitor',
                '-Dlanguage=Cpp',
                grammar_relative_path,
                '-o', generated_dir_relative_path,  check: true)
    message('Parser generation complete! (Location: ' + generated_dir + ')')
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
