# Instruction coverage report.
#
# Scans every .asm program in programs/ and counts how many times each
# instruction is used. Instructions that are never used show a count of 0,
# which is exactly what makes a coverage report useful: it tells you what is
# still untested.
from pathlib import Path
from collections import Counter
import csv

program_dir = Path("programs")
counter = Counter()

for asm in program_dir.glob("*.asm"):
    for line in asm.read_text().splitlines():
        line = line.split("#")[0].split("//")[0].strip()
        if not line or line.endswith(":"):
            continue
        op = line.split()[0].upper()
        counter[op] += 1

# The full instruction set supported by the CPU. Keep this list in step with
# the instructions implemented in src/cpu.cpp.
all_instructions = [
    "ADD", "SUB", "AND", "OR", "XOR", "SLL", "SRL", "SLT",   # register-register
    "ADDI", "ANDI", "ORI", "SLLI", "SLTI",                   # with immediate
    "LW", "SW",                                              # memory
    "BEQ", "BNE", "BLT", "BGE",                              # branches
    "JAL",                                                   # jump
]

with open("coverage.csv", "w", newline="") as f:
    writer = csv.writer(f)
    writer.writerow(["instruction", "count"])
    for op in all_instructions:
        writer.writerow([op, counter.get(op, 0)])

tested = sum(1 for op in all_instructions if counter.get(op, 0) > 0)
print("Coverage report written to coverage.csv")
print(f"Instructions exercised: {tested}/{len(all_instructions)}")
print()
for op in all_instructions:
    count = counter.get(op, 0)
    mark = "ok " if count > 0 else "-- "
    print(f"  {mark} {op:5} {count}")

untested = [op for op in all_instructions if counter.get(op, 0) == 0]
if untested:
    print("\nNot yet tested:", ", ".join(untested))
