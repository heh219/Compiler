#include "hashtable.h"

HashTable::HashTable(int n):size(n){
  table = new Link*[n];
  for(int j=0; j<size;j++)
    table[j] = NULL;
}

HashTable::HashTable():size(50){
  table = new Link*[50];
  for(int j=0;j<size;j++)
    table[j] = NULL;
}

HashTable::HashTable(const HashTable& t):size(t.size){
  table = new Link*[size];
  //check(table != NULL, "(HashTable(HashTable)) Heap overflow");
  for(int j = 0; j < size; j++)
    table[j] = t.table[j];
}

HashTable::~HashTable(){
  /* for(int j = 0; j < size; j++)
    if(table[j] != NULL)
      delete table[j];
      delete [] table;*/
}

HashTable& HashTable::put(string keyp, string value){
  int h;
  //check(!contains(keyp),"(add()) Duplicate key");
  h = Hash(keyp).hash() % size;
  table[h] = new Link(keyp, value, table[h]);
  //check(table[h] != NULL, "(add()) Heap overflow");
  return *this;
}

bool HashTable::contains(string keyp) const{
  int h;
  Link *temp;
  h = Hash(keyp).hash() % size;
  temp = table[h];
  while(temp != NULL && temp->key.compare(keyp) != 0)
    temp = temp->next;
  return temp != NULL;
}

const string HashTable::get(string keyp) const{
  int h;
  Link *temp;
  if(contains(keyp)){
    h = Hash(keyp).hash() % size;
    temp = table[h];
    while(temp != NULL && temp->key.compare(keyp) != 0)
      temp = temp->next;
    return temp->data;
  }
  else{
    return "string is not in the list";
  }
}

void HashTable::remove(string keyp){
  int h;
  Link *tempa, *tempb;
  //check(contains(keyp),"(remove()) Missing key");
  h = Hash(keyp).hash() % size;
  if(table[h]->key.compare(keyp) == 0){
    tempa = table[h];
    table[h] = table[h]->next;
  }
  else{
    tempb = table[h];
    while(tempb->next->key.compare(keyp) == 0)
      tempb = tempb->next;
    tempa = tempb->next;
    tempb->next = tempb->next->next;
  }
  delete tempa;
}
