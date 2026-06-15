# NOVARA RISC-V CPU Verification

A 5-week, project-based introduction to **RISC-V processor verification**, **C++ testbench design**, **debugging**, and **engineering communication**, built for pre-university students.

This repository is designed for A-Level, AP, and IB students interested in Engineering, Computer Science, Electronic Engineering, Computer Architecture, or Chip Design. No prior CPU design experience is required — basic programming helps, but every week is guided step by step.

---

## What students build

Students use a working, simplified RISC-V CPU simulator and a C++ verification testbench. They do not implement the processor from scratch. Instead, they do what real verification engineers do: write functional tests, predict expected results, read execution traces, hunt down a planted bug, measure coverage, and present their findings in an engineering format.

By the end, a student can explain how basic CPU instructions execute, write their own tests in assembly, debug incorrect behaviour from evidence, and communicate the result clearly.

---

## How the course is taught

Each week has **one 60-minute main lecture** delivered by the lead instructor, followed by **hands-on lab time run by the teaching assistants**. The lecture sets up the concept and the mindset; the lab is where students do the work.

| Week | Main lecture (lead instructor, 60 min) | Lab (teaching assistants) | Student output |
|---|---|---|---|
| 1 | What is a CPU, and why verify it? | CPU foundations worksheet | Architecture worksheet |
| 2 | Inside the simulator | Build and run the testbench | First verification log |
| 3 | The art of testing | Write your own functional tests | Verification report v1 |
| 4 | Finding the bug | Debugging challenge | Debugging report |
| 5 | Coverage and communication | Coverage + final presentation | Final report + presentation |

The five lecture decks live alongside the weekly notes and are meant to be presented as-is by the lead instructor. Everything else — setup help, code walkthroughs, debugging support, report feedback — is handled by the TAs in lab.

---

## The CPU core

The simulator is a single-cycle interpreter: 32 registers, 256 words of memory, one instruction per step. It is intentionally small and heavily commented so students can read the whole thing.

**Supported instruction set (RV32I subset, 20 instructions):**

- Register–register: `ADD SUB AND OR XOR SLL SRL SLT`
- Immediate: `ADDI ANDI ORI SLLI SLTI`
- Memory: `LW SW`
- Branch: `BEQ BNE BLT BGE`
- Jump: `JAL`

The first seven (`ADD SUB ADDI LW SW BEQ BNE`) are the required core every student must understand. The rest exist so stronger students can write richer tests and push their coverage higher.

**Execution traces.** Every instruction prints a human-readable line showing its operands and result, for example:

```text
PC=2  ADD x3, x1, x2  | x3 = 5 + 3 = 8
PC=5  BGE x4, x2, loop | 5 >= 2 -> taken, PC -> 3
```

The trace is the heart of the Week 4 debugging lab: when a test fails, the trace shows exactly what the CPU did, so students debug from evidence rather than guesswork.

**Robustness.** Out-of-range registers, lowercase mnemonics, comments, and malformed programs all produce clear, friendly errors instead of crashing the testbench. Infinite loops are detected and reported rather than running silently to the step limit.

---

## Quick start

```bash
cd starter
make
./riscv_testbench
```

Expected output:

```text
[PASS] programs/arithmetic_basic.asm
[PASS] programs/memory_basic.asm
[PASS] programs/branch_basic.asm
[PASS] programs/logic_basic.asm
[PASS] programs/loop_sum.asm
[PASS] programs/jump_basic.asm

Summary: 6/6 tests passed.
```

Generate a coverage report:

```bash
make coverage
```

This writes `coverage.csv` and prints which instructions the test suite has exercised. One instruction is deliberately left untested as a Week 5 exercise.

---

## The Week 4 debugging challenge

`starter/src/cpu_buggy_example.cpp` is a swap-in copy of the core with a single planted bug: the `ADD` instruction computes a subtraction. The trace description still prints `+`, so a failing test produces a self-evidently wrong line such as `x3 = 5 + 3 = 2`. Students follow that contradiction back to the root cause. To run the lab version, compile this file in place of `cpu.cpp`.

---

## Final deliverables

Students complete a GitHub project repository, a RISC-V functional test suite, a verification report, a debugging report, a coverage summary, and a final presentation. Recommendation letters are considered — never automatic — for students who show meaningful technical contribution, clear understanding, and professional communication.

---

## Repository structure

```text
starter/        Student starter code (working CPU + the buggy example for Week 4)
solution/       Reference solution for instructors
lectures/       Weekly lesson notes
labs/           Weekly lab instructions (TA-led)
project/        Milestone tasks
deliverables/   Report, presentation, and rubric templates
resources/      Beginner-friendly explanations
examples/       Example final outputs
```

### Core source files

```text
src/cpu.h          CPU interface (register/memory sizes, run loop, trace)
src/cpu.cpp        The CPU core: fetch-execute loop + all 20 instructions
src/parser.cpp     Assembly parser (case-insensitive, comment-aware, validated)
src/testbench.cpp  Runs each program, checks results, prints traces on failure
tests/coverage.py  Scans programs and reports instruction coverage
```
