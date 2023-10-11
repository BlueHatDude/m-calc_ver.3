#include "mcalc3.h"
#include <string.h>


#include <stdio.h>
int main(const int argc, const char* argv[]) {
  for(unsigned i = 1; i < (unsigned) argc; ++i) {
    printf("%d: %s \n", i, argv[i]);
  }
}

