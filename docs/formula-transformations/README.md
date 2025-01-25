## Discretizzazione di una formula RTLf
Una formula RTLf deve subire una serie di trasformazioni tra cui:
- **1. Discretizzazione secondo le regole del paper**: la formula RTLf viene discretizzata, trasformandola in una formula LTLf;
- **2. Trasformazione in una formula LTL**: La formula LTLf ottenuta viene successivamente trasformata in una formula LTL.

Presumibilmente, queste trasformazioni possono essere eseguite visitando l'albero della formula (utilizzando Spot) solo una volta, riducendo così il numero di passaggi necessari.

Spot fornisce una funzione `spot::from_ltlf` per convertire una formula con semantica LTLf in una formula con semantica LTL.
Esistono quindi due implementazioni per eseguire i due passi di sopra:
1. una implementazione che discretizza e trasforma in LTL in un solo colpo (non usa `spot::from_ltlf` ma implementa da zero la conversione, `--direct-ltl`)
2. una implementazione che discretizza, e poi usa `from_ltlf` per trasformare in LTL (default).

Entrambe le versioni sono state testate. La versione 1 risulta essere più performante:
```
-------------------------------------------------------------------------------
Discretization benchmarks
-------------------------------------------------------------------------------
../tests/benchmarks/discretization/discretization_benchmarks.cpp:7
...............................................................................

-benchmark name                       samples       iterations    est run time
                                     mean          low mean      high mean
                                     std dev       low std dev   high std dev
-------------------------------------------------------------------------------
RTLf to LTL (our version)                      100             1    65.3582 ms 
                                        660.581 us    656.526 us    675.324 us 
                                        34.7105 us    10.0423 us    78.7317 us 
                                                                               
RTLf to LTL (spot)                             100             1    160.943 ms 
                                        1.62471 ms    1.62274 ms    1.62705 ms 
                                        10.8612 us    8.49642 us    16.4507 us 
                                                                               

===============================================================================
All tests passed (838 assertions in 1 test case)

```


Le trasformazioni specifiche per ogni caso sono contenute nella directory `rules`.

![img.png](spot-ltl.png)