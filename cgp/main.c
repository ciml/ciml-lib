/**
 * @file main.c
 *
 * @brief Execute CGP
 *
 * @details This file executes evolutionary process over many different CGP's.
 *
 * @author Lucas Augusto Müller de Souza (lucas.mullers@gmail.com)
 *
 * @date february 20, 2019
 *
 * @copyright Distributed under the Mozilla Public License 2.0 ( https://opensource.org/licenses/MPL-2.0 )
 *
 * @see https://github.com/lucasmullers/
 *
 * Created on: january 15, 2019
 */


#include <time.h>
#include "include/cgp.h"

int maxgen;
int mutation;

int evolves_cgp_bdd(Individual *population, Table *table, int *gates)
{
    long int generation = 0;

    evaluate_parent_sat_count(population, table);
    evaluate_population_sat_count(population, table);

    int best_individual = find_best_individual_sat_count(population);
    set_parent(population, best_individual);

    clone_parent(population);
    fprintf(out_file, "--------------------------\n");
    while (1)
    {
        if(mutation == 1)
        {
            apply_SAM(population, gates, table->num_inputs);
        }
        else if(mutation == 2)
        {
            apply_GAM(population, gates, table->num_inputs);
        }
        else if(mutation == 3)
        {
            apply_SAM_plus_GAM(population, gates, table->num_inputs);
        }

        evaluate_population_sat_count(population, table);
        best_individual = find_best_individual_sat_count(population);
        set_parent(population, best_individual);

        if (population[0].score == 0)
        {
            fprintf(out_file, "SAT COUNT: %d INDIVIDUO: %d GERACAO: %ld\n", population[0].score, best_individual, generation);
            break;
        }
        if (generation % 25000 == 0)
        {
            fprintf(out_file, "SAT COUNT: %d INDIVIDUO: %d GERACAO: %ld\n", population[0].score, best_individual, generation);
        }
        if (bdd_getnodenum() >= (int)(0.95 * bdd_getallocnum()))
        {
            bdd_gbc();
        }
        if(generation == maxgen)
        {
            fprintf(out_file, "SAT COUNT: %d INDIVIDUO: %d GERACAO: %ld\n", population[0].score, best_individual, generation);
            return 0;
        }

        clone_parent(population);
        generation++;
    }
    fprintf(out_file, "--------------------------\n");
    print_post_optimization_data(&population[0], table->num_inputs);
    maxgen -= generation;

    return 1;
}

void optimize_circuit(Individual *population, Table *table, int *gates)
{

    fprintf(out_file,"--------------------------\n");

    int best_individual = 0;
    long int generation = 0;
    while (1)
    {
        if (mutation == 1)
        {
            apply_SAM(population, gates, table->num_inputs);
        }
        else if (mutation == 2)
        {
            apply_GAM(population, gates, table->num_inputs);
        }
        else if (mutation == 3)
        {
            apply_SAM_plus_GAM(population, gates, table->num_inputs);
        }

        evaluate_population_sat_count(population, table);
        clear_population_active_genes(population);
        find_population_active_genes(population, table->num_inputs);
        best_individual = find_optimized_individual(population);
        set_parent(population, best_individual);

        if (generation % 25000 == 0)
        {
            fprintf(out_file,"NUM GATES: %d INDIVIDUO: %d GERACAO: %ld\n", population[0].num_gates, best_individual, generation);
        }
        if (bdd_getnodenum() >= (int)(0.95 * bdd_getallocnum()))
        {
            bdd_gbc();
        }
        if (generation == maxgen)
        {
            fprintf(out_file,"NUM GATES: %d INDIVIDUO: %d GERACAO: %ld\n", population[0].num_gates, best_individual, generation);
            break;
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
    sscanf(argv[2], "seed=%d", &semente);
    sscanf(argv[3], "ncol=%d", &NCOL);
    sscanf(argv[5], "sam=%d", &SAM);
    sscanf(argv[6], "mutation=%d", &mutation);
    LB = NCOL / 2;
    srand(semente);

    if (argc == 8)
    {
        out_file = fopen(argv[7], "w");
    }
    else
    {
        out_file = stdout;
    }

    if (mutation == 1)
    {
        fprintf(out_file, "SAM\n");
    }
    else if (mutation == 2)
    {
        fprintf(out_file, "GAM\n");
    }
    else if (mutation == 3)
    {
        fprintf(out_file, "SAM:%d+GAM:%d\n", SAM, NPOP - 1 - SAM);
    }
    else
    {
        fprintf(out_file, "Mutation code note specified or not recognized!\n ");
        exit(1);
    }

    bdd_init(100000000, 100000);

    Individual *population = (Individual *)malloc(sizeof(Individual) * NPOP);
    int gates[NGATES] = {1, 2, 3, 4, 5, 6, 7};

    Table *table = (Table *)malloc(sizeof(Table));
    table_constructor(table, argv[1]);

    initialize_population(population, gates, table->num_inputs, table->num_outputs);

    clock_t begin, end;
    begin = clock();

    sscanf(argv[4], "maxgen=%d", &maxgen);

    if (evolves_cgp_bdd(population, table, gates))
    {
        optimize_circuit(population, table, gates);
    }


    bdd_done();
    end = clock();
    fprintf(out_file, "TOTAL TIME: %f seconds\n", (end - begin) / (double)CLOCKS_PER_SEC);

    free(population);
    free(table);

    return 0;
}
