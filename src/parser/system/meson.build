current_source_dir = meson.current_source_dir()
generated_dir = current_source_dir / 'generated' / 'g4'

include_directories += include_directories('generated' / 'g4')

polyhedral_system_parser_source_files = [
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
    current_source_dir / 'PolyhedralSystemParsingResult.h',
    current_source_dir / 'systemparser.h',
    current_source_dir / 'systemparser.cpp',
]

project_source_files += polyhedral_system_parser_source_files
