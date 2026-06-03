# Branch test
ADDI x1, x0, 5
ADDI x2, x0, 5
BEQ x1, x2, equal
ADDI x3, x0, 0
BNE x1, x2, end
equal:
ADDI x3, x0, 1
end:
ADDI x4, x0, 0
