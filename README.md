# NOVARA RISC-V CPU Verification Project

A 5-week project-based introduction to **RISC-V processor verification**, **C++ testbench design**, **debugging**, and **engineering communication** for pre-university students.

This repository is designed for A-Level, AP, and IB students who are interested in Engineering, Computer Science, Electronic Engineering, Computer Architecture, or Chip Design.

## Project Goal

Students will build and use a simplified RISC-V CPU simulator and verification testbench.  
By the end of the project, students will be able to explain how basic CPU instructions work, write functional tests, debug incorrect behaviour, and present their results in an engineering-style format.

## What Students Will Learn

- What a CPU does
- What an instruction set architecture is
- Why RISC-V matters
- How registers and memory work
- How verification engineers test processors
- How to write simple test programs
- How to debug incorrect CPU behaviour
- How to create a verification report
- How to present an engineering project

## 5-Week Structure

| Week | Theme | Student Output |
|---|---|---|
| Week 1 | CPU and RISC-V Foundations | CPU architecture worksheet |
| Week 2 | Running the Simulator | First verification log |
| Week 3 | Writing Functional Tests | Verification report v1 |
| Week 4 | Debugging Challenge | Debugging report |
| Week 5 | Coverage and Presentation | Final report + presentation |

## Final Deliverables

Students will complete:

- GitHub project repository
- RISC-V functional test suite
- Verification report
- Debugging report
- Coverage summary
- Final presentation slides

## Repository Structure

```text
starter/        Student starter code
solution/       Reference solution for instructors or later release
lectures/       Weekly lesson notes
labs/           Weekly lab instructions
project/        Milestone tasks
deliverables/   Report and presentation templates
resources/      Beginner-friendly explanations
examples/       Example final outputs
```

## Quick Start

```bash
cd starter
make
./riscv_testbench
```

Expected output:

```text
[PASS] arithmetic_basic.asm
[PASS] memory_basic.asm
[PASS] branch_basic.asm
```

## Student Level

No prior CPU design experience is required.  
Basic programming experience is helpful, but the project is designed to be guided step by step.
