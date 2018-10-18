__kernel void vec_add(__global int* vecA, __global int* vecB, __global int* resul)
{
    int i =  get_global_id(0);

    resul[i] = vecA[i] + vecB[i];
}