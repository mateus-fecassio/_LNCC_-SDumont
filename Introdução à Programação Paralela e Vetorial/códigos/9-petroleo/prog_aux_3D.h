/*
  ACUSTIC MODEL TEST PROCEDURE
  3D FINITE DIFFERENCES

  PETROBRAS/CENPES/GEOF	
*/

#ifndef _prog_aux_3D_h
#define _prog_aux_3D_h

#include "struct_3DTD.h"
//==============================================
// INPUT EXECUTION PARAMETERS (FROM ENVIRONMENT) 
//==============================================
int init_Param_MDF(stParam_MDF *, int, char **);
//=======================
// PRINT INPUT PARAMETERS
//=======================
void print_Param_MDF(stParam_MDF *, FILE *);
//======================
// PRINT STRING AND TIME
//======================
void print_log(char *, FILE *);
//========================
// DEFINE MODEL PROPERTIES
//========================
int def_prop_models(stParam_MDF *, float *, float *);
//==============================================
// FILL NPOP BORDERS OF V[NNOI,NNOJ,NNOK] WITH V
//==============================================
void cria_BordaNeg_NPOP_3D(float *, int, int, int, int, float);
//===============
// SOURCE WAVELET
//===============
void def_Src_wavelet(stParam_MDF *, stParam_SrcWavelet *); 
//===================
// RICKER TYPE SOURCE
//===================
float Source_Ricker(float, float);
//==================================================
// CPU MEMORY ALLOCATION FOR stParam_SrcRcv VARIABLE
//==================================================
int aloc_SrcRcv(const int, stParam_SrcRcv3D *, stParam_MDF *);
//======================================
// READ SOURCE POSITION FROM ENVIRONMENT
//======================================
int read_SrcRcv(stParam_SrcRcv3D *, int, char **, FILE *);
//================
// ALLOCATE FLOATS
//================
int alocMem1D_CPU_f(float **, size_t, stParam_MDF *);
//==================
// ALLOCATE INTEGERS
//==================
int alocMem1D_CPU_i(int **, size_t, stParam_MDF *);
//===================
// GENERIC ALLOCATION
//===================
void* alloc1(size_t, size_t);
//=============================================
// FIND STRING AT ARGV; RETURN ASSOCIATED FLOAT
//=============================================
int find_arg_float(int, char **, char *, float *, int);
//===============================================
// FIND STRING AT ARGV; RETURN ASSOCIATED INTEGER
//===============================================
int find_arg_int(int, char **, char *, int *, int);

#endif