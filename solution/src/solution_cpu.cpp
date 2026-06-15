#include "cpu.h"
#include <sstream>
#include <stdexcept>
#include <cstdint>

SimpleRISCVCPU::SimpleRISCVCPU() {
    reset();
}

void SimpleRISCVCPU::reset() {
    regs.fill(0);
    memory.fill(0);
    instructions.clear();
    trace.clear();
    pc = 0;
    stepLimitReached = false;
}

void SimpleRISCVCPU::loadProgram(const std::vector<Instruction>& program) {
    instructions = program;
    pc = 0;
    trace.clear();
    stepLimitReached = false;
}

bool SimpleRISCVCPU::run(int maxSteps) {
    int steps = 0;
    stepLimitReached = false;

    while (pc >= 0 && pc < static_cast<int>(instructions.size()) && steps < maxSteps) {
        int oldPc = pc;
        const Instruction& inst = instructions[oldPc];

        // Default behaviour: move to the next instruction. A taken branch or a
        // jump inside execute() will overwrite pc with its target. Doing the
        // increment *before* execute() lets a branch jump to its own address
        // (the classic "spin here forever" idiom) without us accidentally
        // skipping past it.
        pc = oldPc + 1;

        std::string detail = execute(inst);

        std::ostringstream oss;
        oss << "PC=" << oldPc << "  " << detail;
        trace.push_back(oss.str());

        regs[0] = 0; // x0 is hard-wired to zero
        steps++;
    }

    // We stopped on the step limit only if we ran out of steps while the
    // program counter was still inside the program (i.e. it had not halted).
    stepLimitReached = (steps >= maxSteps) &&
                       (pc >= 0 && pc < static_cast<int>(instructions.size()));
    return !stepLimitReached;
}

bool SimpleRISCVCPU::hitStepLimit() const {
    return stepLimitReached;
}

int32_t SimpleRISCVCPU::getReg(int index) const {
    return readReg(index);
}

int32_t SimpleRISCVCPU::getMem(int address) const {
    return loadMem(address);
}

void SimpleRISCVCPU::setReg(int index, int32_t value) {
    writeReg(index, value);
}

void SimpleRISCVCPU::setMem(int address, int32_t value) {
    storeMem(address, value);
}

const std::vector<std::string>& SimpleRISCVCPU::getTrace() const {
    return trace;
}

// ---------------------------------------------------------------------------
// Bounds-checked accessors. These give clear, beginner-friendly errors instead
// of cryptic std::array messages or silent undefined behaviour.
// ---------------------------------------------------------------------------
int32_t SimpleRISCVCPU::readReg(int index) const {
    if (index < 0 || index >= NUM_REGISTERS) {
        throw std::runtime_error("Register index out of range: x" +
                                 std::to_string(index) +
                                 " (valid registers are x0..x31)");
    }
    return regs[index];
}

void SimpleRISCVCPU::writeReg(int index, int32_t value) {
    if (index < 0 || index >= NUM_REGISTERS) {
        throw std::runtime_error("Register index out of range: x" +
                                 std::to_string(index) +
                                 " (valid registers are x0..x31)");
    }
    if (index != 0) {            // x0 is permanently 0; writes are discarded
        regs[index] = value;
    }
}

int32_t SimpleRISCVCPU::loadMem(int address) const {
    if (address < 0 || address >= MEMORY_WORDS) {
        throw std::runtime_error("Memory address out of range: " +
                                 std::to_string(address) +
                                 " (valid addresses are 0.." +
                                 std::to_string(MEMORY_WORDS - 1) + ")");
    }
    return memory[address];
}

void SimpleRISCVCPU::storeMem(int address, int32_t value) {
    if (address < 0 || address >= MEMORY_WORDS) {
        throw std::runtime_error("Memory address out of range: " +
                                 std::to_string(address) +
                                 " (valid addresses are 0.." +
                                 std::to_string(MEMORY_WORDS - 1) + ")");
    }
    memory[address] = value;
}

// ---------------------------------------------------------------------------
// execute
//
// The heart of the CPU. Each instruction reads its source registers, computes
// a result, and either writes a destination register, touches memory, or
// changes the program counter. Every branch returns a short description that
// run() stores in the trace.
//
// Instruction set (RV32I subset):
//   Required core (Weeks 1-5):  ADD SUB ADDI LW SW BEQ BNE
//   Extended set  (richer tests, coverage, and stretch goals):
//       register-register : AND OR XOR SLL SRL SLT
//       with immediate    : ANDI ORI SLLI SLTI
//       branches          : BLT BGE
//       jump              : JAL
//
// Good first extensions for strong students (follow the same patterns):
//   XORI, SRLI, SRA, JALR, LUI.
// ---------------------------------------------------------------------------
std::string SimpleRISCVCPU::execute(const Instruction& inst) {
    std::ostringstream d;
    const std::string& op = inst.op;

    // Helper: format "x<rd> = <expr>" and warn if the write targets x0.
    auto wrote = [&](int rd, const std::string& expr) {
        std::ostringstream s;
        s << "x" << rd << " = " << expr;
        if (rd == 0) s << "  (x0 is hard-wired to 0, write discarded)";
        return s.str();
    };

    // ---- Register-register arithmetic and logic ---------------------------
    if (op == "ADD") {
        int32_t a = readReg(inst.rs1), b = readReg(inst.rs2), r = a + b;
        writeReg(inst.rd, r);
        d << "ADD x" << inst.rd << ", x" << inst.rs1 << ", x" << inst.rs2
          << "  | " << wrote(inst.rd, std::to_string(a) + " + " + std::to_string(b) +
                             " = " + std::to_string(r));
    } else if (op == "SUB") {
        int32_t a = readReg(inst.rs1), b = readReg(inst.rs2), r = a - b;
        writeReg(inst.rd, r);
        d << "SUB x" << inst.rd << ", x" << inst.rs1 << ", x" << inst.rs2
          << "  | " << wrote(inst.rd, std::to_string(a) + " - " + std::to_string(b) +
                             " = " + std::to_string(r));
    } else if (op == "AND") {
        int32_t a = readReg(inst.rs1), b = readReg(inst.rs2), r = a & b;
        writeReg(inst.rd, r);
        d << "AND x" << inst.rd << ", x" << inst.rs1 << ", x" << inst.rs2
          << "  | " << wrote(inst.rd, std::to_string(a) + " & " + std::to_string(b) +
                             " = " + std::to_string(r));
    } else if (op == "OR") {
        int32_t a = readReg(inst.rs1), b = readReg(inst.rs2), r = a | b;
        writeReg(inst.rd, r);
        d << "OR x" << inst.rd << ", x" << inst.rs1 << ", x" << inst.rs2
          << "  | " << wrote(inst.rd, std::to_string(a) + " | " + std::to_string(b) +
                             " = " + std::to_string(r));
    } else if (op == "XOR") {
        int32_t a = readReg(inst.rs1), b = readReg(inst.rs2), r = a ^ b;
        writeReg(inst.rd, r);
        d << "XOR x" << inst.rd << ", x" << inst.rs1 << ", x" << inst.rs2
          << "  | " << wrote(inst.rd, std::to_string(a) + " ^ " + std::to_string(b) +
                             " = " + std::to_string(r));
    } else if (op == "SLL") {
        int32_t a = readReg(inst.rs1);
        int sh = readReg(inst.rs2) & 31;
        int32_t r = static_cast<int32_t>(static_cast<uint32_t>(a) << sh);
        writeReg(inst.rd, r);
        d << "SLL x" << inst.rd << ", x" << inst.rs1 << ", x" << inst.rs2
          << "  | " << wrote(inst.rd, std::to_string(a) + " << " + std::to_string(sh) +
                             " = " + std::to_string(r));
    } else if (op == "SRL") {
        int32_t a = readReg(inst.rs1);
        int sh = readReg(inst.rs2) & 31;
        int32_t r = static_cast<int32_t>(static_cast<uint32_t>(a) >> sh);
        writeReg(inst.rd, r);
        d << "SRL x" << inst.rd << ", x" << inst.rs1 << ", x" << inst.rs2
          << "  | " << wrote(inst.rd, std::to_string(a) + " >> " + std::to_string(sh) +
                             " = " + std::to_string(r));
    } else if (op == "SLT") {
        int32_t a = readReg(inst.rs1), b = readReg(inst.rs2), r = (a < b) ? 1 : 0;
        writeReg(inst.rd, r);
        d << "SLT x" << inst.rd << ", x" << inst.rs1 << ", x" << inst.rs2
          << "  | " << wrote(inst.rd, "(" + std::to_string(a) + " < " + std::to_string(b) +
                             ") = " + std::to_string(r));

    // ---- Arithmetic and logic with an immediate ---------------------------
    } else if (op == "ADDI") {
        int32_t a = readReg(inst.rs1), r = a + inst.imm;
        writeReg(inst.rd, r);
        d << "ADDI x" << inst.rd << ", x" << inst.rs1 << ", " << inst.imm
          << "  | " << wrote(inst.rd, std::to_string(a) + " + " + std::to_string(inst.imm) +
                             " = " + std::to_string(r));
    } else if (op == "ANDI") {
        int32_t a = readReg(inst.rs1), r = a & inst.imm;
        writeReg(inst.rd, r);
        d << "ANDI x" << inst.rd << ", x" << inst.rs1 << ", " << inst.imm
          << "  | " << wrote(inst.rd, std::to_string(a) + " & " + std::to_string(inst.imm) +
                             " = " + std::to_string(r));
    } else if (op == "ORI") {
        int32_t a = readReg(inst.rs1), r = a | inst.imm;
        writeReg(inst.rd, r);
        d << "ORI x" << inst.rd << ", x" << inst.rs1 << ", " << inst.imm
          << "  | " << wrote(inst.rd, std::to_string(a) + " | " + std::to_string(inst.imm) +
                             " = " + std::to_string(r));
    } else if (op == "SLLI") {
        int32_t a = readReg(inst.rs1);
        int sh = inst.imm & 31;
        int32_t r = static_cast<int32_t>(static_cast<uint32_t>(a) << sh);
        writeReg(inst.rd, r);
        d << "SLLI x" << inst.rd << ", x" << inst.rs1 << ", " << inst.imm
          << "  | " << wrote(inst.rd, std::to_string(a) + " << " + std::to_string(sh) +
                             " = " + std::to_string(r));
    } else if (op == "SLTI") {
        int32_t a = readReg(inst.rs1), r = (a < inst.imm) ? 1 : 0;
        writeReg(inst.rd, r);
        d << "SLTI x" << inst.rd << ", x" << inst.rs1 << ", " << inst.imm
          << "  | " << wrote(inst.rd, "(" + std::to_string(a) + " < " + std::to_string(inst.imm) +
                             ") = " + std::to_string(r));

    // ---- Memory -----------------------------------------------------------
    } else if (op == "LW") {
        int address = readReg(inst.rs1) + inst.imm;
        int32_t v = loadMem(address);
        writeReg(inst.rd, v);
        d << "LW x" << inst.rd << ", " << inst.imm << "(x" << inst.rs1 << ")"
          << "  | " << wrote(inst.rd, "mem[" + std::to_string(address) + "] = " + std::to_string(v));
    } else if (op == "SW") {
        int address = readReg(inst.rs1) + inst.imm;
        int32_t v = readReg(inst.rs2);
        storeMem(address, v);
        d << "SW x" << inst.rs2 << ", " << inst.imm << "(x" << inst.rs1 << ")"
          << "  | mem[" << address << "] = " << v;

    // ---- Branches (target is an instruction index) ------------------------
    } else if (op == "BEQ" || op == "BNE" || op == "BLT" || op == "BGE") {
        int32_t a = readReg(inst.rs1), b = readReg(inst.rs2);
        bool take = (op == "BEQ" && a == b) ||
                    (op == "BNE" && a != b) ||
                    (op == "BLT" && a <  b) ||
                    (op == "BGE" && a >= b);
        const char* rel = (op == "BEQ") ? " == " : (op == "BNE") ? " != " :
                          (op == "BLT") ? " < "  : " >= ";
        std::string tgt = inst.label.empty() ? std::to_string(inst.imm) : inst.label;
        d << op << " x" << inst.rs1 << ", x" << inst.rs2 << ", " << tgt
          << "  | " << a << rel << b << " -> ";
        if (take) {
            pc = inst.imm;       // overrides the default "next instruction"
            d << "taken, PC -> " << inst.imm;
        } else {
            d << "not taken";
        }

    // ---- Jump (unconditional). rd receives the return address. ------------
    } else if (op == "JAL") {
        // pc currently points at the next instruction (run() set it before
        // calling execute), so that is the return address to save.
        int32_t returnAddr = pc;
        writeReg(inst.rd, returnAddr);
        std::string tgt = inst.label.empty() ? std::to_string(inst.imm) : inst.label;
        pc = inst.imm;
        d << "JAL x" << inst.rd << ", " << tgt
          << "  | " << wrote(inst.rd, std::to_string(returnAddr) + " (return address)")
          << ", PC -> " << inst.imm;

    } else {
        throw std::runtime_error("Unknown instruction: " + op);
    }

    return d.str();
}
