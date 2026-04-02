/*
* module name : Structure From Motion
*
* module descripton : Generates sparse 3D points from optical flow information in camera captured images
*
* Copyright (C) 2009-2021 Texas Instruments Incorporated - http://www.ti.com/
* ALL RIGHTS RESERVED
*
*/

/**
*  @file        sfm_ti_occpGrid.c
*  @author      Deepak Poddar
*  @brief       This file defines the functions related to occupancy grid calcualtion
*  @version     0.0 (Jan 2014) : Base version.
*/

#include "sfm_ti_occpGrid.h"

void SFM_TI_genOccpGrid(float* wp, float* cp, uint8_t* valid, int32_t ocpGrdSizeInPixels, int8_t* ogBufInt)
{
    int32_t og_row_idx, og_col_idx;
    float grid_scale = 10.0f;
    float cam_ht = 1.5f;
    int32_t cur_ocg;
    int32_t isValid = 0;
    int32_t l,kk;

    for (l = 0; l < NUM_TRACKS_TOGATHER; l += VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR)
    {
        for (kk = 0; kk < VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR; kk++)
        {
            if (valid[l + kk])
            {
                float wp_x = wp[0 * VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR + kk];
                //float wp_y = wp[1*VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR + kk];
                float wp_z = wp[2 * VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR + kk];

                float cp_x = cp[0 * VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR + kk];
                float cp_y = cp[1 * VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR + kk];
                float cp_z = cp[2 * VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR + kk];

                og_row_idx = (int32_t)(round(ocpGrdSizeInPixels - (wp_z * grid_scale)));
                og_col_idx = (int32_t)(round((wp_x * grid_scale) + (ocpGrdSizeInPixels >> 1)));
                isValid = 1;

                if ((cp_x > 40.0f) || (cp_x < -40.0f) || (cp_z < 2.9f) || (cp_z > 100.0f))
                {
                    isValid = 0;
                }

                if (og_row_idx < 0)
                {
                    isValid = 0;
                }

                if (og_col_idx < 0)
                {
                    isValid = 0;
                }

                if (og_row_idx > (ocpGrdSizeInPixels - 1))
                {
                    isValid = 0;
                }

                if (og_col_idx > (ocpGrdSizeInPixels - 1))
                {
                    isValid = 0;
                }
                if (isValid > 0)
                {
                    cur_ocg = ogBufInt[og_row_idx * ocpGrdSizeInPixels + og_col_idx];

                    if (cp_y < cam_ht)
                    {
                        cur_ocg += 9;
                    }
                    else
                    {
                        cur_ocg -= 9;
                    }

                    if (cur_ocg > 120)
                    {
                        cur_ocg = 120;
                    }

                    if (cur_ocg < -120)
                    {
                        cur_ocg = -120;
                    }

                    ogBufInt[og_row_idx * ocpGrdSizeInPixels + og_col_idx] = cur_ocg;
                }
            }
        }
        wp += (VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR * 3);
        cp += (VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR * 3);
    }
}

void SFM_TI_genOccpGrid_ci(float* restrict wp, float* restrict cp, uint8_t* restrict valid, int32_t ocpGrdSizeInPixels,
                           int8_t* restrict ogBufInt, uint32_t* restrict scratchBuf1, int8_t* restrict scratchBuf2, void*  pBlock)
{
    int16 og_row_idx, og_col_idx;
    int16 index16;
    float16 grid_scale = (float16)(10.0f);
    float16 cam_ht = (float16)(1.5f);
    int32_t cur_ocg;

    int16 ocpGrdSizeInPixelsPkd  = (int16)(ocpGrdSizeInPixels);
    int16 ocpGrdSizeInPixelsM1Pkd = (int16)((ocpGrdSizeInPixels-1));
    int16 ocpGrdSizeInPixelsPkdBy2 = __shift_right(ocpGrdSizeInPixelsPkd,(int16)(1));

    float16 cp_x_max = (float16)(40.0f);
    float16 cp_x_min = (float16)(-40.0f);
    float16 cp_z_max = (float16)(100.0f);
    float16 cp_z_min = (float16)(2.9f);
    char16  ninePkd  = (char16)9;
    char16  mninePkd = (char16)-9;
    int32_t l;
    int32_t totalValidPts = 0;

    uint32_t* restrict scratchBuf1L = scratchBuf1;
    int8_t* restrict scratchBuf2L = scratchBuf2;

    __SE_TEMPLATE_v1 seTemplate;

    seTemplate = *(__SE_TEMPLATE_v1*)((uint8_t *)pBlock + SE0_PARAM_OFFSET);

#ifdef TIADALG_OG_DEBUG
    displaySEParam(pBlock, seTemplate);
#endif

    __SE0_OPEN((void *)wp, seTemplate);

    seTemplate.ELETYPE = __SE_ELETYPE_8BIT;

    __SE1_OPEN((void *)valid, seTemplate);

    for (l = 0; l < NUM_TRACKS_TOGATHER; l += VLIB_TRIANG_NUM_TRACKS_IN_ONE_VECTOR)
    {

        float16 cp_x = __SE0ADV(float16);
        float16 cp_y = __SE0ADV(float16);
        float16 cp_z = __SE0ADV(float16);

        char16 valid = __SE1ADV(char16);

        og_row_idx = ocpGrdSizeInPixelsPkd - __float_to_int(cp_z * grid_scale);
        og_col_idx = __float_to_int(cp_x * grid_scale) + ocpGrdSizeInPixelsPkdBy2;
        index16    = (og_row_idx*ocpGrdSizeInPixelsPkd) + og_col_idx; // need to check 32 bit multiplication result in 32 bit or not

        vpred cmp1Result =  __and(__cmp_le_pred(cp_x, cp_x_max),__cmp_le_pred(cp_x_min, cp_x)); // 1b in compResult indicates valid point
        vpred cmp2Result =  __and(__cmp_le_pred(cp_z, cp_z_max),__cmp_le_pred(cp_z_min, cp_z));
        vpred cmp3Result =  __and(__cmp_ge_pred(og_row_idx,(int16)0),__cmp_ge_pred(ocpGrdSizeInPixelsM1Pkd,og_row_idx));
        vpred cmp4Result =  __and(__cmp_ge_pred(og_col_idx,(int16)0),__cmp_ge_pred(ocpGrdSizeInPixelsM1Pkd,og_col_idx));

        vpred isValid    =  __and(cmp4Result,__and(__and(cmp1Result,cmp2Result),cmp3Result)); // 1b in compResult indicates valid point

        vpred isPtValid  =  __cmp_eq_pred(valid,(char16)0x1);
              isPtValid  =  __duplicate_pred_low_char(__duplicate_pred_low_char(isPtValid)); // make it word level
                isValid  =  __and(isPtValid,isValid);

        int32_t numValid  = __popcount_char(isValid)>>2;
        char64 index64    = __gather_set_bits(isValid,__as_char64(index16));
               cp_y       = __as_float16(__gather_set_bits(isValid,__as_char64(cp_y)));

               cmp1Result = __cmp_lt_pred(cp_y, cam_ht);
               cmp1Result = __decimate_char(cmp1Result,cmp1Result);
               cmp1Result = __decimate_char(cmp1Result,cmp1Result);
        char16 ocpGdWt    = __select(cmp1Result, ninePkd, mninePkd);

        *stov_ptr(char64, scratchBuf1L) = index64;
        *stov_ptr(char16,scratchBuf2L)  = ocpGdWt;

        scratchBuf1L  += numValid;
        scratchBuf2L  += numValid;
        totalValidPts += numValid;


    }

    __SE0_CLOSE();
    __SE1_CLOSE();
    /* loop unrolling is giving much advantage*/
    for(l = 0 ; l < totalValidPts; l++)
    {
        int32_t index = scratchBuf1[l];
        cur_ocg = ogBufInt[index];
        int32_t curwt = scratchBuf2[l];
        cur_ocg = cur_ocg + curwt;

        if (cur_ocg > 120)
        {
            cur_ocg = 120;
        }

        if (cur_ocg < -120)
        {
            cur_ocg = -120;
        }

        ogBufInt[index] = cur_ocg;
    }
}

