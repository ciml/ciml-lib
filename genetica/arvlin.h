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
    void iniArvore(Arvlin *arv,int N);
    //aloca arvore, precisa ser modificada para memoria estatica
    void arvPrintVetor(Arvlin *arv);
    //imprime na forma linear
    void printNivel(Arvlin *arv,int nivel,int i);
    //formato do output do print arv
    Arvlin *arvTest();
    //gera arvore modelo
    void printArvore(Arvlin *arv);
    //imprime arvore na forma nivel+++elem
    void semeadora(Arvlin *arv,int nivelMax);
    //gera arvore aleatoria com nivel max
    int geraArv(Arvlin *arv,int indice);
    //gera os nos da semeadora
    void mutaArv(Arvlin *arv,Arvlin *arvM);
    //insere nova subarvore gerada
    void skipElemArv(Arvlin *arv,int tamPulo,int onde);
    //desloca elementos da arvore
    int calcTamSubArv(int *filhosArv,int indice);
    //calcula tamanho de uma subarvore usando o vetor de filhos

#ifdef __cplusplus
}
#endif

#endif /* ARVLIN_H */
