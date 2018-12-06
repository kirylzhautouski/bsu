#include "OpenCl.h"
#include "DeviceEx.h"
#include "KernelEx.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace std;
using namespace cl;

class Main
{
public:

	static void Run(int argc, char** argv)
	{
		// -e <input file path> <output file path> <string to encrypt>
		// -d <input file path>
		if (argc == 3 && strcmp(argv[1], "-d") == 0)
			Decrypt(argv[2]);
		else if (argc == 5 && strcmp(argv[1], "-e") == 0)
			Encrypt(argv[2], argv[3], argv[4]);
		else
			PrintUsage();
	}
private:
	static string LoadText(const string& file_name)
	{
		auto source_file = ifstream(file_name);
		return string(istreambuf_iterator<char>(source_file), istreambuf_iterator<char>());
	}

	static void Decrypt(const char* input_file)
	{
		cout << "Decrypt" << endl;

		int n, m;
		char *bytes = nullptr, *s = nullptr;

		if (ReadFromBinary(input_file, &n, &m, bytes))
		{
			try
			{
				int number_of_bytes = 4 * n * m;

				auto device = DeviceEx::Find("Intel");
				auto context = DeviceEx::CreateContext(device);

				auto kernel_code = LoadText("..\\kernels\\steganography-task1\\Decrypt.cl");
				auto kernel = KernelEx::BuildFromSource(device, context, kernel_code, "Decrypt");

				s = new char[number_of_bytes / 8] {};
				auto bytes_buffer = CreateBuffer(context, bytes, number_of_bytes, CL_MEM_READ_ONLY);
				auto s_buffer = CreateBuffer(context, s, number_of_bytes / 8, CL_MEM_READ_WRITE);
				kernel.setArg(0, bytes_buffer);
				kernel.setArg(1, s_buffer);

				auto queue = CommandQueue(context, device);
				queue.enqueueNDRangeKernel(kernel, NullRange, NDRange(number_of_bytes / 8));
				queue.finish();

				ReadFromBuffer(queue, s, number_of_bytes / 8, s_buffer);
			}
			catch (const Error& e)
			{
				cerr << e.err() << endl;
			}
			catch (const exception& ex)
			{
				cerr << ex.what() << endl;
			}

			for (std::size_t i = 0; i < strlen(s); i++)
				cout << s[i];

			delete[] bytes;
		}
	}

	static void Encrypt(const char* input_file, const char* output_file, char* s)
	{
		cout << "Encrypt" << endl;

		int n, m;
		char* bytes = nullptr;

		if (ReadFromBinary(input_file, &n, &m, bytes))
		{
			int number_of_bytes = 4 * n * m;
			if ((unsigned)number_of_bytes < (strlen(s) + 1) * sizeof(char) * 8)
			{
				cout << "binary file is too small" << endl;
				return;
			}

			try
			{
				auto device = DeviceEx::Find("Intel");
				auto context = DeviceEx::CreateContext(device);

				auto kernel_code = LoadText("..\\kernels\\steganography-task1\\Encrypt.cl");
				auto kernel = KernelEx::BuildFromSource(device, context, kernel_code, "Encrypt");

				auto bytes_buffer = CreateBuffer(context, bytes, number_of_bytes, CL_MEM_READ_WRITE);
				auto s_buffer = CreateBuffer(context, s, strlen(s) + 1, CL_MEM_READ_ONLY);
				kernel.setArg(0, bytes_buffer);
				kernel.setArg(1, s_buffer);

				auto queue = CommandQueue(context, device);
				queue.enqueueNDRangeKernel(kernel, NullRange, NDRange(strlen(s) + 1));
				queue.finish();

				ReadFromBuffer(queue, bytes, number_of_bytes, bytes_buffer);
			}
			catch (const Error& e)
			{
				cerr << e.err() << endl;
			}
			catch (const exception& ex)
			{
				cerr << ex.what() << endl;
			}

			ofstream fout(output_file, ios::out | ios::binary);
			fout.write((char*)&n, 4);
			fout.write((char*)&m, 4);
			fout.write(bytes, number_of_bytes);

			delete[] bytes;

			fout.close();
		}
	}

	static Buffer CreateBuffer(const Context& context, char* s, int size, int access_flag)
	{
		return Buffer(context, access_flag | CL_MEM_COPY_HOST_PTR, size * sizeof(char), s);
	}

	static void ReadFromBuffer(const CommandQueue& queue, char* s, int size, const Buffer& buffer)
	{
		queue.enqueueReadBuffer(buffer, CL_TRUE, 0, size * sizeof(char), s);
	}

	static bool ReadFromBinary(const char* input_file, int* n, int* m, char*& bytes)
	{
		ifstream fin(input_file, ios::in | ios::binary);

		if (fin.is_open())
		{
			fin.read((char*)n, sizeof(int));
			fin.read((char*)m, sizeof(int));

			int number_of_bytes = 4 * *n * *m;
			bytes = new char[number_of_bytes];
			fin.read(bytes, number_of_bytes);

			fin.close();
		}
		else
		{
			cout << "Input file cannot be opened" << endl;

			return false;
		}
		
		return true;
	}

	static void PrintUsage()
	{
		cout << "usage: -e <input file path> <output file path> <string to encrypt>\n" <<
			"       -d <input file path>" << endl;
	}
};

int main(int argc, char** argv)
{
	// -e <input file path> <output file path> <string to encrypt>
	// -d <input file path>
	// steganography-task1 -e ..\input\steganography-task1\data-1.bin ..\output\steganography-task1\encrypted.bin hello
	// steganography-task1 -d ..\output\steganography-task1\encrypted.bin
	Main::Run(argc, argv);
}