/*
  ACUSTIC MODEL TEST PROCEDURE
  3D FINITE DIFFERENCES

  PETROBRAS/CENPES/GEOF	
*/
  
#ifndef _struct_3DTD_h
#define _struct_3DTD_h

#define def_NPOP_AC	6    // Arm size at Finite Differences Operator
#define def_COUNT 	500  // Dumping Timestep interval

//============
// WAVE FIELDS
//============
typedef struct{
  float	*U0;     // Full Wavefield 
  float	*U1;     // Full Wavefield
}stParam_WaveField_AC;

//=================
// PROGRAM ARGUMENT
//=================
typedef struct{
  int VERBOSE;	          // debug printing level; =0 to avoid detailed printing
  int type_EQ;	          // Equation type: 0=constant density; 1=variable density
  int nnoi_global;        // grid points at x without border
  int nnoj_global;        // grid points at y without border
  int nnok_global;        // grid points at z without border  
  int nTime;		          // number of timesteps
  float dx;               // delta x
  float dy;               // delta y
  float dz;               // delta z
  float dt;               // delta t
  int PRINT_SNAP;	        // 0 = do not dump snapshots
  int INT_SNAPSHOT;	      // timesteps among snapshot dumps
  float FC;		            // seismic source cut frequency
  float VP_def;	          // fixed velocity value
  float RHO_def;          // fixed density value
  int NPOP;		            // number of Finite Differences Operator points at one arm
  int nborda;	            // number of border points
  int NNOI;               // grid points at X
  int NNOJ;               // grid points at Y
  int NNOK;               // grid points at Z
  float sourceTf;	        // Time delay
  float TotalMemAloc_CPU; // CPU allocated memory (MB)
}stParam_MDF;

//===============
// SOURCE WAVELET
//===============
typedef struct{
  float	*WVLT;  // Wavelet
  int	n1;       // Wavelet size
}stParam_SrcWavelet;

//================
// SOURCE POSITION
//================
typedef struct{
  int	*cI_grid; // X index
  int	*cJ_grid; // Y index
  int	*cK_grid; // Z index
}stParam_SrcRcv3D;

#endif