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

#ifndef _TIDL_IMPORT_LUT_COMMONH_
#define _TIDL_IMPORT_LUT_COMMONH_ 1

using namespace std;

#include "ti_dl.h"
#include <cmath>
#include "tidl_import_diag.h"
#include "tidl_import_lut.h"

#define NUM_ELEMENTS_PERBINS (256U)
#define ADJUST_NUM_NON_LINEAR_OPS (5U)

void TIDL_prepareXval(double *Xdata);

int32_t TIDL_getNonLinearActMethod(int32_t inDataType, int32_t outDataType, int32_t nonLinearactType, int32_t deviceID);

void TIDL_calculateFactors(int16_t *lut_factors, double *lut_scale);

template <typename Tin, typename Tout>
int32_t TIDL_getQuantizeLUT(Tin *inPtr, Tout *Y, int32_t bin, sTIDL_NonLinearAct_LUT_createParams_t *pLUTCreateParams);


template <class Tout>
void TIDL_quantizeLUT(double *double_LUTdata, Tout *LUTdata, double *scale, int32_t *quant_min_val, int32_t *quant_max_val);

// template <class Tin, class Tout>
// void TIDL_prepareSwInterpolLUT(Tin *inPtr, Tout *outPtr, int32_t actType, double *X, double *lutData,
//                                       int32_t in_zero_point, float32_tidl in_scale, int32_t out_zero_point, float32_tidl out_scale);

template <class Tin, class Tout>
void TIDL_prepareSwInterpolLUT(Tin *inPtr, Tout *outPtr, double *X, double *lutData, 
  sTIDL_NonLinearAct_LUT_createParams_t *pLUTCreateParams);

#endif /*_TIDL_IMPORT_LUT_COMMONH_ */