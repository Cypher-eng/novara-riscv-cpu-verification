# Memory test
# Store 42 into memory address 100, then load it back.

ADDI x1 x0 42
SW x1 100
LW x2 100
HALT
