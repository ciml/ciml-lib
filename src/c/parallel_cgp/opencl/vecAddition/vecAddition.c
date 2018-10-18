#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CL/cl.h>

#define SIZE 524288

#define PROGRAM_FILE "vecAdd.cl"


cl_device_id create_device() {

   cl_platform_id platform;
   cl_device_id dev;
   int err;

   /* Identify a platform */
   err = clGetPlatformIDs(1, &platform, NULL);
   if(err < 0) {
      perror("Couldn't identify a platform");
      exit(1);
   } 

   /* Access a device */
   err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &dev, NULL);
   if(err == CL_DEVICE_NOT_FOUND) {
      err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &dev, NULL);
   }
   if(err < 0) {
      perror("Couldn't access any devices");
      exit(1);   
   }

   return dev;
}

/**
 * Function: build_program
 * This function reads from the file
 * the functions created and build an program.
 * 
 * ctx: the context
 * dev : the device
 * filename: the file which containst the functions
 * 
 * returns:this function return the program
*/
cl_program build_program(cl_context ctx, cl_device_id dev, const char* filename){

    FILE* program_handle = fopen(filename, "r");
    size_t program_size, log_size;
    char *program_buffer, *program_log;
    cl_int err;

    if(program_handle == NULL){
        perror("Couldn't find the program file");
        exit(1);
    }

    /** First we go to the end of the file and save it size in program_size,
     * then we allocate the program_buffer and read everithing from the file
     * and store at program_buffer.
    */
    fseek(program_handle, 0, SEEK_END);
    program_size = ftell(program_handle);
    rewind(program_handle);
    program_buffer = (char*)malloc(program_size + 1);
    program_buffer[program_size] = '\0';
    if(fread(program_buffer, sizeof(char), program_size, program_handle)){};
    fclose(program_handle);

    /*Create program from file*/
    cl_program program = clCreateProgramWithSource(ctx, 1, (const char**)&program_buffer,
         &program_size, &err);
    
    if(err < 0){
        perror("Couldn't create the program.\n");
        exit(1);
    }
    free(program_buffer);
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

/**
 * Function: find_kernel
 * This function finds a kernel with a specified name
 * 
 * kernel: the 1d array with all kernels
 * num_kernels : the number of kernel
 * func_name: the function name
 * 
 * returns:this function return the kernel wanted
*/
cl_kernel find_kernel(cl_kernel *kernels, cl_uint num_kernels, char *func_name){
    char kernel_name[50];
    for(int i = 0; i < num_kernels; i++){
        clGetKernelInfo(kernels[i], CL_KERNEL_FUNCTION_NAME, sizeof(kernel_name),
             &kernel_name, NULL);
        if(strcmp(kernel_name, func_name) == 0){
            printf("Found the kernel at index %u.\n", i);
            return kernels[i];
        }
    }

    perror("Couldn't find the kernel");
    exit(1);

    return NULL;
}

/**
 * Function: deallocate_kernels
 * This function deallocates all kernels
 * 
 * kernel: the 1d array with all kernels
 * num_kernels : the number of kernel
 * 
 * returns:this is a void function
*/
void deallocate_kernels(cl_kernel *kernels, cl_uint num_kernels){
    for(int i = 0; i < num_kernels; i++){
        clReleaseKernel(kernels[i]);
    }
}

cl_kernel* create_kernels(cl_program program, cl_uint* num_kernels){
    cl_kernel *kernels;
    cl_int err;

    err = clCreateKernelsInProgram(program, 0, NULL, num_kernels);
    if(err < 0){
        perror("Couldn't find any kernels\n");
        exit(1);
    }

    kernels = malloc(*(num_kernels) * sizeof(cl_kernel));
    clCreateKernelsInProgram(program, *(num_kernels), kernels, NULL);

    return kernels;
}

int main(){

    cl_uint num_kernels;
    size_t local_size;
    cl_int err;

    int vectorA[SIZE], vectorB[SIZE];
    for(int i = 0; i < SIZE; i++)
    {
        vectorA[i] = i;
        vectorB[i] = i*2;
    }
    int vectorC[SIZE];

    cl_device_id device = create_device();
    err = clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_GROUP_SIZE, 	
        sizeof(local_size), &local_size, NULL);	
    if(err < 0) {
      perror("Couldn't obtain device information");
      exit(1);   
   }

    //num_groups = SIZE / local_size;

    /*Create the first context*/
    cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
    if(err < 0){
        perror("Couldn't create a context");
        exit(1);
    }

    cl_program program = build_program(context, device, PROGRAM_FILE);
    cl_kernel *kernels = create_kernels(program, &num_kernels);

    cl_command_queue cmq = clCreateCommandQueueWithProperties(context, device, 0, &err);
    if(err < 0){
        perror("Couldn't create a commandQueue");
        exit(1);
    }

    cl_mem buffA = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(vectorA), vectorA, &err);
    if(err < 0){
        perror("Couldn't create a buffer object");
        exit(1);
    }
    cl_mem buffB = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(vectorB), vectorB, NULL);
    cl_mem resBuff = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(vectorC), NULL, NULL);

    err = clSetKernelArg(kernels[0], 0, sizeof(cl_mem), &buffA);
    if(err < 0){
        perror("Couldn't set the kernel argument");
        exit(1);
    }
    clSetKernelArg(kernels[0], 1, sizeof(cl_mem), &buffB);
    clSetKernelArg(kernels[0], 2, sizeof(cl_mem), &resBuff);

    size_t work_items_per_kernel = SIZE;

    err = clEnqueueNDRangeKernel(cmq, kernels[0], 1, NULL, &work_items_per_kernel, NULL, 0, NULL, NULL);
    if(err < 0){
        perror("Couldn't enqueue the kernel execution command");
        exit(1);   
    }

    err = clEnqueueReadBuffer(cmq, resBuff, CL_TRUE, 0, sizeof(vectorC), vectorC, 0, NULL, NULL);
    if(err < 0){
        perror("Couldn't enqueue the read buffer command");
        exit(1);   
    }

    for(int i = 0; i < SIZE; i++)
    {
        if(vectorC[i] != i*3)
        {
            perror("Different result found!");
            exit(1);
        }
    }
    printf("Sum finished!\n");

    clReleaseMemObject(buffA);
    clReleaseMemObject(buffB);
    clReleaseMemObject(resBuff);
    deallocate_kernels(kernels, num_kernels);
    clReleaseCommandQueue(cmq);
    clReleaseProgram(program);
    clReleaseContext(context);
    clReleaseProgram(program);

    return 0;
};