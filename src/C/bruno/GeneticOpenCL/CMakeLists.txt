cmake_minimum_required(VERSION 3.12)
project(GeneticOpenCL)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)
include_directories(include)
link_directories(lib/x64)



add_executable(GeneticOpenCL
        arvore.cpp arvore.h
        bitwise.cpp  bitwise.h
        constantes.h
        genetica.cpp genetica.h
        GPTime.cpp GPTime.h
        oclConfig.cpp oclConfig.h
        pilha.cpp pilha.h
        utilitarios.cpp utilitarios.h
        main.cpp)

target_link_libraries(GeneticOpenCL OpenCL)