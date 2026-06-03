#include "cpu.h"
#include "parser.h"
#include <iostream>
#include <string>
#include <vector>

struct ExpectedReg {
    int reg;
    int value;
};

bool runTest(const std::string& programPath, const std::vector<ExpectedReg>& expected) {
    SimpleRISCVCPU cpu;
    auto program = parseAssemblyFile(programPath);
    cpu.loadProgram(program);
    cpu.run();

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
    }

    return pass;
}

int main() {
    int passed = 0;
    int total = 0;

    total++;
    if (runTest("programs/arithmetic_basic.asm", {{1, 5}, {2, 3}, {3, 8}, {4, 2}})) passed++;

    total++;
    if (runTest("programs/memory_basic.asm", {{1, 42}, {2, 42}})) passed++;

    total++;
    if (runTest("programs/branch_basic.asm", {{1, 5}, {2, 5}, {3, 1}, {4, 0}})) passed++;

    std::cout << "\nSummary: " << passed << "/" << total << " tests passed.\n";

    if (passed == total) {
        return 0;
    }
    return 1;
}
