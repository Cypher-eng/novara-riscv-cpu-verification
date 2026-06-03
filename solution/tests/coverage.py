from pathlib import Path
from collections import Counter
import csv

program_dir = Path("programs")
counter = Counter()

for asm in program_dir.glob("*.asm"):
    for line in asm.read_text().splitlines():
        line = line.split("#")[0].strip()
        if not line or line.endswith(":"):
            continue
        op = line.split()[0]
        counter[op] += 1

with open("coverage.csv", "w", newline="") as f:
    writer = csv.writer(f)
    writer.writerow(["instruction", "count"])
    for op in ["ADD", "SUB", "ADDI", "LW", "SW", "BEQ", "BNE"]:
        writer.writerow([op, counter.get(op, 0)])

print("Coverage report written to coverage.csv")
for op, count in counter.items():
    print(f"{op}: {count}")
