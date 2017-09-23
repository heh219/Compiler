#ifndef LINK__H
#define LINK_H
#include <string>
using namespace std;

class Link{
public:
  const string key;
  string data;
  Link *next;

  Link(const string key, string data, Link *next);
};
#endif
