current_source_dir = meson.current_source_dir()

if get_option('generate_parser')
    system_grammar_relative_path = 'g4' / 'PolyhedralSystem.g4'
    system_parser_generated_dir_relative_path = 'system' / 'generated'
    message('Generating ANTLR4 PolyhedralSystem parser... ')
    run_command('antlr4',
                '-listener',
                '-visitor',
                '-Dlanguage=Cpp',
                '-v',
                '4.13.2',
                system_grammar_relative_path,
                '-o', system_parser_generated_dir_relative_path,  check: true)
    message('PolyhedralSystem parser generation complete! (Location: ' + current_source_dir / system_parser_generated_dir_relative_path + ')')

    mc_point_grammar_relative_path = 'g4' / 'RationalPoint.g4'
    rational_point_parser_generated_dir_relative_path = 'point' / 'generated'
    message('Generating RationalPoint parser... ')
    run_command('antlr4',
                '-listener',
                '-visitor',
                '-Dlanguage=Cpp',
                '-v',
                '4.13.2',
                mc_point_grammar_relative_path,
                '-o', rational_point_parser_generated_dir_relative_path,  check: true)
    message('RationalPoint parser generation complete! (Location: ' + current_source_dir / rational_point_parser_generated_dir_relative_path + ')')
endif

common_parser_source_files = [
    current_source_dir / 'ParserError.h',
    current_source_dir / 'Position.h',
]

antlr4_error_listeners = [
    current_source_dir / 'ErrorListener.h',
    current_source_dir / 'ErrorListener.cpp',
]

subdir('system')
subdir('rtl')
subdir('point')

polyhedral_system_parser_source_files += [common_parser_source_files, antlr4_error_listeners]
rtl_parser_source_files += [common_parser_source_files]
point_parser_source_files += [common_parser_source_files]
project_source_files += common_parser_source_files + antlr4_error_listeners