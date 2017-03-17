   
#pragma OPENCL EXTENSION cl_amd_printf : enable

    __kernel void somatorioSequencialEsperto(__global float* values,
                                             __local float* localSum,
                                             __const int length,
                                             __global float* sum){
        int global_id = get_global_id(0);                               
        int local_id = get_local_id(0);
        int group_id = get_group_id(0);
        float accumulator = 0;
        //printf("global id fora = %d \n", global_id);
        while(global_id < length){
            //float valor = values[global_id];
            //printf("%d\n", (int)values[global_id]);
            accumulator += values[global_id];
            global_id += get_global_size(0);
        }

        localSum[local_id] = accumulator;
        barrier(CLK_LOCAL_MEM_FENCE);
        printf("%d\n", (int)accumulator);
        //printf("groupid= %d, %f, \n",group_id, values[local_id]);
        //printf("localsize = %d, globalsize = %d\n", get_local_size(0), get_global_size(0));
        //printf("sum= %f, \n", sum[local_id]);
       
        for(int i = get_local_size(0)/2 ; i > 0; i/=2){
            if(local_id < i){
               localSum[local_id] += localSum[local_id + i];
            }
            barrier(CLK_LOCAL_MEM_FENCE);
        }
        
        if(local_id == 0){
            sum[group_id] = localSum[0];
        }
    }



    __kernel void somatorioEsperto(__global float* values,
                                    __global float* sum){
                                       
        int local_id = get_local_id(0);
        int group_id = get_group_id(0);
        //printf("groupid= %d, %f, \n",group_id, values[local_id]);
        /*printf("localsize = %d, globalsize = %d\n", get_local_size(0), get_global_size(0));*/
        //int nextPower2 = pown((float)2, (int)ceil(log2((float)get_local_size(0))));
        
        for(int i = get_local_size(0)/2 /* nextPower2/2*/; i > 0; i/=2){
            barrier(CLK_LOCAL_MEM_FENCE);
            if(local_id < i/* && (local_id + i < get_local_size(0))*/){
                values[local_id] += values[local_id + i];
            }
        }
        
        if(local_id == 0){
            sum[group_id] = values[0];
        }
    }
    
    __kernel void somatorioTeste(__global float* values,
                                 __global float* sum){
                                       
        int local_id = get_local_id(0);
        int group_id = get_group_id(0);
        int group_size = get_local_size(0);
        //printf("groupid= %d, %f, \n",group_id, values[local_id]);
        /*printf("localsize = %d, globalsize = %d\n", get_local_size(0), get_global_size(0));*/
        //int nextPower2 = pown((float)2, (int)ceil(log2((float)get_local_size(0))));
        
        for(int i = get_local_size(0)/2 /* nextPower2/2*/; i > 0; i/=2){
            barrier(CLK_LOCAL_MEM_FENCE);
            if(local_id < i/* && (local_id + i < get_local_size(0))*/){
                values[local_id + group_id *group_size] += values[local_id +group_size * group_id + i];
            }
        }
        
        if(local_id == 0){
            sum[group_id] = values[group_id * group_size];
        }
    }
    

    __kernel void somatorioIngenuo(__global float* values,
                                    __global float* sum){
                                       
        int local_id = get_local_id(0);
        int group_id = get_group_id(0);

        //int nextPower2 = pown((float)2, (int)ceil(log2((float)get_local_size(0))));
        for(int i = 1 /*nextPower2/2*/; i < get_local_size(0); i*=2){
            barrier(CLK_LOCAL_MEM_FENCE);
            if((local_id / i) % 2 == 0 /*&& (local_id + i < get_local_size(0))*/){
                values[local_id] += values[local_id + i];
            }
        }
        if(local_id == 0){
            sum[group_id] = values[0];
        }
    }
    
    __kernel void raiz(__global float * x, __global float * y) {
       int i = get_global_id(0);
       y[i] = sqrt(x[i]);
    };