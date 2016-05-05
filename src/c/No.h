
class No
{
    public:
        No(int tipo, int j, int k);
        virtual ~No();
        int num_entradas;
        int custo;
        int tipo, linha, coluna;
        No **entradas;
        bool isLigante;

        bool getSaida(int j, bool **tabela_entrada);

};

class Entrada : public No {
    public:
        Entrada(bool entrada);
        bool entrada;
        bool getSaida();
};

class And : public No {
    public:

        And(No **ind_entradas);
        virtual ~And();
        bool getSaida();
};

