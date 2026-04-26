#include <vcc.h>

void sumcopy(unsigned A[10], unsigned B[10], unsigned C[10])
  _(requires \thread_local_array(A, 10))
  _(requires \thread_local_array(B, 10))
  _(requires \thread_local_array(C, 10))
  _(writes \array_range(C, 10))
  _(requires \forall unsigned k; k < 10 ==> A[k] < 100)
  _(requires \forall unsigned k; k < 10 ==> B[k] < 100)
  _(requires \disjoint(\array_range(A, 10), \array_range(C, 10)))
  _(requires \disjoint(\array_range(B, 10), \array_range(C, 10)))
  _(ensures \forall unsigned k; (0 <= k && k < 10) ==> C[k] >= A[k])
  _(ensures \forall unsigned k; (0 <= k && k < 10) ==> C[k] >= B[k])
{
    unsigned i;
    for (i = 0; i < 10; i++)
        _(invariant 0 <= i && i <= 10)
        _(invariant \forall unsigned k; (0 <= k && k < i) ==> C[k] == A[k] + B[k])
        _(invariant \forall unsigned k; (0 <= k && k < i) ==> C[k] >= A[k])
        _(invariant \forall unsigned k; (0 <= k && k < i) ==> C[k] >= B[k])
    {
        C[i] = A[i] + B[i];
    }
}