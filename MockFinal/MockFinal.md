# FMSE Mock Final — Practice Paper (VCC + JPF + AFL)

This is a **practice** paper written to match the *style* and tool-usage patterns of **Assignment 4 (VCC)**, **Assignment 5 (JPF-Symbc)**, and **Assignment 6 (AFL)** in this workspace.

It is **not** an official exam.

**Recommended time:** 3 hours (practice)  
**Total marks (if you want to score yourself):** 130

---

## Starter files provided (in this folder)

### VCC (Assignment 4 style)
- `MF_VCC_Q1.c` — loop invariant + VC writing
- `MF_VCC_Q2.c` — library/client contracts + loop invariants
- `MF_VCC_Q3.c` — 6-bit 2’s complement counter refinement

### JPF-Symbc (Assignment 5 style)
- `MF_JPF_Q4_FindMinBug.java`, `MF_JPF_Q4_FindMinBug.jpf`
- `MF_JPF_Q5_NodeCycle.java`, `MF_JPF_Q5_NodeCycle.jpf`

### AFL (Assignment 6 style)
- `MF_AFL_Q6_count_xy.c` — flip1/flip2 exploration
- `MF_AFL_Q7_trap.c` — assertion crash target
- `MF_AFL_Q8_sanitize.c` — “silent” overflow-style bug target

---

## General rules (same spirit as A4–A6)

- **VCC**: you may add loop invariants and contracts using VCC annotations like `_(invariant ...)`, `_(requires ...)`, `_(ensures ...)`, `_(writes ...)`, plus ghost code where helpful.
- **JPF-Symbc**: use `gov.nasa.jpf.symbc.Debug.makeSymbolicInteger(...)` and the provided `.jpf` configs. Keep integer ranges small to keep state space manageable.
- **AFL**: use the **course-provided modified AFL** (`mafl-2.35b`) exactly as in Assignment 6.

---

## Q1 — Loop invariants + verification conditions (VCC) [15]

Work in `MF_VCC_Q1.c`.

1. The loop is annotated with your friend’s proposed invariant: `x < y`.
   - Write out the standard VCs for a while-loop invariant:
     - **Initialization VC** (invariant holds before first iteration)
     - **Preservation VC** (invariant preserved by the body)
     - **Exit VC** (invariant + loop exit implies post-assertion)
   - State which VC(s) are logically valid and which are not.

2. Replace the friend invariant with an invariant that is:
   - **inductive**, and
   - **adequate** to prove the post-assertion.

3. Verify the program in VCC.

Deliverable (practice): a short write-up of the VCs + the final annotated `MF_VCC_Q1.c`.

---

## Q2 — Contracts for a small “bag” library (VCC) [25]

Work in `MF_VCC_Q2.c`.

You are given a small library that stores integers in a global array and tracks a summary value `maxv`. A client inserts some (nondet) values that satisfy a guard, and then searches for a value that should be absent.

Task:

1. Add **contracts** to `bag_init`, `bag_add`, and `bag_contains` that are sufficient to prove the client assertion.
   - Like Assignment 4 Q2: contracts can be **partial** (only strong enough for this client).

2. Add loop invariants in both the **library** and the **client** as needed.

3. The library contains a small intentional defect that blocks verification (and even basic termination). Fix it in the most minimal way.

Goal assertion (in client): `_(assert r == 0)`.

Deliverable (practice): final annotated `MF_VCC_Q2.c`.

---

## Q3 — 6-bit 2’s complement counter refinement (VCC) [20]

Work in `MF_VCC_Q3.c`.

A 6-bit two’s complement scheme stores an unsigned value $u \in [0,63]$ to represent signed integers in $[-32,31]$:

- if $u \in [0,31]$, it represents the same non-negative integer
- if $u \in [32,63]$, it represents a negative integer with value $u - 64$

You are given `init` and `inc` on the concrete representation; `dec` is missing.

Tasks:

1. Implement the concrete `dec()`.
2. Add a ghost `\integer` (e.g., `ival`) for the abstract counter.
3. State:
   - a concrete invariant (range constraint on `u`),
   - an abstract invariant (range constraint on `ival`), and
   - a **gluing relation** between `u` and `ival`.
4. Provide *combined* operations (Rodin-style refinement sketch): each operation should update both the concrete state and the ghost state, and you should prove invariants + glue are preserved.

Deliverable (practice): final annotated `MF_VCC_Q3.c`.

---

## Q4 — Symbolic execution on a buggy array algorithm (JPF-Symbc) [10]

Work in `MF_JPF_Q4_FindMinBug.java` and run with `MF_JPF_Q4_FindMinBug.jpf`.

1. Fill in the TODO to assert that the returned value `r` is the minimum of the array.
2. Run JPF-Symbc to obtain a **counterexample** (concrete values for `a0`, `a1`, `a2`) that violates the property.
3. Explain (1–2 lines) the bug and then **fix** it.
4. Re-run JPF to show the property holds after the fix.

Deliverable (practice):
- the counterexample values and brief explanation,
- the fixed `MF_JPF_Q4_FindMinBug.java`.

---

## Q5 — Lazy initialization + heap counterexample (JPF-Symbc) [10]

Work in `MF_JPF_Q5_NodeCycle.java` and run with `MF_JPF_Q5_NodeCycle.jpf`.

The file contains:
- a **buggy** bounded cycle check (`hasCycleBoundedBug`)
- a bounded “spec” predicate (`repeatsWithin`) that detects repeated nodes within a finite number of steps

Tasks:

1. Add an assertion in `checkSoundness` for the soundness property:

$$
\texttt{hasCycleBoundedBug(head, k)} \Rightarrow \texttt{repeatsWithin(head, k)}
$$

2. Run JPF (with lazy init) to get a **heap-shape counterexample** (describe the smallest list shape that triggers failure).
3. Fix the bug in the cycle checker and re-run JPF to confirm the property.

Deliverable (practice): the counterexample shape + the fixed program.

---

## Q6 — AFL: locate flip1/flip2 queue entries (AFL) [10]

Target program: `MF_AFL_Q6_count_xy.c`.

1. Create a **2-byte seed** exactly (no newline). Use the first two letters of your IISc email ID (or any 2 characters for practice):

- `echo -n "xx" > seed`

2. Fuzz the target using the **modified** AFL (as in Assignment 6).
3. Stop once you have **at least two** queue entries created via `flip1` or `flip2` (or stop after 10 minutes).
4. Zip the entire AFL output directory as `output-countxy.zip`.
5. In a short report `problem6.pdf`, list:
   - the testcase IDs for all `flip1`/`flip2`-generated queue files (or any two if there are many)
   - for each, the flipped bit position(s) relative to its parent

Bit numbering rule (same as A6): the **leftmost bit of the whole input** is position 0 (MSB-first per byte).

---

## Q7 — AFL: crash discovery + queue graph (AFL) [20]

Target program: `MF_AFL_Q7_trap.c`.

1. Create a 1-byte seed file containing only `a` (no newline).
2. Run AFL until it finds the assertion violation.
   - Set `AFL_BENCH_UNTIL_CRASH=1` before fuzzing.

Write everything into `problem7.pdf`:

a) For each file in `queue/` and `crashes/` (ignore README files), list:
   - filename
   - contents, showing each byte as **two hex digits**

b) From the AFL screen, record:
   - cycles done
   - levels
   - total generated/retained inputs (paths)
   - number of inputs retained per fuzzing stage (stage finds)

Include:
- a screenshot of the AFL UI at termination
- the full contents of `fuzzer_stats`

c) Draw the directed parent→child graph over queue testcases (`src:` → `id:`).

d) Add a special node `crash` and edges from the parent(s) of crash inputs.

e) Write one observation about the graph (tree-like? any splicing? where crash occurs?).

---

## Q8 — AFL: silent overflow vs asserted overflow (AFL) [20]

Target program: `MF_AFL_Q8_sanitize.c`.

1. Create a 2-byte seed as in Q6.
2. Run afl-fuzz for **10 minutes**. Zip the output directory as `output-woa.zip`.
3. Create `MF_AFL_Q8_sanitize_assert.c` by adding `assert(...)` checks *just before each write* into the fixed-size buffer.
   - The asserts should catch buffer overflow *before* it happens.
4. Identify inputs in the **original run’s** `queue/` that violate the new asserts but are **not** present in the original `crashes/` folder.
5. Briefly explain (2 lines) why they were not classified as crashes in the original run.
6. Fuzz the assert version for **10 minutes** and zip its output directory as `output-assert.zip`.

Write parts (4)–(6) into `problem8.pdf`.

---

## (Optional) How to keep things tidy

If you want, create a subfolder `MockFinal/your_solutions/` and keep all zips/pdfs there so your practice artifacts don’t mix with the starter files.
