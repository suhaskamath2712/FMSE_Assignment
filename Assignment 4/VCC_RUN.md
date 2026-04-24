# Running VCC on this Linux machine (Assignment 4)

This workspace’s Assignment 4 uses **VCC (Verified C Compiler)** to verify the annotated C files:

- `q1b.c`
- `q2.c`
- `q3.c`

All of them include VCC’s annotations via `#include <vcc.h>`.

---

## 1) What is installed here?

On this system, `vcc` is **already configured as a shell alias** that runs the Windows VCC binary via Wine:

- `vcc` → `WINEDEBUG=-all wine /opt/VCC/vcc/Host/bin/Release/vcc.exe`

You can confirm this in a terminal:

- `type vcc`

VCC uses an SMT solver (typically **Z3**). On this system Z3 is available on `PATH`.

---

## 2) Basic usage (verify a file)

1. Open a terminal (VS Code integrated terminal is fine).
2. Change directory to Assignment 4:
   - `cd "/home/suhas/Desktop/FMSE_Assignment/Assignment 4"`
3. Verify a single file:
   - `vcc q1b.c`

If verification succeeds, VCC prints lines like:

- `Verification of main succeeded.`

Tip: if VCC is *too quiet*, use `/stats` to also show timing:

- `vcc /stats q1b.c`

---

## 3) Verify all Assignment 4 files

From the `Assignment 4/` directory:

- `vcc q1b.c q2.c q3.c`

Or with timing output:

- `vcc /stats q1b.c q2.c q3.c`

---

## 4) Help / common flags

VCC uses Windows-style flags that start with `/`.

- Show help:
  - `vcc /help`

Useful flags you may see in the help output:

- `/stats` — show timing stats
- `/out:<directory>` — write generated intermediate files into a directory
- `/d` or `/d0` — dump transformed source (useful for debugging)
- `/functions:<list>` or `/f:<list>` — verify only a subset of function roots

Example (verify only `main`):

- `vcc /f:main q2.c`

---

## 5) Interpreting results

### Success
You will see messages like:

- `Verification of <function> succeeded.`

Exit status is `0` (success).

### Failure
VCC will print an error/warning describing what could not be proved, typically referencing:

- a failed assertion (`_(assert ...)`)
- a missing/incorrect invariant (`_(invariant ...)`)
- a contract mismatch (`_(requires ...)`, `_(ensures ...)`, `_(writes ...)`)

In that case, exit status is non-zero.

---

## 6) Troubleshooting (most common)

### “vcc: command not found”
The alias may not be loaded in that terminal session.

Fix options:

1. Open a new login shell (or restart your terminal), then re-check with `type vcc`.
2. Run VCC directly via Wine:
   - `WINEDEBUG=-all wine /opt/VCC/vcc/Host/bin/Release/vcc.exe q1b.c`

### Wine issues
If you see `wine: command not found`, Wine is not installed (or not on `PATH`). Install Wine, or use the course/VM environment where VCC was set up.

### Z3/solver issues
If VCC complains about Z3:

- confirm Z3 is on `PATH` (e.g., `command -v z3`)
- confirm Z3 runs (e.g., `z3 -version`)

### Paths with spaces
The folder name `Assignment 4` contains a space.

- Always quote the path when using `cd` or when passing file paths.

---

## 7) Verified on this machine

The following commands were executed successfully on this system:

- `vcc /stats q1b.c`
- `vcc /stats q2.c`
- `vcc /stats q3.c`
