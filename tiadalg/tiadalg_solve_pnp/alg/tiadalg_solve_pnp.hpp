#pragma once

//#include "Common.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <./../common/tiadalg_alg_int_interface.h>
#include <tiadalg_interface.h>
#include <vector>
#include <float.h>

#include <iostream>
#include <cstring>
#define USE_CUSTOM_LCG_RANDOM_NUMBER_GENERATOR 1

#if !USE_CUSTOM_LCG_RANDOM_NUMBER_GENERATOR
#include <random>
#endif

//#define TIADALG_DEBUG_EN

namespace ticv {
	enum {
		SOLVEPNP_TIP3P = 20 //!< An Efficient Algebraic Solution to the Perspective-Three-Point Problem @cite Ke17
	};
#if !USE_CUSTOM_LCG_RANDOM_NUMBER_GENERATOR
	static const std::seed_seq seq{ 1, 2, 3, 4, 5 };	//https://stackoverflow.com/a/15509942
#endif
	class tip3p
	{
	public:
#if USE_CUSTOM_LCG_RANDOM_NUMBER_GENERATOR
		tip3p(const float inFx, const float inFy, const float inCx, const float inCy, const int inIterationsCount, const float inReprojErrorThreshold, const float inInliersRatio, const uint64_t seed){
#else
		tip3p(const float inFx, const float inFy, const float inCx, const float inCy, const int inIterationsCount, const float inReprojErrorThreshold, const float inInliersRatio) : eng(seq) {
#endif
			fx = inFx;
			fy = inFy;
			cx = inCx;
			cy = inCy;
			this->calculateInverseParams();
			iterationsCount = inIterationsCount;
			reprojErrorThreshold = inReprojErrorThreshold;
			inliersRatio = inInliersRatio;
#if USE_CUSTOM_LCG_RANDOM_NUMBER_GENERATOR
			randomNumberGeneratorState = seed;
#endif
		}
		inline uint64_t getRandomNumberGeneratorState() {
			return randomNumberGeneratorState;
		}
#if USE_CUSTOM_LCG_RANDOM_NUMBER_GENERATOR
		inline int32_t getRandomNumberLCG(uint64_t largestValue) {			//Linear congruential generator


			uint64_t    returnValue;
			uint64_t    randomNumberGeneratorStateLoc;
			randomNumberGeneratorStateLoc = ((this->randomNumberGeneratorState * (uint64_t)1103515245) + (uint64_t)12345) >> 0x10U;	// glibc (used by GCC) constants used here
			returnValue = randomNumberGeneratorStateLoc % largestValue;
			this->randomNumberGeneratorState = randomNumberGeneratorStateLoc;
			return ((int32_t)returnValue);
		}
#endif

		bool solveP3PRansac(std::vector<std::vector<float> >& R, std::vector<std::vector<float> >& tvec, const std::vector<std::vector<float> >& twoDPoints, const std::vector<std::vector<float> >& threeDPoints) {
			int numberOfInputPoints = threeDPoints.size();
#if !USE_CUSTOM_LCG_RANDOM_NUMBER_GENERATOR
			std::uniform_int_distribution<int> distr(0, numberOfInputPoints-1);
#endif
			float locInliersRatio = 0.0f;
			bool success = 0;
			std::vector<std::vector<float> > bestR, bestTvec;
			std::vector<int> bestInliers;
			float bestInliersRatio = -1.0f;
      static int32_t total_test=0;

			for (int i = 0; i < iterationsCount || locInliersRatio > inliersRatio; i++) {
				std::vector<std::vector<float> > p3pSolverPoints2D, p3pSolverPoints3D;

				std::vector<std::vector<float> > localR, localTvec;

				for (int j = 0; j < 4; j++) {
#if USE_CUSTOM_LCG_RANDOM_NUMBER_GENERATOR
					int tempPtID = getRandomNumberLCG(numberOfInputPoints);
#else
					int tempPtID = distr(eng);
#endif

#ifdef TIADALG_DEBUG_EN
    printf("NatC :: random number generated is %d \n", tempPtID);
    printf("NatC :: 2d point is %f, %f \n", twoDPoints[tempPtID][0],twoDPoints[tempPtID][1]);
    printf("NatC :: 3d point is %f, %f, %f \n", threeDPoints[tempPtID][0],threeDPoints[tempPtID][1],threeDPoints[tempPtID][2]);
#endif
					p3pSolverPoints2D.push_back(twoDPoints[tempPtID]);
          p3pSolverPoints3D.push_back(threeDPoints[tempPtID]);

				}
#ifdef ENABLE_PROFILE
			long long t1, t0;
			TSCL = 0;
			TSCH = 0;
			t0 = _TSC_read();
#endif

				bool test = this->solve(localR, localTvec, p3pSolverPoints3D, p3pSolverPoints2D);

#ifdef ENABLE_PROFILE
			t1 = _TSC_read();
			//printf("NatC :: Cycle consumed in one iteration of solve is %lld\n",(t1-t0));
#endif

				float reprojectionError, reprojectionErrorTotal, locInliersRatio;
				std::vector<int> inlierIndexes;

				if (test){
          total_test++;
					success = 1;
#ifdef ENABLE_PROFILE
			long long t0,t1;
			TSCL=0;
			TSCH=0;
			t0 = _TSC_read();
#endif

					bool testReproj = this->findReprojectionErrorAndInliersRatio(localR, localTvec, twoDPoints, threeDPoints, &reprojectionError, &reprojectionErrorTotal, &locInliersRatio, inlierIndexes);

#ifdef ENABLE_PROFILE
			t1 = _TSC_read();
			//printf("NatC :: Cycle consumed in inlier computation is %lld\n", (t1-t0));
#endif

          //bool testReproj = 0;
					if (testReproj) {
						if (locInliersRatio >= this->inliersRatio) {
							R = localR;
							tvec = localTvec;
#ifdef TIADALG_DEBUG_EN
  printf("NatC :: Sufficient number of inlier is reached\n");
#endif
							return 1;
						} else if (bestInliersRatio <= locInliersRatio) { // bestReprojectionError > reprojectionError && bestInliersRatio <= locInliersRatio) {
							bestR = localR;
							bestTvec = localTvec;
							bestInliersRatio = locInliersRatio;
							bestInliers = inlierIndexes;
#ifdef TIADALG_DEBUG_EN
  printf("NatC :: Best inlier is found\n");
  printf("NatC :: Rotation matrix solution is %f, %f, %f, %f, %f, %f, %f, %f, %f \n", bestR[0][0],bestR[0][1],bestR[0][2],bestR[1][0],bestR[1][1],bestR[1][2],bestR[2][0],bestR[2][1],bestR[2][2]);
  printf("NatC :: Translation vector solution is %f, %f, %f \n", bestTvec[0][0],bestTvec[1][0],bestTvec[2][0]);
#endif
						}
					}
				}
			}
#ifdef TIADALG_DEBUG_EN
			printf("total number of tests are happening %d \n",total_test);
#endif
			R = bestR;
			tvec = bestTvec;
			return success;
		}
	private:

		bool findReprojectionErrorAndInliersRatio(const std::vector<std::vector<float> >& R, const std::vector<std::vector<float> >& tvec, const std::vector<std::vector<float> >& twoDPoints, const std::vector<std::vector<float> >& threeDPoints,
			float *reprojectionError, float *reprojectionErrorTotal, float *inliersRatio, std::vector<int> &InlierIndexes) {
			float tmpReprojectionError = 0.0f;
			float tmpReprojectionErrorTotal = 0.0f;
			std::vector<int> tmpInlierIndexes;
			int inliersCount = 0;
			for (int i = 0; i < threeDPoints.size(); i++) {
				float X3p = R[0][0] * threeDPoints[i][0] + R[0][1] * threeDPoints[i][1] + R[0][2] * threeDPoints[i][2] + tvec[0][0];
				float Y3p = R[1][0] * threeDPoints[i][0] + R[1][1] * threeDPoints[i][1] + R[1][2] * threeDPoints[i][2] + tvec[1][0];
				float Z3p = R[2][0] * threeDPoints[i][0] + R[2][1] * threeDPoints[i][1] + R[2][2] * threeDPoints[i][2] + tvec[2][0];
				float mu3p = this->cx + this->fx * X3p / Z3p;
				float mv3p = this->cy + this->fy * Y3p / Z3p;

				float mu2p = twoDPoints[i][0];
				float mv2p = twoDPoints[i][1];
				float reproj = sqrtf((mu3p - mu2p) * (mu3p - mu2p) + (mv3p - mv2p) * (mv3p - mv2p));
				tmpReprojectionErrorTotal = tmpReprojectionErrorTotal + reproj;

				if (reproj < this->reprojErrorThreshold) {
					tmpReprojectionError = tmpReprojectionError + reproj;
					tmpInlierIndexes.push_back(i);
					inliersCount++;
				}
			}
			if (inliersCount == 0) {
				*reprojectionErrorTotal = tmpReprojectionErrorTotal;
				InlierIndexes.clear();
				return 0;
			} else {
				*reprojectionErrorTotal = tmpReprojectionErrorTotal;
				*reprojectionError = +tmpReprojectionError;
				*inliersRatio = float(inliersCount) / float(threeDPoints.size());
        InlierIndexes.clear();
				InlierIndexes = tmpInlierIndexes;
				return 1;
			}
		}

		bool solve(std::vector<std::vector<float> >& R, std::vector<std::vector<float> >& tvec, std::vector<std::vector<float> >& opoints, std::vector<std::vector<float> >& ipoints) {
			float rotation_matrix[3][3], translation[3];
			std::vector <std::vector<float> > points;
			this->extract_points(opoints, ipoints, points);


			bool result = this->solve(rotation_matrix, translation, points[0][0], points[0][1], points[0][2], points[0][3], points[0][4], points[1][0], points[1][1], points[1][2], points[1][3], points[1][4], points[2][0], points[2][1], points[2][2], points[2][3], points[2][4], points[3][0], points[3][1], points[3][2], points[3][3], points[3][4]);

			if (result) {


				//resize2DVector(R, 3, 3);
				for (int i = 0; i < R.size(); i++) {
					R[i].clear();
				}
				R.resize(3);
				for (int i = 0; i < R.size(); i++) {
					R[i].resize(3);
				}

				//resize2DVector(tvec, 3, 1);
				for (int i = 0; i < tvec.size(); i++) {
					tvec[i].clear();
				}
				tvec.resize(3);
				for (int i = 0; i < tvec.size(); i++) {
					tvec[i].resize(1);
				}

				for (int i = 0; i <= 2; i++) {
					for (int j = 0; j <= 2; j++) {
						R[i][j] = rotation_matrix[i][j];
					}
					tvec[i][0] = translation[i];
				}
			}
			return result;
		}

		bool solve(float R[3][3], float t[3], float mu0, float mv0, float X0, float Y0, float Z0, float mu1, float mv1, float X1, float Y1, float Z1, float mu2, float mv2, float X2, float Y2, float Z2, float mu3, float mv3, float X3, float Y3, float Z3) {

			float Rs[4][3][3], ts[4][3];
			int n = this->solve(Rs, ts, mu0, mv0, X0, Y0, Z0, mu1, mv1, X1, Y1, Z1, mu2, mv2, X2, Y2, Z2);

			if (n == 0)
				return false;

			int ns = 0;
			float min_reproj = 0.0f;
			for (int i = 0; i < n; i++) {	//4x 2-Comp 6-Mult, 7-Add , 1-Div
				float X3p = Rs[i][0][0] * X3 + Rs[i][0][1] * Y3 + Rs[i][0][2] * Z3 + ts[i][0];
				float Y3p = Rs[i][1][0] * X3 + Rs[i][1][1] * Y3 + Rs[i][1][2] * Z3 + ts[i][1];
				float Z3p = Rs[i][2][0] * X3 + Rs[i][2][1] * Y3 + Rs[i][2][2] * Z3 + ts[i][2];
				float mu3p = cx + fx * X3p / Z3p;
				float mv3p = cy + fy * Y3p / Z3p;
				float reproj = (mu3p - mu3) * (mu3p - mu3) + (mv3p - mv3) * (mv3p - mv3);
				if (i == 0 || min_reproj > reproj) {
					ns = i;
					min_reproj = reproj;
				}
			}

			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++)
					R[i][j] = Rs[ns][i][j];
				t[i] = ts[ns][i];
			}
#ifdef TIADALG_DEBUG_EN
  printf("NatC :: Best solution for current ransac iteration is \n");
  printf("NatC :: Rotation matrix solution is %f, %f, %f, %f, %f, %f, %f, %f, %f \n", Rs[ns][0][0],Rs[ns][0][1],Rs[ns][0][2],Rs[ns][1][0],Rs[ns][1][1],Rs[ns][1][2],Rs[ns][2][0],Rs[ns][2][1],Rs[ns][2][2]);
  printf("NatC :: Translation vector solution is %f, %f, %f \n", ts[ns][0],ts[ns][1],ts[ns][2]);
#endif
			return true;
		}

		int  solve(float R[4][3][3], float t[4][3], float mu0, float mv0, float X0, float Y0, float Z0, float mu1, float mv1, float X1, float Y1, float Z1, float mu2, float mv2, float X2, float Y2, float Z2) {
			float mk0, mk1, mk2;
			float norm;

			mu0 = inv_fx * mu0 - cx_fx;		//3x //Mult + Add
			mv0 = inv_fy * mv0 - cy_fy;		//Mult + Add
			norm = sqrtf(mu0 * mu0 + mv0 * mv0 + 1.0f);		//2-Mult + 2-Add + 1-sqrtf
			mk0 = 1.0f / norm; mu0 *= mk0; mv0 *= mk0;	//1-Div, 2-Mult

			mu1 = inv_fx * mu1 - cx_fx;
			mv1 = inv_fy * mv1 - cy_fy;
			norm = sqrtf(mu1 * mu1 + mv1 * mv1 + 1.0f);
			mk1 = 1.0f / norm; mu1 *= mk1; mv1 *= mk1;

			mu2 = inv_fx * mu2 - cx_fx;
			mv2 = inv_fy * mv2 - cy_fy;
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
  printf("NatC :: distances are %f,%f,%f \n", distances[0],distances[1],distances[2]);
  printf("NatC :: cosines are %f,%f,%f \n", cosines[0],cosines[1],cosines[2]);
#endif

			int n = this->solveForLengths(lengths, distances, cosines);

#ifdef TIADALG_DEBUG_EN
  for(int32_t i = n; i < 4; i++){
    lengths[i][0] = 0;
    lengths[i][1] = 0;
    lengths[i][2] = 0;
  }
  printf("NatC :: lengths are %f,%f,%f \n", lengths[0][0],lengths[0][1],lengths[0][2]);
#endif
			int nb_solutions = 0;
			for (int i = 0; i < n; i++) {
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

				if (!this->align(M_orig, X0, Y0, Z0, X1, Y1, Z1, X2, Y2, Z2, R[nb_solutions], t[nb_solutions]))
					continue;
#ifdef TIADALG_DEBUG_EN
  printf("NatC :: Rotation matrix solution is %f, %f, %f, %f, %f, %f, %f, %f, %f \n",\
   R[nb_solutions][0][0],R[nb_solutions][0][1],R[nb_solutions][0][2],R[nb_solutions][1][0],R[nb_solutions][1][1],R[nb_solutions][1][2],R[nb_solutions][2][0],R[nb_solutions][2][1],R[nb_solutions][2][2]);
  printf("NatC :: Translation vector solution is %f, %f, %f \n", t[nb_solutions][0],t[nb_solutions][1],t[nb_solutions][2]);
#endif
				nb_solutions++;
			}

			return nb_solutions;
		}
		void extract_points(const std::vector<std::vector<float> >& opoints, const std::vector<std::vector<float> >& ipoints, std::vector<std::vector<float> >& points) {
			//resize2DVector(points, opoints.size(), 5);

			for (int i = 0; i < points.size(); i++) {
				points[i].clear();
			}
			points.resize(opoints.size());
			for (int i = 0; i < points.size(); i++) {
				points[i].resize(5);
			}


			for (int i = 0; i < opoints.size(); i++) {
				points[i][0] = ipoints[i][0];// *this->fx + cx;	/// 1-Mul + 1 Addition
				points[i][1] = ipoints[i][1];// *this->fy + cy;	/// 1-Mul + 1 Addition
				for (int j = 0; j < opoints[0].size(); j++) {
					points[i][j + 2] = opoints[i][j];
				}
			}
		}

		void calculateInverseParams() {
			inv_fx = 1.0f / fx;	/// 1-Div
			inv_fy = 1.0f / fy;
			cx_fx = cx / fx;	//1 Mult
			cy_fy = cy / fy;	//1 Mult
		}

		int solveForLengths(float lengths[4][3], float distances[3], float cosines[3]) {
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

			float real_roots[4];
			int n = this->solve_deg4(A, B, C, D, E, real_roots[0], real_roots[1], real_roots[2], real_roots[3]);

			if (n == 0)
				return 0;

			int nb_solutions = 0;
			float r3 = r2 * r, pr2 = p * r2, r3q = r3 * q;				//3-Mul
			float inv_b0 = 1.0f / b0;									//1-Div

			// For each solution of x
			for (int i = 0; i < n; i++) {
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

		bool align(float M_end[3][3], float X0, float Y0, float Z0, float X1, float Y1, float Z1, float X2, float Y2, float Z2, float R[3][3], float T[3]) {
			// Centroids:
			float C_start[3], C_end[3];
			for (int i = 0; i < 3; i++) C_end[i] = (M_end[0][i] + M_end[1][i] + M_end[2][i]) / 3.0f;	//6-Add + 1-Div
			C_start[0] = (X0 + X1 + X2) / 3.0f;	//6-Add + 1-Mul
			C_start[1] = (Y0 + Y1 + Y2) / 3.0f;
			C_start[2] = (Z0 + Z1 + Z2) / 3.0f;

			// Covariance matrix s:
			float s[3 * 3];
			for (int j = 0; j < 3; j++) {
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

			this->jacobi4x4(Qs, evs, U);

			// Looking for the largest eigen value:
			int i_ev = 0;
			float ev_max = evs[i_ev];
			for (int i = 1; i < 4; i++)	//4-Comp
				if (evs[i] > ev_max)
					ev_max = evs[i_ev = i];

			// Quaternion:
			float q[4];
			for (int i = 0; i < 4; i++)
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

			for (int i = 0; i < 3; i++)			//3x 3-Mul+ 2-Add + 1-Add
				T[i] = C_end[i] - (R[i][0] * C_start[0] + R[i][1] * C_start[1] + R[i][2] * C_start[2]);

			return true;
		}

		bool jacobi4x4(float * A, float * D, float * U) {
			float B[4], Z[4];
			float Id[16] = { 1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f };

			std::memcpy(U, Id, 16 * sizeof(float));

			B[0] = A[0]; B[1] = A[5]; B[2] = A[10]; B[3] = A[15];
			std::memcpy(D, B, 4 * sizeof(float));
			std::memset(Z, 0, 4 * sizeof(float));

			for (int iter = 0; iter < 50; iter++) {
				float sum = fabs(A[1]) + fabs(A[2]) + fabs(A[3]) + fabs(A[6]) + fabs(A[7]) + fabs(A[11]);  //5-Abs + 5-Add

				if (sum == 0.0)	//1-comp
					return true;

				float tresh = (iter < 3) ? 0.2f * sum / 16.0f : 0.0f;	//1-comp + 1-Mul
				for (int i = 0; i < 3; i++) {			//x3
					float * pAij = A + 5 * i + 1;		// 1+Mul + 2-Add
					for (int j = i + 1; j < 4; j++) {   //x10
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
							for (int k = 0; k <= i - 1; k++) {		//x3
								float g = A[k * 4 + i], h = A[k * 4 + j];
								A[k * 4 + i] = g - s * (h + g * tau);		//2-Mul, 2-Add
								A[k * 4 + j] = h + s * (g - h * tau);		//2-Mul, 2-Add
							}
							for (int k = i + 1; k <= j - 1; k++) {			//x3
								float g = A[i * 4 + k], h = A[k * 4 + j];
								A[i * 4 + k] = g - s * (h + g * tau);		//2-Mul, 2-Add
								A[k * 4 + j] = h + s * (g - h * tau);		//2-Mul, 2-Add
							}
							for (int k = j + 1; k < 4; k++) {	//x3
								float g = A[i * 4 + k], h = A[j * 4 + k];
								A[i * 4 + k] = g - s * (h + g * tau);		//2-Mul, 2-Add
								A[j * 4 + k] = h + s * (g - h * tau);		//2-Mul, 2-Add
							}
							for (int k = 0; k < 4; k++) { //x4
								float g = U[k * 4 + i], h = U[k * 4 + j];
								U[k * 4 + i] = g - s * (h + g * tau);		//2-Mul, 2-Add
								U[k * 4 + j] = h + s * (g - h * tau);		//2-Mul, 2-Add
							}
						}
						pAij++;
					}
				}

				for (int i = 0; i < 4; i++) B[i] += Z[i];
				std::memcpy(D, B, 4 * sizeof(float));
				std::memset(Z, 0, 4 * sizeof(float));
			}

			return false;
		}

		int solve_deg2(float a, float b, float c, float & x1, float & x2)
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
		int solve_deg3(float a, float b, float c, float d, float & x0, float & x1, float & x2) {
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
				return this->solve_deg2(b, c, d, x0, x1);
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
		int solve_deg4(float a, float b, float c, float d, float e, float & x0, float & x1, float & x2, float & x3) {
			if (a == 0.0f) {	//1-Comp
				x3 = 0.0f;
				return this->solve_deg3(b, c, d, e, x0, x1, x2);
			}

			// Normalize coefficients
			float inv_a = 1.0f / a;	//1-Div
			b *= inv_a; c *= inv_a; d *= inv_a; e *= inv_a;		//3-Mult
			float b2 = b * b, bc = b * c, b3 = b2 * b;	//3-Mult

			// Solve resultant cubic
			float r0, r1, r2;
			int n = solve_deg3(1.0f, -c, d * b - 4.0f * e, 4.0f * c * e - d * d - b2 * e, r0, r1, r2);	//6-Mult
			if (n == 0) return 0;	//1-Comp

			// Calculate R^2
			float R2 = 0.25f * b2 - c + r0, R;	////1-Mult, 2-Add
			if (R2 < 0.0f)	//1-Comp
				return 0;

			R = sqrtf(R2);	//1-sqrtf
			float inv_R = 1.0f / R;	//1-Div

			int nb_real_roots = 0;

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


		float fx, fy, cx, cy;
		float inv_fx, inv_fy, cx_fx, cy_fy;
		int iterationsCount;
		float reprojErrorThreshold, inliersRatio;
#if USE_CUSTOM_LCG_RANDOM_NUMBER_GENERATOR
		uint64_t randomNumberGeneratorState;
#else
		std::mt19937 eng; // seed the generator
#endif
	};
}

