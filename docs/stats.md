# 🔍 Built-in Statistics Engine

The `rtl-mc` tool provides a built-in statistics engine for collecting detailed metrics about each phase of the model-checking pipeline.

## 🚀 Enabling Statistics

Use the `--stats` (or `-s`) command-line option followed by a format string containing one or more placeholders:

```bash
rtl-mc --stats "Solver time: %x[0] s, RTL length: %rl[0]"
```

Each placeholder must be indexed with `[i]` to indicate which instance it refers to (e.g., solver 0, formula 1, etc.).

---

## 📊 Example

The following example checks a polyhedral system and extracts the total execution time of the solver using `%x[0]`:

```bash
sys-gen gap -t 2 -m 10 \
  | rtl-mc --semantics fin \
           -fs "t0 & G(t1) & $(rtl-gen alternation -k 4)" \
           --stats "Total solver execution time: %x[0] seconds."
```

### Output
```
Total solver execution time: 0.027833 seconds.
```

---

## 🧩 Placeholder Reference

All placeholders require an index `[i]`, starting from 0.

| Placeholder | Scope                   | Description                           |
|-------------|-------------------------|---------------------------------------|
| `%x[i]`     | General Solver          | Solver execution time (seconds)       |
| `%rta[i]`   | RTL Formula             | Number of atomic propositions         |
| `%rl[i]`    | RTL Formula             | Formula length                        |
| `%Pta[i]`   | Polyhedral System       | APs in the system                     |
| `%Psp[i]`   | Polyhedral System       | Space dimension                       |
| `%da[i]`    | Discretised RTL Formula | APs after discretisation              |
| `%dl[i]`    | Discretised RTL Formula | Length after discretisation           |
| `%dx[i]`    | Discretised RTL Formula | Time to discretise                    |
| `%As[i]`    | Automaton               | Number of states                      |
| `%Ais[i]`   | Automaton               | Initial states                        |
| `%Aas[i]`   | Automaton               | Accepting states                      |
| `%Ae[i]`    | Automaton               | Transitions (edges)                   |
| `%Ax[i]`    | Automaton               | Construction time                     |
| `%Ascc[i]`  | Automaton               | SCC count                             |
| `%ATx[i]`   | Translated Automaton    | Translation time                      |
| `%ATs[i]`   | Translated Automaton    | States count                          |
| `%ATis[i]`  | Translated Automaton    | Initial states                        |
| `%ATase[i]` | Translated Automaton    | Accepting sets                        |
| `%ATe[i]`   | Translated Automaton    | Edges                                 |
| `%ATscc[i]` | Translated Automaton    | SCCs                                  |
| `%AOx[i]`   | Optimised Automaton     | Optimisation time                     |
| `%AOs[i]`   | Optimised Automaton     | States after optimisation             |
| `%AOis[i]`  | Optimised Automaton     | Initial states                        |
| `%AOas[i]`  | Optimised Automaton     | Accepting states                      |
| `%AOe[i]`   | Optimised Automaton     | Edges                                 |
| `%AOscc[i]` | Optimised Automaton     | SCCs                                  |
| `%Amp[i]`   | Patches                 | Max patches in any state              |
| `%Atp[i]`   | Patches                 | Total patches                         |
| `%ANx[i]`   | Finite Automaton (NFA)  | NFA generation time                   |
| `%ANs[i]`   | Finite Automaton (NFA)  | Number of states                      |
| `%ANis[i]`  | Finite Automaton (NFA)  | Initial states                        |
| `%ANas[i]`  | Finite Automaton (NFA)  | Accepting states                      |
| `%ANe[i]`   | Finite Automaton (NFA)  | Edges                                 |
| `%ANscc[i]` | Finite Automaton (NFA)  | SCCs                                  |
| `%ANr[i]`   | On-the-Fly NFA          | Max recursive depth                   |
| `%Di[i]`    | DenotOnTheFly           | Total iterations                      |
| `%Dx[i]`    | DenotOnTheFly           | Execution time                        |
| `%Dp[i]`    | DenotOnTheFly           | Total paths                           |
| `%Dpr[i]`   | DenotOnTheFly           | Redundant paths                       |
| `%Drc[i]`   | DenotOnTheFly           | Reach operations                      |
| `%Dr[i]`    | DenotOnTheFly           | Final result                          |
| `%Dd[i]`    | DenotOnTheFly           | `true` if result incomplete           |
| `%Dmi[i]`   | DenotOnTheFly           | Max iteration limit (env `DENOT_MAX`) |

---