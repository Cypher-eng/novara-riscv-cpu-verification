# Initialize registers
ADDI x1, x0, 10      # x1 = 10
ADDI x2, x0, 20      # x2 = 20

# Test new instruction 1: MUL (Multiply)
MUL x3, x1, x2       # x3 = 10 * 20 = 200

# Test new instruction 2: SLT (Set Less Than)
SLT x4, x1, x2       # Since 10 < 20, x4 should be 1
SLT x5, x2, x1       # Since 20 is not less than 10, x5 should be 0

# Basic addition test to mix up coverage
ADD x6, x3, x4       # x6 = 200 + 1 = 201

# Stop execution
HALT
