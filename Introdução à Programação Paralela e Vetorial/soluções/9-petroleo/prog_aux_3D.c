/*
  ACUSTIC MODEL TEST PROCEDURE
  3D FINITE DIFFERENCES

  PETROBRAS/CENPES/GEOF	
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

//==============================================
// INPUT EXECUTION PARAMETERS (FROM ENVIRONMENT) 
//==============================================
int init_Param_MDF(stParam_MDF *pMDF, int argc, char **argv){
  int ival;
  float fval,pi;
  int flag;
  char *NAME;

  pi = 4.0f * atan(1.0f);

  //------------------------
  //allocate local variables
  //------------------------
  NAME = (char *) malloc(128 * sizeof(char));

  //---------------------
  //read input parameters
  //---------------------
  sprintf(NAME, "VERBOSE");
  flag = find_arg_int(argc, argv, NAME, &ival, 1);	
  if(flag == 0)
    pMDF->VERBOSE = ival;
  else
    pMDF->VERBOSE = 0;

  sprintf(NAME, "TIPO_EQUACAO");
  flag = find_arg_int(argc, argv, NAME, &ival, 0);
  if(flag)
    return 1;
  /*def*/
  pMDF->type_EQ = ival;

  if(pMDF->type_EQ != 0){
    fprintf(stderr, "EQUATION != 0!\n");
    return 1;
  }

  sprintf(NAME, "N1_GLOBAL");
  flag = find_arg_int(argc, argv, NAME, &ival, 0);	
  if(flag)
    return 1;
  /*def*/
  pMDF->nnok_global = ival;

  sprintf(NAME, "N2_GLOBAL");
  flag = find_arg_int(argc, argv, NAME, &ival, 0);	
  if(flag)
    return 1;
  /*def*/
  pMDF->nnoi_global = ival;

  sprintf(NAME, "N3_GLOBAL");
  flag = find_arg_int(argc, argv, NAME, &ival, 0);
  if(flag)
    return 1;
  /*def*/
  pMDF->nnoj_global = ival;

  sprintf(NAME, "D1");
  flag = find_arg_float(argc, argv, NAME, &fval, 0);	
  if(flag)
    return 1;
  /*def*/
  pMDF->dz = fval;

  sprintf(NAME, "D2");
  flag = find_arg_float(argc, argv, NAME, &fval, 0);	
  if(flag)
    return 1;
  /*def*/
  pMDF->dx = fval;

  sprintf(NAME, "D3");
  flag = find_arg_float(argc, argv, NAME, &fval, 0);	
  if(flag)
    return 1;
  /*def*/
  pMDF->dy = fval;

  sprintf(NAME, "DT");
  flag = find_arg_float(argc, argv, NAME, &fval, 0);	
  if(flag)
    return 1;
  /*def*/
  pMDF->dt= fval;

  sprintf(NAME, "NTSTEP");
  flag = find_arg_int(argc, argv, NAME, &ival, 0);	
  if(flag)
    return 1;
  /*def*/
  pMDF->nTime = ival;

  sprintf(NAME, "FC");
  flag = find_arg_float(argc, argv, NAME, &fval, 0);	
  if(flag)
    return 1;
  /*def*/
  pMDF->FC= fval;
	
  sprintf(NAME, "VP_DEF");
  flag = find_arg_float(argc, argv, NAME, &fval, 0);	
  if(flag)
    return 1;
  pMDF->VP_def	= fval;

  sprintf(NAME, "RHO_DEF");
  flag = find_arg_float(argc, argv, NAME, &fval, 0);	
  if(flag)
    return 1;
  pMDF->RHO_def	= fval;

  sprintf(NAME, "INT_SNAPSHOT");
  flag = find_arg_int(argc, argv, NAME, &ival, 0);
  pMDF->INT_SNAPSHOT = ival;

  //--------------------------
  //define auxiliary variables
  //--------------------------
  /*def*/
  pMDF->PRINT_SNAP = 1;
  /*def*/
  pMDF->sourceTf = 2.0f * sqrtf(pi) / pMDF->FC;	//Source Delay

  /*def*/
  pMDF->nborda = 0;
  /*def*/
  pMDF->NPOP = def_NPOP_AC;

  /*def*/
  pMDF->NNOK = pMDF->nnok_global + 2 * pMDF->nborda;
  /*def*/
  pMDF->NNOI = pMDF->nnoi_global + 2 * pMDF->nborda;
  /*def*/
  pMDF->NNOJ = pMDF->nnoj_global + 2 * pMDF->nborda;

  pMDF->TotalMemAloc_CPU = 0;

  //--------------------------
  //deallocate local variables
  //--------------------------
  free(NAME);
  return 0;
}

//=======================
// PRINT INPUT PARAMETERS
//=======================
void print_Param_MDF(stParam_MDF *pMDF, FILE *f1){
  fprintf(f1, "#########################################################################\n");
  fprintf(f1, "\tINPUT PARAMETERS:\n");
  fprintf(f1, "#########################################################################\n");
  fprintf(f1, "==> VERBOSE............................................ %d\n", pMDF->VERBOSE);
  fprintf(f1, "==> EQUATION TYPE ->type_EQ............................ %d\n", pMDF->type_EQ );
  fprintf(f1, "#########################################################################\n");
  fprintf(f1, "N1_GLOBAL.............................................. %d\n", pMDF->nnok_global);
  fprintf(f1, "N2_GLOBAL.............................................. %d\n", pMDF->nnoi_global);
  fprintf(f1, "N3_GLOBAL.............................................. %d\n", pMDF->nnoj_global);
  fprintf(f1, "D1..................................................... %f\n", pMDF->dz);
  fprintf(f1, "D2..................................................... %f\n", pMDF->dx);
  fprintf(f1, "D3..................................................... %f\n", pMDF->dy);
  fprintf(f1, "DT..................................................... %f\n", pMDF->dt);
  fprintf(f1, "NTSTEP................................................. %d\n", pMDF->nTime);
  fprintf(f1, "FC..................................................... %f\n", pMDF->FC);
  fprintf(f1, "#########################################################################\n");
  fprintf(f1, "\tPROPERTIES MODEL:\n");
  fprintf(f1, "VP_DEF................................................. %f\n", pMDF->VP_def);
  fprintf(f1, "RHO_DEF................................................ %f\n", pMDF->RHO_def);
  fprintf(f1, "#########################################################################\n");
  fprintf(f1, "INT_SNAPSHOT........................................... %d\n", pMDF->INT_SNAPSHOT);
  fprintf(f1, "#########################################################################\n");
}

//======================
// PRINT STRING AND TIME 
//======================
void print_log(char *text, FILE *f1){
  time_t horario;
  horario = time(NULL);
  fprintf(f1, "--> %s ==> %s", text, asctime(gmtime(&horario)));
  fflush(f1);
}

//========================
// DEFINE MODEL PROPERTIES
//========================
int def_prop_models(stParam_MDF *pMDF, float *c_VP0, float *c_RHO){
  long int i, nel;
  float	v, vp, rho;

  nel  = pMDF->nnoi_global;
  nel *= pMDF->nnoj_global;
  nel *= pMDF->nnok_global;

  switch(pMDF->type_EQ){
    case 0:	// constant density
      vp = pMDF->VP_def;
      for(i = 0; i < nel; ++i)
        c_VP0[i] = vp;
      
    break;
    case 1:	// variable density
      vp = pMDF->VP_def;
      rho = pMDF->RHO_def;
      for(i = 0; i < nel; ++i){
        c_VP0[i] = vp;
        c_RHO[i] = rho;
      }
    break;
    default:
      fprintf(stderr, "ERROR ==> UNKNOWN type_EQ=%d\n", pMDF->type_EQ);
      fflush(stderr);
      return 1;
  }

  //-------------------------------------------------
  // Fill velocity model borders with negative values
  //-------------------------------------------------
  v = -vp;
  cria_BordaNeg_NPOP_3D(c_VP0, pMDF->nnoi_global, pMDF->nnoj_global, pMDF->nnok_global, pMDF->NPOP, v);
  return 0;
}

//==============================================
// FILL NPOP BORDERS OF V[NNOI,NNOJ,NNOK] WITH V
//==============================================
void cria_BordaNeg_NPOP_3D(float *V, int NNOI, int NNOJ, int NNOK, int NPOP, float v){
  long int i, j, k;
  long int II, JJ, KK;

  for(k = 0; k < NNOK; k++){
    KK = k * NNOI * NNOJ;
    for(j = 0; j < NNOJ; j++){
      JJ = j * NNOI + KK;
      for(i = 0; i < NPOP; i++){
        II = JJ + i;
        V[II] = v;

        II = JJ + NNOI - i - 1;
        V[II] = v;
      }
    }
  }

  for(k = 0; k < NNOK; k++){
    KK = k * NNOI * NNOJ;
    for(j = 0; j < NPOP; j++){
      JJ = j * NNOI + KK;
      for(i = 0; i < NNOI; i++){
        II = JJ + i;
        V[II] = v;
      }

      JJ = (NNOJ - j - 1) * NNOI + KK;
      for(i = 0; i < NNOI; i++){
        II = JJ + i;
        V[II] = v;
      }
    }
  }

  for(k = 0; k < NPOP; k++){
    KK = k * NNOI * NNOJ;
    for(j = 0; j < NNOJ; j++){
      JJ = j * NNOI + KK;
      for(i = 0; i < NNOI; i++){
        II = JJ + i;
        V[II] = v;
      }
    }
    KK = (NNOK - k - 1) * NNOI * NNOJ;
    for(j = 0; j < NNOJ; j++){
      JJ = j * NNOI + KK;
      for(i = 0; i < NNOI; i++){
        II = JJ + i;
        V[II] = v;
      }
    }
  }

}

//===============
// SOURCE WAVELET
//===============
void def_Src_wavelet(stParam_MDF *pMDF, stParam_SrcWavelet *st_Src){
  size_t SizeT;
  int	i;
  float	FC, TimeDelay, Amp;
  float	fcR, sourceTf, t;
  FILE *f1 = NULL;
  char nameFile[256];

  FC = pMDF->FC;
  TimeDelay = 0.0f;
  Amp = 1000.0f;

  fprintf(stdout, "Source --> FC=%f TimeDelay=%f --> nTime_out=%d sourceTf=%f n1=%d\n",
           FC, TimeDelay, pMDF->nTime, pMDF->sourceTf, st_Src->n1);

  fcR = 0.5f * FC;
  sourceTf = pMDF->sourceTf;

  SizeT = pMDF->nTime * sizeof(float);
  memset(&st_Src->WVLT[0], 0, SizeT);

  for(i = 0; i < pMDF->nTime; i++){
    t = i * pMDF->dt - sourceTf - TimeDelay; 
    st_Src->WVLT[i] = Amp * Source_Ricker(t, fcR);
  }

  //DEBUG
  if(pMDF->VERBOSE >= 3){
    sprintf(nameFile, "srcWvlt_MOD.bin");
    f1 = fopen(nameFile, "wb");
    fwrite(&st_Src->WVLT[0], sizeof(float), pMDF->nTime, f1);
    fclose(f1);
  }

}

//===================
// RICKER TYPE SOURCE
//===================
float Source_Ricker(float t,float fc){ //t-time fc-cut frequency
  static float r, alpha, aux;
  static float pi;

  pi = 4.0f * atan(1.0f);
  alpha = sqrtf(.5) * pi * fc;
  aux = (t * alpha) * (t * alpha);
  r = (1.0 - 2.0 * aux) * expf(-aux);

  return r;
}

//==============================================
// MEMORY ALLOCATION FOR stParam_SrcRcv VARIABLE
//==============================================
int aloc_SrcRcv(const int nel, stParam_SrcRcv3D *pSrcRcv, stParam_MDF *pMDF){
  int flag, i;

  i = nel;
  flag = 0;

  flag += alocMem1D_CPU_i(&pSrcRcv->cI_grid, i, pMDF);
  flag += alocMem1D_CPU_i(&pSrcRcv->cJ_grid, i, pMDF);
  flag += alocMem1D_CPU_i(&pSrcRcv->cK_grid, i, pMDF);	

  return flag;
}

//======================================
// READ SOURCE POSITION FROM ENVIRONMENT
//======================================
int read_SrcRcv(stParam_SrcRcv3D *st_Src, int argc, char **argv, FILE *f1){
  int ival, opc, flag;
  char *NAME;
  char *vNAME;

  NAME = (char *) malloc(128 * sizeof(char));
  vNAME = (char *) malloc(256 * sizeof(char));

  sprintf(NAME, "SRC1");
  opc = 0;
  flag = find_arg_int(argc, argv, NAME, &ival,opc);	
  if(flag)
    return 1;
  /*def*/ 
  st_Src->cI_grid[0] = ival;

  sprintf(NAME, "SRC2");
  opc = 0;
  flag = find_arg_int(argc, argv, NAME, &ival,opc);	
  if(flag)
    return 1;
  /*def*/ 
  st_Src->cJ_grid[0] = ival;

  sprintf(NAME, "SRC3");
  opc = 0;
  flag = find_arg_int(argc, argv, NAME, &ival,opc);	
  if(flag)
    return 1;
  /*def*/ 
  st_Src->cK_grid[0] = ival;

  fprintf(stdout, "#########################################################################\n");
  fprintf(stdout, "\tSOUCE PARAMETERS:\n");	
  fprintf(stdout, "SRC1................................................... %d\n", st_Src->cI_grid[0]);
  fprintf(stdout, "SRC2................................................... %d\n", st_Src->cJ_grid[0]);
  fprintf(stdout, "SRC3................................................... %d\n", st_Src->cK_grid[0]);
  fprintf(f1, "#########################################################################\n");
  fprintf(f1, "\tSOUCE PARAMETERS:\n");	
  fprintf(f1, "SRC1................................................... %d\n", st_Src->cI_grid[0]);
  fprintf(f1, "SRC2................................................... %d\n", st_Src->cJ_grid[0]);
  fprintf(f1, "SRC3................................................... %d\n", st_Src->cK_grid[0]);
  
  free(vNAME);
  free(NAME);

  return 0;
}

//================
// ALLOCATE FLOATS
//================
int alocMem1D_CPU_f(float **cpu, size_t n1, stParam_MDF *pMDF){
  float *p = NULL;
  p = (float *) alloc1(n1, sizeof(float));

  if(p == NULL)
    return 1;

  *cpu = p;
  pMDF->TotalMemAloc_CPU += (1.0f * n1 * sizeof(float));

  return 0;
}

//==================
// ALLOCATE INTEGERS
//==================
int alocMem1D_CPU_i(int **cpu, size_t n1, stParam_MDF *pMDF){
  int *p = NULL;
  p = (int *) alloc1(n1, sizeof(int));

  if(p == NULL)
    return 1;

  *cpu = p;
  pMDF->TotalMemAloc_CPU += (1.0f * n1 * sizeof(int));

  return 0;
}

//===================
// GENERIC ALLOCATION
//===================
void *alloc1(size_t n1, size_t type){
  void *p = memalign(32, n1 * type);

  memset((void*) p, '\0', n1 * type);

  return p;
}

//=============================================
// FIND STRING AT ARGV; RETURN ASSOCIATED FLOAT
//=============================================
int find_arg_float(int argc, char **argv, char *name, float *val, int opc){
  int i = 0;
  int log = 0;
  char *teste = NULL;
  int len;

  strcat(name, "=");

  while(i < argc && log == 0){
    teste = strstr(argv[i], name);
    if(teste != NULL){
      len = strlen(name);
      val[0] = atof(teste + len);
      log = 1;
      return 0;
    }
    ++i;
  }

  if(opc){
    val[0] = 0.0;
    return 2;
  }else{
    fprintf(stderr, "#############################################################\n");
    fprintf(stderr, "==> Parameter not set: %s \n", name);
    fprintf(stderr, "#############################################################\n");
    return 1;
  }
  
}

//===============================================
// FIND STRING AT ARGV; RETURN ASSOCIATED INTEGER
//===============================================
int find_arg_int(int argc, char **argv, char *name, int *val, int opc){
  int i = 0;
  int log = 0;
  char *teste = NULL;
  int len;

  strcat(name, "=");

  while(i < argc && log == 0){
    teste = strstr(argv[i], name);
    if(teste != NULL){
      len = strlen(name);
      val[0] = atoi(teste + len);
      log = 1;
      return 0;
    }
    ++i;
  }

  if(opc){
    val[0] = 0;
    return 2;
  }else{
    fprintf(stderr, "#############################################################\n");
    fprintf(stderr, "==> Parameter not set: %s \n", name);
    fprintf(stderr, "#############################################################\n");
    return 1;
  }

}