/*
  CSE109
  He Huang
  heh219
  Program Description: this program is to initialize all the fileds,
  methods and constructors for the Lexer class
  Program #4
*/

#include "token.h"
#include "lex.h"
#include <ctype.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

Lex::Lex(istream& input):info(input){
  line = 1;
  pos = 0;
}

Lex::~Lex(){
}

char Lex::nextChar(){
  char ch;
  ch = info.get();
  if(ch == EOF){
    return '#';
  }
  else if(ch == '\n'){
    line++;
    pos = 0;
    return ' ';
  }
  else{
    pos++;
    return ch;
  }
}

/*
  this is the method that I used to check whether there is else after
  if block to continue generate the tree node
 */
int Lex::elseCheck(){
  int doit;
  char ch = info.get();
  string store = "";
  string els = "";
  int count = 0;
  store += ch;
  while(ch == '\n' || isblank(ch)){
    ch = info.get();
    store += ch;
  }
  els += ch;
  while(ch != EOF && count<3 && isalpha(ch)){
    ch = info.get();
    store += ch;
    els += ch;
    count++;
  }
  if(els == "else"){
    doit = 1;
  }
  else{
    doit = 0;
  }
  for(unsigned int i=0;i<store.size();i++){
    info.putback(store.at(store.size()-i-1));
  }
  return doit;
}

Token Lex::nextToken(){
  char element = nextChar();
  string lexeme = "";
  Token token;
  int realpos;
  int realline;

  while(isblank(element)){
    element = nextChar();
  }

  realpos = pos;
  realline = line;
  
  //INDENT OR KEYWORD
  if(isalpha(element)){
    lexeme += element;
    while(isdigit(info.peek()) || isalpha(info.peek())){
      element = nextChar();
      lexeme += element;
    }
    
    if(lexeme == "if"){
      return Token(23,lexeme,realline,realpos);
    }
    else if(lexeme == "else"){
      return Token(24,lexeme,realline,realpos);
    }
    else if(lexeme == "while"){
      return Token(25,lexeme,realline,realpos);
    }
    else if(lexeme == "function"){
      return Token(26,lexeme,realline,realpos);
    }
    else if(lexeme == "var"){
      return Token(27,lexeme,realline,realpos);
    }
    else if(lexeme == "printf"){
      return Token(28,"PRINTF",realline,realpos);
    }
    else if(lexeme == "return"){
      return Token(29,lexeme,realline,realpos);
    }
    else{
      return Token(1,lexeme,realline,realpos);
    }  
  }
    
  //INTEGER
  else if(isdigit(element)){
    lexeme += element;
    while(isdigit(info.peek())){
      element = nextChar();
      lexeme += element;
    }
    return Token(2,lexeme,realline,realpos);
  }

  //STRING
  else if(element == '"'){
    element = nextChar();
    lexeme += element;
    element = nextChar();
    while(element != '"'){
      lexeme += element;
      element = nextChar();
    }
    //lexeme += element;
    return Token(3,lexeme,realline,realpos);
  }
    
  //END OF FILE
  else if(element == '#'){
    lexeme += element;
    return Token(30,"ENDOFFILE",realline,realpos);
    exit(1);
  }
    
  //SPECIAL CHARACTER
  else if(ispunct(element)){
    lexeme += element;
    if(element == '('){
      return Token(17,lexeme,realline,realpos);
    }
    else if(element == ')'){
      return Token(18,lexeme,realline,realpos);
    }
    else if(element == '{'){
      return Token(19,lexeme,realline,realpos);
    }
    else if(element == '}'){
      return Token(20,lexeme,realline,realpos);
    }
    
    if(ispunct(info.peek())){ 
      element = nextChar();
      lexeme += element;
    }
    
    if(lexeme.length() == 1){
      switch(lexeme[0]){
      case '+':
	return Token(4,lexeme,realline,realpos);
	break;
      case '-':
	return Token(5,lexeme,realline,realpos);
	break;
      case '*':
	return Token(6,lexeme,realline,realpos);
	break;
      case '/':
	return Token(7,lexeme,realline,realpos);
	break;
      case '=':
	return Token(8,lexeme,realline,realpos);
	break;
      case '<':
	return Token(11,lexeme,realline,realpos);
	break;
      case '>':
	return Token(13,lexeme,realline,realpos);
	break;
      case ',':
	return Token(21,lexeme,realline,realpos);
	break;
      case ';':
	return Token(22,lexeme,realline,realpos);
	break;
      }
    }
    else if(lexeme == "=="){
      return Token(9,lexeme,realline,realpos);
    }
    else if(lexeme == "!="){
      return Token(10,lexeme,realline,realpos);
    }
    else if(lexeme == "<="){
      return Token(12,lexeme,realline,realpos);
    }
    else if(lexeme == ">="){
      return Token(14,lexeme,realline,realpos);
    }
    else if(lexeme == "&&"){
      return Token(15,lexeme,realline,realpos);
    }
    else if(lexeme == "||"){
      return Token(16,lexeme,realline,realpos);
    }
    else{
      return Token(31,"ERROR",realline,realpos);
    }
  }
  //ERROR
  return Token(31,"ERROR",realline,realpos);
}

