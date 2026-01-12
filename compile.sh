#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

echo "--- Generating Lexer ---"
lex SQLParser/Parser.l

echo "--- Compiling lex.yy.c ---"
g++ -g -c lex.yy.c -o lex.yy.o