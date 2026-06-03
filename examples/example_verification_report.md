# Example Verification Report

## Test Name
Arithmetic Basic Test

## Program
```asm
ADDI x1, x0, 5
ADDI x2, x0, 3
ADD x3, x1, x2
```

## Expected Result
x3 = 8

## Actual Result
x3 = 8

## Result
PASS

## Explanation
The ADD instruction correctly added x1 and x2.
