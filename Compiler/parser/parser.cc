/*
  CSE109
  He Huang
  heh219
  Program Description: this is parser.h which is to initialize
  constructors, fields and methods for parser to handle the goal as
  Compiler
  Program Deadline: 10/24/2016
*/


#include "parser.h"
#include <cstring>

const string ops[] = {"ADD", "SUB", "MULT", "DIV",

		      "ISEQ", "ISNE", "ISLT", "ISLE", "ISGT", "ISGE",

		      "AND", "OR",

		      "PUSHL", "PUSHV", "STORE",

		      "JUMP", "JUMPF", "JUMPT", "CALL", "RET",

		      "PRINTF",

		      "LABEL", "SEQ" };


Parser::Parser(Lex& lexerx, ostream& outx): lexer(lexerx), out(outx), lindex(1), tindex(1) {
  token = lexer.nextToken();
}

Parser::~Parser() {
}

void Parser::error(string message) {
  cerr << message << " Found " << token.lexeme() << " at line " << token.line() << " position " << token.pos() << endl;
  exit(1);
}

void Parser::check(int tokenType, string message) {
  if (token.type() != tokenType)
    error(message);
}

Parser::TreeNode* Parser::funcall(){
}

Parser::TreeNode* Parser::factor() {
  TreeNode* factorNode;
  Operation op;
  check(Token::LPAREN,"Expecting (");
  if(token.type() == Token::LPAREN){
    token = lexer.nextToken();
    factorNode = expression();
    check(Token::RPAREN,"Expecting )");
  }
  else if(token.type() == Token::INTLIT){
    op = PUSHL;
    factorNode = new TreeNode(op,token.lexeme());
  }
  else if(token.type() == Token::IDENT){
    op = PUSHV;
    factorNode = new TreeNode(op,token.lexeme());
  }
  else{
    //    factorNode = funcall();
  }
  token = lexer.nextToken();
  return factorNode;
}

Parser::TreeNode* Parser::term() {
  TreeNode* termNode = factor();
  TreeNode* factorNode;
  Operation op;
  while(token.type() == Token::TIMES || token.type() == Token::DIVIDE){
    int type = token.type();
    token = lexer.nextToken();
    factorNode = factor();
    if(type == Token::TIMES){
      op = MULT;
      termNode = new TreeNode(op,termNode,factorNode);
    }
    else{
      op = DIVIDE;
      termNode = new TreeNode(op,termNode,factorNode);
    }
  }
  return termNode;
}

Parser::TreeNode* Parser::expression() {
  TreeNode* expressionNode = term();
  TreeNode* termNode;
  Operation op;
  while(token.type() == Token::PLUS || token.type() == Token::MINUS){
    int type = token.type();
    token = lexer.nextToken();
    termNode = term();
    if(type == Token::PLUS){
      op = ADD;
      expressionNode = new TreeNode(op,expressionNode,termNode);
    }
    else{
      op = SUB;
      expressionNode = new TreeNode(op,expressionNode,termNode);
    }
  }
  return expressionNode;
}

Parser::TreeNode* Parser::relationalExpression() {
  TreeNode* relationNode = expression();
  Operation op;
  TreeNode* expressionNode;
  if(token.type() == Token::EQ){
    int type = token.type();
    token = lexer.nextToken();
    expressionNode = expression();
    op = ISEQ;
    relationNode = new TreeNode(op,relationNode,expressionNode);
  }
  else if(token.type() == Token::LT){
    int type = token.type();
    token = lexer.nextToken();
    expressionNode = expression();
    op = ISLT;
    relationNode = new TreeNode(op,relationNode,expressionNode);
  }
  else if(token.type() == Token::LE){
    int type = token.type();
    token = lexer.nextToken();
    expressionNode = expression();
    op = ISLE;
    relationNode = new TreeNode(op,relationNode,expressionNode);
  }
  else if(token.type() == Token::GT){
    int type = token.type();
    token = lexer.nextToken();
    expressionNode = expression();
    op = ISGT;
    relationNode = new TreeNode(op,relationNode,expressionNode);
  }
  else if(token.type() == Token::GE){
    int type = token.type();
    token = lexer.nextToken();
    expressionNode = expression();
    op = ISGE;
    relationNode = new TreeNode(op,relationNode,expressionNode);
  }
  else if(token.type() == Token::NE){
    int type = token.type();
    token = lexer.nextToken();
    expressionNode = expression();
    op = ISNE;
    relationNode = new TreeNode(op,relationNode,expressionNode);
  }
  return relationNode;
}

Parser::TreeNode* Parser::logicalExpression() {
  TreeNode* logicalNode = relationalExpression();
  TreeNode* relationNode;
  Operation op;
  while(token.type() == Token::AND || token.type() == Token::OR){
    int type = token.type();
    token = lexer.nextToken();
    relationNode = relationalExpression();
    if(type == Token::AND){
      op = AND;
      logicalNode = new TreeNode(op,logicalNode,relationNode);
    }
    else{
      op = OR;
      logicalNode = new TreeNode(op,logicalNode,relationNode);
    }
  }
  return logicalNode;
}

Parser::TreeNode* Parser::returnStatement(){
}

Parser::TreeNode* Parser::ifStatement(){
  TreeNode* logicalNode;
  TreeNode* blockNode;
  TreeNode* seqNode;
  check(Token::IF,"Expecting if");
  token = lexer.nextToken();
  check(Token::LPAREN,"Expecting (");
  logicalNode = logicalExpression();
  TreeNode* node = new TreeNode(JUMP,"L1");
  seqNode = new TreeNode(SEQ,logicalNode,node);
  check(Token::RPAREN,"Expecting )");
  blockNode = block();
  seqNode = new TreeNode(SEQ,seqNode,blockNode);
  node = new TreeNode(JUMP,"L2");
  seqNode = new TreeNode(SEQ,seqNode,node);
  node = new TreeNode(LABEL,"L1");
  seqNode = new TreeNode(SEQ,seqNode,node);
  if(token.type() == Token::ELSE){
    token = lexer.nextToken();
    blockNode = block();
    seqNode = new TreeNode(SEQ,seqNode,blockNode);
  }
  node = new TreeNode(LABEL,"L2");
  seqNode = new TreeNode(SEQ,seqNode,node);
  token = lexer.nextToken();
}

Parser::TreeNode* Parser::assignmentExpression(){
}

Parser::TreeNode* Parser::printfStatement(){
}

Parser::TreeNode* Parser::whileStatement(){
  TreeNode* logicalNode;
  TreeNode* blockNode;
  TreeNode* seqNode;
  check(Token::WHILE,"Expecting while");
  token = lexer.nextToken();
  check(Token::LPAREN,"Expecting (");
  logicalNode = logicalExpression();
  check(Token::RPAREN,"Expecting )");
  blockNode = block();
  Operation op;
  op = LABEL;
  TreeNode* node = new TreeNode(op,"L1");
  op = SEQ;
  seqNode = new TreeNode(op,node,logicalNode);
  op = JUMPF;
  node = new TreeNode(op,"L2");
  op = SEQ;
  seqNode = new TreeNode(op,seqNode,node);
  seqNode = new TreeNode(op,seqNode,blockNode);
  op = JUMP;
  node = new TreeNode(op,"L1");
  op = SEQ;
  seqNode = new TreeNode(op,seqNode,node);
  op = LABEL;
  node = new TreeNode(op,"L2");
  op = SEQ;
  seqNode = new TreeNode(op,seqNode,node);
  token = lexer.nextToken();
}

Parser::TreeNode* Parser::assignStatement(){
  TreeNode* logicalNode;
  TreeNode* assignNode;
  Operation op;
  check(Token::IDENT,"Expecting IDENT");
  op = STORE;
  TreeNode* ident = new TreeNode(op,token.lexeme());
  token = lexer.nextToken();
  check(Token::ASSIGN,"Expecting =");
  token = lexer.nextToken();
  logicalNode = logicalExpression();
  check(Token::SEMICOLON,"Expecting ;");
  op = SEQ;
  assignNode = new TreeNode(op,logicalNode,ident);
  token = lexer.nextToken();
  return assignNode;
}

Parser::TreeNode* Parser::vardefStatenebt(){
}

Parser::TreeNode* Parser::statement(){
  TreeNode* assignNode;
  TreeNode* whileNode;
  TreeNode* ifNode;
  Operation op;
  op = SEQ;
  TreeNode* stateNode;
  stateNode = new TreeNode(op,assignNode,whileNode);
  stateNode = new TreeNode(op,stateNode,ifNode);
  return stateNode;
}

Parser::TreeNode* Parser::block(){
  TreeNode* stateNode;
  check(Token::LBRACE,"Expecting {");
  token = lexer.nextToken();
  while(token.type() != Token::RBRACE){
    stateNode = statement();
    token = lexer.nextToken();
  }
  return stateNode;
}

Parser::TreeNode* Parser::parameterdef(){
}

Parser::TreeNode* Parser::parameterdefs(){
}

Parser::TreeNode* Parser::function(){
}

Parser::TreeNode* Parser::comppilationunit(){
}
