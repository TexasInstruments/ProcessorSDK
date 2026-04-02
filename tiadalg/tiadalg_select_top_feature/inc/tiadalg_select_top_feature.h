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


/**
@file      tiadalg_select_top_feature.h
@brief     This file defines the interface for multiple element descriptor matching 
*/

/** @ingroup    TIADALG_SELECT_TOP_FEAT_H */
/*@{*/
#ifndef TIADALG_SELECT_TOP_FEAT_H
#define TIADALG_SELECT_TOP_FEAT_H
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @func tiadalg_select_top_feature_cn
 *
 * @brief It does 2 way matching of unsigned 16 or 8 bit descriptors to find top matches. API is tested for KAZE descriptor, however 
 * it is not limited to KAZE. Matching happens between two sets/lists of descriptors. Length of each descriptor is configurable.
 *
 * @param desc_a_list          [in]  : List 'A' of discriptors in linear memory
 * @param desc_b_list          [in]  : List 'B' of discriptors in linear memory
 * @param data_type            [in]  : data type of descriptor. 0 vlaue indicates unsigned 8 bit, otherwise unsigned 16 bit
 * @param desc_a_list_offset   [in]  : Indices of descriptors in list 'A' to be considered for matching
 * @param desc_size            [in]  : Number of uint16/uint8 elements in one descriptor as per data type
 * @param num_desc_a           [in]  : Total number of descriptors in list 'A'
 * @param num_desc_b           [in]  : Total number of descriptors in list 'B'
 * @param num_top              [in]  : Total top matches to be found
 * @param top_a_indx           [out] : List of indices of top matched descriptors in list 'A'
 * @param top_b_indx           [out] : List of indices of top matched descriptors in list 'B'
 * @param scartch1             [in]  : Scratch memory needed by API of size 4* \ref num_desc_b
 * @param scratch2             [in]  : Scratch memory needed by API of size 2* \ref num_desc_a
 * @param scratch3             [in]  : Scratch memory needed by API of size 2* \ref num_desc_b
 * @param scratch4             [in]  : Scratch memory needed by API of size 4* \ref num_desc_b
 *
 * @note
 * - API returns total number of matches. It should be less than or equal to \ref num_top
 * - Valid entries in \ref top_a_indx \ref top_b_indx is equal to total number of matches
 * - i <SUP>th</SUP> match is the \ref top_a_indx [i] <SUP>th</SUP> descriptor from list 'A' and \ref top_b_indx [i] <SUP>th</SUP> descriptor from list 'B'
 */

int32_t tiadalg_select_top_feature_cn(void* desc_a_list,
                                    void* desc_b_list,
                                    int32_t data_type, /*0-> 8 bit signed, 1 -> 16 bit signed*/
                                    uint16_t* desc_a_list_offset,
                                    int32_t desc_size, /*descriptor size in terms of int16_t*/
                                    int32_t num_desc_a,
                                    int32_t num_desc_b,
                                    int32_t num_top,
                                    uint16_t *top_a_indx,
                                    uint16_t *top_b_indx,
                                    uint32_t *scartch1,
                                    uint16_t *scratch2,
                                    uint16_t *scratch3,
                                    uint32_t *scratch4);
/**
 * @func tiadalg_feature_matching_u08_c66
 *
 * @brief It does unsigned 8 bit descriptor cost computation with other list of discriptors. 
 *
 * @param desc_a          [in]  : A given descriptor
 * @param desc_b_list     [in]  : List of other discriptors in linear memory
 * @param desc_size       [in]  : Number of uint8 elements in one descriptor
 * @param num_desc_b      [in]  : Total number of descriptors in list \ref desc_b_list
 * @param desc_score      [in]  : Matching score for given descriptor with all descriptor in \ref desc_b_list
 *
 * @note
 * - Not implemented in this version
 */

int32_t tiadalg_feature_matching_u08_c66(uint8_t* desc_a,
                                          uint8_t* desc_b_list,
                                          int32_t desc_size,
                                          int32_t num_desc_b,
                                          uint32_t *desc_score);

/**
 * @func tiadalg_feature_matching_u08_cn
 *
 * @brief It does unsigned 8 bit descriptor cost computation with other list of descriptors. 
 *
 * @param desc_a          [in]  : A given descriptor
 * @param desc_b_list     [in]  : List of other discriptors in linear memory
 * @param desc_size       [in]  : Number of uint8 elements in one descriptor
 * @param num_desc_b      [in]  : Total number of descriptors in list \ref desc_b_list
 * @param desc_score      [in]  : Matching score for given descriptor with all descriptor in \ref desc_b_list
 *
 * @note
 * - Not implemented in this version
 */

int32_t tiadalg_feature_matching_u08_cn(uint8_t* desc_a,
                                          uint8_t* desc_b_list,
                                          int32_t desc_size, /*descriptor size in terms of int16_t*/
                                          int32_t num_desc_b,
                                          uint32_t *desc_score);                                          

/**
 * @func tiadalg_feature_matching_u16_c66
 *
 * @brief It does unsigned 16 bit descriptor cost computation with other list of descriptors. 
 *
 * @param desc_a          [in]  : A given descriptor
 * @param desc_b_list     [in]  : List of other discriptors in linear memory
 * @param desc_size       [in]  : Number of uint16 elements in one descriptor
 * @param num_desc_b      [in]  : Total number of descriptors in list \ref desc_b_list
 * @param desc_score      [in]  : Matching score for given descriptor with all descriptor in \ref desc_b_list
 *
 * @note
 * - Not implemented in this version
 */

int32_t tiadalg_feature_matching_u16_c66(uint16_t* desc_a,
                                          uint16_t* desc_b_list,
                                          int32_t desc_size,
                                          int32_t num_desc_b,
                                          uint32_t *desc_score);
/**
 * @func tiadalg_feature_matching_U16_cn
 *
 * @brief It does unsigned 16 bit descriptor cost computation with other list of descriptors. 
 *
 * @param desc_a          [in]  : A given descriptor
 * @param desc_b_list     [in]  : List of other discriptors in linear memory
 * @param desc_size       [in]  : Number of uint16 elements in one descriptor
 * @param num_desc_b      [in]  : Total number of descriptors in list \ref desc_b_list
 * @param desc_score      [in]  : Matching score for given descriptor with all descriptor in \ref desc_b_list
 *
 * @note
 * - Not implemented in this version
 */

int32_t tiadalg_feature_matching_u16_cn(uint16_t* desc_a,
                                          uint16_t* desc_b_list,
                                          int32_t desc_size, /*descriptor size in terms of int16_t*/
                                          int32_t num_desc_b,
                                          uint32_t *desc_score);                                          
                                            
/**
 * @func tiadalg_select_top_feature_c66
 *
 * @brief It does 2 way matching of unsigned 16 or 8 bit descriptors to find top matches. API is tested for KAZE descriptor, however 
 * it is not limited to KAZE. Matching happens between two sets/lists of descriptors. Length of each descriptor is configurable.
 *
 * @param desc_a_list          [in]  : List 'A' of discriptors in linear memory
 * @param desc_b_list          [in]  : List 'B' of discriptors in linear memory
 * @param data_type            [in]  : data type of descriptor. 0 vlaue indicates unsigned 8 bit, otherwise unsigned 16 bit
 * @param desc_a_list_offset   [in]  : Indices of descriptors in list 'A' to be considered for matching
 * @param desc_size            [in]  : Number of uint16/uint8 elements in one descriptor as per data type
 * @param num_desc_a           [in]  : Total number of descriptors in list 'A'
 * @param num_desc_b           [in]  : Total number of descriptors in list 'B'
 * @param num_top              [in]  : Total top matches to be found
 * @param top_a_indx           [out] : List of indices of top matched descriptors in list 'A'
 * @param top_b_indx           [out] : List of indices of top matched descriptors in list 'B'
 * @param scartch1             [in]  : Scratch memory needed by API of size 4* \ref num_desc_b
 * @param scratch2             [in]  : Scratch memory needed by API of size 2* \ref num_desc_a
 * @param scratch3             [in]  : Scratch memory needed by API of size 2* \ref num_desc_b
 * @param scratch4             [in]  : Scratch memory needed by API of size 4* \ref num_desc_b
 *
 * @note
 * - API returns total number of matches. It should be less than or equal to \ref num_top
 * - Valid entries in \ref top_a_indx \ref top_b_indx is equal to total number of matches
 * - i <SUP>th</SUP> match is the \ref top_a_indx [i] <SUP>th</SUP> descriptor from list 'A' and \ref top_b_indx [i] <SUP>th</SUP> descriptor from list 'B'
 */

int32_t tiadalg_select_top_feature_c66(void *restrict desc_a_list,
                                          void    *restrict desc_b_list,
                                          int32_t data_type, /*0-> 8 bit signed, 1 -> 16 bit signed*/
                                          uint16_t *restrict desc_a_list_offset,
                                          int32_t desc_size, /*descriptor size in terms of int16_t*/
                                          int32_t num_desc_a,
                                          int32_t num_desc_b,
                                          int32_t num_top,
                                          uint16_t *restrict top_a_indx,
                                          uint16_t *restrict top_b_indx,
                                          uint32_t *restrict scartch1,
                                          uint16_t *restrict scratch2,
                                          uint16_t *restrict scratch3,
                                          uint32_t *restrict scratch4);

#ifdef __cplusplus
}
#endif

#endif
/*@}*/
/* TIADALG_SELECT_TOP_FEAT_H */
