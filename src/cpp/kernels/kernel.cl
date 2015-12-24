#include "./src/parametros.h"
#include "./src/representacao.h"
#include "./src/kernels/utils.cl"

/*
#include "./parametros.h"
#include "./representacao.h"
#include "./kernels/utils.cl"*/

typedef anticorpo data_t;

float funcaoObjetivoOneMax_old(__local unsigned * binarios, int lid, int localSize){

	float soma = 0;
	int i,j,k, val;

	for(i=0; i < TAMANHO_ANTICORPO; i++){

		val = binarios[i];

		for(j=0; j < BITS_PER_WORD; j++){
			soma += BIT_CHECK(val, j)/(int)pow(2.0,j);
		}
	}
	
	return -soma;
}

float funcaoObjetivoOneMax(unsigned * binarios, int lid, int localSize){
    
    __local float partial[BITS_PER_WORD];
    //if(lid < 32){

	/*for(int i=0; i < TAMANHO_ANTICORPO; i++){
		soma += binarios[i];
	}*/
    partial[lid]=0;

    for (int j=lid;j < TAMANHO_ANTICORPO; j+=localSize)
    {
       int a = binarios[j];
       for(int i = 0; i < 4; i++){
    	   partial[lid] += BIT_CHECK(a, i)/(int)native_powr(2.0,i);
       }
    }
   
    int next_power_of_2 = BITS_PER_WORD; //(int)native_powr( (float)2.0, (int) ceil( log2( (float) localSize ) ) );
    
    for(int s = next_power_of_2/2; s>0; s=s>>1){
        barrier(CLK_LOCAL_MEM_FENCE);
        if(lid < s && lid+s < localSize){	 
    	    //int value = posicao[lid+s];
            partial[lid] += partial[lid+s];
        }
    }
    //}
    
    barrier(CLK_LOCAL_MEM_FENCE);  
    
    return -partial[0];    
    //return soma*(-1);
}

bool isSet(unsigned b, unsigned n) { return b & ( 1 << n); }

float funcaoObjetivoOneMax2(__local unsigned * binarios, int lid_0, int lid_1, int localSize){
    
    //__local float partial[8][4];
	__local float partial[32];
    //if(lid < 32){

	/*for(int i=0; i < TAMANHO_ANTICORPO; i++){
		soma += binarios[i];
	}*/
    
    partial[lid_0*4+lid_1] = 0;
    
    //barrier(CLK_LOCAL_MEM_FENCE);    
	
    float soma = 0;  
    
    //unsigned mask = 1;
    
	for (int j=lid_1; j < TAMANHO_ANTICORPO; j+=4)
	{
	   int a = binarios[j];
	   for(int i = 0; i < BITS_PER_WORD; i++){
		   //soma += BIT_CHECK(a, i)/(int)native_powr(2.0,i);
		   soma += isSet(a, i);
		   //soma += a & ( 1 << i) != 0;
		   //soma += (a & mask) != 0;
		   //mask = 1 << mask;
	   }
	   //partial[lid_0][lid_1] += countSetBits(a); 
	}
	
	partial[lid_0*4 + lid_1] = soma;
	
	barrier(CLK_LOCAL_MEM_FENCE);
	
	int next_power_of_2 = 4; //(int)native_powr( (float)2.0, (int) ceil( log2( (float) localSize ) ) );
	
	soma = 0;
	
	//for(int s = 0; s<4; s+=1){		
		//soma += partial[lid_0*4+s];
		//barrier(CLK_LOCAL_MEM_FENCE);
	//}
	
	for(int s = next_power_of_2/2; s>0; s=s>>1){
		barrier(CLK_LOCAL_MEM_FENCE);
		if(lid_1 < s && ((lid_1+s) < 4)){	 
			//int value = posicao[lid+s];
			partial[lid_0*4+lid_1] += partial[lid_0*4+lid_1+s];
		}
	}
	
	barrier(CLK_LOCAL_MEM_FENCE);
	
    return -partial[lid_0*4];
}

void avaliaAnticorpo(__global anticorpo * a, int lid, int localSize){
    
	unsigned binarios[BITS_POR_DIMENSAO * DIMENSOES];

	gray_para_binario(a->representacao, binarios);    

	if(FUNCAO_OBJETIVO == ONE_MAX){
		a->afinidade = funcaoObjetivoOneMax(binarios, lid, localSize);
	}
}

void avaliaAnticorpo_l(__local anticorpo * a, int lid_0, int lid_1, int localSize){
    
    //__local short binarios[BITS_POR_DIMENSAO * DIMENSOES];
    
    /*if(lid==0){
        gray_para_binario_l(a->representacao, binarios);
    }
    barrier(CLK_LOCAL_MEM_FENCE);*/
    
	if(FUNCAO_OBJETIVO == ONE_MAX){
		a->afinidade = funcaoObjetivoOneMax2(a->representacao, lid_0, lid_1, localSize);
	}
}

void avaliaAnticorpo_p(anticorpo * a, int lid, int localSize){
    
    short binarios[BITS_POR_DIMENSAO*BITS_PER_WORD * DIMENSOES];
    
    //if(lid==0)
    //gray_para_binario_p(a->representacao, binarios);
    
	if(FUNCAO_OBJETIVO == ONE_MAX){
		a->afinidade = funcaoObjetivoOneMax(binarios, lid, localSize);
	}
}

int numClones(int popSize, float taxaClonagem, int indice){
	return (int)ceil( (float)(taxaClonagem * popSize) / (float)indice);
}

/*float calculaTaxaHiperMutacao(float afinidadeNormalizada){
	return exp(-1*FATOR_MUTACAO * afinidadeNormalizada);
}*/

__kernel void inicializaPopulacao_(__global anticorpo *pop, __global int * D_seeds, 
								  __constant struct KernelParameters* parameters){

    int tid = get_global_id(0),
        lid = get_local_id(0),
        gid = get_group_id(0),
        localSize = get_local_size(0);
    
    int lid_0 = lid / 4;
    int lid_1 = lid % 4;

    int seed = D_seeds[tid];
    
    __local anticorpo novo;
    int a;
    
    int P_REAL_LEN = parameters->REAL_LEN;
    
    for( uint i = lid; i < P_REAL_LEN ; i+=localSize )
    {   
        //novo.representacao[i] = rand(&seed) % 2;    
        a = 0;
        for(uint j=0; j < BITS_PER_WORD; j++){
            if(u_rand(&seed) <= 0.5){
                BIT_SET(a, j);
            }            
        }
        novo.representacao[i] = a;        
    }
    
    D_seeds[tid] = seed;

   /* barrier(CLK_LOCAL_MEM_FENCE);    
    avaliaAnticorpo_l(&novo, lid_0, lid_1, localSize);    
    
    barrier(CLK_LOCAL_MEM_FENCE);    */
    
    if(lid_0 == 0)
        pop[gid].afinidade = novo.afinidade;
    
    barrier(CLK_LOCAL_MEM_FENCE);    
    
    for(uint i = lid; i < P_REAL_LEN; i+=localSize)
    {
        pop[gid].representacao[i] = novo.representacao[i];    
    }
}

/*
__kernel void inicializaPopulacao(__global anticorpo *pop, __global int * D_seeds, 
								  __constant struct KernelParameters* parameters){
	
	int id_individuo = get_global_id(0),
	id_genotipo = get_global_id(1);
	
	if(id_individuo < parameters->POP_SIZE && id_genotipo < parameters->REAL_LEN){
    
	    int tid = id_individuo*get_global_size(1) + id_genotipo;
	    int seed = D_seeds[tid];
	        
	    unsigned word = 0;    
	    
	    for(uint j=0; j < BITS_PER_WORD; j++){
	        
        	word = word << 1;      	
  			
  			if(rand(&seed) % 2){
  				word = word | 1;
  			}      	
        	        	
			//word = u_rand(&seed) <= 0.5f ? word << 1 | 1 : word << 1;
	    }
	    
	    pop[id_individuo].representacao[id_genotipo] = word;    
	    D_seeds[tid] = seed;
    }
}
*/

__kernel void inicializaPopulacao(__global unsigned *pop, 
								  __global int * D_seeds, 
								  __constant struct KernelParameters* parameters){
	
	//int id_individuo = get_global_id(0),
	//id_genotipo = get_global_id(1);
	
	int tid = get_global_id(0);	
	
	if(tid < parameters->POP_SIZE * parameters->REAL_LEN){
    
	    int seed = D_seeds[tid];	        
	    unsigned word = 0;    
	    
	    for(uint j=0; j < BITS_PER_WORD; j++){
	        
        	word = word << 1;      	
  			
  			if(u_rand(&seed) <= 0.5){
  				word = word | 1;
  			}
        	        	
			//word = u_rand(&seed) <= 0.5f ? word << 1 | 1 : word << 1;
	    }
	    
	    pop[tid] = word;    
	    D_seeds[tid] = seed;
    }
}

/*
// Call: <<< POP_SIZE, MAX_THREADS_PER_BLOCK >>>
__kernel void Fitness(__global anticorpo *pop, 
					__constant struct KernelParameters* parameters){

	__local int partial[1024];
	int gid = get_group_id(0); 
	int localID = get_local_id(0);
	partial[localID] = 0;
	
	int i,j;
	unsigned aux, shift, value;
	unsigned mask = CONST2;
	
	for(i=localID; i < parameters->REAL_LEN; i+=MAX_THREADS_PER_BLOCK) {
	 	aux   = pop[gid].representacao[i];
		shift = mask;
				
		for(j=0;j<BITS_PER_WORD && (i*BITS_PER_WORD+j<parameters->CHROM_LEN) ; j+=1) {
		
		   value = (aux & shift)==0?0:1;
		   shift = shift >> 1;
		   partial[localID] = partial[localID] + value;		   
		}	
	}
	
	barrier(CLK_LOCAL_MEM_FENCE); 

    i = get_local_size(0)/2;
    while (i!=0) {
       	if (localID < i)
	  		partial[localID] += partial[localID + i]; 
       	barrier(CLK_LOCAL_MEM_FENCE);
		i = i/2;
    }
       
    if (localID == 0)
		pop[gid].afinidade = partial[0];		
}
*/

#define MAX_THREADS_PER_BLOCK 1024
/*
// Call: <<< POP_SIZE, MAX_THREADS_PER_BLOCK >>>
__kernel void Fitness(__global unsigned *pop,
					  __global float *fitness, 
					__constant struct KernelParameters *parameters){

	__local int partial[64];
	int gid = get_group_id(0); 
	int localID = get_local_id(0);
	partial[localID] = 0;
	
	int i,j;
	unsigned aux, shift, value;
	//unsigned mask = CONST1;
	
	for(i=localID; i < parameters->REAL_LEN; i+=get_local_size(0)) {
	 	//aux   = pop[gid].representacao[i];
	 	aux = pop[gid*parameters->REAL_LEN + i];
		shift = MASK;
				
		for(j=0;j<BITS_PER_WORD && (i*BITS_PER_WORD+j<parameters->CHROM_LEN) ; j+=1) {
		
		   value = (aux & shift) == 0 ? 0 : 1;
		   shift = shift >> 1;
		   partial[localID] = partial[localID] + value;		   
		}	
	}
	
	barrier(CLK_LOCAL_MEM_FENCE); 

    i = get_local_size(0)/2;
    while (i!=0) {
       	if (localID < i)
	  		partial[localID] += partial[localID + i]; 
       	barrier(CLK_LOCAL_MEM_FENCE);
		i = i/2;
    }
       
    if (localID == 0)
		fitness[gid] = -partial[0];		
}*/



__kernel void Fitness(__global unsigned *pop,
					  __global float *fitness, 
					  __constant struct KernelParameters *parameters){

	
	__local int partial[32];

	int gid = get_group_id(0); 
	int localID = get_local_id(0);
	int localSize = get_local_size(0);
	partial[localID] = 0;
	
	int i,j;
	unsigned aux, shift, value;
	//unsigned mask = CONST1;
	
	if(localSize <= parameters->REAL_LEN){
	
		for(i=localID; i < parameters->REAL_LEN; i+=localSize) {
		 	//aux   = pop[gid].representacao[i];
		 	aux = pop[gid*parameters->REAL_LEN + i];
			shift = MASK;
				
			for(j=0;j<BITS_PER_WORD && (i*BITS_PER_WORD+j<parameters->CHROM_LEN) ; j+=1) {
		
			   value = (aux & shift) == 0 ? 0 : 1;
			   shift = shift >> 1;
			   partial[localID] = partial[localID] + value;		   
			}	
		}
	}
	else{
	
		__local unsigned copia[32];
		
		for(i=localID; i < parameters->REAL_LEN; i+=localSize) {
			copia[i] = pop[gid*parameters->REAL_LEN + i];
		}
		
		barrier(CLK_LOCAL_MEM_FENCE);
				
		for(i=0; i < parameters->REAL_LEN; i+=1) {
		 	
		 	aux = copia[i];
			//shift = MASK >> localID;
				
			for(j=localID; j < BITS_PER_WORD; j+=localSize) {
		
			   value = (aux & (MASK >> j) ) == 0 ? 0 : 1;
			   partial[localID] += value;		   			   
			   //shift = shift >> localSize;
			}	
		}
	}	
	
	barrier(CLK_LOCAL_MEM_FENCE); 

    i = get_local_size(0)/2;
    while (i!=0) {
       	if (localID < i)
	  		partial[localID] += partial[localID + i]; 
       	barrier(CLK_LOCAL_MEM_FENCE);
		i = i/2;
    }
       
    if (localID == 0)
		fitness[gid] = -partial[0];		
}

__kernel void CalculateAffinity(__global float * fitness,
								__global float * fitnessNorm,
								__global t_stats *est,
								__constant struct KernelParameters *parameters){
								
	int tid = get_global_id(0);
	
	if(tid < parameters->POP_SIZE){	
	
		float max = est->afinidadeMelhor,
			  min = est->afinidadePior;
			  
	  	if(max-min == 0.0){
			fitnessNorm[tid] = 0.0f;
		}
		else{
			float n = (fitness[tid]- min) / (max-min);
			n = (MAXIMIZACAO == 0) ? 1.0f-n : n;
			fitnessNorm[tid] = n;
		}	
	}
}


__kernel void StatisticsReduction1(__global unsigned *pop, 
								   __global float * fitness,
								  __global t_stats *est,
		__constant struct KernelParameters* parameters){

	//TODO: Two-stages reduction
	
	int tid = get_global_id(0),
        lid = get_local_id(0),
        gid = get_group_id(0),
        localSize = get_local_size(0);
        
	int P_POP_SIZE = parameters->POP_SIZE;
     
	__local float afinidades[32];
	__local int posicao[32];
			
	afinidades[lid] = fitness[lid];
	posicao[lid] = lid;
	   
	 //Encontra o anticorpo com a maior afinidade
	 if(gid==1){      
		
		//Itera por blocos de tamanho localSize (uma comparação por work-item)
		for (int i=localSize+lid; i < P_POP_SIZE; i+=localSize)
		{
			if(fitness[i] > afinidades[lid]){
				afinidades[lid] = fitness[i];
				posicao[lid] = i;
			}
		}
		
		//Redução 
		int next_power_of_2 = (int)native_powr( (float)2.0, (int) ceil( log2( (float) localSize ) ) );
	
		for(int s = next_power_of_2/2; s>0; s=s>>1){
			barrier(CLK_LOCAL_MEM_FENCE);
			if(lid < s && lid+s < localSize){	 
				//int value = posicao[lid+s];
				if(afinidades[lid+s]>afinidades[lid]){
					afinidades[lid] = afinidades[lid+s];
					posicao[lid] = posicao[lid+s];        	    
				}
			}
		}    
		barrier(CLK_LOCAL_MEM_FENCE);
		
		if(lid==0){
			est[0].indiceMelhor = posicao[0];
			est[0].afinidadeMelhor = afinidades[0];
		}
		
	 }
	 //Encontra o anticorpo com a menor afinidade
	 else{
	 
		//Itera por blocos de tamanho localSize (uma comparação por work-item)
		for (int i=localSize+lid; i< P_POP_SIZE; i+=localSize)
		{
			if(fitness[i] < afinidades[lid]){
				afinidades[lid] = fitness[i];
				posicao[lid] = i;
			}
		}     
		 
		//Redução 
		int next_power_of_2 = (int)native_powr( (float)2.0, (int) ceil( log2( (float) localSize ) ) );
	
		for(int s = next_power_of_2/2; s>0; s=s>>1){
			barrier(CLK_LOCAL_MEM_FENCE);
			if(lid < s && lid+s < localSize){	 
				//int value = posicao[lid+s];
				if(afinidades[lid+s] < afinidades[lid]){
					afinidades[lid] = afinidades[lid+s];
					posicao[lid] = posicao[lid+s];        	    
				}
			}
		}
		barrier(CLK_LOCAL_MEM_FENCE);
		
		if(lid==0){
			est[0].indicePior = posicao[0];
			est[0].afinidadePior = afinidades[0];
		}
	 }
}


/*__kernel void StatisticsReduction1(__global anticorpo *pop, 
								  __global t_stats *est,
		__constant struct KernelParameters* parameters){

	//TODO: Two-stages reduction
	
	int tid = get_global_id(0),
        lid = get_local_id(0),
        gid = get_group_id(0),
        localSize = get_local_size(0);
        
	int P_POP_SIZE = parameters->POP_SIZE;
     
	__local float afinidades[128];
	__local int posicao[128];
			
	afinidades[lid] = pop[lid].afinidade;
	posicao[lid] = lid;
	   
	 //Encontra o anticorpo com a maior afinidade
	 if(gid==1){      
		
		//Itera por blocos de tamanho localSize (uma comparação por work-item)
		for (int i=localSize+lid; i < P_POP_SIZE; i+=localSize)
		{
			if(pop[i].afinidade > afinidades[lid]){
				afinidades[lid] = pop[i].afinidade;
				posicao[lid] = i;
			}
		}
		
		//Redução 
		int next_power_of_2 = (int)native_powr( (float)2.0, (int) ceil( log2( (float) localSize ) ) );
	
		for(int s = next_power_of_2/2; s>0; s=s>>1){
			barrier(CLK_LOCAL_MEM_FENCE);
			if(lid < s && lid+s < localSize){	 
				//int value = posicao[lid+s];
				if(afinidades[lid+s]>afinidades[lid]){
					afinidades[lid] = afinidades[lid+s];
					posicao[lid] = posicao[lid+s];        	    
				}
			}
		}    
		barrier(CLK_LOCAL_MEM_FENCE);
		
		if(lid==0){
			est[0].indiceMelhor = posicao[0];
			est[0].afinidadeMelhor = afinidades[0];
		}
		
	 }
	 //Encontra o anticorpo com a menor afinidade
	 else{
	 
		//Itera por blocos de tamanho localSize (uma comparação por work-item)
		for (int i=localSize+lid; i< P_POP_SIZE; i+=localSize)
		{
			if(pop[i].afinidade < afinidades[lid]){
				afinidades[lid] = pop[i].afinidade;
				posicao[lid] = i;
			}
		}     
		 
		//Redução 
		int next_power_of_2 = (int)native_powr( (float)2.0, (int) ceil( log2( (float) localSize ) ) );
	
		for(int s = next_power_of_2/2; s>0; s=s>>1){
			barrier(CLK_LOCAL_MEM_FENCE);
			if(lid < s && lid+s < localSize){	 
				//int value = posicao[lid+s];
				if(afinidades[lid+s] < afinidades[lid]){
					afinidades[lid] = afinidades[lid+s];
					posicao[lid] = posicao[lid+s];        	    
				}
			}
		}
		barrier(CLK_LOCAL_MEM_FENCE);
		
		if(lid==0){
			est[0].indicePior = posicao[0];
			est[0].afinidadePior = afinidades[0];
		}
	 }
}
*/

/*
__kernel void iteracaoClonalg(__global unsigned *pop, 
							  __global unsigned *popClone,
							  __global float * fitness,
  							  __global float * fitnessNorm,
                              __global int * D_seeds,
                              __global t_stats *est,
                              __constant struct KernelParameters* parameters){

    int tid = get_global_id(0),
		lid = get_local_id(0),
        gid = get_group_id(0),
        localSize = get_local_size(0);                
        
    int lid_0 = lid / 4;
    int lid_1 = lid % 4;
    
    uint seed = D_seeds[tid];
        
    __local float taxaMutacao[8];
    __local unsigned clone[8*4], bestClone[8*4];
    __local float bestCloneFitness[8];
    __local float cloneFitness[8];
        
    int j,k;
    unsigned word;    
        
    //Cada work-group é responsável pela clonagem de 8 indivíduos	
	if(lid_1 == 0){	
        taxaMutacao[lid_0] = exp(-parameters->MUTATION_FACTOR * fitnessNorm[gid+lid_0]);
        bestCloneFitness[lid_0] = fitness[gid+lid_0];
    }
   
    barrier(CLK_LOCAL_MEM_FENCE);
    
    for(uint i = 0; i < parameters->NCLON; i++) {
    
        //Clona e muta anticorpo pop[gid]
        for(j = lid; j < parameters->REAL_LEN; j+=localSize){
              
            word = pop[(gid+lid_0)*parameters->REAL_LEN  + j];
            
            for(uint l = 0; l < BITS_PER_WORD; l++){
            
                //Mutação            
                if(u_rand(&seed) <= taxaMutacao[lid_0]){
					BIT_FLIP(word, l);
                }
            }
                                                                        
            clone[lid_0*parameters->REAL_LEN+lid_1] = word;                                
        }
        
        barrier(CLK_LOCAL_MEM_FENCE);
        
        
        cloneFitness[lid_0] = 0;
  
        
        barrier(CLK_LOCAL_MEM_FENCE);
        
        //If current clone is better, then replace bestClone        
        if(cloneFitness[lid_0] > bestCloneFitness[lid_0]){
            barrier(CLK_LOCAL_MEM_FENCE);
            
            bestCloneFitness[lid_0] = cloneFitness[lid_0];

            for(k = lid_1; k < parameters->REAL_LEN; k+=4)
            {   
                bestClone[lid_0*parameters->REAL_LEN + k] = clone[lid_0*parameters->REAL_LEN + k];
            }
        } 
        barrier(CLK_LOCAL_MEM_FENCE);
    }
       
    //Replacement
    if(bestCloneFitness[lid_0] > fitness[gid+lid_0]){

        //barrier(CLK_LOCAL_MEM_FENCE);
        if(lid_1 == 0)   
		    fitness[gid+lid_0] = bestCloneFitness[lid_0];
	
        for(k = lid_1; k < parameters->REAL_LEN; k+=localSize)
        {   
            pop[(gid+lid_0)*parameters->REAL_LEN + k] = bestClone[lid_0*parameters->REAL_LEN + k];
        }       
 
    }
    
	D_seeds[tid] = seed;          
}*/


/*
__kernel void iteracaoClonalg(__global unsigned *pop, 
							  __global unsigned *popClone, 
  							  __global float * fitnessNorm,
                              __global int * D_seeds,
                              __global t_stats *est,
                              __constant struct KernelParameters* parameters){

    int tid = get_global_id(0),
		lid = get_local_id(0),
        gid = get_group_id(0),
        localSize = get_local_size(0);                
        
    if(lid < parameters->REAL_LEN){
    
		uint seed = D_seeds[tid];
		    
		float taxaMutacao = exp(-parameters->MUTATION_FACTOR * fitnessNorm[gid]);
		    
		int j,k;
		unsigned word;  
		
		int offset = gid*parameters->NCLON*parameters->REAL_LEN;    
	   
		//barrier(CLK_LOCAL_MEM_FENCE);
		
		for(uint i = 0; i < parameters->NCLON; i++) {
		
		    //Clona e muta anticorpo pop[gid]
		    for(j = lid; j < parameters->REAL_LEN; j+=localSize){
		          
		        word = pop[gid*parameters->REAL_LEN +j];
		        
		        for(uint l = 0; l < BITS_PER_WORD; l++){
		        
		            //Mutação            
		            if(u_rand(&seed) <= taxaMutacao){
						BIT_FLIP(word, l);
		            }
		        }
		                                            
		        popClone[offset + i*parameters->REAL_LEN + j] = word;                                
		    }
		}        
		    
		D_seeds[tid] = seed;          
	}
}*/

__kernel void iteracaoClonalg(__global unsigned *pop, 
							  __global float * fitness,
  							  __global float * fitnessNorm,
                              __global int * D_seeds,
                              __global t_stats *est,
                              __constant struct KernelParameters* parameters){

    int tid = get_global_id(0),
		lid = get_local_id(0),
        gid = get_group_id(0),
        localSize = get_local_size(0);      
        
 //   if(lid < parameters->REAL_LEN)

		uint seed = D_seeds[tid];
    
	    __local unsigned original[256];
    	__local unsigned clone[256];
    	__local unsigned bestClone[256];
    	__local float cloneFitness, bestCloneFitness;
		    
		float taxaMutacao = exp(-parameters->MUTATION_FACTOR * fitnessNorm[gid]);
		    
		int j,k;
		unsigned word;
		
		for(uint i = lid; i < parameters->REAL_LEN; i+=localSize){
		 	original[i] = pop[gid*parameters->REAL_LEN + i];		 
		 	bestClone[i] = original[i];//pop[gid*parameters->REAL_LEN + i]; 
		}
				 
		bestCloneFitness = fitness[gid];	 		
		
		//int offset = gid*parameters->NCLON*parameters->REAL_LEN;	   
		barrier(CLK_LOCAL_MEM_FENCE);
		
		for(uint i = 0; i < parameters->NCLON; i++) {

			cloneFitness = 0;
		
			for(uint k = lid; k < parameters->REAL_LEN; k+=localSize){
			 	clone[k] = original[k]; //pop[gid*parameters->REAL_LEN + k];
			}	
			
		    //Clona e muta anticorpo gid
		    for(j = lid; j < parameters->REAL_LEN; j+=localSize){
		          
		        word = clone[j];
		        
		        for(uint l = 0; l < BITS_PER_WORD; l++){
		        
		            //Mutação            
		            if(u_rand(&seed) <= taxaMutacao){
						BIT_FLIP(word, l);
		            }
		        }		                                            
		        clone[j] = word;                                
		    }		    
		    barrier(CLK_LOCAL_MEM_FENCE);
		    
		    //------------------------------------------------------------------
		    //clone evaluation		    
		    
		    __local int partial[32];
			partial[lid] = 0;
			int idx, m;
			unsigned aux, shift;
			int value;			
			
			/*for(idx=lid; idx < parameters->REAL_LEN; idx+=localSize) {
			 	aux = clone[idx];
				shift = MASK;
				
				for(m=0;m<BITS_PER_WORD && (idx*BITS_PER_WORD+m<parameters->CHROM_LEN) ; m+=1) {
		
				   value = (aux & shift) == 0 ? 0 : 1;
				   shift = shift >> 1;
				   partial[lid] += value;		   
				}	
			}*/
			
			if(localSize <= parameters->REAL_LEN){
	
				for(int idx=lid; idx < parameters->REAL_LEN; idx+=localSize) {
				 	aux = clone[idx];
					shift = MASK;
				
					for(int j=0;j<BITS_PER_WORD; j+=1) {
		
					   value = (aux & shift) == 0 ? 0 : 1;
					   shift = shift >> 1;
					   partial[lid] = partial[lid] + value;		   
					}	
				}
			}
			else{
				
				for(int idx=0; idx < parameters->REAL_LEN; idx+=1) {
				 	
				 	aux = clone[idx];
					//shift = MASK >> lid;
				
					for(int j=lid; j < BITS_PER_WORD; j+=localSize) {
		
					   //value = (aux & MASK ) == 0 ? 0 : 1;
					   value = (aux & (MASK >> j )) == 0 ? 0 : 1;
					   partial[lid] += value;		   			   
					   ///shift = shift >> localSize;
					}	
				}
			}
	
			barrier(CLK_LOCAL_MEM_FENCE); 

			idx = get_local_size(0)/2;
			while (idx!=0) {
			   	if (lid < idx)
			  		partial[lid] += partial[lid + idx]; 
			   	barrier(CLK_LOCAL_MEM_FENCE);
				idx = idx/2;
			}
			
			barrier(CLK_LOCAL_MEM_FENCE); 
			cloneFitness = -partial[0];
		    
		    //------------------------------------------------------------------		    
		    
		    barrier(CLK_LOCAL_MEM_FENCE);
		    if(cloneFitness > bestCloneFitness){
		    	for(uint k = lid; k < parameters->REAL_LEN; k+=localSize){
			 		bestClone[k] = clone[k];		 
				}		    	
				
				bestCloneFitness = cloneFitness;
		    }
		    barrier(CLK_LOCAL_MEM_FENCE);
		    
		}    
		
		//--------------------------------------------------------------------
		//Replacement
		
		barrier(CLK_LOCAL_MEM_FENCE);
	    if(bestCloneFitness > fitness[gid]){
	    	for(uint k = lid; k < parameters->REAL_LEN; k+=localSize){
		 		pop[gid*parameters->REAL_LEN + k] = bestClone[k];		 
			}		    	
			if(lid==0)
				fitness[gid] = bestCloneFitness;
	    }		    
		    
		D_seeds[tid] = seed;      
	
}

__kernel void substituicaoAleatoria(__global anticorpo *pop, 
                                    __global int * D_seeds){
                              
    int tid = get_global_id(0),
        lid = get_local_id(0),
        gid = get_group_id(0),
        localSize = get_local_size(0);
        
    int seed = D_seeds[tid];
    
    __local float afinidades[128];
    __local int posicao[128];
            
    afinidades[lid] = pop[lid].afinidade;
    posicao[lid] = lid;
       
    //Encontra o anticorpo com a menor afinidade
    //Itera por blocos de tamanho localSize (uma comparação por work-item)
    for (int i=localSize+lid;i<TAMANHO_POPULACAO;i+=localSize)
    {
        if(pop[i].afinidade < afinidades[lid]){
            afinidades[lid] = pop[i].afinidade;
            posicao[lid] = i;
        }
    }     
     
    //Redução 
    int next_power_of_2 = (int)native_powr( (float)2.0, (int) ceil( log2( (float) localSize ) ) );

    for(int s = next_power_of_2/2; s>0; s=s>>1){
        barrier(CLK_LOCAL_MEM_FENCE);
        if(lid < s && lid+s < localSize){	 
    	    //int value = posicao[lid+s];
    	    if(afinidades[lid+s] < afinidades[lid]){
       	        afinidades[lid] = afinidades[lid+s];
                posicao[lid] = posicao[lid+s];        	    
    	    }
        }
    }
    barrier(CLK_LOCAL_MEM_FENCE);
    
    __local anticorpo novo;
    
    for( uint i = lid; i < TAMANHO_ANTICORPO; i+=localSize )
    {   
        novo.representacao[i] = rand(&seed) % 2;    
    }
    
    D_seeds[tid] = seed;

    barrier(CLK_LOCAL_MEM_FENCE);
    avaliaAnticorpo_l(&novo, 0, lid, localSize);    
    
    if(lid == 0){
        pop[gid].afinidade = novo.afinidade;    
    }
   
    barrier(CLK_LOCAL_MEM_FENCE);    
    
    for( uint i = lid; i < TAMANHO_ANTICORPO; i+=localSize )
    {   
        pop[gid].representacao[i] = novo.representacao[i];    
    }
}
