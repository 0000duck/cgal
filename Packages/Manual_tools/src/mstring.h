// mstring.h
// --------------------------------------------------

// Old mstring.h in mstring_old.h
// We assume a standard conform string class and include it here.
// If it is not available, one may try the mstring_old.h again.

#ifndef MY_STRING_H
#define MY_STRING_H 1

#include <string>
using namespace std;

// provide hash function for string class of egcs 1.1.1
#ifdef __GNUC__
#include <hashtable.h>

struct hash<string>
{
  size_t operator()(const string& str) const
  {
    unsigned long h = 0;
    const char* s = str.data();
    for (size_t len = str.length(); len > 0; --len, ++s)
      h = 5*h + (unsigned long)(*s);
    return size_t(h);
  }
};
#endif

#endif // MY_STRING_H //

// EOF //
