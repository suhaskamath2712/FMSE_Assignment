#include <vcc.h>

/*
 * Mock Final — VCC Q3
 *
 * Concrete state:
 *   unsigned int u;  // intended range: 0..63 (6-bit)
 *
 * Interpretation (6-bit 2's complement):
 *   u in 0..31  encodes  0..31
 *   u in 32..63 encodes -32..-1 via (u - 64)
 *
 * Tasks:
 *  1) Implement dec() on the concrete representation.
 *  2) Introduce a ghost \integer (e.g., ival) for the abstract counter.
 *  3) State invariants + a gluing relation, and verify init/inc/dec preserve them.
 *
 * Gluing relation (aka abstraction relation): a predicate that “glues” the concrete state
 * (the machine-level representation, here the 6-bit unsigned value u) to the abstract,
 * specification-level state (a ghost integer like ival).  In this problem the gluing
 * relation captures the 6-bit two’s-complement decoding, e.g. require 0 <= u <= 63 and
 * -32 <= ival <= 31 and then define ival == (u < 32 ? u : u - 64).  By making this
 * relation an invariant, each operation only needs to show its concrete update to u is
 * consistent with the intended abstract update to ival (including wraparound), so proofs
 * can be carried out at the abstract level while still talking about the real bits.
 */

unsigned int u;

_(ghost \integer val)

#define INV (u <= 63 && val >= -32 && val <= 31)
#define GLUE (val == (u < 32 ? u : u - 64))

void init()
  _(writes &u, &val)
  _(requires INV && GLUE)
  _(ensures INV && GLUE)
  _(ensures u == 0 && val == 0)
{
  u = 0;
  _(ghost val = 0;)
}

void inc()
  _(writes &u, &val)
  _(requires INV && GLUE)
  _(ensures INV && GLUE)
  _(ensures u == \old(u) + 1 || (u == 0 && \old(u) == 63))
{
  if (u == 63)
    u = 0;
  else
    u++;

  _(ghost val = (val == 31) ? -32 : val + 1;)
}

void dec()
  _(writes &u, &val)
  _(requires INV && GLUE)
  _(ensures INV && GLUE)
  _(ensures u == \old(u) - 1 || (u == 63 && \old(u) == 0))
{
  if (u == 0)
    u = 63;
  else
    u--;
  _(ghost val = (val == -32) ? 31 : val - 1;)
}
