#ifndef CPU_HPP
#define CPU_HPP

#include <vector>
#include <string>
#include <map>
#include <cstdint>

class CPU {
public:
    int32_t registers[32];

    int32_t memory[1024];
    

    uint32_t pc;
    
    bool halted;

    std::map<std::string, int> instruction_coverage;

    CPU();

    void execute_line(const std::string& line);

    void print_registers();

    void export_coverage_csv(const std::string& filename);

private:
    std::string trim(const std::string& str);
};

#endif // CPU_HPP
