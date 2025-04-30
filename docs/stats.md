# Statistics

All placeholders listed below require a numerical index starting from 0 (e.g., `%As[0]`, `%ATs[1]`, etc.).
This index identifies the specific automaton the statistic refers to, as multiple automata of the same or
different types might be generated during execution.

| Placeholder                        | Scope / Context                             | Description                                                                             |
|------------------------------------|---------------------------------------------|-----------------------------------------------------------------------------------------|
| **General Solver Stats**           | Overall Process                             | *(General solver process statistics)*                                                   |
| `%x[i]`                            | Overall Process                             | Execution time (seconds) for the `i`-th recorded solver execution phase                 |
|                                    |                                             |                                                                                         |
| **RTL Formula Stats**              | RTL Formula                                 | *(RTL formula properties)*                                                              |
| `%rta[i]`                          | RTL Formula `i`                             | Total atomic propositions for RTL formula `i`                                           |
| `%rl[i]`                           | RTL Formula `i`                             | Length of RTL formula `i`                                                               |
|                                    |                                             |                                                                                         |
| **Polyhedral System Stats**        | Polyhedral System                           | *(Polyhedral system properties)*                                                        |
| `%Pta[i]`                          | Polyhedral System `i`                       | Total atomic propositions for polyhedral system `i`                                     |
| `%Psp[i]`                          | Polyhedral System `i`                       | Space dimension for polyhedral system `i`                                               |
|                                    |                                             |                                                                                         |
| **Discretisation Stats**           | Discretisation Process                      | *(Discretisation process statistics)*                                                   |
| `%da[i]`                           | Discretisation (RTL formula `i`)            | Total atomic propositions in discrete LTL formula for RTL formula `i`                   |
| `%dl[i]`                           | Discretisation (RTL formula `i`)            | Length of discrete LTL formula for RTL formula `i`                                      |
| `%dx[i]`                           | Discretisation (RTL formula `i`)            | Discretisation execution time (seconds) for RTL formula `i`                             |
|                                    |                                             |                                                                                         |
| **Base Automaton Stats**           | All Automata                                | *(General properties for any automaton)*                                                |
| `%As[i]`                           | Automaton `i`                               | Total states for automaton `i`                                                          |
| `%Ais[i]`                          | Automaton `i`                               | Total initial states for automaton `i`                                                  |
| `%Aas[i]`                          | Automaton `i`                               | Total accepting states for automaton `i`                                                |
| `%Ae[i]`                           | Automaton `i`                               | Total edges for automaton `i`                                                           |
| `%Ax[i]`                           | Automaton `i`                               | Execution time (seconds) for automaton `i`                                              |
| `%Ascc[i]`                         | Automaton `i`                               | Total SCC count for automaton `i`                                                       |
|                                    |                                             |                                                                                         |
| **Polyhedral LTL Automaton Stats** | Polyhedral LTL Automata                     | *(Includes base automaton stats, plus specifics for Polyhedral LTL Automaton)*          |
| `%ATo[i]`                          | Polyhedral LTL Automaton `i` (Translation)  | Translation optimization level for automaton `i`                                        |
| `%ATx[i]`                          | Polyhedral LTL Automaton `i` (Translation)  | Translation execution time (seconds) for automaton `i`                                  |
| `%ATs[i]`                          | Polyhedral LTL Automaton `i` (Translation)  | Translation total states for automaton `i`                                              |
| `%ATis[i]`                         | Polyhedral LTL Automaton `i` (Translation)  | Translation total initial states for automaton `i`                                      |
| `%ATase[i]`                        | Polyhedral LTL Automaton `i` (Translation)  | Translation total accepting sets for automaton `i`                                      |
| `%ATe[i]`                          | Polyhedral LTL Automaton `i` (Translation)  | Translation total edges for automaton `i`                                               |
| `%ATscc[i]`                        | Polyhedral LTL Automaton `i` (Translation)  | Translation total SCC count for automaton `i`                                           |
| `%AOx[i]`                          | Polyhedral LTL Automaton `i` (Optimization) | Optimized automaton execution time (s) for automaton `i`                                |
| `%AOs[i]`                          | Polyhedral LTL Automaton `i` (Optimization) | Optimized automaton total states for automaton `i`                                      |
| `%AOis[i]`                         | Polyhedral LTL Automaton `i` (Optimization) | Optimized automaton total initial states for automaton `i`                              |
| `%AOas[i]`                         | Polyhedral LTL Automaton `i` (Optimization) | Optimized automaton total accepting states for automaton `i`                            |
| `%AOe[i]`                          | Polyhedral LTL Automaton `i` (Optimization) | Optimized automaton total edges for automaton `i`                                       |
| `%AOscc[i]`                        | Polyhedral LTL Automaton `i` (Optimization) | Optimized automaton total SCC count for automaton `i`                                   |
| `%Amp[i]`                          | Polyhedral LTL Automaton `i`                | Max number of patches in the denotation of any state for automaton `i`                  |
| `%Atp[i]`                          | Polyhedral LTL Automaton `i`                | Total number of patches for automaton `i`                                               |
|                                    |                                             |                                                                                         |
| **Finite Automaton (NFA) Stats**   | Finite Automata (NFA)                       | *(Includes Polyhedral LTL stats, plus specifics for NFA)*                               |
| `%ANx[i]`                          | Finite (NFA) `i`                            | NFA execution time (seconds) for automaton `i`                                          |
| `%ANs[i]`                          | Finite (NFA) `i`                            | NFA total states for automaton `i`                                                      |
| `%ANis[i]`                         | Finite (NFA) `i`                            | NFA total initial states for automaton `i`                                              |
| `%ANas[i]`                         | Finite (NFA) `i`                            | NFA total accepting states for automaton `i`                                            |
| `%ANe[i]`                          | Finite (NFA) `i`                            | NFA total edges for automaton `i`                                                       |
| `%ANscc[i]`                        | Finite (NFA) `i`                            | NFA total SCC count for automaton `i`                                                   |
| `%ANr[i]`                          | Finite (NFA - On-the-fly) `i`               | NFA max recursive depth (on the fly) for automaton `i`                                  |
|                                    |                                             |                                                                                         |
| **Denotation On-The-Fly Stats**    | Denotation On-The-Fly Process               | *(Statistics for the "Denotation On-The-Fly" process)*                                  |
| `%Di[i]`                           | Denotation On-The-Fly `i`                   | Total iterations for the `i`-th Denotation On-The-Fly                                   |
| `%Dx[i]`                           | Denotation On-The-Fly `i`                   | Execution time (seconds) for the `i`-th Denotation On-The-Fly                           |
| `%Dp[i]`                           | Denotation On-The-Fly `i`                   | Total paths for the `i`-th Denotation On-The-Fly                                        |
| `%Dpr[i]`                          | Denotation On-The-Fly `i`                   | Total redundant paths for the `i`-th Denotation On-The-Fly                              |
| `%Drc[i]`                          | Denotation On-The-Fly `i`                   | Total reach operations for the `i`-th Denotation On-The-Fly                             |
| `%Dr[i]`                           | Denotation On-The-Fly `i`                   | Result for the `i`-th Denotation On-The-Fly                                             |
| `%Dd[i]`                           | Denotation On-The-Fly `i`                   | `true` if the result is incomplete (Denot has reached the maximum number of iterations) |
| `%Dmi[i]`                          | Denotation On-The-Fly `i`                   | Maximum number of iterations allowed (settable via `DENOT_MAX` environment variable)    |
