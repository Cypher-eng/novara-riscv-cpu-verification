#pragma once
#include <array>
#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>

// ---------------------------------------------------------------------------
// Instruction
//
// One decoded RISC-V instruction. The parser fills this in from a line of
// assembly. We keep it deliberately small and readable.
//
//   op    : the mnemonic, always stored UPPER-CASE (e.g. "ADD", "BEQ").
//   rd    : destination register index   (0..31)
//   rs1   : first  source register index (0..31)
//   rs2   : second source register index (0..31)
//   imm   : an immediate value, OR a branch/jump target (an instruction index)
//   label : the original target text of a branch/jump, kept only so the
//           execution trace can print a friendly name like "loop" instead of
//           a raw number.
//
// Note: in this teaching simulator a "memory address" and a "jump target" are
// both simple integers. Jump/branch targets are *instruction indices* (which
// line of the program to run next), not byte addresses. This keeps the model
// easy to reason about for a first course.
// ---------------------------------------------------------------------------
struct Instruction {
    std::string op;
    int rd = 0;
    int rs1 = 0;
    int rs2 = 0;
    int imm = 0;
    std::string label;
};

// ---------------------------------------------------------------------------
// SimpleRISCVCPU
//
// A tiny single-cycle RISC-V interpreter. It owns 32 registers and a small
// word-addressed memory, loads a list of decoded instructions, and runs them
// one at a time while recording a human-readable trace.
// ---------------------------------------------------------------------------
class SimpleRISCVCPU {
public:
    SimpleRISCVCPU();

    // Clears registers, memory, the loaded program, and the trace.
    void reset();

    // Loads a decoded program and rewinds the program counter to 0.
    void loadProgram(const std::vector<Instruction>& program);

    // Runs instructions until the program counter leaves the program, or until
    // maxSteps instructions have executed. The step limit protects us from
    // infinite loops.
    //
    // Returns true if the program halted normally (ran off the end), or false
    // if it was stopped by the step limit (a likely infinite loop). See also
    // hitStepLimit().
    bool run(int maxSteps = 1000);

    // Read access (bounds-checked, friendly error messages).
    int32_t getReg(int index) const;
    int32_t getMem(int address) const;

    // Write access (bounds-checked). Writes to x0 are ignored: x0 is the
    // RISC-V zero register and is permanently 0.
    void setReg(int index, int32_t value);
    void setMem(int address, int32_t value);

    // The execution trace: one readable line per executed instruction,
    // including operands and the resulting value. This is the main piece of
    // evidence students use during the debugging lab.
    const std::vector<std::string>& getTrace() const;

    // True if the most recent run() stopped because it hit the step limit.
    bool hitStepLimit() const;

    static constexpr int NUM_REGISTERS = 32;
    static constexpr int MEMORY_WORDS  = 256;

private:
    std::array<int32_t, NUM_REGISTERS> regs;
    std::array<int32_t, MEMORY_WORDS> memory;
    std::vector<Instruction> instructions;
    std::vector<std::string> trace;
    int pc;
    bool stepLimitReached;

    // Executes a single instruction. May change pc (for taken branches and
    // jumps). Returns a human-readable description of what happened, which
    // run() stores in the trace.
    std::string execute(const Instruction& inst);

    // Bounds-checked register/memory helpers with clear error messages.
    int32_t readReg(int index) const;
    void    writeReg(int index, int32_t value);
    int32_t loadMem(int address) const;
    void    storeMem(int address, int32_t value);
};
