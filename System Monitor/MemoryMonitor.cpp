#include "MemoryMonitor.h"
#include "MemoryReader.h"
#include <fstream>
#include <string>
#include <limits>
#include <stdexcept> // required for std::runtime_error

float MemoryMonitor::getUsage()
{
	return MemoryReader::getMemoryUsage();
}