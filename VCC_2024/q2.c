#include <vcc.h>

int max3(int x, int y, int z)
    _(ensures \result >= x && \result >= y && \result >= z)
    _(ensures \result == x || \result == y || \result == z)
{
  if (x <= y)
    if (y <= z)
      return z;
    else
      return y;
  else
    if (x <= z)
      return z;
    else
      return x;
}