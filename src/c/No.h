#include <vector>
class Individuo;
class No
{
    public:
        No(int tipo, int j, int k);
        No(No* copia, Individuo *ind);
        virtual ~No();
        int num_entradas;
        int custo;
        int tipo, linha, coluna;
        bool saida_cache;
        int linha_cache;
        No* entradas[2];
        bool isLigante;


        void mudaTipo(int tipo);
        bool getSaida(int j, bool **tabela_entrada, int *qtd_ligantes);


};



