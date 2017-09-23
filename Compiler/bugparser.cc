hhhh/*
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
  if(token.type() == Token::EQ){
    op = ISEQ;
  }
  else if(token.type() == Token::LT){
    op = ISLT;
  }
  else if(token.type() == Token::LE){
    op = ISLE;
  }
  else if(token.type() == Token::GT){
    op = ISGT;
  }
  else if(token.type() == Token::GE){
    op = ISGE;
  }
  else if(token.type() == Token::NE){
    op = ISNE;
  }
  token = lexer.nextToken();
  expressionNode = expression();
  relationNode = new TreeNode(op,relationNode,expressionNode);
  
  return relationNode;
}

Parser::TreeNode* Parser::logicalExpression() {
  TreeNode* logicalNode = relationalExpression();
  TreeNode* relationNode;
  Operation op;
  while(token.type() == Token::AND || token.type() == Token::OR){
    int type = token.type();
    if(type == Token::AND){
      op = AND;
    }
    else{
      op = OR;
    }
    token = lexer.nextToken();
    relationNode = relationalExpression();
    logicalNode = new TreeNode(op,logicalNode,relationNode);
  }
  return logicalNode;
}

/*Parser::TreeNode* Parser::returnStatement(){
  }*/

Parser::TreeNode* Parser::ifStatement(){
  TreeNode* logicalNode;
  TreeNode* blockNode;
  TreeNode* seqNode;
  if(token.type() == Token::IF){
    token = lexer.nextToken();
    check(Token::LPAREN,"Expecting (");
    token = lexer.nextToken();
    logicalNode = logicalExpression();
    check(Token::RPAREN,"Expecting )");
    token = lexer.nextToken();
    blockNode = block();
    if(token.type() == Token::ELSE){
      seqNode = new TreeNode(SEQ,logicalNode,new TreeNode(JUMPF,"L1"));
      seqNode = new TreeNode(SEQ,seqNode,blockNode);
      seqNode = new TreeNode(SEQ,seqNode,new TreeNode(JUMP,"L2"));
      seqNode = new TreeNode(SEQ,seqNode,new TreeNode(LABEL,"L1:"));
      token = lexer.nextToken();
      blockNode = block();
      seqNode = new TreeNode(SEQ,seqNode,blockNode);
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
  if(token.type() == Token::WHILE){
    check(Token::WHILE,"Expecting while");
    token = lexer.nextToken();
    if(token.type() == Token::LPAREN){
      check(Token::LPAREN,"Expecting (");
      token = lexer.nextToken();
      logicalNode = logicalExpression();
      if(token.type() == Token::RPAREN){
	check(Token::RPAREN,"Expecting )");
	token = lexer.nextToken();
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
      }
    }
  }
  token = lexer.nextToken();
  return seqNode;
}

Parser::TreeNode* Parser::assignStatement(){
  TreeNode* logicalNode;
  TreeNode* assignNode;
  Operation op;
  if(token.type() == Token::IDENT){
    op = STORE;
    TreeNode* ident = new TreeNode(op,token.lexeme());
    token = lexer.nextToken();
    check(Token::ASSIGN,"Expecting =");
    token = lexer.nextToken();
    logicalNode = logicalExpression();
    op = SEQ;
    assignNode = new TreeNode(op,logicalNode,ident);
    check(Token::SEMICOLON,"Expecting ;");
    token = lexer.nextToken();
  }
  return assignNode;
}

/*Parser::TreeNode* Parser::vardefStatenebt(){
  }*/

Parser::TreeNode* Parser::statement(){
  TreeNode* node;
  if(token.type() == Token::WHILE){
    node = whileStatement();
  }
  else if(token.type() == Token::IF){
    node = ifStatement();
  }
  else if(token.type() == Token::IDENT){
    node = assignStatement();
  }
  else{
    return NULL;
  }
  return stateNode;
}

Parser::TreeNode* Parser::block(){
  TreeNode* stateNode;
  if(token.type() == Token::LBRACE){
    check(Token::LBRACE,"Expecting {");
    token = lexer.nextToken();
    stateNode = statement();
    while(token.type() != Token::RBRACE){
      stateNode = statement();
      token = lexer.nextToken();
    }
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
