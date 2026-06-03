# Student Engineering Report Template

## Title

RISC-V CPU Verification Using a C++ Testbench

## 1. Introduction

Briefly explain what this project is about.

Example:

This project investigates how a simplified RISC-V CPU can be verified using a C++ testbench. The testbench runs small instruction programs and compares the final CPU state with expected results.

## 2. Background

Explain:

- what a CPU is
- what RISC-V is
- why verification matters

## 3. Method

Describe:

- the CPU model
- the supported instructions
- the test programs
- how pass/fail checking works

## 4. Test Cases

Include a table:

| Test Name | Purpose | Expected Result | Pass/Fail |
|---|---|---|---|
| arithmetic.asm | Check ADD/SUB/ADDI | x3 = 30 | PASS |
| memory.asm | Check LW/SW | memory[100] = 42 | PASS |
| branch.asm | Check BEQ/BNE | x3 = 999 | PASS |

## 5. Results

Paste terminal output or summarize the results.

## 6. Debugging Reflection

Explain one problem you met and how you solved it.

## 7. Conclusion

Summarize what you learned.

## 8. Future Work

Possible extensions:

- add more instructions
- add coverage tracking
- create a graphical register viewer
- compare with Verilog simulation
