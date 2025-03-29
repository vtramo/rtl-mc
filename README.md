# RTL Model Checking
Please refer to the documentation in the [docs directory](https://github.com/vtramo/rtl-mc/tree/main/docs) for more details on the prerequisites and how to build the project.

# Docker
If you have Docker installed...

1. Build the image (it takes a few minutes...)
   ```shell
   docker build -t rtl-mc .
   ```
2. Run the container
   ```shell
   docker run --rm rtl-mc
   ```
   ```
   Usage: rtl-mc [--help] [--version]
   
   Input options:
   --system <@FILE|STRING|-> --formula <@FILE|STRING|->
   
   Semantics options:
   [--semantics VAR] [[--existential]|[--universal]] [--mc VAR]
   
   Automaton construction optimizations:
   [[--low]|[--medium]|[--high]] [--any]
   
   Output format:
   [[--verbose]...|[--quiet]|[--stats VAR]]
   
   Advanced options:
   [--direct-ltl] [--concurrent]
   
   Model Checking Linear Temporal Properties on Polyhedral Systems
   
   Optional arguments:
   -h, --help                  shows help message and exits
   -v, --version               prints version information and exits
   
   Input options (detailed usage):
   --system <@FILE|STRING|->   A polyhedral system can be provided in one of the following formats:
   > @FILE  : File path (must start with '@').
   > STRING : A direct string representation of the polyhedral system.
   > -      : Reads the polyhedral system from stdin. [required]
   --formula <@FILE|STRING|->  An RTL formula can be provided in one of the following formats:
   > @FILE  : File path (must start with '@').
   > STRING : A direct string representation of the formula.
   > -      : Reads the formula from stdin. [required]
   
   Semantics options (detailed usage):
   --semantics                 E.g. --semantics=fin. Possible semantics:
   > fin:   Considers only finite-time trajectories (default).
   Suitable for properties that are positively verified
   as soon as a prefix of the trajectory satisfies them,
   such as reachability properties.
   > inf:   Considers only infinite-time trajectories.
   Suitable for non-terminating properties, such as
   liveness or fairness properties.
   > may:   Considers all trajectories that are either infinite-time,
   or end in a may-exit point, i.e., a point on the boundary
   of the invariant from which at least one admissible
   direction exits.
   > must:  Considers all trajectories that are either infinite-time,
   or end in a must-exit point, i.e., a point on the boundary
   of the invariant from which all admissible directions exit.
   --existential               Compute the set of points from which there exists a trajectory that satisfies φ (default).
   --universal                 Compute the set of points from which every trajectory satisfies φ.
   --mc                        Check if a given point x ∈ ℚⁿ is the source of a trajectory in the polyhedral system
   that satisfies the temporal formula φ.
   For --existential, checks if some trajectory from the point satisfies φ.
   For --universal, checks if all trajectories from the point satisfy φ.
   Specify all system variables with rational values (e.g., [x=1/3, y=-2/3, z=1]).
   
   Automaton construction optimizations (detailed usage):
   --low                       Minimal optimizations during automaton construction (fast, default).
   --medium                    Moderate optimizations during automaton construction.
   --high                      All available optimizations during automaton construction (slow).
   --any                       Tells the translator that it should attempt to
   reduce or produce a deterministic result: any automaton denoting the
   given formula is OK. This effectively disables post-processings and
   speeds up the translation.
   
   Output format (detailed usage):
   -V, --verbose               Enable verbose output. Each occurrence of -V increases verbosity level (e.g. -VVV). [may be repeated]
   -q, --quiet                 Suppress all normal output.
   -s, --stats                 Formats the execution statistics.
   
   Advanced options (detailed usage):
   --direct-ltl                Discretise the RTLf formula directly into LTL in a single step, improving performance (experimental).
   This option is only effective for finite-time semantics.
   -c, --concurrent            Enable concurrent execution (highly experimental). This option is only effective with
   the on-the-fly algorithm for finite semantics.
   ```