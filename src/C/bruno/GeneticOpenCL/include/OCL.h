#ifndef OCL_H
#define OCL_H


class OCL
{
    public:
        OCL();
        ~OCL();
        cl_ulong inicio, fim;
        ///Evento para controlar tempo gasto
        cl::Event e_tempo;

        cl_int result; //Variavel para verificar erros
        ///TODO: Colocar conferencias de erros pelo código
        std::vector<cl::Platform> platforms;
        std::vector<cl::Device> devices;
        cl::Context* contexto;
        cl::CommandQueue* cmdQueueCPU;
        cl::CommandQueue* cmdQueueGPU;
        cl::Buffer* bufferPopA;
        cl::Buffer* bufferPopF;
        cl::Buffer* bufferOpTerm;
        cl::Buffer* bufferSeeds;
        cl::Buffer* dados;
        cl::Kernel* krnlAvalia;
        cl::Kernel* krnlEvolucao;

    protected:

    private:
};

#endif // OCL_H
