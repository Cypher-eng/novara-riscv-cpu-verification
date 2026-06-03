CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

SRC = src/cpu.cpp src/parser.cpp src/testbench.cpp
TARGET = riscv_testbench

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

coverage:
	python3 tests/coverage.py

clean:
	rm -f $(TARGET) coverage.csv
