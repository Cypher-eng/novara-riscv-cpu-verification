# Week 2 Lecture - RISC-V and Simulation

## Big Question
How can we simulate a CPU using C++?

## Key Ideas
- A simulator models how a CPU behaves.
- We can check whether instructions produce correct results.
- A testbench runs tests automatically.

## Verification Flow
1. Load a program
2. Execute instructions
3. Check final register values
4. Report pass or fail

## Example
If we run:

`ADD x3, x1, x2`

and x1 = 5, x2 = 3, then x3 should become 8.
