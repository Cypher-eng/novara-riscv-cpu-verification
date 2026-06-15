# Logic and shift test
# Exercises the extended instruction set: AND OR XOR SLL SRL SLT ANDI ORI SLLI SLTI
ADDI x1, x0, 12     # x1 = 12  (binary 1100)
ADDI x2, x0, 10     # x2 = 10  (binary 1010)
AND  x3, x1, x2     # x3 = 12 & 10 = 8
OR   x4, x1, x2     # x4 = 12 | 10 = 14
XOR  x5, x1, x2     # x5 = 12 ^ 10 = 6
ADDI x6, x0, 1      # x6 = 1
ADDI x7, x0, 3      # x7 = 3
SLL  x8, x6, x7     # x8 = 1 << 3 = 8
SRL  x9, x1, x6     # x9 = 12 >> 1 = 6
SLT  x10, x2, x1    # x10 = (10 < 12) = 1
SLT  x11, x1, x2    # x11 = (12 < 10) = 0
ANDI x12, x1, 6     # x12 = 12 & 6 = 4
ORI  x13, x1, 1     # x13 = 12 | 1 = 13
SLLI x14, x6, 4     # x14 = 1 << 4 = 16
SLTI x15, x1, 20    # x15 = (12 < 20) = 1
