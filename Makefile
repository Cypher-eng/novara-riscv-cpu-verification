CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = riscv_testbench
SRC = src/main.cpp src/cpu.cpp

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)
