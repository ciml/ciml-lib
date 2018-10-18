
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#define _CRT_SECURE_NO_WARNINGS
#define PROGRAM_FILE "vecMult.cl"

#define ARRAY_SIZE 524288
#define NUM_KERNELS 2

#include <math.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <CL/cl.h>

/*Find a CPU or GPU associated with the first available platform*/
cl_device_id create_device()
{
    cl_platform_id platform;
    cl_device_id dev;
    cl_int err;

    /*Identify a platform*/
    err = clGetPlatformIDs(1, &platform, NULL);
    if (err < 0)
    {
        perror("Couldn't find any platform.");
        exit(1);
    }

    /*Access a device*/
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &dev, NULL);
    if (err == CL_DEVICE_NOT_FOUND)
    {
        err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &dev, NULL);
    }
    if (err < 0)
    {
        perror("Couldn't find any device");
        exit(1);
    }

    return dev;
}

/* Create program from a file and compile it */
cl_program build_program(cl_context ctx, cl_device_id dev, const char *filename)
{

    cl_program myprog;
    size_t program_size;
    FILE *program_handle;
    char *program_buffer;
    cl_int err;

    program_handle = fopen(filename, "r");
    if (program_handle == NULL)
    {
        perror("Couldn't read the program");
        exit(1);
    }
    /*Seeks for the end of the program and get it sizes*/
    fseek(program_handle, 0, SEEK_END);
    program_size = ftell(program_handle);
    rewind(program_handle);

    /*It allocate the program_buffer and read it from the file*/
    program_buffer = (char *)malloc(program_size + 1);
    program_buffer[program_size] = '\0';
    if (fread(program_buffer, sizeof(char), program_size, program_handle))
    {
    };

    /*Create program file*/
    myprog = clCreateProgramWithSource(ctx, 1, (const char **)&program_buffer,
                                       &program_size, &err);
    if (err < 0)
    {
        perror("Couldn't create a program");
        exit(1);
    }

    /*Build a program*/
    err = clBuildProgram(myprog, 0, NULL, NULL, NULL, NULL);
    if (err < 0)
    {
        /*If build program fails we get the error log and print it*/
        size_t log_size;
        clGetProgramBuildInfo(myprog, dev, CL_PROGRAM_BUILD_LOG,
                              0, NULL, &log_size);
        char *program_log = (char *)malloc(log_size + 1);
        program_log[log_size] = '\0';
        clGetProgramBuildInfo(myprog, dev, CL_PROGRAM_BUILD_LOG, log_size + 1,
                              program_log, NULL);
        printf("%s\n", program_log);
        perror("Couldn't build the program");
        exit(1);
    }

    return myprog;
}

int main()
{

    /* OpenCL structures */
    cl_device_id device;
    cl_context context;
    cl_program program;
    cl_kernel kernel[NUM_KERNELS];
    cl_command_queue queue;
    cl_event prof_event;
    cl_int i, j, err;
    size_t local_size, global_size;
    char kernel_name[NUM_KERNELS][20] = {"reduction_scalar", "vec_add"};

    /*Data and buffers*/
    float data[ARRAY_SIZE];
    float data_a[ARRAY_SIZE];
    float data_b[ARRAY_SIZE];
    float sum, *scalar_sum;
    cl_mem data_buffer, scalar_sum_buffer;
    cl_mem data_a_buffer, data_b_buffer;
    cl_int num_groups;
    cl_ulong time_start, time_end, total_time;

    /*Initialize data*/
    for (i = 0; i < ARRAY_SIZE; i++)
    {
        data_a[i] = 2.0f;
        data_b[i] = 2.0f;
    }

    device = create_device();
    err = clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(local_size),
                          &local_size, NULL);
    if (err < 0)
    {
        perror("Couldn't get max work group size");
        exit(1);
    }

    /*Allocate and initialize output arrays*/
    num_groups = ARRAY_SIZE / local_size;
    scalar_sum = (float *)malloc(sizeof(float) * num_groups);
    for (i = 0; i < num_groups; i++)
    {
        scalar_sum[i] = 0.0f;
    }

    /*Create context*/
    context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
    if (err < 0)
    {
        perror("Couldn't create context");
        exit(1);
    }

    /*Build program*/
    program = build_program(context, device, PROGRAM_FILE);

    data_a_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                   ARRAY_SIZE * sizeof(float), data_a, &err);
    data_b_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                   ARRAY_SIZE * sizeof(float), data_b, &err);
    data_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
                                 ARRAY_SIZE * sizeof(float), data, &err);
    scalar_sum_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
                                       sizeof(float) * num_groups, scalar_sum, &err);
    if (err < 0)
    {
        perror("Couldn't create buffers");
        exit(1);
    }

    /*Create command queue*/
    queue = clCreateCommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &err);
    if (err < 0)
    {
        perror("Couldn't create command queue");
        exit(1);
    }

    /*Create a kernel*/
    for (i = 0; i < NUM_KERNELS; i++)
    {
        kernel[i] = clCreateKernel(program, kernel_name[i], &err);
        if (err < 0)
        {
            perror("Couldn't create kernels");
            exit(1);
        }
        global_size = ARRAY_SIZE;
        if (i == 0)
        {
            err = clSetKernelArg(kernel[i], 0, sizeof(cl_mem), &data_buffer);
            err |= clSetKernelArg(kernel[i], 1, sizeof(float) * local_size, NULL);
            err |= clSetKernelArg(kernel[i], 2, sizeof(cl_mem), &scalar_sum_buffer);
        }
        if (i == 1)
        {
            err = clSetKernelArg(kernel[i], 0, sizeof(cl_mem), &data_a_buffer);
            err |= clSetKernelArg(kernel[i], 1, sizeof(cl_mem), &data_b_buffer);
            err |= clSetKernelArg(kernel[i], 2, sizeof(cl_mem), &data_buffer);
        }
        if (err < 0)
        {
            perror("Couldn't create a kernel argument");
            exit(1);
        }
    }

    /*Enqueue Kernel*/
    err = clEnqueueNDRangeKernel(queue, kernel[1], 1, NULL, &global_size, NULL, 0,
                                 NULL, NULL);
    if (err < 0)
    {
        perror("Couldn't enqueue the kernel");
        exit(1);
    }
    err = clEnqueueReadBuffer(queue, data_buffer, CL_TRUE, 0,
                              ARRAY_SIZE * sizeof(float), data, 0, NULL, NULL);
    if (err < 0)
    {
        perror("Couldn't read the buffer");
        exit(1);
    }
    err = clEnqueueNDRangeKernel(queue, kernel[0], 1, NULL, &global_size, &local_size,
                                 0, NULL, &prof_event);
    if (err < 0)
    {
        perror("Couldn't enqueue the kernel");
        exit(1);
    }

    /* Finish processing the queue and get profiling information */
    clFinish(queue);
    clGetEventProfilingInfo(prof_event, CL_PROFILING_COMMAND_START,
                            sizeof(time_start), &time_start, NULL);
    clGetEventProfilingInfo(prof_event, CL_PROFILING_COMMAND_END,
                            sizeof(time_end), &time_end, NULL);
    total_time = time_end - time_start;

    /*Read the result*/
    err = clEnqueueReadBuffer(queue, scalar_sum_buffer, CL_TRUE, 0,
                              num_groups * sizeof(float), scalar_sum, 0, NULL, NULL);
    if (err < 0)
    {
        perror("Couldn't read the buffer");
        exit(1);
    }
    sum = 0.0f;
    for (j = 0; j < num_groups; j++)
    {
        sum += scalar_sum[j];
    }
    if (sum != ARRAY_SIZE * 4)
    {
        printf("Test got wrong result!\n");
    }
    else
    {
        printf("Everything is okay! Yay \\o/\n");
    }
    printf("Total time: %" PRIu64 "\n", total_time);
    clReleaseEvent(prof_event);
    free(scalar_sum);
    clReleaseKernel(kernel[0]);
    clReleaseKernel(kernel[1]);
    clReleaseMemObject(scalar_sum_buffer);
    clReleaseMemObject(data_buffer);
    clReleaseCommandQueue(queue);
    clReleaseProgram(program);
    clReleaseContext(context);

    return 0;
}