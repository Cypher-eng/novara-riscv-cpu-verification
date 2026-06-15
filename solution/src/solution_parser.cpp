#include "parser.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <cctype>
#include <set>

// Remove comments (# or //) and turn separators ( , ( ) ) into spaces so the
// rest of the parser can just split on whitespace.
static std::string clean(const std::string& line) {
    std::string s = line;
    auto hash = s.find('#');
    if (hash != std::string::npos) s = s.substr(0, hash);
    auto slashes = s.find("//");
    if (slashes != std::string::npos) s = s.substr(0, slashes);
    std::replace(s.begin(), s.end(), ',', ' ');
    std::replace(s.begin(), s.end(), '(', ' ');
    std::replace(s.begin(), s.end(), ')', ' ');
    return s;
}

static std::string toUpper(std::string s) {
    for (auto& c : s) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    return s;
}

// Parse a register name such as "x5" (also accepts "X5"). Validates the 0..31
// range so mistakes are caught with a clear message at parse time.
static int regNumber(const std::string& r) {
    if (r.size() < 2 || (r[0] != 'x' && r[0] != 'X')) {
        throw std::runtime_error("Invalid register name '" + r +
                                 "' (expected x0..x31)");
    }
    for (size_t i = 1; i < r.size(); ++i) {
        if (!std::isdigit(static_cast<unsigned char>(r[i]))) {
            throw std::runtime_error("Invalid register name '" + r +
                                     "' (expected x0..x31)");
        }
    }
    int n = std::stoi(r.substr(1));
    if (n < 0 || n > 31) {
        throw std::runtime_error("Register out of range '" + r +
                                 "' (valid registers are x0..x31)");
    }
    return n;
}

// Parse an immediate. Base 0 accepts decimal, negative values, and hex (0x..).
static int parseImm(const std::string& tok) {
    try {
        size_t consumed = 0;
        int value = std::stoi(tok, &consumed, 0);
        if (consumed != tok.size()) throw std::invalid_argument("trailing");
        return value;
    } catch (...) {
        throw std::runtime_error("Invalid immediate value '" + tok + "'");
    }
}

std::vector<Instruction> parseAssemblyFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    // First pass: collect instruction lines and record where each label points
    // (a label points at the *next* instruction's index).
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
            // Trim leading whitespace; keep the line only if something remains.
            size_t start = rest.find_first_not_of(" \t");
            if (start != std::string::npos) lines.push_back(rest.substr(start));
        } else {
            lines.push_back(s);
        }
    }

    static const std::set<std::string> rType  =
        {"ADD", "SUB", "AND", "OR", "XOR", "SLL", "SRL", "SLT"};
    static const std::set<std::string> iType  =
        {"ADDI", "ANDI", "ORI", "SLLI", "SLTI"};
    static const std::set<std::string> branch =
        {"BEQ", "BNE", "BLT", "BGE"};

    // Second pass: decode each line into an Instruction.
    std::vector<Instruction> program;
    for (const auto& line : lines) {
        std::stringstream ss(line);
        Instruction inst;
        ss >> inst.op;
        if (inst.op.empty()) continue;
        inst.op = toUpper(inst.op);

        if (rType.count(inst.op)) {                 // OP rd, rs1, rs2
            std::string rd, rs1, rs2;
            ss >> rd >> rs1 >> rs2;
            inst.rd  = regNumber(rd);
            inst.rs1 = regNumber(rs1);
            inst.rs2 = regNumber(rs2);
        } else if (iType.count(inst.op)) {          // OP rd, rs1, imm
            std::string rd, rs1, imm;
            ss >> rd >> rs1 >> imm;
            inst.rd  = regNumber(rd);
            inst.rs1 = regNumber(rs1);
            inst.imm = parseImm(imm);
        } else if (inst.op == "LW") {               // LW rd, imm(rs1)
            std::string rd, imm, rs1;
            ss >> rd >> imm >> rs1;
            inst.rd  = regNumber(rd);
            inst.imm = parseImm(imm);
            inst.rs1 = regNumber(rs1);
        } else if (inst.op == "SW") {               // SW rs2, imm(rs1)
            std::string rs2, imm, rs1;
            ss >> rs2 >> imm >> rs1;
            inst.rs2 = regNumber(rs2);
            inst.imm = parseImm(imm);
            inst.rs1 = regNumber(rs1);
        } else if (branch.count(inst.op)) {         // OP rs1, rs2, target
            std::string rs1, rs2, target;
            ss >> rs1 >> rs2 >> target;
            inst.rs1 = regNumber(rs1);
            inst.rs2 = regNumber(rs2);
            if (labels.count(target)) {
                inst.imm = labels[target];
                inst.label = target;
            } else {
                inst.imm = parseImm(target);
            }
        } else if (inst.op == "JAL") {              // JAL rd, target
            std::string rd, target;
            ss >> rd >> target;
            inst.rd = regNumber(rd);
            if (labels.count(target)) {
                inst.imm = labels[target];
                inst.label = target;
            } else {
                inst.imm = parseImm(target);
            }
        } else {
            throw std::runtime_error("Unknown instruction '" + inst.op +
                                     "' in line: " + line);
        }

        program.push_back(inst);
    }

    return program;
}
