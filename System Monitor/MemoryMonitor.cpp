#include "MemoryMonitor.h"
#include <fstream>
#include <string>
#include <limits>

float MemoryMonitor::getUsage()
{
	std::ifstream file("/proc/meminfo");
	std::string label;
	long memoryTotal = 0, memoryAvailable = 0;

	//reads one word at a time from the "file" into "label" until end of the file
	while (file >> label)
	{
		if (label == "MemTotal:")
		{
			file >> memoryTotal;
		}
		else if (label == "MemAvailable:")
		{
			file >> memoryAvailable;
			break;
		}
		else
		{
			file.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skip line
		}
	}

	if (memoryTotal == 0) return 0.0f;
	return 100.0f * (1.0f - static_cast<float>(memoryAvailable) / memoryTotal); //calculate usage
}