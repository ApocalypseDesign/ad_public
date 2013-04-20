#ifndef _TRIMACRO_H_
#define _TRIMACRO_H_

// ###################################################################
// ##########  MACRO PER IL DISEGNO DEI TRIANGOLI (filler)  ##########
// ###################################################################

#define YSORTING_VERTEX \
  myv[0]=v1;  \
  myv[1]=v2;  \
  myv[2]=v3;  \
  if (myv[1]->ys < myv[0]->ys)  \
  {          \
    myv[0]=myv[1]; \
    myv[1]=v1; \
  }       \
  if (myv[2]->ys < myv[0]->ys) \
  {         \
    myv[2]=myv[1];  \
    myv[1]=myv[0];  \
    myv[0]=v3; \
  } else if (v3->ys < myv[1]->ys) \
         {    \
           myv[2]=myv[1];  \
           myv[1]=v3; \
         } \

#define CLIP_2D \
   if (myv[2]->ys<0) return; \
   if ((FP_BITS(myv[0]->xs)) & (FP_BITS(myv[1]->xs)) & (FP_BITS(myv[2]->xs)) & 0x80000000) return; \
   if ((myv[0]->xs>scrX) && (myv[1]->xs>scrX) && (myv[2]->xs>scrX)) return; \




#define CALC_LONGEST_SCANLINE \
   temp_scan=(myv[1]->ys - myv[0]->ys)/(myv[2]->ys - myv[0]->ys); \
   scan=temp_scan*(myv[2]->xs - myv[0]->xs) + (myv[0]->xs - myv[1]->xs); \

/*  
if ((scan>0) && (scan<1)) scan=1; \
   if ((scan<0) && (scan>-1)) scan=-1; \
*/

#define CONSTANT_SLOPE_Z \
  scan=1.0f/scan; \
  dz=scan*(temp_scan*(myv[2]->dist-myv[0]->dist)+(myv[0]->dist-myv[1]->dist)); \

#define CONSTANT_SLOPE_TEXTURE \
  du=myfist(scan*(temp_scan*(myv[2]->u-myv[0]->u)+(myv[0]->u-myv[1]->u))); \
  dv=myfist(scan*(temp_scan*(myv[2]->v-myv[0]->v)+(myv[0]->v-myv[1]->v))); \

#define CONSTANT_SLOPE_ENVMAP \
  envdu=myfist(scan*(temp_scan*(myv[2]->envu-myv[0]->envu)+(myv[0]->envu-myv[1]->envu))); \
  envdv=myfist(scan*(temp_scan*(myv[2]->envv-myv[0]->envv)+(myv[0]->envv-myv[1]->envv))); \

#define CONSTANT_SLOPE_RGB \
  dR=myfist(scan*(temp_scan*(myv[2]->R-myv[0]->R)+(myv[0]->R-myv[1]->R))); \
  dG=myfist(scan*(temp_scan*(myv[2]->G-myv[0]->G)+(myv[0]->G-myv[1]->G))); \
  dB=myfist(scan*(temp_scan*(myv[2]->B-myv[0]->B)+(myv[0]->B-myv[1]->B))); \


#endif