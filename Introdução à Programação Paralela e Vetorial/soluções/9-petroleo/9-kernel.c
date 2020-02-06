//===========================================================================
// KERNEL TO EXTRAPOLATE SINGLE TIME STEP - CONSTANT DENSITY TIPO_EQUACAO = 0
//===========================================================================
void kernel_CPU_06_mod_3DRhoCte(float* gU0, float* gU1, float* gVorg,
  int nnoi, int nnoj, int k0, int k1,
  float FATMDFX, float FATMDFY, float FATMDFZ, float *W){

  int index_X, index_Y;      // X, Y grid position
  int stride  = nnoi * nnoj; // stride to next Z
  int index, k;
	 
  for(index_X = 0; index_X < nnoi; index_X++)
    for(index_Y = 0; index_Y < nnoj; index_Y++){
      for(k = 0; k < k1 - k0; k++){
        
        index = (index_Y * nnoi + index_X) + (k0 + k) * stride;
        
        //------------------------------------------------
        // Wavefield Extrapolation Only on Interior Points
        //------------------------------------------------
        if(gVorg[index] > 0.0f){
          gU1[index] = 2.0f * gU0[index] - gU1[index]
            + FATMDFX * gVorg[index] * gVorg[index] * (
              + W[6] * (gU0[index - 6] + gU0[index + 6])
              + W[5] * (gU0[index - 5] + gU0[index + 5])
              + W[4] * (gU0[index - 4] + gU0[index + 4])
              + W[3] * (gU0[index - 3] + gU0[index + 3])
              + W[2] * (gU0[index - 2] + gU0[index + 2])
              + W[1] * (gU0[index - 1] + gU0[index + 1])
              + W[0] * gU0[index]
            )
            + FATMDFY * gVorg[index] * gVorg[index] * (
              + W[6] * (gU0[index - 6 * nnoi] + gU0[index + 6 * nnoi])
              + W[5] * (gU0[index - 5 * nnoi] + gU0[index + 5 * nnoi])
              + W[4] * (gU0[index - 4 * nnoi] + gU0[index + 4 * nnoi])
              + W[3] * (gU0[index - 3 * nnoi] + gU0[index + 3 * nnoi])
              + W[2] * (gU0[index - 2 * nnoi] + gU0[index + 2 * nnoi])
              + W[1] * (gU0[index -     nnoi] + gU0[index +     nnoi])
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
            );
        } // end if
      } // end for k
    }
      
} // end function
