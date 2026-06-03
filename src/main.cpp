#include "cpu.hpp"

#include <iostream>
#include <string>

bool checkExpected(const std::string& filename, const SimpleRiscVCPU& cpu) {
    if (filename.find("arithmetic") != std::string::npos) {
        return cpu.getRegister(1) == 10 &&
               cpu.getRegister(2) == 20 &&
               cpu.getRegister(3) == 30 &&
               cpu.getRegister(4) == 10 &&
               cpu.getRegister(5) == 0;
    }
    if (filename.find("memory") != std::string::npos) {
        return cpu.getRegister(1) == 42 &&
               cpu.getRegister(2) == 42 &&
               cpu.getMemory(100) == 42;
    }
    if (filename.find("branch") != std::string::npos) {
        return cpu.getRegister(3) == 999 &&
               cpu.getRegister(4) == 123;
    }
    return true;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: ./riscv_testbench <program.asm>\n";
        return 1;
    }

    const std::string filename = argv[1];
    SimpleRiscVCPU cpu;

    std::cout << "Running program: " << filename << "\n";

    if (!cpu.loadProgram(filename)) {
        return 1;
    }

    try {
        cpu.run();
    } catch (const std::exception& e) {
        std::cerr << "Runtime error: " << e.what() << "\n";
        return 1;
    }

    cpu.printRegisters();

    bool pass = checkExpected(filename, cpu);
    std::cout << "Status: " << (pass ? "PASS" : "FAIL") << "\n";

    return pass ? 0 : 2;
}
