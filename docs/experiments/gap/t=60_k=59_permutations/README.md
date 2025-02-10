# Esperimento sulle Permutazioni dell'Automa GAP (t=60, k=59)

L'automa prodotto con GAP per i valori **t=60** e **k=59** genera un automa composto da **243 stati** e **481 archi**.

Confrontando l'iterazione **t=60, k=59** con l'iterazione **t=60, k=60** di **DenotRecursive**, abbiamo osservato un notevole incremento nel numero di iterazioni di **DenotRecursive**.

Di seguito, un confronto tra le due iterazioni:

| k  | t  | Iterazioni totali DenotRecursive | Stati totali Translation | Archi totali Translation | Stati totali To Finite | Archi totali To Finite | Stati totali BackwardNFA | Archi totali BackwardNFA |
|----|----|----------------------------------|--------------------------|--------------------------|------------------------|------------------------|--------------------------|--------------------------|
| 59 | 60 | 243                              | 124                      | 365                      | 123                    | 363                    | 243                      | 481                      |
| 60 | 60 | 495                              | 126                      | 433                      | 126                    | 433                    | 250                      | 496                      |

### Osservazioni

Il considerevole aumento nelle iterazioni sembra essere una conseguenza diretta dell'incremento nel numero di archi prodotti dalla traduzione di **Spot**.

### Indagini Aggiuntive

Per approfondire ulteriormente, sono state effettuate delle permutazioni sui nomi degli stati nel grafo **BackwardNFA**. In particolare, i seguenti stati sono stati permutati sia in modo sequenziale che casuale:
```
{ 3, 242, 2, 11, 10, 0, 233, 236, 237, 232 }
```
I risultati di queste permutazioni sono stati archiviati nella cartella associata all'esperimento.
