#include "cpu.h"
#include <sstream>
#include <stdexcept>

SimpleRISCVCPU::SimpleRISCVCPU() {
    reset();
}

void SimpleRISCVCPU::reset() {
    regs.fill(0);
    memory.fill(0);
    instructions.clear();
    trace.clear();
    pc = 0;
}

void SimpleRISCVCPU::loadProgram(const std::vector<Instruction>& program) {
    instructions = program;
    pc = 0;
    trace.clear();
}

void SimpleRISCVCPU::run(int maxSteps) {
    int steps = 0;
    while (pc >= 0 && pc < static_cast<int>(instructions.size()) && steps < maxSteps) {
        Instruction inst = instructions[pc];
        std::ostringstream oss;
        oss << "PC=" << pc << " " << inst.op;
        trace.push_back(oss.str());

        int oldPc = pc;
        execute(inst);

        if (pc == oldPc) {
            pc++;
        }

        regs[0] = 0;
        steps++;
    }
}

int32_t SimpleRISCVCPU::getReg(int index) const {
    return regs.at(index);
}

int32_t SimpleRISCVCPU::getMem(int address) const {
    return memory.at(address);
}

void SimpleRISCVCPU::setReg(int index, int32_t value) {
    writeReg(index, value);
}

void SimpleRISCVCPU::setMem(int address, int32_t value) {
    memory.at(address) = value;
}

const std::vector<std::string>& SimpleRISCVCPU::getTrace() const {
    return trace;
}

void SimpleRISCVCPU::writeReg(int index, int32_t value) {
    if (index != 0) {
        regs.at(index) = value;
    }
}

void SimpleRISCVCPU::execute(const Instruction& inst) {
    if (inst.op == "ADD") {
        writeReg(inst.rd, regs[inst.rs1] - regs[inst.rs2]); // BUG: ADD should not subtract
    } else if (inst.op == "SUB") {
        writeReg(inst.rd, regs[inst.rs1] - regs[inst.rs2]);
    } else if (inst.op == "ADDI") {
        writeReg(inst.rd, regs[inst.rs1] + inst.imm);
    } else if (inst.op == "LW") {
        int address = regs[inst.rs1] + inst.imm;
        writeReg(inst.rd, memory.at(address));
    } else if (inst.op == "SW") {
        int address = regs[inst.rs1] + inst.imm;
        memory.at(address) = regs[inst.rs2];
    } else if (inst.op == "BEQ") {
        if (regs[inst.rs1] == regs[inst.rs2]) {
            pc = inst.imm;
        }
    } else if (inst.op == "BNE") {
        if (regs[inst.rs1] != regs[inst.rs2]) {
            pc = inst.imm;
        }
    } else {
        throw std::runtime_error("Unknown instruction: " + inst.op);
    }
}
