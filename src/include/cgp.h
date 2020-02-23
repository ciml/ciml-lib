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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <bdd.h>

#define NROW 1
#define NPOP 5
#define MUTATIONRATE 0.05
#define NGATES 7

extern char *strtok_r(char *, const char *, char **);

FILE *out_file;
int LB;
int NCOL;

/**
 * GATE CODES
 * 1 - AND
 * 2 - OR
 * 3 - NOT
 * 4 - NAND
 * 5 - NOR
 * 6 - XOR
 * 7 - XNOR
*/

/* TABLE DEFINITION */
typedef struct Table
{
    int num_rows;
    int num_cols;
    int num_inputs;
    int num_outputs;
    char circuit_name[100];
    bdd *input_vars;
    bdd *outputs_bdd;
} Table;

/* GENE DEFINITION */
typedef struct Gene
{
    /* Datas */
    int gate;
    int input[2];
    int active;
    bdd bdd_output;
} Gene;

/* INDIVIDUAL DEFINITION */
typedef struct Individual
{
    Gene *genotype;
    int genotype_size;
    int *output;
    int output_size;
    long int *score_per_output;
    long int score;
    int num_transistors;

} Individual;


/**
* @brief Function that randomize a value between smaller value(included) and greater value(not included)
* @param smaller_value - Smaller value
* @param greater_value - Greater value
* @return the randomizede value
*/
int randomize(int smaller_value, int greater_value);

/**
* @brief Function that save in temporary char array the string matching the gate code
* @param gate - the gate code
* @param temp - temporary array to save the string
* @return none
*/
void get_gate_string(int gate, char *temp);

/**
* @brief Function that get a part from boolean expression and return the bdd referent to the input
* @param str - the string containing the boolean function broken on ANDS
* @param table - table struct
* @return the bdd of the boolean expression
*/
bdd get_from_token(const char *str, Table *table);

/**
* @brief Function that break the boolean function in the ANDS and calls get_from_token to get the bdd of the input
* @param str - the string containing the boolean function
* @param table - table struct
* @return the bdd of the multiplications in boolean expression
*/
bdd analyze_mult(const char *str, Table *table);

/**
* @brief Function that break the boolean function in the ORS and calls analyze_mult to make bdd
* @param str - the string containing the boolean function
* @param table - table struct
* @return the bdd of the boolean expression
*/
bdd analyze_sum(const char *str, Table *table);

/**
* @brief Constructor
* @param table - table struct
* @param filename - the name of the file containing the truth table
* @return none
*/
void table_constructor(Table* table, const char* filename);

/**
* @brief Randomize genes that couldn't be used when sowing the individual with a factible solution, 
* this process grant that 33,3% of genotype is randomly selected to be inactive in the 
* initial invidual.   
* @param individual - the individual struct
* @param num_gates - the number of gates used in the evolutionary process
* @param num_inputs_table - number of inputs in truth table
* @return none
*/
void set_genes_not_used(Individual *individual, int num_gates, int num_inputs_table);

/**
* @brief Set the initial nodes of genotype as the inputs complemented.   
* @param individual - the individual struct
* @param num_inputs_table - number of inputs in truth table
* @return none
*/
void set_nots_on_individual(Individual *individual, int num_inputs_table);

/**
* @brief Parse the string and get the number of inputs and if its complemented.  
* @param str - the string being parsed
* @param num_inputs_table - number of inputs in truth table
* @return the number of the input on string
*/
int get_input_position(const char *str, int num_inputs_table);

/**
* @brief Traverse the genotype searching the first inactive node available.  
* @param individual - the individual struct
* @param num_inputs_table - number of inputs in truth table
* @return the position into the genotype of the first inactive node foundS
*/
int get_next_free_position(Individual *individual, int num_inputs_table);

/**
* @brief Sow the initial invidual based on the string that contains only AND operations.  
* @param individual - the individual struct
* @param str - the string with part of the circuits expression
* @param num_inputs_table - number of inputs in truth table
* @return the position into the genotype that the last AND gate is set.
*/
int parse_expression_only_ands(Individual *individual, const char *str, int num_inputs_table);

/**
* @brief Sow the initial invidual based on the string that contains only OR operations.  
* @param individual - the individual struct
* @param str - the string with part of the circuits expression
* @param num_inputs_table - number of inputs in truth table
* @return the position into the genotype that the last OR gate is set.
*/
int parse_expression_only_ors(Individual *individual, const char *str, int num_inputs_table);

/**
* @brief Sow the initial invidual based on the string that contains AND & OR operations.  
* @param individual - the individual struct
* @param str - the string with part of the circuits expression
* @param num_inputs_table - number of inputs in truth table
* @return the position into the genotype that the last AND/OR gate is set.
*/
int parse_expression_ands_ors(Individual *individual, const char *str, int num_inputs_table);

/**
* @brief Sow the initial invidual with a factible solution 
* based on the string that contains the circuits boolean expression. 
* @param individual - the individual struct
* @param table - the table struct that will store the circuit 
* boolean expression for each output
* @param filename - the name of the input file
* @param num_gates - the number of gates used in the evolutionary process
* @return the position into the genotype that the last AND/OR gate is set.
*/
void sow_population(Individual *individual, Table *table, const char *filename, int num_gates);

/**
* @brief Constructor
* @param gene - gene struct
* @return none
*/
void gene_constructor(Gene *gene);

/**
* @brief Function to calculate the position of the gene in vector based on CGP representation
* @param row - the row in the 2d array
* @param col - the column in the 2d array
* @return none
*/
int get_gene_position(int row, int col);

/**
* @brief Function that calculates the row of some gene based on its position
* @param pos - gene's position in genotype
* @param num_inputs_table - the number of inputs in truth table
* @return the row of the gene in genotype
*/
int get_gene_row(int pos, int num_inputs_table);

/**
* @brief Function that calculates the column of some gene based on its position
* @param pos - gene's position in genotype
* @param num_inputs_table - the number of inputs in truth table
* @return the column of the gene in genotype
*/
int get_gene_col(int pos, int num_inputs_table);

/**
* @brief Function that copies every data from an gene to another
* @param dest - the destination gene
* @param src - the source gene
* @return none
*/
void copy_gene_data(Gene *dest, Gene *src);

/**
* @brief Function that prints gene's data
* @param gene - gene struct
* @param num_gene - number of the gene in CGP representation
* @return none
*/
void print_gene(Gene *gene, int num_gene);

/**
* @brief Constructor
* @param individual - the individual struct
* @param num_outputs_table - the number of outputs in truth table
* @return none
*/
void individual_constructor(Individual *individual, int num_outputs_table);

/**
* @brief Function that prints individual's data
* @param individual - the individual struct
* @param num_inputs_table - number of inputs in truth table
* @return none
*/
void print_individual(Individual *individual, int num_inputs_table);

/**
* @brief Function that randomizes gene inputs when levels-back is lower than gene position
* @param individual - the individual struct
* @param row - gene row in the genotype
* @param col - gene column in the genotype
* @param num_input - the gene input being randomized
* @param num_inputs_table - the number of inputs in truth table
* @return none
*/
void randomize_inputs_low_lb(Individual *individual, int row, int col, int num_input, int num_inputs_table);

/**
* @brief Function that randomizes gene inputs
* @param individual - the individual struct
* @param row - gene row in the genotype
* @param col - gene column in the genotype
* @param num_input - the gene input being randomized
* @param num_inputs_table - the number of inputs in truth table
* @return none
*/
void randomize_inputs(Individual *individual, int row, int col, int num_input, int num_inputs_table);

/**
* @brief Function that randomly initializes the individual
* @param individual - the individual struct
* @param gates - the array containing the gates codes that will be used
* @param num_inputs_table - the number of inputs in truth table
* @param num_outputs_table - the number of outputs in truth table
* @return none
*/
void initialize_individual(Individual *individual, int *gates, int num_inputs_table, int num_ouputs_table);

/**
* @brief Function that randomly mutates some gene
* @param individual - the individual struct
* @param gates - the array containing the gates codes that will be used
* @param gene_pos - the selected gene to be mutated
* @param num_inputs_table - the number of inputs in truth table
* @return none
*/
void mutate_gene(Individual *individual, int *gates, int gene_pos, int num_inputs_table);

/**
* @brief Function that mutates an individual output
* @param individual - the individual struct
* @param gene_pos - the selected gene to be mutated
* @param num_inputs_table - the number of inputs in truth table
* @return none
*/
void mutate_output(Individual *individual, int gene_pos, int num_inputs_table);

/**
* @brief Function that select the right type of mutation (regular gene mutation or output gene mutation)
* @param individual - the individual struct
* @param gates - the array containing the gates codes that will be used
* @param num_inputs_table - the number of inputs in truth table
* @param sorted_gene - the selected gene to be mutated
* @return none
*/
void mutate_individual(Individual *individual, int *gates, int num_inputs_table, int sorted_gene);

/**
* @brief Function who apply point mutation in the individual - the number of mutations is defined in MUTATIONRATE
* @param individual - the individual struct
* @param gates - the array containing the gates codes that will be used
* @param num_inputs_table - the number of inputs in truth table
* @return none
*/
void point_mutation(Individual *individual, int *gates, int num_inputs_table);

/**
* @brief Function to checks if there are another subgraph with the same score from worst - 
* if there is, it randomly choose between them
* @param individual - the individual struct
* @param worst - output corresponding to the worst subgraph
* @return none
*/
int validate_worst(Individual *individual, int worst);

/**
 * is more than one individual with same output score
* @brief Function to checks if there is more than one individual with same (worst) output score - 
* if there is, it randomly choose between them
* @param individual - the individual struct
* @param worst - output corresponding to the worst subgraph
* @return none
*/
int find_worst_subgraph(Individual *individual);

/**
* @brief Function to apply Single Active Mutation at the individual
* @param individual - the individual struct
* @param gates - the array containing the gates codes that will be used
* @param num_inputs_table - the number of inputs in truth table
* @return none
*/
void sam(Individual *individual, int *gates, int num_inputs_table);

/**
* @brief Function to apply Guided Active Mutation at the individual
* @param individual - the individual struct
* @param gates - the array containing the gates codes that will be used
* @param worst_subgraph - the output with the worst subgraph
* @param num_inputs_table - the number of inputs in truth table
* @return none
*/
void gam(Individual *individual, int *gates, int worst_subgraph, int num_inputs_table);

/**
* @brief Function to traverse throught a single individual's output and set which gene is active 
* @param individual - the individual struct
* @param output - the output gene
* @param num_inputs_table - the number of inputs in truth table
* @return none
*/
void finds_active_genes_for_each_output(Individual *individual, int output, int num_inputs_table);

/**
* @brief Function to traverse throught all individual's output and set which gene is active 
* @param individual - the individual struct
* @param num_inputs_table - the number of inputs in truth table
* @return none
*/
void finds_individual_active_genes(Individual *individual, int num_inputs_table);

/**
* @brief Function that sets all genes as inactive
* @param individual - the individual struct
* @return none
*/
void clear_individiual_active_genes(Individual *individual);

/**
* @brief Function to check if there is more than one individual with same output score - if there is, 
* it randomly choose between them
* @param population - the individual struct with all individuals
* @param best - the best individual
* @param output - the individual's output being checked
* @return the best individual
*/
int validate_best_individual(Individual *population, int best, int output);

/**
* @brief Function to find for each output the individual with greater score
* @param population - the individual struct with all individuals
* @param individual_per_output - array to save the individual with greater score for each output
* @return none
*/
void find_best_individual_per_output(Individual *population, int *individual_per_output);

/**
* @brief Function to check if there is more than one individual with the amount of greater score per output - if there is,
* it randomly choose between them
* @param temp - array containing the number of outputs where the individual has the greater score
* @param best - the best individual
* @return the best individual
*/
int validate_best_one(int *temp, int best);

/**
* @brief Function to find the individual who has more greater score per output
* @param individual_per_output - array containing best individual for each output
* @param size - individual_per_output array size
* @return the best individual
*/
int find_best_one(int *individual_per_output, int size);

/**
* @brief Function to copy the subgraph from source to destination, respecting the active genes in destination 
* @param dest - the destination individual
* @param src - the source individual
* @param num_inputs_table - the number of inputs in truth table
* @param output - the output gene
* @return the new position of the gene in destination individual
*/
int copy_subgraph(Individual *dest, Individual *src, int *buffer, int num_inputs_table, int output);

/**
* @brief Function to print the boolean expression for a single individual's output
* @param individual - the individual struct
* @param output - the output gene
* @param num_inputs_table - the number of inputs in truth table
* @return none
*/
void print_boolean_expression_for_each_output(Individual *individual, int output, int num_inputs_table);

/**
* @brief Function to print the boolean expression for all individual's output 
* @param individual - the individual struct
* @param num_inputs_table - the number of inputs in truth table
* @return none
*/
void print_boolean_expression(Individual *individual, int num_inputs_table);

/**
* @brief Function to trasverse to a single individual's subgraph and get its maximum depth
* @param individual - the individual struct
* @param output - the output gene
* @param num_inputs_table - the number of inputs in truth table
* @return the max depth found
*/
int get_max_depth_for_each_output(Individual *individual, int output, int num_inputs_table);

/**
* @brief Function to trasverse to all individual's subgraph and prints the maximum depth
* @param individual - the individual struct
* @param num_inputs_table - the number of inputs in truth table
* @return the max depth found
*/
void print_max_depth(Individual *individual, int num_inputs_table);

/**
* @brief Function to get the number of transistors for an logical gate
* @param gate - the code to the corresponding to the logical gate
* @return the number of transistors for that gate
*/
int get_num_transistors(int gate);

/**
* @brief Function to traverse through all active gene and count the total number of resistors
* @param individual - the individual struct
* @return none
*/
void count_num_transistors_individual(Individual *individual);

/**
* @brief Function to print the total number of gates and the number of each gate from an individual
* @param gates_count - array containing the number which each gate appears
* @return none
*/
void print_num_gates(Individual *individual);

/**
* @brief Function that gather and print all necessary datas
* @param individual - the individual struct
* @param num_inputs_table - the number of inputs in truth table
* @return none
*/
void print_post_optimization_data(Individual *individual, int num_inputs_table);

/**
* @brief Function that copies every data from an individual to another
* @param dest - the destination individual
* @param src - the source individual
* @return none
*/
void copy_individual_data(Individual *dest, Individual *src);

/**
* @brief Function that calculates gene's bdd output based on inputs and the gate
* @param input0 - the first bdd input
* @param gate - the code to the corresponding to the logical gate
* @param input1 - the second bdd input
* @return the bdd output
*/
bdd get_bdd_output(bdd input0, int gate, bdd input1);

/**
* @brief Function that calculates bdd output for a single individual's output
* @param individual - individual struct
* @param table - table struct
* @param output - the output gene
* @return the  bdd gene output
*/
bdd get_individual_bdd_for_each_output(Individual *individual, Table *table, int output);

/**
* @brief Function that traverse through all active genes cleaning the bdd_output ref
* @param individual - individual struct
* @param table - table struct
* @param output - the output gene
* @return none
*/
void clear_bddref(Individual *individual, Table *table, int output);

/**
* @brief Function that calculates individual score based on the sat count 
    between truth table bdd output and individual's output connected by an
    xor logical gate
* @param individual - individual struct
* @param table - table struct
* @return none
*/
void calculate_individual_sat_count(Individual *individual, Table *table);

/**
* @brief Function to randomly initialize all indivividuals in an population
* @param individual - individual struct
* @param gates - the array containing the gates codes that will be used
* @param num_inputs_table - number of inputs in truth table
* @param num_output_table - number of outputs in truth table
* @return none
*/
void initialize_population(Individual *population, int *gates, int num_inputs_table, int num_outputs_table);

/**
* @brief Function to set individual's genes to inactive for all individuals
* @param population - individual array struct
* @return none
*/
void clear_population_active_genes(Individual *population);

/**
* @brief Function to apply POINT MUTATION in all childs
* @param population - individual array struct
* @param gates - the array containing the gates codes that will be used
* @param num_inputs_table - the number of inputs in truth table
* @return none
*/
void apply_PM(Individual *population, int *gates, int num_inputs_table);

/**
* @brief Function to apply SINGLE ACTIVE MUTATION in all childs
* @param population - individual array struct
* @param gates - the array containing the gates codes that will be used
* @param num_inputs_table - the number of inputs in truth table
* @return none
*/
void apply_SAM(Individual *population, int *gates, int num_inputs_table);

/**
* @brief Function to apply SINGLE ACTIVE MUTATION in the first 2 individuals (excluding the parent) and
    apply G ACTIVE MUTATION in the others 2
* @param population - individual array struct
* @param gates - the array containing the gates codes that will be used
* @param num_inputs_table - the number of inputs in truth table
* @return none
*/
void apply_SAM_plus_GAM(Individual *population, int *gates, int num_inputs_table);

/**
* @brief Function to evaluate all individuals's score, using the bdd with sat count method
* @param population - individual array struct
* @param table - table struct
* @return none
*/
void evaluate_population_sat_count(Individual *population, Table *table);

/**
* @brief Function to evaluate parent's score, using the bdd with sat count method
* @param population - individual array struct
* @param table - table struct
* @return none
*/
void evaluate_parent_sat_count(Individual *population, Table *table);

/**
* @brief Function to traverse through all individual's output setting the active genes
* @param population - individual array struct
* @param num_inputs_table - the number of inputs in truth table
* @return none
*/
void find_population_active_genes(Individual *population, int num_inputs_table);

/**
* @brief Function to find the invidual with lower sat count score
* @param population - individual array struct
* @return the individual with the lower sat count score
*/
int find_best_individual_sat_count(Individual *population);

/**
* @brief Function to implement the crossover method proposed in the paper referenced as "dasilva2018" in this file header
* @param population - individual array struct
* @param num_inputs_table - the number of inputs in truth table
* @return the individual with higher score
*/
int crossover(Individual* population, int num_inputs_table);

/**
* @brief Function to find the invidual with the higher score and the lower number of transistors
* @param population - individual array struct
* @return the individual with the higher score and the lower number of transistors
*/
int find_optimized_individual(Individual *population);

/**
* @brief Function to copy the best individual to the parent position
* @param population - individual array struct
* @param best_individual - the number of the best individual
* @return none
*/
void set_parent(Individual* population, int best_individual);

/**
* @brief Function to copy the parent to all others individuals
* @param population - individual array struct
* @return none
*/
void clone_parent(Individual* population);

/**
* @brief Function to print all population's data
* @param population - individual array struct
* @param num_inputs_table - the number of inputs in truth table
* @return none
*/
void print_population(Individual *population, int num_inputs_table);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////// IMPLEMENTATION ///////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int randomize(int smaller_value, int greater_value)
{
    return (int)(smaller_value + rand() % (greater_value - smaller_value));
}

void get_gate_string(int gate, char *temp)
{
    switch (gate)
    {
    case 1:
        strncpy(temp, "AND", 6);
        break;
    case 2:
        strncpy(temp, "OR", 6);
        break;
    case 3:
        strncpy(temp, "NOT", 6);
        break;
    case 4:
        strncpy(temp, "NAND", 6);
        break;
    case 5:
        strncpy(temp, "NOR", 6);
        break;
    case 6:
        strncpy(temp, "XOR", 6);
        break;
    case 7:
        strncpy(temp, "XNOR", 6);
        break;
    default:
        fprintf(out_file, "Gate number not possible!\n");
        exit(1);
        break;
    }
}

bdd get_from_token(const char *str, Table *table)
{
    int pos = -1;
    if (strstr(str, "~") != NULL)
    {
        sscanf(str, "~i%d", &pos);
        //printf("%s  %d\n", str, pos);
        return bdd_not(table->input_vars[pos]);
    }
    else
    {
        sscanf(str, "i%d", &pos);
        //printf("%s  %d\n", str, pos);
        return table->input_vars[pos];
    }
}

bdd analyze_mult(const char *str, Table *table)
{
    char *token;
    char *temp;
    char *saveptr;
    int counter = 0;
    bdd aux = -1;

    for (temp = (char *)str;; temp = NULL)
    {
        token = strtok_r(temp, "*", &saveptr);
        if (token == NULL)
            break;
        if (counter == 0)
        {
            aux = get_from_token(token, table);
            counter += 1;
        }
        else
        {
            aux = bdd_addref(bdd_apply(aux, get_from_token(token, table), bddop_and));
        }
    }
    return aux;
}

bdd analyze_sum(const char *str, Table *table)
{
    char *token;
    char *temp;
    char *saveptr;
    int counter = 0;
    bdd aux = -1;

    for (temp = (char *)str;; temp = NULL)
    {
        token = strtok_r(temp, "+", &saveptr);
        if (token == NULL)
            break;
        if (counter == 0)
        {
            aux = analyze_mult(token, table);
            counter += 1;
        }
        else
        {
            aux = bdd_addref(bdd_apply(aux, analyze_mult(token, table), bddop_or));
        }
    }
    return aux;
}

void table_constructor(Table *table, const char *filename)
{
    size_t fname_size = sizeof(filename);
    strncpy(table->circuit_name, filename, fname_size);

    FILE *file = fopen(filename, "r");
    char buffer[100000];

    if(file == NULL)
    {
        printf("Couldn't open the file %s!\n", filename);
        exit(1);
    }
    else
    {
        for(int i = 0; i < 3; i++)
        {
            
            if(fgets(buffer, 100000, file)){};
            fprintf(out_file,"%s", buffer);
            if(strstr(buffer, ".p") != 0)
            {
                sscanf(buffer, ".p %d\n", &table->num_rows);
            }
            else if(strstr(buffer, ".i") != 0)
            {
                sscanf(buffer, ".i %d\n", &table->num_inputs);
            }
            if(strstr(buffer, ".o") != 0)
            {
                sscanf(buffer, ".o %d\n", &table->num_outputs);
                table->num_cols = table->num_inputs + table->num_outputs;
            }
        }
    }

    //table->data = (int *)malloc(sizeof(int) * table->num_rows * table->num_cols);

    bdd_setvarnum(table->num_inputs);
    table->input_vars = (bdd *)malloc(sizeof(bdd) * table->num_inputs);
    for (int i = 0; i < table->num_inputs; i++)
    {
        table->input_vars[i] = bdd_ithvar(i);
    }

    table->outputs_bdd = (bdd *)malloc(sizeof(bdd) * table->num_outputs);
    for (int i = 0; i < table->num_outputs; i++)
    {

        if(fgets(buffer, 100000, file))
        {
            buffer[strlen(buffer) - 1] = '\0'; 
            fprintf(out_file, "%s\n", buffer);
            table->outputs_bdd[i] = analyze_sum(buffer, table);
        }
    }
    fflush(out_file);
    //exit(1);
    /*
    for (int i = 0; i < table->num_outputs; i++)
    {
        printf("%d ", find_less_common(table, i));
        table->outputs_bdd[i] = make_bdd_for_each_output(table, find_less_common(table, i), i);
    }    
    printf("\n");

    free(table->data);
    */
}

void set_genes_not_used(Individual *individual, int num_gates, int num_inputs_table)
{
    int genes_not_used = individual->genotype_size - num_gates;
    if(genes_not_used < 0)
    {
        fprintf(out_file, "Number of columns is lower than needed!\n");
        exit(1);
    }
    int counter = 0;


    while(1)
    {
        int temp = randomize(num_inputs_table, individual->genotype_size);

        if(individual->genotype[temp].active == 0)
        {
            individual->genotype[temp].active = 1;
            counter += 1;
        }

        if(counter == genes_not_used)
        {
            break;
        }
    }
}

void set_nots_on_individual(Individual *individual, int num_inputs_table)
{
    for(int i = 0; i < num_inputs_table; i++)
    {
        individual->genotype[i].gate = 3;
        individual->genotype[i].input[0] = i;
        individual->genotype[i].active = 1;
    }
}

int get_input_position(const char *str, int num_inputs_table)
{
    int pos = 0;
    if(strstr(str, "~") != NULL)
    {
        sscanf(str, "~i%d", &pos);
        return pos + num_inputs_table;
    }
    else
    {
        sscanf(str, "i%d", &pos);
        return pos;
    }
}

int get_next_free_position(Individual *individual, int num_inputs_table)
{
    for(int i = num_inputs_table; i < individual->genotype_size; i++)
    {
        if(individual->genotype[i].active == 0)
        {
            return i;
        }
    }

    fprintf(out_file, "Couldn't find any free space on genotype!\n");
    exit(1);

    return 0;
}

int parse_expression_only_ands(Individual *individual, const char *str,  int num_inputs_table)
{
    char *token;
    char *temp;
    char *saveptr;
    int flag = 0;
    int input0 = 0;
    int input1 = 0;
    int pos = 0;

    if(strstr(str, "*") == NULL)
        return get_input_position(str, num_inputs_table);

    for (temp = (char *)str;; temp = NULL)
    {
        token = strtok_r(temp, "*", &saveptr);
        if (token == NULL)
            break;
        if (flag == 0)
        {
            input0 = get_input_position(token, num_inputs_table);
            flag = 1;
        }
        else if(flag == 1)
        {
            input1 = get_input_position(token, num_inputs_table);
            pos = get_next_free_position(individual, num_inputs_table);

            individual->genotype[pos].gate = 1;
            individual->genotype[pos].input[0] = input0;
            individual->genotype[pos].input[1] = input1;
            individual->genotype[pos].active = 1;

            input1 = pos + num_inputs_table;
            flag = 2;
        }
        else if(flag == 2)
        {
            input0 = get_input_position(token, num_inputs_table);
            pos = get_next_free_position(individual, num_inputs_table);

            individual->genotype[pos].gate = 1;
            individual->genotype[pos].input[0] = input0;
            individual->genotype[pos].input[1] = input1;
            individual->genotype[pos].active = 1;

            input1 = pos + num_inputs_table;
        }       
    }
    return pos + num_inputs_table;
}

int parse_expression_only_ors(Individual *individual, const char *str, int num_inputs_table)
{
    char *token;
    char *temp;
    char *saveptr;
    int flag = 0;
    int input0 = 0;
    int input1 = 0;
    int pos = 0;

    if (strstr(str, "+") == NULL)
        return get_input_position(str, num_inputs_table);

    for (temp = (char *)str;; temp = NULL)
    {
        token = strtok_r(temp, "+", &saveptr);
        if (token == NULL)
            break;
        if (flag == 0)
        {
            input0 = get_input_position(token, num_inputs_table);
            flag = 1;
        }
        else if (flag == 1)
        {
            input1 = get_input_position(token, num_inputs_table);
            pos = get_next_free_position(individual, num_inputs_table);

            individual->genotype[pos].gate = 2;
            individual->genotype[pos].input[0] = input0;
            individual->genotype[pos].input[1] = input1;
            individual->genotype[pos].active = 1;

            input1 = pos + num_inputs_table;
            flag = 2;
        }
        else if (flag == 2)
        {
            input0 = get_input_position(token, num_inputs_table);
            pos = get_next_free_position(individual, num_inputs_table);

            individual->genotype[pos].gate = 2;
            individual->genotype[pos].input[0] = input0;
            individual->genotype[pos].input[1] = input1;
            individual->genotype[pos].active = 1;

            input1 = pos + num_inputs_table;
        }
    }
    return pos + num_inputs_table;
}

int parse_expression_ands_ors(Individual *individual, const char *str, int num_inputs_table)
{
    char *token;
    char *temp;
    char *saveptr;
    int flag = 0;
    int input0 = 0;
    int input1 = 0;
    int pos = 0;

    for (temp = (char *)str;; temp = NULL)
    {
        token = strtok_r(temp, "+", &saveptr);
        if (token == NULL)
            break;
        if (flag == 0)
        {
            input0 = parse_expression_only_ands(individual, token, num_inputs_table);
            flag = 1;
        }
        else if (flag == 1)
        {
            input1 = parse_expression_only_ands(individual, token, num_inputs_table);
            pos = get_next_free_position(individual, num_inputs_table);

            individual->genotype[pos].gate = 2;
            individual->genotype[pos].input[0] = input0;
            individual->genotype[pos].input[1] = input1;
            individual->genotype[pos].active = 1;

            input1 = pos + num_inputs_table;
            flag = 2;
        }
        else if (flag == 2)
        {
            input0 = parse_expression_only_ands(individual, token, num_inputs_table);
            pos = get_next_free_position(individual, num_inputs_table);

            individual->genotype[pos].gate = 2;
            individual->genotype[pos].input[0] = input0;
            individual->genotype[pos].input[1] = input1;
            individual->genotype[pos].active = 1;

            input1 = pos + num_inputs_table;
        }
    }
    return pos + num_inputs_table;
}

void sow_population(Individual *individual, Table *table, const char *filename, int num_gates)
{
    FILE *file = fopen(filename, "r");
    char buffer[100000];

    set_genes_not_used(individual, num_gates, table->num_inputs);
    set_nots_on_individual(individual, table->num_inputs);

    if (file == NULL)
    {
        printf("Couldn't open the file %s!\n", filename);
        exit(1);
    }
    else
    {   
        for(int i = 0; i < 3; i++)
        {
            if(fgets(buffer, 100000, file)){};
        }
        for (int i = 0; i < table->num_outputs; i++)
        {

            if (fgets(buffer, 100000, file))
            {
                buffer[strlen(buffer) - 1] = '\0';
            }
            else
            {
                fprintf(out_file, "Couldn't read expression!\n");
                exit(1);
            }
            

            if (strstr(buffer, "*") == NULL && strstr(buffer, "+") == NULL)
            {
                individual->output[i] = get_input_position(buffer, table->num_inputs);
            }
            else if(strstr(buffer, "+") == NULL)
            {
                individual->output[i] = parse_expression_only_ands(individual, buffer, table->num_inputs);
            }
            else if(strstr(buffer, "*") == NULL)
            {
                individual->output[i] = parse_expression_only_ors(individual, buffer, table->num_inputs);
            }
            else if (strstr(buffer, "*") != NULL && strstr(buffer, "+") != NULL)
            {
                individual->output[i] = parse_expression_ands_ors(individual, buffer, table->num_inputs);
            }
        }
    }
    
}

void gene_constructor(Gene *gene)
{
    gene->input[0] = -1;
    gene->input[1] = -1;
    gene->gate = 0;
    gene->active = 0;
}

int get_gene_position(int row, int col)
{
    return row * NCOL + col;
}

int get_gene_row(int pos, int num_inputs_table)
{
    if (pos < num_inputs_table)
    {
        fprintf(out_file, "Couldn't get gene row!\n");
        exit(1);
    }
    return (pos - num_inputs_table) % (NROW);
}

int get_gene_col(int pos, int num_inputs_table)
{
    if (pos < num_inputs_table)
    {
        fprintf(out_file, "Couldn't get gene col!\n");
        exit(1);
    }
    return (pos - num_inputs_table) / (NROW);
}

void copy_gene_data(Gene *dest, Gene *src)
{
    dest->gate = src->gate;
    dest->input[0] = src->input[0];
    dest->input[1] = src->input[1];
    dest->active = src->active;
}

void print_gene(Gene *gene, int num_gene)
{
    char temp[6];
    get_gate_string(gene->gate, temp);
    fprintf(out_file, "Gene number: %d\t%d %s %d ACTIVE:%d\n", num_gene, gene->input[0], temp, gene->input[1], gene->active);
}

void individual_constructor(Individual *individual, int num_outputs_table)
{
    individual->score = 0;
    individual->num_transistors = 0;

    individual->genotype = (Gene*)malloc(sizeof(Gene) * NROW * NCOL);
    individual->genotype_size = NROW * NCOL;

    individual->output = (int *)malloc(sizeof(int) * num_outputs_table);
    individual->score_per_output = (long int *)malloc(sizeof(long int) * num_outputs_table);
    individual->output_size = num_outputs_table;

    for (int i = 0; i < individual->genotype_size; i++)
    {
        gene_constructor(&individual->genotype[i]);
    }
}

void print_individual(Individual *individual, int num_inputs_table)
{
    fprintf(out_file, "INDIVIDUAL OUTPUTS: ");
    for (int i = 0; i < individual->output_size; i++)
    {
        fprintf(out_file, "%d ", individual->output[i]);
    }
    fprintf(out_file, "\n");

    fprintf(out_file, "SCORE PER OUTPUT: ");
    for (int i = 0; i < individual->output_size; i++)
    {
        fprintf(out_file, "%ld ", individual->score_per_output[i]);
    }
    fprintf(out_file, "\n");

    for (int row = 0; row < NROW; row++)
    {
        for (int col = 0; col < NCOL; col++)
        {
            int temp = get_gene_position(row, col);
            print_gene(&individual->genotype[temp], temp + num_inputs_table);
        }
    }
    fflush(out_file);
}

void randomize_inputs_low_lb(Individual *individual, int row, int col, int num_input, int num_inputs_table)
{
    int temp = randomize(0, num_inputs_table + LB * NROW);
    int pos = get_gene_position(row, col);

    if (temp >= 0 && temp < num_inputs_table)
    {
        individual->genotype[pos].input[num_input] = temp;
    }
    else
    {
        individual->genotype[pos].input[num_input] = randomize(
            num_inputs_table + (col - LB) * NROW,
            num_inputs_table + (NROW * col));
    }
}

void randomize_inputs(Individual *individual, int row, int col, int num_input, int num_inputs_table)
{
    int pos = 0;
    pos = get_gene_position(row, col);

    if (col == 0 || LB == 0) ///aleatoriza uma entrada
    {
        individual->genotype[pos].input[num_input] = randomize(0, num_inputs_table);
    }
    else if (LB >= col || LB == -1) ///aleatoriza qualquer coluna antes da entrada
    {
        individual->genotype[pos].input[num_input] = randomize(0, num_inputs_table + NROW * col);
    }
    else if (LB < col) ///aleatoriza coluna até o levels-back
    {
        randomize_inputs_low_lb(individual, row, col, num_input, num_inputs_table);
    }
}

void initialize_individual(Individual *individual, int *gates, int num_inputs_table, int num_ouputs_table)
{
    int temp = 0;
    int pos = 0;

    individual_constructor(individual, num_ouputs_table);

    for (int row = 0; row < NROW; row++) /// (For) utilizado para preencher os valores de cada nó do indivíduo
    {
        for (int col = 0; col < NCOL; col++)
        {
            randomize_inputs(individual, row, col, 0, num_inputs_table);
            randomize_inputs(individual, row, col, 1, num_inputs_table);
            temp = randomize(0, NGATES);
            pos = get_gene_position(row, col);
            individual->genotype[pos].gate = gates[temp];
        }
    }

    for (int i = 0; i < individual->output_size; i++)
    {
        temp = randomize(0, num_inputs_table + individual->genotype_size);
        individual->output[i] = temp;
        individual->score_per_output[i] = 0;
    }
}

void mutate_gene(Individual *individual, int *gates, int gene_pos, int num_inputs_table)
{
    int temp = randomize(0, 3);

    int row = get_gene_row(gene_pos, num_inputs_table);
    int col = get_gene_col(gene_pos, num_inputs_table);
    if (temp == 0 || temp == 1)
    {
        randomize_inputs(individual, row, col, temp, num_inputs_table);
    }
    else
    {
        int pos = get_gene_position(row, col);
        temp = randomize(0, NGATES);
        individual->genotype[pos].gate = gates[temp];
    }
}

void mutate_output(Individual *individual, int gene_pos, int num_inputs_table)
{
    int pos = gene_pos - individual->genotype_size - num_inputs_table;
    int temp = randomize(0, num_inputs_table + individual->genotype_size);
    individual->output[pos] = temp;
}

void mutate_individual(Individual *individual, int *gates, int num_inputs_table, int sorted_gene)
{

    if (sorted_gene < num_inputs_table + individual->genotype_size)
    {
        mutate_gene(individual, gates, sorted_gene, num_inputs_table);
    }
    else
    {
        mutate_output(individual, sorted_gene, num_inputs_table);
    }
}

void point_mutation(Individual *individual, int *gates, int num_inputs_table)
{
    for (int i = 0; i < (int)(MUTATIONRATE*NROW*NCOL); i++)
    {
        int temp = randomize(num_inputs_table, num_inputs_table + individual->genotype_size + individual->output_size);
        mutate_individual(individual, gates, num_inputs_table, temp);
    }
}

int validate_worst(Individual *individual, int worst)
{
    int counter = 0;
    int i;
    for (i = 0; i < individual->output_size; i++)
    {
        if (individual->score_per_output[i] == individual->score_per_output[worst])
        {
            counter += 1;
        }
    }

    if (counter == 1)
    {
        return worst;
    }
    else
    {
        int temp = randomize(1, counter + 1);
        counter = 0;
        for (i = 0; i < individual->output_size; i++)
        {
            if (individual->score_per_output[i] == individual->score_per_output[worst])
            {
                counter += 1;
            }
            if (counter == temp)
            {
                return i;
            }
        }
    }

    fprintf(out_file, "Erro validating worst subgraph!\n");
    exit(1);

    return 0;
}

int find_worst_subgraph(Individual *individual)
{
    int worst = 0;

    for (int i = 1; i < individual->output_size; i++)
    {
        if (individual->score_per_output[i] > individual->score_per_output[worst])
        {
            worst = i;
        }
    }
    return validate_worst(individual, worst);
}

void sam(Individual *individual, int *gates, int num_inputs_table)
{
    int count = 0;
    while (1)
    {
        int temp = randomize(num_inputs_table, num_inputs_table + individual->genotype_size + individual->output_size);
        int row = get_gene_row(temp, num_inputs_table);
        int col = get_gene_col(temp, num_inputs_table);
        int pos = get_gene_position(row, col);

        if (individual->genotype[pos].active == 1 || temp >= num_inputs_table + individual->genotype_size)
        {
            mutate_individual(individual, gates, num_inputs_table, temp);
            break;
        }

        mutate_individual(individual, gates, num_inputs_table, temp);
        count++;
    }
}

void gam(Individual *individual, int *gates, int worst_subgraph, int num_inputs_table)
{
    while (1)
    {
        int temp = randomize(num_inputs_table, num_inputs_table + individual->genotype_size + individual->output_size);
        int row = get_gene_row(temp, num_inputs_table);
        int col = get_gene_col(temp, num_inputs_table);
        int pos = get_gene_position(row, col);

        if (individual->genotype[pos].active == 1 || temp == num_inputs_table + individual->genotype_size + worst_subgraph)
        {
            mutate_individual(individual, gates, num_inputs_table, temp);
            break;
        }
    }
}

void finds_active_genes_for_each_output(Individual *individual, int output, int num_inputs_table)
{
    if (output >= num_inputs_table)
    {
        int row = get_gene_row(output, num_inputs_table);
        int col = get_gene_col(output, num_inputs_table);
        int pos = get_gene_position(row, col);

        finds_active_genes_for_each_output(individual, individual->genotype[pos].input[0], num_inputs_table);
        if(individual->genotype[pos].gate != 3)
            finds_active_genes_for_each_output(individual, individual->genotype[pos].input[1], num_inputs_table);

        individual->genotype[pos].active = 1;
    }
}

void finds_individual_active_genes(Individual *individual, int num_inputs_table)
{
    for (int i = 0; i < individual->output_size; i++)
    {
        finds_active_genes_for_each_output(individual, individual->output[i], num_inputs_table);
    }
}

void clear_individiual_active_genes(Individual *individual)
{
    for (int i = 0; i < individual->genotype_size; i++)
    {
        individual->genotype[i].active = 0;
    }
}

int validate_best_individual(Individual *population, int best, int output)
{
    int counter = 0;

    for (int i = 0; i < NPOP; i++)
    {
        if (population[i].score_per_output[output] == population[best].score_per_output[output])
        {
            counter += 1;
        }
    }

    if (counter == 1)
    {
        return best;
    }

    int temp = randomize(1, counter + 1);
    counter = 0;
    for (int i = 0; i < NPOP; i++)
    {
        if (population[i].score_per_output[output] == population[best].score_per_output[output])
        {
            counter += 1;
        }
        if (counter == temp)
        {
            return i;
        }
    }

    fprintf(out_file, "Error validating best individual!\n");
    exit(1);

    return 0;
}

void find_best_individual_per_output(Individual *population, int *individual_per_output)
{
    for (int i = 0; i < population[0].output_size; i++)
    {
        int best = 0;
        for (int j = 1; j < NPOP; j++)
        {
            if (population[j].score_per_output[i] < population[best].score_per_output[i])
            {
                best = j;
            }
        }
        individual_per_output[i] = validate_best_individual(population, best, i);
        //printf("%d  ", individual_per_output[i]);
    }
    //printf("\n");
}

int validate_best_one(int *temp, int best)
{
    int counter = 0;
    for (int i = 0; i < NPOP; i++)
    {
        if (temp[i] == temp[best])
        {
            counter += 1;
        }
    }

    if (counter == 1)
    {
        return best;
    }

    int temp1 = randomize(1, counter + 1);
    counter = 0;
    for (int i = 0; i < NPOP; i++)
    {
        if (temp[i] == temp[best])
        {
            counter += 1;
        }
        if (counter == temp1)
        {
            return i;
        }
    }

    fprintf(out_file, "Error validating best one!\n");
    exit(1);

    return 0;
}

int find_best_one(int *individual_per_output, int size)
{
    int temp[NPOP];

    for (int i = 0; i < NPOP; i++)
    {
        temp[i] = 0;
    }

    for (int i = 0; i < size; i++)
    {
        temp[individual_per_output[i]] += 1;
    }

    int best = 0;
    for (int i = 1; i < NPOP; i++)
    {
        if (temp[i] >= temp[best])
        {
            best = i;
        }
    }

    return validate_best_one(temp, best);
}

int copy_subgraph(Individual *dest, Individual *src, int *buffer, int num_inputs_table, int output)
{
    if (output >= num_inputs_table)
    {
        int input0 = 0;
        int input1 = 0;

        int row = get_gene_row(output, num_inputs_table);
        int col = get_gene_col(output, num_inputs_table);
        int pos = get_gene_position(row, col);

        input0 = copy_subgraph(dest, src, buffer, num_inputs_table, src->genotype[pos].input[0]);
        if (src->genotype[pos].gate != 3)
            input1 = copy_subgraph(dest, src, buffer, num_inputs_table, src->genotype[pos].input[1]);

        if (buffer[pos] != -1)
            return buffer[pos];

        int i;
        for (i = pos; i < dest->genotype_size; i++)
        {
            if (dest->genotype[i].active == 0)
            {
                copy_gene_data(&dest->genotype[i], &src->genotype[pos]);
                dest->genotype[i].input[0] = input0;
                if (src->genotype[pos].gate != 3)
                    dest->genotype[i].input[1] = input1;
                buffer[pos] = i + num_inputs_table;
                dest->genotype[i].active = 1;
                //printf("DEST: %d SRC: %d\n", i + num_inputs_table, pos + num_inputs_table);
                //print_individual(dest, num_inputs_table);
                return i + num_inputs_table;
            }
        }

        return -1;
    }

    return output;
}

void print_boolean_expression_for_each_output(Individual *individual, int output, int num_inputs_table)
{
    if (output < num_inputs_table)
    {
        fprintf(out_file, "i%d", output);
    }
    else
    {
        int row = get_gene_row(output, num_inputs_table);
        int col = get_gene_col(output, num_inputs_table);
        int pos = get_gene_position(row, col);
        char temp[6];
        get_gate_string(individual->genotype[pos].gate, temp);

        if (individual->genotype[pos].gate == 3)
        {
            fprintf(out_file, "(");
            fprintf(out_file, "%s ", temp);
            print_boolean_expression_for_each_output(individual, individual->genotype[pos].input[0], num_inputs_table);
            fprintf(out_file, ")");
        }
        else
        {
            fprintf(out_file, "(");
            print_boolean_expression_for_each_output(individual, individual->genotype[pos].input[0], num_inputs_table);
            fprintf(out_file, " %s ", temp);
            print_boolean_expression_for_each_output(individual, individual->genotype[pos].input[1], num_inputs_table);
            fprintf(out_file, ")");
        }
    }
}

void print_boolean_expression(Individual *individual, int num_inputs_table)
{
    for (int i = 0; i < individual->output_size; i++)
    {
        print_boolean_expression_for_each_output(individual, individual->output[i], num_inputs_table);
        fprintf(out_file, "\n\n");
    }
}

int get_max_depth_for_each_output(Individual *individual, int output, int num_inputs_table)
{
    if(output < num_inputs_table)
        return 1;
    else
    {
        int row = get_gene_row(output, num_inputs_table);
        int col = get_gene_col(output, num_inputs_table);
        int pos = get_gene_position(row, col);
        int left_depth = 0, right_depth = 0;

        left_depth = get_max_depth_for_each_output(individual, individual->genotype[pos].input[0], num_inputs_table);
        if(individual->genotype[pos].gate != 3)
            right_depth = get_max_depth_for_each_output(individual, individual->genotype[pos].input[1], num_inputs_table);

        if (left_depth > right_depth)
            return left_depth + 1;
        else
            return right_depth + 1;
    }
}

void print_max_depth(Individual *individual, int num_inputs_table)
{
    int max_depth = get_max_depth_for_each_output(individual, individual->output[0], num_inputs_table);
    int temp;
    for (int i = 1; i < individual->output_size; i++)
    {
        temp = get_max_depth_for_each_output(individual, individual->output[i], num_inputs_table);
        if (temp > max_depth)
        {
            max_depth = temp;
        }
    }
    fprintf(out_file, "Circuit max depth: %d\n", max_depth);
}

int get_num_transistors(int gate)
{
    switch (gate)
    {
    case 1: //AND
        return 2;
        break;
    case 2: //OR
        return 2;
        break;
    case 3: //NOT
        return 1;
        break;
    case 4: //NAND
        return 2;
        break;
    case 5: //NOR
        return 1;
        break;
    case 6: //XOR
        return 3;
        break;
    case 7: //XNOR
        return 4;
        break;
    default:
        fprintf(out_file, "Gate code unknow!\n");
        exit(1);
        break;
    }
}

void count_num_transistors_individual(Individual *individual)
{
    int temp = 0;
    for(int j = 0; j < individual->genotype_size; j++)
    {
        if (individual->genotype[j].active == 1)
        {
            temp += get_num_transistors(individual->genotype[j].gate);
        }
    }
    individual->num_transistors = temp;
}

void print_num_gates(Individual *individual)
{
    int gates_count[7] = {0, 0, 0, 0, 0, 0, 0};
    char temp[6];
    int total = 0;

    for (int i = 0; i < individual->genotype_size; i++)
    {
        if (individual->genotype[i].active == 1)
        {
            int temp = individual->genotype[i].gate - 1;
            gates_count[temp] += 1;
        }
    }

    for (int i = 0; i < NGATES; i++)
    {
        get_gate_string(i + 1, temp);
        fprintf(out_file, "%s: %d\n", temp, gates_count[i]);
        total += gates_count[i];
    }
    fprintf(out_file, "TOTAL GATES: %d\n", total);
}

void print_post_optimization_data(Individual *individual, int num_inputs_table)
{
    clear_individiual_active_genes(individual);
    finds_individual_active_genes(individual, num_inputs_table);
    count_num_transistors_individual(individual);
    print_max_depth(individual, num_inputs_table);
    print_num_gates(individual);
    fprintf(out_file, "Num transistors: %d\n", individual->num_transistors);
    //print_boolean_expression(individual, num_inputs_table);
    fflush(out_file);
}

void copy_individual_data(Individual *dest, Individual *src)
{
    for (int i = 0; i < dest->genotype_size; i++)
    {
        copy_gene_data(&dest->genotype[i], &src->genotype[i]);
    }

    for (int i = 0; i < dest->output_size; i++)
    {
        dest->output[i] = src->output[i];
        dest->score_per_output[i] = src->score_per_output[i];
    }
    dest->score = src->score;
    dest->num_transistors = src->num_transistors;
}

bdd get_bdd_output(bdd input0, int gate, bdd input1)
{
    switch (gate)
    {
    case 1: //AND
        return bdd_addref(bdd_apply(input0, input1, bddop_and));
        break;
    case 2: //OR
        return bdd_addref(bdd_apply(input0, input1, bddop_or));
        break;
    case 3: //NOT
        return bdd_addref(bdd_not(input0));
        break;
    case 4: //NAND
        return bdd_addref(bdd_apply(input0, input1, bddop_nand));
        break;
    case 5: //NOR
        return bdd_addref(bdd_apply(input0, input1, bddop_nor));
        break;
    case 6: //XOR
        return bdd_addref(bdd_apply(input0, input1, bddop_xor));
        break;
    case 7: //XNOR
        return bdd_addref(bdd_not(bdd_apply(input0, input1, bddop_xor)));
        break;
    default:
        fprintf(out_file, "Gate code unknow!\n");
        exit(1);
        break;
    }
}

bdd get_individual_bdd_for_each_output(Individual *individual, Table *table, int output)
{
    if (output < table->num_inputs)
    {
        return table->input_vars[output];
    }
    else
    {
        int row = get_gene_row(output, table->num_inputs);
        int col = get_gene_col(output, table->num_inputs);
        int pos = get_gene_position(row, col);
        bdd input0;
        bdd input1 = 0;

        input0 = get_individual_bdd_for_each_output(individual, table, individual->genotype[pos].input[0]);
        if (individual->genotype[pos].gate != 3)
            input1 = get_individual_bdd_for_each_output(individual, table, individual->genotype[pos].input[1]);

        individual->genotype[pos].bdd_output = get_bdd_output(input0, individual->genotype[pos].gate, input1);

        return individual->genotype[pos].bdd_output;
    }
}

void clear_bddref(Individual *individual, Table *table, int output)
{
    if(output >= table->num_inputs)
    {
        int row = get_gene_row(output, table->num_inputs);
        int col = get_gene_col(output, table->num_inputs);
        int pos = get_gene_position(row, col);

        clear_bddref(individual, table, individual->genotype[pos].input[0]);
        if (individual->genotype[pos].gate != 3)
            clear_bddref(individual, table, individual->genotype[pos].input[1]);

        bdd_delref(individual->genotype[pos].bdd_output);
    }
}

void calculate_individual_sat_count(Individual *individual, Table *table)
{
    bdd temp;
    individual->score = 0;

    for (int i = 0; i < individual->output_size; i++)
    {
        temp = get_individual_bdd_for_each_output(individual, table, individual->output[i]);
        temp = bdd_addref(bdd_apply(temp, table->outputs_bdd[i], bddop_xor));
        individual->score_per_output[i] = bdd_satcount(temp);
        individual->score += individual->score_per_output[i];

        bdd_delref(temp);
        clear_bddref(individual, table, individual->output[i]);
    }
}

void initialize_population(Individual *population, int *gates, int num_inputs_table, int num_outputs_table)
{
    for (int i = 0; i < NPOP; i++)
    {
        initialize_individual(&population[i], gates, num_inputs_table, num_outputs_table);
    }
}

void clear_population_active_genes(Individual *population)
{
    for (int i = 0; i < NPOP; i++)
    {
        clear_individiual_active_genes(&population[i]);
    }
}

void apply_PM(Individual *population, int *gates, int num_inputs_table)
{
    for (int i = 1; i < NPOP; i++)
    {
        point_mutation(&population[i], gates, num_inputs_table);
    }
}

void apply_SAM(Individual *population, int *gates, int num_inputs_table)
{
    clear_population_active_genes(population);

    for(int i = 1; i < NPOP; i++)
    {
        clear_individiual_active_genes(&population[i]);
        finds_individual_active_genes(&population[i], num_inputs_table);
        sam(&population[i], gates, num_inputs_table);
    }
}

void apply_SAM_plus_GAM(Individual *population, int *gates, int num_inputs_table)
{
    int worst_subgraph = find_worst_subgraph(&population[0]);

    for (int i = 1; i < 3; i++)
    {
        clear_individiual_active_genes(&population[i]);
        finds_individual_active_genes(&population[i], num_inputs_table);
        sam(&population[i], gates, num_inputs_table);
    }

    for (int i = 3; i < NPOP; i++)
    {
        clear_individiual_active_genes(&population[i]);
        finds_active_genes_for_each_output(&population[i], population[i].output[worst_subgraph], num_inputs_table);
        gam(&population[i], gates, worst_subgraph, num_inputs_table);
    }
}

void evaluate_population_sat_count(Individual *population, Table *table)
{
    for (int i = 1; i < NPOP; i++)
    {
        calculate_individual_sat_count(&population[i], table);
    }
}

void evaluate_parent_sat_count(Individual *population, Table *table)
{
    calculate_individual_sat_count(&population[0], table);
}

void find_population_active_genes(Individual *population, int num_inputs_table)
{
    for (int i = 0; i < NPOP; i++)
    {
        finds_individual_active_genes(&population[i], num_inputs_table);
    }
}

int find_best_individual_sat_count(Individual *population)
{
    int best_indv = 0;
    for (int i = 1; i < NPOP; i++)
    {
        if (population[i].score <= population[best_indv].score)
        {
            best_indv = i;
        }
    }
    return best_indv;
}

int crossover(Individual *population, int num_inputs_table)
{
    int *best_individual_per_output = (int *)malloc(sizeof(int) * population[0].output_size);
    find_best_individual_per_output(population, best_individual_per_output);
    int best_one = find_best_one(best_individual_per_output, population[0].output_size);

    Individual *temp = (Individual *)malloc(sizeof(Individual));
    individual_constructor(temp, population[0].output_size);
    copy_individual_data(temp, &population[best_one]);

    int buffer[NPOP - 1][NROW * NCOL];
    for (int i = 0; i < NPOP - 1; i++)
    {
        for (int j = 0; j < NROW * NCOL; j++)
        {
            buffer[i][j] = -1;
        }
    }

    //printf("BEST ONE: %d\n", best_one);
    //find_population_active_genes(&population[best_one], num_inputs_table);
    //print_population(population, num_inputs_table);


    clear_population_active_genes(population);

    for (int i = 0; i < population[0].output_size; i++)
    {
        if (best_one == best_individual_per_output[i])
        {
            finds_active_genes_for_each_output(temp, temp->output[i], num_inputs_table);
        }
    }

    for (int i = 0; i < population[0].output_size; i++)
    {
        if (best_one != best_individual_per_output[i])
        {
            //printf("BEST INDV: %d OUTPUT: %d\n", best_individual_per_output[i], i);
            int temp_indv = best_individual_per_output[i];
            finds_active_genes_for_each_output(&population[temp_indv], population[temp_indv].output[i], num_inputs_table);

            if (temp_indv < best_one)
            {
                temp->output[i] = copy_subgraph(temp, &population[temp_indv], buffer[temp_indv], num_inputs_table, population[temp_indv].output[i]);
            }
            if (temp_indv > best_one)
            {
                temp->output[i] = copy_subgraph(temp, &population[temp_indv], buffer[temp_indv - 1], num_inputs_table, population[temp_indv].output[i]);
            }

            if (temp->output[i] == -1)
            {
                free(temp);
                return find_best_individual_sat_count(population);
            }

            temp->score_per_output[i] = population[temp_indv].score_per_output[i];
            clear_individiual_active_genes(&population[temp_indv]);
            //print_individual(&population[best_one], num_inputs_table);
        }
    }

    //print_individual(temp, num_inputs_table);
    //exit(1);

    temp->score = 0;
    for (int i = 0; i < temp->output_size; i++)
    {
        temp->score += temp->score_per_output[i];
    }

    clear_individiual_active_genes(temp);
    finds_individual_active_genes(temp, num_inputs_table);
    copy_individual_data(&population[best_one], temp);

    free(temp);
    free(best_individual_per_output);

    return best_one;
}

int find_optimized_individual(Individual *population)
{
    count_num_transistors_individual(&population[0]);

    int best_individual = 0;
    for (int i = 1; i < NPOP; i++)
    {
        if (population[i].score == population[best_individual].score)
        {
            count_num_transistors_individual(&population[i]);
            if(population[i].num_transistors <= population[best_individual].num_transistors)
            {
                best_individual = i;
            }
        }
    }
    return best_individual;
}

void set_parent(Individual *population, int best_individual)
{
    copy_individual_data(&population[0], &population[best_individual]);
}

void clone_parent(Individual *population)
{
    for (int i = 1; i < NPOP; i++)
    {
        copy_individual_data(&population[i], &population[0]);
        for (int k = 0; k < population[0].output_size; k++)
        {
            population[i].score_per_output[k] = 0;
        }
        population[i].score = 0;
    }
}

void print_population(Individual *population, int num_inputs_table)
{
    for (int i = 0; i < NPOP; i++)
    {
        print_individual(&population[i], num_inputs_table);
        fprintf(out_file, "\n\n");
    }
    fflush(out_file);
}
