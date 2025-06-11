#pragma once
#include <string>

//reads CPU usage from /proc/stat and calculates usage percentage.
class CPUMonitor {
public:
    CPUMonitor();           //initialises with current CPU statistics
    float getUsage();       //returns current CPU usage (%)

private:
    long prevIdleTime;      //idle time from previous reading
    long prevTotalTime;     //total CPU time from previous reading

    void readCPUStats(long& idleTime, long& totalTime);     //parses /proc/stat
};
