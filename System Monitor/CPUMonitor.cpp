#include "CPUMonitor.h"
#include <fstream>
#include <sstream>
#include <thread>

CPUMonitor::CPUMonitor() {
    readCPUStats(prevIdleTime, prevTotalTime);  //initialise with current values
}

float CPUMonitor::getUsage() {
    long idleTime, totalTime;
    readCPUStats(idleTime, totalTime);  //read new values

    long deltaIdle = idleTime - prevIdleTime;
    long deltaTotal = totalTime - prevTotalTime;

    prevIdleTime = idleTime;            //update stored values
    prevTotalTime = totalTime;

    if (deltaTotal == 0) return 0.0f;
    return 100.0f * (1.0f - static_cast<float>(deltaIdle) / deltaTotal);  //calculate usage
}

void CPUMonitor::readCPUStats(long& idleTime, long& totalTime) {
    //std::ifstream file("/proc/meminfo"); // for linux
    std::ifstream file("cpuinfo_mock.txt"); // for Windows testing purposes
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open meminfo_mock.txt. Make sure the file exists in the working directory.");
    }
    std::string line;
    std::getline(file, line);  //read first line

    std::istringstream ss(line);
    std::string cpuLabel;
    long user, nice, system, idle, iowait, irq, softirq, steal;

    ss >> cpuLabel >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;

    idleTime = idle + iowait;  //total idle time
    totalTime = user + nice + system + idle + iowait + irq + softirq + steal;  //total time
}
