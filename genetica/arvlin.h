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
    void arvPrintVetor(Arvlin *arv);
    void printNivel(Arvlin *arv,int nivel,int i);
    Arvlin *arvTest();
    void printArvore(Arvlin *arv);
    void semeadora(Arvlin *arv,int nivelMax);
    int geraArv(Arvlin *arv,int indice);    

#ifdef __cplusplus
}
#endif

#endif /* ARVLIN_H */
