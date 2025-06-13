#include "MemoryMonitor.h"
#include <fstream>
#include <string>
#include <limits>
#include <stdexcept> // required for std::runtime_error

float MemoryMonitor::getUsage()
{
	//std::ifstream file("/proc/meminfo"); // for linux
	std::ifstream file("meminfo_mock.txt"); // for Windows testing purposes
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open meminfo_mock.txt. Make sure the file exists in the working directory.");
	}
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