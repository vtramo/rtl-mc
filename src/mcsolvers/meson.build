current_source_dir = meson.current_source_dir()

mc_solvers_source_files = [
    current_source_dir / 'automata_builder.h',
    current_source_dir / 'brink_stay_atoms.h',
    current_source_dir / 'infinite_semantics_emptiness.h',
    current_source_dir / 'Solver.h',
    current_source_dir / 'FiniteOnTheFlySolver.h',
    current_source_dir / 'ExplicitSolver.h',
    current_source_dir / 'OmnidirectionalSolver.h',
    current_source_dir / 'OmnidirectionalFiniteSolver.h',
    current_source_dir / 'FiniteSemanticsDfs.h',
    current_source_dir / 'GeneralSolver.h',
    current_source_dir / 'GeneralFiniteSolver.h',
    current_source_dir / 'GeneralInfiniteSolver.h',
]

subdir('onthefly')

project_source_files += mc_solvers_source_files