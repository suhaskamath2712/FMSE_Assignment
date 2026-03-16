//Suhas Kamath
//25945

#include <vcc.h>

//y = 2x  ^ x <= 100 is the invariant
//It is valid is before the loop:
//  true ^ (x = 0 ^ y = 0) ==> y = 2x ^ x <= 100 evaluates true
//It is valid during the loop:
//  y = 2x ^ x < 100 ==> y = 2x ^ x <= 100 evaluates true
//It is valid after the loop:
//  y = 2x ^ x >= 100 ==> y = 2x ^ x <= 100 evaluates true

void main()
{
  int x = 0;
  int y = 0;
  while (x < 100)
    _(invariant y == 2 * x && x <= 100)
  {
    x = x + 1;
    y = y + 2;
  }
  _(assert y >= 102);
}