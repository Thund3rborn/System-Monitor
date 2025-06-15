CXX = g++
CXXFLAGS = -std=c++17 -Wall
SRC = main.cpp CPUMonitor.cpp CPUReader.cpp MemoryMonitor.cpp MemoryReader.cpp
OUT = sysmon

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)
