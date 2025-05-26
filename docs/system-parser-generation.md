# Prerequisites

- ANTLR4 Runtime >= 4.13.0: https://www.antlr.org/
- ANTLR4 CLI Tools: https://github.com/antlr/antlr4-tools/blob/master/README.md

## Parser Generation

The generation of the ANTLR4 parsers can be toggled via the `generate_parser` build option (default: enabled).  
When enabled, a message is printed before the parser is generated:

```
Generating ANTLR4 <parser_name> parser...
```

Once completed, a confirmation message is shown:

```
Parser generation completed! (Location: ...)'
```

### To disable parser generation:

```sh
meson configure -Dgenerate_parser=false <BUILD_DIRECTORY_PATH>
```

Parser generation occurs during the project configuration phase. This happens whenever you run one of the following Meson commands:

- `meson setup <BUILD_DIR_PATH>` (to create a new build directory)
- `meson setup <BUILD_DIR_PATH> --reconfigure` (to update an existing build directory)

Make sure the `generate_parser` build option is set to `true` when you want the parser to be generated (this is the default).

## Grammar Testing Tools

- http://lab.antlr.org/
