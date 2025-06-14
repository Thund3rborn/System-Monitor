#include "MemoryReader.h"
#include <stdexcept>

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>

//return memory usage percentage on Windows using GLobalMemoryStatusEx
float MemoryReader::getMemoryUsage()
{
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(memInfo);

	//fill memInfo with current memory stats
	if (!GlobalMemoryStatusEx(&memInfo))
	{
		throw std::runtime_error("GlobalMemoryStatusEx failed.");
	}

	DWORDLONG total = memInfo.ullTotalPhys;
	DWORDLONG available = memInfo.ullAvailPhys;

	if (total == 0) return 0.0f;

	//calculate memory usage %: (used / total ) * 100
	return 100.0f * (1.0 - static_cast<double>(available) / total);
}

#else
#include <fstream>
#include <string>
#include <limits>

const std::string MEMORY_FILE = "/proc/meminfo";

//returns memory usage percentage on Linux by parsing /proc/meminfo
float MemoryReader::getMemoryUsage()
{
	std::ifstream file(MEMORY_FILE);
	if (!file.is_open())
	{
		throw std::runtime_error("Failed to open memory info file: " + MEMORY_FILE);
	}

	std::string label;
	long memoryTotal = 0, memoryAvailable = 0;

	while (file >> label)
	{
		if (label == "MemTotal:")
		{
			file >> memoryTotal;
		}
		else if (label == "MemAvailable:")
		{
			file >> memoryAvailable;
			break;	//stop one we have both values
		}
		else
		{
			//skip unused lines
			file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}

		if (memoryTotal == 0) return 0.0f;

		//calculate memory usage %: (used / total) * 100
		return 100.0f * (1.0f - static_cast<float>(memoryAvailable / memoryTotal));
	}

}

#endif