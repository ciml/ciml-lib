/*
 * OpenCLUtils.cpp
 *
 *  Created on: 4 de jun de 2016
 *      Author: igor
 */

#include "OpenCLUtils.h"
#define __NO_STD_VECTOR // Use cl::vector instead of STL version

OpenCLUtils::OpenCLUtils() {
	// TODO Auto-generated constructor stub
}

OpenCLUtils::~OpenCLUtils() {
	// TODO Auto-generated destructor stub
}

Program OpenCLUtils::CreateProgramFromSource(Context context, vector<Device> devices, std::string compilerOptions,
		const char* fileName)
{

	//Read source file
	std::ifstream sourceFile(fileName);
	//std::ifstream sourceFile("./src/kernels/kernel.cl");
	std::string sourceCode(
		std::istreambuf_iterator<char>(sourceFile),
		(std::istreambuf_iterator<char>()));
	Program::Sources source(1, std::make_pair(sourceCode.c_str(), sourceCode.length()+1));

	// Make program of the source code in the context
	Program program = Program(context, source);

	// Build program for these specific devices
	try{
		program.build(devices, compilerOptions.c_str());
	}
	catch (Error error) {

		if(error.err() == CL_BUILD_PROGRAM_FAILURE) {

			for(unsigned i=0;i<devices.size(); i++){
				cout << "Build log:" << endl << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[i]) << endl;
			}
		}
	}

	return program;
}

int OpenCLUtils::GetBinarySize(const char *filename, char* &buffer)
{
    try
    {
        std::ifstream is;
        is.open (filename, std::ios::binary);
        is.seekg (0, std::ios::end);
        int length = is.tellg();
        is.seekg (0, std::ios::beg);
        buffer = new char [length];
        is.read (buffer, length);
        is.close();
        return length;
    }
    catch(...) {
    	//std::cout << "Binary read function failure" << std::endl;
    }

    return 0;
}


Program OpenCLUtils::CreateProgramFromBinary(Context context, vector<Device> devices, const char* fileName)
{
	char *bin;
	int bin_length = GetBinarySize(fileName, bin);

	Program::Binaries   binary(1, std::make_pair(bin, bin_length));
	Program             program = Program(context, devices, binary);
	program.build(devices);

	delete [] bin;

	return program;
}

void OpenCLUtils::SaveProgramBinary(Program program, vector<Device> devices, const char* fileName)
{
    //cout << "Attempting to save binary code to file \"" << fileName << "\"" << endl;

    std::ofstream bfile(fileName, std::ios::binary);

    vector<int> sizes;
    vector<char*> binaries;

    program.getInfo(CL_PROGRAM_BINARY_SIZES, &sizes);

    int numDevices = sizes.size();

    unsigned char **programBinaries = new unsigned char*[numDevices];
	for (int i = 0; i < numDevices; i++)
	{
		programBinaries[i] = new unsigned char[sizes[i]];
	}

	//Get all of the program binaries
	int errNum = program.getInfo(CL_PROGRAM_BINARIES, programBinaries);

	if (errNum != CL_SUCCESS)
	{
		std::cerr << "Error querying for program binaries" << std::endl;

		for (int i = 0; i < numDevices; i++)
		{
			delete [] programBinaries[i];
		}
		delete [] programBinaries;
	}

	// Store the binaries for the device to disk
	FILE *fp = fopen(fileName, "ab");
	for (int i = 0; i < numDevices; i++)
	{
		//if (devices[i] == device)
		//{
			fwrite(programBinaries[i], 1, sizes[i], fp);
			fclose(fp);
			break;
		//}
	}
}

void OpenCLUtils::ShowDeviceInfo(Device device)
{
	std::cout << "Device Name: " << device.getInfo<CL_DEVICE_NAME>() << std::endl;
	std::cout << "Device Type: " << device.getInfo<CL_DEVICE_TYPE>();
	std::cout << " (GPU: " << CL_DEVICE_TYPE_GPU << ", CPU: " << CL_DEVICE_TYPE_CPU << ")" << std::endl;
	std::cout << "Device Vendor: " << device.getInfo<CL_DEVICE_VENDOR>() << std::endl;
	std::cout << "Device Max Compute Units: " << device.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>() << std::endl;
	std::cout << "Device Global Memory: " << device.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>() << std::endl;
	std::cout << "Device Max Clock Frequency: " << device.getInfo<CL_DEVICE_MAX_CLOCK_FREQUENCY>() << std::endl;
	std::cout << "Device Max Allocateable Memory: " << device.getInfo<CL_DEVICE_MAX_MEM_ALLOC_SIZE>() << std::endl;
	std::cout << "Device Local Memory: " << device.getInfo<CL_DEVICE_LOCAL_MEM_SIZE>() << std::endl;
	std::cout << "Device Available: " << device.getInfo< CL_DEVICE_AVAILABLE>() << std::endl;
}


double OpenCLUtils::getElapsedTime(Event evt){

	double elapsed = evt.getProfilingInfo<CL_PROFILING_COMMAND_END>() -
	            evt.getProfilingInfo<CL_PROFILING_COMMAND_START>();

    return (double)(elapsed)/1000000000.0;
}
