current_source_dir = meson.current_source_dir()
generated_dir = current_source_dir / 'generated' / 'g4'

include_directories += include_directories('generated' / 'g4')

mc_point_parser_source_files = [
    generated_dir / 'McPointBaseListener.cpp',
    generated_dir / 'McPointBaseListener.h',
    generated_dir / 'McPointBaseVisitor.h',
    generated_dir / 'McPointBaseVisitor.cpp',
    generated_dir / 'McPointLexer.h',
    generated_dir / 'McPointLexer.cpp',
    generated_dir / 'McPointListener.h',
    generated_dir / 'McPointListener.cpp',
    generated_dir / 'McPointParser.h',
    generated_dir / 'McPointParser.cpp',
    generated_dir / 'McPointVisitor.h',
    generated_dir / 'McPointVisitor.cpp',
    current_source_dir / 'McPointBuilderVisitor.h',
    current_source_dir / 'McPointBuilderVisitor.cpp',
    current_source_dir / 'mcparser.h',
    current_source_dir / 'mcparser.cpp'
]
project_source_files += mc_point_parser_source_files