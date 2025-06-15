#include "CPUReader.h"
#include <stdexcept>

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#include <pdh.h>
#include <pdhmsg.h>
#pragma comment (lib, "pdh.lib") //link with the Performance Data Helper library

// returns overall CPU usage as % on Windows using PDH API
float CPUReader::getCPUUsage()
{
	static PDH_HQUERY cpuQuery;			//query handle for PDH
	static PDH_HCOUNTER cpuTotal;		//counter for total CPU usage
	static bool initialised = false;	//flag to ensure one-time setup

	if (!initialised)
	{
		//open a new PDH query
		if (PdhOpenQuery(NULL, 0, &cpuQuery) != ERROR_SUCCESS)
		{
			throw std::runtime_error("PdhOpenQuery failed");
		}
		//add a counter for total processor time (all cores)
		if (PdhAddCounter(cpuQuery, L"\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal) != ERROR_SUCCESS)
		{
			throw std::runtime_error("PdhAddCounter failed");
		}
		//collect initial data to prime the counter
		if (PdhCollectQueryData(cpuQuery) != ERROR_SUCCESS)
		{
			throw std::runtime_error("PdhCollectQueryData (init) failed");
		}

		Sleep(100); //give the counter time to gather data
		initialised = true;
	}

	//collect new sample for the counter
	if (PdhCollectQueryData(cpuQuery) != ERROR_SUCCESS)
	{
		throw std::runtime_error("PdhCollectQueryData failed");
	}

	//format the counter value into a usable float
	PDH_FMT_COUNTERVALUE counterVal;
	if (PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal))
	{
		throw std::runtime_error("PdhGetFormattedCounterValue failed");
	}

	//return the CPU Usage as a %, 0.0 to 100.0
	return static_cast<float>(counterVal.doubleValue);
}

#else

#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>

const std::string CPU_FILE = "/proc/stat";

//returns CPU usage on Linux using /proc/stat sampling
float CPUReader::getCPUUsage()
{
	std::ifstream file(CPU_FILE);
	if (!file.is_open())
	{
		throw std::runtime_error("Failed to open CPU info file: " + CPU_FILE);
	}

	//read first line of /proc/stat (contains aggregate CPU stats)
	std::string line;
	std::getline(file, line);
	std::istringstream ss1(line);
	std::string cpuLabel;
	long user1, nice1, system1, idle1, iowait1, irq1, softirq1, steal1;

	//fields represent time spen in various CPU modes
	ss1 >> cpuLabel >> user1 >> nice1 >> system1 >> idle1 >> iowait1 >> irq1 >> softirq1 >> steal1;

	//idle time = idle + iowait
	long prevIdleTime = idle1 + iowait1;
	//total time = all fields together
	long prevTotalTime = user1 + nice1 + system1 + idle1 + iowait1 + irq1 + softirq1 + steal1;

	//reopen file for second sample
	file.close();                // close file completely
	//wait for a short interbal before second sample
	std::this_thread::sleep_for(std::chrono::seconds(1));

	std::ifstream file2(CPU_FILE);
	if (!file2.is_open()) {
		throw std::runtime_error("Failed to reopen CPU info file: " + CPU_FILE);
	}

	std::getline(file2, line);   // read updated CPU stats
	std::istringstream ss2(line);

	long user2, nice2, system2, idle2, iowait2, irq2, softirq2, steal2;
	ss2 >> cpuLabel >> user2 >> nice2 >> system2 >> idle2 >> iowait2 >> irq2 >> softirq2 >> steal2;

	long idleTime = idle2 + iowait2;
	long totalTime = user2 + nice2 + system2 + idle2 + iowait2 + irq2 + softirq2 + steal2;

	//calcualte deltas = how much time has passed between the two readings
	long deltaIdle = idleTime - prevIdleTime;
	long deltaTotal = totalTime - prevTotalTime;

	if (deltaTotal == 0) return 0.0f;

	//calculate CPU usage: (busy time / total time) * 100
	return 100.0f * (1.0f - static_cast<float>(deltaIdle) / deltaTotal);
}

#endif