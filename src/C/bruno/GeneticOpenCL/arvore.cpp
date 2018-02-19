#include "arvore.h"

///Não está sendo utilizado no programa principal
void inicializaArvore(Arvore* arv){
    arv->numNos = 0;
}

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

void geradorArvore(Arvore* arv, int maxTam, int* conjuntoOpTerm, int NUM_OPBIN, int NUM_OPUN, int N, int* seed,
                    float maxDados, float minDados){
    Pilha pilha;
    pilha.topo = -1;

    int indice = 0;
    int ultimo = 0;
    int soma = 1;

    int aux[MAX_NOS];
    int sorteio, j, num, tipo;

    for(j = 0; j < MAX_NOS; j++){
        aux[j] = 0;
    }

    arv->numNos = 0;
    empilha(&pilha, indice);

    while(pilha.topo != -1){
        indice = desempilha(&pilha);

        if(aux[indice] == 0){
            sorteio = conjuntoOpTerm[randomType(NUM_OPBIN, NUM_OPUN, N, seed)];
            tipo = unpackTipo(sorteio);

            if(tipo == VAR || tipo == CTE){
                num = 0;
            } else if (tipo == FUN){
                num = 1;
            } else if (tipo == FBIN){
                num = 2;
            }

            arv->numeroFilhos[indice] = num;
            arv->numNos++;
            soma += num;

            if(soma > maxTam){
                soma-=arv->numeroFilhos[indice];
                arv->numeroFilhos[indice] = 0;
                sorteio = conjuntoOpTerm[randomLeafType(NUM_OPBIN, NUM_OPUN, N,seed)];
                num = 0;
                tipo = unpackTipo(sorteio);
            }

            if(tipo == CTE){
                sorteio = packFloat(CTE, randomConst(seed, maxDados, minDados));
            }
            arv->informacao[indice] = sorteio;
        }

        if(aux[indice] < arv->numeroFilhos[indice]){
            empilha(&pilha, indice);
            aux[indice]++;
            ultimo++;
            empilha(&pilha, ultimo);
        }
    }
}


void criaCheia(Arvore* arv, int maxDepth, int* conjuntoOpTerm, int NUM_OPBIN, int NUM_OPUN, int N, int* seed,
               float maxDados, float minDados){

    //conferir de alguma forma que cabe uma árvore cheia no vetor (no caso confere para uma arvore binaria)
    //nao funciona no caso se sortear '1' filho várias vezes, pois acaba cabendo uma arvore mais profunda e ainda 'cheia'
    //if(pow(2, maxDepth+1)-1 > MAX_NOS)
    //printf("Espaco insuficiente para arvore cheia nesta profundidade.");
    //return;

    Pilha pilha;
    pilha.topo = -1;

    int indice = 0;
    int ultimo = 0;
    int j, num, tipo, sorteio;
    //se tirar 'soma' tem que garantir que a profundidade maxima garanta uma arvore nao-degenerada
    //int soma = 1;

    int aux[MAX_NOS];

    for(j = 0; j < MAX_NOS; j++){
        aux[j] = 0;
    }
    arv->numNos = 0;
    empilha(&pilha, indice);

    while(pilha.topo != -1){
        indice = desempilha(&pilha);

        if(aux[indice] == 0){
            if(pilha.topo+1 != maxDepth){
                sorteio = conjuntoOpTerm[randomNodeType(NUM_OPBIN, NUM_OPUN, N,seed)];
                tipo = unpackTipo(sorteio);
                if (tipo == FUN){
                    num = 1;
                } else if (tipo == FBIN){
                    num = 2;
                }

            } else {
                sorteio = conjuntoOpTerm[randomLeafType(NUM_OPBIN, NUM_OPUN, N,seed)];
                tipo = unpackTipo(sorteio);
                if(tipo == CTE){
                    sorteio = packFloat(CTE, randomConst(seed, maxDados, minDados));
                }
                num = 0;
            }
            arv->numeroFilhos[indice] = num;
            arv->numNos++;
            arv->informacao[indice] = sorteio;
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
            if(tipo == CTE)
                printf("%f | %d\n", unpackFloat(arv->informacao[indice]), arv->numeroFilhos[indice]);
            else
                printf("%d | %d\n", unpackInt(arv->informacao[indice]), arv->numeroFilhos[indice]);
        }

        if(aux[indice] < arv->numeroFilhos[indice]){
            empilha(&pilha, indice);
            aux[indice]++;
            ultimo++;
            empilha(&pilha, ultimo);
        }
    }
}

void imprimeArvorePre(Arvore* arv, char** LABELS){
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
            printf("(");
            imprimeSinxate(arv, indice, LABELS);
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

void imprimeArvorePos(Arvore* arv, char** LABELS){
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
            imprimeSinxate(arv, indice, LABELS);
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

void mutacao(Arvore* arvore, int* conjuntoOpTerm, int NUM_OPBIN, int NUM_OPUN, int N, int* seed,
             float maxDados, float minDados){
    int i;
    //sorteia uma subarvore da arvore inicial
        //pega o tamanho dessa subarvore
    int indiceSub = rand2(seed) % (arvore->numNos);
    int tamanhoSub = calculaTamanhoSubArvore(arvore, indiceSub);

    //confere o tamanho max para criar uma nova arvore
        //tamanho disponivel+tamanho da subarvore sorteada
    int espacoLivre = MAX_NOS-(arvore->numNos)+tamanhoSub;

    //cria uma nova arvore aleatoria de até o tamanho maximo calculado
    //TODO: criar uma nova arvore ou nao?
    Arvore novaArvore;
    //inicializaArvore(&novaArvore);
    geradorArvore(&novaArvore, espacoLivre, conjuntoOpTerm, NUM_OPBIN, NUM_OPUN, N,seed, maxDados, minDados);

    //determina o tamanho do deslocamento da arvore

    //TODO: colocar if para raiz?
    //if(indiceSub != 0 && indiceSub != arvore->numNos-1){
    int tamShift = novaArvore.numNos - tamanhoSub;
    shift(arvore, tamShift, indiceSub+tamanhoSub);

    //troca a informação
    for(i = 0; i < novaArvore.numNos; i++){
        arvore->numeroFilhos[i+indiceSub] = novaArvore.numeroFilhos[i];
        arvore->informacao[i+indiceSub] = novaArvore.informacao[i];
    }
}

void crossOver(Arvore* arvore1, Arvore* arvore2, int* seed, int id){

    int espacoLivre1, espacoLivre2, indiceSub1, indiceSub2, tamanhoSub1, tamanhoSub2;
    int cont = 0;
    do{
        indiceSub1 = rand2(seed) % (arvore1->numNos);
        tamanhoSub1 = calculaTamanhoSubArvore(arvore1, indiceSub1);

        indiceSub2 = rand2(seed) % (arvore2->numNos);
        tamanhoSub2 = calculaTamanhoSubArvore(arvore2, indiceSub2);

        espacoLivre1 = MAX_NOS-(arvore1->numNos)+tamanhoSub1;
        espacoLivre2 = MAX_NOS-(arvore2->numNos)+tamanhoSub2;

        if(cont++ == 5) return;
    } while(espacoLivre1-tamanhoSub2 < 0 || espacoLivre2-tamanhoSub1 < 0);

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

void imprimeSinxate(Arvore* arv, int j, char* LABELS[]){
    int tipo = retornaTipo(arv, j);
    int info = arv->informacao[j];
    //unpackTipo(info);
    switch(tipo){
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
            printf("sin");
            break;
        case COS:
            printf("cos");
            break;
        case SQR:
            printf("sqrt");
            break;
        case EXP:
            printf("exp");
            break;
        case CTE:
            {


            float valorF = unpackFloat(info);
            printf("%.20f", valorF);
            break;
            }
        case VAR:
            {
            int valor2 = unpackInt(info);
            printf("%s", LABELS[valor2]);
            break;
            }
        default:
            break;

    }
}

int retornaTipo(Arvore* arv, int j){
    if(arv->numeroFilhos[j] == 0){
        return unpackTipo(arv->informacao[j]);
    } else {
        return unpackInt(arv->informacao[j]);
    }
}


float executa(Arvore* arv, float dados[], int N){

    PilhaEx pilhaEx;
    pilhaEx.topo = -1;

    int j;
    int tipo;
    float num, div;

    for(j = arv->numNos -1; j>=0; j--){
        tipo = retornaTipo(arv, j);
        //printf("tipo = %d\n", tipo);
        switch(tipo){
            case PLUS:
                empilhaF(&pilhaEx,desempilhaF(&pilhaEx) + desempilhaF(&pilhaEx));
                break;
            case MIN:
                empilhaF(&pilhaEx,desempilhaF(&pilhaEx) - desempilhaF(&pilhaEx));
                break;
            case MULT:
                empilhaF(&pilhaEx,desempilhaF(&pilhaEx) * desempilhaF(&pilhaEx));
                break;
            case DIV:
                num = desempilhaF(&pilhaEx);
                div = desempilhaF(&pilhaEx);
                empilhaF(&pilhaEx,proDiv(num,div));
                break;
            case SIN:
                empilhaF(&pilhaEx,sin(desempilhaF(&pilhaEx)));
                break;
            case COS:
                empilhaF(&pilhaEx,cos(desempilhaF(&pilhaEx)));
                break;
            case SQR:
               empilhaF(&pilhaEx,proSqrt(desempilhaF(&pilhaEx)));
                break;
            case EXP:
                empilhaF(&pilhaEx,exp(desempilhaF(&pilhaEx)));
                break;
            case CTE:
                {


                float valorF = unpackFloat(arv->informacao[j]);
                empilhaF(&pilhaEx, valorF);
                break;
                }
            case VAR:
                {


                int valor2 = unpackInt(arv->informacao[j]);
                empilhaF(&pilhaEx, dados[valor2]);
                break;
                }
        }
    }

    float erro = desempilhaF(&pilhaEx)- dados[N-1];
    erro = ( isinf( erro ) || isnan( erro ) ) ? /*FLT_MAX*/ INFINITY : erro;
    return erro*erro;//pow(erro, 2);
}

float opBinaria(int operador, float valor1, float valor2){
    switch(operador){
        case PLUS:
            return (valor1 + valor2);
        case MIN:
            return (valor1 - valor2);
        case MULT:
            return (valor1 * valor2);
        case DIV:
            return proDiv(valor1, valor2);
    }
    return -1;
}

float opUnaria(int operador, float valor){
    switch(operador){
        case SIN:
            return (float)sin(valor);
        case COS:
            return (float)cos(valor);
        case SQR:
            return proSqrt(valor);
        default:
            return -1;
    }
    return -1;
}

void leIndividuo(char *fileName, Arvore* individuo, char** LABELS, float** dados, int M, int N) {
    //printf("%s", fileName);
    FILE* arquivo = fopen(fileName, "r");
    char cp[1000000];
    int posicao = 0;
    int informacao;

//    fseek(arquivo, 0, SEEK_END);
//    long fsize = ftell(arquivo);
//    fseek(arquivo, 0, SEEK_SET);

    if (arquivo == NULL) {
        fprintf(stderr, "Error opening data file.\n");
        return;
    }
    const char delimiters[] = " ()";

    while (fgets(cp, 1000000, arquivo)) {

        char *token;

        token = strtok (cp, delimiters);      /* token => "words" */
          //  printf("%s\n", token);

        while(token != NULL){
            //printf("%s\n", token);
            if(strcmp(token, "+") == 0){
                //printf("aaa\n");
                informacao = packInt(FBIN, PLUS);
                individuo->informacao[posicao] = informacao;
                individuo->numeroFilhos[posicao] = 2;
            }
            else if(strcmp(token, "-") == 0){
                informacao = packInt(FBIN, MIN);
                individuo->informacao[posicao] = informacao;
                individuo->numeroFilhos[posicao] = 2;
            }
            else if(strcmp(token, "*") == 0){
                informacao = packInt(FBIN, MULT);
                individuo->informacao[posicao] = informacao;
                individuo->numeroFilhos[posicao] = 2;
            }
            else if(strcmp(token, "/") == 0){
                informacao = packInt(FBIN, DIV);
                individuo->informacao[posicao] = informacao;
                individuo->numeroFilhos[posicao] = 2;
            }
            else if(strcmp(token, "sin") == 0){
                informacao = packInt(FUN, SIN);
                individuo->informacao[posicao] = informacao;
                individuo->numeroFilhos[posicao] = 1;
            }
            else if(strcmp(token, "cos") == 0){
                informacao = packInt(FUN, COS);
                individuo->informacao[posicao] = informacao;
                individuo->numeroFilhos[posicao] = 1;
            }
            else if(strcmp(token, "sqrt") == 0){
                informacao = packInt(FUN, SQR);
                individuo->informacao[posicao] = informacao;
                individuo->numeroFilhos[posicao] = 1;
            }
            else if(strcmp(token, "exp") == 0){
                informacao = packInt(FUN, EXP);
                individuo->informacao[posicao] = informacao;
                individuo->numeroFilhos[posicao] = 1;
            }
            else if(token[0] == 'X'){
                token++;
                //informacao = packInt(VAR, atoll(token)-1);
                individuo->informacao[posicao] = informacao;
                individuo->numeroFilhos[posicao] = 0;
            } else {
                informacao = packFloat(CTE, atof(token));
                individuo->informacao[posicao] = informacao;
                individuo->numeroFilhos[posicao] = 0;
            }
            posicao++;
            token = strtok (NULL, delimiters);
            //printf("%d\n", posicao);
        }
        individuo->numNos = posicao;
//        int i;
//        for(i= 0; i < posicao-1; i++){
//            printf("%d ", individuo->informacao[posicao]);
//        }
        imprimeArvorePre(individuo, LABELS);
        printf("\nnos = %d\n", individuo->numNos);
        //executa(individuo, dados[0], N);
        int j;
        float erro = 0;
        for(j = 0; j < M; j++){
            //printf("%d - %.20f \n",j, erro);
            erro = erro + executa(individuo, dados[j], N);
            //printf("%f - %f\n", erro, dados[k+M*(N-1)]);
            //printf("erro = %.20f\n", erro);
        }
        printf("erro = %.20f\n", erro);

//        token = strtok (NULL, delimiters);    /* token => "separated" */
//        printf("%s", token);
//        token = strtok (NULL, delimiters);    /* token => "by" */
//        printf("%s", token);
//        token = strtok (NULL, delimiters);    /* token => "spaces" */
//        printf("%s", token);
//        token = strtok (NULL, delimiters);    /* token => "and" */
//        printf("%s", token);
//        token = strtok (NULL, delimiters);    /* token => "punctuation" */
//        printf("%s", token);
//        token = strtok (NULL, delimiters);    /* token => NULL */
//        printf("%s", token);
    }

}
