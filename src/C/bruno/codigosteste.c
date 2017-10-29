void testaExecuta(float* dados[]){
//    Arvore arvore1;
//    criaArvTeste(&arvore1);
//    imprimeArvorePre(&arvore1);

//    int i, j;
//    for(i = 0; i < M; i++){
//        for(j = 0; j < N; j++){
//            printf("%.2f ", dados[i][j]);
//        }
//        printf("\n");
//    }

//    printf("%f", executa(&arvore1, dados[1]));
}

void generate(Arvore* arv, int min, int max){
    int j;
    int tamanho = min + randomSuperior(max-min+1);
    arv->numNos = tamanho;
    //printf("p=%d\n", tamanho);
    int d[max];
    arv->numeroFilhos[0] = randomSuperior(tamanho-1)%3;
    //printf("numF=%d", arv->numeroFilhos[0]);
    d[0] = tamanho-1;

    for(j = 1; j < tamanho-1; j++){
        //printf("aaaaaaa\n");
        d[j] = d[j-1]-arv->numeroFilhos[j-1];
        if(d[j] == (tamanho-j-1)){
            //printf("bbbbbbb\n");
            arv->numeroFilhos[j] = 1 + randomSuperior(d[j])%2;

        } else {
            //printf("ccccccc\n");
            //printf("%d\n", d[j]);
            arv->numeroFilhos[j] = randomSuperior(d[j])%3;

        }
    }
    arv->numeroFilhos[tamanho-1] = 0;
    return;
}

void testaPrint(Arvore *arvore){
//    printf("POR NIVEL: \n");
//    imprimeArvoreNivel(arvore);
//    printf("\nPOS ORDEM: \n");
//    imprimeArvorePos(arvore);
//    printf("\nPRE ORDEM: \n");
//    imprimeArvorePre(arvore);
}

void criaArvTeste(Arvore *arvore1){
    arvore1->numeroFilhos[0] = 2;
    arvore1->numeroFilhos[1] = 0;
    arvore1->numeroFilhos[2] = 2;
    arvore1->numeroFilhos[3] = 2;
    arvore1->numeroFilhos[4] = 1;
    arvore1->numeroFilhos[5] = 1;
    arvore1->numeroFilhos[6] = 0;
    arvore1->numeroFilhos[7] = 0;
    arvore1->numeroFilhos[8] = 0;


    arvore1->informacao[0] = packInt(FBIN, 1);
    arvore1->informacao[1] = packInt(VAR, 0);
    arvore1->informacao[2] = packInt(FBIN, 2);
    arvore1->informacao[3] = packInt(FBIN, 1);
    arvore1->informacao[4] = packInt(FUN, 5);
    arvore1->informacao[5] = packInt(FUN, 6);
    arvore1->informacao[6] = packInt(VAR, 0);
    arvore1->informacao[7] = packInt(VAR, 0);
    arvore1->informacao[8] = packInt(VAR, 0);

    arvore1->numNos = 9;
}

void realizaTestes(){

    int i;
    Arvore arvore1, arvore2;


    inicializaArvore(&arvore1);
    inicializaArvore(&arvore2);
    for(i = 0; i < MAX_NOS ; i++){
        arvore1.numeroFilhos[i]=-99;
    }
    //teste(&arvore1);
    generate(&arvore1,5, 10);
    //geradorArvore(&arvore1, MAX_NOS);
//    criaCheia(&arvore2, 2);
//
//    imprimeArvoreNivel(&arvore1);
//    printf("\n\n");
//    imprimeArvoreNivel(&arvore2);
//    printf("\n\n");

    for(i = 0; i < MAX_NOS ; i++){
        printf("%d ", arvore1.numeroFilhos[i]);
    }

    printf("\n");



//    for(i = 0; i < arvore2.numNos ; i++){
//        printf("%d ", arvore2.numeroFilhos[i]);
//    }
//
//    printf("\n\n");
//
//    printf("MUTAÇÃO ARVORE 1: \n");
//    mutacao(&arvore1);
//    imprimeArvoreNivel(&arvore1);
//    printf("\n\n");
//
//    printf("CROSS OVER ARVORES 1 E 2: \n");
//    crossOver(&arvore1, &arvore2);
//
//    imprimeArvoreNivel(&arvore1);
//    printf("\n");
//    imprimeArvoreNivel(&arvore2);
//    printf("\n");
}