# Lab 3 - Write Your Own Functional Tests

## Goal

Create your own RISC-V test program.

## Task 1: Open

`starter/programs/student_custom_test.asm`

## Task 2: Write a Test

Write a program that checks at least one instruction.

Example:

```asm
ADDI x1, x0, 10
ADDI x2, x0, 20
ADD x3, x1, x2
```

Expected result:

`x3 = 30`

## Task 3: Add Expected Output

Modify the testbench or record the expected register result in your report.

## Challenge

Write:

- one arithmetic test
- one memory test
- one branch test

## Submission

Submit:

`verification_report_v1.pdf`
