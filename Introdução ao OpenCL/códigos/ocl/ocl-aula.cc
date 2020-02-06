// Habilita disparar exceções C++
#define __CL_ENABLE_EXCEPTIONS

// Cabeçalho OpenCL para C++
#include <cl.hpp>

#include <iostream>
#include <vector>
#include <utility>
#include <cstdlib>

using namespace std;

// ------------------------------------------------------------------------
// Kernel que computa a raiz quadrada de um elemento do vetor 'v'
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
   // Dados de entrada: vetor de número reais
   const int elementos = atoi( argv[1] );

   float *X = new float[elementos];
   for( int i = 0; i < elementos; ++i ) X[i] = i;
   float *Y = new float[elementos];


   // 1) Inicialização
   //    Descobrir e escolher as plataformas e dispositivos
   vector<cl::Platform> plataformas;
   vector<cl::Device> dispositivos;

   cl::Platform::get( &plataformas ); // plataformas
   plataformas[0].getDevices( CL_DEVICE_TYPE_ALL, &dispositivos ); // dispositivos

   //    Criar o contexto
   cl::Context contexto( dispositivos );

   //    Criar a fila de comandos para um dispositivo
   cl::CommandQueue fila( contexto, dispositivos[0] );

   //    Carregar o programa, compilá-lo e gerar o kernel
   cl::Program::Sources fonte( 1, make_pair( kernel_str, strlen( kernel_str ) ) );
   cl::Program programa( contexto, fonte );

   programa.build( vector<cl::Device>() );

   cl::Kernel kernel( programa, "raiz" );


   // 2) Preparação da memória (leitura e escrita}
   cl::Buffer bufferX( contexto, CL_MEM_READ_ONLY, elementos * sizeof( float ) );
   cl::Buffer bufferY( contexto, CL_MEM_WRITE_ONLY, elementos * sizeof( float ) );


   // 3) Execução
   //    Transferência de dados para o dispositivo
   fila.enqueueWriteBuffer( bufferX, CL_TRUE, 0, elementos * sizeof( float ), X );

   //    Execução do kernel: definição dos argumentos e trabalho/particionamento
   kernel.setArg( 0, bufferX );
   kernel.setArg( 1, bufferY );
   fila.enqueueNDRangeKernel( kernel, cl::NDRange(), cl::NDRange( elementos ), cl::NDRange() );

   //    Espera pela finalização da execução do kernel
   fila.finish();

   //    Transferência dos resultados para o hospedeiro
   fila.enqueueReadBuffer( bufferY, CL_TRUE, 0, elementos * sizeof( float ), Y );


   // Impressão do resultado
   for( int i = 0; i < elementos; ++i ) cout << '[' << Y[i] << ']'; cout << endl;

   // Limpeza
   delete[] X, Y;

   return 0;
}
