/*
 * OpenCLUtils.h
 *
 *  Created on: 4 de jun de 2016
 *      Author: igor
 */

#ifndef OPENCLUTILS_H_
#define OPENCLUTILS_H_

#include <iostream>
#include <fstream>
#define __NO_STD_VECTOR // Use cl::vector instead of STL version
#define __CL_ENABLE_EXCEPTIONS
#include "CL/cl.h"
#include <string>

using namespace std;
using namespace cl;

/**
 * \brief Class for handling OpenCL common operations.
 *
 * This class is able to compile/create OpenCL kernels from source or binary,
 * and to show the properties of the devices being used.
 */
class OpenCLUtils {
public:
	OpenCLUtils();
	virtual ~OpenCLUtils();

	Program CreateProgramFromSource(Context context, vector<Device> devices, std::string compilerOptions, const char* fileName);
	Program CreateProgramFromBinary(Context context, vector<Device> devices, const char* fileName);
	void SaveProgramBinary(Program program, vector<Device> devices, const char* fileName);

	int GetBinarySize(const char *filename, char* &buffer);
	void ShowDeviceInfo(Device device);
	double getElapsedTime(Event evt);
};

#endif /* OPENCLUTILS_H_ */
