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

echo "--- Compiling BPlusTreeLib ---"
g++ -g -c  -fpermissive BPlusTreeLib/BPlusTree.c -o BPlusTreeLib/BPlusTree.o

echo "--- Compiling BPlusTreeCompFn.c ---"
g++ -g -c  -fpermissive core/BPlusTreeCompFn.c -o core/BPlusTreeCompFn.o

echo "--- Compiling catalog.c ---"
g++ -g -c core/catalog.c -o core/catalog.o

echo "--- Compiling sql_create.c ---"
g++ -g -c core/sql_create.c -o core/sql_create.o

echo "--- Compiling sql_delete.c ---"
g++ -g -c core/sql_delete.c -o core/sql_delete.o

echo "--- Creating DBMS Executable ---"
g++ -g SQLParserMain.o SQLCreateParserCFG.o lex.yy.o BPlusTreeLib/BPlusTree.o core/BPlusTreeCompFn.o core/catalog.o core/sql_create.o core/sql_delete.o -o dbms.exe -lfl
