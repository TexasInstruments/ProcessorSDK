/*
* module name : Structure From Motion
*
* module descripton : Generates sparse 3D points from optical flow information in camera captured images
*
* Copyright (C) 2007-2017 Texas Instruments Incorporated - http://www.ti.com/
* ALL RIGHTS RESERVED
*
*/
#include "sfm_ti_alg_int.h"
#include "sfm_ti_math.h"
#include "float.h"
#include "VLIB_triangulatePoints.h"

//#define TIADALG_TRIANG_PROFILE_EN

#ifdef TIADALG_TRIANG_PROFILE_EN
long long acc0,acc1,acc2,acc3,acc4,acc5,acc6, acc7;
long long t0, t1;
#include <stdio.h>
#endif

static void makeTriangMatrix_ci(VXLIB_F32 curTrack[restrict],
                                VXLIB_F32 normCamExtPrm[restrict],
                                VXLIB_F32 matrixA[restrict],
                                VXLIB_F32 matrixb[restrict],
                                int32_t totalTracks,
                                void* pBlock);

static void getPseudoMatrices_ci(VXLIB_F32 matrixA[restrict],
                                 VXLIB_F32 matrixb[restrict],
                                 VXLIB_F32 matrixP_AtA[restrict],
                                 VXLIB_F32 matrixP_Atb[restrict],
                                 int32_t totalTracks,
                                 void* pBlock);

static void getFinalMatrices_ci(VXLIB_F32 matrixP_AtA[restrict],
                                VXLIB_F32 matrixP_Atb[restrict],
                                VXLIB_F32 matrixAtA[restrict],
                                VXLIB_F32 matrixAtb[restrict],
                                VXLIB_F32 weight[restrict],
                                uint8_t curTrackLen[restrict],
                                int32_t totalTracks,
                                void* pBlock);

static void findTriangWt_ci(VXLIB_F32 outXcam[restrict],
                            VXLIB_F32 normCamExtPrm[restrict],
                            VXLIB_F32 weights[restrict],
                            int32_t totalTracks);

static void solveMatEq3x3_ci(VXLIB_F32 matAtAPtr[restrict],
                             VXLIB_F32 matBPtr[restrict],
                             VXLIB_F32 resultPtr[restrict],
                             uint8_t valid[restrict],
                             int32_t totalTracks);

static void solve3x3MatEqDouble(VXLIB_F32 matAtAPtr[restrict],
                                VXLIB_F32 matBPtr[restrict],
                                VXLIB_F32 resultPtr[restrict],
                                uint8_t valid[restrict],
                                int32_t totalTracks);

static void solve3x3MatEqDoubleSelect(VXLIB_F32 matAtAPtr[restrict],
                                      VXLIB_F32 matBPtr[restrict],
                                      VXLIB_F32 resultPtr[restrict],
                                      uint8_t valid[restrict],
                                      int32_t totalTracks);



/**
* @func makeTriangMatrix_ci
*
* @par Description:
*   This sub module forms initial data matrix for triangulation (Ax=b). Initial data
*   matrix are A & b. Size of data Matrix A is 2Nx3, and of b is 2Nx1, where
*   N is the total number of feature points in a given track. Two tracks are worked
*   together hence data matrix of two tracks are interleaved. For ease of access of
*   data, matrix A is formed in the form of 3x2N, and matrix b is formed in the
*   form of 1x2N. Maximum possible of feature points in a track is VXLIB_TRIANG_MAX_POINTS_IN_TRACK.
*   If a track doesn't have maximum number of feature points then some portion of
*   A & b remains un-initialized. Every feature point of track generates 2 columns
*   in matrix A & b. Storing the matrix A & b in transpose fashion also helps in
*   calculating AtA, and Atb.
*
* @par
*   @param [in]  curTrack      :Current track data after multiplying with intrinsic parameters.
*                               Two track data are clubbed together. x/y-coordinates of two
*                               tracks are placed together. Used in forming initial data matrices.
*                               Lets assume two tracks are {x0y0 x1y1 x2x2 x3y3 x4y4 x5y5} &
*                               {u0v0 u1v1 u2v2 u3v3 u4v4 u5v5}, then packed information is
*                               like {x0u0 y0v0 x1u1 y1v1 x2u2 y2v2 .. so on}
*   @param [in]  normCamExtPrm :Normalized projection matrix
*   @param [out] matrixA       :Output data matrix A of equation Ax=b
*   @param [out] matrixb       :Output data matrix b of equation Ax=b
*   @param [in]  curTrackLength:Array of values for track length
*   @param [in]  totalTracks   : Total number of tracks
*
* @par Assumptions:
*    -   NA
*
* @par Implementation Notes:
*    - NA
*
*/
//CODE_SECTION(makeTriangMatrix_ci,  ".text:optimized")
static void makeTriangMatrix_ci(VXLIB_F32 curTrack[restrict],
                                VXLIB_F32 normCamExtPrm[restrict],
                                VXLIB_F32 matrixA[restrict],
                                VXLIB_F32 matrixb[restrict],
                                int32_t totalTracks,
                                void* restrict pBlock)
{
    int32_t               i, l;
    VXLIB_F32             * restrict normCamExtPrmL;
    uint8_t               curTrackLengthLMax;
    float16               cur_x1x0, cur_y1y0;
    float16               vfTemp1;
    float16               vfTemp2;
    float16               vfTemp3;
    float16               vfTemp6;
    float16               vfTemp7;

    float *restrict    matrixAL  = (float*)matrixA;
    float *restrict    matrixbL  = (float*)matrixb;
    float *restrict    curTrackL = (float*)curTrack;

    __SE_TEMPLATE_v1 seTemplate;

    seTemplate = *(__SE_TEMPLATE_v1*)((uint8_t *)pBlock + SE1_PARAM_OFFSET);
#ifdef TIADALG_TRIANG_DEBUG
    displaySEParam(pBlock, seTemplate);
#endif
    __SE0_OPEN((void *)normCamExtPrm, seTemplate);

    __SE1_OPEN((void *)(normCamExtPrm + (VXLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_SIZE >> 1)), seTemplate);

    for( l = 0; l < totalTracks; l += VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR ) {

        curTrackLengthLMax = VXLIB_TRIANG_MAX_POINTS_IN_TRACK;

        matrixAL       = (float*)matrixA;
        matrixbL       = (float*)matrixb;
        normCamExtPrmL = normCamExtPrm;

// c7x compiler is unrolling this and unncessary making loop compicated. Hence puttin unroll(1) to avoid unrolling for now.
// right now loop is scheduled for ii=8, exactly bottelneck on write. However there is possiblity to merge with outer loop if we use SA
#pragma UNROLL(1)
        for( i = 0; i < curTrackLengthLMax; i++ ) {

            cur_x1x0                = *stov_ptr(float16,curTrackL);curTrackL+=SIMD_WIDTH;
            cur_y1y0                = *stov_ptr(float16,curTrackL);curTrackL+=SIMD_WIDTH;
            /*P[8]*/
            /* Dummy SE advance, this elements is needed to be loaded as regular load to reduce LCD*/
            //vfTemp1                 = __SE0ADV(float16);
            vfTemp1                 = (float16)normCamExtPrmL[0];

            /*P[0]*/
            //vfTemp2                 = __SE0ADV(float16);
            vfTemp2                 = (float16)normCamExtPrmL[1];

            /*P[4]*/
            //vfTemp3                 = __SE0ADV(float16);
            vfTemp3                 = (float16)normCamExtPrmL[2];

            *stov_ptr(float16, matrixAL)             = (vfTemp1 * cur_x1x0) - vfTemp2;
            matrixAL               +=SIMD_WIDTH;

            *stov_ptr(float16, matrixAL)             = (vfTemp1 * cur_y1y0) - vfTemp3;

            matrixAL               += (VXLIB_TRIANG_MAT_COL - 1)*SIMD_WIDTH;

            /*P[9]*/
            /* Dummy SE advance, this elements is needed to be loaded as regular load to reduce LCD*/
            //vfTemp1                 = __SE0ADV(float16);
            vfTemp1                 = (float16)normCamExtPrmL[3];

            /*P[1]*/
            //vfTemp2                 = __SE0ADV(float16);
            vfTemp2                 = (float16)normCamExtPrmL[4];

            /*P[5]*/
            //vfTemp3                 = __SE0ADV(float16);
            vfTemp3                 = (float16)normCamExtPrmL[5];

            *stov_ptr(float16, matrixAL)             = (vfTemp1 * cur_x1x0) - vfTemp2;
            matrixAL                +=SIMD_WIDTH;

            *stov_ptr(float16, matrixAL)           = (vfTemp1 * cur_y1y0) - vfTemp3;
            matrixAL             += (VXLIB_TRIANG_MAT_COL  - 1)*SIMD_WIDTH;

            /*P[10]*/
            /* Dummy SE advance, this elements is needed to be loaded as regular load to reduce LCD*/
            //vfTemp6               = __SE1ADV(float16);
            vfTemp6                 = (float16)normCamExtPrmL[6];

            /*P[11]*/
            /* Dummy SE advance, this elements is needed to be loaded as regular load to reduce LCD*/
            //vfTemp7               = __SE1ADV(float16);
            vfTemp7               = (float16)normCamExtPrmL[7];

            vfTemp1               = vfTemp6;
            /*P[2]*/
            //vfTemp2               = __SE1ADV(float16);
            vfTemp2               = (float16)normCamExtPrmL[8];

            /*P[6]*/
            //vfTemp3               = __SE1ADV(float16);
            vfTemp3               = (float16)normCamExtPrmL[9];

            *stov_ptr(float16, matrixAL)           = (vfTemp1 * cur_x1x0) - vfTemp2;
            matrixAL             +=SIMD_WIDTH;
            *stov_ptr(float16, matrixAL)           = (vfTemp1 * cur_y1y0) - vfTemp3;
            matrixAL             += (1 - (2 * VXLIB_TRIANG_MAT_COL ))*SIMD_WIDTH;
            /*P[11]*/
            vfTemp1               = vfTemp7;
            /*P[3]*/
            //vfTemp2               = __SE1ADV(float16);
            vfTemp2               = (float16)normCamExtPrmL[10];

            /*P[7]*/
            //vfTemp3               = __SE1ADV(float16);
            vfTemp3               = (float16)normCamExtPrmL[11];

            *stov_ptr(float16, matrixbL)           = vfTemp2 - (vfTemp1 * cur_x1x0);
            matrixbL             +=SIMD_WIDTH;
            *stov_ptr(float16, matrixbL)           = vfTemp3 - (vfTemp1 * cur_y1y0);
            matrixbL             +=SIMD_WIDTH;
            normCamExtPrmL       += VXLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE;
        }

        matrixA   += (VXLIB_TRIANG_MAT_ROW *
                      (VXLIB_TRIANG_MAT_COL *
                       VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR));

        matrixb   += (VXLIB_TRIANG_MAT_COL *
                      VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR);
    }
    __SE0_CLOSE();
    __SE1_CLOSE();
}
/**
* @func getPseudoMatrices
*
* @par Description:
*   This API finds the partial solution of AtA, and Atb
*
* @par
*   @param [in]  matrixA      : Original data matrix A
*   @param [in]  matrixb      : Original data matrix b
*   @param [out] matrixP_AtA  : Partial calculated AtA
*   @param [out] matrixP_Atb  : Partial calculated Atb
*   @param [in]  curTrackLen  : each track length in frames
*   @param [in]  totalTracks  : Total number of tracks
*
*
*
* @par Assumptions:
*    -   NA
*
* @par Implementation Notes:
*    -   NA
*
*/
//CODE_SECTION(getPseudoMatrices_ci,  ".text:optimized")
static void getPseudoMatrices_ci(VXLIB_F32 matrixA[restrict],
                                 VXLIB_F32 matrixb[restrict],
                                 VXLIB_F32 matrixP_AtA[restrict],
                                 VXLIB_F32 matrixP_Atb[restrict],
                                 int32_t totalTracks,
                                 void* pBlock )
{
    int32_t    outPitchA = VXLIB_TRIANG_MAX_POINTS_IN_TRACK*SIMD_WIDTH;

    int32_t               i = 0;
    int32_t               j = 0;
    int32_t               l = 0;
    float *restrict    matrixP_AtAL;
    float *restrict    matrixP_AtbL;

    __SE_TEMPLATE_v1 seTemplate;
    float16 vfTemp00, vfTemp10, vfTemp20, vfTemp01, vfTemp11, vfTemp21;

    seTemplate = *(__SE_TEMPLATE_v1*)((uint8_t *)pBlock + SE2_PARAM_OFFSET);

    __SE0_OPEN(matrixA,seTemplate);
    __SE1_OPEN((void*)(matrixA + VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR), seTemplate);
    matrixP_AtAL = (float *)matrixP_AtA;
    matrixP_AtbL = (float *)matrixP_Atb;

    /* Cycle performance of the inner most loop is 9 + 6*trackLength.
     *  ------------------------------------------------------------
     *  curTrackLength     | Merged scenario | Non Merge scenario
     *  ------------------------------------------------------------
     *        2            |   36            |   21
     *        3            |   36            |   27
     *        4            |   36            |   33  < kind of Break even>
     *        5            |   36            |   39  < Break even>
     *
     * Currently fixed length loop is implemented. For different shorter tracks loop can
     * written seperately.
     */
    for( l = 0; l < totalTracks; l += VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR ) {
#pragma UNROLL(1)
        for( j = 0; j < VXLIB_TRIANG_MAX_POINTS_IN_TRACK; j++ ) {
            vfTemp00   = __SE0ADV(float16);
            vfTemp10   = __SE0ADV(float16);
            vfTemp20   = __SE0ADV(float16);
            vfTemp01   = __SE1ADV(float16);
            vfTemp11   = __SE1ADV(float16);
            vfTemp21   = __SE1ADV(float16);
            *stov_ptr(float16, matrixP_AtAL + (0*outPitchA)) = vfTemp00*vfTemp00 + vfTemp01*vfTemp01;
            *stov_ptr(float16, matrixP_AtAL + (1*outPitchA)) = vfTemp00*vfTemp10 + vfTemp01*vfTemp11;
            *stov_ptr(float16, matrixP_AtAL + (2*outPitchA)) = vfTemp00*vfTemp20 + vfTemp01*vfTemp21;
            *stov_ptr(float16, matrixP_AtAL + (3*outPitchA)) = vfTemp10*vfTemp10 + vfTemp11*vfTemp11;
            *stov_ptr(float16, matrixP_AtAL + (4*outPitchA)) = vfTemp10*vfTemp20 + vfTemp11*vfTemp21;
            *stov_ptr(float16, matrixP_AtAL + (5*outPitchA)) = vfTemp20*vfTemp20 + vfTemp21*vfTemp21;

            matrixP_AtAL+=SIMD_WIDTH;
        }
        matrixP_AtAL += (((VXLIB_TRIANG_MAX_POINTS_IN_TRACK * VXLIB_TRIANG_NUM_UNIQUE_ELEMENTS_IN_ATA) - VXLIB_TRIANG_MAX_POINTS_IN_TRACK))*SIMD_WIDTH;
    }
    __SE0_CLOSE();
    __SE1_CLOSE();


    seTemplate = *(__SE_TEMPLATE_v1*)((uint8_t *)pBlock + SE0_PARAM_OFFSET);
    __SE0_OPEN(matrixA,seTemplate);

    seTemplate = *(__SE_TEMPLATE_v1*)((uint8_t *)pBlock + SE3_PARAM_OFFSET);
    __SE1_OPEN(matrixb, seTemplate);

    for( l = 0; l < totalTracks; l += VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR ){
        for( i = 0; i < 3; i++ ){
            for( j = 0; j < VXLIB_TRIANG_MAX_POINTS_IN_TRACK; j++ ){

                *stov_ptr(float16, matrixP_AtbL)  = (__SE0ADV(float16)*__SE1ADV(float16)) +
                                 (__SE0ADV(float16)*__SE1ADV(float16));
                matrixP_AtbL += SIMD_WIDTH;
            }
        }
    }
    __SE0_CLOSE();
    __SE1_CLOSE();

}
/**
* @func getFinalMatrices
*
* @par Description:
*   This API finds actual AtA , Atb from partial calculated
*   AtA & Atb
*
* @par
*   @param [in]   matrixP_AtA      : Partially calculated AtA
*   @param [in]   matrixP_Atb      : Partially calculated Atb
*   @param [out]  matrixAtA        : Final calculated AtA
*   @param [out]  matrixAtb        : Final Calculated Atb
*   @param [in]   weight           : Weights for scaling matrixP_AtA,matrixP_Atb
*                                    before processing
*   @param [in]  totalTracks       : Total number of tracks
*
*
*
* @par Assumptions:
*    -   NA
*
* @par Implementation Notes:
*    -   NA
*
*/

//CODE_SECTION(getFinalMatrices_ci,  ".text:optimized")
static void getFinalMatrices_ci(VXLIB_F32 matrixP_AtA[restrict],
                                VXLIB_F32 matrixP_Atb[restrict],
                                VXLIB_F32 matrixAtA[restrict],
                                VXLIB_F32 matrixAtb[restrict],
                                VXLIB_F32 weight[restrict],
                                uint8_t curTrackLen[restrict],
                                int32_t totalTracks,
                                void* pBlock)
{

    int32_t       i, l;
    float16       acc;
    VXLIB_F32 * restrict weightBackUp = weight;

    float16 wt10   = *stov_ptr(float16, &weight[0 * VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR]);
    float16 wt32   = *stov_ptr(float16, &weight[1 * VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR]);
    float16 wt54   = *stov_ptr(float16, &weight[2 * VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR]);
    float16 wt76   = *stov_ptr(float16, &weight[3 * VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR]);
    float16 wt98   = *stov_ptr(float16, &weight[4 * VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR]);
    float16 wtba   = *stov_ptr(float16, &weight[5 * VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR]);

    __vpred  vpred0,vpred1,vpred2,vpred3,vpred4,vpred5;
    __SE_TEMPLATE_v1 seTemplate;

    seTemplate = *(__SE_TEMPLATE_v1*)((uint8_t *)pBlock + SE4_PARAM_OFFSET);
    __SE0_OPEN(matrixP_AtA,seTemplate);
    __SE1_OPEN(matrixP_AtA + VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR,seTemplate);

    //assert(VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR == 4);

    for( l = 0; l < totalTracks; l += VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR ) {

        /*
         * Compiler not unrolling the loop "for( j = 0; j < VXLIB_TRIANG_MAX_POINTS_IN_TRACK; j++ ) "
         * efficiently hence mannualy code is unrolled temprorarly assuming VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR = 6
         */
        wt10   = *stov_ptr(float16, &weight[0 * VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR]);
        wt32   = *stov_ptr(float16, &weight[1 * VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR]);
        wt54   = *stov_ptr(float16, &weight[2 * VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR]);
        wt76   = *stov_ptr(float16, &weight[3 * VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR]);
        wt98   = *stov_ptr(float16, &weight[4 * VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR]);
        wtba   = *stov_ptr(float16, &weight[5 * VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR]);

        int16 vcurLength = __convert_int16(*stov_ptr(char16,&curTrackLen[l]));

        vpred0 = __cmp_ge_pred(vcurLength, (int16)(VXLIB_TRIANG_MAX_POINTS_IN_TRACK - 0));
        vpred1 = __cmp_ge_pred(vcurLength, (int16)(VXLIB_TRIANG_MAX_POINTS_IN_TRACK - 1));
        vpred2 = __cmp_ge_pred(vcurLength, (int16)(VXLIB_TRIANG_MAX_POINTS_IN_TRACK - 2));
        vpred3 = __cmp_ge_pred(vcurLength, (int16)(VXLIB_TRIANG_MAX_POINTS_IN_TRACK - 3));
        vpred4 = __cmp_ge_pred(vcurLength, (int16)(VXLIB_TRIANG_MAX_POINTS_IN_TRACK - 4));
        vpred5 = __cmp_ge_pred(vcurLength, (int16)(VXLIB_TRIANG_MAX_POINTS_IN_TRACK - 5));

        for( i = 0; i < VXLIB_TRIANG_NUM_UNIQUE_ELEMENTS_IN_ATA; i++ ) { /* VXLIB_TRIANG_NUM_UNIQUE_ELEMENTS_IN_ATA = 6*/
            acc = (float16)0.0f;
#if 0
            for( j = 0; j < VXLIB_TRIANG_MAX_POINTS_IN_TRACK; j++ ) {  /* VXLIB_TRIANG_MAX_POINTS_IN_TRACK = 6*/
                wt10   = *((float16*)&weight[j * VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR]);
                pAtA10 = *((float16*)matrixP_AtA);
                matrixP_AtA += VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR;
                acc    = acc + (wt10 * pAtA10);
            }
#else

            acc    = acc + __select(vpred0,(wt10 * __SE0ADV(float16)), (float16)0.0f);
            acc    = acc + __select(vpred1,(wt32 * __SE1ADV(float16)), (float16)0.0f);
            acc    = acc + __select(vpred2,(wt54 * __SE0ADV(float16)), (float16)0.0f);
            acc    = acc + __select(vpred3,(wt76 * __SE1ADV(float16)), (float16)0.0f);
            acc    = acc + __select(vpred4,(wt98 * __SE0ADV(float16)), (float16)0.0f);
            acc    = acc + __select(vpred5,(wtba * __SE1ADV(float16)), (float16)0.0f);

#endif

            *stov_ptr(float16, matrixAtA) = acc;
            matrixAtA           += VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR;
        }
        weight      += (VXLIB_TRIANG_MAX_POINTS_IN_TRACK *
                        VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR);

    }

    __SE0_CLOSE();
    __SE1_CLOSE();

    weight = weightBackUp;

    __SE0_OPEN(matrixP_Atb,seTemplate);
    __SE1_OPEN(matrixP_Atb + VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR,seTemplate);

    for( l = 0; l < totalTracks; l += VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR ) {

        /*
         * Compiler not unrolling the loop "for( j = 0; j < VXLIB_TRIANG_MAX_POINTS_IN_TRACK; j++ ) "
         * efficiently hence mannualy code is unrolled temprorarly assuming VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR = 6
         */
        wt10   = *stov_ptr(float16, &weight[0 * VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR]);
        wt32   = *stov_ptr(float16, &weight[1 * VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR]);
        wt54   = *stov_ptr(float16, &weight[2 * VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR]);
        wt76   = *stov_ptr(float16, &weight[3 * VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR]);
        wt98   = *stov_ptr(float16, &weight[4 * VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR]);
        wtba   = *stov_ptr(float16, &weight[5 * VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR]);

        int16 vcurLength = __convert_int16(*stov_ptr(char16,&curTrackLen[l]));

        vpred0 = __cmp_ge_pred(vcurLength, (int16)(VXLIB_TRIANG_MAX_POINTS_IN_TRACK - 0));
        vpred1 = __cmp_ge_pred(vcurLength, (int16)(VXLIB_TRIANG_MAX_POINTS_IN_TRACK - 1));
        vpred2 = __cmp_ge_pred(vcurLength, (int16)(VXLIB_TRIANG_MAX_POINTS_IN_TRACK - 2));
        vpred3 = __cmp_ge_pred(vcurLength, (int16)(VXLIB_TRIANG_MAX_POINTS_IN_TRACK - 3));
        vpred4 = __cmp_ge_pred(vcurLength, (int16)(VXLIB_TRIANG_MAX_POINTS_IN_TRACK - 4));
        vpred5 = __cmp_ge_pred(vcurLength, (int16)(VXLIB_TRIANG_MAX_POINTS_IN_TRACK - 5));

#pragma UNROLL(1)
        for( i = 0; i < 3; i++ ) {
            acc = (float16)0.0f;
#if 0
            for( j = 0; j < VXLIB_TRIANG_MAX_POINTS_IN_TRACK; j++ ) { /* VXLIB_TRIANG_MAX_POINTS_IN_TRACK = 6*/
                wt10   = *((float16*)&weight[j * VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR]);
                pAtA10 = *((float16*)matrixP_Atb);
                matrixP_Atb += VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR;
                acc    = acc + (wt10 * pAtA10);
            }
#else
            acc    = acc + __select(vpred0,(wt10 * __SE0ADV(float16)), (float16)0.0f);
            acc    = acc + __select(vpred1,(wt32 * __SE1ADV(float16)), (float16)0.0f);
            acc    = acc + __select(vpred2,(wt54 * __SE0ADV(float16)), (float16)0.0f);
            acc    = acc + __select(vpred3,(wt76 * __SE1ADV(float16)), (float16)0.0f);
            acc    = acc + __select(vpred4,(wt98 * __SE0ADV(float16)), (float16)0.0f);
            acc    = acc + __select(vpred5,(wtba * __SE1ADV(float16)), (float16)0.0f);
#endif
            *stov_ptr(float16, matrixAtb) = acc;
            matrixAtb           += VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR;
        }
        weight      += (VXLIB_TRIANG_MAX_POINTS_IN_TRACK *
                        VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR);
    }

    __SE0_CLOSE();
    __SE1_CLOSE();

}

/**
* @func findTriangWt_ci
*
* @par Description:
*   This API Calculates weights for data matrices ( A & b) after each iteration of
*   triangulation. After weighting, AtA and Atb are again calculated to find new
*   refined 3D location of given track. Weights are also packed together for two tracks.
*
* @par
*   @param [in]  outXcam          :3-D output generated after triangulation API. two 3D outputs are
*                               clubbed together. X(or Y or Z) co-ordinates are clubbed together.
*                               e.g X1X0Y1Y0Z1Z0X3X2Y3Y2Z3Z2....so on
*   @param [in]  normCamExtPrm :Normalized projection matrix
*   @param [out] weights       :Output data matrix A of equation Ax=b
*   @param [in]  curTrackLength:Array of values for track length
*   @param [in]  totalTracks   :Total number of tracks
*
* @par Assumptions:
*    -   NA
*
* @par Implementation Notes:
*    - NA
*
*/

//CODE_SECTION(findTriangWt_ci,  ".text:optimized")
static void findTriangWt_ci(VXLIB_F32 outXcam[restrict],
                            VXLIB_F32 normCamExtPrm[restrict],
                            VXLIB_F32 weights[restrict],
                            int32_t totalTracks)
{
    int32_t       i, l;
    int32_t       pmatrixIndx;
    VXLIB_F32     *normCamExtPrmL;
    uint8_t       curTrackLengthMax;
    float16       temp1f2, temp2f2, temp3f2, temp4f2;
    float16       X1X0, Y1Y0, Z1Z0, W1W0;
    VXLIB_F32     * restrict weightsL = (VXLIB_F32*)weights;


    for( l = 0; l < totalTracks; l += VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR ) {
        // TODO find the maximum track length, needs to be replaced by intrinsics
        weightsL = (VXLIB_F32*)weights;

        //vtrackLength    = __convert_int16(*((char16*)&curTrackLength[l]));
        //curTrackLengthMax  = __sort_desc(vtrackLength).s[0];

        curTrackLengthMax = VXLIB_TRIANG_MAX_POINTS_IN_TRACK;

        pmatrixIndx        = VXLIB_TRIANG_MAX_POINTS_IN_TRACK - curTrackLengthMax;
        normCamExtPrmL     = normCamExtPrm + (pmatrixIndx * VXLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE);

        X1X0               = *stov_ptr(float16, outXcam);
        outXcam           += VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR;

        Y1Y0               = *stov_ptr(float16, outXcam);
        outXcam           += VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR;

        Z1Z0               = *stov_ptr(float16, outXcam);
        outXcam           += VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR;

        //for( i = VXLIB_TRIANG_MAX_POINTS_IN_TRACK - curTrackLengthMax; i < VXLIB_TRIANG_MAX_POINTS_IN_TRACK; i++ ) {
        /* If track length is not fixed then the loop overhead is comparable to core cycle, which doubles the
         * cycle performance. it is 49 + 6*trackLength.
         * Its bnificial to keep fixed length to make ii of 50 + 6*VXLIB_TRIANG_MAX_POINTS_IN_TRACK*totalTracks.
         * Loop is bottelneck on multiply.
         */
#pragma UNROLL(1)
        for( i = 0; i < VXLIB_TRIANG_MAX_POINTS_IN_TRACK; i++ ) {

            temp1f2  = (float16)*(&normCamExtPrmL[12]);
            temp2f2  = (float16)*(&normCamExtPrmL[13]);
            temp3f2  = (float16)*(&normCamExtPrmL[14]);
            temp4f2  = (float16)*(&normCamExtPrmL[15]);

            temp1f2  = X1X0 * temp1f2;
            temp2f2  = Y1Y0 * temp2f2;
            temp3f2  = Z1Z0 * temp3f2;

            temp2f2  = temp1f2 + temp2f2;
            temp3f2  = temp2f2 + temp3f2;
            temp4f2  = temp3f2 + temp4f2;

            /* Even if the change in weight is small then continue doing triangulation
             * dont break
             */
            W1W0  = VXLIB_oneByXVecF32(temp4f2);
            *stov_ptr(float16, &weightsL[(i * VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR)]) =  W1W0 * W1W0;
            normCamExtPrmL        += VXLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE;
        }
        weights += (VXLIB_TRIANG_MAX_POINTS_IN_TRACK * VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR);
    }
}

/**
* @func solveMatEq3x3_ci
*
* @par Description:
*   This API finds the solution x for Ax = b. Solution used here is
*   based on normal inverse calculation and multiplying with b. All
*   data A & b are interleaved for two tracks.
*
* @par
*   @param [in]  matAtAPtr  : Matrix A of the equation Ax=b.
*   @param [in]  matBPtr    : Matrix b of the equation Ax=b.
*   @param [out] resultPtr  : Final result of solution x
*   @param [out] valid      : Validity of each 3D point generated. It is
*                             possible that matrix A might be singular in that case
*                             solution can not be found.
*   @param [in]  totalTracks: Total number of tracks
*
* @par Assumptions:
*    -   NA
*
* @par Implementation Notes:
*    - NA
*
*/
//CODE_SECTION(solveMatEq3x3_ci,  ".text:optimized")
static void solveMatEq3x3_ci(VXLIB_F32 matAtAPtr[restrict],
                             VXLIB_F32 matBPtr[restrict],
                             VXLIB_F32 resultPtr[restrict],
                             uint8_t valid[restrict],
                             int32_t totalTracks)
{
    int32_t              ctr;
    float *restrict    matAtAPtrL = (float *)matAtAPtr;
    float *restrict    matBPtrL   = (float *)matBPtr;
    float *restrict    resultPtrL = (float *)resultPtr;
    float16              vfTemp1, vfTemp2, vfTemp3;

    for( ctr = 0; ctr < totalTracks; ctr += VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR ) {
        float16    a1, a2, a3, b2, b3, c3;
        float16    d1, d2, d3, d4, d5, d6;
        float16    det, invDet;
        float16    atb1, atb2, atb3;

        a1 = *stov_ptr(float16, matAtAPtrL);
        matAtAPtrL+=SIMD_WIDTH;

        a2 = *stov_ptr(float16, matAtAPtrL);
        matAtAPtrL+=SIMD_WIDTH;

        a3 = *stov_ptr(float16, matAtAPtrL);
        matAtAPtrL+=SIMD_WIDTH;

        b2 = *stov_ptr(float16, matAtAPtrL);
        matAtAPtrL+=SIMD_WIDTH;

        b3 = *stov_ptr(float16, matAtAPtrL);
        matAtAPtrL+=SIMD_WIDTH;

        c3 = *stov_ptr(float16, matAtAPtrL);
        matAtAPtrL+=SIMD_WIDTH;

        /* Co-factor calculation Starts
        */
        /* Row 1 b2*c3 - b3*b3*/
        d1 = (b2 * c3) - (b3 * b3);

        /* Row 1 b3*a3 - a2*c3*/
        d2 = (b3 * a3) - (a2 * c3);

        /* Row 1 a2*b3 - b2*a3*/
        d3 = (a2 * b3) - (b2 * a3);

        /* Row 1 a1*c3 - a3*a3*/
        d4 = (a1 * c3) - (a3 * a3);

        /* Row 1 a2*a3 - a1*b3 */
        d5 = (a2 * a3) - (a1 * b3);

        /* Row 1 a1*b2 - a2*a2*/
        d6 = (a1 * b2) - (a2 * a2);

        /* Co-factor calculation Ends
        */
        det    = ((a1 * d1) + (a3 * d3)) + (a2 * d2);

#if TRACE_ON
            PRINTV("det", __as_long8(det), PRINTV_FLOAT, PRINTV_HEX_OFF);
#endif

        /*
         * invDet = 1.0f/det;
         */
        invDet = __recip(det);
        invDet = invDet * ((float16)(2.0f) - (det * invDet));
        invDet = invDet * ((float16)(2.0f) - (det * invDet));
#if TRACE_ON
            PRINTV("invDet", __as_long8(invDet), PRINTV_FLOAT, PRINTV_HEX_OFF);
#endif
        /*
         * if(det > FLT_EPSILON)
         *   valid[i] = 0xff;
         * else
         *   valid[i] = 0x00;
         */
        __vpred temp0 = __cmp_le_pred((float16)FLT_EPSILON,det);
        temp0         = __decimate_char(temp0, temp0);
        temp0         = __decimate_char(temp0, temp0);

        *stov_ptr(char16, valid) = __as_char16(__expand(temp0).lo().lo());

        atb1   = *stov_ptr(float16, matBPtrL);
        matBPtrL+=SIMD_WIDTH;

        atb2   = *stov_ptr(float16, matBPtrL);
        matBPtrL+=SIMD_WIDTH;

        atb3   = *stov_ptr(float16, matBPtrL);
        matBPtrL+=SIMD_WIDTH;

        vfTemp1   = d1 * atb1;
        vfTemp2   = d2 * atb2;
        vfTemp3   = d3 * atb3;
        vfTemp1   = vfTemp1 + (vfTemp2 + vfTemp3);
        vfTemp1   = invDet * vfTemp1;
        *stov_ptr(float16, resultPtrL) = vfTemp1;
        resultPtrL+=SIMD_WIDTH;
#if TRACE_ON
            PRINTV("resultPtrL", __as_long8(*resultPtrL), PRINTV_FLOAT, PRINTV_HEX_OFF);
#endif

        vfTemp1   = d2 * atb1;
        vfTemp2   = d4 * atb2;
        vfTemp3   = d5 * atb3;
        vfTemp1   = vfTemp1 + (vfTemp2 + vfTemp3);
        vfTemp1   = invDet * vfTemp1;
        *stov_ptr(float16, resultPtrL) = vfTemp1;
        resultPtrL+=SIMD_WIDTH;
#if TRACE_ON
            PRINTV("resultPtrL", __as_long8(*resultPtrL), PRINTV_FLOAT, PRINTV_HEX_OFF);
#endif

        vfTemp1   = d3 * atb1;
        vfTemp2   = d5 * atb2;
        vfTemp3   = d6 * atb3;
        vfTemp1   = vfTemp1 + (vfTemp2 + vfTemp3);
        vfTemp1   = invDet * vfTemp1;
        *stov_ptr(float16, resultPtrL) = vfTemp1;
        resultPtrL+=SIMD_WIDTH;
#if TRACE_ON
            PRINTV("resultPtrL", __as_long8(*resultPtrL), PRINTV_FLOAT, PRINTV_HEX_OFF);
#endif

        valid      += VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR;
    }
}

/**
* @func solve3x3MatEqDouble
*
* @par Description:
*   This API finds the solution x for Ax = b. Solution used here is
*   based on normal inverse calculation and multiplying with b. All
*   data A & b are interleaved for two tracks.
*
* @par
*   @param [in]  matAtAPtr  : Matrix A of the equation Ax=b.
*   @param [in]  matBPtr    : Matrix b of the equation Ax=b.
*   @param [out] resultPtr  : Final result of solution x
*   @param [out] valid      : Validity of each 3D point generated. It is
*                             possible that matrix A might be singular in that case
*                             solution can not be found.
*   @param [in]  totalTracks: Total number of tracks
*
* @par Assumptions:
*    -   NA
*
* @par Implementation Notes:
*    - NA
*
*/
//CODE_SECTION(solve3x3MatEqDouble,  ".text:optimized")
static void solve3x3MatEqDouble(VXLIB_F32 matAtAPtr[restrict],
                                VXLIB_F32 matBPtr[restrict],
                                VXLIB_F32 resultPtr[restrict],
                                uint8_t valid[restrict],
                                int32_t totalTracks)
{
    int32_t              ctr,ctr2;
    float *restrict      matAtAPtrL;
    float *restrict      matBPtrL;
    float *restrict      resultPtrL;
    char  *restrict      validL;
    double8              vdtemp1, vdtemp2, vdtemp3;
    float16              vfTemp1;
    __vpred              vpredPatternW;
    __vpred              vpredPatternB;
    char64               c64temp1;

    double8    a1, a2, a3, b2, b3, c3;
    double8    d1, d2, d3, d4, d5, d6;
    double8    det, invDet;
    double8    atb1, atb2, atb3;

    // Added dummy initialization for klockworks error
    vfTemp1 = __vdupw_kv((float)(FLT_MAX));

    vfTemp1.s[0] = FLT_MIN;vfTemp1.s[1] = FLT_MAX;vfTemp1.s[2] = FLT_MIN;vfTemp1.s[3] = FLT_MAX;
    vfTemp1.s[4] = FLT_MIN;vfTemp1.s[5] = FLT_MAX;vfTemp1.s[6] = FLT_MIN;vfTemp1.s[7] = FLT_MAX;
    vfTemp1.s[8] = FLT_MIN;vfTemp1.s[9] = FLT_MAX;vfTemp1.s[10] = FLT_MIN;vfTemp1.s[11] = FLT_MAX;
    vfTemp1.s[12] = FLT_MIN;vfTemp1.s[13] = FLT_MAX;vfTemp1.s[14] = FLT_MIN;vfTemp1.s[15] = FLT_MAX;

    /*
     * vpred register can not be written directly, hence used instruction to assign the desired pattern
     * vpredPatternW :: .....000011110000111100001111
     * vpredPatternB :: .....010101
     */
    vpredPatternW = __cmp_lt_pred(vfTemp1,(float16)FLT_MAX);
    vpredPatternB = __decimate_char(vpredPatternW,vpredPatternW);
    vpredPatternB = __decimate_char(vpredPatternB,vpredPatternB);

    for(ctr2 = 0; ctr2 < 2; ctr2++){

      matAtAPtrL = (float *)(matAtAPtr + ctr2);
      matBPtrL   = (float *)(matBPtr   + ctr2);
      resultPtrL = (float *)(resultPtr + ctr2);
      validL     = (char  *)(valid     + ctr2);

      for( ctr = 0; ctr < totalTracks; ctr += VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR ){

        /*
         * Each low 32 bit word (float) in 64 bit data is converted into double
         */
        a1 = __low_float_to_double(*stov_ptr(float16,(float*)&matAtAPtrL[0*VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR]));
        a2 = __low_float_to_double(*stov_ptr(float16,(float*)&matAtAPtrL[1*VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR]));
        a3 = __low_float_to_double(*stov_ptr(float16,(float*)&matAtAPtrL[2*VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR]));
        b2 = __low_float_to_double(*stov_ptr(float16,(float*)&matAtAPtrL[3*VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR]));
        b3 = __low_float_to_double(*stov_ptr(float16,(float*)&matAtAPtrL[4*VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR]));
        c3 = __low_float_to_double(*stov_ptr(float16,(float*)&matAtAPtrL[5*VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR]));

        /* Co-factor calculation Starts
        */
        /* Row 1 b2*c3 - b3*b3*/
        d1 = (b2 * c3) - (b3 * b3);

        /* Row 1 b3*a3 - a2*c3*/
        d2 = (b3 * a3) - (a2 * c3);

        /* Row 1 a2*b3 - b2*a3*/
        d3 = (a2 * b3) - (b2 * a3);

        /* Row 1 a1*c3 - a3*a3*/
        d4 = (a1 * c3) - (a3 * a3);

        /* Row 1 a2*a3 - a1*b3 */
        d5 = (a2 * a3) - (a1 * b3);

        /* Row 1 a1*b2 - a2*a2*/
        d6 = (a1 * b2) - (a2 * a2);

        /* Co-factor calculation Ends
        */
        det    = ((a1 * d1) + (a3 * d3)) + (a2 * d2);

        /* TODO : Increase the precision of division
        */
        invDet = __recip(det);

        invDet = invDet * ((double8)(2.0f) - (det * invDet));
        invDet = invDet * ((double8)(2.0f) - (det * invDet));

        __vpred temp0 = __cmp_lt_pred((double8)DBL_EPSILON,det);

        temp0         = __decimate_int(temp0,temp0);
        temp0         = __decimate_char(temp0, temp0);
        temp0         = __decimate_char(temp0, temp0);

        c64temp1      = __expand(temp0);
        c64temp1      = __shuffle_stride2(c64temp1);
        __vstore_pred(vpredPatternB,stov_ptr(char16,(char*)validL),c64temp1.lo().lo());


        atb1   = __low_float_to_double(*stov_ptr(float16,(float*)&matBPtrL[0*VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR]));
        atb2   = __low_float_to_double(*stov_ptr(float16,(float*)&matBPtrL[1*VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR]));
        atb3   = __low_float_to_double(*stov_ptr(float16,(float*)&matBPtrL[2*VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR]));

        vdtemp1   = d1 * atb1;
        vdtemp2   = d2 * atb2;
        vdtemp3   = d3 * atb3;
        vdtemp1   = vdtemp1 + (vdtemp2 + vdtemp3);
        vdtemp1   = invDet * vdtemp1;
        vfTemp1  = __double_to_float(vdtemp1);

        __vstore_pred(vpredPatternW,stov_ptr(float16,(float*)(resultPtrL)),vfTemp1);

        resultPtrL+=VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR;

        vdtemp1   = d2 * atb1;
        vdtemp2   = d4 * atb2;
        vdtemp3   = d5 * atb3;
        vdtemp1   = vdtemp1 + (vdtemp2 + vdtemp3);
        vdtemp1   = invDet * vdtemp1;
        vfTemp1  = __double_to_float(vdtemp1);

        __vstore_pred(vpredPatternW,stov_ptr(float16,(float*)(resultPtrL)),vfTemp1);


        resultPtrL+=VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR;

        vdtemp1   = d3 * atb1;
        vdtemp2   = d5 * atb2;
        vdtemp3   = d6 * atb3;
        vdtemp1   = vdtemp1 + (vdtemp2 + vdtemp3);
        vdtemp1   = invDet * vdtemp1;
        vfTemp1  = __double_to_float(vdtemp1);

        __vstore_pred(vpredPatternW,stov_ptr(float16,(float*)(resultPtrL)),vfTemp1);

        resultPtrL+=VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR;

        validL      += VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR;
        matAtAPtrL  += 6*VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR;
        matBPtrL    += 3*VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR;
      }
    }
}

/**
* @func solve3x3MatEqDoubleSelect
*
* @par Description:
*   This API finds the solution x for Ax = b. Solution used here is
*   based on normal inverse calculation and multiplying with b. All
*   data A & b are interleaved for two tracks.
*
* @par
*   @param [in]  matAtAPtr  : Matrix A of the equation Ax=b.
*   @param [in]  matBPtr    : Matrix b of the equation Ax=b.
*   @param [out] resultPtr  : Final result of solution x
*   @param [out] valid      : Validity of each 3D point generated. It is
*                             possible that matrix A might be singular in that case
*                             solution can not be found.
*   @param [in]  totalTracks  : Total number of tracks
*
* @par Assumptions:
*    -   NA
*
* @par Implementation Notes:
*    - NA
*
*/
//CODE_SECTION(solve3x3MatEqDoubleSelect,  ".text:optimized")
static void solve3x3MatEqDoubleSelect(VXLIB_F32 matAtAPtr[restrict],
                                      VXLIB_F32 matBPtr[restrict],
                                      VXLIB_F32 resultPtr[restrict],
                                      uint8_t valid[restrict],
                                      int32_t totalTracks)
{
    int32_t    inPitchA = VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR;
    int32_t    inPitchB = VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR;
    int32_t    outPitch = VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR;

    VXLIB_D64    det, invDet;
    VXLIB_D64    d1, d2, d3, d4, d5, d6;
    VXLIB_D64    a1, a2, a3, b2, b3, c3;
    VXLIB_D64    atB1, atB2, atB3;
    int32_t     k, l;

    /* a1 a2 a3
       b2 b3
       c3
    */
    for( l = 0; l < totalTracks; l+=VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR ) {
        for( k = 0; k < VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR; k++ ) {
            if( valid[k] == 0 ) {
                a1    = (VXLIB_D64)matAtAPtr[(0 * inPitchA) + k];
                a2    = (VXLIB_D64)matAtAPtr[(1 * inPitchA) + k];
                a3    = (VXLIB_D64)matAtAPtr[(2 * inPitchA) + k];
                b2    = (VXLIB_D64)matAtAPtr[(3 * inPitchA) + k];
                b3    = (VXLIB_D64)matAtAPtr[(4 * inPitchA) + k];
                c3    = (VXLIB_D64)matAtAPtr[(5 * inPitchA) + k];

                atB1  = (VXLIB_D64)matBPtr[(0 * inPitchB) + k];
                atB2  = (VXLIB_D64)matBPtr[(1 * inPitchB) + k];
                atB3  = (VXLIB_D64)matBPtr[(2 * inPitchB) + k];

                d1    = (b2 * c3) - (b3 * b3);
                d2    = (b3 * a3) - (a2 * c3);
                d3    = (a2 * b3) - (b2 * a3);
                d4    = (a1 * c3) - (a3 * a3);
                d5    = (a2 * a3) - (a1 * b3);
                d6    = (a1 * b2) - (a2 * a2);

                det   = (a1 * d1) + ((a3 * d3) + (a2 * d2));

                if( det <= DBL_EPSILON ) {
                    valid[k] = (int32_t)0x0;
                } else {
                    valid[k] = (int32_t)0xff;
                }

                invDet = (VXLIB_D64)my_OneByX((VXLIB_F32)det);

                resultPtr[(0 * outPitch) + k] = (VXLIB_F32)(invDet * ((d1 * atB1) + ((d2 * atB2) + (d3 * atB3))));
                resultPtr[(1 * outPitch) + k] = (VXLIB_F32)(invDet * ((d2 * atB1) + ((d4 * atB2) + (d5 * atB3))));
                resultPtr[(2 * outPitch) + k] = (VXLIB_F32)(invDet * ((d3 * atB1) + ((d5 * atB2) + (d6 * atB3))));
            }
        }

        matAtAPtr  += (VXLIB_TRIANG_NUM_UNIQUE_ELEMENTS_IN_ATA * VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR);
        matBPtr    += (VXLIB_TRIANG_MAT_ROW * VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR);
        resultPtr  += (3 * VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR);
        valid      += VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR;
    }
}



//CODE_SECTION(VXLIB_triangulatePoints_i32f_o32f_kernel,  ".text:optimized")
VXLIB_STATUS VXLIB_triangulatePoints_i32f_o32f_kernel(VXLIB_F32 curTrack[restrict],
                                             VXLIB_F32 camExtPrm[restrict],
                                             uint8_t   curTrackLength[restrict],
                                             int32_t  maxIter,
                                             VXLIB_F32 scratch[restrict],
                                             int32_t  totalTracks,
                                             int32_t  precisionFlag,
                                             VXLIB_F32 outXcam[restrict],
                                             uint8_t   outValid[restrict],
                                             void *pBlock)

{

    int32_t    i, j, l, k;
    int32_t    maxLength;

    VXLIB_F32 *restrict    matrixA;     /*[VXLIB_TRIANG_MAT_ROW][VXLIB_TRIANG_MAT_COL]                              [VXLIB_TRIANG_NUM_TRACKS_TOGATHER] - 12x3xN*/
    VXLIB_F32 *restrict    matrixb;     /*[VXLIB_TRIANG_MAT_COL]                                                    [VXLIB_TRIANG_NUM_TRACKS_TOGATHER] - 12xN  */
    VXLIB_F32 *restrict    matrixP_AtA; /*[VXLIB_TRIANG_NUM_UNIQUE_ELEMENTS_IN_ATA*VXLIB_TRIANG_MAX_POINTS_IN_TRACK][VXLIB_TRIANG_NUM_TRACKS_TOGATHER] - 6x6xN */
    VXLIB_F32 *restrict    matrixP_Atb; /*[VXLIB_TRIANG_MAT_ROW*VXLIB_TRIANG_MAX_POINTS_IN_TRACK]                   [VXLIB_TRIANG_NUM_TRACKS_TOGATHER] - 3x6xN */

    VXLIB_F32 *restrict    matrixAtA; /*[VXLIB_TRIANG_NUM_UNIQUE_ELEMENTS_IN_ATA][VXLIB_TRIANG_NUM_TRACKS_TOGATHER] - 6xN*/
    VXLIB_F32 *restrict    matrixAtb; /*[VXLIB_TRIANG_MAT_ROW]                   [VXLIB_TRIANG_NUM_TRACKS_TOGATHER] - 3xN*/
    VXLIB_F32 *restrict    weights;   /*[VXLIB_TRIANG_MAX_POINTS_IN_TRACK]       [VXLIB_TRIANG_NUM_TRACKS_TOGATHER] - 6xN*/

    VXLIB_STATUS    status = VXLIB_SUCCESS;

#ifdef TIADALG_TRIANG_PROFILE_EN
    acc0 = 0;
    acc1 = 0;
    acc2 = 0;
    acc3 = 0;
    acc4 = 0;
    acc5 = 0;
    acc6 = 0;
    acc7 = 0;
#endif

#if defined(VXLIB_CHECK_PARAMS)
    status = VXLIB_triangulatePoints_i32f_o32f_checkParams(curTrack, camExtPrm, curTrackLength, maxIter, scratch, totalTracks, precisionFlag, outXcam, outValid);
    if( status == VXLIB_SUCCESS )
#endif
    {

      matrixA     = scratch;
      matrixb     = (VXLIB_F32 *)matrixA + (VXLIB_TRIANG_MAT_ROW * VXLIB_TRIANG_MAT_COL * totalTracks);
      matrixP_AtA = (VXLIB_F32 *)matrixb + (VXLIB_TRIANG_MAT_COL * totalTracks);
      matrixP_Atb = (VXLIB_F32 *)matrixP_AtA + (VXLIB_TRIANG_NUM_UNIQUE_ELEMENTS_IN_ATA * VXLIB_TRIANG_MAX_POINTS_IN_TRACK * totalTracks);

      matrixAtA   = (VXLIB_F32 *)scratch;
      matrixAtb   = (VXLIB_F32 *)matrixAtA + (VXLIB_TRIANG_NUM_UNIQUE_ELEMENTS_IN_ATA * totalTracks);
      weights     = (VXLIB_F32 *)matrixAtb + (VXLIB_TRIANG_MAX_POINTS_IN_TRACK * totalTracks);

#ifdef TIADALG_TRIANG_PROFILE_EN
     t0 = _TSC_read();
#endif
      makeTriangMatrix_ci((VXLIB_F32 *)curTrack,
                          (VXLIB_F32 *)camExtPrm,
                          (VXLIB_F32 *)matrixA,
                          matrixb,
                          //curTrackLength,
                          totalTracks,
                          pBlock);

#if TRACE_ON
            PRINTV("matrixA[4]", __as_long8(*((float16*)(matrixA + 4))), PRINTV_FLOAT, PRINTV_HEX_OFF);
            PRINTV("matrixA[5]", __as_long8(*((float16*)(matrixA + 5))), PRINTV_FLOAT, PRINTV_HEX_OFF);
#endif

#ifdef TIADALG_TRIANG_PROFILE_EN
     t1    = _TSC_read();
     acc0  = (t1 - t0);
#endif

#ifdef TIADALG_TRIANG_PROFILE_EN
     t0 = _TSC_read();
#endif

      getPseudoMatrices_ci(matrixA,
                           matrixb,
                           matrixP_AtA,
                           matrixP_Atb,
                           //curTrackLength,
                           totalTracks,
                           pBlock);

#if TRACE_ON
            PRINTV("matrixP_AtA[2]", __as_long8(*((float16*)(matrixP_AtA +2))), PRINTV_FLOAT, PRINTV_HEX_OFF);
            PRINTV("matrixP_AtA[3]", __as_long8(*((float16*)(matrixP_AtA +3))), PRINTV_FLOAT, PRINTV_HEX_OFF);
#endif

#ifdef TIADALG_TRIANG_PROFILE_EN
     t1    = _TSC_read();
     acc1  = (t1 - t0);
#endif

#ifdef TIADALG_TRIANG_PROFILE_EN
     t0 = _TSC_read();
#endif

      for( l = 0; l < totalTracks; l+= VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR ) {
          for( k = 0; k < VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR; k++ ) {
              outValid[l + k] = (uint8_t)0x1;
              maxLength       = (int32_t)curTrackLength[l + k];
#pragma UNROLL(VXLIB_TRIANG_MAX_POINTS_IN_TRACK)
              for( j = 0; j < VXLIB_TRIANG_MAX_POINTS_IN_TRACK; j++ ) {
                  if( j >= (VXLIB_TRIANG_MAX_POINTS_IN_TRACK - maxLength)) {
                      weights[(l * VXLIB_TRIANG_MAX_POINTS_IN_TRACK) + ((j * VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR) + k)] = 1.0f;
                  } else {
                      weights[(l * VXLIB_TRIANG_MAX_POINTS_IN_TRACK) + ((j * VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR) + k)] = 0.0f;
                  }
              }
          }
      }

#ifdef TIADALG_TRIANG_PROFILE_EN
     t1 = _TSC_read();
     acc6  = (t1-t0);
#endif

      for( i = 0; i < maxIter; i++ ) {

#ifdef TIADALG_TRIANG_PROFILE_EN
     t0 = _TSC_read();
#endif
          getFinalMatrices_ci(matrixP_AtA,
                              matrixP_Atb,
                              matrixAtA,
                              matrixAtb,
                              weights,
                              curTrackLength,
                              totalTracks,
                              pBlock);
#ifdef TIADALG_TRIANG_PROFILE_EN
     t1    = _TSC_read();
     acc2 += (t1 - t0);
#endif
#if TRACE_ON
            PRINTV("matrixAtA[0]", __as_long8(*((float16*)matrixAtA + 0)), PRINTV_FLOAT, PRINTV_HEX_OFF);
            PRINTV("matrixAtA[1]", __as_long8(*((float16*)matrixAtA + 1)), PRINTV_FLOAT, PRINTV_HEX_OFF);
            PRINTV("matrixAtA[2]", __as_long8(*((float16*)matrixAtA + 2)), PRINTV_FLOAT, PRINTV_HEX_OFF);
            PRINTV("matrixAtA[3]", __as_long8(*((float16*)matrixAtA + 3)), PRINTV_FLOAT, PRINTV_HEX_OFF);
            PRINTV("matrixAtA[4]", __as_long8(*((float16*)matrixAtA + 4)), PRINTV_FLOAT, PRINTV_HEX_OFF);
            PRINTV("matrixAtA[5]", __as_long8(*((float16*)matrixAtA + 5)), PRINTV_FLOAT, PRINTV_HEX_OFF);
#endif

          if( precisionFlag == 0x0 ) {

#ifdef TIADALG_TRIANG_PROFILE_EN
     t0 = _TSC_read();
#endif

              solveMatEq3x3_ci(matrixAtA,
                               matrixAtb,
                               outXcam,
                               outValid,
                               totalTracks);
#ifdef TIADALG_TRIANG_PROFILE_EN
     t1    = _TSC_read();
     acc3 += (t1 - t0);
#endif

#ifdef TIADALG_TRIANG_PROFILE_EN
     t0 = _TSC_read();
#endif

              solve3x3MatEqDoubleSelect(matrixAtA,
                                        matrixAtb,
                                        outXcam,
                                        outValid,
                                        totalTracks);

#ifdef TIADALG_TRIANG_PROFILE_EN
     t1    = _TSC_read();
     acc7 += (t1 - t0);
#endif

          } else {
              solve3x3MatEqDouble(matrixAtA,
                                  matrixAtb,
                                  outXcam,
                                  outValid,
                                  totalTracks);
          }

#ifdef TIADALG_TRIANG_PROFILE_EN
     t0 = _TSC_read();
#endif

          findTriangWt_ci(outXcam,
                          camExtPrm,
                          weights,
                          totalTracks);
#ifdef TIADALG_TRIANG_PROFILE_EN
     t1    = _TSC_read();
     acc4 += (t1 - t0);
#endif

      }

#ifdef TIADALG_TRIANG_PROFILE_EN
     t0 = _TSC_read();
#endif

      VXLIB_F32 * restrict outXcamL = outXcam;
      uint8_t   *restrict outValidL = outValid;

      for( l = 0; l < totalTracks; l+= VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR ) {
          /* only 16 bytes are useful in below read, as of now if just reading of
             16 bytes is done then some crash in compiler happens.
           */
          char64    vOutValid16 = *stov_ptr(char64,&outValidL[l]);

          __vpred pred = __cmp_eq_pred(vOutValid16, (char64)0);
                  pred = __pdupl2b_pp(pred);
                  pred = __pdupl2b_pp(pred);
#if TRACE_ON
            PRINTV("cur3DPos", __as_long8(*cur3DPos), PRINTV_FLOAT, PRINTV_HEX_OFF);
            PRINTV("cur3DPos", __as_long8(*(cur3DPos+1)), PRINTV_FLOAT, PRINTV_HEX_OFF);
            PRINTV("cur3DPos", __as_long8(*(cur3DPos+2)), PRINTV_FLOAT, PRINTV_HEX_OFF);
#endif

          __vstore_pred(pred,stov_ptr(float16,(float*)&outXcamL[l * 3]), (float16)0.0f);
          __vstore_pred(pred,stov_ptr(float16,(float*)&outXcamL[l * 3 + 1*VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR]), (float16)0.0f);
          __vstore_pred(pred,stov_ptr(float16,(float*)&outXcamL[l * 3 + 2*VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR]), (float16)0.0f);
      }
#ifdef TIADALG_TRIANG_PROFILE_EN
     t1 = _TSC_read();
     acc5 = (t1-t0);
#endif
    }

#ifdef TIADALG_TRIANG_PROFILE_EN
    printf(" Total Number of tracks are %d \n", totalTracks);
    printf("makeTriangMatrix_ci = %8lld getPseudoMatrices_ci = %8lld getFinalMatrices_ci = %8lld \n \
           weightInit = %8lld findTriangWt_ci = %8lld solveMatEq3x3_ci = %8lld \n \
           outputCollection = %8lld, doubleSelect = %8lld \n", acc0,acc1,acc2,acc6,acc4,acc3,acc5,acc7);
#endif

    return (status);
}

VXLIB_STATUS VXLIB_triangulatePoints_i32f_o32f_init(VXLIB_F32 curTrack[],
                                             VXLIB_F32 camExtPrm[],
                                             uint8_t   curTrackLength[],
                                             int32_t  maxIter,
                                             VXLIB_F32 scratch[],
                                             int32_t  totalTracks,
                                             int32_t  precisionFlag,
                                             VXLIB_F32 outXcam[],
                                             uint8_t   outValid[],
                                             void *pBlock)
{
    VXLIB_STATUS    status = VXLIB_SUCCESS;

    if((curTrack == NULL)||(camExtPrm==NULL)||(curTrackLength==NULL) ||
       (scratch == NULL) || (outXcam == NULL) || (outValid == NULL) || (pBlock == NULL)
    ){
        return(VXLIB_ERR_FAILURE);
    }

    if((maxIter == 0) || (totalTracks == 0) || ((precisionFlag != 0) && (precisionFlag != 1))){
        return(VXLIB_ERR_FAILURE);
    }

#if defined(VXLIB_CHECK_PARAMS) || defined(VXLIB_TRIANGUKATEPOINTS_I32F_O32F_CHECK_PARAMS)
    status = VXLIB_triangulatePoints_i32f_o32f_checkParams(curTrack,
                                                           camExtPrm,
                                                           curTrackLength,
                                                           maxIter,
                                                           scratch,
                                                           totalTracks,
                                                           precisionFlag,
                                                           outXcam,
                                                           outValid);
    if( status == VXLIB_SUCCESS )
#endif
    {
      __SE_TEMPLATE_v1 *  pBlockTemplate = (__SE_TEMPLATE_v1 *)pBlock;

      __SE_TEMPLATE_v1 se0_param = __gen_SE_TEMPLATE_v1();

      /* SE0 programming. Continious float data read*/
      /* Making generic SE engine which fetches linear float16 or int16 data*/

      se0_param.DIMFMT    = __SE_DIMFMT_3D;
      se0_param.ELETYPE   = __SE_ELETYPE_32BIT;
      se0_param.VECLEN    = __SE_VECLEN_16ELEMS;
      se0_param.ICNT0     = 0x7fffffffu;
      se0_param.ICNT1     = 1;
      se0_param.ICNT2     = 1;
      se0_param.DIM1      = 0;
      se0_param.DIM2      = 0;
      pBlockTemplate[0]   = se0_param;

      se0_param.ELEDUP   = __SE_ELEDUP_16X;
      se0_param.ICNT0    = VXLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_SIZE >> 1;
      se0_param.ICNT1    = VXLIB_TRIANG_MAX_POINTS_IN_TRACK;
      se0_param.ICNT2    = (int)((float)totalTracks * my_OneByX(VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR));
      se0_param.DIM1     = VXLIB_TRIANG_CAMERA_EXTRENSIC_PARAM_ASIZE;
      se0_param.DIM2     = 0;
      pBlockTemplate[1]  = se0_param;

      se0_param.DIMFMT   = __SE_DIMFMT_4D;
      se0_param.ELEDUP   = __SE_ELEDUP_OFF;
      se0_param.ICNT0    = VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR;
      se0_param.ICNT1    = 3;
      se0_param.ICNT2    = VXLIB_TRIANG_MAX_POINTS_IN_TRACK;
      se0_param.ICNT3    = (int)((float)totalTracks * my_OneByX(VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR));
      se0_param.DIM1     = VXLIB_TRIANG_MAX_POINTS_IN_TRACK*2*VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR;
      se0_param.DIM2     = VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR*2;
      se0_param.DIM3     = (VXLIB_TRIANG_MAT_ROW *(VXLIB_TRIANG_MAT_COL * VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR));
      pBlockTemplate[2]  = se0_param;

      se0_param.DIMFMT   = __SE_DIMFMT_3D;
      se0_param.ELEDUP   = __SE_ELEDUP_OFF;
      se0_param.ICNT0    = 2*VXLIB_TRIANG_MAX_POINTS_IN_TRACK*VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR;
      se0_param.ICNT1    = 3;
      se0_param.ICNT2    = (int)((float)totalTracks * my_OneByX(VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR));
      se0_param.DIM1     = 0;
      se0_param.DIM2     = 2*VXLIB_TRIANG_MAX_POINTS_IN_TRACK*VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR;
      pBlockTemplate[3]  = se0_param;


      se0_param.DIMFMT   = __SE_DIMFMT_3D;
      se0_param.ELEDUP   = __SE_ELEDUP_OFF;
      se0_param.ICNT0    = VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR;
      se0_param.ICNT1    = 0x7fffffffu;
      se0_param.ICNT2    = 1;
      se0_param.DIM1     = 32;
      se0_param.DIM2     = 0;
      pBlockTemplate[4]  = se0_param;

      se0_param.DIMFMT   = __SE_DIMFMT_4D;
      se0_param.ELEDUP   = __SE_ELEDUP_OFF;
      se0_param.ICNT0    = VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR;
      se0_param.ICNT1    = 3;
      se0_param.ICNT2    = 3;
      se0_param.ICNT3    = 0x7fffffffu;
      se0_param.DIM1     = VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR;
      se0_param.DIM2     = 0;
      se0_param.DIM3     = 3*VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR;
      pBlockTemplate[5]  = se0_param;

    }
    return (status);
}

VXLIB_STATUS VXLIB_triangulatePoints_i32f_o32f_checkParams(VXLIB_F32 curTrack[],
                                             VXLIB_F32 camExtPrm[],
                                             uint8_t   curTrackLength[],
                                             int32_t   maxIter,
                                             VXLIB_F32 scratch[],
                                             int32_t   totalTracks,
                                             int32_t   precisionFlag,
                                             VXLIB_F32 outXcam[],
                                             uint8_t   outValid[]){
    VXLIB_STATUS    status = VXLIB_SUCCESS;

    _nassert(VXLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR == 16);

    if((curTrack == NULL) || (camExtPrm == NULL) || (curTrackLength == NULL) ||
        (scratch == NULL) || (outXcam == NULL) || (outValid == NULL)
        ){
      status = VXLIB_ERR_FAILURE;
    }

    if((maxIter == 0x0U) || ((precisionFlag !=0) && (precisionFlag != 1))){
      status = VXLIB_ERR_FAILURE;
    }
     /* For N tracks total number of matrix A elements will be
      * N * 2* VXLIB_TRIANG_MAX_POINTS_IN_TRACK * 3 = 65535, which
      * just fits ICNT in streaming engine programming
      */

    if(totalTracks > my_YByX(65535,(2* VXLIB_TRIANG_MAX_POINTS_IN_TRACK * 3))){
     status = VXLIB_ERR_FAILURE;
    }

    return (status);
}

/* ======================================================================== */
/*  End of file:  VLIB_triangulatePoints.c                                  */
/* ======================================================================== */

