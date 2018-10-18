#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#define _CRT_SECURE_NO_WARNINGS
#define PROGRAM_FILE "result_comparison.cl"

#define ARRAY_SIZE 524288
#define NUM_KERNELS 2

#include <math.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <CL/cl.h>

cl_device_id create_device(){
    cl_platform_id platform;
    cl_device_id dev;
    cl_int err;

    err = clGetPlatformIDs(1, &platform, NULL);
    if(err < 0){
        perror("Couldn't get platform");
        exit(1);
    }

    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &dev, NULL);
    if(err == CL_DEVICE_NOT_FOUND){
        err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &dev, NULL);
    }
    if(err < 0){
        perror("Couldn't get device");
        exit(1);
    }

    return dev;
}

cl_program build_program(cl_context ctx, cl_device_id dev, const char* filename) {

   cl_program program;
   FILE *program_handle;
   char *program_buffer, *program_log;
   size_t program_size, log_size;
   int err;

   /* Read program file and place content into buffer */
   program_handle = fopen(filename, "r");
   if(program_handle == NULL) {
      perror("Couldn't find the program file");
      exit(1);
   }

   fseek(program_handle, 0, SEEK_END);
   program_size = ftell(program_handle);
   rewind(program_handle);

   program_buffer = (char*)malloc(program_size + 1);
   program_buffer[program_size + 1] = '\0';
   if(fread(program_buffer, sizeof(char), program_size, program_handle)){};
   fclose(program_handle);

   /* Create program from file */
   program = clCreateProgramWithSource(ctx, 1, 
      (const char**)&program_buffer, &program_size, &err);
   if(err < 0) {
      perror("Couldn't create the program");
      exit(1);
   }
   free(program_buffer);

   /* Build program */
   err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
   if(err < 0) {

      /* Find size of log and print to std output */
      clGetProgramBuildInfo(program, dev, CL_PROGRAM_BUILD_LOG, 
            0, NULL, &log_size);
      program_log = (char*) malloc(log_size + 1);
      program_log[log_size] = '\0';
      clGetProgramBuildInfo(program, dev, CL_PROGRAM_BUILD_LOG, 
            log_size + 1, program_log, NULL);
      printf("%s\n", program_log);
      free(program_log);
      exit(1);
   }

   return program;
}

int main(){
    
    srand(time(NULL));

    cl_device_id device;
    cl_context context;
    cl_program program;
    cl_command_queue queue;
    cl_kernel kernel[NUM_KERNELS];
    cl_mem data_buffer, output_buffer;
    cl_int i, err, num_groups;
    size_t local_size, global_size;

    int data[ARRAY_SIZE];
    int output[ARRAY_SIZE];
    int sequencial_sum = 0;

    for(i = 0; i < ARRAY_SIZE; i++){
        data[i] = rand() % 2;
        output[i] = rand() % 2;
        if(data[i] != output[i]){
            sequencial_sum += 1;
        }
    }


    device = create_device();
    err = clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(local_size), &local_size, NULL);
    if(err < 0){
        perror("Couldn't get max work group size");
        exit(1);
    }

    global_size = ARRAY_SIZE;
    num_groups = ARRAY_SIZE / local_size;

    context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
    if(err < 0){
        perror("Couldn't create context");
        exit(1);
    }

    program = build_program(context, device, PROGRAM_FILE);

    data_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
        sizeof(data), &data, &err);
    output_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
        sizeof(output), &output, &err);
    if(err < 0){
        perror("Couldn't create buffer");
        exit(1);
    }

    queue = clCreateCommandQueue(context, device, 0, &err);
    if(err < 0){
        perror("Couldn't create command queue");
        exit(1);
    }

    kernel[0] = clCreateKernel(program, "sum", &err);
    if(err < 0){
        perror("Couldn't create kernel");
        exit(1);
    }
    kernel[1] = clCreateKernel(program, "reduction_scalar", &err);
    if(err < 0){
        perror("Couldn't create kernel 2");
        exit(1);
    }

    err = clSetKernelArg(kernel[0], 0, sizeof(cl_mem), &data_buffer);
    err |= clSetKernelArg(kernel[0], 1, sizeof(cl_mem), &output_buffer);
    if(err < 0){
        perror("Couldn't set kernel argument");
        exit(1);
    }

    err = clEnqueueNDRangeKernel(queue, kernel[0], 1, NULL, &global_size,
        NULL, 0, NULL, NULL);
    if(err < 0){
        perror("Couldn't enqueue the kernel");
        exit(1);
    }
    

    err = clSetKernelArg(kernel[1], 0, sizeof(cl_mem), &output_buffer);
    err |= clSetKernelArg(kernel[1], 1, sizeof(float) * local_size, NULL);
    if(err < 0){
        perror("Couldn't set kernel argument 2");
        exit(1);
    }

    err = clEnqueueNDRangeKernel(queue, kernel[1], 1, NULL, &global_size,
        &local_size, 0, NULL, NULL);
    if(err < 0){
        perror("Couldn't enqueue the kernel 2");
        exit(1);
    }
    err = clEnqueueReadBuffer(queue, output_buffer, CL_TRUE, 0, sizeof(int) * ARRAY_SIZE,
        output, 0, NULL, NULL);
    if(err < 0){
        perror("Couldn't enqueue read buffer");
        exit(1);
    }
    

    int sum = 0;
    for(i = 0; i < num_groups; i++){
        sum += output[i];
    }

    if(sequencial_sum == sum){
        printf("It works \\o/\n");
    }

    clReleaseMemObject(data_buffer);
    clReleaseKernel(kernel[0]);
    clReleaseKernel(kernel[1]);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
    clReleaseDevice(device);

}