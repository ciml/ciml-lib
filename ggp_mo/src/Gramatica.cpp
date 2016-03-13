#include "Gramatica.h"

Gramatica* gram;

void Gramatica::addRegra(Regra* r){
    Regras.push_back(r);
    totalRegra++;
}
void Gramatica::addMap(char* s){
    Mapa.push_back(new Hash());
    Mapa[totalMapa]->s = s;
    Mapa[totalMapa]->pos = totalMapa;
    totalMapa++;
}
int Gramatica::checkMap(char* s){
    int ind = 0;
    while(ind < totalMapa){
        if(strcmp(Mapa[ind]->getS(), s) == 0)
            return ind;
        ind++;
    }
    return -1;
}
void Gramatica::addMapV(char* s){
    for(string str : MapaVariaveis)
        if(strcmp(str.c_str(), s) == 0)
            return;

    MapaVariaveis.push_back(s);
    totalMapaVariaveis++;
}
int Gramatica::checkMapV(string s){
    int i = 0;
    while(i < totalMapaVariaveis){
        if(strcmp(MapaVariaveis[i].c_str(), s.c_str()) == 0){
            return i;
        }
        i++;
    }
    return -1;
}

Gramatica::Gramatica(string sarq){
    totalRegra = 0;
    totalMapa = 0;
    totalMapaVariaveis = 0;
    print = false;

//    cout << "gramatica: " << sarq.c_str() << endl;
    ifstream arq;
    arq.open(sarq.c_str());
    string line;
    char* str;
    char* temp;
    int var = 0;

    while(getline(arq, line)){
        str = new char[line.length()+1];
        int i = 0;
        while(line[i]){
            str[i] = line[i];
            i++;
        }
        str[i] = '\0';

        temp = strtok(str, " :");

        if(checkMap(temp) == -1){
            addMap(temp);
        }
        temp = strtok(NULL, " ");
        temp = strtok(NULL, "|");

        int j = 0;
        vector<char*> vet;
        while(temp != NULL){
            vet.push_back(temp);
            j++;
            temp = strtok(NULL, "|");
        }
        rulesStrings.push_back(vet);
    }
    arq.close();

    /***************** Agora Gerar as Regras de Acordo com o conteúdo salvo de rulesStrings ***************/

    Terminal* t = new Terminal();
    for(int i = 0; i < totalMapa; i++){
        Regra* r = new Regra();
//            cout << "Criando regras de " << Mapa[i]->getS() << endl;
        int j = 0;
        while(j < (int)rulesStrings.at(i).size()){
            Prod* p = new Prod();
//                cout << "       Producao: " << rulesStrings[i][j] << endl;
            char* produto = strtok(rulesStrings[i][j], " ");
            while(produto != NULL){
//                    cout << "           Produto: " << produto << endl;
                Node* n;
//                    cout << "           No: " << produto << endl;
                if(produto[0] == '<' && produto[1] != '=' && produto[1] != NULL){
//                        cout << "               E um nao terminal;" << endl;
                    int pos = checkMap(produto);
                    if(pos != -1){
                        n = new Node(0, pos, produto);
                    }
                }else{
                    n = t->folha(produto, var);
                    if(n->tipo == 3){
                        var++;
                        addMapV(produto);
                        n->val = checkMapV(produto);
                    }
                }
                produto = strtok(NULL, " ");
                p->add(n);
            }
            r->add(p);
            j++;
        }
        Regras.push_back(r);
        totalRegra++;
//            cout << "TotalRegra: " << totalRegra << endl;
//        addRegra(r);
    }
//        cout << "Carregado!" << endl;

    alturaGramatica();
//        cout << "Calculado Altura\n";
//        imprimeProfs();
//        imprimeGramatica();
    ordenaProducoes();
//        cout << "Ordenado\n";
//        imprimeGramatica();
};

void Gramatica::ordenaProducoes(){
    for(int i = 0; i < totalRegra; i++){
        for(int j = 0; j < Regras[i]->qnt; j++){
            for(int k = j + 1; k < Regras[i]->qnt; k++){
                if(Regras[i]->Producao[j]->menorProfundidadeProd > Regras[i]->Producao[k]->menorProfundidadeProd){
                    Prod* a = Regras[i]->Producao[j];
                    Regras[i]->Producao[j] = Regras[i]->Producao[k];
                    Regras[i]->Producao[k] = a;
                }
            }
        }
    }
};

void Gramatica::imprimeMapa(){
    for(int i = 0; i < totalMapa; i++)
        cout << Mapa[i]->getS() << endl;
    for(int i = 0; i < totalMapaVariaveis; i++)
        cout << MapaVariaveis[i] << endl;
};

void Gramatica::imprimeGramatica(){
    for(int i = 0; i < totalRegra; i++){
        cout << "Regra " << i << " menor prof -> " << Regras[i]->menorProfundidadeRegra << endl;
        for(int j = 0; j < Regras[i]->qnt; j++){
            cout << "   Producao " << j << " menor prof > " << Regras[i]->Producao[j]->menorProfundidadeProd << endl;
            for(int k = 0; k < Regras[i]->Producao[j]->qnt; k++){
                cout << "       Tipo: " << Regras[i]->Producao[j]->produto[k]->tipo << " Valor: " << Regras[i]->Producao[j]->produto[k]->val << " Mascara: '" << Regras[i]->Producao[j]->produto[k]->str << "'" << endl;
    }}}
};

void Gramatica::imprimeProfs(){
    cout << "*-*-*-* Imprimindo Profundidades *-*-*-*\n\n";
    for(int i = 0; i < totalRegra; i++){
        cout << "Regra " << i << " -> " << Regras[i]->menorProfundidadeRegra << endl;
        for(int j = 0; j < Regras[i]->qnt; j++){
            cout << "   Producao " << j << " -> " << Regras[i]->Producao[j]->menorProfundidadeProd << endl;
        }
    }
};

void Gramatica::alturaGramatica(){
    //identifica primeiro quais regras tem terminal com profundidade 1
//        cout << totalRegra << endl;
//        cin.get();
    for(int i = 0; i < totalRegra; i++){
//            cout << "Regra " << i << endl;
        for(int j = 0; j < Regras[i]->qnt; j++){
//                cout << "   Producao " << j << endl;
            if(Regras[i]->Producao[j]->qnt == 1 && Regras[i]->Producao[j]->produto[0]->tipo != 0){
                Regras[i]->menorProfundidadeRegra = 1;
                Regras[i]->Producao[j]->menorProfundidadeProd = 0;
//                    cout << "       Tem Terminal\n";
            }
//                for(int k = 0; k < Regras[i]->Producao[j]->qnt; k++){
//                    if(Regras[i]->Producao[j]->produto[k]->tipo != 0){
//                        Regras[i]->menorProfundidadeRegra = 1;
//                        Regras[i]->Producao[j]->menorProfundidadeProd = 0;
//                        cout << "       Tem Terminal\n";
//                    }
//                }
        }
    }
    bool calculado = false;
    while(!calculado){
        for(int i = 0; i < totalRegra; i++){
//                cout << "Regra " << i << endl;
            for(int j = 0; j < Regras[i]->qnt; j++){
//                    cout << "   Producao " << j << endl;
                if(Regras[i]->Producao[j]->menorProfundidadeProd == -1){
                    bool ok = true;
                    for(int k = 0; k < Regras[i]->Producao[j]->qnt; k++){
                        if(Regras[i]->Producao[j]->produto[k]->tipo != 0){
//                                cout << "           Nao possivel calcular, Terminal sozinho\n";
                        }
                        else if(Regras[(int)Regras[i]->Producao[j]->produto[k]->val]->menorProfundidadeRegra == -1){
                            ok = false;
//                                cout << "           Nao possivel calcular, falta prof\n";
                        }
                        else if(Regras[i]->Producao[j]->produto[k]->val == i){
                            ok = false;
                            Regras[i]->Producao[j]->menorProfundidadeProd = -2;
//                                cout << "           Nao calcular, repete regra\n";
                        }
                    }
                    if(ok){
//                            cout << "           Calcular\n";
                        int profundidadeProducao = -1;
                        for(int k = 0; k < Regras[i]->Producao[j]->qnt; k++)
                            if(Regras[i]->Producao[j]->produto[k]->tipo == 0 && Regras[(int)Regras[i]->Producao[j]->produto[k]->val]->menorProfundidadeRegra > profundidadeProducao)
                                profundidadeProducao = Regras[(int)Regras[i]->Producao[j]->produto[k]->val]->menorProfundidadeRegra;
                        if(Regras[i]->Producao[j]->menorProfundidadeProd == -1)
                            Regras[i]->Producao[j]->menorProfundidadeProd = profundidadeProducao;
                        else if(Regras[i]->Producao[j]->menorProfundidadeProd > profundidadeProducao)
                            Regras[i]->Producao[j]->menorProfundidadeProd = profundidadeProducao;
                        if(Regras[i]->menorProfundidadeRegra == -1 || Regras[i]->menorProfundidadeRegra > profundidadeProducao)
                            Regras[i]->menorProfundidadeRegra = profundidadeProducao + 1;
                    }
                }
            }
        }

        calculado = true;
        for(int i = 0; i < totalRegra; i++){
            for(int j = 0; j < Regras[i]->qnt; j++){
                if(Regras[i]->Producao[j]->menorProfundidadeProd == -1){
                calculado = false;
//                        cout << "Outro turno\n";
                }
            }
        }
//            cout << endl;
//            imprimeProfs();
//            cin.get();
//            system("clear");
    }

    for(int i = 0; i < totalRegra; i++)
        for(int j = 0; j < Regras[i]->qnt; j++)
            if(Regras[i]->Producao[j]->menorProfundidadeProd == -2){
                for(int k = 0; k < Regras[i]->Producao[j]->qnt; k++){
                    if(Regras[i]->Producao[j]->produto[k]->tipo == 0 &&
                       Regras[(int)Regras[i]->Producao[j]->produto[k]->val]->menorProfundidadeRegra > Regras[i]->Producao[j]->menorProfundidadeProd){
                        Regras[i]->Producao[j]->menorProfundidadeProd = Regras[(int)Regras[i]->Producao[j]->produto[k]->val]->menorProfundidadeRegra;
                    }
                    if(Regras[i]->Producao[j]->menorProfundidadeProd < Regras[i]->menorProfundidadeRegra){
                        Regras[i]->menorProfundidadeRegra = Regras[i]->Producao[j]->menorProfundidadeProd;
                    }
                }
            }

//        cout << "\n\n   Altura da Gramatica calculada!!!\n\n";
//        imprimeProfs();
//        cin.get();
};

int Gramatica::randint(int x){
    if(x == 0){
        return 0;
    }
    return rand() % x;
};

Tree* Gramatica::criaArvExpO(){
    Tree* arv = new Tree();
//        Criar No inicial
    No* inicial = new No();
    inicial->setNo(0, 0, Mapa[0]->getS()); //<EXP>
    arv->setTree(inicial);
    criaNoO(inicial);
    arv->refresh();
    return arv;
};

Tree* Gramatica::criaArvExpO(int valor){
    if(valor < totalRegra){
        Tree* arv = new Tree();
//            Criar No inicial
        No* inicial = new No();
        inicial->setNo(0, valor, Mapa[valor]->getS()); //<NT>
        criaNoO(inicial);
        arv->setTree(inicial);
        arv->refresh();
        return arv;
    }
    else{
        cout << "Nao existe regra com este valor";
        return NULL;
    }
};

No* Gramatica::criaArvExp(float t, float v, int prof){
//        Criar No inicial
    No* inicial = new No();
    inicial->setNo(t, v, Regras[(int)t]->Producao[(int)v]->produto[0]->str);
    criaNo(inicial, prof);
    return inicial;
};

void Gramatica::criaNoO(No* n){
    if(n->type == 0){
        int sorteado = randint(Regras[(int)n->value]->qnt);
        for(int i = 0; i < Regras[(int)n->value]->Producao[sorteado]->qnt; i++){
            No* f = new No();
            f->setNo(Regras[(int)n->value]->Producao[sorteado]->produto[i]->tipo, Regras[(int)n->value]->Producao[sorteado]->produto[i]->val, Regras[(int)n->value]->Producao[sorteado]->produto[i]->str);
            n->setSon(f);
            criaNoO(f);
        }
    }
};

Tree* Gramatica::criaArvExp(){
    Tree* arv = new Tree();
    //Criar No inicial
    No* inicial = new No();
    inicial->setNo(0, 0, "<exp>"); //<EXP>
    inicial->setFather(NULL, -1);
    criaNo(inicial, 1);
    arv->setTree(inicial);
    arv->refresh();
    return arv;
};


void Gramatica::criaNo(No* n, int prof){
    if(n->type == 0){
        int valor = n->value;
        int pos = 0;
//            n->print(0);
        while(pos < Regras[valor]->qnt && (Regras[valor]->Producao[pos]->menorProfundidadeProd + prof) < conf->treeHigh){
            pos++;
        }
//            for(int p = 0; p < prof; p++) cout << "   ";
//            cout << "Profundidade " << prof << endl << pos << " possibilidades de derivacao para " << valor << endl;
//            cout << pos << endl;
        if(pos > 0){
            int sorteado = randint(pos);
            n->derivation = sorteado;

            n->sons.resize(Regras[valor]->Producao[sorteado]->qnt);

//                for(int p = 0; p <= prof; p++) cout << "   ";
//                cout << "escolhido " << sorteado << endl;
            for(int i = 0; i < Regras[valor]->Producao[sorteado]->qnt; i++){
                No* p = new No();
                p->setNo(Regras[valor]->Producao[sorteado]->produto[i]->tipo, Regras[valor]->Producao[sorteado]->produto[i]->val, Regras[valor]->Producao[sorteado]->produto[i]->str);
                p->setFather(n, i);
                n->setSon(p);
                criaNo(p, prof + 1);
            }
        }
    }
};

void Gramatica::criaNoGuiado(No* n, No* q){
    if(q->type == 0){
        n->setNo(q->type, q->value, q->mask);
        n->derivation = q->derivation;
        int valor = n->value;
        n->sons.resize(Regras[valor]->Producao[n->derivation]->qnt);

//            for(int p = 0; p <= prof; p++) cout << "   ";
//            cout << "escolhido " << sorteado << endl;
        for(int i = 0; i < Regras[valor]->Producao[n->derivation]->qnt; i++){
            No* p = new No();
            p->setNo(Regras[valor]->Producao[n->derivation]->produto[i]->tipo, Regras[valor]->Producao[n->derivation]->produto[i]->val, Regras[valor]->Producao[n->derivation]->produto[i]->str);
            p->setFather(n, i);
            n->setSon(p);
            criaNoGuiado(p, q->sons.at(i));
        }
    }
};

