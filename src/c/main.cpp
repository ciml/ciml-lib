#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <../No.h>
#include <../include/Individuo.h>
#include <cstdlib>
#include <stdlib.h>
#include <time.h>
#include <vector>
using namespace std;

#define MAX_IT 100000
#define tipos 1

void leArquivo(int *num_individuos,int *num_colunas,int *num_linhas,int *retorno,int *num_entradas,int *num_saidas,int *tipo){
    ifstream arquivo;
    std::string line;
    int i = 0;
    arquivo.open("entrada.txt");
    if(arquivo.is_open()){
        while(getline(arquivo,line)){
            switch(i){
                case 0: *num_individuos = stoi(line);
                 break;
                case 1: *num_colunas = stoi(line);
                 break;
                case 2: *num_linhas = stoi(line); break;
                case 3: *retorno = stoi(line); break;
                case 4: *num_entradas = stoi(line); break;
                case 5: *num_saidas = stoi(line); break;
                case 6: *tipo = stoi(line); break;
                default: break;
            }
            i++;
        }
    } else {
        cout << "Arquivo não foi aberto!";
        exit(1);
    }
    arquivo.close();
}

Individuo ** init_pop(int num_individuos, int num_colunas, int num_linhas, int num_entradas, int num_saidas, int retorno){
    Individuo **lista = new Individuo*[num_individuos];
    int random, ind_coluna, ind_linha, entradas_no, j;
    No *aux;
    //srand (time(NULL));
    for(int i = 0; i<num_individuos; i++){
        lista[i] = new Individuo(num_linhas, num_colunas, num_entradas, num_saidas);
        for(j = 0; j<num_colunas; j++){
            for(int k = 0; k<num_linhas; k++){
                do{
                random = rand() % tipos; //tipos
                try{
                    aux = new No(random, k, j);

                    lista[i]->matrizNo[k][j] = aux;
                    for(int l = 0; l<aux->num_entradas; l++){
                        ind_coluna = rand()%(retorno+1);
                        while(j - ind_coluna < 0)
                            ind_coluna = rand()%(retorno+1);

                        if(ind_coluna == 0){ //camada entrada
                            ind_linha = rand()%num_entradas;
                            aux->entradas.push_back(*lista[i]->entradas[ind_linha]);
                        } else {
                            ind_linha = rand()%num_linhas;
                            aux->entradas.push_back(*lista[i]->matrizNo[ind_linha][j-ind_coluna]);
                        }

                    }

                    break;
                } catch(const char* msg){
                    //cout << random << " tipo invalido.";
                }
                }while(true);

            }
        }
        for(j=0;j<num_saidas;j++){
            ind_coluna = rand()%retorno +1;
            ind_linha = rand()%num_linhas;
            lista[i]->saidas[j] = lista[i]->matrizNo[ind_linha][num_colunas - ind_coluna];
        }
    }

    return lista;
}


int main()
{
    int num_individuos, num_filhos, num_colunas, num_linhas, retorno, num_entradas, num_saidas, tipo;
    leArquivo(&num_filhos, &num_colunas, &num_linhas, &retorno, &num_entradas, &num_saidas, &tipo);
    num_individuos = 1;
    //cria tabelas.
    int linhas = pow(2,num_entradas);
    bool **tabela_target, **tabela_entrada;


    tabela_target = new bool *[linhas];
    tabela_entrada = new bool *[linhas];
    int i, j, k, aux, aux2;
    for(i = 0; i<linhas; i++){
        tabela_entrada[i] = new bool[num_entradas];
        tabela_target[i] = new bool[num_saidas];


        aux = i;
        j = num_entradas-1;
        k = 0;
        while(j>=0){
            if(aux >= pow(2,j)){
                aux = aux - pow(2,j);
                tabela_entrada[i][k] = 1;

            } else {
                tabela_entrada[i][k] = 0;
            }

            j--;
            k++;
        }

        switch(tipo){
            case 0: //multiplicação
                if(num_entradas%2 != 0){
                    cout << "numero de entradas deve ser par.";
                    exit(1);
                }
                k=0;
                aux = 0;
                for(j=num_entradas/2 -1;j>=0;j--){
                    //if(tabela_entrada[i][j]){
                        aux = aux + tabela_entrada[i][j]*pow(2,k);
                   // }
                    k++;
                }
                k=0;
                aux2 = 0;
                for(j=num_entradas-1;j>=num_entradas/2;j--){
                    //if(tabela_entrada[i][j]){
                        aux2 = aux2 + tabela_entrada[i][j]*pow(2,k);
                    //}
                    k++;
                }

                aux = aux*aux2;
                k = num_saidas-1;
                j = 0;
                while(k>=0){
                    if(aux >= pow(2,k)){
                        aux = aux - pow(2,k);
                        tabela_target[i][j] = 1;
                    } else {
                        tabela_target[i][j] = 0;
                    }
                    k--;
                    j++;
                }

                break;
                default: break;
        }

    }
    //Fim de criar tabelas...
    //Inicializar população
    Individuo **lista_ind = init_pop(num_individuos, num_colunas, num_linhas, num_entradas, num_saidas, retorno);
    lista_ind[0]->imprime();


    lista_ind[0]->avalia(tabela_entrada, tabela_target);
    cout << lista_ind[0]->pontuacao << endl;
    //populacao inicializada.

    //Inicio ag
    Individuo *aux_ind[num_filhos];
    Individuo *melhor_filho;
    int melhor_pontuacao = 0;
    for(int it=1;it<=MAX_IT;it++){
        melhor_pontuacao = 0;
        for(int filho = 0; filho < num_filhos; filho++){
            aux_ind[filho] = new Individuo(lista_ind[0]); //copia
            aux_ind[filho]->mutation();
            aux_ind[filho]->avalia(tabela_entrada, tabela_target);
            if(aux_ind[filho]->pontuacao > melhor_pontuacao){
                melhor_filho = aux_ind[filho];
                melhor_pontuacao = aux_ind[filho]->pontuacao;
            }
            //cout << "Filho " << filho << ": " << aux_ind[filho]->pontuacao <<", ";
        }
        //cout << endl;
        //cout << "Filho: " << melhor_filho->pontuacao << ". Pai: " <<lista_ind[0]->pontuacao << endl;
        if(melhor_filho->pontuacao >= lista_ind[0]->pontuacao){
            lista_ind[0]->~Individuo(); //destroi
            lista_ind[0] = melhor_filho;
            for(int filho = 0; filho < num_filhos; filho++){
                if(aux_ind[filho] != melhor_filho){
                    aux_ind[filho]->~Individuo();
                }
            }
        } else {
            for(int filho = 0; filho < num_filhos; filho++){
                    aux_ind[filho]->~Individuo();
            }
        }
    }
    cout << " Pai: " <<lista_ind[0]->pontuacao << endl;
    lista_ind[0]->~Individuo();
    return 0;
}
