#include<vcc.h>

void main() {
  unsigned int p;
  unsigned int j = 0;

  _(assume 10 < p && p < 70)
  unsigned int i = 0;
  while (i < 100)
    _(invariant 0 <= i && i <= 100)
    _(invariant (i >= 100 ==> j == 1))
  {
    if (i >= p)
      j = 1;
    i++;
  }
  _(assert (j == 1))
}
