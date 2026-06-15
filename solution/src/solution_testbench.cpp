#include "cpu.h"
#include "parser.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

struct ExpectedReg {
    int reg;
    int value;
};

// Print the execution trace so students can see exactly what the CPU did.
// Very long traces (e.g. from an infinite loop) are shortened so the terminal
// stays readable.
static void printTrace(const std::vector<std::string>& trace) {
    std::cout << "    --- execution trace ---\n";
    const size_t cap = 50;
    if (trace.size() <= cap) {
        for (const auto& line : trace) std::cout << "    " << line << "\n";
    } else {
        for (size_t i = 0; i < 25; ++i) std::cout << "    " << trace[i] << "\n";
        std::cout << "    ... (" << (trace.size() - 50) << " lines omitted) ...\n";
        for (size_t i = trace.size() - 25; i < trace.size(); ++i)
            std::cout << "    " << trace[i] << "\n";
    }
}

// Runs one program and checks the expected register values. Any failure or
// error prints the trace as evidence and returns false, but never crashes the
// whole testbench: a broken program in one test cannot stop the others.
bool runTest(const std::string& programPath, const std::vector<ExpectedReg>& expected) {
    SimpleRISCVCPU cpu;

    try {
        auto program = parseAssemblyFile(programPath);
        cpu.loadProgram(program);
        bool halted = cpu.run();

        if (!halted) {
            std::cout << "[FAIL] " << programPath
                      << " did not finish within the step limit"
                      << " (possible infinite loop)\n";
            printTrace(cpu.getTrace());
            return false;
        }

        bool pass = true;
        for (const auto& e : expected) {
            int actual = cpu.getReg(e.reg);
            if (actual != e.value) {
                std::cout << "[FAIL] " << programPath
                          << " expected x" << e.reg << "=" << e.value
                          << " but got " << actual << "\n";
                pass = false;
            }
        }

        if (pass) {
            std::cout << "[PASS] " << programPath << "\n";
        } else {
            printTrace(cpu.getTrace());
        }
        return pass;

    } catch (const std::exception& ex) {
        std::cout << "[ERROR] " << programPath << ": " << ex.what() << "\n";
        if (!cpu.getTrace().empty()) printTrace(cpu.getTrace());
        return false;
    }
}

int main() {
    int passed = 0;
    int total = 0;

    // ---- Required core instructions --------------------------------------
    total++;
    if (runTest("programs/arithmetic_basic.asm", {{1, 5}, {2, 3}, {3, 8}, {4, 2}})) passed++;

    total++;
    if (runTest("programs/memory_basic.asm", {{1, 42}, {2, 42}})) passed++;

    total++;
    if (runTest("programs/branch_basic.asm", {{1, 5}, {2, 5}, {3, 1}, {4, 0}})) passed++;

    // ---- Extended instruction set ----------------------------------------
    total++;
    if (runTest("programs/logic_basic.asm",
                {{3, 8}, {4, 14}, {5, 6}, {8, 8}, {9, 6}, {10, 1},
                 {11, 0}, {12, 4}, {13, 13}, {14, 16}, {15, 1}})) passed++;

    total++;
    if (runTest("programs/loop_sum.asm", {{1, 15}, {2, 6}, {4, 5}})) passed++;

    total++;
    if (runTest("programs/jump_basic.asm", {{1, 1}, {2, 0}, {3, 7}})) passed++;

    std::cout << "\nSummary: " << passed << "/" << total << " tests passed.\n";

    return (passed == total) ? 0 : 1;
}
