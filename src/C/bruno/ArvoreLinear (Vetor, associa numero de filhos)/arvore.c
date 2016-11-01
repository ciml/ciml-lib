#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "arvore.h"


void inicializaArvore(Arvore* arv){
    arv->numNos = 0;
}

//TODO: receber só o vetor de filhos...
int calculaTamanhoSubArvore(Arvore* arv, int indice){
    int soma = arv->numeroFilhos[indice];
    int tam = 1;

    while(soma != tam-1){
        indice++;
        soma += arv->numeroFilhos[indice];
        tam++;
    }
    return tam;
}

//TODO: mudar o gerador para receber uma profundidade máxima?
void geradorArvore(Arvore* arv, int maxTam){
    Pilha pilha;
    pilha.topo = -1;

    int indice = 0;
    int ultimo = 0;
    int soma = 1;

    int aux[maxTam];
    int j, num;

    //usar 'num' o inves de j pra nao ter variavel sobrando...
    for(j = 0; j < maxTam; j++){
        aux[j] = 0;
    }

    arv->numNos = 0;
    empilha(&pilha, indice);

    while(pilha.topo != -1){
        indice = desempilha(&pilha);

        if(aux[indice] == 0){
            num = rand()%(MAX_FILHOS+1);
            arv->numeroFilhos[indice] = num;
            arv->numNos++;
            soma += num;

        if(soma > maxTam){
            soma-=arv->numeroFilhos[indice];
            arv->numeroFilhos[indice] = 0;
            num = 0;
        }
        //escolhendo um valor para a informação do nó
        //printf("num = %d\n", num);
            if(num == 0){
                //arv->informacao[indice] = randomTerminal();
                int valor = randomTerminal();
                //printf("valor = %d\n", valor);
                if(valor >= (N-1))
                    arv->informacao[indice] = packFloat(CONST, randomConst());
                else
                    arv->informacao[indice] = packInt(VAR, valor);

            } else if (num == 1){
                arv->informacao[indice] = packInt(FUN, randomFunctionUn());

            } else{
                arv->informacao[indice] = packInt(FBIN, randomFunctionBin());
            }
        }

        if(aux[indice] < arv->numeroFilhos[indice]){
            empilha(&pilha, indice);
            aux[indice]++;
            ultimo++;
            empilha(&pilha, ultimo);
        }
    }
}

void criaCheia(Arvore* arv, int maxDepth){

    //conferir de alguma forma que cabe uma árvore cheia no vetor (no caso confere para uma arvore binaria)
    //nao funciona no caso se sortear '1' filho várias vezes, pois acaba cabendo uma arvore mais profunda e ainda 'cheia'
    //if(pow(2, maxDepth+1)-1 > MAX_NOS)
    //printf("Espaco insuficiente para arvore cheia nesta profundidade.");
    //return;
    //

    Pilha pilha;
    pilha.topo = -1;

    int indice = 0;
    int ultimo = 0;
    //se tirar 'soma' tem que garantir que a profundidade maxima garanta uma arvore nao-degenerada
    //int soma = 1;

    int aux[MAX_NOS];
    int j, num;

    //usar 'num' o inves de j pra nao ter variavel sobrando...
    for(j = 0; j < MAX_NOS; j++){
        aux[j] = 0;
    }
    arv->numNos = 0;
    empilha(&pilha, indice);

    while(pilha.topo != -1){
        indice = desempilha(&pilha);

        if(aux[indice] == 0){
            if(pilha.topo+1 != maxDepth){
                num = rand()%MAX_FILHOS + 1;
                //arv->numeroFilhos[indice] = num;
                //soma += num;
            } else {
                num = 0;
                //arv->numeroFilhos[indice] = 0;
            }
            arv->numeroFilhos[indice] = num;
            arv->numNos++;

//            if(num == 0)
//                arv->informacao[indice] = randomTerminal();
//            else if (num == 1)
//                arv->informacao[indice] = randomFunctionUn();
//            else
//                arv->informacao[indice] = randomFunctionBin();

            if(num == 0){
                int valor = randomTerminal();
                if(valor >= N-1)
                    arv->informacao[indice] = packFloat(CONST, randomConst());
                else
                    arv->informacao[indice] = packInt(VAR, valor);

            } else if (num == 1){
                arv->informacao[indice] = packInt(FUN, randomFunctionUn());

            } else{
                arv->informacao[indice] = packInt(FBIN, randomFunctionBin());
            }
        }

        /*
        if(soma > MAX_NOS){
            soma-=arv->numeroFilhos[indice];
            arv->numeroFilhos[indice] = 0;
        }
        */

        if(aux[indice] < arv->numeroFilhos[indice]){
            empilha(&pilha, indice);
            aux[indice]++;
            ultimo++;
            empilha(&pilha, ultimo);
        }
    }
}

//pos ordem -avaliar / conversor
void imprimeArvoreNivel(Arvore* arv){
    Pilha pilha;
    pilha.topo = -1;

    int indice = 0;
    int ultimo = 0;
    int aux[MAX_NOS];
    int j;

    for(j = 0; j < MAX_NOS; j++){
        aux[j] = 0;
    }

    if(arv->numNos != 0)
        empilha(&pilha, indice);

    while(pilha.topo != -1){

        indice = desempilha(&pilha);

        if(aux[indice] == 0){
            printf("(%d)", pilha.topo+1);
            for(j = 0; j <= pilha.topo+1; j++)
                printf("-");

            int tipo = unpackTipo(arv->informacao[indice]);
            if(tipo == CONST)
                printf("%f | %d\n", unpackFloat(arv->informacao[indice]), arv->numeroFilhos[indice]);
            else
                printf("%d | %d\n", unpackInt(arv->informacao[indice]), arv->numeroFilhos[indice]);
                //printf("%d | %d\n", arv->informacao[indice], arv->numeroFilhos[indice]);
        }

        if(aux[indice] < arv->numeroFilhos[indice]){
            empilha(&pilha, indice);
            aux[indice]++;
            ultimo++;
            empilha(&pilha, ultimo);
        }
    }
}

void imprimeArvorePre(Arvore* arv){
    Pilha pilha;
    pilha.topo = -1;

    int indice = 0;
    int ultimo = 0;
    int aux[MAX_NOS];
    int j;

    for(j = 0; j < MAX_NOS; j++){
        aux[j] = 0;
    }

    if(arv->numNos != 0){
        empilha(&pilha, indice);
    }

    while(pilha.topo != -1){
        indice = desempilha(&pilha);
        if(aux[indice] == 0){
            printf("("/*%d", arv->informacao[indice]*/);

            imprimeSinxate(arv->informacao[indice]); //arv->informacao[indice], arv->numeroFilhos[indice]);
        }

        if(aux[indice] < arv->numeroFilhos[indice]){
            empilha(&pilha, indice);
            aux[indice]++;
            ultimo++;
            empilha(&pilha, ultimo);
        } else {
            printf(")");
        }
    }
}

void imprimeArvorePos(Arvore* arv){
    Pilha pilha;
    pilha.topo = -1;

    int indice = 0;
    int ultimo = 0;
    int aux[MAX_NOS];
    int j;

    for(j = 0; j < MAX_NOS; j++){
        aux[j] = 0;
    }

    if(arv->numNos != 0){
        empilha(&pilha, indice);
        printf("(");
    }

    while(pilha.topo != -1){
        indice = desempilha(&pilha);

        if(aux[indice] == arv->numeroFilhos[indice]){
            imprimeSinxate(arv->informacao[indice]);
            printf(")");
        }

        if(aux[indice] < arv->numeroFilhos[indice]){
            printf("(");
            empilha(&pilha, indice);
            aux[indice]++;
            ultimo++;
            empilha(&pilha, ultimo);
        }

    }
}

void shift(Arvore* arv, int tam, int indice){//indice a partir de onde começa o shift, tam = tamanho do shift
    int i;
    if(tam > 0){
        for(i = arv->numNos-1; i >= indice; i--){
            arv->numeroFilhos[i+tam] = arv->numeroFilhos[i];
            arv->informacao[i+tam] = arv->informacao[i];
        }
    } else {
        for(i = indice; i < arv->numNos; i++){
            arv->numeroFilhos[i+tam] = arv->numeroFilhos[i];
            arv->informacao[i+tam] = arv->informacao[i];
        }
    }
    arv->numNos += tam;
}

void mutacao(Arvore* arvore){
    int i;
    //sorteia uma subarvore da arvore inicial
        //pega o tamanho dessa subarvore
    int indiceSub = rand() % (arvore->numNos);
    int tamanhoSub = calculaTamanhoSubArvore(arvore, indiceSub);

    //confere o tamanho max para criar uma nova arvore
        //tamanho disponivel+tamanho da subarvore sorteada
    int espacoLivre = MAX_NOS-(arvore->numNos)+tamanhoSub;

    //cria uma nova arvore aleatoria de até o tamanho maximo calculado
    //TODO: criar uma nova arvore ou nao?
    Arvore novaArvore;
    //inicializaArvore(&novaArvore);
    geradorArvore(&novaArvore, espacoLivre);


    //determina o tamanho do deslocamento da arvore

//TODO: colocar if para raiz?
//if(indiceSub != 0 && indiceSub != arvore->numNos-1){
        int tamShift = novaArvore.numNos - tamanhoSub;
        shift(arvore, tamShift, indiceSub+tamanhoSub);

//

    //troca a informação
    for(i = 0; i < novaArvore.numNos; i++){
        arvore->numeroFilhos[i+indiceSub] = novaArvore.numeroFilhos[i];
        arvore->informacao[i+indiceSub] = novaArvore.informacao[i];
    }
}

void crossOver(Arvore* arvore1, Arvore* arvore2){
    //int i;
    int espacoLivre1, espacoLivre2, indiceSub1, indiceSub2, tamanhoSub1, tamanhoSub2;
    //printf("\nCHEGOU\n");
    do{
        indiceSub1 = rand() % (arvore1->numNos);
        tamanhoSub1 = calculaTamanhoSubArvore(arvore1, indiceSub1);
        //printf("ind1= %d\n", indiceSub1);
        //printf("tam1= %d\n", tamanhoSub1);
        indiceSub2 = rand() % (arvore2->numNos);
        tamanhoSub2 = calculaTamanhoSubArvore(arvore2, indiceSub2);
        //printf("ind2= %d\n", indiceSub2);
        //printf("tam1= %d\n", tamanhoSub2);
        espacoLivre1 = MAX_NOS-(arvore1->numNos)+tamanhoSub1;
        espacoLivre2 = MAX_NOS-(arvore2->numNos)+tamanhoSub2;

    }while(espacoLivre1-tamanhoSub2 < 0 || espacoLivre2-tamanhoSub1 < 0);

//    Arvore arvAux;
//    for(i = 0; i < tamanhoSub1; i++){
//        arvAux.informacao[i] = arvore1->informacao[indiceSub1+i];
//        arvAux.numeroFilhos[i] = arvore1->numeroFilhos[indiceSub1+i];
//    }

    int tamShift1 = tamanhoSub2 - tamanhoSub1;
    //shift(arvore1, tamShift1, indiceSub1+tamanhoSub1);

    int tamShift2 = tamanhoSub1 - tamanhoSub2;


    if(tamanhoSub1 >= tamanhoSub2){
        shift(arvore2, tamShift2, indiceSub2+tamanhoSub2);
        trocaSubArv(arvore1, arvore2, indiceSub1, indiceSub2, tamanhoSub1, tamanhoSub2);
        shift(arvore1, tamShift1, indiceSub1+tamanhoSub1);
    } else {
        shift(arvore1, tamShift1, indiceSub1+tamanhoSub1);
        trocaSubArv(arvore2, arvore1, indiceSub2, indiceSub1, tamanhoSub2, tamanhoSub1);
        shift(arvore2, tamShift2, indiceSub2+tamanhoSub2);
    }

//    for(i = 0; i < tamanhoSub2; i++){
//        arvore1->numeroFilhos[i+indiceSub1] = arvore2->numeroFilhos[i+indiceSub2];
//        arvore1->informacao[i+indiceSub1] = arvore2->informacao[i+indiceSub2];
//    }
//
//    for(i = 0; i < tamanhoSub1; i++){
//        arvore2->numeroFilhos[i+indiceSub2] = arvAux.numeroFilhos[i];
//        arvore2->informacao[i+indiceSub2] = arvAux.informacao[i];
//    }
}

void  trocaSubArv(Arvore* arvMaior,Arvore* arvMenor,int ind1,int ind2,int tamanhoSubMenor, int tamanhoSubMaior){
    int i=0, aux;
    while(i<tamanhoSubMenor){
        aux = arvMaior->informacao[ind1+i];
        arvMaior->informacao[ind1+i] = arvMenor->informacao[ind2+i];
        arvMenor->informacao[ind2+i] = aux;
        aux = arvMaior->numeroFilhos[ind1+i];
        arvMaior->numeroFilhos[ind1+i] = arvMenor->numeroFilhos[ind2+i];
        arvMenor->numeroFilhos[ind2+i] = aux;
        i++;
    }
    while(i<tamanhoSubMaior){
        arvMenor->informacao[ind2+i] = arvMaior->informacao[ind1+i];
        arvMenor->numeroFilhos[ind2+i] = arvMenor->numeroFilhos[ind2+i];
    }
}

void imprimeSinxate(int info){ //int id, int tipo){
    int tipo = unpackTipo(info);
    switch(tipo){
    case FUN:;//This is an empty statement.
    case FBIN:;//This is an empty statement.
        int valor1 = unpackInt(info);
        switch(valor1){
        case PLUS:
            printf("+");
            break;
        case MIN:
            printf("-");
            break;
        case MULT:
            printf("*");
            break;
        case DIV:
            printf("/");
            break;
        case SIN:
            printf("sin ");
            break;
        case COS:
            printf("cos ");
            break;
        case SQR:
            printf("sqrt ");
            break;
        }
        break;
    case CONST:;//This is an empty statement.
        float valorF = unpackFloat(info);
        printf("%.2f", valorF);
        break;
    case VAR:;
        int valor2 = unpackInt(info);
        printf("%s", LABELS[valor2]);
        break;
    default:
        break;

    }
//    switch(numFilhos){
//        case 0:
//            printf("%c", LABELS[id]);
//            break;
//        default:
//            switch(id){
//                case 0:
//                    printf("+");
//                    break;
//                case 1:
//                    printf("-");
//                    break;
//                case 2:
//                    printf("*");
//                    break;
//                case 3:
//                    printf("/");
//                    break;
//                case 4:
//                    printf("sin ");
//                    break;
//                case 5:
//                    printf("cos ");
//                    break;
//                case 6:
//                    printf("sqrt ");
//                    break;
//            }
//        break;
//    }
}

//como acessa tipo
//como acessar informaçao
//se preci fazer ida e volta
//como jogar os bits em int pra real?

//tipagem
//impressao
//contas
//leitura de arquivos
//alterar as faixas de valores

//float executa(Arvore* arv, float dados[]){
//    //float dados[3] = {10, 20, 30};
//    Pilha pilha, pilhaEx;
//    //PilhaEx pilhaEx;
//
//    pilha.topo = -1;
//    //pilhaEx.topo = -1;
//
//    int indice = 0;
//    int ultimo = 0;
//    int aux[MAX_NOS];
//    int j;
//    //float auxVet[MAX_NOS];
//    int k = 0;
//
//    float  resultado;
//    int aux1;
//    int resultadoInt, tipo, valor;
//
//    for(j = 0; j < MAX_NOS; j++){
//        aux[j] = 0;
//    }
//
//    empilha(&pilha, indice);
//    //printf("(");
//
//    while(pilha.topo != -1){
//        indice = desempilha(&pilha);
//
//        if(aux[indice] == arv->numeroFilhos[indice]){
//            if(arv->numeroFilhos[indice] == 0){
//                tipo = unpackTipo(arv->informacao[indice]);
//                if(tipo == CONST){
//                    empilha(&pilhaEx, arv->informacao[indice]);
//                } else {
//                    valor = unpackInt(arv->informacao[indice]);
//                    resultadoInt = packFloat(CONST, dados[aux1]);
//                    empilha(&pilhaEx, resultadoInt);
//                }
//                //empilha2(&pilhaEx, dados[arv->informacao[indice]]); //TODO pensar em constantes
//            }else{
//                //empilha2(&pilhaEx, arv->informacao[indice]);
//            //printf("inf ind %d = %d\n",indice,  arv->informacao[indice]);
//            //printf("topo = %d\n",pilhaEx.topo);
//            //printf("aaaa");
//
//                if(arv->numeroFilhos[indice] == 1){
//                    aux1 = desempilha(&pilhaEx);
//
//                    //aux1 = desempilha2(&pilhaEx);
//
//                    resultado = opUnaria(arv->informacao[indice], aux1);
//                    resultadoInt = packFloat(CONST, resultado);
//
//                    empilha(&pilhaEx, resultadoInt);
//                    //empilha2(&pilhaEx, resultado);
//                } else if (arv->numeroFilhos[indice] == 2){
//                    aux1 = desempilha(&pilhaEx);
//                    aux2 = desempilha(&pilhaEx);
//                    //aux1 = desempilha2(&pilhaEx);
//                    //aux2 = desempilha2(&pilhaEx);
//                    resultado = opBinaria(arv->informacao[indice], aux2, aux1);
//
//                    resultadoInt = packFloat(CONST, resultado);
//                    empilha(&pilhaEx, resultadoInt);
//                    //empilha2(&pilhaEx, resultado);
//                }
//
//                    //Consultar a pilha (o operador)
//                    //desempilhar a quantidade de operandos de acordo com o operador
//
//
//    //                while(k <= arv->numeroFilhos[indice]){
//    //                    auxVet[k] = desempilha2(&pilhaEx);
//    //                    printf("%.2f \n", auxVet[k]);
//    //                    k++;
//    //                }
//    //                    float resultado = fazConta(k-1, auxVet);
//    //                    printf("resultado = %.2f\n", resultado);
//    //                    empilha2(&pilhaEx, resultado);
//    //                    k = 0;
//                }
//            }
//
//
//        if(aux[indice] < arv->numeroFilhos[indice]){
//            //printf("(");
//            empilha(&pilha, indice);
//            aux[indice]++;
//            ultimo++;
//            empilha(&pilha, ultimo);
//        }
//    }
//
//    //printf("topo: %d\n", pilhaEx.topo);
//    float erro = desempilha2(&pilhaEx) - dados[N-1];
//    //printf("erro: %f\n", erro*erro);
//    return erro*erro;
//}



//arquivo entrada
//3 2 1
//x0 Y
//1 1
//2 4
//3 9
//x0*x0
float executa(Arvore* arv, float dados[]){
    //float dados[3] = {10, 20, 30};
    Pilha pilha;
    PilhaEx pilhaEx;

    pilha.topo = -1;
    pilhaEx.topo = -1;

    int indice = 0;
    int ultimo = 0;
    int aux[MAX_NOS];
    int j;

    //float auxVet[MAX_NOS];
    int k = 0;
    float aux1, aux2, resultado;
    int tipo;

    for(j = 0; j < MAX_NOS; j++){
        aux[j] = 0;
    }

    empilha(&pilha, indice);
    //printf("(");

    while(pilha.topo != -1){
        indice = desempilha(&pilha);

        if(aux[indice] == arv->numeroFilhos[indice]){
            if(arv->numeroFilhos[indice] == 0){
                tipo = unpackTipo(arv->informacao[indice]);
                if(tipo ==  CONST){
                    empilha2(&pilhaEx, unpackFloat(arv->informacao[indice]));
                } else {
                    empilha2(&pilhaEx, dados[unpackInt(arv->informacao[indice])]);
                } //TODO pensar em constantes
            }else{
                //empilha2(&pilhaEx, arv->informacao[indice]);
            //printf("inf ind %d = %d\n",indice,  arv->informacao[indice]);
            //printf("topo = %d\n",pilhaEx.topo);
            //printf("aaaa");

                if(arv->numeroFilhos[indice] == 1){
                    aux1 = desempilha2(&pilhaEx);
                    resultado = opUnaria(unpackInt(arv->informacao[indice]), aux1);
                    empilha2(&pilhaEx, resultado);
                } else if (arv->numeroFilhos[indice] == 2){
                    aux1 = desempilha2(&pilhaEx);
                    aux2 = desempilha2(&pilhaEx);
                    resultado = opBinaria(unpackInt(arv->informacao[indice]), aux2, aux1);
                    empilha2(&pilhaEx, resultado);
                }

                    //Consultar a pilha (o operador)
                    //desempilhar a quantidade de operandos de acordo com o operador


    //                while(k <= arv->numeroFilhos[indice]){
    //                    auxVet[k] = desempilha2(&pilhaEx);
    //                    printf("%.2f \n", auxVet[k]);
    //                    k++;
    //                }
    //                    float resultado = fazConta(k-1, auxVet);
    //                    printf("resultado = %.2f\n", resultado);
    //                    empilha2(&pilhaEx, resultado);
    //                    k = 0;
                }
            }


        if(aux[indice] < arv->numeroFilhos[indice]){
            //printf("(");
            empilha(&pilha, indice);
            aux[indice]++;
            ultimo++;
            empilha(&pilha, ultimo);
        }
    }

    //printf("topo: %d\n", pilhaEx.topo);
    float erro = desempilha2(&pilhaEx)- dados[N-1];
    //printf("\nN = %d\n\n", N);//dados[N-1]
//int i,j;

//        for(j = 0; j < N; j++){
//            printf("%.2f ", dados[j]);
//        }
//        printf("\n");

    //printf("----------------------------------dados: %f\n", dados[N-1]);
    //printf("----------------------------------erro: %f\n", erro);
    return erro*erro;
}


//float fazConta(int k, float vet[]){
//    switch(k){
//    case 1:
//        return opUnaria(vet[0], vet[1]);
//        break;
//    case 2:
//        return opBinaria(vet[0], vet[2], vet[1]);
//        break;
//    default:
//        return 0;
//    }
//}

float opBinaria(int operador, float valor1, float valor2){
    switch(operador){
        case 0:
            return (valor1 + valor2);
        case 1:
            return (valor1 - valor2);
        case 2:
            return (valor1 * valor2);
        case 3:
            if(valor2 != 0)
                return (valor1 / valor2);
            else
                return 1;
    }
    return -1;
}

float opUnaria(int operador, float valor){
    switch(operador){
        case 4:
            return (float)sin(valor);
        case 5:
            return (float)cos(valor);
        case 6:
            if(valor < 0) //valor = -1 * valor;
                return 1;//(float)sqrt(valor);
        default:
            return 0;
    }
    return -1;
}

void testaExecuta(float* dados[]){
    Arvore arvore1;
    criaArvTeste(&arvore1);
    imprimeArvorePre(&arvore1);

//    int i, j;
//    for(i = 0; i < M; i++){
//        for(j = 0; j < N; j++){
//            printf("%.2f ", dados[i][j]);
//        }
//        printf("\n");
//    }

    printf("%f", executa(&arvore1, dados[0]));
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
    printf("POR NIVEL: \n");
    imprimeArvoreNivel(arvore);
    printf("\nPOS ORDEM: \n");
    imprimeArvorePos(arvore);
    printf("\nPRE ORDEM: \n");
    imprimeArvorePre(arvore);
}

void criaArvTeste(Arvore *arvore1){
    arvore1->numeroFilhos[0] = 2;
    arvore1->numeroFilhos[1] = 2;
    arvore1->numeroFilhos[2] = 0;
    arvore1->numeroFilhos[3] = 2;
    arvore1->numeroFilhos[4] = 0;
    arvore1->numeroFilhos[5] = 0;
    arvore1->numeroFilhos[6] = 0;

    arvore1->informacao[0] = packInt(FBIN, 0);
    arvore1->informacao[1] = packInt(FBIN, 2);
    arvore1->informacao[2] = packFloat(CONST, 10.00);
    arvore1->informacao[3] = packInt(FBIN, 1);
    arvore1->informacao[4] = packInt(VAR, 0);
    arvore1->informacao[5] = packFloat(CONST, 5.0);;
    arvore1->informacao[6] = packFloat(VAR, 0);

    arvore1->numNos = 7;
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
