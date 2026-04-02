/*
*
* Copyright (c) {2015 - 2018} Texas Instruments Incorporated
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

#include <stdio.h>
#include <stdlib.h>
#include <./../common/tiadalg_alg_int_interface.h>
#include <tiadalg_interface.h>
#include <float.h>
#include <cstring>

#include <string.h>
//#define SOLVE_PNP_PROFILE
#ifdef SOLVE_PNP_PROFILE
long long t0,t1,t2,t3,t4,t5,t6,t7,t8,t9,t10;
long long acc0,acc1,acc2,acc3,acc4,acc5;
#endif

//#define TIADALG_DEBUG_EN

typedef struct tiadalg_p3p_params_int
{
	float fx;
	float fy;
	float cx;
	float cy;
	int iterationsCount;
	float reprojErrorThreshold;
	float reprojErrorThresholdSq;
	float inliersRatio;
	uint64_t seed;
  float inv_fx;
  float inv_fy;
  float cx_fx;
  float cy_fy;
}tiadalg_p3p_params_int;

static tiadalg_p3p_params_int p3p_params_int;

static bool findReprojectionErrorAndInliersRatio(float R[3][3], float tvec[3], const float twoDPoints[][2], const float threeDPoints[][3],float *inliersRatio,int32_t num_points);
static bool solve(float R[3][3], float t[3], float mu0, float mv0, float X0, float Y0, float Z0, float mu1, float mv1, float X1, float Y1, float Z1, float mu2, float mv2, float X2, float Y2, float Z2, float mu3, float mv3, float X3, float Y3, float Z3);
static int32_t  solve_n_solution(float R[4][3][3], float t[4][3], float mu0, float mv0, float X0, float Y0, float Z0, float mu1, float mv1, float X1, float Y1, float Z1, float mu2, float mv2, float X2, float Y2, float Z2);
static int32_t solveForLengths(float lengths[4][3], float distances[3], float cosines[3]);
static bool align(float M_end[3][3], float X0, float Y0, float Z0, float X1, float Y1, float Z1, float X2, float Y2, float Z2, float R[3][3], float T[3]);
static bool jacobi4x4(float * restrict A, float * restrict D, float * restrict U);
static int32_t solve_deg3(float a, float b, float c, float d, float & x0, float & x1, float & x2);
static int32_t solve_deg2(float a, float b, float c, float & x1, float & x2);
static int32_t solve_deg4(float a, float b, float c, float d, float e, float & x0, float & x1, float & x2, float & x3);
static inline int32_t getRandomNumberLCG(uint64_t largestValue);

int32_t tiadalg_solve_pnp_c66(tiadalg_p3p_params *params, float R[3][3], float tvec[3], const float twoDPoints[][2], const float threeDPoints[][3], const int numberOfpoints){

  float bestInliersRatio = -1.0f;
  float points_2d[4][2];
  float points_3d[4][3];
  float locInliersRatio = 0;
  float localR[3][3],localTvec[3], bestR[3][3], bestTvec[3];
  int32_t success = TIADALG_PROCESS_FAILURE;

  std::memset(bestTvec,0,3*sizeof(float));
  std::memset(bestR,0,3*3*sizeof(float));

  p3p_params_int.fx = params->fx;
  p3p_params_int.fy = params->fy;
  p3p_params_int.cx = params->cx;
  p3p_params_int.cy = params->cy;
  p3p_params_int.iterationsCount = params->iterationsCount;
  p3p_params_int.reprojErrorThreshold = params->reprojErrorThreshold;
  p3p_params_int.reprojErrorThresholdSq = params->reprojErrorThreshold*params->reprojErrorThreshold;
  p3p_params_int.inliersRatio = params->inliersRatio;
  p3p_params_int.seed = params->seed;
  p3p_params_int.inv_fx = 1.0f / params->fx;	/// 1-Div
  p3p_params_int.inv_fy = 1.0f / params->fy;
  p3p_params_int.cx_fx = params->cx / params->fx;	//1 Mult
  p3p_params_int.cy_fy = params->cy / params->fy;	//1 Mult

#ifdef SOLVE_PNP_PROFILE
  TSCL=0;
  TSCH=0;
  acc0=0;acc1=0,acc2=0,acc3=0;acc4=0;acc5=0;
#endif

#ifdef SOLVE_PNP_PROFILE
  t2 = _TSC_read();
#endif

	for (int i = 0; i < params->iterationsCount || locInliersRatio > params->inliersRatio; i++) {
    for (int32_t j = 0; j < 4; j++) {
      int32_t tempPtID = getRandomNumberLCG(numberOfpoints);

  #ifdef TIADALG_DEBUG_EN
      printf("C66 :: random number generated is %d \n", tempPtID);
      printf("C66 :: 2d point is %f, %f \n", twoDPoints[tempPtID][0],twoDPoints[tempPtID][1]);
      printf("C66 :: 3d point is %f, %f, %f \n", threeDPoints[tempPtID][0],threeDPoints[tempPtID][1],threeDPoints[tempPtID][2]);
  #endif
      points_2d[j][0] = twoDPoints[tempPtID][0];
      points_2d[j][1] = twoDPoints[tempPtID][1];

      points_3d[j][0] = threeDPoints[tempPtID][0];
      points_3d[j][1] = threeDPoints[tempPtID][1];
      points_3d[j][2] = threeDPoints[tempPtID][2];

    }
#ifdef SOLVE_PNP_PROFILE
  t0 = _TSC_read();
#endif

    bool test =
    solve(localR,localTvec,
          points_2d[0][0],points_2d[0][1],points_3d[0][0],points_3d[0][1],points_3d[0][2],
          points_2d[1][0],points_2d[1][1],points_3d[1][0],points_3d[1][1],points_3d[1][2],
          points_2d[2][0],points_2d[2][1],points_3d[2][0],points_3d[2][1],points_3d[2][2],
          points_2d[3][0],points_2d[3][1],points_3d[3][0],points_3d[3][1],points_3d[3][2]);

#ifdef SOLVE_PNP_PROFILE
  t1 = _TSC_read();
  acc0 += (t1-t0);
#endif

    if (test){
      success = TIADALG_PROCESS_SUCCESS;

#ifdef SOLVE_PNP_PROFILE
  t0 = _TSC_read();
#endif

      bool testReproj = findReprojectionErrorAndInliersRatio(localR, localTvec, twoDPoints, threeDPoints, &locInliersRatio,
      numberOfpoints);

#ifdef SOLVE_PNP_PROFILE
  t1 = _TSC_read();
  acc1 += (t1-t0);
#endif

      if (testReproj) {
        if (locInliersRatio >= p3p_params_int.inliersRatio) {
          std::memcpy(R,localR,3*3*sizeof(float));
          std::memcpy(tvec,localTvec,3*sizeof(float));
          success =TIADALG_PROCESS_SUCCESS;
#ifdef TIADALG_DEBUG_EN
  printf("C66 :: Sufficient number of inlier is reached\n");
#endif
          return success;
        } else if (bestInliersRatio <= locInliersRatio) {
          std::memcpy(bestR,localR,3*3*sizeof(float));
          std::memcpy(bestTvec,localTvec,3*sizeof(float));
          bestInliersRatio = locInliersRatio;

#ifdef TIADALG_DEBUG_EN
  printf("C66 :: Best inlier is found\n");
  printf("C66 :: Rotation matrix solution is %f, %f, %f, %f, %f, %f, %f, %f, %f \n", bestR[0][0],bestR[0][1],bestR[0][2],bestR[1][0],bestR[1][1],bestR[1][2],bestR[2][0],bestR[2][1],bestR[2][2]);
  printf("C66 :: Translation vector solution is %f, %f, %f \n", bestTvec[0],bestTvec[1],bestTvec[2]);
#endif

        }
      }
    }
  }
#ifdef SOLVE_PNP_PROFILE
  t3 = _TSC_read();
  acc2 += (t3-t2);
#endif

#ifdef SOLVE_PNP_PROFILE
  printf("Total cycle consumed is %lld \n",acc2);
  printf("Total cycle consumed in solve is %lld \n",acc0);
  printf("Total cycle consumed in inlier computation is %lld \n",acc1);
  printf("Total cycle consumed in align is %lld \n",acc3);
  printf("Total cycle consumed in jacobian is %lld \n",acc4);

#endif
  std::memcpy(R, bestR, 3*3*sizeof(float));
  std::memcpy(tvec, bestTvec, 3*sizeof(float));

  return success;
}

static bool solve(float R[3][3], float t[3], float mu0, float mv0, float X0, float Y0, float Z0, float mu1, float mv1, float X1, float Y1, float Z1, float mu2, float mv2, float X2, float Y2, float Z2, float mu3, float mv3, float X3, float Y3, float Z3) {

  float Rs[4][3][3], ts[4][3];
  int32_t n = solve_n_solution(Rs, ts, mu0, mv0, X0, Y0, Z0, mu1, mv1, X1, Y1, Z1, mu2, mv2, X2, Y2, Z2);

  if (n == 0)
    return false;

  int32_t ns = 0;
  float min_reproj = 0.0f;
  for (int32_t i = 0; i < n; i++) {	//4x 2-Comp 6-Mult, 7-Add , 1-Div
    float X3p = Rs[i][0][0] * X3 + Rs[i][0][1] * Y3 + Rs[i][0][2] * Z3 + ts[i][0];
    float Y3p = Rs[i][1][0] * X3 + Rs[i][1][1] * Y3 + Rs[i][1][2] * Z3 + ts[i][1];
    float Z3p = Rs[i][2][0] * X3 + Rs[i][2][1] * Y3 + Rs[i][2][2] * Z3 + ts[i][2];
    float mu3p = p3p_params_int.cx + p3p_params_int.fx * X3p / Z3p;
    float mv3p = p3p_params_int.cy + p3p_params_int.fy * Y3p / Z3p;
    float reproj = (mu3p - mu3) * (mu3p - mu3) + (mv3p - mv3) * (mv3p - mv3);
    if (i == 0 || min_reproj > reproj) {
      ns = i;
      min_reproj = reproj;
    }
  }

  for (int32_t i = 0; i < 3; i++) {
    for (int32_t j = 0; j < 3; j++)
      R[i][j] = Rs[ns][i][j];
      t[i] = ts[ns][i];
  }
#ifdef TIADALG_DEBUG_EN
  printf("C66 :: Best solution for current ransac iteration is \n");
  printf("C66 :: Rotation matrix solution is %f, %f, %f, %f, %f, %f, %f, %f, %f \n", Rs[ns][0][0],Rs[ns][0][1],Rs[ns][0][2],Rs[ns][1][0],Rs[ns][1][1],Rs[ns][1][2],Rs[ns][2][0],Rs[ns][2][1],Rs[ns][2][2]);
  printf("C66 :: Translation vector solution is %f, %f, %f \n", ts[ns][0],ts[ns][1],ts[ns][2]);
#endif
  return true;
}

static int32_t  solve_n_solution(float R[4][3][3], float t[4][3], float mu0, float mv0, float X0, float Y0, float Z0, float mu1, float mv1, float X1, float Y1, float Z1, float mu2, float mv2, float X2, float Y2, float Z2) {
  float mk0, mk1, mk2;
  float norm;

  mu0 = p3p_params_int.inv_fx * mu0 - p3p_params_int.cx_fx;		//3x //Mult + Add
  mv0 = p3p_params_int.inv_fy * mv0 - p3p_params_int.cy_fy;		//Mult + Add
  norm = sqrtf(mu0 * mu0 + mv0 * mv0 + 1.0f);		//2-Mult + 2-Add + 1-sqrtf
  mk0 = 1.0f / norm; mu0 *= mk0; mv0 *= mk0;	//1-Div, 2-Mult

  mu1 = p3p_params_int.inv_fx * mu1 - p3p_params_int.cx_fx;
  mv1 = p3p_params_int.inv_fy * mv1 - p3p_params_int.cy_fy;
  norm = sqrtf(mu1 * mu1 + mv1 * mv1 + 1.0f);
  mk1 = 1.0f / norm; mu1 *= mk1; mv1 *= mk1;

  mu2 = p3p_params_int.inv_fx * mu2 - p3p_params_int.cx_fx;
  mv2 = p3p_params_int.inv_fy * mv2 - p3p_params_int.cy_fy;
  norm = sqrtf(mu2 * mu2 + mv2 * mv2 + 1.0f);
  mk2 = 1.0f / norm; mu2 *= mk2; mv2 *= mk2;

  float distances[3];
  distances[0] = sqrtf((X1 - X2) * (X1 - X2) + (Y1 - Y2) * (Y1 - Y2) + (Z1 - Z2) * (Z1 - Z2));		//3x  3D Euclidean norm ( 3-Addition + 3-Mul + 3 Addition + 1sqrtf)
  distances[1] = sqrtf((X0 - X2) * (X0 - X2) + (Y0 - Y2) * (Y0 - Y2) + (Z0 - Z2) * (Z0 - Z2));
  distances[2] = sqrtf((X0 - X1) * (X0 - X1) + (Y0 - Y1) * (Y0 - Y1) + (Z0 - Z1) * (Z0 - Z1));

  // Calculate angles
  float cosines[3];
  cosines[0] = mu1 * mu2 + mv1 * mv2 + mk1 * mk2;	// 3x 3-Mul + 2 Addition
  cosines[1] = mu0 * mu2 + mv0 * mv2 + mk0 * mk2;
  cosines[2] = mu0 * mu1 + mv0 * mv1 + mk0 * mk1;

  float lengths[4][3];
#ifdef TIADALG_DEBUG_EN
  printf("C66 :: distances are %f,%f,%f \n", distances[0],distances[1],distances[2]);
  printf("C66 :: cosines are %f,%f,%f \n", cosines[0],cosines[1],cosines[2]);
#endif
  int32_t n = solveForLengths(lengths, distances, cosines);
#ifdef TIADALG_DEBUG_EN
  for(int32_t i = n; i < 4; i++){
    lengths[i][0] = 0;
    lengths[i][1] = 0;
    lengths[i][2] = 0;
  }
  printf("C66 :: lengths are %f,%f,%f \n", lengths[0][0],lengths[0][1],lengths[0][2]);
#endif

  int32_t nb_solutions = 0;

#ifdef TIADALG_DEBUG_EN
  printf("value of n is %d \n",n);
#endif

  for (int32_t i = 0; i < n; i++) {
    float M_orig[3][3];

    M_orig[0][0] = lengths[i][0] * mu0;		// 1-Mul
    M_orig[0][1] = lengths[i][0] * mv0;
    M_orig[0][2] = lengths[i][0] * mk0;

    M_orig[1][0] = lengths[i][1] * mu1;
    M_orig[1][1] = lengths[i][1] * mv1;
    M_orig[1][2] = lengths[i][1] * mk1;

    M_orig[2][0] = lengths[i][2] * mu2;
    M_orig[2][1] = lengths[i][2] * mv2;
    M_orig[2][2] = lengths[i][2] * mk2;

    if (!align(M_orig, X0, Y0, Z0, X1, Y1, Z1, X2, Y2, Z2, R[nb_solutions], t[nb_solutions]))
      continue;

#ifdef TIADALG_DEBUG_EN
  printf("C66 :: Rotation matrix solution is %f, %f, %f, %f, %f, %f, %f, %f, %f \n",\
   R[nb_solutions][0][0],R[nb_solutions][0][1],R[nb_solutions][0][2],R[nb_solutions][1][0],R[nb_solutions][1][1],R[nb_solutions][1][2],R[nb_solutions][2][0],R[nb_solutions][2][1],R[nb_solutions][2][2]);
  printf("C66 :: Translation vector solution is %f, %f, %f \n", t[nb_solutions][0],t[nb_solutions][1],t[nb_solutions][2]);
#endif

    nb_solutions++;
  }

  return nb_solutions;
}


static int32_t solveForLengths(float lengths[4][3], float distances[3], float cosines[3]) {
  float p = cosines[0] * 2.0f;
  float q = cosines[1] * 2.0f;
  float r = cosines[2] * 2.0f;

  float inv_d22 = 1.0f / (distances[2] * distances[2]);	//1-Div + 1-Mul
  float a = inv_d22 * (distances[0] * distances[0]);		//2-Mul
  float b = inv_d22 * (distances[1] * distances[1]);		//2-Mul

  float a2 = a * a, b2 = b * b, p2 = p * p, q2 = q * q, r2 = r * r;		//7-Mul
  float pr = p * r, pqr = q * pr;

  // Check reality condition (the four points should not be coplanar)
  if (p2 + q2 + r2 - pqr - 1.0f == 0.0f)	//5-Add + 1-Comp
    return 0;

  float ab = a * b, a_2 = 2.0f * a;		//2-Mul

  float A = -2.0f * b + b2 + a2 + 1.0f + ab * (2.0f - r2) - a_2;	//2-Mul + 6-Add

  // Check reality condition
  if (A == 0.0f) return 0;	//1-Comp

  float a_4 = 4.0f * a;		//1-Mul

  float B = q * (-2.0f * (ab + a2 + 1.0f - b) + r2 * ab + a_4) + pr * (b - b2 + ab);						//5-Add + 4-Mul + 4-Add
  float C = q2 + b2 * (r2 + p2 - 2.0f) - b * (p2 + pqr) - ab * (r2 + pqr) + (a2 - a_2)*(2.0f + q2) + 2.0f;	// 6-Add + 4-Mul + 5-Add
  float D = pr * (ab - b2 + b) + q * ((p2 - 2.0f)*b + 2.0f * (ab - a2) + a_4 - 2.0f);							//4-Add + 4-Mul + 4-Add
  float E = 1.0f + 2.0f * (b - a - ab) + b2 - b * p2 + a2;													//2-Add + 2-Mul + 4-Add

  float temp = (p2*(a - 1.0f + b) + r2 * (a - 1.0f - b) + pqr - a * pqr);									//2-Add + 2-Mul + 4-Add
  float b0 = b * temp * temp;																		//2-Mul
  // Check reality condition
  if (b0 == 0.0f)																						//1-Comp
    return 0;

  float real_roots[4] = {0.0f,0.0f,0.0f,0.0f};
  int32_t n = solve_deg4(A, B, C, D, E, real_roots[0], real_roots[1], real_roots[2], real_roots[3]);

  if (n == 0)
    return 0;

  int32_t nb_solutions = 0;
  float r3 = r2 * r, pr2 = p * r2, r3q = r3 * q;				//3-Mul
  float inv_b0 = 1.0f / b0;									//1-Div

  // For each solution of x
  for (int32_t i = 0; i < n; i++) {
    float x = real_roots[i];

    // Check reality condition
    if (x <= 0.0f)					//1-Comp
      continue;

    float x2 = x * x;			//1-Mul

    float b1 =	//37-Mul, 38-Add
      ((1.0f - a - b)*x2 + (q*a - q)*x + 1.0f - a + b) *
      (((r3*(a2 + ab * (2.0f - r2) - a_2 + b2 - 2.0f * b + 1.0f)) * x + ( r3q * (2.0f * (b - a2) + a_4 + ab * (r2 - 2.0f) - 2.0f) + pr2 * (1.0f + a2 + 2.0f * (ab - a - b) + r2 * (b - b2) + b2))) * x2 +
                  (r3*(q2*(1.0f - 2.0f * a + a2) + r2 * (b2 - ab) - a_4 + 2.0f * (a2 - b2) + 2.0f) + r * p2*(b2 + 2.0f * (ab - b - a) + 1.0f + a2) + pr2 * q*(a_4 + 2.0f * (b - ab - a2) - 2.0f - r2 * b)) * x +
                  2.0f * r3q*(a_2 - b - a2 + ab - 1.0f) +
        pr2 * (q2 - a_4 + 2.0f * (a2 - b2) + r2 * b + q2 * (a2 - a_2) + 2.0f) +
        p2 * (p*(2.0f * (ab - a - b) + a2 + b2 + 1.0f) + 2.0f * q*r*(b + a_2 - a2 - ab - 1.0f)));

    // Check reality condition
    if (b1 <= 0.0f)	//1-Comp
      continue;

    float y = inv_b0 * b1;  //1-Mul
    float v = x2 + y * y - x * y*r; //3-Mul + 3 Add

    if (v <= 0.0f)  //1-Comp
      continue;

    float Z = distances[2] / sqrtf(v); //1-sqrtf + 1-Div
    float X = x * Z;	//1-Mul
    float Y = y * Z;	//1-Mul

    lengths[nb_solutions][0] = X;
    lengths[nb_solutions][1] = Y;
    lengths[nb_solutions][2] = Z;

    nb_solutions++;
  }

  return nb_solutions;
}

static bool align(float M_end[3][3], float X0, float Y0, float Z0, float X1, float Y1, float Z1, float X2, float Y2, float Z2, float R[3][3], float T[3]) {
  // Centroids:
  float C_start[3], C_end[3];
  for (int32_t i = 0; i < 3; i++) C_end[i] = (M_end[0][i] + M_end[1][i] + M_end[2][i]) / 3.0f;	//6-Add + 1-Div
  C_start[0] = (X0 + X1 + X2) / 3.0f;	//6-Add + 1-Mul
  C_start[1] = (Y0 + Y1 + Y2) / 3.0f;
  C_start[2] = (Z0 + Z1 + Z2) / 3.0f;

#ifdef SOLVE_PNP_PROFILE
  t4 = _itoll(TSCH, TSCL);
#endif
  // Covariance matrix s:
  float s[3 * 3];
  for (int32_t j = 0; j < 3; j++) {
    s[0 * 3 + j] = (X0 * M_end[0][j] + X1 * M_end[1][j] + X2 * M_end[2][j]) / 3.0f - C_end[j] * C_start[0];	// 15-Mul + 9-Add
    s[1 * 3 + j] = (Y0 * M_end[0][j] + Y1 * M_end[1][j] + Y2 * M_end[2][j]) / 3.0f - C_end[j] * C_start[1];
    s[2 * 3 + j] = (Z0 * M_end[0][j] + Z1 * M_end[1][j] + Z2 * M_end[2][j]) / 3.0f - C_end[j] * C_start[2];
  }

  float Qs[16], evs[4], U[16];

  Qs[0 * 4 + 0] = s[0 * 3 + 0] + s[1 * 3 + 1] + s[2 * 3 + 2];	// 20-Add
  Qs[1 * 4 + 1] = s[0 * 3 + 0] - s[1 * 3 + 1] - s[2 * 3 + 2];
  Qs[2 * 4 + 2] = s[1 * 3 + 1] - s[2 * 3 + 2] - s[0 * 3 + 0];
  Qs[3 * 4 + 3] = s[2 * 3 + 2] - s[0 * 3 + 0] - s[1 * 3 + 1];

  Qs[1 * 4 + 0] = Qs[0 * 4 + 1] = s[1 * 3 + 2] - s[2 * 3 + 1];
  Qs[2 * 4 + 0] = Qs[0 * 4 + 2] = s[2 * 3 + 0] - s[0 * 3 + 2];
  Qs[3 * 4 + 0] = Qs[0 * 4 + 3] = s[0 * 3 + 1] - s[1 * 3 + 0];
  Qs[2 * 4 + 1] = Qs[1 * 4 + 2] = s[1 * 3 + 0] + s[0 * 3 + 1];
  Qs[3 * 4 + 1] = Qs[1 * 4 + 3] = s[2 * 3 + 0] + s[0 * 3 + 2];
  Qs[3 * 4 + 2] = Qs[2 * 4 + 3] = s[2 * 3 + 1] + s[1 * 3 + 2];

#ifdef SOLVE_PNP_PROFILE
  t6 = _TSC_read();
#endif

#ifdef TIADALG_DEBUG_EN
printf("call of jacobian \n");
#endif

  jacobi4x4(Qs, evs, U);

#ifdef SOLVE_PNP_PROFILE
  t7 = _TSC_read();
  acc4+=(t7-t6);
#endif
  // Looking for the largest eigen value:
  int32_t i_ev = 0;
  float ev_max = evs[i_ev];
  for (int32_t i = 1; i < 4; i++)	//4-Comp
    if (evs[i] > ev_max)
      ev_max = evs[i_ev = i];

  // Quaternion:
  float q[4];
  for (int32_t i = 0; i < 4; i++)
    q[i] = U[i * 4 + i_ev];

  float q02 = q[0] * q[0], q12 = q[1] * q[1], q22 = q[2] * q[2], q32 = q[3] * q[3];	//4-Mul
  float q0_1 = q[0] * q[1], q0_2 = q[0] * q[2], q0_3 = q[0] * q[3];	//3-Mul
  float q1_2 = q[1] * q[2], q1_3 = q[1] * q[3];	//2-Mul
  float q2_3 = q[2] * q[3];	//1-Mul

  R[0][0] = q02 + q12 - q22 - q32;	//3-Add
  R[0][1] = 2.0f * (q1_2 - q0_3);		//1-Add + 1-Mul
  R[0][2] = 2.0f * (q1_3 + q0_2);		//1-Add + 1-Mul

  R[1][0] = 2.0f * (q1_2 + q0_3);		//1-Add + 1-Mul
  R[1][1] = q02 + q22 - q12 - q32;	//3-Add
  R[1][2] = 2.0f * (q2_3 - q0_1);		//1-Add + 1-Mul

  R[2][0] = 2.0f * (q1_3 - q0_2);		//1-Add + 1-Mul
  R[2][1] = 2.0f * (q2_3 + q0_1);		//1-Add + 1-Mul
  R[2][2] = q02 + q32 - q12 - q22;	//3-Add

  for (int32_t i = 0; i < 3; i++)			//3x 3-Mul+ 2-Add + 1-Add
    T[i] = C_end[i] - (R[i][0] * C_start[0] + R[i][1] * C_start[1] + R[i][2] * C_start[2]);

#ifdef SOLVE_PNP_PROFILE
  t5 = _itoll(TSCH, TSCL);
  acc3 += (t5-t4);
#endif

  return true;
}

static bool jacobi4x4(float * restrict A, float * restrict D, float * restrict U) {
  float B[4], Z[4];
  float Id[16] = { 1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f };

#ifdef TIADALG_DEBUG_EN
    printf("Addresses are %x, %x,%x,%x,%x,%x \n", A,D,U,B,Z,Id);
#endif

  std::memcpy(U, Id, 16 * sizeof(float));

  B[0] = A[0]; B[1] = A[5]; B[2] = A[10]; B[3] = A[15];
  std::memcpy(D, B, 4 * sizeof(float));
  std::memset(Z, 0, 4 * sizeof(float));
  for (int32_t iter = 0; iter < 50; iter++) {

#ifdef TIADALG_DEBUG_EN
    printf("Jacobian iter no %d \n",iter);
#endif

    float sum = fabs(A[1]) + fabs(A[2]) + fabs(A[3]) + fabs(A[6]) + fabs(A[7]) + fabs(A[11]);  //5-Abs + 5-Add

    if (sum == 0.0){	//1-comp
#ifdef TIADALG_DEBUG_EN
     printf("return from internal check inside jacobian \n");
#endif
      return true;
    }
#ifdef SOLVE_PNP_PROFILE
    t9 = _TSC_read();
#endif

    float tresh = (iter < 3) ? 0.2f * sum / 16.0f : 0.0f;	//1-comp + 1-Mul
    for (int32_t i = 0; i < 3; i++) {			//x3
      float * pAij = A + 5 * i + 1;		// 1+Mul + 2-Add
      for (int32_t j = i + 1; j < 4; j++) {   //x10
        float Aij = *pAij;
        float eps_machine = 100.0f * fabs(Aij);	//1-Mul

        if (iter > 3 && fabs(D[i]) + eps_machine == fabs(D[i]) && fabs(D[j]) + eps_machine == fabs(D[j]))	//2-Add + 3-Comp
          *pAij = 0.0f;
        else if (fabs(Aij) > tresh) {		//1-Comp
          float hh = D[j] - D[i], t;		//1-Add
          if (fabs(hh) + eps_machine == fabs(hh))		//1-Comp
            t = Aij / hh;
          else {
            float theta = 0.5f * hh / Aij;		//1-Div + 1-Mul
            t = 1.0f / (fabs(theta) + sqrtf(1.0f + theta * theta)); //1-Mul 1-Add + 1sqrtf + 1-Add + 1-Div
            if (theta < 0.0f) t = -t;	//1-Comp
          }

          hh = t * Aij;	//1-Comp, 4-Add
          Z[i] -= hh;
          Z[j] += hh;
          D[i] -= hh;
          D[j] += hh;
          *pAij = 0.0f;

          float c = 1.0f / sqrtf(1.0f + t * t);		//1-Mul 1-Add + 1-sqrtf + 1-Div
          float s = t * c;						//1-Mul
          float tau = s / (1.0f + c);				//1-Add + 1-Div
          for (int32_t k = 0; k <= i - 1; k++) {		//x3 // 20 cycle loop
            float g = A[k * 4 + i], h = A[k * 4 + j];
            A[k * 4 + i] = g - s * (h + g * tau);		//2-Mul, 2-Add
            A[k * 4 + j] = h + s * (g - h * tau);		//2-Mul, 2-Add
          }
          for (int32_t k = i + 1; k <= j - 1; k++) {			//x3 // 20 cycle loop
            float g = A[i * 4 + k], h = A[k * 4 + j];
            A[i * 4 + k] = g - s * (h + g * tau);		//2-Mul, 2-Add
            A[k * 4 + j] = h + s * (g - h * tau);		//2-Mul, 2-Add
          }
          for (int32_t k = j + 1; k < 4; k++) {	//x3 // 3 cycle loop
            float g = A[i * 4 + k], h = A[j * 4 + k];
            A[i * 4 + k] = g - s * (h + g * tau);		//2-Mul, 2-Add
            A[j * 4 + k] = h + s * (g - h * tau);		//2-Mul, 2-Add
          }
          for (int32_t k = 0; k < 4; k++) { //x4
            float g = U[k * 4 + i], h = U[k * 4 + j];
            U[k * 4 + i] = g - s * (h + g * tau);		//2-Mul, 2-Add
            U[k * 4 + j] = h + s * (g - h * tau);		//2-Mul, 2-Add
          }
        }
        pAij++;
      }
    }
#ifdef SOLVE_PNP_PROFILE
    t10 = _TSC_read();
    acc5 += (t10-t9);
  #endif

    for (int32_t i = 0; i < 4; i++) B[i] += Z[i];
    std::memcpy(D, B, 4 * sizeof(float));
    std::memset(Z, 0, 4 * sizeof(float));
  }

#ifdef SOLVE_PNP_PROFILE
  printf("Jacobian internal loop cycle is %lld \n",acc5);
#endif

  return false;
}

static int32_t solve_deg2(float a, float b, float c, float & x1, float & x2)
{
  float delta = b * b - 4.0f * a * c;	//3-Mul + 1-Add

  if (delta < 0.0f) return 0;	//1-Comp

  float inv_2a = 0.5f / a;	//1-Div

  if (delta == 0.0f) {			//1-Comp
    x1 = -b * inv_2a;		//1-Mul
    x2 = x1;
    return 1;
  }

  float sqrt_delta = sqrtf(delta);	//1-sqrtf
  x1 = (-b + sqrt_delta) * inv_2a;	//1-Add + 1-Mul
  x2 = (-b - sqrt_delta) * inv_2a;	//1-Add + 1-Mul
  return 2;
}

/// Reference : Eric W. Weisstein. "Cubic Equation." From MathWorld--A Wolfram Web Resource.
/// http://mathworld.wolfram.com/CubicEquation.html
/// \return Number of real roots found.
static int32_t solve_deg3(float a, float b, float c, float d, float & x0, float & x1, float & x2) {
  if (a == 0) {	//3-Comp + 1-Div
    // Solve second order system
    if (b == 0) {
      // Solve first order system
      if (c == 0)
        return 0;

      x0 = -d / c;
      return 1;
    }

    x2 = 0.0f;
    return solve_deg2(b, c, d, x0, x1);
  }

  // Calculate the normalized form x^3 + a2 * x^2 + a1 * x + a0 = 0
  float inv_a = 1.0f / a;								//1-Div
  float b_a = inv_a * b, b_a2 = b_a * b_a;			//2-Mul
  float c_a = inv_a * c;								//1-Mul
  float d_a = inv_a * d;								//1-Mul

  // Solve the cubic equation
  float Q = (3.0f * c_a - b_a2) / 9.0f;					//2-Mul + 1-Add
  float R = (9.0f * b_a * c_a - 27.0f * d_a - 2.0f * b_a * b_a2) / 54.0f;	//6-Mul + 2 Add
  float Q3 = Q * Q * Q;								//2-Mul
  float D = Q3 + R * R;								//1-Mul + 1-Add
  float b_a_3 = (1.f / 3.f) * b_a;						//1-Mul

  if (Q == 0) {				//2-Comp + 1-powf + 1-Add
    if (R == 0) {
      x0 = x1 = x2 = -b_a_3;
      return 3;
    }
    else {
      x0 = powf(2.0f * R, 1.0f / 3.0f) - b_a_3;
      return 1;
    }
  }

  if (D <= 0.0f) {		//1-Comp + 1-acos + 1-sqrtf + + 3-Cos + 3-Mul + 3-Add
    const float pi = 3.1415926535897932384626433832795f;
    // Three real roots
    float theta = acosf(R / sqrtf(-Q3));
    float sqrtf_Q = sqrtf(-Q);
    x0 = 2.0f * sqrtf_Q * cosf(theta / 3.0f) - b_a_3;
    x1 = 2.0f * sqrtf_Q * cosf((theta + 2.0f * pi) / 3.0f) - b_a_3;
    x2 = 2.0f * sqrtf_Q * cosf((theta + 4.0f * pi) / 3.0f) - b_a_3;

    return 3;
  }

  // D > 0, only one real root
  float AD = powf(fabs(R) + sqrtf(D), 1.0f / 3.0f) * (R > 0.0f ? 1.0f : (R < 0.0f ? -1.0f : 0.0f));		//2-Comp, 1-sqrtf, 1-abs,
  float BD = (AD == 0.0f) ? 0.0f : -Q / AD;	//1-Comp, 1-Div

  // Calculate the only real root
  x0 = AD + BD - b_a_3;	//2-Add

  return 1;
}


/// Reference : Eric W. Weisstein. "Quartic Equation." From MathWorld--A Wolfram Web Resource.
/// http://mathworld.wolfram.com/QuarticEquation.html
/// \return Number of real roots found.
static int32_t solve_deg4(float a, float b, float c, float d, float e, float & x0, float & x1, float & x2, float & x3) {
  if (a == 0.0f) {	//1-Comp
    x3 = 0.0f;
    return solve_deg3(b, c, d, e, x0, x1, x2);
  }

  // Normalize coefficients
  float inv_a = 1.0f / a;	//1-Div
  b *= inv_a; c *= inv_a; d *= inv_a; e *= inv_a;		//3-Mult
  float b2 = b * b, bc = b * c, b3 = b2 * b;	//3-Mult

  // Solve resultant cubic
  float r0 = 0.0f;
  float r1 = 0.0f;
  float r2 = 0.0f;

  int32_t n = solve_deg3(1.0f, -c, d * b - 4.0f * e, 4.0f * c * e - d * d - b2 * e, r0, r1, r2);	//6-Mult
  if (n == 0) return 0;	//1-Comp

  // Calculate R^2
  float R2 = 0.25f * b2 - c + r0, R;	////1-Mult, 2-Add
  if (R2 < 0.0f)	//1-Comp
    return 0;

  R = sqrtf(R2);	//1-sqrtf
  float inv_R = 1.0f / R;	//1-Div

  int32_t nb_real_roots = 0;

  // Calculate D^2 and E^2
  float D2, E2;
  if (R < 10E-12) {	//1-Comp
    float temp = r0 * r0 - 4.0f * e;	//2-Mult, 1-Add
    if (temp < 0)	//1-sqrtf, 4-Mult, + 3-Add
      D2 = E2 = -1.0f;
    else {
      float sqrtf_temp = sqrtf(temp);
      D2 = 0.75f * b2 - 2.0f * c + 2.0f * sqrtf_temp;
      E2 = D2 - 4.0f * sqrtf_temp;
    }
  } else {
    float u = 0.75f * b2 - 2.0f * c - R2,
      v = 0.25f * inv_R * (4.0f * bc - 8.0f * d - b3);
    D2 = u + v;
    E2 = u - v;
  }

  float b_4 = 0.25f * b, R_2 = 0.5f * R;		//2-Mult
  if (D2 >= 0.0f) {	//1-cOMP, 1-sqrtf, 1-Mult, + 3-Add
    float D = sqrtf(D2);
    nb_real_roots = 2;
    float D_2 = 0.5f * D;
    x0 = R_2 + D_2 - b_4;
    x1 = x0 - D;
  }

  // Calculate E^2
  if (E2 >= 0.0f) {	//2-Comp, 1-sqrtf, 1-Mult, + 3-Add
    float E = sqrtf(E2);
    float E_2 = 0.5f * E;
    if (nb_real_roots == 0) {
      x0 = -R_2 + E_2 - b_4;
      x1 = x0 - E;
      nb_real_roots = 2;
    } else {
      x2 = -R_2 + E_2 - b_4;
      x3 = x2 - E;
      nb_real_roots = 4;
    }
  }

  return nb_real_roots;
}

static bool findReprojectionErrorAndInliersRatio(float R[3][3], float tvec[3], const float twoDPoints[][2], const float threeDPoints[][3],float *inliersRatio, int32_t num_points) {

  int32_t inliersCount = 0;

  // Cycle estimate
  for (int i = 0; i < num_points; i++) {
    float X3p = R[0][0] * threeDPoints[i][0] + R[0][1] * threeDPoints[i][1] + R[0][2] * threeDPoints[i][2] + tvec[0];
    float Y3p = R[1][0] * threeDPoints[i][0] + R[1][1] * threeDPoints[i][1] + R[1][2] * threeDPoints[i][2] + tvec[1];
    float Z3p = R[2][0] * threeDPoints[i][0] + R[2][1] * threeDPoints[i][1] + R[2][2] * threeDPoints[i][2] + tvec[2];
    float mu3p = p3p_params_int.cx*Z3p + p3p_params_int.fx * X3p;
    float mv3p = p3p_params_int.cy*Z3p + p3p_params_int.fy * Y3p ;

    float mu2p = twoDPoints[i][0]*Z3p;
    float mv2p = twoDPoints[i][1]*Z3p;
    //float reproj = sqrtf((mu3p - mu2p) * (mu3p - mu2p) + (mv3p - mv2p) * (mv3p - mv2p));
    float reproj = ((mu3p - mu2p) * (mu3p - mu2p) + (mv3p - mv2p) * (mv3p - mv2p));

    if (reproj < p3p_params_int.reprojErrorThresholdSq*Z3p*Z3p) {
      inliersCount++;
    }
  }

  if (inliersCount == 0) {
    return 0;
  } else {
    *inliersRatio = float(inliersCount) / float(num_points);
    return 1;
  }
}
static inline int32_t getRandomNumberLCG(uint64_t largestValue) {			//Linear congruential generator


  uint64_t    returnValue;
  uint64_t    randomNumberGeneratorStateLoc;
  randomNumberGeneratorStateLoc = ((p3p_params_int.seed * (uint64_t)1103515245) + (uint64_t)12345) >> 0x10U;	// glibc (used by GCC) constants used here
  returnValue = randomNumberGeneratorStateLoc % largestValue;
  p3p_params_int.seed = randomNumberGeneratorStateLoc;
  return ((int32_t)returnValue);
}
