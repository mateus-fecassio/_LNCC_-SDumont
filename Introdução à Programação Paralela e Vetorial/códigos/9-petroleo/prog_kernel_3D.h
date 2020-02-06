/*
	ACUSTIC MODEL TEST PROCEDURE
	3D FINITE DIFFERENCES

	PETROBRAS/CENPES/GEOF	
*/

#ifndef _prog_kernel_3D_h
#define _prog_kernel_3D_h

//===================================================
// ACOUSTIC MODELING FOR CONSTANT OR VARIABLE DENSITY
//===================================================
float ExecLoop_MOD_AC_3D(stParam_MDF *, stParam_SrcWavelet *, stParam_SrcRcv3D *,
		float *, float *, stParam_WaveField_AC *, FILE *);
//=========================
// WAVE FIELD EXTRAPOLATION
//=========================
int forward_wavefield_AC_3D(stParam_MDF*, stParam_SrcWavelet*, stParam_SrcRcv3D*,
		stParam_WaveField_AC *, float *, float *, FILE*);
//==========================================================================
// EXTRAPOLATE WAVEFIELD ONE TIMESTEP WITH CONSTANT DENSITY TIPO_EQUACAO = 0
//==========================================================================
void exec_mod_06_ACRhoCte3D_CPU(stParam_WaveField_AC *, float *, int, int, int,
		float, float, float, float *);
//==========================================================================
// EXTRAPOLATE WAVEFIELD ONE TIMESTEP WITH VARIABLE DENSITY TIPO_EQUACAO = 1
//==========================================================================
void exec_mod_06_ACRhoVar3D_CPU(stParam_WaveField_AC *, float *, float *, int, int, int, 
		float, float, float, float *, float *);
//=========================================
// KERNEL TO UPDATE SOURCE WAVEFIELD SAMPLE
//=========================================
void kernel_CPU_srcVet_3D(float *, float *, int, int *, int *, int *, float, float, int, int);
//===========================================================================
// KERNEL TO EXTRAPOLATE SINGLE TIME STEP - CONSTANT DENSITY TIPO_EQUACAO = 0
//===========================================================================
void kernel_CPU_06_mod_3DRhoCte(float *, float *, float *, int, int, int, int,
		float, float, float, float *);
//===========================================================================
// KERNEL TO EXTRAPOLATE SINGLE TIME STEP - VARIABLE DENSITY TIPO_EQUACAO = 1
//===========================================================================
void kernel_CPU_06_mod_3DRhoVar(float * ,float * ,float * ,float * , int, int, int, int,
		float, float, float, float *, float *);
//==============
// SET CONSTANTS
//==============
int init_cte3D(stParam_MDF *, float *, float *, float *, float *, float *);

#endif