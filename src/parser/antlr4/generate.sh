#!/usr/bin/env bash

antlr4 -listener -visitor -Dlanguage=Cpp ./g4/PolyhedralSystem.g4 -o ./generated
mv ./generated/g4/* ./generated
rm -r ./generated/g4