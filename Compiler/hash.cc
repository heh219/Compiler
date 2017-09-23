#include "hash.h"

Hash::Hash(string str) {
  int temp = 0;
  for(int i=0; i<(int)str.length();++i) temp += str[i];
  if(temp < 0) key = -temp; else key = temp;
}

int Hash::hash() const{
  return key;
}
