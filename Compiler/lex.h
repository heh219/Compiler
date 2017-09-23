/*
  CSE109
  He Huang
  heh219
  Porgram Description:this program is to declar the fields,
  constructors, methods for Lexer class
  Program #4
*/

#ifndef LEXER_H
#define LEXER_H
#include <iostream>

using namespace std;

class Token;

class Lex{
public:
  istream& info;
  int line;
  int pos;
  
  Lex(istream& input);
  ~Lex();

private:
  char nextChar();

public:
  Token nextToken();
  int elseCheck();
};
#endif
