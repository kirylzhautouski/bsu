#include "KernelEx.h"


cl::Kernel KernelEx::BuildFromSource(const cl::Device& device, const cl::Context& context,
	const std::string& kernel_code, const std::string& kernel_name)
{
	auto source = cl::Program::Sources(1, std::make_pair(kernel_code.c_str(), kernel_code.length() + 1));
	auto program = cl::Program(context, source);
	auto context_devices = std::vector<cl::Device>{ device };

	program.build(context_devices);

	return cl::Kernel(program, kernel_name.c_str());
}
