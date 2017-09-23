#include "symboltable.h"
#include <iostream>
#include <sstream>

string itos(int i){ stringstream ss; ss << i; string res = ss.str(); return res;}

SymbolTable::SymbolTable():index(1){
}

SymbolTable::~SymbolTable(){}

void SymbolTable::enterScope(){
  symtable.push(HashTable(50));
}

void SymbolTable::exitScope(){
  symtable.pop();
}

int SymbolTable::addSymbol(string sym){
  HashTable topscope = symtable.peek();
  if(topscope.contains(sym)){
    return 0;
  }
  symtable.peek().put(sym,sym+"$"+itos(index++));
  return 1;
}

string SymbolTable::getUniqueSymbol(string sym){
  for(int i=symtable.tos;i>=0;--i){
    HashTable& scope = symtable[i];
    if(scope.contains(sym)){
      return scope.get(sym);
    }
  }
  return "";
}
