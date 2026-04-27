# Running the tools for Assignments 4–6

This repo contains solutions for:

- **Assignment 4**: VCC (C verification)
- **Assignment 5**: Java PathFinder (JPF) + Symbolic PathFinder (SPF / `jpf-symbc`)
- **Assignment 6**: AFL (course-provided **modified** AFL: `mafl-2.35b`)

All paths below assume the workspace root is:

- `/home/suhas/Desktop/FMSE_Assignment`

---

## Assignment 4 — VCC (Verified C Compiler)

### What to run

VCC verifies the annotated C sources in `Assignment 4/`:

- `Assignment 4/q1b.c`
- `Assignment 4/q2.c`
- `Assignment 4/q3.c`

All three use VCC annotations via:

- `#include <vcc.h>`

### How to run (verification)

1. Open a shell in the **VCC environment** (course VM / Windows setup / or wherever `vcc` is installed).
2. Change into the Assignment 4 folder:
  - `cd "/home/suhas/Desktop/FMSE_Assignment/Assignment 4"`
3. Run VCC on each file:

- Verify a single file (repeat for each `q*.c`):
  - `vcc q1b.c`

- Verify all in one go:
  - `vcc q1b.c q2.c q3.c`

If you’re unsure about the exact command-line switches supported by your VCC build, check:

- `vcc /help`

### “Options” used in the assignment (inside the code)

The Assignment 4 solutions rely on VCC’s specification/annotation features:

- Loop invariants: `_(invariant ...)`
- Function contracts:
  - preconditions: `_(requires ...)`
  - postconditions: `_(ensures ...)`
  - frame conditions: `_(writes ...)`
- Quantifiers: `\forall`
- Old values: `\old(e)`
- Return value: `\result`
- Array framing helpers: `\array_range(arr, n)`
- Ghost state:
  - declaration: `_(ghost \integer intval)`
  - assignments: `_(ghost ... ;)`

Files where these show up:

- `q1b.c`: loop invariant proving the post-assert
- `q2.c`: contracts for `listinit`, `listinsert`, `listsearch` + invariants in loops
- `q3.c`: ghost integer `intval`, invariant `INV`, gluing relation `GLUE`, refinement-style reasoning

---

## Assignment 5 — JPF + SPF (symbolic execution)

### What to run

This assignment is configured using JPF “.jpf” config files:

- `Assignment 5/FindMin.jpf` (runs `FindMin`)
- `Assignment 5/NodeSimple.jpf` (runs `NodeSimple`)

### How to run

From the `Assignment 5/` directory, run JPF on the config file:

- `jpf FindMin.jpf`
- `jpf NodeSimple.jpf`

If you want to save the exploration tree / console log to a file (as in this repo’s `*.out` files), redirect stdout:

- `jpf FindMin.jpf > findmin-tree.out`
- `jpf NodeSimple.jpf > node-tree.out`

If the `jpf` launcher script is not on your `PATH`, use the JPF jar launcher instead (paths depend on your installation):

- `java -jar <JPF_CORE>/build/RunJPF.jar FindMin.jpf`

### SPF/JPF options used (from the `.jpf` configs)

Both `.jpf` files set:

- `classpath=${config_path}` and `sourcepath=${config_path}`
  - Meaning: *the directory containing the `.jpf` is used as the classpath/sourcepath.*
- `target = <ClassName>`
  - Entry point class (`main()` is the default entry).
- `listener = gov.nasa.jpf.symbc.SymbolicListener`
  - Enables SPF symbolic execution.
- `listener+=,gov.nasa.jpf.listener.ExecTracker`
  - Prints executed source lines (configured via the `et.*` options).
- `symbolic.debug = true`
  - Turns on verbose symbolic debugging output (constraints, path conditions, etc.).
- `et.print_src=true`, `et.print_insn=false`, `et.print_mth=true`
  - Print source lines + method names, suppress bytecode.
- `vm.storage.class=nil`
  - Disables state storage (keeps the run simpler/lighter for these tasks).

#### FindMin configuration (`FindMin.jpf`)

Key SPF options used:

- `symbolic.method = FindMin.findMin(con)`
  - Marks `FindMin.findMin(...)` for symbolic execution.
  - `con` means the **parameter is treated as concrete** (here: the array object is concrete, but its elements can still be symbolic via `Debug.makeSymbolicInteger`).
- `search.multiple_errors=true`
  - Continue searching to report multiple assertion errors (useful when `assert` may fail).
- `symbolic.min_int=-10` and `symbolic.max_int=10`
  - Restrict integer domain for symbolic variables (keeps the path explosion manageable).

#### NodeSimple configuration (`NodeSimple.jpf`)

Key SPF options used:

- `symbolic.method = NodeSimple.hasCycleBounded(sym#con)`
  - First argument (`head`) is **symbolic** (`sym`) so SPF explores different heap shapes.
  - Second argument (`maxSteps`) is **concrete** (`con`).
  - The `sym#con` syntax matches the method parameters left-to-right.
- `symbolic.lazy = true`
  - Enables **lazy initialization** of heap fields (`next` pointers), which is essential for exploring linked structures.
- (Optional, commented in the file): `symbolic.lazy.subtypes = true`
  - Use this when inheritance/subtyping matters for lazy object creation.

### Where the relevant code is

- `Assignment 5/FindMin.java`
  - Uses `Debug.makeSymbolicInteger("m0")`, `Debug.makeSymbolicInteger("m1")`
  - Contains an `assert` that checks the returned min is $\le$ every element.

- `Assignment 5/NodeSimple.java`
  - Floyd cycle detection with a bound (`maxSteps`).

---

## Assignment 6 — AFL (course `mafl-2.35b`)

### One-time setup / environment

This repo includes a helper script:

- `Assignment 6/exports.sh`

In a terminal, you can load the same environment variables used during the run via:

- `source /home/suhas/Desktop/FMSE_Assignment/Assignment\ 6/exports.sh`

Those variables cover the “options” required/used in the assignment:

- `AFL_PATH=/home/suhas/Downloads/AFL/mafl-2.35b/`
  - Must point to the course-provided `mafl-2.35b/` directory on your machine.
- `AFL_KEEP_ASSEMBLY=1`
  - Keeps the hidden `.afl-<pid>-<timestamp>.s` assembly artifact.
- `TMPDIR=.`
  - Keeps temp files in the current directory (so `random-loc.txt` and assembly land next to the build).
- `AFL_SKIP_CPUFREQ=1`
  - Skips AFL CPU scaling/governor checks (useful if you can’t change governor).
- `AFL_I_DONT_CARE_ABOUT_MISSING_CRASHES=1` (optional)
  - Only use if AFL complains; it may affect how hangs/crashes are reported.

If `mafl-2.35b` isn’t built yet, build it where it lives (outside this repo, in your setup):

- `cd $AFL_PATH && make`

### AFL command shape (what the flags mean)

All assignment runs follow the same pattern:

- `afl-fuzz -i <input_dir> -o <output_dir> -- <program_under_test>`

Where:

- `-i` is the seed corpus directory
- `-o` is the AFL output directory (contains `queue/`, `crashes/`, `hangs/`, `fuzzer_stats`, ...)
- `--` separates afl-fuzz options from the target program

If a program reads **a filename argument** instead of stdin, use `@@`:

- `... -- ./program @@`

Practical note: AFL generally expects the output directory (`-o`) to be **fresh**.
Since this repo already contains prior runs (e.g., `op_dir/`, `assert_op_dir/`), either:

- delete/rename the old output directory before re-running, or
- choose a new output directory name (e.g., `op_dir_rerun/`).

### Problem 1 (for-class-while)

Location:

- `Assignment 6/problem1/`

Inputs/outputs in this repo:

- seed input dir: `ip_dir/` (contains `seed`)
- AFL output dir: `op_dir/`
- instrumented binary: `for-class-while`

Seed creation (exactly **2 bytes**, no newline):

- `echo -n "xx" > ip_dir/seed`  (replace `xx` with the **last two digits** of your IISc registration number)
- verify: `od -cb ip_dir/seed`

Compile with course AFL wrapper (if rebuilding):

- `$AFL_PATH/afl-gcc for-class-while.c -o for-class-while`

Run AFL and stop after **20 cycles** (per the spec):

- `$AFL_PATH/afl-fuzz -i ip_dir/ -o op_dir/ -- ./for-class-while`

The exact command used in this repo run is recorded in:

- `Assignment 6/problem1/op_dir/fuzzer_stats` (`command_line` field)

### Problem 2 (assertion crash program)

Location:

- `Assignment 6/problem2/`

Inputs/outputs:

- seed input dir: `ip_dir/` (seed is a single byte: `"a"`)
- AFL output dir: `op_dir/`
- instrumented binary: `problem2`

Seed creation:

- `echo -n "a" > ip_dir/seed`

Compile (if rebuilding):

- `$AFL_PATH/afl-gcc problem2.c -o problem2`

Run until the assertion is found, using the assignment-required option:

- `export AFL_BENCH_UNTIL_CRASH=1`
- `$AFL_PATH/afl-fuzz -i ip_dir/ -o op_dir/ -- ./problem2`

### Problem 3 (crack-addr, with and without asserts)

Location:

- `Assignment 6/problem3/`

Inputs/outputs:

- seed input dir: `ip_dir/`
- AFL output dir (without asserts): `op_dir/`
- AFL output dir (with asserts): `assert_op_dir/`
- binaries in this repo: `crack-addr.o` and `crack-addr-assert.o`

Compile without-assert binary (if rebuilding):

- `$AFL_PATH/afl-gcc crack-addr.c -o crack-addr.o`

Run AFL for **10 minutes** (as required) using Linux `timeout` so AFL exits cleanly:

- `timeout --signal=INT 10m $AFL_PATH/afl-fuzz -i ip_dir/ -o op_dir/ -- ./crack-addr.o`

Create the assert version:

- edit/copy to `crack-addr-assert.c` and add `assert(...)` before each array access

Compile assert version:

- `$AFL_PATH/afl-gcc crack-addr-assert.c -o crack-addr-assert.o`

Run AFL on assert build (also commonly done with a 10-minute timeout):

- `timeout --signal=INT 10m $AFL_PATH/afl-fuzz -i ip_dir/ -o assert_op_dir/ -- ./crack-addr-assert.o`

### Packaging (zip) as used in the assignment

AFL outputs are submitted by zipping the **whole output directory** (the directory that contains `queue/`, `hangs/`, etc.). Examples already present in the repo:

- `Assignment 6/problem1/output-fcw.zip` (zipped `op_dir/`)
- `Assignment 6/problem3/output-woa.zip` (zipped `op_dir/`)
- `Assignment 6/problem3/output-assert.zip` (zipped `assert_op_dir/`)

VCC Command: alias vcc='WINEDEBUG=-all wine /opt/VCC/vcc/Host/bin/Release/vcc.exe'
