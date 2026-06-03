# Arithmetic test
# Expected final values:
# x1 = 10
# x2 = 20
# x3 = 30
# x4 = 10
# x5 = 0

ADDI x1 x0 10
ADDI x2 x0 20
ADD x3 x1 x2
SUB x4 x2 x1
XOR x5 x1 x1
HALT
