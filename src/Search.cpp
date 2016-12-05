#include "Search.h"

/**
* Construtores da função de busca
*   Recebem todos os parametros relativos a busca
*   Parameters
*       Parser - responsavel por avaliar os individuos
*       PopulationReplace - reponsavel pela substituição da população para a proxima geração
*       IndividuoBuilder - responsavel pela criação de individuos
*       Mutation - Operador de mutação
*       Crossover - Operador genetico de mutação
*       Selection - Operador de seleção de indivudos
**/
Search::Search()
{
    setParser(NULL);
    setPopulationReplace(NULL);
    setIndividuoBuilder(NULL);
    setMutation(NULL);
    setCrossover(NULL);
    setSelection(NULL);

   // omp_set_num_threads(conf->NUM_THREADS);

}


Search::Search(Parser* parser,PopulationReplacement * populationreplace,IndividuoBuilder * individuobuilder)
{
    setParser(parser);
    setPopulationReplace(populationreplace);
    setIndividuoBuilder(individuobuilder);

    setMutation(NULL);
    setCrossover(NULL);
    setSelection(NULL);

   // omp_set_num_threads(conf->NUM_THREADS);

}

/**
    Função privada que instancia e preenche a população inicial de individuos
**/
void Search::createsInitialPopulation(){
    cout << "Create Initial Population..." << endl;
    pop = new Subject*[conf->popSize * 2];

   // #pragma omp parallel for num_threads(conf->NUM_THREADS)
    for(int i = 0; i < conf->popSize; i++){
        pop[i] = individuoBuilder->generateIndividuo();
    }
}

/**
*
*   Realiza a busca atravez do processo evolutivo
*   @return void
**/
void Search::evolve(){

    double last_best_fitness;
    int tolerance_count = 0;

    printParameters();

    this->createsInitialPopulation();

    //initial population evaluation
    EvaluatePopulation(0, conf->popSize);

    sort(pop, pop + conf->popSize, SortPopulationFitness);

    last_best_fitness = INFINITY;

    cout << "Evolving..." << endl;
    for(int it = 1; it < conf->generations; it++){
        //aplica operadores geneticos
         //cout << "Operate" << endl;
        Operate();
        // cout << "Evaluate" << endl;
        //Avalia população
        EvaluatePopulation(conf->popSize, conf->popSize * 2);
         //cout << "Replace" << endl;
        //substitui a população
        replacer->Replace(pop);

        // free temporary population
        // cout << "Free" << endl;
       // #pragma omp parallel for num_threads(conf->NUM_THREADS)
        for(int i = conf->popSize; i < conf->popSize * 2; i++){
            delete pop[i];
        }

        // cout << "print" << endl;
        if(stepByStep){
            cout << " generation " << it;
            printBestIndividuo();
        }

//        if(pop[0]->fitness < tolerance){
//            cout << "The process lasted: " << it << " generations " << endl;
//            break;
//        }
    }

    printResult();

}

void Search::Replace(){
    sort(pop, pop + conf->popSize * 2, SortPopulationFitness);
}

/**
    Parameters
        int initialIndex - indice inicial a ser avaliado
        int finalIndex - indice final a pop sera avaliada até o finalIndex -1
    Percorre o vetor pop (população) avaliando os individuos do indice initialIndex até finalIndex
    @return void
**/
void Search::EvaluatePopulation(int initialIndex, int finalIndex){

    #pragma omp parallel for num_threads(conf->NUM_THREADS)
    for(int i = initialIndex; i < finalIndex; i++){
        parser->Evaluate(pop[i]);
    }
}
/**
    Aplica os operadores genéticos de mutação e cruzamento
    @return void
**/
void Search::Operate(){
    int num_individuos = seletor->tam_selection;
    Subject ** selecionados;

    //#pragma omp parallel for private(selecionados)
    for(int i = conf->popSize; i < conf->popSize * 2; i += num_individuos){

        selecionados = new Subject*[num_individuos];

        for(int j = 0; j < num_individuos; j++){
            selecionados[j] = seletor->selects(pop)->clone();
            pop[i+j] = selecionados[j];
        }

        crossoverOperator->cross(selecionados,num_individuos);

        for(int j = 0; j < num_individuos; j++){
            mutationOperator->mutate(selecionados[j]);
        }

        for(int j = 0; j < num_individuos; j++){
            selecionados[j] = NULL;
        }
        delete selecionados;
    }

}


/**
    Set Parameters
**/
void Search::setParser(Parser * parser){
  if(parser==NULL){
        SimpleParser * avaliador  = new SimpleParser();
        avaliador->setDataSet(data->training,data->totalTraining);
        this->parser = avaliador;
    }else{
        this->parser = parser;
    }
}

void Search::setPopulationReplace(PopulationReplacement * popReplace){
    if(popReplace == NULL){
        this->replacer = new SimpleReplace();
    }else{
        this->replacer = popReplace;
    }
}

void Search::setIndividuoBuilder(IndividuoBuilder * indBuilder){
    if(indBuilder == NULL){
        this->individuoBuilder = new SimpleIndividuoBuilder();
    }else{
        this->individuoBuilder = indBuilder;
    }
}

void Search::setMutation(Mutation * opMutation){
    if(opMutation==NULL){
        mutationOperator = new SimpleMutation();
    }else{
        mutationOperator = opMutation;
    }
}

void Search::setCrossover(Crossover * opCrossover){
    if(opCrossover==NULL){
        crossoverOperator = new SimpleCrossover();
    }else{
        crossoverOperator = opCrossover;
    }
}

void Search::setSelection(Selection * seletor){
    if(seletor==NULL){
        this->seletor = new TournamentSelection();
    }else{
        this->seletor = seletor;
    }
}

/** Print Functions **/
void Search::printParameters(){
    cout << endl;
    cout << "************* PARAMETERS **************" << endl;
    cout << "*   GENERATIONS    " << conf->generations << endl;
    cout << "*   POP SIZE       " << conf->popSize << endl;
    cout << "*   ELITISM        " << conf->elitism << endl;
    cout << "*   MAXDEEP        " << conf->MAXDEEP << endl;
    cout << "*   CROSSOVER RATE " << conf->crossoverRate << endl;
    cout << "*   MUTATION RATE  " << conf->mutationRate << endl;
    cout << "*   INDIVIDUO      " << individuoBuilder->nameIndividuo() << endl;
    cout << "*   PARSER         " << parser->nameParser() << endl;
    cout << "*   THREADS AVAILABLE " << omp_get_num_threads() << endl;
    cout << "***************************************" << endl << endl;
}

void Search::printResult(){
    cout << "--------------Results--------------" << endl;
    pop[0]->print();

}

void Search::printBestIndividuo(){

        pop[0]->print();
//        cout << " (" << pop[0]->fitness << ") " << endl;
//        for(int m = 0; m < conf->numTree; m++){
//            cout << "   f" << m << "() = (" << pop[0]->trees[m]->fitness << ") ";
//            pop[0]->trees[m]->root->print();
////            cout << "leastSquare=[";
////            for(int j = 0; j < pop[0]->trees[m]->leastSquareSize; j++)
////                cout << pop[0]->trees[m]->leastSquare[j] << ", ";
////            cout << "]";
//        }
        cout << endl;
}

 bool Search::SortPopulationFitness(Subject* a, Subject* b){
    return a->fitness < b->fitness;
 }

Search::~Search()
{
    //dtor
}
