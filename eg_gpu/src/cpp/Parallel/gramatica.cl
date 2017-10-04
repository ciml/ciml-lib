
int GetRegra(__constant t_regra *gramatica, type_simbolo simbolo){

	int i;

	for(i=0;i<TAMANHO_GRAMATICA;i++){
	    if(gramatica[i].simbolo.v[1] == simbolo.v[1]) return i;
	}

	return -1;  
}

int Decodifica(__constant t_regra *Gramatica, __global short fenotipo[], __local t_item_programa * programa){

	int m, fenotipo_ctr = 0;

	/* Inicializa o programa com o símbolo inicial */

	/*type_simbolo inicial;
	
	inicial.v[0] = NAOTERMINAL;
	inicial.v[1] = 0*/
	
	programa[0].t.v[0]  = NAOTERMINAL;
	programa[0].t.v[1]  = 0;
	programa[0].proximo = FIM_PROGRAMA;

	int program_ctr=1;
	
	//Mapeamento
	while(1){

		/* Verifica se todo o fenótipo foi utilizado */
		if(fenotipo_ctr == DIMENSOES_PROBLEMA-1){		
		    program_ctr = -1;
		    break;
		}		

		/* Procura primeiro não terminal à esquerda */
		int i=0;

		while((int)programa[i].t.v[0] != NAOTERMINAL && programa[i].proximo != -1) {
 
		    i = programa[i].proximo;
		}
        
		/* Verifica se há somente terminais */
		if((int)programa[i].t.v[0] != NAOTERMINAL) break;
		
		int idRegra = GetRegra(Gramatica, programa[i].t);

		int opcao = fenotipo[fenotipo_ctr++] % Gramatica[idRegra].num_escolhas;

		int proximoAnterior = programa[i].proximo;

		/* Substitui pelo primeiro simbolo */

		/*type_simbolo auxs = Gramatica[idRegra].escolhas[opcao].simbolos[0];
		programa[i].t.v[0] = auxs.v[0];
		programa[i].t.v[1] = auxs.v[1];*/
		
		programa[i].t.v[0] = Gramatica[idRegra].escolhas[opcao].simbolos[0].v[0];
		programa[i].t.v[1] = Gramatica[idRegra].escolhas[opcao].simbolos[0].v[1];
		
		if(Gramatica[idRegra].escolhas[opcao].num_simbolos>1){

		    programa[i].proximo = program_ctr;

		    for(m=1; m < Gramatica[idRegra].escolhas[opcao].num_simbolos; m++){

			    programa[program_ctr].t = Gramatica[idRegra].escolhas[opcao].simbolos[m];
			    programa[program_ctr].proximo = program_ctr+1;

			    program_ctr++;
		    }
			
		    programa[program_ctr-1].proximo = proximoAnterior;
		}		
	}
	
	return program_ctr;
}
