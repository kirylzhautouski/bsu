#include "KernelEx.h"

#include <string>
#include <fstream>
#include <iostream>

cl::Kernel KernelEx::BuildFromSource(const cl::Device& device, const cl::Context& context,
	const std::string& kernel_code, const std::string& kernel_name)
{
	auto source = cl::Program::Sources(1, std::make_pair(kernel_code.c_str(), kernel_code.length() + 1));
	auto program = cl::Program(context, source);
	auto context_devices = std::vector<cl::Device>{ device };

	try
	{
		program.build(context_devices);
	}
	catch (const cl::Error&)
	{
		std::cerr << "Build log: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device) << std::endl;
	}

	return cl::Kernel(program, kernel_name.c_str());
}

std::string LoadText(const std::string& path)
{
	auto source_file = std::ifstream(path.c_str());
	return std::string(std::istreambuf_iterator<char>(source_file), std::istreambuf_iterator<char>());
}

cl::Kernel KernelEx::BuildFromFile(const cl::Device& device, const cl::Context& context, 
	const std::string& kernel_file, const std::string& kernel_name)
{
	auto kernel_source = LoadText(kernel_file);
	return BuildFromSource(device, context, kernel_source, kernel_name);
}
