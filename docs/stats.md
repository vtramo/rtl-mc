# Statistics


| Placeholder | Description                                                          |
|-------------|----------------------------------------------------------------------|
| `%Pta`      | Polyhedral System total atomic propositions                          |
| `%Psp`      | Polyhedral System space dimension                                    |
| `%rta`      | RTL formula total atomic propositions                                |
| `%rl`       | RTL formula length                                                   |
| `%dta`      | Discrete LTL formula total atomic propositions                       |
| `%dl`       | Discrete LTL formula length                                          |
| `%dx`       | Discretisation execution time in s                                   |
| `%Ata`      | Translation Discrete LTL formula into TGBA total accepting sets      |
| `%Ats`      | Translation Discrete LTL formula into TGBA total states              |
| `%Ate`      | Translation Discrete LTL formula into TGBA total edges               |
| `%Ato`      | Translation Discrete LTL formula into TGBA optimization level        |
| `%Atx`      | Translation Discrete LTL formula into TGBA execution time in s       |
| `%Afx`      | TGBA to NFA execution time in s                                      |
| `%Afs`      | TGBA to NFA total states                                             |
| `%Afe`      | TGBA to NFA total edges                                              |
| `%Aff`      | TGBA to NFA total final states                                       |
| `%Afox`     | NFA optimizations execution time in s                                |
| `%Afos`     | NFA after optimizations total states                                 |
| `%Afoe`     | NFA after optimizations total edges                                  |
| `%Afof`     | NFA after optimizations total final states                           |
| `%Abx`      | BackwardNFA construction execution time in s                         |
| `%Abs`      | BackwardNFA total states                                             |
| `%Abe`      | BackwardNFA total edges                                              |
| `%Abf`      | BackwardNFA total final states                                       |
| `%Abi`      | BackwardNFA total initial states                                     |
| `%Abc`      | BackwardNFA total strongly connected component                       |
| `%Abm`      | BackwardNFA maximum number of patches in the denotation of any state |
| `%Abp`      | BackwardNFA total patches                                            |
| `%Abr`      | BackwardNFA max recursive depth                                      |
| `%Dt`       | Denot total iterations                                               |
| `%Dx`       | Denot execution time in s                                            |

---

# Example Usage

```bash
rtl-mc --gap 10 1 -s "Polyhedral System total atoms: %Pta | Discretisation execution time: %dx s | Denot execution time: %Dx s."
```

```
Polyhedral System total atoms: 4 | Discretisation execution time: 0.021374 s | Denot execution time: 48.379416000000006 s.
```