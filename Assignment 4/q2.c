//Suhas Kamath
//25945

#include <vcc.h>
#include <limits.h>

#define MAX 100

int list[MAX];
unsigned len;
int min;

void listinit()
    _(writes &len, &min)
    _(ensures len == 0)
{
  len = 0;
  min = INT_MAX;
}

void listinsert(int val)
    _(requires len < MAX)
    _(requires len == 0 || (\forall unsigned i; i < len ==> list[i] >= min))
    _(writes &len, &min, &list[len])
    _(ensures len == \old(len) + 1)
    _(ensures \forall unsigned i; i < len ==> list[i] >= min)
    _(ensures (val >= 0 && (\old(len) == 0 || \old(min) >= 0)) ==> min >= 0)
{
  list[len] = val;
  len++;
  if (len == 1)
    min = val;
  else if (val < min)
    min = val;
}  

int listsearch(int val)
    _(writes &len, &min, \array_range(list, MAX))
    _(requires len <= MAX)
    _(requires len == 0 || (\forall unsigned i; i < len ==> list[i] >= min))
    _(ensures len == \old(len))
    _(ensures min == \old(min))
    _(ensures (len > 0 && val < min) ==> \result == 0)
    _(ensures len == 0 ==> \result == 0)
{
  unsigned i = 0;

  while (i < len)
    _(invariant i <= len)
    _(invariant len == 0 || (\forall unsigned j; j < len ==> list[j] >= min))
  {
    if (list[i] == val)
        return 1;

    //Without this increment, the loop would be infinite
    i++;
  }
  return 0;
}

int main()
    _(writes &len, &min, \array_range(list, MAX))
{
  listinit();

  unsigned i = 0;
     
  while (i < MAX)
    _(invariant i <= MAX)
    _(invariant len <= i)
    _(invariant len == 0 || min >= 0)
    _(invariant len == 0 || (\forall unsigned j; j < len ==> list[j] >= min))
  {
    int n;  // n is uninitialized
    if (n >= 0)
      listinsert(n);
    i++;
  }
    
  int ret = listsearch(-13);
  _(assert ret == 0);
  return 0;
}