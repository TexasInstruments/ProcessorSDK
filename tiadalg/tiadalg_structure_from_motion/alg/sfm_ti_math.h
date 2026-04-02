/*
* module name : Structure From Motion
*
* module descripton : Generates sparse 3D points from optical flow information in camera captured images
*
* Copyright (C) 2009-2017 Texas Instruments Incorporated - http://www.ti.com/
* ALL RIGHTS RESERVED
*
*/

/**
***************************************************************************
*  @file         sfm_ti_math.h
*  @author       Deepak Poddar
*  @brief        This file defines the functions related matrix operations
*  @version 0.1 (Oct 2014) : Base version.
***************************************************************************
*/

#ifndef TI_SFM_MATH_H
#define TI_SFM_MATH_H

#include "float.h"

static inline float16 VXLIB_oneByXVecF32(float16 vX);
static inline float16 VXLIB_oneBySqrtXVecF32(float16 vX);

#if HOST_EMULATION
/* float absolute*/
#define _fabsf fabs

/*double absolute*/
#define _fabs  fabs
#endif

#define DSP_INTRINSICS

#ifdef DSP_INTRINSICS
#define DSP_INTRINSIC_FOR_RECIPORCAL
//#define RCP_BOUND_CHK
#endif

#define VLIB_F32 float
#define VLIB_D64 double
#define Void void
#define VLIB_OneByXF32 my_OneByX
#define VLIB_OneByX1X0F32 my_OneByX1X0
#define VLIB_SqrtXF32 my_SqrtX

#ifndef __mmax
#define __mmax(x,y) ((x>y)?x:y)
#endif

#ifndef __mmin
#define __mmin(x,y) ((x>y)?y:x)
#endif

#ifndef SWAP_ME
#define SWAP_ME(X,Y) {temp=(X);(X)=(Y);(Y)=temp;}
#endif

#define SIGN(a, b) ((b) >= 0.0 ? _fabsf(a) : -_fabsf(a))

#ifndef bool
#define bool uint8_t
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif


#define SFM_TI_PI (3.14159265358979323846f)

static inline float my_OneByX(float x)
{
#ifdef DSP_INTRINSIC_FOR_RECIPORCAL
  float TWO = 2.0f;
  float X;

#ifdef RCP_BOUND_CHK
  float Big = FLT_MAX;
  float Y;
  Y = _fabsf(x);
#endif

  X = __recip(x);
  X = X  * (TWO - x*X);
  X = X  * (TWO - x*X);

#ifdef RCP_BOUND_CHK
  if (Y > Big) {
    X = 0.0f;
  }
  if((x == 0.0f) | (x == -0.0f))
    X = Big;
#endif

  return (X);
#else
  float den;
  den = 1/x;
  return(den);
#endif
}

static inline float my_OneBySqrtX(float x)
{
#ifdef DSP_INTRINSIC_FOR_RECIPORCAL
  float normInv, val;

  normInv   = __recip_sqrt(x);
  val       = normInv*(3.0f-x*normInv*normInv)*0.5f;
  normInv   = val*(3.0f-x*val*val)*0.5f;
#ifdef RCP_BOUND_CHK
 @TODO enable boundry check
#endif

  return (normInv);
#else
  return((1.0/sqrtf(x)));
#endif
}

static inline float16 VXLIB_oneByXVecF32(float16 vX)
{

    float16    vX0;

#ifdef DSP_INTRINSIC_FOR_RECIPORCAL
    float16    f2Pkd = (float16)2.0f;

    vX0 = __recip(vX);

    vX0 = vX0 * (f2Pkd - vX0 * vX);

    vX0 = vX0 * (f2Pkd - vX0 * vX);
#ifdef RCP_BOUND_CHK
    __vpred    vp;
    float16    vY;

    vY  = __abs(vX);

    /* If input value is higher or equal to FLT_MAX
    */
    vp  = __cmp_le_pred((float16)FLT_MAX, vY);

    vX0 = __select(vp,(float16)FLT_MIN,vX0);

    /* If input value is lesser or equal to FLT_MIN
    */
    vp  = __cmp_le_pred(vY, (float16)FLT_MIN);

    vX0 = __select(vp,(float16)FLT_MAX,vX0);
#endif
#else
    vX0.s[0] = 1.0f/vX.s[0]; vX0.s[1] = 1.0f/vX.s[1]; vX0.s[2] = 1.0f/vX.s[2]; vX0.s[3] = 1.0f/vX.s[3];
    vX0.s[4] = 1.0f/vX.s[4]; vX0.s[5] = 1.0f/vX.s[5]; vX0.s[6] = 1.0f/vX.s[6]; vX0.s[7] = 1.0f/vX.s[7];
    vX0.s[8] = 1.0f/vX.s[8]; vX0.s[9] = 1.0f/vX.s[9]; vX0.s[10] = 1.0f/vX.s[10]; vX0.s[11] = 1.0f/vX.s[11];
    vX0.s[12] = 1.0f/vX.s[12]; vX0.s[13] = 1.0f/vX.s[13]; vX0.s[14] = 1.0f/vX.s[14]; vX0.s[15] = 1.0f/vX.s[15];
#endif
    return (vX0);
}
static inline float16 VXLIB_oneBySqrtXVecF32(float16 vX)
{
    float16    vX0;
    float16    f3Pkd = (float16)3.0f;
#ifdef DSP_INTRINSIC_FOR_RECIPORCAL
    vX0      = __recip_sqrt(vX);

    vX0  = vX0 * (f3Pkd - vX * vX0 * vX0) * (float16)0.5f;
    vX0  = vX0 * (f3Pkd - vX * vX0 * vX0) * (float16)0.5f;

#ifdef RCP_BOUND_CHK
    __vpred    vp;
    float16    vY;
    
    vY       = __abs(vX);

    /* If input value is higher or equal to FLT_MAX
    */
    vp  = __cmp_le_pred((float16)FLT_MAX, vY);

    vX0 = __select(vp,(float16)FLT_MIN,vX0);

    /* If input value is lesser or equal to FLT_MIN
    */
    vp  = __cmp_le_pred(vY, (float16)FLT_MIN);

    vX0 = __select(vp,(float16)FLT_MAX,vX0);


    /* If input is negative then this is undefined scenarion.
     * For now just making output as FLT_MIN in this undefined scenarion.
     */
    vp  = __cmp_le_pred(vX, (float16)0.0f);

    vX0 = __select(vp,(float16)FLT_MIN,vX0);
#endif

#else
    vX0.s[0] = 1.0f/sqrt(vX.s[0]); vX0.s[1] = 1.0f/sqrt(vX.s[1]); vX0.s[2] = 1.0f/sqrt(vX.s[2]); vX0.s[3] = 1.0f/sqrt(vX.s[3]);
    vX0.s[4] = 1.0f/sqrt(vX.s[4]); vX0.s[5] = 1.0f/sqrt(vX.s[5]); vX0.s[6] = 1.0f/sqrt(vX.s[6]); vX0.s[7] = 1.0f/sqrt(vX.s[7]);
    vX0.s[8] = 1.0f/sqrt(vX.s[8]); vX0.s[9] = 1.0f/sqrt(vX.s[9]); vX0.s[10] = 1.0f/sqrt(vX.s[10]); vX0.s[11] = 1.0f/sqrt(vX.s[11]);
    vX0.s[12] = 1.0f/sqrt(vX.s[12]); vX0.s[13] = 1.0f/sqrt(vX.s[13]); vX0.s[14] = 1.0f/sqrt(vX.s[14]); vX0.s[15] = 1.0f/sqrt(vX.s[15]);
#endif
    return (vX0);
}
static inline float my_YByX(float y, float x)
{
  return(y*my_OneByX(x));
}

static inline float my_SqrtX(float x)
{
  return (x * my_OneBySqrtX(x));
}

static inline float16 approxAtan(float16 z)
{
    float16 n1 = (float16)0.97239411f;
    float16 n2 = (float16)-0.19194795f;
    return (n1 + n2 * z * z) * z;
}
#endif
