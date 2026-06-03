#pragma once
#include <array>
#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>

struct Instruction {
    std::string op;
    int rd = 0;
    int rs1 = 0;
    int rs2 = 0;
    int imm = 0;
    std::string label;
};

class SimpleRISCVCPU {
public:
    SimpleRISCVCPU();

    void reset();
    void loadProgram(const std::vector<Instruction>& program);
    void run(int maxSteps = 1000);

    int32_t getReg(int index) const;
    int32_t getMem(int address) const;

    void setReg(int index, int32_t value);
    void setMem(int address, int32_t value);

    const std::vector<std::string>& getTrace() const;

private:
    std::array<int32_t, 32> regs;
    std::array<int32_t, 256> memory;
    std::vector<Instruction> instructions;
    std::vector<std::string> trace;
    int pc;

    void execute(const Instruction& inst);
    void writeReg(int index, int32_t value);
};
