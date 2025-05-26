# RTL Model Checking Tool on Polyhedral Systems [![Compile & Test & Push](https://github.com/vtramo/rtl-mc/actions/workflows/compile-test-image.yaml/badge.svg)](https://github.com/vtramo/rtl-mc/actions/workflows/compile-test-image.yaml)

`rtl-mc` is a C++ tool for model checking temporal properties over *polyhedral systems*, continuous-time systems
governed by linear differential inclusions. These systems correspond to a decidable class of
**single-location linear hybrid systems**, where the dynamics are purely continuous and do not involve
discrete switching between modes.

The tool supports the verification of temporal properties written in **Real-Time Logic (RTL)**, a
logic that retains the syntax of standard LTL but is interpreted over real-valued, continuous-time behaviours.
This allows users to reason about how system trajectories evolve over time with respect to polyhedral constraints.

> 📘 This project was developed as part of my [Master's thesis](docs/thesis.pdf) in Computer Science at the University of
Naples Federico II, entitled _"Design and Implementation of a Model-Checking Tool for Verifying Linear Temporal Properties over Polyhedral Systems"_.
For a comprehensive understanding of the problem this tool solves, reading the thesis is strongly recommended.
See Chapter 12 for a wide range of experiments and evaluations.

## 🔍 Motivation
Hybrid systems combine discrete and continuous dynamics.
While verifying general hybrid systems is undecidable
in most cases, recent theoretical results, developed by my thesis advisors **Massimo Benerecetti**, **Marco Faella**, and **Fabio Mogavero**,
show that LTL model checking becomes decidable for single-location systems (i.e., systems with no discrete mode switching) under certain conditions.
`rtl-mc` is a practical realisation of these ideas, targeting this decidable class and offering exact verification without over-approximation.

## 🧾 Input Specification Format
The `rtl-mc` tool accepts two types of inputs:
- 🧱 Polyhedral Systems are specified in a custom textual format based on a formal grammar.
  The specification includes the invariant, flow, and atomic propositions. See Chapter 7 of the [Master's thesis](docs/thesis.pdf) for full grammar and examples.
- ⏱ RTL Formulae (Real-Time Logic) define temporal properties over polyhedral systems using a dedicated syntax with temporal and Boolean operators. 
  See Chapter 8 of the [Master's thesis](docs/thesis.pdf) for full grammar and examples.

## 🛠️ Usage
```
Usage: rtl-mc [--help] [--version]

Input options:
              [[--system-file <FILE>]|[--system-string <STRING>]|
              [--system-stdin]] [[--formula-file <FILE>]|
              [--formula-string <STRING>]|[--formula-stdin]]

Semantics options:
              [--semantics VAR] [[--existential]|[--universal]] [--mc VAR]

Automaton formula translation optimizations (Spot library flags):
              [[--low]|[--medium]|[--high]] [--any]

Output format:
              [[--verbose]...|[--quiet]|[--stats VAR]]

Advanced options:
              [--export-automaton-dot] [--export-denot-path-tree-dot]
              [--direct-ltl] [--concurrent]

Model Checking Linear Temporal Properties on Polyhedral Systems

Optional arguments:
  -h, --help                      shows help message and exits 
  -v, --version                   prints version information and exits 

Input options (detailed usage):
  -sf, --system-file <FILE>       Provide the polyhedral system from a file. 
  -ss, --system-string <STRING>   Provide the polyhedral system directly as a string. 
  -si, --system-stdin             Provide the polyhedral system via stdin (default). 
  -ff, --formula-file <FILE>      Provide the RTL formula from a file. 
  -fs, --formula-string <STRING>  Provide the RTL formula directly as a string. 
  -fi, --formula-stdin            Provide the RTL formula via stdin (default). 

Semantics options (detailed usage):
  --semantics                     E.g. --semantics=fin. Possible semantics:
                                       > fin:   Considers only finite-time trajectories (default).
                                                Suitable for properties that are positively verified
                                                as soon as a prefix of the trajectory satisfies them,
                                                such as reachability properties.
                                       > inf:   Considers only infinite-time trajectories.
                                                Suitable for non-terminating properties, such as
                                                safety or fairness properties.
                                       > may:   Considers all trajectories that are either infinite-time,
                                                or end in a may-exit point, i.e., a point on the boundary
                                                of the invariant from which at least one admissible
                                                direction exits.
                                       > must:  Considers all trajectories that are either infinite-time,
                                                or end in a must-exit point, i.e., a point on the boundary
                                                of the invariant from which all admissible directions exit. 
  --existential                   Compute the set of points from which there exists a trajectory that satisfies φ (default). 
  --universal                     Compute the set of points from which every trajectory satisfies φ. 
  --mc                            Check if a given point x ∈ ℚⁿ is the source of a trajectory in the polyhedral system 
                                  that satisfies the temporal formula φ. 
                                  For --existential, checks if some trajectory from the point satisfies φ. 
                                  For --universal, checks if all trajectories from the point satisfy φ. 
                                  Specify all system variables with rational values (e.g., [x=1/3, y=-2/3, z=1]). 

Automaton formula translation optimizations (Spot library flags) (detailed usage):
  --low                           Minimal optimizations during automaton construction (fast, default). 
  --medium                        Moderate optimizations during automaton construction. 
  --high                          All available optimizations during automaton construction (slow). 
  --any                           No preference. 

Output format (detailed usage):
  -V, --verbose                   Enable verbose output. Each occurrence of -V increases verbosity level (e.g. -VVV). [may be repeated]
  -q, --quiet                     Suppress all normal output. 
  -s, --stats                     Formats the execution statistics. 

Advanced options (detailed usage):
  --export-automaton-dot          Create a .dot file for each graph/automaton created (including any intermediate changes)
                                  during the solving process. 
  --export-denot-path-tree-dot    Create dot files representing path trees generated during the on-the-fly algorithm execution. 
  --direct-ltl                    Discretise the RTLf formula directly into LTL in a single step, improving performance (experimental).
                                  This option is only effective for finite-time semantics. 
  -c, --concurrent                Enable concurrent execution (highly experimental). This option is only effective with
                                  the on-the-fly algorithm for finite semantics.
```
## ⚙️ Installation
To install `rtl-mc` on your system, refer to the detailed [build instructions for Ubuntu 24.04](https://github.com/vtramo/rtl-mc/blob/main/docs/how-to-build-ubuntu.md).
### 🐳 Docker
If you have Docker installed, you can use this image:
```sh
docker pull vtramo/rtl-mc:1.0.0
```
## 🧰 Supporting Tools
In addition to the main model-checking engine `rtl-mc`, this project includes several companion tools that support
experimentation, benchmarking, and result visualisation:
- 📐 `rtl-gen`: Generates parameterised families of RTL formulae. Currently, it supports the alternation subcommand for
      creating RTL formulae that alternate between two atomic propositions.
- 🧪 `sys-gen`: Generates parametric polyhedral systems. Includes support for the scalable GAP experiment,
      which models interconnected tanks with uncertain flows.
- 🎨 `poly-ggb`: Converts polyhedral regions into interactive .ggb files for use with GeoGebra.
       Useful for visualising model-checking results in two dimensions.

All tools are documented in the [Master's thesis](docs/thesis.pdf). For example usages, refer to Chapter 11.