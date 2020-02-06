/*
  ACUSTIC MODEL TEST PROCEDURE
  3D FINITE DIFFERENCES

  PETROBRAS / CENPES / GEOF	

  switch(pMDF.type_EQ)
    case 0:	//constant density
    case 1:	//variable density
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <malloc.h>

#include "struct_3DTD.h"
#include "prog_aux_3D.h"
#include "prog_kernel_3D.h"

//-------------------
// MODELING PROCEDURE
//-------------------
void EXEC_TASKS_WORKERS_3D(stParam_MDF *pMDF, stParam_SrcWavelet *st_SrcWavelet,
  stParam_SrcRcv3D *st_Src, float *c_VP0, float *c_RHO,
  stParam_WaveField_AC *cpu_stAC_U, FILE *fdb){

  float execGfps = 0.0f;

  //---------
  // Modeling
  //---------
  execGfps = ExecLoop_MOD_AC_3D(pMDF, st_SrcWavelet, st_Src, c_VP0, c_RHO, cpu_stAC_U, fdb);

  if(execGfps <= 0.0){
    fprintf(stderr, "ERROR ==> executing ExecLoop_MOD_AC_3D\n");
    exit(EXIT_FAILURE);
  }
  fprintf(fdb   , "###################################################################################\n");
  fprintf(fdb   , "###                  P E R F O R M A N C E       M E A S U R E                  ###\n");
  fprintf(fdb   , "### execGSps=%e  -> Giga Samples per seconds  (the higher the better) ###\n", execGfps);
  fprintf(fdb   , "###################################################################################\n");
  fprintf(stdout, "###################################################################################\n");
  fprintf(stdout, "###                  P E R F O R M A N C E       M E A S U R E                  ###\n");
  fprintf(stdout, "### execGSps=%e  -> Giga Samples per seconds  (the higher the better) ###\n", execGfps);
  fprintf(stdout, "###################################################################################\n");
  fflush(stdout);
}

//-----
// MAIN
//-----
int main(int argc, char** argv){
  char name[256];
  int flag;
  FILE	*fdb=NULL;
  size_t Nel2D;
  size_t SizeNel;
  size_t SizeT;

  stParam_MDF pMDF;
  stParam_SrcWavelet st_SrcWavelet; // Source wavelet and its size
  stParam_SrcRcv3D st_Src;          // Source info  

  float	*c_VP0 = NULL;    // Velocity model
  float	*c_RHO = NULL;    // Density model
  float	fat= 1.0f / (1024 * 1024);

  stParam_WaveField_AC 	cpu_stAC_U;

  //------------------
  // Creating log file
  //------------------
  fdb = fopen("run_log.txt", "w");
  if(fdb == NULL){
    fprintf(stderr, "ERROR ==> Opening File run_log.txt\n");
    return EXIT_FAILURE;
  }
  sprintf(name, "WALLTIME --> PROGRAM STARTS ....... OK");
  print_log(name, fdb);

  //--------------------------------------
  // Reading Finite Differences Parameters
  //--------------------------------------
  flag = init_Param_MDF(&pMDF, argc, argv);
  if(flag){
    fprintf(stderr, "ERROR ==> Reading Finite Differences Parameters\n");
    exit(EXIT_FAILURE);
  }
  fprintf(stdout, "\t==>Reading Finite Differences Parameters.................... OK!\n");
  fflush(stdout);

  //------------------------------------
  // Print Finite Differences Parameters
  //------------------------------------
  print_Param_MDF(&pMDF, stdout);
  print_Param_MDF(&pMDF, fdb);

  //------------------------------
  // Allocate Velocity and Density
  //------------------------------
  Nel2D   = pMDF.nnoi_global * pMDF.nnoj_global;
  SizeNel = Nel2D * pMDF.nnok_global;

  flag = alocMem1D_CPU_f((float **) &c_VP0, SizeNel, &pMDF);
  if(flag){
    fprintf(stdout, "\t==> Error allocating c_VP0\n");
    fflush(stdout);
    exit(EXIT_FAILURE);
  }

  flag = alocMem1D_CPU_f((float **) &c_RHO, SizeNel, &pMDF);
  if(flag){
    fprintf(stdout, "\t==> Error allocating c_RHO\n");
    fflush(stdout);
    exit(EXIT_FAILURE);
  }
  sprintf(name, "WALLTIME --> CPU Memory Allocation...... OK");
  print_log(name, fdb);

  //---------------
  // Velocity Model
  //---------------
  flag = def_prop_models(&pMDF, c_VP0, c_RHO);
  if(flag){
    fprintf(stderr, "ERROR ==> At velocity model definition\n");
    exit(EXIT_FAILURE);
  }

  //-----------------
  // Allocate wavelet 
  //-----------------
  flag = alocMem1D_CPU_f(&st_SrcWavelet.WVLT, pMDF.nTime, &pMDF);
  if(flag){
    fprintf(stderr, "ERROR ==> Allocating wavelet\n");
    exit(EXIT_FAILURE);
  }
  st_SrcWavelet.n1 = pMDF.nTime;
  fprintf(stdout, "\t Alocating wavelet structure.......... OK!\n");
  fprintf(stdout, "\t st_SrcWavelet.n1=%d pMDF.nTime=%d\n",st_SrcWavelet.n1, pMDF.nTime);
  fflush(stdout);

  //-----------------------------
  // Define source wavelet values
  //-----------------------------
  def_Src_wavelet(&pMDF, &st_SrcWavelet); 

  //-----------------
  // Allocate src rcv
  //-----------------
  flag = aloc_SrcRcv(1, &st_Src, &pMDF);
  if(flag){
    fprintf(stderr, "==> ERROR ==> Allocating SrcRcv\n");
    exit(EXIT_FAILURE);
  }

  //---------------------
  // Read source position
  //---------------------
  flag = read_SrcRcv(&st_Src, argc, argv, fdb);
  if(flag){
    fprintf(stderr, "==> ERROR ==> Reading src position \n");
    exit(EXIT_FAILURE);
  }

  //----------------
  // Allocate memory
  //----------------
  SizeT = sizeof(float);
  SizeT *= pMDF.NNOI;
  SizeT *= pMDF.NNOJ;
  SizeT *= pMDF.NNOK;

  flag = alocMem1D_CPU_f(&cpu_stAC_U.U0, SizeT, &pMDF);
  if(flag)
    exit(EXIT_FAILURE);

  flag = alocMem1D_CPU_f(&cpu_stAC_U.U1, SizeT, &pMDF);	
  if(flag)
    exit(EXIT_FAILURE);

  //------------
  // Run problem
  //------------
  fprintf(stdout, "###################################################################################\n");
  fprintf(stdout, "######################    P R O G R A M        S T A R T S   ######################\n");
  fprintf(stdout, "###################################################################################\n");
  fprintf(stdout, "\t ==> TotalMemAloc_CPU~%f Mb\n", pMDF.TotalMemAloc_CPU * fat);
  sprintf(name, "%s", "WALLTIME -->  Finish Initialization......OK");
  print_log(name, fdb);

  EXEC_TASKS_WORKERS_3D(&pMDF, &st_SrcWavelet,&st_Src, c_VP0, c_RHO, &cpu_stAC_U, fdb);

  fprintf(stdout, "PROGRAM ENDS OK\n");
  sprintf(name, "%s", "WALLTIME --> PROGRAM ENDS............... OK");
  print_log(name, fdb);

  return 0;
}
