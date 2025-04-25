current_source_dir = meson.current_source_dir()
generated_dir = current_source_dir / 'generated' / 'g4'

include_directories += include_directories('generated' / 'g4')

point_parser_source_files = [
    generated_dir / 'RationalPointBaseListener.cpp',
    generated_dir / 'RationalPointBaseListener.h',
    generated_dir / 'RationalPointBaseVisitor.h',
    generated_dir / 'RationalPointBaseVisitor.cpp',
    generated_dir / 'RationalPointLexer.h',
    generated_dir / 'RationalPointLexer.cpp',
    generated_dir / 'RationalPointListener.h',
    generated_dir / 'RationalPointListener.cpp',
    generated_dir / 'RationalPointParser.h',
    generated_dir / 'RationalPointParser.cpp',
    generated_dir / 'RationalPointVisitor.h',
    generated_dir / 'RationalPointVisitor.cpp',
    current_source_dir / 'RationalPointBuilderVisitor.h',
    current_source_dir / 'RationalPointBuilderVisitor.cpp',
    current_source_dir / 'RationalPoint.h',
    current_source_dir / 'pointparser.h',
    current_source_dir / 'pointparser.cpp'
]

project_source_files += point_parser_source_files