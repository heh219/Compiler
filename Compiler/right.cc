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

const string Parser::ops[] = {"ADD", "SUB", "MULT", "DIV",

		      "ISEQ", "ISNE", "ISLT", "ISLE", "ISGT", "ISGE",

		      "AND", "OR",

		      "PUSHL", "PUSHV", "STORE",

		      "JUMP", "JUMPF", "JUMPT", "CALL", "RET",
			      //add something
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

/*Parser::TreeNode* Parser::funcall(){
  }*/

Parser::TreeNode* Parser::factor() {
  TreeNode* factorNode;
  Operation op;
  if(token.type() == Token::LPAREN){
    check(Token::LPAREN,"Expecting (");
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
      op = DIV;
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
  if(token.type() == Token::EQ || token.type() == Token::LT || token.type() == Token::LE || token.type() == Token::GT || token.type() == Token::GE || token.type() == Token::NE){
    int tokenType;
    tokenType = token.type();
    switch(tokenType){
    case Token::EQ: op = ISEQ;
      break;
    case Token::LT: op = ISLT;
      break;
    case Token::LE: op = ISLT;
      break;
    case Token::GT: op = ISGT;
      break;
    case Token::GE: op = ISGE;
      break;
    case Token::NE: op = ISNE;
      break;
    }
    token = lexer.nextToken();
    expressionNode = expression();
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

/*Parser::TreeNode* Parser::returnStatement(){
  }*/

Parser::TreeNode* Parser::ifStatement(){
  TreeNode* logicalNode;
  TreeNode* blockNode;
  TreeNode* seqNode;
  TreeNode* node;
  if(token.type() == Token::IF){
    token = lexer.nextToken();
    check(Token::LPAREN,"Expecting (");
    token = lexer.nextToken();
    logicalNode = logicalExpression();
    check(Token::RPAREN,"Expecting )");
    token = lexer.nextToken();
    blockNode = block();
    token = lexer.nextToken();
    if(token.type() == Token::ELSE){
      seqNode = new TreeNode(SEQ,logicalNode,new TreeNode(JUMPF,"L3"));
      node = new TreeNode(SEQ,blockNode,new TreeNode(JUMP,"L2"));
      seqNode = new TreeNode(SEQ,seqNode,node);
      token = lexer.nextToken();
      blockNode = block();
      node = new TreeNode(SEQ,new TreeNode(LABEL,"L3:"),blockNode);
      seqNode = new TreeNode(SEQ,seqNode,node);
    }
    else{
      seqNode = new TreeNode(SEQ,logicalNode,new TreeNode(JUMPF,"L2"));
      seqNode = new TreeNode(SEQ,seqNode,blockNode);
    }
    seqNode = new TreeNode(SEQ,seqNode,new TreeNode(LABEL,"L2:"));
  }
  return seqNode;
}

/*Parser::TreeNode* Parser::assignmentExpression(){
  }

Parser::TreeNode* Parser::printfStatement(){
}*/

Parser::TreeNode* Parser::whileStatement(){
  TreeNode* logicalNode;
  TreeNode* blockNode;
  TreeNode* seqNode;
  TreeNode* node1;
  TreeNode* node2;
  if(token.type() == Token::WHILE){
    token = lexer.nextToken();
    check(Token::LPAREN,"Expecting (");
    token = lexer.nextToken();
    logicalNode = logicalExpression();
    check(Token::RPAREN,"Expecting )");
    token = lexer.nextToken();
    blockNode = block();
    seqNode = new TreeNode(SEQ,new TreeNode(LABEL,"L2:"),logicalNode);
    node1 = new TreeNode(SEQ,new TreeNode(JUMPF,"L3"),blockNode);
    seqNode = new TreeNode(SEQ,seqNode,node1);
    node2 = new TreeNode(SEQ,new TreeNode(JUMP,"L2"),new TreeNode(LABEL,"L3:"));
    seqNode = new TreeNode(SEQ,seqNode,node2);
    token = lexer.nextToken();
  }
  return seqNode;
}

Parser::TreeNode* Parser::assignStatement(){
  TreeNode* logicalNode;
  TreeNode* assignNode;
  check(Token::IDENT,"Hi There!");
  TreeNode* ident = new TreeNode(STORE,token.lexeme());
  token = lexer.nextToken();
  check(Token::ASSIGN,"Expecting =");
  token = lexer.nextToken();
  logicalNode = logicalExpression();
  check(Token::SEMICOLON,"Expecting ;");
  assignNode = new TreeNode(SEQ,logicalNode,ident);
  
  return assignNode;
}

/*Parser::TreeNode* Parser::vardefStatenebt(){
  }*/

Parser::TreeNode* Parser::statement(){
  TreeNode* stateNode;
  if(token.type() == Token::IF){
    stateNode = ifStatement();
  }
  else if(token.type() == Token::WHILE){
    stateNode = whileStatement();
  }
  else if(token.type() == Token::IDENT){
    stateNode = assignStatement();
  }
  return stateNode;
}

Parser::TreeNode* Parser::block(){
  TreeNode* stateNode = NULL;
  check(Token::LBRACE,"Expecting {");
  token = lexer.nextToken();
  while(token.type() != Token::RBRACE){
    stateNode = statement();
    token = lexer.nextToken();
  }
  return stateNode;
}

/*Parser::TreeNode* Parser::parameterdef(){
}

Parser::TreeNode* Parser::parameterdefs(){
}

Parser::TreeNode* Parser::function(){
}

Parser::TreeNode* Parser::comppilationunit(){
}*/
