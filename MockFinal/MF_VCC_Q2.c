#include <vcc.h>
#include <limits.h>

#define MAX 64

int bag[MAX];
unsigned len;
int maxv;

/*
 * Mock Final — VCC Q2
 *
 * Library intent (you may choose your own invariants/contracts):
 *  - len <= MAX
 *  - if len == 0, maxv == INT_MIN
 *  - if len > 0, all stored elements are <= maxv
 *
 * Goal: Prove the client assertion in main() using VCC, by adding:
 *  - contracts for bag_init, bag_add, bag_contains
 *  - loop invariants (library + client)
 *
 * Note: bag_contains currently has a small intentional defect.
 */

void bag_init()
  _(writes &len, &maxv)
  _(ensures len == 0 && maxv == INT_MIN)
{
  len = 0;
  maxv = INT_MIN;
}

void bag_add(int v)
  _(requires len < MAX)
  _(requires len == 0 || (\forall unsigned i; i < len ==> bag[i] <= maxv))
  _(writes &len, &maxv, &bag[len])
  _(ensures len == \old(len) + 1)
  _(ensures \forall unsigned i; i < len ==> bag[i] <= maxv)
{
  bag[len] = v;
  len++;

  if (len == 1)
    maxv = v;
  else if (v > maxv)
    maxv = v;
}

/* Returns 1 iff v is present in bag[0..len-1], else 0. */
int bag_contains(int v)
  _(writes &len, &maxv, \array_range(bag, MAX))
  _(requires len <= MAX)
  _(ensures len == \old(len) && maxv == \old(maxv)) //No values are modified by this function
  _(ensures \result == 1 <==> (\exists unsigned i; i < len && bag[i] == v)) //If the function returns 1, then there exists an index i such that bag[i] == v
  _(ensures \result == 0 <==> (\forall unsigned i; i < len ==> bag[i] != v)) //If the function returns 0, then for all indices i, bag[i] != v
{
  unsigned i = 0;

  while (i < len)
    _(invariant i <= len) //Loop invariant: i is always less than or equal to len
    _(invariant \forall unsigned j; j < i ==> bag[j] != v) //Loop invariant: For all indices j less than i, bag[j] is not equal to v
  {
    if (bag[i] == v)
      return 1;

    i++;
  }

  return 0;
}

int main()
  _(writes &len, &maxv, \array_range(bag, MAX))
{
  bag_init();

  unsigned i = 0;

  while (i < MAX)
    _(invariant i <= MAX)
    _(invariant len <= i)
    _(invariant len == 0 || (\forall unsigned j; j < len ==> bag[j] <= maxv))
  {
    int n; /* n is uninitialized (treat as nondet) */

    if (0 <= n && n <= 50)
      bag_add(n);

    i++;
  }

  int r = bag_contains(99);
  _(assert r == 0);

  return 0;
}
