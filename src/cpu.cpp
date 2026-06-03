#include "cpu.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

SimpleRiscVCPU::SimpleRiscVCPU() {
    registers_.fill(0);
}

std::string SimpleRiscVCPU::trim(const std::string& text) {
    const auto start = text.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    const auto end = text.find_last_not_of(" \t\r\n");
    return text.substr(start, end - start + 1);
}

std::vector<std::string> SimpleRiscVCPU::split(const std::string& text) {
    std::stringstream ss(text);
    std::string token;
    std::vector<std::string> tokens;
    while (ss >> token) {
        if (!token.empty() && token.back() == ',') token.pop_back();
        tokens.push_back(token);
    }
    return tokens;
}

int SimpleRiscVCPU::regIndex(const std::string& reg) {
    if (reg.size() < 2 || reg[0] != 'x') {
        throw std::runtime_error("Invalid register name: " + reg);
    }
    int index = std::stoi(reg.substr(1));
    if (index < 0 || index > 31) {
        throw std::runtime_error("Register index out of range: " + reg);
    }
    return index;
}

bool SimpleRiscVCPU::loadProgram(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << filename << "\n";
        return false;
    }

    program_.clear();
    labels_.clear();
    pc_ = 0;
    halted_ = false;
    registers_.fill(0);
    memory_.clear();

    std::string line;
    while (std::getline(file, line)) {
        const auto commentPos = line.find('#');
        if (commentPos != std::string::npos) {
            line = line.substr(0, commentPos);
        }
        line = trim(line);
        if (line.empty()) continue;

        if (!line.empty() && line.back() == ':') {
            std::string label = line.substr(0, line.size() - 1);
            labels_[label] = static_cast<int>(program_.size());
        } else {
            program_.push_back(line);
        }
    }
    return true;
}

void SimpleRiscVCPU::run() {
    while (!halted_ && pc_ >= 0 && pc_ < static_cast<int>(program_.size())) {
        executeLine(program_[pc_]);
        keepZeroRegisterConstant();
    }
}

void SimpleRiscVCPU::executeLine(const std::string& line) {
    auto tokens = split(line);
    if (tokens.empty()) {
        pc_++;
        return;
    }

    const std::string& op = tokens[0];

    if (op == "ADD") {
        registers_[regIndex(tokens[1])] = registers_[regIndex(tokens[2])] + registers_[regIndex(tokens[3])];
        pc_++;
    } else if (op == "SUB") {
        registers_[regIndex(tokens[1])] = registers_[regIndex(tokens[2])] - registers_[regIndex(tokens[3])];
        pc_++;
    } else if (op == "ADDI") {
        registers_[regIndex(tokens[1])] = registers_[regIndex(tokens[2])] + std::stoi(tokens[3]);
        pc_++;
    } else if (op == "AND") {
        registers_[regIndex(tokens[1])] = registers_[regIndex(tokens[2])] & registers_[regIndex(tokens[3])];
        pc_++;
    } else if (op == "OR") {
        registers_[regIndex(tokens[1])] = registers_[regIndex(tokens[2])] | registers_[regIndex(tokens[3])];
        pc_++;
    } else if (op == "XOR") {
        registers_[regIndex(tokens[1])] = registers_[regIndex(tokens[2])] ^ registers_[regIndex(tokens[3])];
        pc_++;
    } else if (op == "LW") {
        int address = std::stoi(tokens[2]);
        registers_[regIndex(tokens[1])] = memory_[address];
        pc_++;
    } else if (op == "SW") {
        int address = std::stoi(tokens[2]);
        memory_[address] = registers_[regIndex(tokens[1])];
        pc_++;
    } else if (op == "BEQ") {
        if (registers_[regIndex(tokens[1])] == registers_[regIndex(tokens[2])]) {
            pc_ = labels_.at(tokens[3]);
        } else {
            pc_++;
        }
    } else if (op == "BNE") {
        if (registers_[regIndex(tokens[1])] != registers_[regIndex(tokens[2])]) {
            pc_ = labels_.at(tokens[3]);
        } else {
            pc_++;
        }
    } else if (op == "HALT") {
        halted_ = true;
    } else {
        throw std::runtime_error("Unknown instruction: " + op);
    }
}

void SimpleRiscVCPU::keepZeroRegisterConstant() {
    registers_[0] = 0;
}

void SimpleRiscVCPU::printRegisters() const {
    std::cout << "Final Register State:\n";
    for (int i = 0; i < 8; ++i) {
        std::cout << "x" << i << " = " << registers_[i] << "\n";
    }
}

int32_t SimpleRiscVCPU::getRegister(int index) const {
    return registers_.at(index);
}

int32_t SimpleRiscVCPU::getMemory(int address) const {
    auto it = memory_.find(address);
    if (it == memory_.end()) return 0;
    return it->second;
}
