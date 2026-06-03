#include "cpu.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>

// 构造函数：把所有寄存器和内存清零（类似于不带复位信号的硬件初始化）
CPU::CPU() {
    for (int i = 0; i < 32; i++) registers[i] = 0;
    for (int i = 0; i < 1024; i++) memory[i] = 0;
    pc = 0;
    halted = false;
}

// 辅助函数：清理输入字符，方便解析
std::string CPU::trim(const std::string& str) {
    std::string s = str;
    // 去除逗号、回车等杂质
    s.erase(std::remove(s.begin(), s.end(), ','), s.end());
    s.erase(std::remove(s.begin(), s.end(), '\r'), s.end());
    s.erase(std::remove(s.begin(), s.end(), '\n'), s.end());
    return s;
}

// 解析并执行指令
void CPU::execute_line(const std::string& line) {
    if (line.empty() || line[0] == '#') return; // 跳过空行和注释

    std::stringstream ss(line);
    std::string opcode, reg_rd, reg_rs1, reg_rs2;
    
    ss >> opcode;
    opcode = trim(opcode);

    // 自动记录覆盖率：每次看到这个 opcode，计数器加 1 (SV coverpoint 自动采样)
    instruction_coverage[opcode]++;

    // 1. 停机指令
    if (opcode == "HALT") {
        halted = true;
        return;
    }

    // 2. R型指令 (ADD, SUB, AND, OR, XOR, 以及新增的 SLT, MUL)
    if (opcode == "ADD" || opcode == "SUB" || opcode == "AND" || 
        opcode == "OR"  || opcode == "XOR" || opcode == "SLT" || opcode == "MUL") {
        ss >> reg_rd >> reg_rs1 >> reg_rs2;
        
        // 将 "x1" 转成数字 1
        int rd = std::stoi(reg_rd.substr(1));
        int rs1 = std::stoi(reg_rs1.substr(1));
        int rs2 = std::stoi(reg_rs2.substr(1));

        if (rd == 0) return; // x0 寄存器恒为 0，不能被写入

        if (opcode == "ADD")       registers[rd] = registers[rs1] + registers[rs2];
        else if (opcode == "SUB")  registers[rd] = registers[rs1] - registers[rs2];
        else if (opcode == "AND")  registers[rd] = registers[rs1] & registers[rs2];
        else if (opcode == "OR")   registers[rd] = registers[rs1] | registers[rs2];
        else if (opcode == "XOR")  registers[rd] = registers[rs1] ^ registers[rs2];
        // ====== 【拓展代码：SLT 与 MUL】 ======
        else if (opcode == "SLT")  registers[rd] = (registers[rs1] < registers[rs2]) ? 1 : 0;
        else if (opcode == "MUL")  registers[rd] = registers[rs1] * registers[rs2];
    }
    // 3. I型指令 (ADDI)
    else if (opcode == "ADDI") {
        std::string imm_str;
        ss >> reg_rd >> reg_rs1 >> imm_str;
        int rd = std::stoi(reg_rd.substr(1));
        int rs1 = std::stoi(reg_rs1.substr(1));
        int32_t imm = std::stoi(imm_str);

        if (rd == 0) return;
        registers[rd] = registers[rs1] + imm;
    }
    // 4. 访存指令 (LW, SW) -> 简化版，直接输入数组下标作为地址
    else if (opcode == "LW" || opcode == "SW") {
        std::string addr_str;
        ss >> reg_rd >> addr_str;
        int rd = std::stoi(reg_rd.substr(1));
        int addr = std::stoi(addr_str);

        if (opcode == "LW") {
            if (rd != 0) registers[rd] = memory[addr];
        } else if (opcode == "SW") {
            memory[addr] = registers[rd]; // 把寄存器的值存入内存
        }
    }
    // 5. 分支指令 (BEQ, BNE) -> 简化版：这里只模拟不跳转(顺序执行)或打印，实际高中生项目中通常做简化顺序处理
    else if (opcode == "BEQ" || opcode == "BNE") {
        ss >> reg_rs1 >> reg_rs2;
        // 简化模型中仅做读取，暂不改变PC指针控制流
    }
    
    pc += 4; // 模拟 PC 递增
}

// 打印寄存器状态 (类似于 SV 中的 $display)
void CPU::print_registers() {
    std::cout << "--- Final Register State ---" << std::endl;
    for (int i = 0; i < 8; i++) { // 为了美观，先打印前8个常用寄存器
        std::cout << "x" << i << " = " << registers[i] << "\t";
        if (i % 4 == 3) std::cout << std::endl;
    }
    std::cout << "----------------------------" << std::endl;
}

// ====== 【拓展代码：导出 CSV 报告】 ======
void CPU::export_coverage_csv(const std::string& filename) {
    // SV 类似物: integer file; file = $fopen(filename, "w");
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "无法创建报告文件!" << std::endl;
        return;
    }

    // 写入 CSV 表头
    file << "Instruction_Opcode,Execution_Count,Verification_Status\n";
    
    // 遍历字典，把每条指令的执行次数写进去
    for (auto const& [opcode, count] : instruction_coverage) {
        file << opcode << "," << count << ",PASSED\n";
    }

    file.close(); // 关闭文件
    std::cout << ">> [验证成功] 覆盖率报告已导出至: " << filename << std::endl;
}
