/*
  ACUSTIC MODEL TEST PROCEDURE
  3D FINITE DIFFERENCES

  PETROBRAS/CENPES/GEOF
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>

#include "struct_3DTD.h"
#include "prog_aux_3D.h"
#include "prog_kernel_3D.h"

#define min(x, y) ({                \
    typeof(x) _min1 = (x);          \
    typeof(y) _min2 = (y);          \
    (void) (&_min1 == &_min2);      \
    _min1 < _min2 ? _min1 : _min2; })

//===================================================
// ACOUSTIC MODELING FOR CONSTANT OR VARIABLE DENSITY
//===================================================
float ExecLoop_MOD_AC_3D(stParam_MDF *pMDF, stParam_SrcWavelet *st_SrcWavelet,
  stParam_SrcRcv3D *st_Src, float *c_VP0, float *c_RHO,
  stParam_WaveField_AC *cpu_stAC_U, FILE *fdb){

  double clock_exec;
  double execGfps;
  double clock0p, clock1p;
  int flag;

omp_sched_t type = omp_sched_auto;
int chunk = 0;

omp_get_schedule(&type, &chunk);

#pragma omp parallel
{
#pragma omp master
{
        printf("THREADS = %d\n", omp_get_num_threads());
        printf("Schedule: ");
        if(type == omp_sched_static)
                printf("static\n");
        else if(type == omp_sched_dynamic)
                printf("dynamic\n");
        else if(type == omp_sched_guided)
                printf("guided\n");
        else if(type == omp_sched_auto)
                printf("auto\n");
        else
                printf("error\n");
        if(chunk != 0)
                printf("Chunk: %d\n", chunk);
}
}


  //-------------------------
  // Wave Field Extrapolation
  //-------------------------
  clock0p = omp_get_wtime();

  flag = forward_wavefield_AC_3D(pMDF, st_SrcWavelet, st_Src, cpu_stAC_U, c_VP0, c_RHO, fdb);

  clock1p = omp_get_wtime();


  if(flag){
    fprintf(stderr, "ERROR ==>  at forward_wavefield_AC_3D\n");
    return -1.0;
  }

  //-------------------------
  // Performance (GSamples/s)
  //-------------------------
  clock_exec = (clock1p - clock0p);
  execGfps =  (1.0f * (
                  1.0f * pMDF->nTime * 1.0f * pMDF->NNOI * 1.0f * pMDF->NNOJ * pMDF->NNOK
                ) / clock_exec
              ) * 1.0e-9;
  return execGfps;
}

//=========================
// WAVE FIELD EXTRAPOLATION
//=========================
int forward_wavefield_AC_3D(stParam_MDF *pMDF, stParam_SrcWavelet *st_SrcWavelet,
  stParam_SrcRcv3D *st_Src, stParam_WaveField_AC *cpu_stAC_U,
  float *c_VP0, float *c_RHO, FILE *fdb){

  int n;
  size_t SizeT;
  double clock0_tot, clock1_tot;
  double clock0_snap, clock1_snap;
  double clock0_src, clock1_src;
  double clock0_exec, clock1_exec;
  double clock_tot, clock_snap, clock_src, clock_exec;
  float *pCPU = NULL;
  float FAT_AMP;
  char mFileName[256];
  FILE *f1;
  long int M;
  double kernel = 0.0, kernel_begin, kernel_end;
  float W[7], C[3];
  float FATMDFX, FATMDFY, FATMDFZ;

  //--------------
  // Set constants
  //--------------
  n = init_cte3D(pMDF, &FATMDFX, &FATMDFY, &FATMDFZ, W, C);
  if(n){
    fprintf(stderr, "ERROR ==> Initializing constants\n");
    exit(EXIT_FAILURE);
  }

  //-----------------------
  // Initialize Wave Fields
  //-----------------------
  SizeT = sizeof(float);
  SizeT *= pMDF->NNOI;
  SizeT *= pMDF->NNOJ;
  SizeT *= pMDF->NNOK;

  memset(cpu_stAC_U->U0, 0, SizeT);
  memset(cpu_stAC_U->U1, 0, SizeT);

  n = 0;
  clock_tot = clock_snap = clock_src = clock_exec = 0.0f;

  //----------
  // Time Loop
  //----------
  clock0_tot = omp_get_wtime();
  while(n < pMDF->nTime){

    //--------------------------------------------------
    // Dispatch Kernel to Update Source Wavefield Sample
    //--------------------------------------------------
    clock0_src = omp_get_wtime();
    FAT_AMP = 1.0f;
    if(n < st_SrcWavelet->n1 - 1){
      kernel_CPU_srcVet_3D(cpu_stAC_U->U0, c_VP0, 0, st_Src->cI_grid,
        st_Src->cJ_grid, st_Src->cK_grid, st_SrcWavelet->WVLT[n],
        FAT_AMP, pMDF->NNOI, pMDF->NNOJ);
    }
    clock1_src = omp_get_wtime();
    clock_src += (clock1_src - clock0_src);



    //-----------------------------------
    // Extrapolate Wavefield One Timestep
    //-----------------------------------
    clock0_exec = omp_get_wtime();
    kernel_begin = omp_get_wtime();
    switch(pMDF->type_EQ){
      case 0: // Constant Density
        exec_mod_06_ACRhoCte3D_CPU(cpu_stAC_U, c_VP0,
          pMDF->NNOI, pMDF->NNOJ, pMDF->NNOK, FATMDFX, FATMDFY, FATMDFZ, W);
      break;
      case 1: // Variable Density
        exec_mod_06_ACRhoVar3D_CPU(cpu_stAC_U, c_VP0, c_RHO,
          pMDF->NNOI, pMDF->NNOJ, pMDF->NNOK, FATMDFX, FATMDFY, FATMDFZ, W, C);
      break;
      default:
        fprintf(stderr, "ERROR ==> type_EQ=%d UNKNOWN\n", pMDF->type_EQ);
      break;
    }
    kernel_end = omp_get_wtime();
    kernel += (kernel_end - kernel_begin);
    clock1_exec = omp_get_wtime();
    clock_exec += (clock1_exec - clock0_exec);



    //----------------------
    // Toggle CPU Wavefields
    //----------------------
    pCPU = cpu_stAC_U->U0;
    cpu_stAC_U->U0 = cpu_stAC_U->U1;
    cpu_stAC_U->U1 = pCPU;

    n++;
  /*  if(n == 1 || n % def_COUNT == 0){
      fprintf(stdout, "\tModeling  -> step=%6d  \n",n);
      fflush(stdout);
    }*/
  }
  clock1_tot =  omp_get_wtime();
  clock_tot = (clock1_tot - clock0_tot);
  printf("TOTAL TIME: %.5lf\n", clock_tot);
  printf("Kernel:%.5lf\n", kernel);

    //----------------
    // Snapshot Output
    //----------------
    clock0_snap = omp_get_wtime();
//    if(pMDF->PRINT_SNAP == 1 &&  n % pMDF->INT_SNAPSHOT == 0){
/*    if(pMDF->PRINT_SNAP == 1){
      fprintf(stdout, "\tDumping snapshot at step %6d", n);
      fflush(stdout);

      M  = pMDF->NNOI;
      M *= pMDF->NNOJ;
      M *= pMDF->NNOK;

      sprintf(mFileName, "snap_U_%06d.bin", n);
      f1 = fopen(mFileName, "wb");
      fwrite(cpu_stAC_U->U0, sizeof(float), M, f1);
      fclose(f1);
      fprintf(stdout, " on file: < %s > \n", mFileName);

      sprintf(mFileName, "snap_U_%06d.txt", n);
      f1 = fopen(mFileName, "w");
      int isnap;
      for(isnap = 0; isnap < M; isnap++)
        //if(cpu_stAC_U->U0[isnap] < -0.001 || cpu_stAC_U->U0[isnap] > 0.001)
          fprintf(f1, "%.2lf\n", cpu_stAC_U->U0[isnap]);
      fclose(f1);
      fprintf(stdout, " on file: < %s > \n", mFileName);
    }
*/
    clock1_snap = omp_get_wtime();
    clock_snap += (clock1_snap - clock0_snap);

  //--------------------
  // Dump Execution Time
  //--------------------
  fprintf(fdb, "\t ==> @@@ %s @@@ -> Statistics Time \n", __func__);

  fprintf(fdb, "\t\tTotal time.................... = %e -> %6.2lf%c \n",
    clock_tot,  (100.0 * clock_tot  / clock_tot), '%');

  fprintf(fdb, "\t\tKernel execution time......... = %e -> %6.2lf%c \n",
    clock_exec, (100.0 * clock_exec / clock_tot), '%');

  fprintf(fdb, "\t\tSnapshot (I/O) time........... = %e -> %6.2lf%c \n",
    clock_snap, (100.0 * clock_snap / clock_tot), '%');

  fprintf(fdb, "\t\tPrescribes Source time........ = %e -> %6.2lf%c \n",
    clock_src,  (100.0 * clock_src   / clock_tot), '%');
  fflush(fdb);


  return 0;
}

//==========================================================================
// EXTRAPOLATE WAVEFIELD ONE TIMESTEP WITH CONSTANT DENSITY TIPO_EQUACAO = 0
//==========================================================================
void exec_mod_06_ACRhoCte3D_CPU(stParam_WaveField_AC *cpu_stAC_U,
  float *c_VP0, int nnoi, int nnoj, int nnok,
  float FATMDFX, float FATMDFY, float FATMDFZ, float *W){

  static int k0, k1;

  //----------------------------------------------------
  // Dispatch CPU Kernel to Extrapolate Single Time Step
  //----------------------------------------------------
  k0 = 1 * def_NPOP_AC;
  k1 = nnok - 1 * def_NPOP_AC;

  kernel_CPU_06_mod_3DRhoCte(cpu_stAC_U->U0, cpu_stAC_U->U1,
    c_VP0, nnoi, nnoj, k0, k1, FATMDFX, FATMDFY, FATMDFZ, W);

}

//==========================================================================
// EXTRAPOLATE WAVEFIELD ONE TIMESTEP WITH VARIABLE DENSITY TIPO_EQUACAO = 1
//==========================================================================
void exec_mod_06_ACRhoVar3D_CPU(stParam_WaveField_AC *cpu_stAC_U,
  float *c_VP0, float *c_RHO, int nnoi, int nnoj, int nnok,
  float FATMDFX, float FATMDFY, float FATMDFZ, float *W, float *C){

  static int  k0,k1;

  //----------------------------------------------------
  // Dispatch CPU Kernel to Extrapolate Single Time Step
  //----------------------------------------------------
  k0 = 1 * def_NPOP_AC;
  k1 = nnok - 1 * def_NPOP_AC;

  kernel_CPU_06_mod_3DRhoVar(cpu_stAC_U->U0, cpu_stAC_U->U1,
    c_VP0, c_RHO, nnoi, nnoj, k0, k1, FATMDFX, FATMDFY, FATMDFZ, W, C);
}

//-----------------------------------------
// KERNEL TO UPDATE SOURCE WAVEFIELD SAMPLE
//-----------------------------------------
void kernel_CPU_srcVet_3D(float* cpu_U1, float *cpu_V, int ind,
  int *ind_SrcI, int *ind_SrcJ, int *ind_SrcK,
  float val, float fat, int NNOI, int NNOJ){

  unsigned long int i = ind_SrcK[ind] * NNOI * NNOJ + ind_SrcJ[ind] * NNOI + ind_SrcI[ind];
  float v = cpu_V[i];

  cpu_U1[i] += fat * v * v * val;
}

//===========================================================================
// KERNEL TO EXTRAPOLATE SINGLE TIME STEP - VARIABLE DENSITY TIPO_EQUACAO = 1
//===========================================================================
void kernel_CPU_06_mod_3DRhoVar(float* gU0, float* gU1, float* gVorg, float* gDorg,
  int nnoi, int nnoj, int k0, int k1,
  float FATMDFX, float FATMDFY, float FATMDFZ, float *W, float *C){

  int index_X, index_Y;     // X, Y grid position
  int stride = nnoi * nnoj; // stride to next Z
  int index, k;

  //-----------------------------------------------
  // This Thread X-Y Grid
  //-----------------------------------------------
  #ifdef BLOCKING
  int block_X, block_Y;

  for(block_X = 0; block_X < nnoi; block_X += def_BSIZE_X)
    for(block_Y = 0; block_Y < nnoj; block_Y += def_BSIZE_Y)
      for(index_X = block_X; index_X < block_X + def_BSIZE_X; index_X++)
        for(index_Y = block_Y; index_Y < block_Y + def_BSIZE_Y; index_Y++)
  #endif

      #ifndef BLOCKING
      for(index_X = 0; index_X < nnoi; index_X++)
        for(index_Y = 0; index_Y < nnoj; index_Y++)
      #endif
        {
          //------------------------
          // Calculate initial index
          //------------------------
          index = (index_Y * nnoi + index_X) + k0 * stride;

          //-------
          // Z Loop
          //-------
          for(k = k0; k < k1; ++k){
            //------------------------------------------------
            // Wavefield Extrapolation Only on Interior Points
            //------------------------------------------------
            if(gVorg[index] > 0.0f){
              gU1[index] = 2.0f * gU0[index] - gU1[index]
                + FATMDFX * gVorg[index] * gVorg[index] * (
                  + W[6] * (gU0[index + 6] + gU0[index - 6])
                  + W[5] * (gU0[index + 5] + gU0[index - 5])
                  + W[4] * (gU0[index + 4] + gU0[index - 4])
                  + W[3] * (gU0[index + 3] + gU0[index - 3])
                  + W[2] * (gU0[index + 2] + gU0[index - 2])
                  + W[1] * (gU0[index + 1] + gU0[index - 1])
                  + W[0] * gU0[index]
                )
                + FATMDFY * gVorg[index] * gVorg[index] * (
                  + W[6] * (gU0[index + 6 * nnoi] + gU0[index - 6 * nnoi])
                  + W[5] * (gU0[index + 5 * nnoi] + gU0[index - 5 * nnoi])
                  + W[4] * (gU0[index + 4 * nnoi] + gU0[index - 4 * nnoi])
                  + W[3] * (gU0[index + 3 * nnoi] + gU0[index - 3 * nnoi])
                  + W[2] * (gU0[index + 2 * nnoi] + gU0[index - 2 * nnoi])
                  + W[1] * (gU0[index +     nnoi] + gU0[index -     nnoi])
                  + W[0] * gU0[index]
                )
                + FATMDFZ * gVorg[index] * gVorg[index] * (
                  + W[6] * (gU0[index + 6 * stride] + gU0[index - 6 * stride])
                  + W[5] * (gU0[index + 5 * stride] + gU0[index - 5 * stride])
                  + W[4] * (gU0[index + 4 * stride] + gU0[index - 4 * stride])
                  + W[3] * (gU0[index + 3 * stride] + gU0[index - 3 * stride])
                  + W[2] * (gU0[index + 2 * stride] + gU0[index - 2 * stride])
                  + W[1] * (gU0[index +     stride] + gU0[index -     stride])
                  + W[0] * gU0[index]
                )
                - FATMDFX * gVorg[index] * gVorg[index] * (1.0f / gDorg[index]) * (
                  + C[0] * (gDorg[index + 1] - gDorg[index - 1])
                  + C[1] * (gDorg[index + 2] - gDorg[index - 2])
                  + C[2] * (gDorg[index + 3] - gDorg[index - 3])
                  ) * (
                  + C[0] * (gU0[index + 1] - gU0[index - 1])
                  + C[1] * (gU0[index + 2] - gU0[index - 2])
                  + C[2] * (gU0[index + 3] - gU0[index - 3])
                )
                - FATMDFY * gVorg[index] * gVorg[index] * (1.0f / gDorg[index]) * (
                  + C[0] * (gDorg[index +     nnoi] - gDorg[index -     nnoi])
                  + C[1] * (gDorg[index + 2 * nnoi] - gDorg[index - 2 * nnoi])
                  + C[2] * (gDorg[index + 3 * nnoi] - gDorg[index - 3 * nnoi])
                  ) * (
                  + C[0] * (gU0[index +     nnoi] - gU0[index -     nnoi])
                  + C[1] * (gU0[index + 2 * nnoi] - gU0[index - 2 * nnoi])
                  + C[2] * (gU0[index + 3 * nnoi] - gU0[index - 3 * nnoi])
                )
                - FATMDFZ * gVorg[index] * gVorg[index] * (1.0f / gDorg[index]) * (
                  + C[0] * (gDorg[index +     stride] - gDorg[index -     stride])
                  + C[1] * (gDorg[index + 2 * stride] - gDorg[index - 2 * stride])
                  + C[2] * (gDorg[index + 3 * stride] - gDorg[index - 3 * stride])
                  ) * (
                  + C[0] * (gU0[index +     stride] - gU0[index -     stride])
                  + C[1] * (gU0[index + 2 * stride] - gU0[index - 2 * stride])
                  + C[2] * (gU0[index + 3 * stride] - gU0[index - 3 * stride])
                );
            } // end if

            //------------------------------
            // Advance Global Memory Index
            //------------------------------
            index += stride;
          } // end for k
        } // end for index_Y
}

//==============
// SET CONSTANTS
//==============
int init_cte3D(stParam_MDF *pMDF, float *FATMDFX, float *FATMDFY, float *FATMDFZ, float *W, float *C){

  if(pMDF->NPOP==6){
    switch(pMDF->type_EQ){
      case 0:
      case 1:
        *FATMDFX = ( (pMDF->dt * pMDF->dt) / (pMDF->dx * pMDF->dx) );
        *FATMDFY = ( (pMDF->dt * pMDF->dt) / (pMDF->dy * pMDF->dy) );
        *FATMDFZ = ( (pMDF->dt * pMDF->dt) / (pMDF->dz * pMDF->dz) );
        //---------------------------
        // Finite Differences Weights
        //---------------------------
        W[0] = -3.0822809296250264930;
        W[1] = +1.8019078703451239918;
        W[2] = -0.32734121207503490301;
        W[3] = +0.83457210633103019141e-1;
        W[4] = -0.20320182331671760958e-1;
        W[5] = +0.38589461566722754295e-2;
        W[6] = -0.42216791567937593928e-3;

        C[0] = +0.76775350395009123508;
        C[1] = -0.16481185505302090350;
        C[2] = +0.20648054998780843275e-1;
        break;
      default:
        fprintf(stdout, "#### ERROR ==> %s -> Unknown Equation Type; Equation type=%d \n",
          __func__, pMDF->type_EQ);
        fflush(stdout);
        return 1;
    }
  }

  return 0;
}
