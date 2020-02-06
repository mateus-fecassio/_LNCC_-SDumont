// ------------------------------------------------------------------------
//
// Código em OpenCL para o cálculo paralelo da raiz quadrada dos elementos de um vetor.

// Disciplina de "Computação Paralela Heterogênea", GB-500-202 - LNCC - 4°/2011
// Douglas A. Augusto (douglas@lncc.br)

// ------------------------------------------------------------------------
// Habilita disparar exceções C++
#define __CL_ENABLE_EXCEPTIONS

// Cabeçalho OpenCL para C++
#include <cl.hpp>

#include <iostream> // para jogar na tela as saídas
#include <vector>   // utiliza o vetor com alocação dinâmica do C++
#include <utility>  // para usar a função make_pair: "junta" duas variáveis
#include <cstdlib>  // usar a função 'atoi': converte uma string em um valor inteiro

using namespace std;

// ------------------------------------------------------------------------
// Kernel OpenCL que computa a raiz quadrada de um elemento do vetor 'x'
// e coloca-a em 'y'
const char * kernel_str  = 
   "__kernel void "
   "raiz( __global const float * x, __global float * y ) "
   "{ "
   "   int i = get_global_id(0); "
   "   y[i] = sqrt( x[i] ); "
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

   // Aloca as memórias para os vetores X e Y e faz cada elemento do vetor
   // X ter o valor do seu próprio índice
   float *X = new float[elementos];
   for( int i = 0; i < elementos; ++i ) X[i] = i;
   float *Y = new float[elementos];


   // 1) Inicialização
   //    Descobrir e escolher as plataformas e dispositivos
   vector<cl::Platform> plataformas;
   vector<cl::Device> dispositivos;

   // Descobre as plataformas instaladas no hospedeiro
   cl::Platform::get( &plataformas );
   // Descobre os dispositivos: para simplificar, vamos procurar apenas os
   // dispositivos da primeira plataforma (plataformas[0])
   plataformas[0].getDevices( CL_DEVICE_TYPE_ALL, &dispositivos );

   //    Criar o contexto
   cl::Context contexto( dispositivos );

   //    Criar a fila de comandos para um dispositivo (aqui só o primeiro)
   cl::CommandQueue fila( contexto, dispositivos[0] );

   //    Carregar o programa, compilá-lo e gerar o kernel
   cl::Program::Sources fonte( 1, make_pair( kernel_str, strlen( kernel_str ) ) );
   cl::Program programa( contexto, fonte );

   // Compila para todos os dispositivos associados a 'programa' através do
   // 'contexto': vector<cl::Device>() é um vetor nulo
   programa.build( vector<cl::Device>() );

   // Cria a variável kernel que vai representar o kernel "raiz"
   cl::Kernel kernel( programa, "raiz" );


   // 2) Preparação da memória dos dispositivos (leitura e escrita}
   cl::Buffer bufferX( contexto, CL_MEM_READ_ONLY, elementos * sizeof( float ) );
   cl::Buffer bufferY( contexto, CL_MEM_WRITE_ONLY, elementos * sizeof( float ) );


   // 3) Execução
   //    Transferência de dados para o dispositivo
   fila.enqueueWriteBuffer( bufferX, CL_TRUE, 0, elementos * sizeof( float ), X );

   //    Execução do kernel: definição dos argumentos e trabalho/particionamento
   kernel.setArg( 0, bufferX );
   kernel.setArg( 1, bufferY );

   // Paralelismo implícito: tamanho local é definido como "nulo"; a implementação é 
   // que vai decidir se divide em grupos e como dividi-los
   fila.enqueueNDRangeKernel( kernel, cl::NDRange(), cl::NDRange( elementos ), cl::NDRange() );

   //    Espera pela finalização da execução do kernel (forçar sincronia)
   fila.finish();

   //    Transferência dos resultados para o hospedeiro (e joga no Y)
   fila.enqueueReadBuffer( bufferY, CL_TRUE, 0, elementos * sizeof( float ), Y );


   // Impressão do resultado
   for( int i = 0; i < elementos; ++i ) cout << '[' << Y[i] << ']'; 
   cout << endl;

   // Limpeza (as variáveis específicas do OpenCL já são automaticamente destruídas)
   delete[] X, Y;

   return 0;
}
