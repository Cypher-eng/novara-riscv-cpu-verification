#!/usr/bin/env bash
set -e

make

echo "\n--- Running arithmetic test ---"
./riscv_testbench tests/arithmetic.asm

echo "\n--- Running memory test ---"
./riscv_testbench tests/memory.asm

echo "\n--- Running branch test ---"
./riscv_testbench tests/branch.asm

echo "\nAll tests completed."
