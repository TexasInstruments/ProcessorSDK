/*
 * Copyright (c) 2016-2022 The Khronos Group Inc.
 * Copyright 2022, QNX Software Systems.
 * Copyright (C) 2022 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject
 * to the following conditions:
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef _TI_OMAX_WRAPPER_PRIV_H_
#define _TI_OMAX_WRAPPER_PRIV_H_

/* Standard headers. */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/procmgr.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/neutrino.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <OMX_Core.h>
#include <OMX_Component.h>
#include <OMX_Extension_video_TI.h>
#if defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2) || defined(SOC_AM62A)
#include <OMX_Extension_index_TI.h>
#endif /* SOC_J721S2 or SOC_J784S4 or SOC_J742S2 or SOC_AM62A */

#include <utils/openmax_wrapper/include/omax_wrapper.h>

// #define WRAPPER_DEBUG_MODE

#if defined (WRAPPER_DEBUG_MODE)
#define WRAPPER_PRINTF(f_, ...) printf((f_), ##__VA_ARGS__)
#else
#define WRAPPER_PRINTF(f_, ...)
#endif

#define WRAPPER_ERROR(f_, ...) printf((f_), ##__VA_ARGS__)

#define OMAX_SPEC_VERSION 0x00000001     /* OMX Version */
#define SET_OMAX_VERSION_SIZE( param, size ) {             \
    param.nVersion.nVersion = OMAX_SPEC_VERSION;           \
    param.nSize = size;                                   \
}

#if defined (WRAPPER_DEBUG_MODE)
    #define TIMEOUT_WAIT (0x1FFFFFFFFFFF) /* for debug */
#else
    #define TIMEOUT_WAIT (5000 * 1000LL * 1000LL)  /* 5000 ms */
#endif

#define OMAX_MAX_FILE_PATH               (256u)
#define OMAX_CONFIG_DATA_BUFFER_SIZE     8096
#define OMAX_SECOND_BYTE                 0x8000000
#define OMAX_H264_CUSTOM_ASPECT_RATIO    255
#define OMAX_DEFAULT_BITRATE             10000000
#define OMAX_DEFAULT_IDR_PERIOD          30
#define OMAX_DEFAULT_FRAME_RATE          30

#if defined(SOC_J721E)
#define OMAX_DEFAULT_RATE_CONTROL_MODE   OMX_Video_ControlRateConstant  //Other option: OMX_Video_ControlRateVariable
#define OMAX_DEFAULT_BUFFER_SIZE         (10*1024*1024)
#else
#define OMAX_DEFAULT_RATE_CONTROL_MODE   OMX_Video_ControlRateVariable  //Other option: OMX_Video_ControlRateConstant
#define OMAX_DEFAULT_BUFFER_SIZE         (2*1024*1024)
#define OMAX_DEC_CUSTOM_IN_BUFFERS       2
#endif /* SOC_J721E */

typedef struct {
    void *addr;
    uint32_t size;
    off64_t offset; /* offset or physical address of the buffer. */
} OmxilBuffer_t;


/*! \brief A Boolean value.
 * This allows 0 to be FALSE, as it is in C, and any non-zero to be TRUE.
 * \code
 * omxil_bool ret = omxil_true_e;
 * if (ret) printf("true!\n");
 * ret = omxil_false_e;
 * if (!ret) printf("false!\n");
 * \endcode
 * This would print both strings.
 * \see omxil_bool
 * \ingroup group_basic_features
 */
typedef enum _omxil_bool_e {
    /*! \brief The "false" value. */
    omxil_false_e = 0,
    /*! \brief The "true" value. */
    omxil_true_e,
} omxil_bool_e;

/*! \brief A formal boolean type with known fixed size.
 * \see omxil_bool_e
 * \ingroup group_basic_features
 */
typedef int32_t omxil_bool;

typedef struct OmxilVideoEncDec_ {
    /* File paths*/
    char out_path[OMAX_MAX_FILE_PATH];
    char in_path[OMAX_MAX_FILE_PATH];
    char conf_path[OMAX_MAX_FILE_PATH];
    int32_t in_fd;
    int32_t out_fd;

    uint8_t* large_input_buf;
    uint8_t* currPtr;
    OMX_U32 large_input_buf_data_size;

    uint32_t config_size;
    uint8_t configPtr[OMAX_CONFIG_DATA_BUFFER_SIZE];

    OmxilBuffer_t* input_bufs[CODEC_MAX_BUFFER_DEPTH];
    OMX_U32        input_buf_num;

    OmxilBuffer_t* output_bufs[CODEC_MAX_BUFFER_DEPTH];
    OMX_U32        output_buf_num;

    int32_t input_format;

    omxil_bool user_request_exit;

    int32_t frame_rate;
    int32_t src_width;
    int32_t src_height;
    int32_t aligned_width;
    int32_t aligned_height;
    int32_t frame_size;
    int32_t src_stride;
    int32_t channelIdx;

    /* component */
    OMX_HANDLETYPE compHandle;
    OMX_VIDEO_CODINGTYPE compressFmt;
    OMX_U32 inPortIndex;
    OMX_U32 outPortIndex;
    uint16_t numOfPorts; /* Data ports */
    omxil_bool cmdComplete;
    omxil_bool inPortFlushed;
    omxil_bool outPortFlushed;
    omxil_bool eos_received;
    omxil_bool eos_sent;
    omxil_bool eof_received;
    OMX_ERRORTYPE compError;

    /* output port */
    OMX_U32 outputPortBufSize;
    OMX_U32 nOutputBufs;
    int32_t qOutputBufHdr[CODEC_MAX_BUFFER_DEPTH];
    OMX_BUFFERHEADERTYPE* outputBufHdrList[CODEC_MAX_BUFFER_DEPTH];
    int32_t qOutputBufHdrLastIdx;
    int32_t qOutputBufHdrFirstIdx;
    omxil_bool outBufFull[CODEC_MAX_BUFFER_DEPTH];
    omxil_bool sentOutBuf[CODEC_MAX_BUFFER_DEPTH];

    int32_t qOutputBufReturn[CODEC_MAX_BUFFER_DEPTH];
    int32_t qOutputBufReturnLastIdx;
    int32_t qOutputBufReturnFirstIdx;

    /* Input port */
    OMX_U32 nInputBufs;
    OMX_U32 inputPortBufSize;
    int32_t qInputBufHdr[CODEC_MAX_BUFFER_DEPTH];
    OMX_BUFFERHEADERTYPE* inputBufHdrList[CODEC_MAX_BUFFER_DEPTH];
    int32_t qInputBufHdrLastIdx;
    int32_t qInputBufHdrFirstIdx;
    omxil_bool inBufEmpty[CODEC_MAX_BUFFER_DEPTH];

    uint32_t srcWidth; /* source width, width from configure data, output buffer is allocated with this size */
    uint32_t srcHeight; /* source height, height from configure data */

    /* encoder configure parameters */
    int32_t bitrate;
    int32_t idr_period;
    int32_t rcmode;

    int32_t input_frame_cnt;

    uint64_t enc_start_time_ms;
    uint64_t enc_eos_time_ms;
    uint64_t enc_time_ms;
    uint64_t enc_frame_rate;

    /* Threads */
    pthread_t push_thread;
    pthread_mutex_t mutex;
    pthread_cond_t cond;

    int32_t push_count;
    int32_t pull_count;

    uint32_t frame_duration;
    uint64_t frame_ts;
    uint64_t last_frame_ts;
    omxil_bool frame_rate_control;

    uint32_t mLumaDepth;
    uint32_t mChromaDepth;
    int32_t mChromaFmt;

    int32_t core_idx;

    omxil_bool doneReadFrames;
} OmxilVideoEncDec_t;

typedef struct
{
    app_codec_wrapper_params_t params;

    int32_t push_count;
    int32_t pull_count;

    OmxilVideoEncDec_t compHandleArray[CODEC_MAX_NUM_CHANNELS];

    int32_t input_format;
    omxil_bool isH264;
    omxil_bool isH265;

    void* (*pdataPtr)[CODEC_MAX_NUM_CHANNELS][CODEC_MAX_NUM_PLANES];
} app_omax_wrapper_obj_t;

extern app_omax_wrapper_obj_t g_app_omax_wrapper_obj;


#endif /* _TI_OMAX_WRAPPER_PRIV_H_ */

