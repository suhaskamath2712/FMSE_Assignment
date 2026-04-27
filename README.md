# FMSE_Assignment

This repository is a collection of coursework artefacts for formal-methods tooling. It includes specifications, solutions, and saved tool outputs across multiple assignments and practice exams.

If you’re looking for **how to re-run the verification/fuzzing tools**, start with `Run.md`.

If you're looking for the class notes, visit https://github.com/suhaskamath2712/IISc_E0272_Formal_Methods_in_Software_Engineering.

## What’s in here (at a glance)

The repo spans multiple tools and file types:

- **Alloy** models (`.als`) + PDFs (Assignment 1, MidSem)
- **SPIN/Promela** models (`.pml`) + results/trails (Assignment 2, MidSem)
- **Rodin / Event-B** style deliverables (Assignment 3) — PDFs + project zips
- **VCC**-annotated C verification tasks (Assignment 4, VCC_2024, MockFinal)
- **JPF / SPF** (Java PathFinder / Symbolic PathFinder) configs (`.jpf`) + Java code (Assignment 5, Final, MockFinal)
- **AFL** fuzzing problems, seeds, and run outputs (Assignment 6, Final, MockFinal)

## Repository layout

- `Run.md`
  - Practical run notes for **Assignments 4–6** (VCC, JPF/SPF, AFL) as used for this repo.
  - Some paths are machine-specific; adjust if your tool installations live elsewhere.

- `Assignment 1/` (Alloy)
  - `Assignment1_Alloy.als`, `Assignment1_Alloy_Corrected.als`
  - Spec/answer PDFs, submission zip, and `marks.txt`.

- `Assignment 2/` (SPIN/Promela)
  - `counter-crdt-*.pml` — a counter-CRDT model under different channel assumptions (direct/lossy/duplicate/reorder)
  - `results.txt` — summary of model-checking outcomes
  - Spec/answer PDFs and submission zip.

- `Assignment 3/` (Rodin / Event-B)
  - Assignment PDFs plus zipped project snapshots (`vmemory-2026*.zip`).

- `Assignment 4/` (VCC)
  - `q1b.c`, `q2.c`, `q3.c` — C verification tasks with VCC annotations
  - `assignment-vcc-2026.txt`, `VCC_RUN.md`, and supporting PDF(s).

- `Assignment 5/` (JPF + SPF)
  - `FindMin.java` / `FindMin.jpf` and `NodeSimple.java` / `NodeSimple.jpf`
  - Saved exploration output (`*.out`) and rendered trees (`*.pdf`).

- `Assignment 6/` (AFL)
  - `exports.sh` — environment variables used during AFL runs
  - `problem1/`, `problem2/`, `problem3/` each contain:
    - `ip_dir/` seed corpus directory
    - `op_dir/` (and sometimes `assert_op_dir/`) saved AFL outputs (`queue/`, `crashes/`, `hangs/`, `fuzzer_stats`, ...)

- `MidSem/`
  - Mid-sem practice artefacts: Promela model + trail, Alloy model, and a zip bundle.

- `MockFinal/`
  - Mock-final practice problems covering AFL/JPF/VCC, plus `MockFinal.md` and seed input directory.

- `Final/`
  - Final practice artefacts:
    - C programs (`argmax.c`, `setp.c`, `pgm-for-afl.c`) and a compiled target (`pgm-for-afl`)
    - JPF/SPF (`BinaryTree.java`, `BinaryTree.jpf`, `BinaryTree.out`)
    - AFL artefacts (`ip_dir/`, `op_dir/`, `showmap-result-*.txt`, `Table*.txt`, `random-loc.txt`, and any `.afl-*.s` files)

- `VCC_2024/`
  - Older VCC assignment set (`q1.c`–`q4.c`) plus `assignment-vcc-2024.txt`.

## Notes on generated outputs

Some folders intentionally include **saved tool outputs** (especially AFL `op_dir/` trees). These can be large and contain many files.

When re-running AFL, it’s usually best to use a **fresh output directory** (or rename/delete the existing one) to avoid mixing runs.
