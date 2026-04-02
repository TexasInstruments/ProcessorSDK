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
*  @file         sfm_ti_matrixOps.h
*  @author       Deepak Poddar
*  @brief        This file defines the functions related matrix operations
*  @version 0.1 (Oct 2014) : Base version.
***************************************************************************
*/

#ifndef TI_SFM_MATRIXOPS_H
#define TI_SFM_MATRIXOPS_H

int32_t SFM_TI_gaussJordanElimination_cn(float *a, int32_t n, float *b,int32_t aMatPitch, uint8_t* scratch);
int32_t SFM_TI_gaussJordanElimination_ci(float a[restrict], int32_t n, float b[restrict],int32_t aMatPitch, uint8_t scratch[restrict]);

void SFM_TI_matrixMultiply_3x3(float A[], float B[], float C[]);
void SFM_TI_matrixMultiply_4x4(float A[], float B[], float C[]);

void SFM_TI_svdcmp(float* restrict Amat, int16_t rows, int16_t cols,int16_t pitch, float*  restrict Wmat, float* restrict vMat, float * restrict scratch);
float SFM_TI_PYTHAG(float a, float b);

void SFM_TI_multiplyByTranspose(float A[], float B[], float out[], uint16_t ma, uint16_t na, uint16_t nb);
void SFM_TI_multiplyByTranspose_ci(float A[restrict], float B[restrict], float out[restrict], uint16_t ma, uint16_t na, uint16_t nb) ;
void SFM_TI_multiplyByItsTranspose(float A[], float AtA[], uint16_t m, uint16_t n);
void SFM_TI_multiplyByItsTranspose_ci(float A[restrict],
                                      float AtA[restrict],
                                      float B[restrict],
                                      float Atb[restrict],
                                      uint16_t* curTrackLength,
                                      uint16_t n);
void SFM_TI_matrixMultiply_4x4By4x1(float A[], float B[], float C[]);

int32_t SFM_TI_LUDecomposion_3x3(float* A,float* b);
void SFM_TI_matrixMultiply_9x9(float A[], float B[], float C[], int32_t rowSteps);
int32_t SFM_TI_LUDecomposion_3x3_ci(float * restrict A, float * restrict b, int32_t* restrict result);


int32_t SFM_TI_Crout_LU_Decomposition_with_Pivoting(float *  Ain,
                                                    int16_t pivot[],
                                                    int32_t n,
                                                    float *  Aout,
                                                    int32_t pitch);

int32_t SFM_TI_Crout_LU_Decomposition(float *  ALUin,
                                      int32_t n,
                                      float *  ALout,
                                      float *  AUout,
                                      int32_t pitch);

int32_t SFM_TI_Crout_LU_with_Pivoting_Solve(float *LU,
                                            float B[],
                                            int16_t pivot[],
                                            float x[],
                                            int32_t n,
                                            int32_t pitch);

void SFM_TI_preMultiplyByTranspose( float * Ain, float * AtA, int32_t rows, int32_t cols, int32_t pitch);

int32_t SFM_TI_Crout_LU_Decomposition(float *  ALUin,
                                      int32_t  n,
                                      float *  ALout,
                                      float *  AUout,
                                      int32_t  pitch);

int32_t SFM_TI_Crout_LU_Decomposition_ci(float * restrict Ain,
                                         float * restrict ALUin,
                                         int32_t n,
                                         float * restrict ALout,
                                         float * restrict AUout,
                                         int32_t pitch);


void SFM_TI_preMultiplyByTranspose_ci( float * restrict Ain, float * restrict AtA, int32_t rows, int32_t cols, int32_t pitch);

int32_t SFM_TI_Crout_LU_Decomposition_with_Pivoting_ci(float * restrict Ain,
                                                       int16_t * restrict pivot,
                                                       int32_t n,
                                                       float * restrict Aout,
                                                       int32_t pitch);


int32_t SFM_TI_Crout_LU_with_Pivoting_Solve_ci(float L[restrict],
                                               float U[restrict],
                                               float B[restrict],
                                               short pivot[restrict],
                                               float x[restrict],
                                               int32_t n,
                                               int32_t pitch);
float SFM_TI_NormalizeVec(float * vec, uint16_t length, float * vecOut);

float SFM_TI_NormalizeVecAndComputeNorm(float * restrict vec,
                                        uint16_t length,
                                        float * restrict vecOut);

void SFM_TI_matrixMultiply_3x4By4x1(float A[restrict], float B[restrict], float C[restrict], int32_t num_matrices);
void SFM_TI_matrixMultiply_3x3By3x1(float A[restrict], float B[restrict], float C[restrict], int32_t num_matrices);

void SFM_TI_matrixMultiply_3x4By4x1_ci(float A[restrict], float B[restrict], float C[restrict], int32_t num_matrices, uint32_t* pBlock);
void SFM_TI_matrixMultiply_3x3By3x1_ci(float A[restrict], float B[restrict], float C[restrict], int32_t num_matrices, uint32_t* pBlock);

#endif
