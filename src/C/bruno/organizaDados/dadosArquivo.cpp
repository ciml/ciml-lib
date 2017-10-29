#include "dadosArquivo.h"

extern "C" void cabecalho(){
    ofstream arq;
    arq.open ("tabela.txt",ios::app);
    arq << setw(10) << "Prob. Mutacao"  <<" | "
         << setw(15) << "Media"         <<" | "
         << setw(15) << "Mediana"       <<" | "
         << setw(15) << "Melhor"        << " | "
         << setw(15) << "Modelo"        <<"\n";
    arq.close();
}



extern "C" void montaTabelas(Arvore melhores[]){
    fstream arq;
    arq.open("tabela.txt",ios_base::app);

    cout << "aaaaaaaaaa";
    ordena(melhores, NUM_TESTES);
    cout << "aaaaaaaaaa";
    float media = 0;
    float melhor = INT_MAX;
    int indiceMelhor;

    for(int i = 0; i < NUM_TESTES; i++){
        media += melhores[i].aptidao;
        if(melhor > melhores[i].aptidao){
            melhor = melhores[i].aptidao;
            indiceMelhor = i;
        }
    }
    media = media/NUM_TESTES;

    arq << setw(10) << PROB_MUT<< " | "
        << setw(15) << media << " | "
        << setw(15) << melhores[NUM_TESTES/2].aptidao << " | "
        << setw(15) << melhor<< " | ";

         salvaArvorePre(&melhores[indiceMelhor], arq);
         arq << "\n";

    arq.close();
}

void ordena(Arvore pop[], int k){
    Arvore aux;
    int i, j;
    int indiceMelhor;

    for(i = 0; i < k; i++){
        indiceMelhor = i;
        for(j = i+1; j < k; j++){

            if(pop[j].aptidao < pop[indiceMelhor].aptidao){
                indiceMelhor = j;
            }
        }

        aux = pop[i];
        pop[i] = pop[indiceMelhor];
        pop[indiceMelhor] = aux;
    }
}

void salvaArvorePre(Arvore* arv, fstream& arq){
    //ofstream arq;
    //arq.open ("tabela.txt", ios_base::app);
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
            arq << "(" ;

            salvaSinxate(arv->informacao[indice], arq); //arv->informacao[indice], arv->numeroFilhos[indice]);
        }

        if(aux[indice] < arv->numeroFilhos[indice]){
            empilha(&pilha, indice);
            aux[indice]++;
            ultimo++;
            empilha(&pilha, ultimo);
        } else {
            arq << ")";
        }
    }
}

void salvaSinxate(int info, fstream& arq){
    //ofstream arq;
    //arq.open ("tabela.txt", ios_base::app);
    int valor1, valor2;
    float valorF;
    int tipo = unpackTipo(info);
    switch(tipo){
    case FUN:;//This is an empty statement.
    case FBIN:;//This is an empty statement.
        valor1 = unpackInt(info);
        switch(valor1){
        case PLUS:
            arq << "+";
            break;
        case MIN:
            arq << "-";
            break;
        case MULT:
            arq << "*";
            break;
        case DIV:
            arq << "/";
            break;
        case SIN:
            arq << "sin ";
            break;
        case COS:
            arq << "cos ";
            break;
        case SQR:
            arq << "sqrt ";
            break;
        }
        break;
    case CONST:;//This is an empty statement.
        valorF = unpackFloat(info);
        arq << valorF;
        break;
    case VAR:;
        valor2 = unpackInt(info);
        arq << LABELS[valor2];
        break;
    default:
        break;

    }
}
