# NOVARA RISC-V CPU Verification Project

A beginner-friendly **RISC-V CPU verification project** designed for high school students interested in Engineering, Computer Science, Computer Engineering, and Chip Design.

The goal is simple:

> Build a small C++ testbench that checks whether a simplified RISC-V CPU executes instructions correctly.

This project is not a full industrial CPU. It is a clean educational project that introduces the real engineering ideas behind processor verification:

- instruction execution
- registers
- memory
- expected vs actual results
- test cases
- debugging
- technical reporting

---

## Project Outcomes

By the end of the project, students will have:

1. A simple RISC-V CPU simulator in C++
2. A testbench that runs instruction-level tests
3. A test report showing pass/fail results
4. A short engineering report suitable for a portfolio
5. A GitHub repository that can be discussed in interviews

---

## Supported Instructions

This beginner version supports a small subset of RISC-V instructions:

| Instruction | Meaning |
|---|---|
| `ADD rd rs1 rs2` | rd = rs1 + rs2 |
| `SUB rd rs1 rs2` | rd = rs1 - rs2 |
| `ADDI rd rs1 imm` | rd = rs1 + imm |
| `AND rd rs1 rs2` | rd = rs1 & rs2 |
| `OR rd rs1 rs2` | rd = rs1 \| rs2 |
| `XOR rd rs1 rs2` | rd = rs1 ^ rs2 |
| `LW rd address` | rd = memory[address] |
| `SW rs address` | memory[address] = rs |
| `BEQ rs1 rs2 label` | branch if rs1 == rs2 |
| `BNE rs1 rs2 label` | branch if rs1 != rs2 |
| `HALT` | stop execution |

---

## Repository Structure

```text
novara-riscv-cpu-verification/
├── README.md
├── Makefile
├── src/
│   ├── cpu.hpp
│   ├── cpu.cpp
│   └── main.cpp
├── tests/
│   ├── arithmetic.asm
│   ├── memory.asm
│   ├── branch.asm
│   └── expected_results.txt
├── docs/
│   ├── project_brief.md
│   ├── student_report_template.md
│   └── week_plan.md
└── scripts/
    └── run_all.sh
```

---

## How to Run

### Option 1: Use Make

```bash
make
./riscv_testbench tests/arithmetic.asm
./riscv_testbench tests/memory.asm
./riscv_testbench tests/branch.asm
```

### Option 2: Run all tests

```bash
bash scripts/run_all.sh
```

### Clean build files

```bash
make clean
```

---

## Example Output

```text
Running program: tests/arithmetic.asm
Final Register State:
x1 = 10
x2 = 20
x3 = 30
x4 = 10
Status: PASS
```

---

## Suggested Student Tasks

### Beginner Tasks

- Run the provided tests
- Read the register output
- Modify immediate values
- Add new arithmetic test cases

### Intermediate Tasks

- Add a new instruction such as `SLT`
- Add more memory tests
- Improve the pass/fail checker
- Write a short debugging log

### Advanced Tasks

- Add instruction coverage tracking
- Add CSV test output
- Add a simple pipeline diagram in the report
- Compare your testbench with real CPU verification workflows

---

## Academic Value

This project helps students demonstrate:

- engineering thinking
- debugging ability
- computer architecture interest
- systems-level understanding
- project-based learning experience

It is suitable for students applying to:

- Electrical Engineering
- Electronic and Information Engineering
- Computer Engineering
- Computer Science
- Robotics
- AI Hardware / Chip Design related fields

---

## Disclaimer

This is an educational project for pre-university students. It is intentionally simplified and does not represent a complete industrial RISC-V processor.
