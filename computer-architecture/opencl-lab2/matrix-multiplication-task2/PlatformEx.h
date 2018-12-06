#pragma once

#include "OpenCl.h"

#include <vector>

class PlatformEx
{
public:
	static std::vector<cl::Platform> GetPlatforms();
	
	static std::vector<cl::Device> GetDevices(const cl::Platform& platform);
};

