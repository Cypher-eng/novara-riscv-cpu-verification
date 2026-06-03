#ifndef CPU_HPP
#define CPU_HPP

#include <vector>
#include <string>
#include <map>
#include <cstdint>

class CPU {
public:
    // 32 general-purpose registers, corresponding to RISC-V x0 to x31
    int32_t registers[32];
    
    // Simplified data memory (1024 words)
    int32_t memory[1024];
    
    // Program Counter
    uint32_t pc;
    
    // Halt flag
    bool halted;

    // [Extension] Functional coverage tracker
    // Tracks how many times each instruction (e.g., "ADD") is executed
    std::map<std::string, int> instruction_coverage;

    // Constructor: Initializes the CPU state
    CPU();

    // Executes a single instruction string (includes parsing)
    void execute_line(const std::string& line);

    // Prints the current state of all registers
    void print_registers();

    // [Extension] Exports verification coverage to a CSV file
    void export_coverage_csv(const std::string& filename);

private:
    // Helper function: Removes spaces and commas from strings
    std::string trim(const std::string& str);
};

#endif // CPU_HPP
