__kernel void sum(__global int* data, __global int* sum){
    int id = get_global_id(0);
    
    sum[id] = abs_diff(data[id], sum[id]);
}

__kernel void reduction_scalar(__global float* data, __local float* partial_sums) {

   int lid = get_local_id(0);
   int group_size = get_local_size(0);

   partial_sums[lid] = data[get_global_id(0)];
   barrier(CLK_LOCAL_MEM_FENCE);

   for(int i = group_size/2; i>0; i /= 2) {
      if(lid < i){
         partial_sums[lid] += partial_sums[lid + i];
      }
      barrier(CLK_LOCAL_MEM_FENCE);
   }

   if(lid == 0){
      data[get_group_id(0)] = partial_sums[0];
   }
}