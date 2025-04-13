current_source_dir = meson.current_source_dir()

if get_option('generate_parser')
    polyhedral_grammar_relative_path = 'g4' / 'Polyhedra.g4'
    polyhedral_generated_parser_source_files = 'generated'
    message('Generating ANTLR4 Geogebra Polyhedra parser... ')
    run_command('antlr4',
                '-listener',
                '-visitor',
                '-Dlanguage=Cpp',
                '-v',
                '4.13.2',
                polyhedral_grammar_relative_path,
                '-o', polyhedral_generated_parser_source_files,  check: true)
    message('Parser generation complete! (Location: ' + current_source_dir / polyhedral_generated_parser_source_files + ')')
endif

generated_dir = current_source_dir / 'generated' / 'g4'
poly_ggb_project_source_files += [
    generated_dir / 'PolyhedraBaseListener.cpp',
    generated_dir / 'PolyhedraBaseListener.h',
    generated_dir / 'PolyhedraBaseVisitor.cpp',
    generated_dir / 'PolyhedraBaseListener.h',
    generated_dir / 'PolyhedraLexer.cpp',
    generated_dir / 'PolyhedraLexer.h',
    generated_dir / 'PolyhedraListener.cpp',
    generated_dir / 'PolyhedraListener.h',
    generated_dir / 'PolyhedraParser.cpp',
    generated_dir / 'PolyhedraParser.h',
    generated_dir / 'PolyhedraVisitor.cpp',
    generated_dir / 'PolyhedraVisitor.h',

    current_source_dir / 'SymbolTableListener.h',
    current_source_dir / 'SymbolTableListener.cpp',
    current_source_dir / 'GeogebraPatchGenerator.h',
    current_source_dir / 'GeogebraPatchGenerator.cpp',
    current_source_dir / 'geogebra_polyhedra_parser.h',
]