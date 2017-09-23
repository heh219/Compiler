/*
  CSE109
  He Huang
  heh219
  Program Description: this is microc.cc which is to read the File as
  oustream input to parser to handle those information
  Program Deadline: 10/24/2016
*/


#include "token.h"
#include "lex.h"
#include "parser.h"
#include <iostream>
#include <fstream>

using namespace std;

void processFile(istream& in) {
  Lex lexer(in);
  Token token;
  Parser parser(lexer, cout);
  Parser::TreeNode* program = parser.compilationunit();
  //cout << Parser::TreeNode::toString(program) << endl;
  parser.genasm(program);
}

int main(int argc, char **argv) {
  ifstream in;
  
  if (argc > 1) {
      in.open(argv[1]);
      processFile(in);
      in.close();
  } else {
    processFile(cin);
  }
  return 0;
  
}
