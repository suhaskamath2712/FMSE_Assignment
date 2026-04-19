# AFL Assignment 2026 — extremely detailed step-by-step (Linux, your workspace)

This guide is **customized to your current workspace on Linux**:

- Workspace root: `/home/suhas/Downloads/AFL`
- Course-provided AFL tools: `/home/suhas/Downloads/AFL/mafl-2.35b/`
- Example target (Problem 1): `/home/suhas/Downloads/AFL/ForClassWhile/for-class-while.c`

It covers **exactly what to do**, what files to create, which commands to run, what to verify, and what to submit.

> You do **not** need to run anything while reading this file. When you’re ready to solve the assignment, follow the steps and run the commands in a Linux terminal.

---

## 0) One-time setup (do this before any problem)

### 0.1) Confirm you are using the provided AFL (not system AFL)

The assignment requires the modified AFL in `mafl-2.35b/`. Your “golden rule”:

- **Compile targets with**: `.../mafl-2.35b/afl-gcc`
- **Fuzz targets with**: `.../mafl-2.35b/afl-fuzz`
- **Show coverage with**: `.../mafl-2.35b/afl-showmap`

Avoid running `afl-fuzz` or `afl-gcc` from `/usr/bin` (system-installed AFL), because:
- the course toolchain is slightly modified, and
- grading expectations usually assume the provided behavior/output.

### 0.2) Build AFL (if not already built)

If any AFL tools in `mafl-2.35b/` are missing or not executable, build them:

1. `cd /home/suhas/Downloads/AFL/mafl-2.35b`
2. Run `make`

After a successful build you should have (at least):
- `mafl-2.35b/afl-gcc`
- `mafl-2.35b/afl-fuzz`
- `mafl-2.35b/afl-showmap`

### 0.3) Export required environment variables **before compiling** with `afl-gcc`

The provided `README.txt` (course instructions) requires you to set these variables in the **same terminal** where you will compile:

- `AFL_PATH` — absolute path to the `mafl-2.35b/` directory
- `AFL_KEEP_ASSEMBLY=1` — keep the generated assembly file (a hidden `.afl-...s` file)
- `TMPDIR=.` — keep temp files in the current directory

Recommended setup for your environment:

- Set:
  - `export AFL_PATH=/home/suhas/Downloads/AFL/mafl-2.35b/`
  - `export AFL_KEEP_ASSEMBLY=1`
  - `export TMPDIR=.`

Notes:
- Keep the trailing `/` in `AFL_PATH` (matches the course README style).
- The hidden assembly file will appear in **the directory where you run `afl-gcc`**.
- `afl-gcc` also emits `random-loc.txt` in that directory.

### 0.4) CPU frequency requirement before fuzzing

AFL prefers “performance” CPU governor; otherwise it may warn.

**Preferred (best performance):**
- Set all cores to performance governor using `cpufreq-set` (requires sudo).

**If you cannot change governors:**
- In the same terminal where you will run `afl-fuzz`, set:
  - `export AFL_SKIP_CPUFREQ=1`

Optional (only if AFL insists / your run behaves oddly):
- `export AFL_I_DONT_CARE_ABOUT_MISSING_CRASHES=1`
  - This can move some “crashes” into `hangs/`, so only use it if needed.

### 0.5) Keep runs separate (recommended folder conventions)

To avoid overwriting outputs (and to keep zips clean), use separate in/out directories per problem.

A suggested layout inside the workspace:

- Problem 1 (in `ForClassWhile/`):
  - `ForClassWhile/fcwIn/` (input seeds)
  - `ForClassWhile/fcwOut/` (AFL output)

- Problem 2 (create a new folder under workspace root):
  - `Problem2/p2In/`, `Problem2/p2Out/`

- Problem 3 (create a new folder under workspace root):
  - `Problem3/p3In/`, `Problem3/p3Out-woa/`, `Problem3/p3Out-assert/`

This makes it easy to zip exactly what the assignment expects.

---

## 1) Problem 1 (for-class-while.c)

### What you must submit for Problem 1

You will create:

1. `output-fcw.zip` — zip of the entire AFL output directory from your run
2. `problem1.pdf` — answers for part (b)
3. `problem1.zip` containing `output-fcw.zip` and `problem1.pdf`

### 1.1) Create the seed file (exactly 2 bytes)

The assignment says:

- Create a seed file using:
  - `echo -n "xx" > seed`
- Replace `xx` with the **last two digits of your IISc student registration number**.

Do this in a fresh input directory.

**Steps:**

1. Choose your last-two-digits string. Example format:
   - If your last two digits are 4 and 7, use `"47"`.

2. Create the input directory (example):
   - `ForClassWhile/fcwIn/`

3. Create the seed file inside it:
   - `cd /home/suhas/Downloads/AFL/ForClassWhile/fcwIn`
   - `echo -n "<LAST_TWO_DIGITS>" > seed`

4. Verify the seed file is **exactly 2 bytes** and has no newline:

   - Run `od -cb seed`
     - You should see **exactly two characters** (two bytes).
   - Also check size:
     - `wc -c seed` should print `2`.

> If you accidentally used `echo "47" > seed` (without `-n`), it will add a newline and the file becomes 3 bytes. That is **wrong** for this assignment.

### 1.2) Compile `for-class-while.c` with the course `afl-gcc`

Target source is already in your workspace:

- `/home/suhas/Downloads/AFL/ForClassWhile/for-class-while.c`

This program reads from **stdin** (via `fgets`) and counts `a` and `b`.

**Steps:**

1. Open a terminal.
2. Export the compilation environment variables (see §0.3):
   - `export AFL_PATH=/home/suhas/Downloads/AFL/mafl-2.35b/`
   - `export AFL_KEEP_ASSEMBLY=1`
   - `export TMPDIR=.`

3. Move to the build directory:
   - `cd /home/suhas/Downloads/AFL/ForClassWhile`

4. Compile using the provided wrapper:
   - `/home/suhas/Downloads/AFL/mafl-2.35b/afl-gcc for-class-while.c -o for-class-while`

5. Verify artifacts were produced in `ForClassWhile/`:
   - A hidden assembly file like `.afl-<pid>-<timestamp>.s`
   - `random-loc.txt` (may overwrite the existing one)
   - The instrumented binary `for-class-while`

6. Quick sanity run (optional but recommended):
   - `./for-class-while < fcwIn/seed`
   - You should get an exit status equal to the count of `a` + `b` in the input.

### 1.3) Run AFL fuzzing and stop after **20 cycles**

You need to run `afl-fuzz` on the instrumented binary with:

- Input dir: your `fcwIn/` containing the 2-byte seed
- Output dir: a fresh directory (AFL writes `queue/`, `hangs/`, `crashes/`, etc.)

**Steps:**

1. From `ForClassWhile/`, ensure input directory exists:
   - `ForClassWhile/fcwIn/seed`

2. Ensure output directory does **not** already exist (important). If it exists from a previous run, delete or rename it.

3. In the same terminal, export fuzzing environment variables as needed:
   - If you cannot set CPU governor:
     - `export AFL_SKIP_CPUFREQ=1`

4. Run afl-fuzz:

   - `cd /home/suhas/Downloads/AFL/ForClassWhile`
   - `/home/suhas/Downloads/AFL/mafl-2.35b/afl-fuzz -i fcwIn -o fcwOut -- ./for-class-while`

5. Let it run until the status screen shows **20 cycles**.

   What to watch in the AFL UI:
   - Look for the “cycles done” field.
   - When it reaches **20**, stop the run.

6. Stop cleanly:
   - Press `Ctrl+C` once.
   - AFL will do some cleanup and exit.

> The assignment says you “can kill AFL after it completes 20 cycles.” Prefer `Ctrl+C` so AFL flushes stats nicely.

### 1.4) Create `output-fcw.zip`

You must zip the **entire output folder** (the one containing `queue/`, `hangs/`, etc.).

Assuming your output folder is `ForClassWhile/fcwOut/`:

1. Go to `ForClassWhile/` (parent of `fcwOut/`).
2. Create zip that contains the folder itself:
   - `zip -r output-fcw.zip fcwOut`

Verify the zip contents include (at least):
- `fcwOut/queue/`
- `fcwOut/crashes/` (may be empty)
- `fcwOut/hangs/` (may be empty)
- `fcwOut/fuzzer_stats`

### 1.5) Problem 1(b): Identify all **1-bit flip** queue entries and the flipped bit position

You must write a `problem1.pdf` with:

- All test input **file ids** obtained using **1-bit flips** (look for `op:flip1` in filename)
- For each such file, identify the **bit position flipped compared to its parent**
- Bit positions are counted with:
  - **left-most bit of the entire input as position 0**
  - i.e., bit numbering is **MSB-first** within each byte

#### 1.5.1) Find all queue entries created by 1-bit flip

AFL stores retained testcases in:

- `fcwOut/queue/`

Each filename encodes metadata, e.g.:

- `id:000001,src:000000,op:flip1,pos:0`

**Steps:**

1. List queue entries with `op:flip1`:
   - Look for filenames containing `op:flip1`.

2. For each matching file:
   - Extract `id:XXXXXX` (child id)
   - Extract `src:YYYYYY` (parent id)

3. Create a table for your PDF with columns like:
   - Child id
   - Parent id
   - Filename (full)
   - Flipped bit position (to compute next)

#### 1.5.2) Locate the parent file for each flip1 entry

Given a child filename like:

- `id:000123,src:000045,op:flip1,pos:...`

The parent is the queue entry whose filename starts with:

- `id:000045,`

**Steps:**

1. In `fcwOut/queue/`, find the parent file by matching the `id:` prefix.
2. Confirm parent exists; if not, double-check you copied the id correctly.

#### 1.5.3) Compute the flipped bit position (MSB-first indexing)

You must compute where the child differs from the parent.

For a 1-bit flip, there should be:
- exactly **one byte** that differs, and
- in that byte, exactly **one bit** differs.

**Method A (byte diff + XOR; robust and fast):**

1. Compare parent and child to find differing byte offset(s):
   - Use `cmp -l <parent> <child>`
   - Output format is typically:
     - `byte_index  parent_value  child_value`

2. For a true 1-bit flip, `cmp -l` should report exactly **one line**.
   - If you see multiple lines, investigate (you may be comparing wrong files, or the mutation isn’t actually a single bit).

3. Let:
   - `b` = the differing **byte index** (0-based for this assignment; `cmp` prints 1-based)
   - `P` = parent byte value
   - `C` = child byte value

4. Compute `X = P XOR C`.
   - For a 1-bit flip, `X` must be one of:
     - `0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01`

5. Convert `X` into bit-within-byte using **MSB-first** indexing:

   - If `X = 0x80` → bit-in-byte = 0
   - If `X = 0x40` → bit-in-byte = 1
   - If `X = 0x20` → bit-in-byte = 2
   - If `X = 0x10` → bit-in-byte = 3
   - If `X = 0x08` → bit-in-byte = 4
   - If `X = 0x04` → bit-in-byte = 5
   - If `X = 0x02` → bit-in-byte = 6
   - If `X = 0x01` → bit-in-byte = 7

6. Compute global bit position:

- `bit_position = (b * 8) + bit_in_byte`

Remember:
- The assignment’s “position 0” is the **left-most bit of the first byte**.

**Method B (visualize bits):**

1. Use `xxd -b <file>` to see each byte in binary.
2. Compare parent vs child line-by-line.
3. Identify the single flipped bit.
4. Compute global bit position the same way: `byte_index * 8 + bit_in_byte`.

#### 1.5.4) What to write in `problem1.pdf`

Include a clear, graded-friendly table.

Minimum recommended content per row:

- Child testcase id (XXXXXX)
- Parent testcase id (YYYYYY)
- Flipped bit position (integer)
- (Optional but helpful) Parent and child bytes in hex, and the XOR byte

### 1.6) Create `problem1.zip`

You must create `problem1.zip` containing:

- `output-fcw.zip`
- `problem1.pdf`

Make sure the filenames match exactly.

---

## 2) Problem 2 (the `bad!` assertion program)

### What you must submit for Problem 2

You must produce:

- `problem2.pdf` containing answers for parts (a)–(e)

The assignment does **not** explicitly ask you to zip the output folder for Problem 2, but you still need the output folder locally to extract:
- `queue/` contents
- `crashes/` contents
- `fuzzer_stats`
- screenshot of AFL screen at termination

### 2.1) Create the C program exactly “as-is”

Create a new directory, e.g.:

- `/home/suhas/Downloads/AFL/Problem2/`

Inside it, create a file, e.g.:

- `Problem2/problem2.c`

Copy the program **exactly as given in the spec**:

- includes (`stdio.h`, `assert.h`)
- the same nested `if` structure
- the `assert(0);` line

> Keep it as-is (even if `void main(...)` is non-standard). The assignment explicitly says “copy as-is.”

### 2.2) Create the seed file containing only `"a"` (1 byte)

The spec says: seed file contains only the single character `a`, created using the command style from Problem 1.

**Steps:**

1. Create input directory:
   - `Problem2/p2In/`

2. Create seed:
   - `cd /home/suhas/Downloads/AFL/Problem2/p2In`
   - `echo -n "a" > seed`

3. Verify it is exactly 1 byte:
   - `wc -c seed` should print `1`
   - `od -cb seed` should show exactly one character

### 2.3) Compile the target using the provided `afl-gcc`

1. Export (or re-export) compile variables in this terminal (§0.3):
   - `export AFL_PATH=/home/suhas/Downloads/AFL/mafl-2.35b/`
   - `export AFL_KEEP_ASSEMBLY=1`
   - `export TMPDIR=.`

2. Compile:
   - `cd /home/suhas/Downloads/AFL/Problem2`
   - `/home/suhas/Downloads/AFL/mafl-2.35b/afl-gcc problem2.c -o problem2`

### 2.4) Enable “stop after crash” behavior

The spec requires:

- `export AFL_BENCH_UNTIL_CRASH=1`

Do this in the **same terminal** where you will run `afl-fuzz`.

(Optionally also set `AFL_SKIP_CPUFREQ=1` if you can’t change CPU scaling.)

### 2.5) Run `afl-fuzz` until it violates the assertion

1. Ensure output directory is fresh, e.g.:
   - `Problem2/p2Out/` should not exist yet.

2. Run:

- `cd /home/suhas/Downloads/AFL/Problem2`
- `/home/suhas/Downloads/AFL/mafl-2.35b/afl-fuzz -i p2In -o p2Out -- ./problem2`

3. Wait until AFL finds the crashing input (assertion failure).

Because `AFL_BENCH_UNTIL_CRASH=1` is set:
- AFL should stop shortly after finding the first crash.

If AFL does not stop:
- check whether `p2Out/crashes/` is getting files
- ensure the env var was exported in the same shell

### 2.6) Prepare `problem2.pdf`

Your PDF must contain written answers for (a)–(e).

#### 2.6.1) Part (a): print every file name + contents in `queue/` and `crashes/`

The spec says:

- For each file in `queue` and `crashes`, print:
  - file name
  - file contents
- Ignore any `README.txt` inside those directories.

**Steps:**

1. Go to `p2Out/queue`.
2. For each file matching `id:*`:
   - record the filename
   - record the content

Repeat for `p2Out/crashes`.

Practical notes:
- These testcases should be tiny and mostly printable (since the target is checking characters like `b a d !`).
- If any file contains non-printable bytes, represent it using `od -An -t u1 -v` or similar.

#### 2.6.2) Part (b): copy AFL screen fields + screenshot + `fuzzer_stats`

You must write down from AFL screen:

1. **No. cycles**
   - From the “cycles done” field.

2. **No. levels**
   - AFL displays “levels” on its status screen.

3. **No. of generated test inputs**
   - Most consistent interpretation: the number of retained queue entries.
   - On screen this corresponds to “paths total” (or equivalent), and it also appears in `fuzzer_stats`.

4. **No of inputs retained due to each fuzzing operation**
   - Use the “stage finds” section of the AFL UI.
   - For each stage (flip1, flip2, ..., havoc, splice, etc.), record how many “finds” occurred.

Additionally include:

- A **screenshot** of AFL when terminated
  - Take the screenshot after it stops (assert found), with the final stats visible.

- Copy-paste the full contents of:
  - `Problem2/p2Out/fuzzer_stats`

Recommended approach for building the PDF:
- Create a document (LibreOffice / LaTeX / Markdown→PDF via pandoc)
- Paste:
  - the screenshot image
  - the `fuzzer_stats` text
  - the values you wrote down

#### 2.6.3) Part (c): draw the directed graph of `queue/`

The graph definition:

- Each node: a testcase id `XXXXXX` from `id:XXXXXX`
- Edge(s): from each parent `src:YYYYYY` to the child `id:XXXXXX`

**Steps to extract edges:**

1. List all files in `p2Out/queue/` excluding README.
2. For each filename:
   - Identify the child id `id:XXXXXX`
   - Identify parent(s):
     - Usually `src:YYYYYY`
     - Sometimes AFL uses splicing with `src:AAAAAA+BBBBBB` (two parents)

3. Create directed edges:
   - `YYYYYY → XXXXXX` (one per parent)

4. The seed file is typically:
   - `id:000000,orig:seed`
   - It has no `src:`; it is a root node.

5. Draw the directed graph manually or with a tool.

Optional (tool-assisted, if you want neat output):
- Use Graphviz:
  - Create a `.dot` file with edges like `000000 -> 000001;`
  - Render to PDF.

#### 2.6.4) Part (d): add special node `crash` and its parent edges

1. Look at `p2Out/crashes/`.
2. For each crash file:
   - extract its `src:YYYYYY` field from the filename
3. Add node named exactly `crash`.
4. Add edges:
   - `YYYYYY → crash`

If multiple crash files exist:
- You can either:
  - add multiple edges into the same `crash` node, or
  - create nodes like `crash1`, `crash2` (only do this if your instructor prefers).

The spec says “add a special node as `crash`” — so prefer a **single** crash node.

#### 2.6.5) Part (e): write your observation

Write a short observation about the structure of the graph. Examples of useful observations:

- It is mostly a tree rooted at `id:000000` (one parent per node), except when splicing introduces 2-parent nodes.
- Early nodes are often produced by deterministic stages (flip/arith/interest), later exploration comes from havoc/splice.
- The crash typically appears after AFL gradually mutates the seed toward the `bad!` string pattern.

---

## 3) Problem 3 (`crack-addr.c` + asserts)

### Important: `crack-addr.c` is NOT currently in your workspace

I searched your workspace and did **not** find `crack-addr.c`.

Before starting Problem 3, obtain `crack-addr.c` from the assignment distribution and place it somewhere in this workspace.

Recommended location:

- Create: `/home/suhas/Downloads/AFL/Problem3/`
- Put the file at: `/home/suhas/Downloads/AFL/Problem3/crack-addr.c`

### What you must submit for Problem 3

You will create:

1. `output-woa.zip` — zip of the entire output folder from fuzzing **without asserts**
2. `crack-addr-assert.c` — modified source with asserts before each array access
3. `output-assert.zip` — zip of the entire output folder from fuzzing the assert version
4. `problem3.pdf` — answers for the analysis parts
5. `problem3.zip` containing all four items above

### 3.1) Create the seed file (same as Problem 1, exactly 2 bytes)

Repeat Problem 1 seed creation (same `<LAST_TWO_DIGITS>`), but in `Problem3/p3In/`.

Verify with:
- `od -cb seed` shows exactly 2 bytes
- `wc -c seed` prints `2`

### 3.2) Compile `crack-addr.c` with `afl-gcc`

1. Export compile variables (§0.3):
   - `AFL_PATH`, `AFL_KEEP_ASSEMBLY=1`, `TMPDIR=.`

2. Compile in the Problem3 directory:
   - `/home/suhas/Downloads/AFL/mafl-2.35b/afl-gcc crack-addr.c -o crack-addr`

### 3.3) Run `afl-fuzz` for a TOTAL of 10 minutes (without asserts)

The spec says: “Run afl-fuzz … using a 10-minute timeout.”

Your `afl-fuzz` version (2.35b) does **not** include a built-in `-V` “stop after N seconds” option, so do this using Linux `timeout`.

Recommended approach (clean stop):

- Use `timeout` to send `SIGINT` after 10 minutes so AFL exits cleanly:
  - `timeout --signal=INT 10m <afl-fuzz command>`

**Steps:**

1. Ensure output directory is fresh:
   - `Problem3/p3Out-woa/` should not exist.

2. Run afl-fuzz for 10 minutes:

- `cd /home/suhas/Downloads/AFL/Problem3`
- `timeout --signal=INT 10m /home/suhas/Downloads/AFL/mafl-2.35b/afl-fuzz -i p3In -o p3Out-woa -- ./crack-addr`

3. After `timeout` stops AFL, confirm output folder exists and contains:
- `p3Out-woa/queue/`
- `p3Out-woa/crashes/`
- `p3Out-woa/hangs/`
- `p3Out-woa/fuzzer_stats`

### 3.4) Zip the output folder as `output-woa.zip`

From the parent directory (`Problem3/`):

- `zip -r output-woa.zip p3Out-woa`

### 3.5) Create `crack-addr-assert.c` (assert before each array access)

The spec: “Just before each array access statement in program, write an assert statement to check whether array access that can catch array overflows.”

Interpretation:
- You must detect when an index is outside bounds **before** doing `arr[index]`.
- The assert should fail when an overflow would happen.

**Steps (systematic):**

1. Copy the original source:
   - `cp crack-addr.c crack-addr-assert.c`

2. Open `crack-addr-assert.c` and locate every array access.

Array access patterns to search for:
- `arr[i]`
- `arr[i + k]`
- `arr[f(x)]`
- Multi-dim: `arr[i][j]`
- Writes and reads both count:
  - `x = arr[i];`
  - `arr[i] = x;`

3. For each array `arr`:
   - Find its declared size, e.g. `int arr[16];`
   - The valid index range is:
     - `0 <= i < 16`

4. Immediately before the access, insert an assert:

- `assert(i >= 0 && i < 16);`

Notes:
- If `i` is an unsigned type (e.g. `size_t`), `i >= 0` is always true; you can omit it:
  - `assert(i < 16);`

5. For computed indices like `i + k`:
- assert the computed value is in bounds.

6. For multi-dimensional arrays:
- `int a[4][8];` and access `a[i][j]` → add:
  - `assert(i >= 0 && i < 4);`
  - `assert(j >= 0 && j < 8);`

7. Make sure `#include <assert.h>` is present.

8. Recompile the assert version:
- `/home/suhas/Downloads/AFL/mafl-2.35b/afl-gcc crack-addr-assert.c -o crack-addr-assert`

### 3.6) Identify queue inputs that violate asserts BUT are not in `crashes/` (from the non-assert run)

The goal:

- Take the inputs from the **non-assert fuzz run** (`p3Out-woa/queue/`)
- Replay them against the **assert-instrumented binary** (`./crack-addr-assert`)
- Find which queue inputs now cause assertion failures
- Verify those inputs are **not** present in `p3Out-woa/crashes/`

**Steps:**

1. Ensure you have both binaries:
- `./crack-addr` (original)
- `./crack-addr-assert` (with asserts)

2. Determine how the program consumes input:
- If it reads from stdin, you can replay like:
  - `./crack-addr-assert < testcase`
- If it expects a filename argument, use:
  - `./crack-addr-assert testcase`

(Inspect `main()` in `crack-addr.c` to know which it is.)

3. For each file in `p3Out-woa/queue/id:*`:
- run it through `crack-addr-assert`
- if you see an assertion failure (usually message + abort), record:
  - the testcase filename and its `id:XXXXXX`

4. Cross-check `p3Out-woa/crashes/`:
- Verify these ids are **not** present there.

5. Add your findings to `problem3.pdf`:
- list of `id:XXXXXX` that violate asserts
- (optional) which assertion/array/index triggered

### 3.7) Explain (2 lines) why these are not in the crashes folder (from part a)

Write something like:

- In the original program, out-of-bounds array access is **undefined behavior** and often does **not** immediately raise a fatal signal; the program may continue and exit normally.
- AFL only classifies inputs as “crashes” when the process terminates with a crash signal (SIGSEGV/SIGABRT/etc.); without explicit checks, many overflows are silent.

### 3.8) Run `afl-fuzz` on `crack-addr-assert.c` and zip the output

Now fuzz the assert-instrumented version so that out-of-bounds accesses become reliable “crashes” (assert abort).

**Steps:**

1. Ensure output directory is fresh:
- `Problem3/p3Out-assert/` should not exist.

2. Run afl-fuzz (you can also use `timeout 10m` unless the spec wants “until crash”; it does not — it just says run afl-fuzz and upload output folder):

- `cd /home/suhas/Downloads/AFL/Problem3`
- `timeout --signal=INT 10m /home/suhas/Downloads/AFL/mafl-2.35b/afl-fuzz -i p3In -o p3Out-assert -- ./crack-addr-assert`

3. Zip the output folder:
- `zip -r output-assert.zip p3Out-assert`

### 3.9) Create `problem3.zip`

Create `problem3.zip` containing:

- `output-woa.zip`
- `crack-addr-assert.c`
- `output-assert.zip`
- `problem3.pdf`

---

## Final packaging: `<your-name>.zip`

The spec says your final submission is:

- a single zip named `<your-name>.zip`
- containing the items mentioned in the assignment

A safe final bundle that matches the spec is:

- `problem1.zip`
- `problem2.pdf`
- `problem3.zip`

Before zipping, verify filenames match exactly:

- `problem1.zip` contains:
  - `output-fcw.zip`
  - `problem1.pdf`

- `problem3.zip` contains:
  - `output-woa.zip`
  - `crack-addr-assert.c`
  - `output-assert.zip`
  - `problem3.pdf`

---

## Troubleshooting checklist (common AFL issues)

### “No instrumentation detected”

- You likely compiled with system `gcc` instead of `mafl-2.35b/afl-gcc`.
- Recompile using the wrapper.

### CPU governor warning

- Either set performance governor OR export `AFL_SKIP_CPUFREQ=1`.

### Output directory already exists

- AFL may refuse to run or may prompt.
- Rename/delete the old output dir so the new run is clean.

### `crack-addr.c` input style is not stdin

- If it expects a filename, you must run afl-fuzz as:
  - `... -- ./crack-addr @@`

Check `main()` to confirm.

### Need coverage/branch mapping (optional)

If you want to inspect which branches a testcase covers:

1. Run `afl-showmap` (course-modified version) to produce `showmap-result.txt`.
2. Run `xor.py` to produce `Table.txt`.

Note: `xor.py` requires **python2**.

---

## Deliverables recap (quick)

- Problem 1:
  - `output-fcw.zip`
  - `problem1.pdf`
  - `problem1.zip` (contains the above two)

- Problem 2:
  - `problem2.pdf`

- Problem 3:
  - `output-woa.zip`
  - `crack-addr-assert.c`
  - `output-assert.zip`
  - `problem3.pdf`
  - `problem3.zip` (contains the above four)

- Final:
  - `<your-name>.zip` containing `problem1.zip`, `problem2.pdf`, `problem3.zip`
