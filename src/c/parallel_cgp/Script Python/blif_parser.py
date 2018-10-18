from __future__ import print_function
from sys import argv
import itertools
import re

# /home/lucasmullers/Downloads/IC-BLIF/MCNC/Combinational/blif
# 5xp1.blif

# PORTAS USADAS EM CADA SAIDA
def inputs_used(line):
    position = []
    for l in range(0, len(line)):
        while True:
            length = len(line) - 1
            if line[length] == "\\":
                line.pop(length)
                line += file.readline().split()
            else:
                break
        if line[l] in inputs_name:
            position.append(chr(65 + inputs_name.index(line[l])))
    return position


# GERADOR EXPRESSAO BOOLEANA
def boolean_expression_generator(file_var):
    expression = ''
    expression_output = []
    full_expression = ''
    position = []
    string_len = 0

    while True:
        line = file_var.readline()
        if ".end" in line or ".exdc" in line:
            break
        if ".names" in line:
            if full_expression != '':
                full_expression = full_expression[:-1]
                expression_output.append(full_expression)
                full_expression = ''
            position = inputs_used(line.split())
            string_len = len(position)
        else:
            for n in range(string_len):
                if line[n] == '1':
                    expression += str(position[n]) + '*'
                elif line[n] == '0':
                    expression += '~' + str(position[n]) + '*'
                elif line[n] == ' ':
                    break
            expression = expression[:-1]
            full_expression += expression + '+'
            expression = ''
    expression_output.append(full_expression[:-1])
    return expression_output


# LEITURA INFORMACOES DO CABECALHO
def header_information(file_var):
    info = file_var.readline().split()
    info.pop(0)
    while True:
        length = len(info) - 1
        if info[length] == "\\":
            info.pop(length)
            info += file_var.readline().split()
        else:
            break
    return info

def canonicalize(expr):
    REPLACEMENTS = {
       '~': ' not ',
       '+': ' or ',
       '*': ' and ',
    }
    return re.sub('|'.join(re.escape(sym) for sym in REPLACEMENTS.keys()),
                  lambda sym: REPLACEMENTS[sym.group(0)],
                  expr).strip()

def extract_variables(expr):
    return sorted(set(re.findall(r'\b[A-Za-z]\b', expr)))

def truth_table_output(expr, num_inputs):
    expr = canonicalize(expr)
    #print(expr)
    vars = extract_variables(expr)
    NO_GLOBALS = {'__builtins__': {}}
    output_result = []

    for vals in itertools.product(range(2), repeat=num_inputs):
        locals = dict(zip(vars, vals))
        result = eval(expr, NO_GLOBALS, locals)
        # print('\t'.join([str(v) for v in vals] + [str(result)]))
        if result is True:
            output_result.append('1')
        elif result is False:
            output_result.append('0')
        else:
            output_result.append(result)
    return output_result

def truth_table_inputs(num_inputs):
    return list(itertools.product([0, 1], repeat=num_inputs))


if __name__ == '__main__':
    caminho = argv[1]
    nome_arquivo = argv[2]
    # ABERTURA DO ARQUIVO
    file = open(caminho + "/" + nome_arquivo, "r")

    # INFORMACOES DO CABECALHO
    model_name = header_information(file)
    inputs_name = header_information(file)
    outputs_name = header_information(file)
    inputs_num = len(inputs_name)
    outputs_num = len(outputs_name)

    # EXPRESSAO BOOLEANO DO CIRCUITO
    outputs_expression = boolean_expression_generator(file)

    print(inputs_name)
    print(outputs_name)
    print("SAIDAS:")
    for j in range(0, len(outputs_expression)):
        print(outputs_expression[j])

    inputs_table = truth_table_inputs(inputs_num)
    outputs_table = []
    for i in range(outputs_num):
        outputs_table.append(truth_table_output(outputs_expression[i], inputs_num))

    for i in range(2**inputs_num):
        for j in range(inputs_num):
            print(inputs_table[i][j], end='')
        print(end=' ')
        for k in range(outputs_num):
            print(outputs_table[k][i], end='')
        print()

