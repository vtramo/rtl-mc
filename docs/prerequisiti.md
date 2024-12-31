# Prerequisiti
La documentazione su come buildare il progetto è in corso d'opera.

- PPL >= 14.0.0
- GMP >= 6.3.0
- GMPXX >= 6.3.0
- ANTLR4 Runtime >= 4.13.0: https://www.antlr.org/
- ANTLR4 CLI Tools: https://github.com/antlr/antlr4-tools/blob/master/README.md
- Meson >= 0.64.0 https://mesonbuild.com/Getting-meson.html
- Catch2 >=3.7.1 (test framework, installato automaticamente da meson)
- SPOT 2.12.1 (Installare la nostra versione modificata https://github.com/vtramo/spot/tree/mc/rtlf). Questa versione
introduce un nuovo operatore strict until operator (S). Il branch è `mc/rtlf`.

Please note that there might be some issues with the build process as I'm currently working on the best way
to streamline it after the dependencies are installed. However, it shouldn't be anything complicated, even at this stage.
Once the dependencies listed above are installed, there should be no issues moving forward.