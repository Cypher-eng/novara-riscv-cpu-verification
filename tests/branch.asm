# Branch test
# If x1 == x2, jump to equal_label.
# x3 should become 999.
# x4 should become 123 after the branch section.

ADDI x1 x0 5
ADDI x2 x0 5
BEQ x1 x2 equal_label
ADDI x3 x0 111
BNE x1 x2 end_label

equal_label:
ADDI x3 x0 999

end_label:
ADDI x4 x0 123
HALT
