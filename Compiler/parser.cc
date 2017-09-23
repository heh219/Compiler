 /*
  CSE109
  He Huang
  heh219
  Program Description: this is parser.h which is to initialize
  constructors, fields and methods for parser to handle the goal as
  Compiler
  Program Deadline: 10/24/2016
*/

#include "symboltable.h"
#include "parser.h"
#include <cstring>

const string Parser::ops[] = {"ADD", "SUB", "MULT", "DIV",

		      "ISEQ", "ISNE", "ISLT", "ISLE", "ISGT", "ISGE",

		      "AND", "OR",

		      "PUSHL", "PUSHV", "STORE",

		      "JUMP", "JUMPF", "JUMPT", "CALL", "RET",

		      "PRINTF",

		      "LABEL", "SEQ",

	     	      "FUNC","PARAM1","PARAM2","PARAM3","PARAM4"
};


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

string vars[100];
int n = 1;
void Parser::vardefs(TreeNode *node){
  if(node != NULL){
    if(node->op == PUSHV || node->op == STORE){
      vars[0] = node->val;
      int i;
      for(i=n-1;vars[i] != node->val;--i);
	if(i==0){
	  vars[n++] = node->val;
	}
    }
      vardefs(node->leftChild);
      vardefs(node->rightChild);
  }
}

void emit(string txt){
  cout << txt << endl;
}

int jindex = 0;
string jlabel(){
  string temp = "J";
  stringstream ss;
  ss << ++jindex;
  string res = ss.str();
  temp = temp + res;
  return temp;
}

void cond(string op){
  string j1 = jlabel();
  string j2 = jlabel();
  emit("  pop rbx");
  emit("  pop rax");
  emit("  cmp rax,rbx");
  emit("  "+op+" "+j1);
  emit("  mov rax,0");
  emit("  jmp "+j2);
  emit(j1 + ":");
  emit("  mov rax,1");
  emit(j2 + ":");
  emit("  push rax");
}

string currentFunc;
int nfmts = 0;
string fmts[100];
void Parser::geninst(TreeNode *node){
  string fmt = "";
  int nparams = 0;
  if(node != NULL){
    geninst(node->leftChild);
    geninst(node->rightChild);
    switch(node->op){
    case SEQ:
      break;
    case PUSHV:
      emit("  push qword["+node->val+"]");
      break;
    case PUSHL:
      emit("  mov rax,"+node->val);
      emit("  push rax");
      break;
    case STORE:
      emit("  pop qword["+node->val+"]");
      break;
    case ADD:
      emit("  pop rbx");
      emit("  pop rax");
      emit("  add rax,rbx");
      emit("  push rax");
      break;
    case SUB:
      emit("  pop rbx");
      emit("  pop rax");
      emit("  sub rax,rbx");
      emit("  push rax");
      break;
    case AND:
      emit("  pop rbx");
      emit("  pop rax");
      emit("  and rax,rbx");
      emit("  push rax");
      break;
    case OR:
      emit("  pop rbx");
      emit("  pop rax");
      emit("  or rax,rbx");
      emit("  push rax");
      break;
    case MULT:
      emit("  pop rbx");
      emit("  pop rax");
      emit("  imul rbx");
      emit("  push rax");
      break;
    case DIV:
      emit("  mov rdx,0");
      emit("  pop rbx");
      emit("  pop rax");
      emit("  idiv rbx");
      emit("  push rax");
      break;
    case LABEL:
      emit(node->val);
      break;
    case ISEQ:
      cond("je");
      break;
    case ISNE:
      cond("jne");
      break;
    case ISLT:
      cond("jl");
      break;
    case ISLE:
      cond("jle");
      break;
    case ISGT:
      cond("jg");
      break;
    case ISGE:
      cond("jge");
      break;
    case JUMP:
      emit("  jmp "+node->val);
      break;
    case JUMPF:
      emit("  pop rax");
      emit("  cmp rax,0");
      emit("  je "+node->val);
      break;
    case JUMPT:
      emit("  pop rax");
      emit("  cmp rax,0");
      emit("  jne "+node->val);
      break;
    case CALL:
      emit("  call "+node->val);
      emit("  push rax");
      break;
    case FUNC:
      currentFunc = node->val;
      emit(currentFunc);
      if(currentFunc != "main:")
	emit("  pop r15");
      break;
    case RET:
      emit("  pop rax");
      if(currentFunc != "main:")
	emit("  push r15");
      emit("  ret");
      break;
    case PRINTF:
      fmt = node->val;
      nparams = fmt.at(0)-'0';
      fmt = "`"+fmt.substr(1)+"`";
      fmts[nfmts++] = fmt;
      emit("  mov rdi,fmt" + itos(nfmts));
      if(nparams == 5){
	emit("  pop r9");
	--nparams;
      }
      if(nparams == 4){
	emit("  pop r8");
	--nparams;
      }
      if(nparams == 3){
	emit("  pop rcx");
	--nparams;
      }
      if(nparams == 2){
	emit("  pop rdx");
	--nparams;
      }
      if(nparams == 1){
	emit("  pop rsi");
      }
      emit("  mov rax,0");
      emit("  push rbp");
      emit("  call printf");
      emit("  pop rbp");
      break;
    default:
      cerr<<"In geninst: Unknown operation: "<<node->op<<endl;
      exit(1);
      break;      
    }
  }
}

void Parser::genasm(TreeNode *node){
  emit("  global main");
  emit("  extern printf");
  cout<<endl;
  emit("  segment .bss");

  vardefs(node);
  for(int i=1;i<n;++i){
    cout<<"  "<<vars[i]<<" resq 1"<<endl;
  }
  cout<<endl<<"  section .text"<<endl;
 
  geninst(node);
  cout<<endl<<"  section .data"<<endl;
  for(int i=0;i<nfmts;++i){
    cout<<"  fmt"<<i+1<<": db "<<fmts[i]<<", 0"<<endl;
  }
}

Parser::TreeNode* Parser::funcall(string functionName){
  TreeNode* expressionNode;
  TreeNode* funcallNode = NULL;
  if(token.type() == Token::LPAREN){
    token = lexer.nextToken();
    expressionNode = expression();
    while(token.type() == Token::COMMA){
      token = lexer.nextToken();
      expressionNode = new TreeNode(SEQ,expressionNode,expression());
    }
    check(Token::RPAREN,"Expecting )");
    funcallNode = new TreeNode(SEQ,expressionNode,new TreeNode(CALL,functionName));
    token = lexer.nextToken();
  }
  return funcallNode;
}

Parser::TreeNode* Parser::factor() {
  TreeNode* factorNode = NULL;
  Operation op;
  if(token.type() == Token::LPAREN){
    check(Token::LPAREN,"Expecting (");
    token = lexer.nextToken();
    factorNode = expression();
    check(Token::RPAREN,"Factor: Expecting )");
    token = lexer.nextToken();
  }
  else if(token.type() == Token::INTLIT){
    op = PUSHL;
    factorNode = new TreeNode(op,token.lexeme());
    token = lexer.nextToken();
  }
  else if(token.type() == Token::IDENT){
    string lex = token.lexeme();
    token = lexer.nextToken();
    if(token.type() == Token::LPAREN){
      factorNode = funcall(lex);
    }
    else{
      lex = symboltable.getUniqueSymbol(lex);
      if(lex == ""){
	error("Factor: IDENT: "+lex+"is not defined");
	exit(1);
      }
      op = PUSHV;
      factorNode = new TreeNode(op,lex);
    }
  }
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
    case Token::LE: op = ISLE;
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

Parser::TreeNode* Parser::returnStatement(){
  TreeNode* logicalNode;
  TreeNode* returnNode;
  if(token.lexeme() == "return"){
    token = lexer.nextToken();
    logicalNode = logicalExpression();
  }
  check(Token::SEMICOLON,"Expecting ;");
  returnNode = new TreeNode(SEQ,logicalNode,new TreeNode(RET));
  return returnNode;
}

/*
  this is the part I wrote for the label of If and While statement in
  the case that there will be same label by adding the statement twice
  and more
 */
int lindex = 1;
string llabel(){
  string temp = "L";
  stringstream ss;
  ss << ++lindex;
  string res = ss.str();
  temp = temp + res;
  return temp;
}

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
    if(doit == true){
      symboltable.exitScope();
    }
    string label2 = llabel();
    //this is the method in lex to check whether there is next else
    int exam = lexer.elseCheck();
    if(exam == 1){
      token = lexer.nextToken();
      if(token.type() == Token::ELSE){
	string label3 = llabel();
	seqNode = new TreeNode(SEQ,logicalNode,new TreeNode(JUMPF,label3));
	node = new TreeNode(SEQ,blockNode,new TreeNode(JUMP,label2));
	seqNode = new TreeNode(SEQ,seqNode,node);
	token = lexer.nextToken();
	blockNode = block();
	if(doit == true){
	  symboltable.exitScope();
	}
	node = new TreeNode(SEQ,new TreeNode(LABEL,label3+":"),blockNode);
	seqNode = new TreeNode(SEQ,seqNode,node);
      }
    }
    else{
      seqNode = new TreeNode(SEQ,logicalNode,new TreeNode(JUMPF,label2));
      seqNode = new TreeNode(SEQ,seqNode,blockNode);
    }
    seqNode = new TreeNode(SEQ,seqNode,new TreeNode(LABEL,label2+":"));
  }
  return seqNode;
}

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
    if(doit == true){
      symboltable.exitScope();
    }
    string label = llabel();
    string label1 = llabel();
    seqNode = new TreeNode(SEQ,new TreeNode(LABEL,label+":"),logicalNode);
    node1 = new TreeNode(SEQ,new TreeNode(JUMPF,label1),blockNode);
    seqNode = new TreeNode(SEQ,seqNode,node1);
    node2 = new TreeNode(SEQ,new TreeNode(JUMP,label),new TreeNode(LABEL,label1+":"));
    seqNode = new TreeNode(SEQ,seqNode,node2);
  }
  return seqNode;
}

Parser::TreeNode* Parser::assignStatement(){
  TreeNode* logicalNode;
  TreeNode* assignNode;
  check(Token::IDENT,"Assingn: Expecting IDENT");
  string lex = symboltable.getUniqueSymbol(token.lexeme());
  if(lex == ""){
    error("Assign: Ident"+token.lexeme()+"is not defined");
  }
  token = lexer.nextToken();
  if(token.type() == Token::ASSIGN){
    TreeNode* ident = new TreeNode(STORE,lex);
    token = lexer.nextToken();
    logicalNode = logicalExpression();
    check(Token::SEMICOLON,"Expecting ;");
    assignNode = new TreeNode(SEQ,logicalNode,ident);
  }
  else if(token.type() == Token::LPAREN){
    assignNode = funcall(lex);
  }
  return assignNode;
}

Parser::TreeNode* Parser::vardefStatement(){
  TreeNode* vardefNode = NULL;
  check(Token::VAR,"Expecting VAR");
  token = lexer.nextToken();
  check(Token::IDENT,"Var: Expecting IDENT");
  symboltable.enterScope();
  doit = true;
  symboltable.addSymbol(token.lexeme());
  token = lexer.nextToken();
  while(token.type() == Token::COMMA){
    token = lexer.nextToken();
    check(Token::IDENT,"Var2: Expecting IDENT");
    symboltable.addSymbol(token.lexeme());
    token = lexer.nextToken();
  }
  check(Token::SEMICOLON,"Expecting ;");
  return vardefNode;
}

Parser::TreeNode* Parser::printfStatement(){
  TreeNode* paramList = NULL;
  int nparams = 0;
  check(Token::PRINTF,"Expecting printf");
  token = lexer.nextToken();
  check(Token::LPAREN,"Expecting (");
  token = lexer.nextToken();
  check(Token::STRINGLIT,"Expecting string literal");
  string formatString = token.lexeme();
  token = lexer.nextToken();
  if(token.type() == Token::COMMA){
    token = lexer.nextToken();
    paramList = expression();
    ++nparams;
    while(token.type() == Token::COMMA){
      token = lexer.nextToken();
      paramList = new TreeNode(SEQ,paramList,expression());
      ++nparams;
    }
  }
  else{
    check(Token::RPAREN,"Expecting )");
    token = lexer.nextToken();
    check(Token::SEMICOLON,"Expecting ;");
  }
  TreeNode* printfStatement = new TreeNode(SEQ,paramList,new TreeNode(PRINTF,itos(nparams)+formatString));
  return printfStatement;
}

Parser::TreeNode* Parser::statement(){
  TreeNode* stateNode = NULL;
  if(token.type() == Token::IF){
    stateNode = ifStatement();
  }
  else if(token.type() == Token::WHILE){
    stateNode = whileStatement();
  }
  else if(token.type() == Token::VAR){
    stateNode = vardefStatement();
  }
  else if(token.type() == Token::RETURN){
    stateNode = returnStatement();
  }
  else if(token.type() == Token::PRINTF){
    stateNode = printfStatement();
  }
  else if(token.type() == Token::IDENT){
    stateNode = assignStatement();
  }
  token = lexer.nextToken();
  return stateNode;
}

Parser::TreeNode* Parser::block(){
  doit = false;
  TreeNode* stateNode;
  check(Token::LBRACE,"Expecting {");
  token = lexer.nextToken();
  stateNode = statement();
  while(token.type() != Token::RBRACE){
    stateNode = new TreeNode(SEQ,stateNode,statement());
  }
  return stateNode;
}

Parser::TreeNode* Parser::parameterdef(Operation op,string lex){
  TreeNode* paraNode;
  paraNode = new TreeNode(op,lex);
  return paraNode;
}

Parser::TreeNode* Parser::parameterdefs(string functionName){
  TreeNode* paraNode;
  TreeNode* returnNode = NULL;
  string parameter[50];
  int index = 0;
  if(token.type() == Token::IDENT){
    string lex = token.lexeme();
    parameter[index] = lex;
    index++;
    token = lexer.nextToken();
  }
  else{
    returnNode = new TreeNode(FUNC,functionName+":");
    return returnNode;
  }
  while(token.type() == Token::COMMA){
    token = lexer.nextToken();
    string lex = token.lexeme();
    parameter[index] = lex;
    index++;
    token = lexer.nextToken();
  }
  if(index != 0){
    string para = parameter[index-1];
    int exam = symboltable.addSymbol(para);
    if(!exam) error("Variable "+token.lexeme()+" already defined in this scope");
    para = symboltable.getUniqueSymbol(para);
    paraNode = parameterdef(STORE,para);
    returnNode = new TreeNode(SEQ,new TreeNode(FUNC,functionName+":"),paraNode);
    for(int i=index-2;i>-1;i--){
      para = parameter[i];
      exam = symboltable.addSymbol(para);
      if(!exam) error("Variable "+token.lexeme()+" already defined in this scope");
      para = symboltable.getUniqueSymbol(para);
      paraNode = parameterdef(STORE,para);
      returnNode = new TreeNode(SEQ,returnNode,paraNode);
    }
  }
  return returnNode;
}

Parser::TreeNode* Parser::function(){
  TreeNode* functionNode = NULL;
  if(token.type() == Token::FUNCTION){
    token = lexer.nextToken();
    check(Token::IDENT,"Function: Expecting IDENT");
    string name = token.lexeme();
    token = lexer.nextToken();
    check(Token::LPAREN,"Expecting (");
    token = lexer.nextToken();
    symboltable.enterScope();
    functionNode = parameterdefs(name);
    check(Token::RPAREN,"Function: Expecting )");
    token = lexer.nextToken();
    TreeNode* blockNode = block();
    symboltable.exitScope();
    functionNode = new TreeNode(SEQ,functionNode,blockNode);
    token = lexer.nextToken();
  }
  return functionNode;
}

Parser::TreeNode* Parser::compilationunit(){
  TreeNode* funcNode = function();
  while(token.type() == Token::FUNCTION){
    funcNode = new TreeNode(SEQ,funcNode,function());
  }
  return funcNode;
}
