#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

echo "--- Generating Lexer ---"
lex -o lex_parser/lex.yy.c lex_parser/Parser.l


echo "--- Compiling lex.yy.c ---"
g++ -g -Iheaders -c lex_parser/lex.yy.c -o lex_parser/lex.yy.o

echo "--- Compiling ExpressionParser.c ---"
g++ -g -Iheaders -c ExpressionParser/ExpressionParser.c -o ExpressionParser/ExpressionParser.o

echo "--- Compiling EnumConverter.cpp ---"
g++ -g -Iheaders -c Enums/EnumConverter.cpp -o Enums/EnumConverter.o

echo "--- Compiling Operators.cpp ---"
g++ -g -Iheaders -c Operators/Operators.cpp -o Operators/Operators.o

echo "--- Compiling Dtype.cpp ---"
g++ -g -Iheaders -c Dtypes/Dtype.cpp -o Dtypes/Dtype.o

echo "--- Compiling MexprTree.cpp ---"
g++ -g -Iheaders -c MexprTree/MexprTree.cpp -o MexprTree/MexprTree.o

echo "--- Compiling calculator.cpp---"
g++ -g -Iheaders -c calculator/calculator.cpp -o calculator/calculator.o

echo "--- Creating calculator executable ---"
g++ -g lex_parser/lex.yy.o ExpressionParser/ExpressionParser.o Enums/EnumConverter.o  Operators/Operators.o Dtypes/Dtype.o MexprTree/MexprTree.o calculator/calculator.o -o executables/calculator/calculator -lfl

echo "--- Build Successful! Run with ./executables/calculator/calculator ---"

echo "--- Linking executable ---"
g++ -g lex_parser/lex.yy.o ExpressionParser/ExpressionParser.o Enums/EnumConverter.o -o executables/parser/exe -lfl

echo "--- Build Successful! Run with ./executables/parser/exe ---"

echo "--- Creating Static Library libMexpr.a ---"
ar rcs libMexpr.a ExpressionParser/ExpressionParser.o Operators/Operators.o Dtypes/Dtype.o MexprTree/MexprTree.o Enums/EnumConverter.o