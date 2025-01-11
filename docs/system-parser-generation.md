# Prerequisiti
- ANTLR4 Runtime >= 4.13.0: https://www.antlr.org/
- ANTLR4 CLI Tools: https://github.com/antlr/antlr4-tools/blob/master/README.md

## Generazione parser
The generation of the ANTLR4 PolyhedralSystem parser can be enabled or disabled using the `generate_parser` option (default: enabled).
Displays the message "Generating ANTLR4 PolyhedralSystem parser..." before running the appropriate script to generate the parser.
Once the generation is complete, the message "Parser generation complete!" is shown,indicating the parser has been generated successfully at the specified location:
`/home/vincenzo/projects/rtl-mc/src/parser/antlr4/generated/g4`.

Per disabilitare la generazione del parser:
```shell
meson configure -Dgenerate_parser=false <BUILD_DIRECTORY_PATH> 
```

La generazione del parser avviene a tempo di configurazione del progetto, sarebbe a dire ogni qualvolta viene eseguito
uno di questi comandi meson:
- `meson setup <BUILD_DIR_PATH>` (per creare una nuova build directory)
- `meson setup <BUILD_DIR_PATH> --reconfigure` (per riflettere i cambiamenti di configurazione in una build directory gia' esistente)

Ricordarsi di settare la build option `generate_parser` uguale a true (default).

## Tools per testing della grammatica
- http://lab.antlr.org/