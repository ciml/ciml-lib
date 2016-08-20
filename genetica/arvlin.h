#ifndef ARVLIN_H
#define ARVLIN_H

#ifdef __cplusplus
extern "C" {
#endif

     typedef struct{
      int Nfilhos;
      int *elementos;
      int *filhos;
      int max;
    }Arvlin;
    //aloca arvore, precisa ser modificada para memoria estatica
    void iniArvore(Arvlin *arv,int N);
    //desaloca memoria dinamica dos vetores da arv
    void lenhador(Arvlin *arv);    
    //imprime na forma linear
    void arvPrintVetor(Arvlin *arv);    
    //formato do output do print arv
    void printNivel(Arvlin *arv,int nivel,int i);    
    //gera arvore modelo
    Arvlin *arvTest();    
    //imprime arvore na forma nivel+++elem
    void printArvore(Arvlin *arv);        
    //gera arvore aleatoria com nivel max
    void semeadora(Arvlin *arv,int nivelMax);    
    //gera os nos da semeadora
    int geraArv(Arvlin *arv,const int* indice);    
    //insere nova arvore gerada em alguns lugar aleatorio da arvore
    void mutaArv(Arvlin *arv);    
    //desloca elementos da arvore
    void skipElemArv(Arvlin *arv,const int* tamPulo,const int* onde,const int* tamSubArv);
    //calcula tamanho de uma subarvore usando o vetor de filhospp
    int calcTamSubArv(int *filhosArv,const int* indice);
    //calcula tamanho de uma subarvore usando o vetor de filhos
    void copiaSubArv(Arvlin* arv,Arvlin* arv2,int onde1, int onde2,const int* );
    //copia uma subarv na subarv da outra onde sao os indices das subarvores    
    void combinacao(Arvlin *arv1,Arvlin *arv2);
    //copia uma subarvore na outra    
    void trocaSubArv(Arvlin* arvM,Arvlin* arvm,int *icol1,int *icol2,int tamSubArvMenor);
    // troca os elementos de uma subarvore menor em outra subarvore 
   
#ifdef __cplusplus
}
#endif

#endif /* ARVLIN_H */
