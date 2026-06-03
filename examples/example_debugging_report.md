# Example Debugging Report

## Symptom
The arithmetic test failed.

## Expected
x3 = 8

## Actual
x3 = 2

## Investigation
The ADD instruction was expected to add x1 and x2. However, the output looked like subtraction.

## Root Cause
The ADD implementation used subtraction instead of addition.

## Fix
Changed the ADD operation from subtraction to addition.

## Final Result
The arithmetic test passed.
