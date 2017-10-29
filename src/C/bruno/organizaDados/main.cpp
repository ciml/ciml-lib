#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <algorithm>
#include <iomanip>
using namespace std;

typedef struct {
    string individuo;
    int tamanho;
    float erro;
} melhoresResultados;

bool comparaErro(const melhoresResultados &lhs, const melhoresResultados &rhs) {
    return lhs.erro < rhs.erro;
}

melhoresResultados get(list<melhoresResultados> _list, int _i){
    list<melhoresResultados>::iterator it = _list.begin();
    for(int i=0; i<_i; i++){
        ++it;
    }
    return *it;
}



void cabecalho(){
    cout << std::fixed;
    cout << std::setprecision(15);

    cout << "Organizando resultados..." << endl;
    ofstream resultados("tabelaResultadosTeste.txt");

    resultados.precision(15);
    resultados.setf(ios::fixed);
    resultados.setf(ios::showpoint);

    resultados << left << setw(30) << "Prob. Mutacao" <<" | "
               << left << setw(35) << "Melhor"        << " | "
               << left << setw(35) << "Mediana"       <<" | "
               << left << setw(35) << "Media"         <<" | "
               << left << setw(35) << "Desvio Padrão" <<" | "
               << left << setw(35) << "Pior"          <<" | "
               << left << setw(35) << "Modelo (Melhor)"  <<"\n";

    resultados.close();
}

void analisaDados(string nomeArquivo){
    cout << std::fixed;
    cout << std::setprecision(15);

    cout << "Organizando resultados..." << endl;
    ofstream resultados("tabelaResultadosTeste.txt", ios_base::app);

    resultados.precision(15);
    resultados.setf(ios::fixed);
    resultados.setf(ios::showpoint);

    /*
    resultados << left << setw(30) << "Prob. Mutacao"  <<" | "
               << left << setw(35) << "Media"         <<" | "
               << left << setw(35) << "Mediana"       <<" | "
               << left << setw(35) << "Melhor"        << " | "
               << left << setw(35) << "Modelo"        <<"\n";
    */

    list<melhoresResultados> bestResult;
    melhoresResultados auxMelhores;

    string indiv;
    int tam;
    float erro;

    string aux1;
    string aux2;

    float media = 0;
    float mediana = 0;

    int prob = 3;
    int it = 0;

//    string nomeArquivo1;
//    file >> nomeArquivo1;
//    cout << nomeArquivo1;

    //for(it = 0; it < 3; it++){
    //while(file>>nomeArquivo1){
        //string nomeArquivo1;
        //nomeArquivo1 << "resultadosProbMut0_" << prob << ".txt";
        //lista >> nomeArquivo1
        //cout << nomeArquivo1 <<endl;

        ifstream file(nomeArquivo);
        //ifstream file("MelhoresIndividuos.txt");


        while(file.good()){
            getline(file, indiv, ':');
            getline(file, aux2, ' ');
            getline(file, indiv, '|');

            getline(file, aux1, ':');
            getline(file, aux1, ' ');
            getline(file, aux1, '|');


            getline(file, aux2, ':');
            getline(file, aux2, ' ');
            getline(file, aux2);

            tam = atoi(aux1.c_str());
            erro = atof(aux2.c_str());

            auxMelhores.individuo = indiv;
            auxMelhores.tamanho = tam;
            auxMelhores.erro = erro;


            bestResult.push_front(auxMelhores);
        }
        //está repetindo a ultima linha
        bestResult.pop_front();

//        while(!bestResult.empty()){
//            cout << bestResult.front().erro << endl;
//            bestResult.pop_front();
//        }
//
        bestResult.sort(&comparaErro);
        mediana = get(bestResult, bestResult.size()/2).erro;
        for (list<melhoresResultados>::const_iterator iterator = bestResult.begin(), end = bestResult.end(); iterator != end; ++iterator) {
            media+= (iterator)->erro;
        }

        media = media/bestResult.size();
        float desvPd = 0;
        list<melhoresResultados> aux;
        aux = bestResult;


        for(int i = 0; i < bestResult.size(); i++){
            desvPd += (aux.front().erro - media);
            aux.pop_front();
        }
        desvPd = sqrt(desvPd / (bestResult.size()-1));

        stringstream printProb;
        printProb << "0." << prob;
        resultados  << setw(30) << nomeArquivo << setfill(' ') <<" | "
                    << left << setw(35) << bestResult.front().erro << setfill(' ') <<" | "
                    << left << setw(35) << mediana <<setfill(' ') << " | "
                    << left << setw(35) << media << setfill(' ') << " | "
                    << left << setw(35) << desvPd << setfill(' ') << " | "
                    << left << setw(35) << bestResult.back().erro << setfill(' ') <<" | "
                    << left << setw(35) << bestResult.front().individuo <<setfill(' ') << "\n";

        file.close();
        prob++;
    //}

    resultados.close();
}

//void leScript(const char *caminho){
//    ifstream entrada(caminho);
//    string leitura;
//    ofstream saida("MelhoresIndividuos.txt");
//
//    while(entrada>>leitura){
//
//        leArquivo(leitura.c_str());
//        saida << leitura << endl;
//
//        saida << "HD Guloso: " << r[1] << " | " << (double)(eNd.QuadPart - start.QuadPart)/frequency.QuadPart << " s " << endl;
//
//        saida << "HD GulosoRandomizado: " << r[1] << " | " << (double)(eNd.QuadPart - start.QuadPart)/frequency.QuadPart << " s " << endl;
//
//        saida << "HD GulosoReativo: " << r[1] << " | " << (double)(eNd.QuadPart - start.QuadPart)/frequency.QuadPart << " s " << endl;
//
//    }
//}




//passar os nomes por parametro no mais
int main2(int argc, char **argv){
    cout << argv[1] << endl;
    ifstream entrada(argv[1]);
    string leitura, leitura2, linha;

    fstream saida("MelhoresIndividuos.txt");
    cabecalho();

    cout << "Selecionando melhores resultados..." << endl;

    //ofstream tabela("tabelaResultados.txt");


    while(entrada>>leitura){
        cout << leitura << endl;
//        char c;
//        cin >> c;

        ifstream file(leitura.c_str());

        string sufixo = leitura.substr(leitura.find("_"));
        ofstream saida2(sufixo.c_str());
        cout << sufixo << endl;

        while(file >> leitura2){

            ifstream file2(leitura2.c_str());
//            //saida2 << sufixo << endl;
            size_t pos;
            while (getline(file2, linha)){

                pos = linha.find("*MELHOR");
                if(pos != string::npos){
                    saida << linha << endl;
                    saida2 << linha << endl;
                }
            }
        }
        saida2.close();

        analisaDados(sufixo.c_str());
        file.close();
    }

//    for(it = 0; it < 3; it++){
//        stringstream nomeArquivo1;
//        stringstream nomeArquivo2;
//        //nomeArquivo1 << "probMut0_" << prob << ".txt";
//        //cout << nomeArquivo1.str()<<endl;
//        //nomeArquivo2 << "resultadosProbMut0_" << prob << ".txt";
//        //cout << nomeArquivo2.str()<<endl;
//
//        //tabela << "probMut0_" << prob << endl;
//
//        ifstream file(nomeArquivo1.str().c_str());
//        ofstream resultados(nomeArquivo2.str().c_str());
//
//        string linha;
//
//        size_t pos;
//        while (getline(file, linha)){
//            pos = linha.find("*MELHOR");
//            if(pos != string::npos){
//                resultados << linha << endl;
//                tabela << linha << endl;
//            }
//        }
//        file.close();
//        resultados.close();
//        prob++;
//    }
    //analisaDados(saida);
}

int main(int argc, char **argv){
    cout << argv[1] << endl;
    ifstream entrada(argv[1]);
    string leitura, leitura2, linha;

    fstream saida("MelhoresIndividuos.txt");
    //cabecalho();

    cout << "Selecionando melhores resultados..." << endl;

    //ofstream tabela("tabelaResultados.txt");

    while(entrada>>leitura){

        cout << leitura << endl;

//        char c;
//        cin >> c;

        ifstream file(leitura.c_str());
       // ofstream saida2("tempos.txt");
        string prefixoTempo ("times_");
        string sufixo = prefixoTempo + leitura.substr(0,leitura.find("_seed")) + leitura.substr(leitura.find("_popVal"), leitura.size());
        ofstream saida2(sufixo.c_str(), ios::app);

        cout << sufixo << endl;
            size_t pos;
            while (getline(file, linha)){
                pos = linha.find("Tempo total Evolucao");
                if(pos != string::npos){
                    cout << linha << endl;
                    saida << linha.substr(linha.find("=")+2, linha.size()) << " ";
                    saida2 <<  linha.substr(linha.find("=")+2, linha.size()) << " ";
                }else {
                    pos = linha.find("Tempo total Avaliacao");
                    if(pos != string::npos){
                        cout << linha << endl;
                        saida << linha.substr(linha.find("=")+2, linha.size()) << " ";
                        saida2 <<  linha.substr(linha.find("=")+2, linha.size()) << " ";
                    } else {
                        pos = linha.find("Tempo total");
                        if(pos != string::npos){
                            cout << linha << endl;
                            saida << linha.substr(linha.find("=")+2, linha.size()) << " ";
                            saida2 <<  linha.substr(linha.find("=")+2, linha.size()) << endl;
                        }
                    }
                }
            }
        //saida2 << endl;
        saida2.close();

        //analisaDados(sufixo.c_str());
        file.close();
    }

}
