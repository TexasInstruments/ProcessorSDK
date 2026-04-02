/*
*
* Copyright (c) {2015 - 2020} Texas Instruments Incorporated
*
* All rights reserved not granted herein.
*
* Limited License.
*
* Texas Instruments Incorporated grants a world-wide, royalty-free, non-exclusive
* license under copyrights and patents it now or hereafter owns or controls to make,
* have made, use, import, offer to sell and sell ("Utilize") this software subject to the
* terms herein.  With respect to the foregoing patent license, such license is granted
* solely to the extent that any such patent is necessary to Utilize the software alone.
* The patent license shall not apply to any combinations which include this software,
* other than combinations with devices manufactured by or for TI ("TI Devices").
* No hardware patent is licensed hereunder.
*
* Redistributions must preserve existing copyright notices and reproduce this license
* (including the above copyright notice and the disclaimer and (if applicable) source
* code license limitations below) in the documentation and/or other materials provided
* with the distribution
*
* Redistribution and use in binary form, without modification, are permitted provided
* that the following conditions are met:
*
* *       No reverse engineering, decompilation, or disassembly of this software is
* permitted with respect to any software provided in binary form.
*
* *       any redistribution and use are licensed by TI for use only with TI Devices.
*
* *       Nothing shall obligate TI to provide you with source code for the software
* licensed and provided to you in object code.
*
* If software source code is provided to you, modification and redistribution of the
* source code are permitted provided that the following conditions are met:
*
* *       any redistribution and use of the source code, including any resulting derivative
* works, are licensed by TI for use only with TI Devices.
*
* *       any redistribution and use of any object code compiled from the source code
* and any resulting derivative works, are licensed by TI for use only with TI Devices.
*
* Neither the name of Texas Instruments Incorporated nor the names of its suppliers
*
* may be used to endorse or promote products derived from this software without
* specific prior written permission.
*
* DISCLAIMER.
*
* THIS SOFTWARE IS PROVIDED BY TI AND TI'S LICENSORS "AS IS" AND ANY EXPRESS
* OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL TI AND TI'S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
* OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/

/**
 ----------------------------------------------------------------------------
 @file    tidl_mathlib_utils.h
 @brief   This file defines the utilities based on mathlib library 
 @version 0.1 (Sep 2024) : Initial Version
 ----------------------------------------------------------------------------
*/


#ifndef ITIDL_MATHLIB_UTILS_H
#define ITIDL_MATHLIB_UTILS_H

#include <stdint.h>
#include "itidl_ti.h"
#include "tidl_alg_int.h"
using namespace c7x;
#include <c7x_scalable.h>

// the following functions are prefixed c6x to represent the utility functions based on dsp specific calls
static inline uint32_t c6x_clr(uint32_t s1, uint32_t s2, uint32_t s3)
{
    uint64_t e_s2 = (((uint64_t) s2) << 59) >> 54;
    uint64_t e_s3 = (((uint64_t) s3) << 59) >> 59;
    uint64_t tmp  = e_s2 | e_s3;
    return __as_uint(__clear(s1, (uint32_t) tmp));
}

static inline uint32_t c6x_hi(float64_tidl s1){
  return __as_uint2(s1).s[1];
}
static inline uint32_t c6x_lo(float64_tidl s1){
  return __as_uint2(s1).s[0];
}

static inline float64_tidl c6x_itod(uint32_t s1, uint32_t s2)
{
    return __as_double(__uint2(s2, s1));
}

static inline uint32_t c6x_extu(uint32_t s1, uint32_t s2, uint32_t s3)
{
    return (s1 << s2) >> s3;
}

static inline float32_tidl __u32_bits_as_f32(uint32_t x) {
    const union { float32_tidl f; uint32_t i; } rep = {.i = x};
    return rep.f;
}
static inline uint32_t __f32_bits_as_u32(float32_tidl x) {
    const union { float32_tidl f; uint32_t i; } rep = {.f = x};
    return rep.i;
}
static inline float32_tidl c6x_itof(uint32_t x)
{
   return __u32_bits_as_f32(x);
}

static inline uint32_t c6x_ftoi(float32_tidl x)
{
   return __f32_bits_as_u32(x);
}

const float64_tidl MATHLIB_kTable[4] = {
    1.000000000, /* 2^(0/4) */
    1.189207115, /* 2^(1/4) */
    1.414213562, /* 2^(2/4) */
    1.681792831  /* 2^(3/4) */
};

const float64_tidl MATHLIB_jTable[4] = {
    1.000000000, /* 2^(0/16) */
    1.044273782, /* 2^(1/16) */
    1.090507733, /* 2^(2/16) */
    1.138788635  /* 2^(3/16) */
};

const float64_tidl MATHLIB_logTable[8] = {0.0000000000,  -0.1177830356, -0.2231435513, -0.3184537311,
                                    -0.4054651081, -0.4855078157, -0.5596157879, -0.6286086594};


static inline float32_tidl sqrtsp_i(float32_tidl x)
{
   const float32_tidl half  = 0.5f;
   const float32_tidl OneP5 = 1.5f;
   float32_tidl       y, y0, y1, y2, x_half;

   x_half = x * half;
   // debug test: valgrind error
   //  ==============================================
   float32_tidl recip_result = __recip_sqrt(x);
   float_vec y0_vec = (float_vec)recip_result;
   y0                    = y0_vec.s[0];
   /* x0     = _rsqrsp(a); */
   // ===============================================


   y1 = OneP5 - (y0 * y0 * x_half);
   y1 = y0 * y1;
   y2 = y1 * (OneP5 - (y1 * y1 * x_half));
   y  = x * y2;

   if (x <= 0.0f) {
      y = 0.0f;
   }

   return y;
}

static inline float32_tidl sqrtsp_asinhsp_i(float32_tidl a, float32_tidl x)
{
   const float32_tidl half  = 0.5f;
   const float32_tidl OneP5 = 1.5f;
   float32_tidl       x0, x1, x2, res, a_half;

   a_half = a * half;
   // debug test: valgrind error
   //  ==============================================
   float_vec a_vec  = (float_vec)0;
   a_vec                 = (float_vec) a;
   float_vec x0_vec = __recip_sqrt(a_vec);
   x0                    = x0_vec.s[0];
   /* x0     = _rsqrsp(a); */
   // ===============================================

   x1  = OneP5 - (a_half * x0 * x0);
   x1  = x0 * x1;
   x2  = x1 * (OneP5 - (x1 * x1 * a_half));
   res = a * x2;

   if (a == (x * x)) {
      res = x;
   }

   return res;
} /* End of sqrtsp_asinhsp_i */

//Polynomial calculation for asin, acos and tanh
static inline float32_tidl pol_est_i(float32_tidl x, const float32_tidl c2, const float32_tidl c4, const float32_tidl c6, 
                              const float32_tidl c8, const float32_tidl c10, const float32_tidl c12, const float32_tidl c14, const float32_tidl c16)
{

   float32_tidl x2, x4, x6, x8, x10, x12;
   float32_tidl pol, tmp1, tmp2;

   /* calculate the powers of x */
   x2  = x * x;
   x4  = x2 * x2;
   x6  = x2 * x4;
   x8  = x4 * x4;
   x10 = x6 * x4;
   x12 = x8 * x4;

   /* ====================================================================== */
   /* The polynomial calculation is done in two seperate parts.              */
   /*   tmp1 =  c2 x^2 + c4 x^4 + c6 x^6 + c8 x^8                            */
   /*   tmp2 =  c10 x^10 + c12 x^12 + c14 x^14 + c16 x^16                    */
   /* In order to reduce the number of multiplications x is factored out of  */
   /* the polynomial and multiplied by later.                                */
   /* ====================================================================== */

   tmp1 = ((c8 * x8) + (c6 * x6)) + ((c4 * x4) + (c2 * x2));
   tmp2 = ((((c16 * x4) + (c14 * x2)) + c12) * x12) + (c10 * x10);

   pol = tmp1 + tmp2;
   pol = (pol * x) + x;

   return pol;
}

static inline float32_tidl divsp_i(float32_tidl a, float32_tidl b)
{
   const float32_tidl two = 2.0f;
   float32_tidl       x1, x2, y;

   x1 = __recip(b);
   x1 = x1 * (two - (b * x1));
   x2 = x1 * (two - (b * x1));
   y  = a * x2;

   return (y);
}

// Exp (exponential) activation 
inline float32_tidl MATHLIB_exp(float32_tidl x)
{
   const float32_tidl  log2_base_x16 = 23.083120654f; /*1.442695041 * 16*/
   const float32_tidl  half          = 0.5f;
   const float32_tidl  LnMin         = -87.33654475f;
   const float32_tidl  LnMax         = 88.72283905f;
   const float32_tidl  Max           = 3.402823466E+38f;
   const float64_tidl p             = 0.0433216987816623; /* 1/log2_base_x16 */

   /* coefficients to approximate the decimal part of the result */
   const float32_tidl c0 = 0.166668549286041f;
   const float32_tidl c1 = 0.500016170012920f;
   const float32_tidl c2 = 0.999999998618401f;

   float32_tidl        pol, r, r2, r3, res;
   uint32_t Ttemp, J, K;
   float32_tidl        Nf;
   int32_t          N;
   float64_tidl       dT;

   /* Get N such that |N - x*16/ln(2)| is minimized */
   Nf = (x * log2_base_x16) + half;
   N  = (int32_t) Nf; /* Cast from intermediate variable to appease MISRA */

   if ((x * log2_base_x16) < -half) {
      N--;
   }

   /* Argument reduction, r, and polynomial approximation pol(r) */
   r  = (float32_tidl) ((float64_tidl) x - (p * (float64_tidl) N));
   r2 = r * r;
   r3 = r * r2;

   pol = (r * c2) + ((r3 * c0) + (r2 * c1));

   /* Get index for ktable and jtable */
   K  = c6x_extu((uint32_t) N, 28u, 30u);
   J  = (uint32_t) N & 0x3u;
   dT = MATHLIB_kTable[K] * MATHLIB_jTable[J];

   /* Scale exponent to adjust for 2^M */
   Ttemp = c6x_hi(dT) + (((uint32_t) N >> 4) << 20);
   dT    = c6x_itod(Ttemp, c6x_lo(dT));
   res   = (float32_tidl) dT * (1.0f + pol);

   /* < LnMin returns 0 */
   if (x < LnMin) {
      res = 0.0f;
   }

   /* > LnMax returns MAX */
   if (x > LnMax) {
      res = Max;
   }

   return (res);
}

static inline float32_tidl exp_tanhsp_i(float32_tidl x)
{
   const float32_tidl  log2_base_x16 = 23.0831206542234f; /* 1.442695041 * 16.0 */
   const float32_tidl  Half          = 0.5f;
   const float32_tidl  c0            = 0.166668549286041f;
   const float32_tidl  c1            = 0.500016170012920f;
   const float32_tidl  c2            = 0.999999998618401f;
   const float64_tidl p             = 0.0433216987816623;
   float32_tidl        pol, r, r2, r3, res;
   uint32_t Ttemp, J, K;
   float32_tidl        Nf;
   int32_t          N;
   float64_tidl       dT;

   /* Get N such that |N - x*16/ln(2)| is minimized */
   Nf = (x * log2_base_x16) + Half;
   N  = (int32_t) Nf; /* Cast from intermediate variable to appease MISRA */

   /* Commenting condition for coverage- Condition is always TRUE */
   /* if ((x * log2_base_x16) < -Half) */ {
      N--;
   }

   /* Argument reduction, r, and polynomial approximation pol(r) */
   r  = (float32_tidl) ((float64_tidl) x - (p * (float64_tidl) N));
   r2 = r * r;
   r3 = r * r2;

   pol = (r * c2) + ((r3 * c0) + (r2 * c1));

   /* Get index for ktable and jtable */
   K  = c6x_extu((uint32_t) N, 28u, 30u);
   J  = (uint32_t) N & 0x3u;
   dT = MATHLIB_kTable[K] * MATHLIB_jTable[J];

   /* Scale exponent to adjust for 2^M */
   Ttemp = c6x_hi(dT) + (((uint32_t) N >> 4) << 20);
   dT    = c6x_itod(Ttemp, c6x_lo(dT));

   res = (float32_tidl) (dT * (1.0 + (float64_tidl) pol));

   return (res);
}


static inline float32_tidl MATHLIB_tanh(float32_tidl x)
{
   /* coefficients for the polynomial */
   const float32_tidl coeff_16 = 0.000244528812992865f;
   const float32_tidl coeff_14 = -0.00119005741172407f;
   const float32_tidl coeff_12 = 0.00349212803657248f;
   const float32_tidl coeff_10 = -0.00886323552990220f;
   const float32_tidl coeff_8  = 0.0218794885361552f;
   const float32_tidl coeff_6  = -0.0539682539682540f;
   const float32_tidl coeff_4  = 0.133333333333333f;
   const float32_tidl coeff_2  = -0.333333333333333f;

   const float32_tidl maxTanh   = 1.0f;
   const float32_tidl limit     = 9.0f;
   const float32_tidl negTwo    = -2.0f;
   const float32_tidl pol_bound = 1.0f;
   float32_tidl       sign      = 1.0f;
   float32_tidl       res, x_abs, temp;

   x_abs = __abs(x);

   /*res = maxTanh;*/ /* assume tanh reaches max value */

   if (x < 0.0f) {
      sign = -sign;
   }

   if (x_abs < pol_bound) {          /* |x| < 1.0 */
      res = pol_est_i(x_abs, coeff_2, coeff_4, coeff_6, coeff_8, coeff_10, coeff_12, coeff_14, coeff_16); /* estimation for very small input */
   }
   else if (x_abs <= limit) {              /* 1.0 < |x|<= 9 */
      temp = exp_tanhsp_i(negTwo * x_abs); /* e^(-2x) */
      temp++;
      res = divsp_i(2.0f, temp) - 1.0f; /* -1 + 2 / (1 + e^(-2x)) */
   }
   else { /* |x| > 9 */
      res = maxTanh;
   }

   return (res * sign); /* restore sign */

} 

// Log (natural logarithm) activation
static inline float32_tidl MATHLIB_ln(float32_tidl x) 
{
  float64_tidl       ln2  = 0.693147180559945f;
  float32_tidl        c1   = -0.2302894f;
  float32_tidl        c2   = 0.1908169f;
  float32_tidl        c3   = -0.2505905f;
  float32_tidl        c4   = 0.3333164f;
  float32_tidl        c5   = -0.5000002f;
  float32_tidl        MAXe = 3.402823466E+38f;
  float32_tidl        pol, r1, r2, r3, r4, res;
  float64_tidl       dr, frcpax, rcp, T;
  uint32_t T_index;
  int32_t          N; 


  rcp = __recip((float64_tidl) x);
  frcpax = c6x_itod(c6x_clr(c6x_hi(rcp), 0u, 16u), 0u);
  dr = (frcpax * (float64_tidl) x) - 1.0;

  r1 = (float32_tidl) dr;
  r2 = r1 * r1;
  r3 = r1 * r2;
  r4 = r2 * r2;

  pol = (c5 * r2) + ((c4 * r3) + ((((c2 * r1) + c3) + (c1 * r2)) * r4));

  N       = (int32_t) c6x_extu(c6x_hi(frcpax), 1u, 21u) - 1023;
  T_index = c6x_extu(c6x_hi(frcpax), 12u, 29u);
  T       = MATHLIB_logTable[T_index] - (ln2 * (float64_tidl) N);
  res     = (float32_tidl) (dr + T) + pol;

  if (x <= 0.0f) {
    res = __as_float(0xFF800000u);
  }
  if (x > MAXe) {
    res = 709.7827f;
  }

  return (res);
}

// Mish activation
static inline float32_tidl MATHLIB_mish(float32_tidl x){
  float32_tidl ex = 1.0f + MATHLIB_exp(x);
  ex = MATHLIB_ln(ex);
  ex = MATHLIB_tanh(ex);
  return x * ex;
}

// Asin activation
static inline float32_tidl rsqrt( float32_tidl number ) 
{ 
 const float32_tidl threehalfs = 1.5F; 
 
 float32_tidl x2 = number * 0.5F; 
 float32_tidl y = number; 
  
 uint64_t i = * ( int64_t * ) &y; 
 i = 0x5f3759dfU - ( i >> 1U ); 
 y = * ( float32_tidl * ) &i; 
 
 y = y * ( threehalfs - ( x2 * y * y ) ); 

 return y; 
} 

static inline float32_tidl MATHLIB_asin(float32_tidl x)
{
   /* coefficients for the polynomial for asin(x) */
   const float32_tidl coeff_16 = 0.053002771381990f;
   const float32_tidl coeff_14 = -0.010980624698693f;
   const float32_tidl coeff_12 = 0.020659425186833f;
   const float32_tidl coeff_10 = 0.022862784546374f;
   const float32_tidl coeff_8  = 0.030636056280974f;
   const float32_tidl coeff_6  = 0.044450959710588f;
   const float32_tidl coeff_4  = 0.075034659380970f;
   const float32_tidl coeff_2  = 0.166664771293503f;

   const float32_tidl pi2   = 1.570796327f; /* pi/2 */
   const float32_tidl rsqr2 = 0.7071067811f;
   float32_tidl       s     = 1.0f;
   float32_tidl       res, x_abs, a, temp;
   float32_tidl ret_val = 0.0f;

   x_abs = fabsf(x);

    if(x_abs > 1.0f)
    {
     ret_val = FLT_MAX;
    }

   if (ret_val != FLT_MAX)
   {
   if (x_abs > rsqr2) { /* |x| > 1/sqrt(2) */
      temp = 1.0f - (x_abs * x_abs);
      a    = sqrtsp_i(temp); /* a= sqrt(1 - x^2) */
      temp = pol_est_i(a, coeff_2, coeff_4, coeff_6, coeff_8, coeff_10, coeff_12, coeff_14, coeff_16);
      res  = pi2 - temp;
   }
   else { /* |x| <= 1/sqrt(2) */
      res = pol_est_i(x_abs, coeff_2, coeff_4, coeff_6, coeff_8, coeff_10, coeff_12, coeff_14, coeff_16);
   }

   if (x < 0.0f) {
      s = -s; /* sign var */
   }
     ret_val = res * s;
   }

   return (ret_val); /* restore sign for quadrant 3 & 4*/
}

static inline float32_tidl MATHLIB_acos(float32_tidl x)
{
   /* coefficients for the polynomial for asin(x) */
   const float32_tidl coeff_16 = 0.053002771381990f;
   const float32_tidl coeff_14 = -0.010980624698693f;
   const float32_tidl coeff_12 = 0.020659425186833f;
   const float32_tidl coeff_10 = 0.022862784546374f;
   const float32_tidl coeff_8  = 0.030636056280974f;
   const float32_tidl coeff_6  = 0.044450959710588f;
   const float32_tidl coeff_4  = 0.075034659380970f;
   const float32_tidl coeff_2  = 0.166664771293503f;

   const float32_tidl pi2   = 1.570796327f;  /* pi/2 */
   const float32_tidl rsqr2 = 0.7071067811f; /* 1/sqrt(2) */
   float32_tidl       s     = 1.0f;
   float32_tidl       res, x_abs, a, temp;

   x_abs = fabsf(x);

   if (x_abs > 1.0f) {
      res = c6x_itof(0x7FFFFFFFu); /* NaN */
   }

   if (x_abs > rsqr2) { /* |x| > 1/sqrt(2) */
      temp = 1.0f - (x_abs * x_abs);
      a    = sqrtsp_i(temp); /* a = sqrt(1 - x^2) */
      temp = pol_est_i(a, coeff_2, coeff_4, coeff_6, coeff_8, coeff_10, coeff_12, coeff_14, coeff_16);
      res  = pi2 - temp;
   }
   else { /* |x| <= 1/sqrt(2) */
      res = pol_est_i(x_abs, coeff_2, coeff_4, coeff_6, coeff_8, coeff_10, coeff_12, coeff_14, coeff_16);
   }

   if (x < 0.0f) {
      s = -s; /* sign var */
   }

   res = pi2 - (res * s); /* restore sign for quadrants 2 & 3*/

   return (res);
}
\
//Polynomial calculation for Atan
static inline float32_tidl pol_est_atanspi(float32_tidl g1, bool s, bool an)
{
   float32_tidl coef;
   float32_tidl g2;
   float32_tidl g4;
   float32_tidl g6;
   float32_tidl g8;
   float32_tidl g10;
   float32_tidl g12;
   float32_tidl pol;
   float32_tidl tmp1;
   float32_tidl tmp2;
   float32_tidl pih = 1.57079632679f;
   float32_tidl c1  = 0.00230158202f;
   float32_tidl c2  = -0.01394551000f;
   float32_tidl c3  = 0.03937087815f;
   float32_tidl c4  = -0.07235669163f;
   float32_tidl c5  = 0.10521499322f;
   float32_tidl c6  = -0.14175076797f;
   float32_tidl c7  = 0.19989300877f;
   float32_tidl c8  = -0.33332930041f;

   coef = pih;
   /* get coef based on the flags */

   /* check 'swap' flag */
   if (s == FALSE) {
      coef = 0.0f;
   }

   /* check if input to atansp_i is negative  */
   if (an != FALSE) { /* MISRA requires explicit checks, != FALSE is faster than == TRUE*/
      coef = -coef;
   }

   /* calculate polynomial */
   g2  = g1 * g1;
   g4  = g2 * g2;
   g6  = g2 * g4;
   g8  = g4 * g4;
   g10 = g6 * g4;
   g12 = g8 * g4;

   tmp1 = ((c5 * g8) + (c6 * g6)) + ((c7 * g4) + (c8 * g2));
   tmp2 = ((((c1 * g4) + (c2 * g2)) + c3) * g12) + (c4 * g10);

   pol = tmp1 + tmp2;
   pol = (pol * g1) + g1;

   /* MISRA requires explicit checks, != FALSE is faster than == TRUE*/
   return ((s != FALSE) ? (coef - pol) : (coef + pol));
}

static inline float32_tidl MATHLIB_atan(float32_tidl a)
{
   float32_tidl g;
   float32_tidl res;
   float32_tidl temp = 1.0f;

   bool an;
   bool s = FALSE;

   an = (a < 0.0f) ? TRUE : FALSE; /* flag for a negative */

   /* swap a and b before calling division sub routine if a > b */
   if (fabsf(a) > 1.0f) {
      temp = a;
      a    = 1.0f;
      s    = TRUE; /* swap flag */
   }

   g = divsp_i(a, temp);

   /* do polynomial estimation */
   res = pol_est_atanspi(g, s, an);

   if (a == 0.0f) {
      res = 0.0f;
   }

   return (res);
}

// Asinh activation
static inline float32_tidl logsp_asinhsp_i(float32_tidl x)
{
   /* coeffincients for the polynomial p(r) */
   const float32_tidl c1 = -0.2302894f;
   const float32_tidl c2 = 0.1908169f;
   const float32_tidl c3 = -0.2505905f;
   const float32_tidl c4 = 0.3333164f;
   const float32_tidl c5 = -0.5000002f;

   const float64_tidl ln2 = 0.693147180559945;
   const float32_tidl  max = 88.7228390519551f;
   float32_tidl        pol, r1, r2, r3, r4, res;
   float64_tidl       dr, frcpax, rcp, T;
   uint32_t T_index;
   int32_t          N;

   /* r = x * frcpa(x) -1 */
   // debug test: valgrind error
   // ===============================================
   double_vec x_vec   = (double_vec)(0);
   x_vec                   = (double_vec) x;
   double_vec rcp_vec = __recip(x_vec);
   rcp                     = rcp_vec.s[0];
   /* rcp    = _rcpdp((float64_tidl) x); */
   // ===============================================
   frcpax = c6x_itod(c6x_clr(c6x_hi(rcp), 0u, 16u), 0u);
   dr     = (frcpax * (float64_tidl) x) - 1.0;

   /* calculate powers of r */
   r1 = (float32_tidl) dr;
   r2 = r1 * r1;
   r3 = r1 * r2;
   r4 = r2 * r2;

   /* Polynomial p(r) that approximates ln(1+r) - r */
   pol = (c5 * r2) + (c4 * r3) + ((((c2 * r1) + c3) + (c1 * r2)) * r4);

   /* Reconstruction: result = T + r + p(r) */
   N       = (int32_t) c6x_extu(c6x_hi(frcpax), 1u, 21u) - 1023;
   T_index = c6x_extu(c6x_hi(frcpax), 12u, 29u);
   T       = MATHLIB_logTable[T_index] - (ln2 * (float64_tidl) N);
   res     = (float32_tidl) (dr + T) + pol;

   if (x > FLT_MAX) {
      res = max;
   }

   return (res);
} /* logsp_asinhsp_i */


static inline float32_tidl MATHLIB_asinh(float32_tidl x)
{
   const float32_tidl ln2       = 0.69314718056f; /* ln(2) */
   const float32_tidl pol_bound = 0.5f;
   const float32_tidl half      = 0.5f;
   float32_tidl       sign      = 1.0f;

   /* Coefficients for the polynomial */
   const float32_tidl c2 = -0.166605362341955f;
   const float32_tidl c4 = 0.0734464812833510f;
   const float32_tidl c6 = -0.0330279320352987f;

   float32_tidl res, sqrt_, pol;
   float32_tidl x2, x4, x6, x_abs;
   float32_tidl temp;

   // debug test: valgrind error
   //======================================================
   /* x_abs                    = _fabsf(x); /\* |x| *\/ */
   float_vec x_vec     = (float_vec)0;
   x_vec                    = (float_vec) (x);
   float_vec x_abs_vec = __abs(x_vec);
   x_abs                    = x_abs_vec.s[0];
   x2                       = x * x; /* x^2 */
   //======================================================

   if (x < 0.0f) {
      sign = -sign;
   }

   /* powers of x */
   x4 = x2 * x2;
   x6 = x4 * x2;

   /* use polynomial estimation for |x| <= 0.5 */
   /* polynomial to estimate asinh(x) for small inputs*/
   pol = (x2 * c2) + (x4 * c4) + (x6 * c6);
   pol = (pol * x) + x;

   /* assume |x| > 0.5 and calculate asinh(x) */
   sqrt_ = sqrtsp_asinhsp_i(x2 + 1.0f, x_abs); /* sqrt(x^2 + 1) */

   /* prevent overflow for large x, log(2x) where x > max/2 */
   temp = (sqrt_ * half) + (x_abs * half); /* (x+sqrt(x^2 + 1))/2 */
   res  = logsp_asinhsp_i(temp) + ln2;     /* ln((x + sqrt(x^2 + 1))/2) +ln(2) */
   res  = res * sign;                      /* restore sign */

   /* set the result to the right value depending on the input value */
   if (x_abs <= pol_bound) {
      res = pol;
   }

   return res;
}

static inline float32_tidl MATHLIB_sin(float32_tidl a)
{
   float32_tidl InvPI = 0.318309886183791f;
   float32_tidl One   = 1.0f;
   float32_tidl MAX   = 1048576.0f;
   float32_tidl Zero  = 0.0f;
   float32_tidl s1    = -1.666665668e-1f;
   float32_tidl s2    = 8.333025139e-3f;
   float32_tidl s3    = -1.980741872e-4f;
   float32_tidl s4    = 2.601903036e-6f;
   float32_tidl C1    = 3.140625f;
   float32_tidl C2    = 9.67653589793e-4f;
   float32_tidl Sign, X, Y, Z, F, G, R;
   int32_t   N;

   Sign = One;
   Y    = a;
   /* LDRA_JUSTIFY_START
   <metric start> statement branch <metric end>
   <justification start> SAFETY_CHECK: Safe programming hard to hit this condition with real world data.
   <justification end> */
   if (fabsf(Y) > MAX) {
      Y = Zero;
   }
   /* LDRA_JUSTIFY_END */

   X = Y * InvPI; /* X = Y * (1/PI)  */
   N = float_to_int_c7x(X);
   // N = _spint(X); /* N = integer part of X  */
   Z = (float32_tidl) N;

   if ((N % 2) != 0) {
      Sign = -Sign; /* Quadrant 3 or 4 */
   }

   F = (Y - (Z * C1)) - (Z * C2);
   G = F * F;
   R = ((((((s4 * G) + s3) * G) + s2) * G) + s1) * G;

   return ((F + (F * R)) * Sign);
}

static inline float32_tidl MATHLIB_cos(float32_tidl a)
{
   float32_tidl result = 0.0f;
   float32_tidl InvPI  = 0.318309886183791f;
   float32_tidl HalfPI = 1.5707963268f;
   float32_tidl One    = 1.0f;
   float32_tidl MAX    = 1048576.0f;
   float32_tidl MIN    = 2.4414062e-4f;
   float32_tidl Zero   = 0.0f;

   float32_tidl s1    = -1.666665668e-1f;
   float32_tidl s2    = 8.333025139e-3f;
   float32_tidl s3    = -1.980741872e-4f;
   float32_tidl s4    = 2.601903036e-6f;
   float32_tidl C1    = 3.140625f;
   float32_tidl C2    = 9.67653589793e-4f;

   float32_tidl Sign, X, Y, Z, F, G, R;
   int32_t   N;

   Sign = One;
   Y    = fabsf(a) + HalfPI;

   /* LDRA_JUSTIFY_START
   <metric start> statement branch <metric end>
   <justification start> SAFETY_CHECK: Safe programming hard to hit this condition with real world data.
   <justification end> */
   if (Y > MAX) {
      Y = HalfPI;
   }
   /* LDRA_JUSTIFY_END */

   X = Y * InvPI; /* X = Y * (1/PI)  */
   N = float_to_int_c7x(X);
   Z = (float32_tidl) N;

   if ((N % 2) != 0) {
      Sign = -Sign; /* Quadrant 3 or 4 */
   }

   F = (Y - (Z * C1)) - (Z * C2);
   R = F;

   if(F < Zero){
      R = -R;
   }

   if (R < MIN)
   {
      result = (R * Sign);
   }
   else
   {
      G = F * F;
      R = ((((((s4 * G) + s3) * G) + s2) * G) + s1) * G;
      result = ((F + (F * R)) * Sign);
   }

return (result);
}

static inline float32_tidl recipsp_tansp_i(float32_tidl a)
{
   const float32_tidl two = 2.0f;
   float32_tidl       x1, y;

   x1 = __recip(a);
   x1 = x1 * (two - (a * x1));
   y  = x1 * (two - (a * x1));

   if (a == 0.0f) {
      y = 0.0f;
   }

   return (y);
} /* End of recipsp_tansp_i */

static inline float32_tidl pol_est_tansp_i(float32_tidl x)
{
   /* coefficients for the polynomial for tan(x)*/
   const float32_tidl c12 = 0.00457708084094171f;
   const float32_tidl c10 = 0.00867365970442499f;
   const float32_tidl c8  = 0.0236488997700456f;
   const float32_tidl c6  = 0.0523817525811416f;
   const float32_tidl c4  = 0.133766510899828f;
   const float32_tidl c2  = 0.333299506194791f;

   float32_tidl x2, x4, x6, x8, pol;
   float32_tidl tmp1, tmp2;

   /* calculate the powers of x */
   x2 = x * x;
   x4 = x2 * x2;
   x6 = x2 * x4;
   x8 = x4 * x4;

   /* ====================================================================== */
   /* The polynomial calculation is done in two seperate parts.              */
   /*   tmp1 =  c2 x^2 + c4 x^4 + c6 x^6 + c8 x^8                            */
   /*   tmp2 =  c10 x^10 + c12 x^12                                          */
   /* In order to reduce the number of multiplications x is factored out of  */
   /* the polynomial and multiplied by later.                                */
   /* ====================================================================== */

   tmp1 = ((c8 * x8) + (c6 * x6)) + ((c4 * x4) + (c2 * x2));
   tmp2 = x8 * ((c10 * x2) + (c12 * x4));

   pol = ((tmp1 + tmp2) * x) + x;

   return pol;
} /* End of pol_est_tansp_i */

static inline float32_tidl MATHLIB_tan(float32_tidl x)
{
   const float32_tidl pi4   = 0.785398163f; /* pi/4 */
   const float32_tidl pi2_h = 1.570796f;
   const float32_tidl pi2_l = 3.26794896476912e-7f; /* pi2_h + pi2_l = pi/2 */
   const float32_tidl pi    = 3.14159265358979f;
   const float32_tidl C1    = 3.140625f;
   const float32_tidl C2    = 9.676535e-4f;
   const float32_tidl C3    = 8.97930618748433e-11f; /* C1 + C2 + C3 = PI */
   const float32_tidl invPI = 0.318309886183791f;    /* 1/pi */
   float32_tidl       sign  = 1.0f;

   float32_tidl res, y, temp;
   int32_t   N;

   /* extract multiples of pi */
   temp = x * invPI;
   N    = float_to_int_c7x(temp); /* N = multiple of pi in x  */
   y    = ((x - ((float32_tidl) N * C1)) - ((float32_tidl) N * C2)) - ((float32_tidl) N * C3);

   if (y > pi2_h) {
      y = y - pi; /* x is in the range [-pi/2, pi/2] */
   }

   if (y < 0.0f) {
      sign = -sign; /* sign variable */
      y    = -y;
   }

   /* calculate tan(y) */
   if (y > pi4) {                 /* |y| > pi/4 */
      y    = (pi2_h - y) + pi2_l; /* pi/2 - y */
      temp = pol_est_tansp_i(y);
      res  = recipsp_tansp_i(temp); /* 1/tan(pi/2 - y) */
   }
   else { /* |y| <= pi/4 */
      res = pol_est_tansp_i(y);
   }

   res = res * sign; /* sign for Quadrant 2 & 4 */
   return (res);
}

static inline float32_tidl MATHLIB_sqrt(float32_tidl a)
{
   const float32_tidl Half  = 0.5f;
   const float32_tidl OneP5 = 1.5f;
   float32_tidl       x, y;
   int32_t         i;

   x = __recip_sqrt(a); /* compute square root reciprocal */

#pragma UNROLL(1) /* PRAGMA: do not unroll this loop */
   for (i = 0; i < 2; i++) {
      x = x * (OneP5 - (a * x * x * Half));
   }
   y = a * x;

   if (a <= 0.0f) {
      y = 0.0f;
   }
   /* LDRA_JUSTIFY_START
   <metric start> statement branch <metric end>
   <justification start> SAFETY_CHECK: Safe programming hard to hit this condition with real world data.
   <justification end> */
   if (a > FLT_MAX) {
      y = FLT_MAX;
   }
   /* LDRA_JUSTIFY_END */

   return (y);
}

// mathlib pow implementation
static inline float32_tidl MATHLIB_log_pow_scalar_ci(float32_tidl a)
{
   float64_tidl ln2  = 0.693147180559945;
   float32_tidl  MAXe = 3.402823466E+38f;
   float32_tidl  c1   = -0.2302894f;
   float32_tidl  c2   = 0.1908169f;
   float32_tidl  c3   = -0.2505905f;
   float32_tidl  c4   = 0.3333164f;
   float32_tidl  c5   = -0.5000002f;
   float32_tidl  pol, r1, r2, r3, r4, res;
   float64_tidl dr, frcpax, rcp, T;
   int32_t    N, T_index;

   /* r = x * frcpa(x) -1 */
   rcp    = __recip((float64_tidl) a);
   frcpax = c6x_itod(c6x_clr(c6x_hi(rcp), 0u, 16u), 0u);
   dr     = (frcpax * (float64_tidl) a) - 1.0;

   /* Polynomial p(r) that approximates ln(1+r) - r */
   r1 = (float32_tidl) dr;
   r2 = r1 * r1;
   r3 = r1 * r2;
   r4 = r2 * r2;

   pol = (c5 * r2) + (c4 * r3) + (((c2 * r1) + c3 + (c1 * r2)) * r4);

   /* Reconstruction: result = T + r + p(r) */
   N       = (int32_t) c6x_extu(c6x_hi(frcpax), 1u, 21u) - 1023;
   T_index = (int32_t) c6x_extu(c6x_hi(frcpax), 12u, 29u);
   T       = MATHLIB_logTable[T_index] - (ln2 * (float64_tidl) N);
   res     = (float32_tidl) (dr + T + (float64_tidl) pol);

   if (a > MAXe) {
      res = 88.72283905313f;
   }

   return (res);
}

static inline float32_tidl MATHLIB_exp_pow_scalar_ci(float32_tidl a)
{
   float32_tidl                 log2_base_x16 = 1.442695041f * 16.0f;
   float32_tidl                 Halfe         = 0.5f;
   float32_tidl                 LnMine        = -87.33654475f;
   float32_tidl                 LnMaxe        = 88.72283905f;
   float32_tidl                 Maxe          = 3.402823466E+38f;
   float32_tidl                 c0            = 0.1667361910f;
   float32_tidl                 c1            = 0.4999999651f;
   float32_tidl                 c2            = 0.9999998881f;
   float32_tidl                 P1            = 0.04331970214844f;
   float32_tidl                 P2            = 1.99663646e-6f;
   float32_tidl                 pol, r, r2, r3, res;
   uint32_t          Ttemp;
   uint32_t J, K;
   float32_tidl                 Nf;
   int32_t               N;
   float64_tidl                dT;

   /* Get N such that |N - x*16/ln(2)| is minimized */
   Nf = (a * log2_base_x16) + Halfe;
   N  = (int32_t) Nf; /* Cast from intermediate variable to appease MISRA */

   if ((a * log2_base_x16) < -Halfe) {
      N--;
   }

   /* Argument reduction, r, and polynomial approximation pol(r) */
   r  = (a - (P1 * (float32_tidl) N)) - (P2 * (float32_tidl) N);
   r2 = r * r;
   r3 = r * r2;

   pol = (r * c2) + ((r3 * c0) + (r2 * c1));

   /* Get index for ktable and jtable */
   K  = c6x_extu((uint32_t /*!blw*/) N, 28u, 30u);
   J  = (uint32_t /*!blw*/) N & 0x3u;
   dT = MATHLIB_kTable[K] * MATHLIB_jTable[J];

   /* Scale exponent to adjust for 2^M */
   Ttemp = c6x_hi(dT) + (((uint32_t) /*!blw*/ N >> 4) << 20);
   dT    = c6x_itod(Ttemp, c6x_lo(dT));

   res = (float32_tidl) (dT * (1.0 + (float64_tidl) pol));

   /* Early exit for small a */
   if (c6x_extu(c6x_ftoi(a), 1u, 24u) < 114u) {
      res = 1.0f + a;
   }

   /* < LnMin returns 0 */
   if (a < LnMine) {
      res = 0.0f;
   }

   /* > LnMax returns MAX */
   if (a > LnMaxe) {
      res = Maxe;
   }

   return (res);
}

static inline float32_tidl MATHLIB_pow(float32_tidl a, float32_tidl b)
{
   float32_tidl arg, W, X2;
   int32_t   y, Sign = 1;

   /*!blw Negative base w/ odd-integer power should be negative */
   /*!blw this is not fully correct */
   y = float_to_int_c7x(b);
   if ((a < 0.0) && /*!blw*/ (b == (float32_tidl) y)) {
      if ((y % 2 /*!blw*/) != 0) {
         Sign = -1;
      }
   }

   arg = fabsf(a);
   W   = b * MATHLIB_log_pow_scalar_ci(arg);

   if (arg == 1.0f) {
      W = 0.0f;
   }

   X2 = (float32_tidl /*!blw*/) Sign * MATHLIB_exp_pow_scalar_ci(W);

   if ((a < 0.0f) && /*!blw*/ (b != (float32_tidl) y)) { /*!blw combine with other sign check*/
      X2 = c6x_itof(0x7fffffffu);
   }

   if (a == 0.0f) {
      X2 = (b >= 0.0f) ? 0.0f : c6x_itof(0x7F800000u);
   }

   if (b == 0.0f) {
      X2 = 1.0f;
   }

   return (X2);
}
#endif /*ITIDL_MATHLIB_UTILS_H */
