#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

echo "--- Generating Lexer ---"
lex SQLParser/Parser.l

echo "--- Compiling lex.yy.c ---"
g++ -g -c lex.yy.c -o lex.yy.o

echo "--- Compiling SQLParserMain.c ---"
g++ -g -c SQLParser/SQLParserMain.c -o SQLParserMain.o

echo "--- Compiling SQLCreateParserCFG.c ---"
g++ -g -c SQLParser/SQLCreateParserCFG.c -o SQLCreateParserCFG.o

echo "--- Creating DBMS Executable ---"
g++ -g SQLParserMain.o SQLCreateParserCFG.o lex.yy.o -o dbms.exe -lfl
