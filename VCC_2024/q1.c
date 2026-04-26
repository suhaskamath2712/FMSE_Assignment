#include <vcc.h>

unsigned mult (unsigned n, unsigned m)
_(requires n <= 100 && m <= 100)
_(ensures \result == n*m)
{
  unsigned a = 0;
  unsigned x = 0;
  while (x < m) 
  _(invariant a == n*x && x <= m)
  {
    a = a + n;
    x = x + 1;
  }
  return a;
}