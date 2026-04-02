/*
 *  Copyright (C) 2021 Texas Instruments Incorporated - http://www.ti.com/
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _TI_OMAX_DEC_PRIV_H_
#define _TI_OMAX_DEC_PRIV_H_

#include "omax_wrapper_priv.h"

#ifndef DIM
#  define DIM(a) (sizeof((a)) / sizeof((a)[0]))
#endif

#define OMAX_BS_PART_READ( buffptr, retval, bytesleft ) do {             \
    int32_t remainShift = (4 - bytesleft) * 8; \
    while (bytesleft > 0) { \
        retval <<= 8; \
        retval |= *buffptr; \
        buffptr++; \
        bytesleft--; \
    } \
    retval <<= remainShift; \
} while( 0 )

typedef struct mm_buffer mm_buffer_t;

typedef struct {
    int32_t width;
    int32_t height;
    int32_t par_x;
    int32_t par_y;
    uint32_t num_units_in_tick;
    uint32_t time_scale;
    uint32_t fixed_frame_rate_flag;
} decoder_info_t;

/* H.264 AVC-specific return data. (fourcc "AVC1") */
typedef struct avc_param_set
{
    uint8_t*   string;
    uint8_t*   nalu;
    int16_t    len;
    int16_t    nalu_length;
    int16_t    mmbuffer_offset;
} avc1_param_set_t;

typedef struct avc_crop_info
{
    uint32_t               crop_left;
    uint32_t               crop_right;
    uint32_t               crop_top;
    uint32_t               crop_bottom;
} avc1_crop_info_t;

typedef struct avc_config
{
    uint8_t                profile_indication;
    uint8_t                profile_compatibility;
    uint8_t                level_indication;
    uint8_t                bit_depth_luma;
    uint8_t                bit_depth_chroma;
    int8_t                 size_length;
    int8_t                 sps_count;
    int8_t                 pps_count;
    decoder_info_t*        sps_info;
    avc1_param_set_t*      sps;
    avc1_param_set_t*      pps;
    avc1_crop_info_t       cropping;
    int32_t                num_ref_frames;
    uint8_t                frame_mbs_only_flag;
    uint8_t                video_full_range_flag;
    int32_t                chroma_format_idc;
} avc1_decoder_specific_t;

typedef struct {
    const uint8_t       *buff;
    uint32_t            buff_size;
    int                 bit_offset;
} psbitstream_t;

/**
 * \brief 
 * 
 * Loads the config settings of the input file to a pointer and sets the size.
 * Needs to be run before setting config settings of the OMAX Component.
 * 
 * \param [in]      encH         OmxilVideoEncDec_t parameters
 */
int32_t loadConfig(OmxilVideoEncDec_t *encH);

/**
 * \brief 
 * 
 * Reads the next frame and copies the data to the input buffer of the
 * OMAX Component.
 * 
 * \param [in]      encH         OmxilVideoEncDec_t parameters
 * \param [in]      bufHdr       OMX_BUFFERHEADERTYPE parameters
 */
int32_t readFrame(OmxilVideoEncDec_t *encH, OMX_BUFFERHEADERTYPE *bufHdr);

/* @} */

#endif /* _TI_OMAX_DEC_PRIV_H_ */

