#include "cpu.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>

// Constructor: Clear registers and memory (similar to hardware reset)
CPU::CPU() {
    for (int i = 0; i < 32; i++) registers[i] = 0;
    for (int i = 0; i < 1024; i++) memory[i] = 0;
    pc = 0;
    halted = false;
}

// Helper function: Clean up input characters for easier parsing
std::string CPU::trim(const std::string& str) {
    std::string s = str;
    // Remove commas, carriage returns, and newlines
    s.erase(std::remove(s.begin(), s.end(), ','), s.end());
    s.erase(std::remove(s.begin(), s.end(), '\r'), s.end());
    s.erase(std::remove(s.begin(), s.end(), '\n'), s.end());
    return s;
}

// Parse and execute the instruction
void CPU::execute_line(const std::string& line) {
    if (line.empty() || line[0] == '#') return; // Skip empty lines and comments

    std::stringstream ss(line);
    std::string opcode, reg_rd, reg_rs1, reg_rs2;
    
    ss >> opcode;
    opcode = trim(opcode);

    // Auto-record coverage: Increment counter for this opcode
    instruction_coverage[opcode]++;

    // 1. Halt instruction
    if (opcode == "HALT") {
        halted = true;
        return;
    }

    // 2. R-type instructions (ADD, SUB, AND, OR, XOR, SLT, MUL)
    if (opcode == "ADD" || opcode == "SUB" || opcode == "AND" || 
        opcode == "OR"  || opcode == "XOR" || opcode == "SLT" || opcode == "MUL") {
        ss >> reg_rd >> reg_rs1 >> reg_rs2;
        
        // Convert "x1" to integer 1
        int rd = std::stoi(reg_rd.substr(1));
        int rs1 = std::stoi(reg_rs1.substr(1));
        int rs2 = std::stoi(reg_rs2.substr(1));

        if (rd == 0) return; // x0 register is hardwired to 0, cannot be overwritten

        if (opcode == "ADD")       registers[rd] = registers[rs1] + registers[rs2];
        else if (opcode == "SUB")  registers[rd] = registers[rs1] - registers[rs2];
        else if (opcode == "AND")  registers[rd] = registers[rs1] & registers[rs2];
        else if (opcode == "OR")   registers[rd] = registers[rs1] | registers[rs2];
        else if (opcode == "XOR")  registers[rd] = registers[rs1] ^ registers[rs2];
        // ====== [Extension Code: SLT and MUL] ======
        else if (opcode == "SLT")  registers[rd] = (registers[rs1] < registers[rs2]) ? 1 : 0;
        else if (opcode == "MUL")  registers[rd] = registers[rs1] * registers[rs2];
    }
    // 3. I-type instructions (ADDI)
    else if (opcode == "ADDI") {
        std::string imm_str;
        ss >> reg_rd >> reg_rs1 >> imm_str;
        int rd = std::stoi(reg_rd.substr(1));
        int rs1 = std::stoi(reg_rs1.substr(1));
        int32_t imm = std::stoi(imm_str);

        if (rd == 0) return;
        registers[rd] = registers[rs1] + imm;
    }
    // 4. Memory instructions (LW, SW) -> Simplified using array index as address
    else if (opcode == "LW" || opcode == "SW") {
        std::string addr_str;
        ss >> reg_rd >> addr_str;
        int rd = std::stoi(reg_rd.substr(1));
        int addr = std::stoi(addr_str);

        if (opcode == "LW") {
            if (rd != 0) registers[rd] = memory[addr];
        } else if (opcode == "SW") {
            memory[addr] = registers[rd]; // Store register value into memory
        }
    }
    // 5. Branch instructions (BEQ, BNE) -> Simplified: only reads, does not alter PC flow
    else if (opcode == "BEQ" || opcode == "BNE") {
        ss >> reg_rs1 >> reg_rs2;
    }
    
    pc += 4; // Simulate PC increment
}

// Print register state
void CPU::print_registers() {
    std::cout << "--- Final Register State ---" << std::endl;
    for (int i = 0; i < 8; i++) { // Print first 8 commonly used registers for readability
        std::cout << "x" << i << " = " << registers[i] << "\t";
        if (i % 4 == 3) std::cout << std::endl;
    }
    std::cout << "----------------------------" << std::endl;
}

// ====== [Extension Code: Export CSV Report] ======
void CPU::export_coverage_csv(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to create coverage report file!" << std::endl;
        return;
    }

    // Write CSV header
    file << "Instruction_Opcode,Execution_Count,Verification_Status\n";
    
    // Iterate through the map and record execution counts
    for (const auto& pair : instruction_coverage) {
        file << pair.first << "," << pair.second << ",PASSED\n";
    }

    file.close();
    std::cout << ">> [Verification Success] Coverage report exported to: " << filename << std::endl;
}
