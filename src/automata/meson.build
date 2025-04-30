current_source_dir = meson.current_source_dir()

automaton_source_files = [
    current_source_dir / 'Automaton.h',
    current_source_dir / 'AutomatonAcceptance.h',
    current_source_dir / 'Automaton.cpp',
    current_source_dir / 'TranslationOptimization.h',
    current_source_dir / 'PolyhedralLtlAutomaton.h',
    current_source_dir / 'PolyhedralLtlAutomaton.cpp',
    current_source_dir / 'PolyhedralSystemFormulaDenotationMap.h',
    current_source_dir / 'PolyhedralSystemFormulaDenotationMap.cpp',
    current_source_dir / 'StateDenotation.h',
    current_source_dir / 'automata_builder.h',
]

subdir('finite')
subdir('büchi')

project_source_files += automaton_source_files