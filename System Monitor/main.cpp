#include <iostream>
#include "CPUMonitor.h"
#include "MemoryMonitor.h"

#include <chrono>
#include <thread>


int main() {
    CPUMonitor cpu;
    MemoryMonitor memory;

    while (true)
    {
        system("clear");
        std::cout << "=== System Monitor Test ===\n";
        std::cout << "CPU Usage: " << cpu.getUsage() << "%\n";
        std::cout << "Memory usage: " << memory.getUsage() << "%\n";

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
