#include <vcc.h>

void main(){
  unsigned res = sum(10);
  _(assert res >= 10)
}

unsigned sum (unsigned n)
_(requires n <= 100)
_(ensures \result == (n * (n + 1)) / 2)
{
  if (n == 0)
    return 0;
  else
    return (n + sum(n-1));
}