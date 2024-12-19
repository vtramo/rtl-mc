# Prerequisiti
- ANTLR4 Runtime >= 4.13.0: https://www.antlr.org/
- ANTLR4 CLI Tools: https://github.com/antlr/antlr4-tools/blob/master/README.md

## Generazione parser
The generation of the ANTLR4 PolyhedralSystem parser can be enabled or disabled using the `generate_parser` option (default: enabled).
Displays the message "Generating ANTLR4 PolyhedralSystem parser..." before running the appropriate script to generate the parser.
The script executed depends on the host operating system:
- On Windows, `generate-parser.bat` is run.
- On Unix-like systems, `generate-parser.sh` is run.

Once the generation is complete, the message "Parser generation complete!" is shown,indicating the parser has been generated successfully at the specified location:
`/home/vincenzo/projects/rtl-mc/src/parser/antlr4/generated/`.

Per disabilitare la generazione del parser:
```shell
meson configure -Dgenerate_parser=false <BUILD_DIRECTORY_PATH> 
```

Commit: [42fc4d2733b0c959268161fb3283467add395163](https://github.com/vtramo/rtl-mc/commit/42fc4d2733b0c959268161fb3283467add395163)