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
int evolves_cgp_bdd(Individual *population, Table *table, int *gates)
{
    long int generation = 0;

    evaluate_parent_sat_count(population, table);
    evaluate_population_sat_count(population, table);

    int best_individual = find_best_individual_sat_count(population);
    set_parent(population, best_individual);

    clone_parent(population);
    fprintf(out_file, "--------------------------\n");
    fflush(out_file);
    while (1)
    {
        if(mutation == 1)
            apply_SAM(population, gates, table->num_inputs);
        else if(mutation == 2)
            apply_SAM_plus_GAM(population, gates, table->num_inputs);
        else if(mutation == 3)
            apply_PM(population, gates, table->num_inputs);
        evaluate_population_sat_count(population, table);
        best_individual = find_best_individual_sat_count(population);
        set_parent(population, best_individual);

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

        clone_parent(population);
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

        if (generation % 1000 == 0)
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
    LB = NCOL/2;
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
