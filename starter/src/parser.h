#pragma once
#include "cpu.h"
#include <string>
#include <vector>

std::vector<Instruction> parseAssemblyFile(const std::string& filename);
