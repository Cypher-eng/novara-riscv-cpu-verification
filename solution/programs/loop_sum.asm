# Loop test: add the integers 1 + 2 + 3 + 4 + 5
# Demonstrates a real algorithm built from branches.
# Expected final result: x1 = 15
ADDI x1, x0, 0      # x1 = running sum  = 0
ADDI x2, x0, 1      # x2 = counter i    = 1
ADDI x4, x0, 5      # x4 = limit        = 5
loop:
ADD  x1, x1, x2     # sum = sum + i
ADDI x2, x2, 1      # i = i + 1
BGE  x4, x2, loop   # if limit >= i (i.e. i <= 5) keep looping
