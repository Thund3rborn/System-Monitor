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

//Linux implementation will come here later

#endif