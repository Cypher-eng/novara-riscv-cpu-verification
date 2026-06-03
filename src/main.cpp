#include "cpu.hpp"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: ./riscv_testbench <test_file.asm>" << std::endl;
        return 1;
    }

    std::string asm_filename = argv[1];
    std::ifstream file(asm_filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open assembly file " << asm_filename << std::endl;
        return 1;
    }

    std::cout << "Running verification program: " << asm_filename << std::endl;
    
    // Instantiate the virtual CPU (UUT - Unit Under Test)
    CPU my_cpu;

    std::string line;
    // Read assembly instructions line by line and execute them
    while (std::getline(file, line) && !my_cpu.halted) {
        my_cpu.execute_line(line);
    }

    // 1. Print final register state
    my_cpu.print_registers();

    // 2. [Extension] Export coverage report
    my_cpu.export_coverage_csv("coverage_report.csv");

    return 0;
}
