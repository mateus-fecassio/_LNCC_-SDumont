// g++ -I/usr/include/CL -L/opt/ati-stream-sdk/lib/x86_64/ -lOpenCL ocl.cc
// -L não necessário caso libOpenCL.so esteja em /usr/lib

// prjcuda02
// g++ -I. -lOpenCL ocl.cc

#define __CL_ENABLE_EXCEPTIONS

//#include <cl.h>
#include <cl.hpp>

#include <iostream>
#include <vector>

#include <utility>

using namespace std;


// Kernel que computa o valor absoluto de um elemento do vetor 'v'
const char * kernel_str  = 
   "__kernel void "
   "absoluto( __global float * v ) "
   "{ "
   "   int id = get_global_id(0); "
   "   v[id] = fabs( v[id] ); "
   "} ";

int main()
{
   // Dados de entrada: vetor de número reais
   const int elementos = 10;

   float *V = new float[elementos];
   for( int i = 0; i < elementos; ++i ) V[i] = -i;
   //for( int i = 0; i < elementos; ++i ) cout << '[' << V[i] << ']'; cout << endl;

   try {
      // ----- 1) Inicialização
      // Descobrir e escolher as plataformas
      vector<cl::Platform> plataformas;
      cl::Platform::get( &plataformas );

      cout << "Num plataformas: " << plataformas.size() << endl;

      for( int i = 0; i < plataformas.size(); ++i )
      {
         cout << '[' << plataformas[i].getInfo<CL_PLATFORM_NAME>().c_str() << ']' << endl;
         cout << '[' << plataformas[i].getInfo<CL_PLATFORM_VENDOR>().c_str() << ']' << endl;
         cout << '[' << plataformas[i].getInfo<CL_PLATFORM_EXTENSIONS>().c_str() << ']' << endl;
         cout << '[' << plataformas[i].getInfo<CL_PLATFORM_PROFILE>().c_str() << ']' << endl;
         cout << '[' << plataformas[i].getInfo<CL_PLATFORM_VERSION>().c_str() << ']' << endl;
         cout << endl;
      }

      // Descobrir e escolher os dispositivos
      vector<cl::Device> dispositivos;
      plataformas[0].getDevices( CL_DEVICE_TYPE_ALL, &dispositivos );

      cout << "Num dispositivos: " << dispositivos.size() << endl;

      // Criar o contexto
      cl::Context contexto( dispositivos );

      // Criar a fila de comandos para um dispositivo
      cl::CommandQueue fila( contexto, dispositivos[0] );


      // Carregar o programa, compilá-lo e gerar o kernel
      cl::Program::Sources codigo_fonte( 1, make_pair( kernel_str, strlen( kernel_str ) ) );
      cl::Program programa( contexto, codigo_fonte );
      programa.build( dispositivos );

      cl::Kernel kernel( programa, "absoluto" );


      // ------ 2) Preparação da memória de leitura e escrita
      cl::Buffer buffer( contexto, CL_MEM_READ_WRITE, elementos * sizeof( float ) );


      // ------ 3) Execução
      // Transferência de dados para o dispositivo
      fila.enqueueWriteBuffer( buffer, CL_TRUE, 0, elementos * sizeof( float ), V );

      // Execução do kernel: definição dos argumentos e trabalho/particionamento
      kernel.setArg( 0, buffer );
      fila.enqueueNDRangeKernel( kernel, cl::NullRange, cl::NDRange( elementos ), cl::NullRange );

      // Transferência do resultado para o hospedeiro
      fila.enqueueReadBuffer( buffer, CL_TRUE, 0, elementos * sizeof( float ), V );
   }
   catch (cl::Error err) {
      std::cerr << "ERROR: " << err.what() << " ( " << err.err() << " )" << std::endl;
   }

   // Impressão do resultado
   for( int i = 0; i < elementos; ++i ) cout << '[' << V[i] << ']'; cout << endl;

   // Limpeza
   delete[] V;

   return 0;
}
