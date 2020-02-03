// Habilita disparar exceções C++
#define __CL_ENABLE_EXCEPTIONS

// Cabeçalho OpenCL para C++
#include <cl.hpp>

#include <iostream>
#include <vector>
#include <utility>

using namespace std;

// ------------------------------------------------------------------------
// Kernel que computa a raiz quadrada de um elemento do vetor 'v'
const char * kernel_str  = 
   "__kernel void "
   "raiz( __global float * v ) "
   "{ "
   "   int id = get_global_id(0); "
   "   v[id] = sqrt( v[id] ); "
   "} ";

// ------------------------------------------------------------------------
int main()
{
   // Dados de entrada: vetor de número reais
   const int elementos = 10;

   float *V = new float[elementos];
   for( int i = 0; i < elementos; ++i ) V[i] = i;


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
   //programa.build( dispositivos );
   programa.build( vector<cl::Device>() );

   cl::Kernel kernel( programa, "raiz" );


   // 2) Preparação da memória de leitura e escrita
   cl::Buffer buffer( contexto, CL_MEM_READ_WRITE, elementos * sizeof( float ) );


   // 3) Execução
   //    Transferência de dados para o dispositivo
   fila.enqueueWriteBuffer( buffer, CL_TRUE, 0, elementos * sizeof( float ), V );

   //    Execução do kernel: definição dos argumentos e trabalho/particionamento
   kernel.setArg( 0, buffer );
   fila.enqueueNDRangeKernel( kernel, cl::NDRange(), cl::NDRange( elementos ), cl::NDRange() );

   //    Espera pela finalização da execução do kernel
   fila.finish();

   //    Transferência do resultado para o hospedeiro
   fila.enqueueReadBuffer( buffer, CL_TRUE, 0, elementos * sizeof( float ), V );


   // Impressão do resultado
   for( int i = 0; i < elementos; ++i ) cout << '[' << V[i] << ']'; cout << endl;

   // Limpeza
   delete[] V;

   return 0;
}
