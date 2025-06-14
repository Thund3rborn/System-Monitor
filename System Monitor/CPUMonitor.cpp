#include "CPUMonitor.h"
#include "CPUReader.h"

float CPUMonitor::getUsage()
{
	return CPUReader::getCPUUsage();
}
