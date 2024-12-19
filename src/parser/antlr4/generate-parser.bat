@echo off
antlr4 -listener -visitor -Dlanguage=Cpp .\g4\PolyhedralSystem.g4 -o .\generated
move .\generated\g4\* .\generated\
rmdir /s /q .\generated\g4
