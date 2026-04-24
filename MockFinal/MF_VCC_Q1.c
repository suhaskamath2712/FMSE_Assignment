#include <vcc.h>

/*
 * Mock Final — VCC Q1
 *
 * Part (a): The loop below uses a friend's proposed invariant: x < y.
 *          Write out the VCs (init/preserve/exit) and decide which are valid.
 *
 * Part (b): Replace the invariant with one that is inductive + adequate to
 *          prove the post-assertion, and verify in VCC.
 */

void main()
{
  int x = 0;
  int y = 0;

  while (x < 60)
    _(invariant y == 3 * x && x <= 60) /* friend invariant (likely incorrect) */
  {
    x = x + 1;
    y = y + 3;
  }

  _(assert y >= 180);
}
