
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
        No **entradas;
        bool isLigante;

        bool getSaida(int j, bool **tabela_entrada);

};



