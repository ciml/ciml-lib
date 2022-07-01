/**
 * @file cgp.h
 *
 * @brief Cartesian Genetic Programming Algorithm
 *
 * @details This file implements the Cartesian Genetic Programming Algorithm based in 
 * those materials listed below.
 *  -Cartesian Genetic Programming - ISBN-10: 3642269982 ISBN-13: 978-3642269981
 *  -How to evolve complex circuits from scratch - DOI: 10.1109/ICES.2014.7008732
 *  -CGP with Guided and Single Active Mutations for Designing CLCs - DOI: 
 * 
 * @author Lucas Augusto Müller de Souza (lucasmuller@ice.ufjf.br)
 * Computational Engineering student at Universidade Federal de Juiz de Fora
 *
 *
 * @copyright Distributed under the Mozilla Public License 2.0 ( https://opensource.org/licenses/MPL-2.0 )
 *
 * @code available at https://github.com/ciml/ciml-lib/tree/applied-soft-computing-2019
 * @see https://github.com/lucasmullers/
 *
 * Created on: january 15, 2019
 * Updated on: october 27, 2019
 */


#include <time.h>
#include <bdd.h>
#include "include/cgp.h"

long int maxgen;
long int mediangen;
int mutation;

/**
* @brief Function that implements the cartesian genetic programming evolutionary process to
* evolve CLCs. The process start with a random population and ends with the first factible
* circuit found.
* @param population - the population struct that will be used into the evolution
* @param table - the table struct that stores the circuits information
* @param gates - the logical gates used into the evolution
* @return 1 if a factible ciruit is found and 0 otherwise
*/

int getMax(Individual array[]) {
    int max = array[0].score;
    for(int i = 1; i < NPOP; i++) {
        if(array[i].score > max)
            max = array[i].score;
    }
    return max;
}

void sort_pop(Individual *population) {
    long int output[NPOP];
    Individual sorted_pop[NPOP];

    int max = getMax(population);

    int *count = malloc((max + 1)*sizeof(*count));

    for(int i = 0; i <= max; i++) {
        count[i] = 0;     //inicializa vetor de contagem com 0 em todas as posições
    }

    for(int i = 0; i < NPOP; i++) {
        count[population[i].score]++;     //aumenta a contagem de cada elemento no vetor de contagem
    }

    for(int i = 1; i <= max; i++)
        count[i] += count[i-1];     //encontra frequência acumulada

    for(int i = NPOP - 1; i >= 0; i--) {
        output[count[population[i].score] - 1] = population[i].score;
        sorted_pop[(count[population[i].score] - 1)] = population[i];
        count[population[i].score] -= 1; //diminui a contagem para os números do vetor
    }

    for(int i = 0; i < NPOP; ++i)
        population[i] = sorted_pop[i];

    free(count);
}

void compare_pops(Individual parent[], Individual children[]) {
    for(int i = 0; i < NPOP; i++) {
        if(parent[i].score >= children[i].score) {
            parent[i] = children[i];
        }
    }
}

int evolves_cgp_bdd(Individual *population, Table *table, int *gates)
{
    long int generation = 0;

    evaluate_parent_sat_count(population, table);
    evaluate_population_sat_count(population, table);

    Individual population_children[NPOP];
    int best_individual;
    //int best_individual = find_best_individual_sat_count(population);
    //set_parent(population, best_individual);

    //clone_parent(population);
    fprintf(out_file, "--------------------------\n");
    fflush(out_file);
    while (1)
    {

        sort_pop(population);
        // copy parent to child
        best_individual = 0;

        for(int i = 0; i < NPOP; i++) {
            population_children[i] = population[i];
        }


        if(mutation == 1)
            apply_SAM(population_children, gates, table->num_inputs);
        else if(mutation == 2)
            apply_SAM_plus_GAM(population, gates, table->num_inputs);
        else if(mutation == 3)
            apply_PM(population, gates, table->num_inputs);
        else if(mutation == 4) {
            apply_MAM(population_children, gates, table->num_inputs);
        }

        evaluate_all_pop(population_children, table);

/*        for(int i = 0; i < NPOP; i++) {
            printf("Father: %ld", population[i].score);
            printf(" Child: %ld", population_children[i].score);
        }
        printf("\n");*/

        printf("---------------------------------------\n");
        for(int i = 0; i < NPOP; i++) {
            printf("%ld ", population[i].score);
        }
        printf("\n");
        for(int i = 0; i < NPOP; i++) {
            printf("%ld ", population_children[i].score);
        }
        printf("\n");
        compare_pops(population, population_children);
        for(int i = 0; i < NPOP; i++) {
            printf("%ld ", population[i].score);
        }
        printf("\n");
        printf("---------------------------------------\n");

        printf("--------------sort----------------------\n");
        sort_pop(population);
        for(int i = 0; i < NPOP; i++) {
            printf("%ld ", population[i].score);
        }
        printf("\n");
        printf("---------------------------------------\n");
        //best_individual = find_best_individual_sat_count(population);
        //set_parent(population, best_individual);

        if (population[0].score == 0)
        {
            fprintf(out_file, "SAT COUNT: %ld INDIVIDUAL: %d GENERATION: %ld\n", population[0].score, best_individual, generation);
            fflush(out_file);
            break;
        }
        if (generation % 50000 == 0)
        {
            fprintf(out_file, "SAT COUNT: %ld INDIVIDUAL: %d GENERATION: %ld\n", population[0].score, best_individual, generation);
            fflush(out_file);
        }
        if(bdd_getnodenum() >= (int) (0.75 * bdd_getallocnum()))
        {
            bdd_gbc();
        }
        if(generation == maxgen)
        {
            fprintf(out_file, "SAT COUNT: %ld INDIVIDUAL: %d GENERATION: %ld\n", population[0].score, best_individual, generation);
            fflush(out_file);
            return 0;
        }

        //clone_parent(population);
        generation++;
    }
    fprintf(out_file, "--------------------------\n");
    fflush(out_file);
    print_post_optimization_data(&population[0], table->num_inputs);

    maxgen = maxgen - generation;

    return 1;
}

/**
* @brief Function that implements the cartesian genetic programming evolutionary process to
* optimize CLCs. The process start with a factible population and ends with best factible
* circuit found (the best individual is choosed with respect to the lower number of transistors).
* @param population - the population struct that will be used into the evolution
* @param table - the table struct that stores the circuits information
* @param gates - the logical gates used into the evolution
* @return none
*/
void optimize_circuit(Individual *population, Table *table, int *gates)
{

    fprintf(out_file,"--------------------------\n");
    fflush(out_file);

    int best_individual = 0;
    long int generation = 0;
    clock_t start = clock();
    while (1)
    {
        if (mutation == 1)
            apply_SAM(population, gates, table->num_inputs);
        else if (mutation == 2)
            apply_SAM_plus_GAM(population, gates, table->num_inputs);
        else if (mutation == 3)
            apply_PM(population, gates, table->num_inputs);
        evaluate_population_sat_count(population, table);
        clear_population_active_genes(population);
        find_population_active_genes(population, table->num_inputs);
        best_individual = find_optimized_individual(population);
        set_parent(population, best_individual);

        if (generation % 50000 == 0)
        {
            fprintf(out_file,"NUM TRANSISTORS: %d INDIVIDUAL: %d GENERATION: %ld\n", population[0].num_transistors, best_individual, generation);
            fflush(out_file);
        }
        if (bdd_getnodenum() >= (int)(0.75 * bdd_getallocnum()))
        {
            bdd_gbc();
        }
        if((clock() - start) / (double)CLOCKS_PER_SEC >= 3600.0)
        {
            print_post_optimization_data(&population[0], table->num_inputs);
            start = clock();
        }
        if (generation == maxgen)
        {
            fprintf(out_file,"NUM TRANSISTORS: %d INDIVIDUAL: %d GENERATION: %ld\n", population[0].num_transistors, best_individual, generation);
            fflush(out_file);
            break;
        }
        if(generation == mediangen)
        {
            fprintf(out_file,"NUM TRANSISTORS: %d INDIVIDUAL: %d GENERATION: %ld\n", population[0].num_transistors, best_individual, generation);
            print_post_optimization_data(&population[0], table->num_inputs);
        }
        clone_parent(population);
        generation++;
    }
    fprintf(out_file,"--------------------------\n");
    print_post_optimization_data(&population[0], table->num_inputs);
}

int main(int argc, char const *argv[])
{
    int semente;
    mediangen = -1;
    sscanf(argv[2], "seed=%d", &semente);
    sscanf(argv[3], "ncol=%d", &NCOL);
    sscanf(argv[4], "maxgen=%ld", &maxgen);
    sscanf(argv[5], "mutation=%d", &mutation);
    LB = NCOL;
    srand(semente);

    if (argc == 7)
    {
        out_file = fopen(argv[6], "w");
    }
    else if(argc == 9)
    {
        out_file = fopen(argv[8], "w");
    }
    else
    {
        out_file = stdout;
    }

    if (mutation == 1)
        fprintf(out_file, "SAM\n");
    else if (mutation == 2)
        fprintf(out_file, "SAM+GAM\n");
    else if (mutation == 3)
        fprintf(out_file, "PM\n");
    else if (mutation == 4)
        fprintf(out_file, "MAM\n");
    else
    {
        fprintf(out_file, "Mutation value isnt valid!\n");
        exit(1);
    }
    fflush(out_file);
    bdd_init(10000000, 100000);

    Individual *population = (Individual *)malloc(sizeof(Individual) * NPOP);
    int gates[NGATES] = {1, 2, 3, 4, 5, 6, 7};

    Table *table = (Table *)malloc(sizeof(Table));
    table_constructor(table, argv[1]);

    initialize_population(population, gates, table->num_inputs, table->num_outputs);

    clock_t begin, end;
    begin = clock();

    if(argc <= 7)
    {
        if (evolves_cgp_bdd(population, table, gates))
        {
            optimize_circuit(population, table, gates);
        }
    }
    else if(argc > 7)
    {
        int aux;
        sscanf(argv[6], "ngates=%d", &aux);
        sscanf(argv[7], "mediangen=%ld", &mediangen);
        mediangen = maxgen - mediangen;
        sow_population(&population[0], table, argv[1], aux);
        clear_individiual_active_genes(&population[0]);

        calculate_individual_sat_count(&population[0], table); 
        if (population[0].score != 0)
        {
            printf("Sow population didn't work!\n");
            exit(1);
        }
        clone_parent(population);
        optimize_circuit(population, table, gates);
    }

    bdd_done();
    end = clock();
    fprintf(out_file, "TOTAL TIME: %f seconds\n", (end - begin) / (double)CLOCKS_PER_SEC);
    fflush(out_file);

    free(population);
    free(table);

    return 0;
}
