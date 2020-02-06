/*
   Trechos de código:

   - Ler kernel em arquivo
   - Bloco try-catch (capturar erros do OpenCL runtime)
   - Depurar compilação de kernel
   - Procurar por um tipo específico de dispositivo
   - Medição de tempo ("profiling")
*/


// Ler kernel em arquivo:
   ifstream file("kernels.cl");
   string kernel_str( istreambuf_iterator<char>(file),
                      (istreambuf_iterator<char>()) );
   cl::Program::Sources fonte( 1, make_pair( kernel_str.c_str(), kernel_str.size() ) );


// Bloco try-catch (capturar erros do OpenCL runtime):
   try {
      // Comandos OpenCL aqui dentro...
   } catch( cl::Error e ) {
      cerr << "ERRO: " << e.what() << " ( " << e.err() << " )\n";
   }


// Depurar compilação de kernel:
   try {
      programa.build( vector<cl::Device>() );
   } catch( cl::Error& e ) {
      if( e.err() == CL_BUILD_PROGRAM_FAILURE )
      {
         string str =
            programa.getBuildInfo<CL_PROGRAM_BUILD_LOG>( dispositivos[0] );
         cerr << "Build log: " << str << endl;
      }
      throw;
   }


// Procurar por um tipo específico de dispositivo:
   vector<cl::Platform> plataformas;
   vector<cl::Device> dispositivos;
   cl::Platform::get( &plataformas );
   cl_device_type tipo = CL_DEVICE_TYPE_GPU;
   for( int i = 0; i < plataformas.size() && dispositivos.empty(); i++ )
   {
      vector<cl::Device> tmp;
      plataformas[i].getDevices( CL_DEVICE_TYPE_ALL, &tmp );
      for( int j = 0; j < tmp.size(); ++j )
         if( tmp[j].getInfo<CL_DEVICE_TYPE>() == tipo ) {
            dispositivos.push_back( tmp[j] ); break;
         }
   }


// Medição de tempo ("profiling"):
   cl::CommandQueue fila( ..., CL_QUEUE_PROFILING_ENABLE  );
   cl::Event e_tempo;
   fila.enqueueNDRangeKernel( ..., NULL, &e_tempo );
   cl_ulong inicio, fim;
   e_tempo.getProfilingInfo( CL_PROFILING_COMMAND_START, &inicio );
   e_tempo.getProfilingInfo( CL_PROFILING_COMMAND_END, &fim );
   double tempo_execucao_s = (fim - inicio)/1.0E9;
