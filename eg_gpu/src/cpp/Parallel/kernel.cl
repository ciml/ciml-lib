#define DATABASE(x,y) dataBase[x*NUM_VARIAVEIS + y]

//#define DATABASE(row,column) dataBase[(column)*TAMANHO_DATABASE + row]

#include "representacao.h"
#include "substituicao.cl"
#include "utils.cl"
#include "parser.cl"
#include "gramatica.cl"
#include "avaliacao.cl"

__kernel void initializeEvaluatedNodes(__global long int *evaluatedNodes)
{
	int tid = get_global_id(0);
	evaluatedNodes[tid] = 0;
}

 __kernel void sumGPULongInt( __global long int *input,
                              __local  long int *localSums)
 {
 
  uint local_id = get_local_id(0);
  uint group_size = get_local_size(0);
  uint global_id = get_global_id(0);
  
  long int workItemResult = 0;
  
  for(uint index = local_id; index < TAMANHO_POPULACAO; index+=group_size)
  {  	
  	if(global_id < TAMANHO_POPULACAO)
		workItemResult += input[index];
  }  

  //Copy partial result
  localSums[local_id] = workItemResult;

  // Loop for computing localSums : divide WorkGroup into 2 parts
  for (uint stride = group_size/2; stride>0; stride /=2)
  {
      // Waiting for each 2x2 addition into given workgroup
      barrier(CLK_LOCAL_MEM_FENCE);

      // Add elements 2 by 2 between local_id and local_id + stride
      if (local_id < stride)
        localSums[local_id] += localSums[local_id + stride];
   }

  // Write result into partialSums[nWorkGroups]
  if (local_id == 0)
    input[0] = localSums[0];
}     


__kernel void inicializa_populacao(__global individuo *pop, 
                                   __global int *D_seeds){  
    
    int tid = get_global_id(0),
        lid = get_local_id(0),
        gid = get_group_id(0);

    int seed = D_seeds[tid];

    /*
		granularidade mais fina e acesso coalescido à memória global.
    */
    pop[gid].genotipo[lid] = rand2(&seed) % 256;
    
    D_seeds[tid] = seed;
}

__kernel void selecao(__global individuo *pop, 
			__global individuo *newPop,
			__global int *D_seeds){												
			
    int tid  = get_global_id(0), 
        lid  = get_local_id(0),
        gid  = get_group_id(0);

    int seed = D_seeds[tid];        
    /*
    	Seleção
    */
                   
    __local int indicePais[1];    
    __local int melhores1[TAMANHO_TORNEIO];
    __local float aptidoes1[TAMANHO_TORNEIO];
    
	melhores1[0] = gid;
    aptidoes1[0] = pop[gid].aptidao;    
    
    if(lid > 0 && lid < TAMANHO_TORNEIO){
    
        int aleatorio = rand2(&seed) % TAMANHO_POPULACAO;
	    
	    melhores1[lid] = aleatorio;
        aptidoes1[lid] = pop[aleatorio].aptidao;
    }

    barrier(CLK_LOCAL_MEM_FENCE);  
    
    if(lid == 0){      
      
        int idmelhor1=0, idmelhor2=0;
        
        for(int i=1; i < TAMANHO_TORNEIO; i++){
        
            if(aptidoes1[i] > aptidoes1[idmelhor1]){
                idmelhor1 = i;
            }
        }  
        
        indicePais[0] = melhores1[idmelhor1];
    }
    
    barrier(CLK_LOCAL_MEM_FENCE);

    newPop[gid].genotipo[lid] = pop[indicePais[0]].genotipo[lid];
    //newPop[gid].genotipo[2*lid+1] = pop[indicePais[1]].genotipo[2*lid+1];        
       
    D_seeds[tid] = seed;
}

__kernel void crossOverEMutacao(__global individuo *pop, 
		//	const int geracao, 
			__global individuo *newPop,
			__global int * D_seeds)
{
     int tid  = get_global_id(0)*2, 
        lid   = get_local_id(0),
        gid   = get_group_id(0);
        
    int seed  = D_seeds[tid];
   
    /* Cada grupo de trabalho realiza o crossover entre os elementos pop[2*gid]
     e pop[2*gid+1];
     Cada item de trabalho é responsável por um bit do cromossomo;
     A decisão de realizar o xover e o ponto de corte são tomadas pelo item 0. 
    */
    __local int crossOver;
    __local int ponto;
    
    if(lid == 0){
        //Gera um número entre 0 e 1
        float aleatorio = u_rand2(&seed);
        if(aleatorio <= (float)TAXA_DE_RECOMBINACAO){
            crossOver = 1;            
            ponto = rand2(&seed) % TAMANHO_INDIVIDUO;
        }        
        else{
            crossOver = 0;			
        }
    }    
    
    barrier(CLK_LOCAL_MEM_FENCE);    
     
    int bitFilho1, bitFilho2;    
    
    if(crossOver){
    
        if(lid > ponto){           
            bitFilho1 = newPop[2*gid+1].genotipo[lid];
            bitFilho2 = newPop[2*gid].genotipo[lid];
        }
        else{
            bitFilho1 = newPop[2*gid].genotipo[lid];
            bitFilho2 = newPop[2*gid+1].genotipo[lid];
        }
    } 
    else{
        bitFilho1 = newPop[2*gid].genotipo[lid];
        bitFilho2 = newPop[2*gid+1].genotipo[lid];
    }
        
    /* MUTAÇÃO */
    
    if(u_rand2(&seed) <= (float)TAXA_DE_MUTACAO){
        //bitFilho1 = 1 - bitFilho1;
        bitFilho1 = rand2(&seed) % 256;
    }
    
    if(u_rand2(&seed) <= (float)TAXA_DE_MUTACAO){
        bitFilho2 = rand2(&seed) % 256;     
    }
    
    newPop[2*gid].genotipo[lid]   = bitFilho1;
    newPop[2*gid+1].genotipo[lid] = bitFilho2;
    
    D_seeds[tid] = seed;
}








