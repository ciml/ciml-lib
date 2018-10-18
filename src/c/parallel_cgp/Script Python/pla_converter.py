import multiprocessing as mp
import numpy as np
import itertools
import re

def input_parser(input, inputs_name):

    expression = ""
    for index in range(len(input)):
        if input[index] is "1": #1 input uncomplemented
            expression += inputs_name[index] + "*"
        elif input[index] is "0": #0 input complemented
            expression += "~"+inputs_name[index] + "*"

    return expression[:-1]

def output_parser(output, final_expression, expression):

    for index in range(len(output)):
        if output[index] is "1": #term connected to output
            final_expression[index] += expression + "+"

def convert_instruction(final_expression, inputs_name, line):
    input = line.split( )[0]
    output = line.split( )[1]
    expression = input_parser(input, inputs_name)
    output_parser(output, final_expression, expression)

def generate_inputs_tt(input_numbers):
    return list(itertools.product([0, 1], repeat=input_numbers))

def canonicalize(expression):
    SUBSTITUICOES = {
        '~': ' not ',
        '+': ' or ',
        '*': ' and ',
    }
    return re.sub('|'.join(re.escape(sym) for sym in SUBSTITUICOES.keys()),
                  lambda sym: SUBSTITUICOES[sym.group(0)],
                  expression).strip()

def generate_outputs_tt(expressions, input_tt, inputs_name, position, output):
    
    line = []
    for item in expressions:
        expression = canonicalize(item)
        NO_GLOBALS = {'__builtins__': {}}

        locals = dict(zip(inputs_name, input_tt[position]))
        result = eval(expression, NO_GLOBALS, locals)
        line.append(result)
        
    line = [int(item) for item in line]
    output.put((position, line))

def main():

    output = mp.Queue()

    #open the file
    file = open("/home/lucasmullers/Documents/MCNC/Combinational/pla/All_PLAs/alu4.pla", "r")

    # read the number of inputs from file
    line = file.readline()
    if ".i" in line: #define the numbers of input
        input_numbers = int(line.split()[1])

    # read the number of outputs from file
    line = file.readline()
    if ".o" in line: #define the numbers of output
        output_numbers = int(line.split()[1])

    print("Inputs: " + str(input_numbers)+" Outputs: "+str(output_numbers))
    #list with all expressions
    final_expression = [""] * output_numbers

    inputs_name = []
    for i in range(input_numbers):
        inputs_name.append("_i_"+str(i))

    while True:
        line = file.readline()

        if ".ilb" in line: #ilb define the name of each input
            inputs_name =  line.split()[1:]
            print(inputs_name)
        elif ".ob" in line: #ob define the name of each output
            outputs_name = line.split()[1:]
            print(outputs_name)
        elif ".p" in line: #.p define the number of instructions
            pass
        elif ".e" in line:
            break
        else: #convert the instruction to some expression
            convert_instruction(final_expression, inputs_name, line)
    
    final_expression = [index[:-1] for index in final_expression]
    print(final_expression)
    print(str(input_numbers) + " " + str(output_numbers))
    
    input_tt = generate_inputs_tt(input_numbers)
    results = []

    for i in range(int((2 ** input_numbers) / 32)):
        #create 32 parallel process
        processes = [mp.Process(target=generate_outputs_tt, args=(final_expression, input_tt, inputs_name, x + i*32, output)) for x in range(32)]

        #start processes
        for p in processes:
            p.start()

        #finish the processes
        for p in processes:
            p.join()

        #save all the outputs
        results += [output.get() for p in processes]
    #sort the outputs in order
    results.sort()
    #remove the number that indicate the order
    output_tt = [x[1] for x in results]
    
    #print(output_tt)

    file = open("/home/lucasmullers/Desktop/alu4.tt","w")
    file.write(str(2**input_numbers) + " " + str(input_numbers) + " " + str(output_numbers) +"\n")
    for i in range(2** input_numbers):
        for j in range(input_numbers):
            file.write(str(input_tt[i][j]) + " ")
        for j in range(output_numbers):
            file.write(str(output_tt[i][j]))
        file.write("\n")

main()
