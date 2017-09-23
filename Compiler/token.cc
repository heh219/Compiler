/*
  CSE109
  He Huang
  heh219
  Program Description:this program is to initialize all fileds,
  constructors and methods for Token class
  Program #4
*/

#include <stdio.h>
#include "token.h"

int Token::type(){
  return Type;
}

string Token::lexeme(){
  return Lexeme;
}

int Token::line(){
  return Line;
}

int Token::pos(){
  return Pos;
}


Token::Token(int type, string lexeme, int line, int pos){
  this->Type = type;
  this->Lexeme = lexeme;
  this->Line = line;
  this->Pos = pos;
}

Token::Token(){
  /*  this->Type = 0;
  this->Lexeme = NULL;
  this->Line = 0;
  this->Pos = 0;*/
}
  
Token::~Token(){
}
