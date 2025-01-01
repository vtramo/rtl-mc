subdir('system')
subdir('rtlf')

current_source_dir = meson.current_source_dir()
common_parser_source_files = [
    current_source_dir / 'ParserError.h',
    current_source_dir / 'LocationError.h',
]

polyhedral_system_parser_source_files += [common_parser_source_files]
rtlf_parser_source_files += [common_parser_source_files]
project_source_files += common_parser_source_files