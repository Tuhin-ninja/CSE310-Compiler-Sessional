#!/bin/bash 


flex 2105002.l
mv lex.yy.c lex.yy.cpp
g++ -std=c++17 lex.yy.cpp 2105002_hash_functions.cpp SymbolTableHelper.cpp -o compiler
./compiler input.c