#include "DeviceEx.h"
#include "PlatformEx.h"

#include <vector>

std::string DeviceEx::GetVendor(const cl::Device& device)
{
	return std::string(device.getInfo<CL_DEVICE_VENDOR>());
}

std::string DeviceEx::GetName(const cl::Device& device)
{
	return std::string(device.getInfo<CL_DEVICE_NAME>());
}

cl::Device DeviceEx::Find(std::string name)
{
	for (const auto& platform : PlatformEx::GetPlatforms())
	{
		for (const auto& device : PlatformEx::GetDevices(platform))
		{
			auto device_name = GetName(device);
			auto vendor = GetVendor(device);

			if (device_name.find(name) != std::string::npos || vendor.find(name) != std::string::npos)
				return device;
		}
	}

	throw std::exception("Can't find OpenCl device!");
}

cl::Context DeviceEx::CreateContext(const cl::Device& device)
{
	auto context_devices = std::vector<cl::Device>();
	context_devices.push_back(device);

	return cl::Context(context_devices);
}