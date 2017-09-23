#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <string.h>
#include "stack.cc"
#include "hashtable.h"

using namespace std;

class SymbolTable{
 public:
  SymbolTable();
  ~SymbolTable();

  void enterScope();
  void exitScope();
  int addSymbol(string sym);
  string getUniqueSymbol(string sym);

 private:
  Stack<HashTable> symtable;
  int index;
};
#endif
