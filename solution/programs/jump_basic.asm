# Jump test: JAL performs an unconditional jump and saves a return address.
# The ADDI in the middle is skipped over.
JAL  x1, skip       # x1 = return address, jump to "skip"
ADDI x2, x0, 999    # SKIPPED: should never run
skip:
ADDI x3, x0, 7      # x3 = 7
