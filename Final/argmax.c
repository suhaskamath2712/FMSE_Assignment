#include<vcc.h>

unsigned int argmax(int A[], unsigned int len)
  _(requires \thread_local_array(A,len))
  _(requires len > 0)
  _(requires \forall unsigned j; j < len ==> A[j] < A[0])
  _(ensures \result == 0)
{
  unsigned int i, pos = 0;
  for (i = 0; i < len; i++)
    _(invariant i <= len && pos < len)
  {
    if (A[i] > A[pos])
      pos = i;
  }
  return pos;
}
