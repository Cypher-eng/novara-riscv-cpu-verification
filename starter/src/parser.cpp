#include "parser.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

static std::string clean(const std::string& line) {
    std::string s = line;
    auto hash = s.find('#');
    if (hash != std::string::npos) s = s.substr(0, hash);
    std::replace(s.begin(), s.end(), ',', ' ');
    std::replace(s.begin(), s.end(), '(', ' ');
    std::replace(s.begin(), s.end(), ')', ' ');
    return s;
}

static int regNumber(const std::string& r) {
    if (r.size() < 2 || r[0] != 'x') {
        throw std::runtime_error("Invalid register: " + r);
    }
    return std::stoi(r.substr(1));
}

std::vector<Instruction> parseAssemblyFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    std::vector<std::string> lines;
    std::unordered_map<std::string, int> labels;
    std::string raw;

    while (std::getline(file, raw)) {
        std::string s = clean(raw);
        std::stringstream ss(s);
        std::string first;
        if (!(ss >> first)) continue;

        if (!first.empty() && first.back() == ':') {
            first.pop_back();
            labels[first] = static_cast<int>(lines.size());
            std::string rest;
            std::getline(ss, rest);
            if (!rest.empty()) lines.push_back(rest);
        } else {
            lines.push_back(s);
        }
    }

    std::vector<Instruction> program;
    for (const auto& line : lines) {
        std::stringstream ss(line);
        Instruction inst;
        ss >> inst.op;
        if (inst.op.empty()) continue;

        if (inst.op == "ADD" || inst.op == "SUB") {
            std::string rd, rs1, rs2;
            ss >> rd >> rs1 >> rs2;
            inst.rd = regNumber(rd);
            inst.rs1 = regNumber(rs1);
            inst.rs2 = regNumber(rs2);
        } else if (inst.op == "ADDI") {
            std::string rd, rs1;
            ss >> rd >> rs1 >> inst.imm;
            inst.rd = regNumber(rd);
            inst.rs1 = regNumber(rs1);
        } else if (inst.op == "LW") {
            std::string rd, rs1;
            ss >> rd >> inst.imm >> rs1;
            inst.rd = regNumber(rd);
            inst.rs1 = regNumber(rs1);
        } else if (inst.op == "SW") {
            std::string rs2, rs1;
            ss >> rs2 >> inst.imm >> rs1;
            inst.rs2 = regNumber(rs2);
            inst.rs1 = regNumber(rs1);
        } else if (inst.op == "BEQ" || inst.op == "BNE") {
            std::string rs1, rs2, label;
            ss >> rs1 >> rs2 >> label;
            inst.rs1 = regNumber(rs1);
            inst.rs2 = regNumber(rs2);
            if (labels.count(label)) inst.imm = labels[label];
            else inst.imm = std::stoi(label);
        } else {
            throw std::runtime_error("Unknown instruction in parser: " + inst.op);
        }

        program.push_back(inst);
    }

    return program;
}
