void avaliaIndividuos(Arvore pop[], float* dados[], int M, int N){

    int i, j = 0;
    float erro = 0;
    for(i = 0; i < NUM_INDIV; i++){
        for(j = 0; j < M; j++){
            erro = erro + executa(&pop[i], dados[j], N);
        }
        //int k; scanf("%d", &k);
        pop[i].aptidao = erro; // + 2 * pop[i].numNos;
        erro = 0;
    }
}


    __kernel void somatorioSequencialEsperto(__global float* values,
                                             __local float* localSum,
                                             __const int length,
                                             __global float* sum){
        int global_id = get_global_id(0);                               
        int local_id = get_local_id(0);
        int group_id = get_group_id(0);
        float accumulator = 0;

        while(global_id < length){
            accumulator += values[global_id];
            global_id += get_global_size(0);
        }

        localSum[local_id] = accumulator;
        barrier(CLK_LOCAL_MEM_FENCE);

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


__kernel void avaliaIndividuos(__global Arvore* pop,
                               __global float* dados,
                               __local float* error, 
                               __const int M, 
                               __const int N){
    int i, j = 0;
    float erro = 0;

    int local_id = get_local_id();
    int local_size = get_local_size();
    int group_id = get_group_id();

    for(j = 0; j < (M/local_size) - 1; j++){
        int n = j * local_size + local_id;
        if( n < M){
            erro = erro + executa(&pop[group_id], dados[n], N);
        }
    }
    //mudar nome dessas variaveis
    error[local_id] = erro;
    barrier(CLK_LOCAL_MEM_FENCE);

    ///redução erros por work group
    for(int i = get_local_size(0)/2 ; i > 0; i/=2){
        if(local_id < i){
           error[local_id] += error[local_id + i];
        }
        barrier(CLK_LOCAL_MEM_FENCE);
    }
        
    if(local_id == 0){
        pop[group_id].aptidao = error[0];
    }

}