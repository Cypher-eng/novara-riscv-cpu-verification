#ifndef SIMPLE_RISCV_CPU_HPP
#define SIMPLE_RISCV_CPU_HPP

#include <array>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

class SimpleRiscVCPU {
public:
    SimpleRiscVCPU();

    bool loadProgram(const std::string& filename);
    void run();
    void printRegisters() const;
    int32_t getRegister(int index) const;
    int32_t getMemory(int address) const;

private:
    std::array<int32_t, 32> registers_{};
    std::unordered_map<int, int32_t> memory_;
    std::vector<std::string> program_;
    std::unordered_map<std::string, int> labels_;
    int pc_ = 0;
    bool halted_ = false;

    void executeLine(const std::string& line);
    static std::string trim(const std::string& text);
    static std::vector<std::string> split(const std::string& text);
    static int regIndex(const std::string& reg);
    void keepZeroRegisterConstant();
};

#endif
