#include "adpcm.h" 
/* Intel ADPCM step variation table */
static int indexTable[16] = {
    -1, -1, -1, -1, 2, 4, 6, 8,
    -1, -1, -1, -1, 2, 4, 6, 8,
};
static int stepsizeTable[89] = {
    7, 8, 9, 10, 11, 12, 13, 14, 16, 17,
    19, 21, 23, 25, 28, 31, 34, 37, 41, 45,
    50, 55, 60, 66, 73, 80, 88, 97, 107, 118,
    130, 143, 157, 173, 190, 209, 230, 253, 279, 307,
    337, 371, 408, 449, 494, 544, 598, 658, 724, 796,
    876, 963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066,
    2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358,
    5894, 6484, 7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899,
    15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767
};
void adpcm_decoder(char *inbuff,char *outbuff,int len_of_in,struct adpcm_state * state )
{
 int  i=0,j=0;
    char tmp_data;
    struct adpcm_state * tmp_state =state;
    long step; /* Quantizer step size */
    signed long predsample; /* Output of ADPCM predictor */
    signed long diffq; /* Dequantized predicted difference */
    int index; /* Index into step size table */
    int Samp;
    unsigned char SampH, SampL;
    unsigned char inCode; 
 
    /* Restore previous values of predicted sample and quantizer step
    size index
    */
    predsample = state->valprev;
    index = state->index;
 
    for(i=0;i<len_of_in*2;i++)
 {
  tmp_data=inbuff[i/2];  
  if(i%2)
   inCode=(tmp_data& 0xf0) >> 4;
  else
   inCode=tmp_data & 0x0f;
    
  step = stepsizeTable[index];
   /* Inverse quantize the ADPCM code into a predicted difference
    using the quantizer step size
   */
    
  diffq = step >> 3;
  if( inCode & 4 )
   diffq += step;
  if( inCode & 2 )
   diffq += step >> 1;
  if( inCode & 1 )
   diffq += step >> 2;
    /* Fixed predictor computes new predicted sample by adding the
    old predicted sample to predicted difference
    */
  if( inCode & 8 )
   predsample -= diffq;
  else
   predsample += diffq;
    /* Check for overflow of the new predicted sample
    */
   if( predsample > 32767 )
   predsample = 32767;
   else if( predsample < -32768 )
   predsample = -32768;
    /* Find new quantizer stepsize index by adding the old index
    to a table lookup using the ADPCM code
    */
   index += indexTable[inCode];
    /* Check for overflow of the new quantizer step size index
    */
  if( index < 0 )
   index = 0;
  if( index > 88 )
   index = 88;
    /* Return the new ADPCM code */
  Samp=predsample;
  if (Samp >= 0)
  {
   SampH=Samp / 256;
   SampL = Samp - 256 * SampH;
  }
  else
  {
   Samp= 32768+Samp;
   SampH = Samp / 256;
   SampL = Samp - 256 * SampH;
   SampH += 0x80;
  }
  outbuff[j++]=SampL;
  outbuff[j++]=SampH;   
 }
 
 /* Save the predicted sample and quantizer step size index for
 next iteration
 */
 state->valprev = (short)predsample;
 state->index = (char)index;
}