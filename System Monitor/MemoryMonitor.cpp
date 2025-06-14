#include "MemoryMonitor.h"
#include "MemoryReader.h"

float MemoryMonitor::getUsage()
{
	return MemoryReader::getMemoryUsage();
}