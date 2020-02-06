// ------------------------------------------------------------------------
/*
  Código em OpenCL para o cálculo paralelo heterogêneo (CPU + GPU) de:
                              ___
                x = 3 * x - \| x  ,

  onde x é um vetor.
  
  A multiplicação e raiz quadrada são processadas concorrentemente, onde a CPU
  calcula paralelamente '3*x' e a GPU 'sqrt(x)'. Finalmente, apenas a CPU faz,
  em paralelo, a subtração dos resultados parciais e atribuição final.

        Programação Paralela de Aceleradores em OpenCL - LNCC - Jul/2013
                      Douglas A. Augusto (douglas@lncc.br)
*/

// ------------------------------------------------------------------------
// Habilita disparar exceções C++
#define __CL_ENABLE_EXCEPTIONS

// Cabeçalho OpenCL para C++
#include "CL/cl.hpp"

#include <iostream> // para jogar na tela as saídas
#include <vector>   // utiliza o vetor com alocação dinâmica do C++
#include <utility>  // para usar a função make_pair: "junta" duas variáveis
#include <cstdlib>  // usar a função 'atoi': converte uma string em um valor inteiro

using namespace std;

// ------------------------------------------------------------------------
// Kernels OpenCL
const char * kernel_str  = 
   "__kernel void "
   "raiz( __global const float * x, __global float * y ) "
   "{ "
   "   int i = get_global_id(0); "
   "   y[i] = sqrt( x[i] ); "
   "} "
   " "
   "__kernel void "
   "mult( __global const float * x, __global float * y, float s ) "
   "{ "
   "   int i = get_global_id(0); "
   "   y[i] = s * x[i]; "
   "} "
   " "
   "__kernel void "
   "subt( __global float * x, __global const float * y, __global const float * z ) "
   "{ "
   "   int i = get_global_id(0); "
   "   x[i] = y[i] - z[i]; "
   "} ";

// ------------------------------------------------------------------------
int main( int argc, char* argv[] )
{
   // Teste para verificar se o número de elementos foi informado
   if( argc < 2 || atoi( argv[1] ) < 1 )
   {
      cerr << "ERRO: Especificar o número de elementos.\n";
      return 1;
   }

   // Dados de entrada: vetor de número reais
   const unsigned elementos = atoi( argv[1] );

   // Aloca as memórias para os vetores X, cpuY e gpuY, e faz cada elemento do
   // vetor X ter o valor do seu próprio índice
   float *X = new float[elementos];
   float *cpuY = new float[elementos];
   float *gpuY = new float[elementos];

   for( int i = 0; i < elementos; ++i ) X[i] = i;

   // Descobrir e escolher as plataformas e dispositivos
   vector<cl::Platform> plataformas;
   vector<cl::Device> cpu_dispositivos, gpu_dispositivos;

   // Descobre as plataformas instaladas no hospedeiro
   cl::Platform::get( &plataformas );
   // Descobre os dispositivos CPU e GPU: para simplificar, vamos procurar
   // apenas os dispositivos da primeira plataforma (plataformas[0])
   plataformas[0].getDevices( CL_DEVICE_TYPE_GPU, &gpu_dispositivos );
   plataformas[1].getDevices( CL_DEVICE_TYPE_CPU, &cpu_dispositivos );

   // Criar os contextos
   cl::Context cpu_contexto( cpu_dispositivos );
   cl::Context gpu_contexto( gpu_dispositivos );

   // Criar as filas de comandos para cada arquitetura (o primeiro dispositivo)
   cl::CommandQueue cpu_fila( cpu_contexto, cpu_dispositivos[0] );
   cl::CommandQueue gpu_fila( gpu_contexto, gpu_dispositivos[0] );

   // Carregar os programas, compilá-los e gerar os kernels
   cl::Program::Sources fonte( 1, make_pair( kernel_str, strlen( kernel_str ) ) );
   cl::Program cpu_programa( cpu_contexto, fonte );
   cl::Program gpu_programa( gpu_contexto, fonte );

   // Compila para todos os dispositivos associados a '[cpu|gpu]_programa'
   // através de '[cpu|gpu]_contexto'
   cpu_programa.build( cpu_dispositivos );
   gpu_programa.build( gpu_dispositivos );

   // Cria os objetos que representarão cada um dos três kernels
   cl::Kernel kernel_mult( cpu_programa, "mult" );
   cl::Kernel kernel_subt( cpu_programa, "subt" );
   cl::Kernel kernel_raiz( gpu_programa, "raiz" );

   // Preparação da memória dos dispositivos (leitura e escrita}
   cl::Buffer cpu_bufferX( cpu_contexto, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, 
                           elementos * sizeof(float), X );
   cl::Buffer gpu_bufferX( gpu_contexto, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                           elementos * sizeof(float), X );

   cl::Buffer cpu_bufferY( cpu_contexto, CL_MEM_READ_WRITE, elementos * sizeof(float) );
   cl::Buffer gpu_bufferY( gpu_contexto, CL_MEM_WRITE_ONLY, elementos * sizeof(float) );

   // Execução dos kernels: definição dos argumentos e trabalho/particionamento
   kernel_mult.setArg( 0, cpu_bufferX );
   kernel_mult.setArg( 1, cpu_bufferY );
   kernel_mult.setArg( 2, float(3) );

   kernel_raiz.setArg( 0, gpu_bufferX );
   kernel_raiz.setArg( 1, gpu_bufferY );

   // Paralelismo implícito: tamanho local é definido como "nulo"; a
   // implementação é que vai decidir se divide em grupos e como dividi-los
   cpu_fila.enqueueNDRangeKernel( kernel_mult, cl::NDRange(), 
                                  cl::NDRange( elementos ), cl::NDRange() );
   gpu_fila.enqueueNDRangeKernel( kernel_raiz, cl::NDRange(),
                                  cl::NDRange( elementos ), cl::NDRange() );

   cpu_fila.flush(); // força a execução dos comandos da fila
   gpu_fila.flush(); // força a execução dos comandos da fila

   // Transferência dos resultados da GPU para o hospedeiro (joga em gpuY)
   // (comando bloqueante: CL_TRUE)
   gpu_fila.enqueueReadBuffer( gpu_bufferY, CL_TRUE, 0,
                               elementos * sizeof(float), gpuY );

   // Criar um buffer na CPU com os resultados oriundos da GPU
   cl::Buffer cpu_bufferZ( cpu_contexto, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                           elementos * sizeof(float), gpuY );

   // Execução do kernel final: definição dos argumentos e trabalho/particionamento
   kernel_subt.setArg( 0, cpu_bufferX );
   kernel_subt.setArg( 1, cpu_bufferY );
   kernel_subt.setArg( 2, cpu_bufferZ );

   cpu_fila.enqueueNDRangeKernel( kernel_subt, cl::NDRange(),
                                  cl::NDRange( elementos ), cl::NDRange() );

   // Transferência dos resultados da CPU para o hospedeiro (joga em X)
   // (comando bloqueante: CL_TRUE)
   cpu_fila.enqueueReadBuffer( cpu_bufferX, CL_TRUE, 0, elementos * sizeof(float), X );

   // Impressão do resultado
   for( int i = 0; i < elementos; ++i ) cout << '[' << X[i] << ']'; cout << endl;

   // Limpeza (as variáveis específicas do OpenCL já são automaticamente destruídas)
   delete[] X, cpuY, gpuY;

   return 0;
}
