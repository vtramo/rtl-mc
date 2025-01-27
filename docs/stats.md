# Statistics


| Placeholder | Description                                                     |
|-------------|-----------------------------------------------------------------|
| `%Pta`      | Polyhedral System total atomic propositions                     |
| `%Psp`      | Polyhedral System space dimension                               |
| `%rta`      | Rtlf formula total atomic propositions                          |
| `%rl`       | Rtlf formula length                                             |
| `%dta`      | Discrete LTL formula total atomic propositions                  |
| `%dl`       | Discrete LTL formula length                                     |
| `%dx`       | Discretization execution time in ms                             |
| `%Ata`      | Translation Discrete LTL formula into TGBA total accepting sets |
| `%Ats`      | Translation Discrete LTL formula into TGBA total states         |
| `%Ate`      | Translation Discrete LTL formula into TGBA total edges          |
| `%Ato`      | Translation Discrete LTL formula into TGBA optimization level   |
| `%Atx`      | Translation Discrete LTL formula into TGBA execution time in ms |
| `%Afx`      | TGBA to NFA execution time in ms                                |
| `%Afs`      | TGBA to NFA total states                                        |
| `%Afe`      | TGBA to NFA total edges                                         |
| `%Aff`      | TGBA to NFA total final states                                  |
| `%Afox`     | NFA optimizations execution time in ms                          |
| `%Afos`     | NFA after optimizations total states                            |
| `%Afoe`     | NFA after optimizations total edges                             |
| `%Afof`     | NFA after optimizations total final states                      |
| `%Abx`      | BackwardNFA construction execution time in ms                   |
| `%Abs`      | BackwardNFA total states                                        |
| `%Abe`      | BackwardNFA total edges                                         |
| `%Abf`      | BackwardNFA total final states                                  |
| `%Abi`      | BackwardNFA total initial states                                |
| `%Dt`       | Denot total iterations                                          |
| `%Dx`       | Denot execution time in ms                                      |

---

# Example Usage

```bash
rtl-mc --gap 10 1 -s "Polyhedral System total atoms: %Pta | Discretization execution time: %dx ms | Denot execution time: %Dx ms."
```

```
Polyhedral System total atoms: 4 | Discretization execution time: 0.021915 | Denot execution time: 48.369822000000006 ms.
```