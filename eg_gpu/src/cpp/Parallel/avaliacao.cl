
__kernel void avaliacao(__global individuo *pop, 
			__constant t_regra * Gramatica,
			
			#ifdef Y_DOES_NOT_FIT_IN_CONSTANT_BUFFER
	 		__global const
			#else
			__constant 
			#endif 
 			float * dataBase,
 			__global long int *evaluatedNodes){
	
	int tid = get_global_id(0),
   	    lid = get_local_id(0),
   	    gid = get_group_id(0),
   	    LOCAL_SIZE = get_local_size(0);
   	    
    /*__local short genotipo[DIMENSOES_PROBLEMA]; 	 
	//obtem_fenotipo_individuo3(pop[gid], fenotipo, lid, get_local_size(0));
 	
 	for(int i=lid; i < TAMANHO_INDIVIDUO; i+= LOCAL_SIZE){ 	
 	    genotipo[lid] = pop[gid].genotipo[lid];	
 	}*/   	    
        
	if(lid==0){
		//__local short fenotipo[DIMENSOES_PROBLEMA];
		        
		__local t_item_programa programa[TAMANHO_MAX_PROGRAMA]; 	

		int program_ctr = Decodifica(Gramatica, pop[gid].genotipo, programa);	 	
	 	
	 	if(program_ctr == -1){
		    pop[gid].aptidao = MAXFLOAT*(-1);
	 	}
	 	else{
	 	
	 		evaluatedNodes[gid] += program_ctr * TAMANHO_DATABASE;
	 		 		 		 	
	 	    float erro = 0;
	 		 	
	 	    for(int j=0; j < TAMANHO_DATABASE; j++){
			
				erro += pown(Avalia(programa, dataBase, j), 2);
			
				if( isinf( erro ) || isnan( erro ) ) { erro = MAXFLOAT; break; }
		    }

	   	    if(erro == MAXFLOAT) pop[gid].aptidao = MAXFLOAT*(-1);
	   	       	    	
		    else{
		    	pop[gid].aptidao = -erro;
    	    }		    
		}
	}	
}

__kernel void avaliacao_gpu(__global individuo *pop, 
			__constant t_regra * Gramatica,
			#ifdef Y_DOES_NOT_FIT_IN_CONSTANT_BUFFER
	 		__global const
			#else
			__constant 
			#endif 
			float * dataBase,
			__local float * erros,
			__global long int *evaluatedNodes){
	
	int tid = get_global_id(0),
   	    lid = get_local_id(0),
   	    gid = get_group_id(0),
	    LOCAL_SIZE = get_local_size(0);
                
	__local t_item_programa programa[TAMANHO_MAX_PROGRAMA]; 	 	 
 	__local int program_ctr;
 	 
 	/*__local short genotipo[DIMENSOES_PROBLEMA]; 	 
	//obtem_fenotipo_individuo3(pop[gid], fenotipo, lid, get_local_size(0));
 	
 	for(int i=lid; i < TAMANHO_INDIVIDUO; i+= LOCAL_SIZE){ 	
 	    genotipo[lid] = pop[gid].genotipo[lid];	
 	}
 	*/
 	//barrier(CLK_LOCAL_MEM_FENCE);	

   	//1 workitem realiza o mapeamento
	if(lid==0){
		
		program_ctr = Decodifica(Gramatica, pop[gid].genotipo, programa);

	 	if(program_ctr == -1){
 		    pop[gid].aptidao = MAXFLOAT*(-1);
	 	}
	 	else{
	 		evaluatedNodes[gid] += program_ctr * TAMANHO_DATABASE;
	 	}
 	}
 	 	
 	//Sincronismo local para que todos os workitens acessem o programa mapeado 	
 	barrier(CLK_LOCAL_MEM_FENCE);	
 	
 	if(program_ctr != -1){
		
  		 erros[lid] = 0.0f;
		 //Avaliação paralela entre work-itens do mesmo work-group

		#ifndef NUM_POINTS_IS_NOT_DIVISIBLE_BY_LOCAL_SIZE
		   for(uint iter = 0; iter < TAMANHO_DATABASE/LOCAL_SIZE; iter++ )
		   {	
		   int line = iter * LOCAL_SIZE + lid;
		#else
   		   for( uint iter = 0; iter < ceil( TAMANHO_DATABASE / (float) LOCAL_SIZE ); iter++ )
   		   {
  		     int line = iter * LOCAL_SIZE + lid;
              if( line < TAMANHO_DATABASE)
              {
	    #endif		
	            erros[lid] += pown(Avalia(programa, dataBase, iter * LOCAL_SIZE + lid), 2);
		        //dataBase[line] - dataBase[line+1]; // 
		
	          #ifdef NUM_POINTS_IS_NOT_DIVISIBLE_BY_LOCAL_SIZE
		      }
            #endif
		  }

		  for(int s = LOCAL_SIZE_ROUNDED_UP_TO_POWER_OF_2/2; s>0 ; s= s/2){
		  	barrier(CLK_LOCAL_MEM_FENCE);
			
			#ifndef LOCAL_SIZE_IS_NOT_POWER_OF_2
			      if( lid < s )
			#else
			      if(lid < s && (lid + s < LOCAL_SIZE ) )
			#endif		        
			           erros[lid] += erros[lid+s];
		  }
			
		  if(lid==0){
                
              if( isinf( erros[0] ) || isnan( erros[0] ) ) 
                  erros[0] = MAXFLOAT;
		      pop[gid].aptidao = -erros[0];
		  } 		
	}	

}
