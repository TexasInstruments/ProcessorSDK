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

#ifndef _TI_CODEC_WRAPPER_H_
#define _TI_CODEC_WRAPPER_H_


/* Standard headers. */
#include <stdint.h>


/**
 * \defgroup group_vision_apps_utils_codec_wrapper Codec Wrapper APIs
 *
 * \brief This section contains APIs to use CODEC Pipelines within demos
 *
 * \ingroup group_vision_apps_utils
 *
 * @{
 */


#define CODEC_MAX_LEN_CMD_STR   4096u
#define CODEC_MAX_LEN_ELEM_NAME   32u
#define CODEC_MAX_NUM_PLANES       4u
#define CODEC_MAX_BUFFER_DEPTH    16u
#if defined(SOC_J784S4) || defined(SOC_J742S2)
#define CODEC_MAX_NUM_CHANNELS    12u
#else
#define CODEC_MAX_NUM_CHANNELS     8u
#endif /* SOC_J784S4 or SOC_J742S2 */


/**
 * \brief Init parameters
 */
typedef struct
{
    char        m_AppSrcNameArr[CODEC_MAX_NUM_CHANNELS][CODEC_MAX_LEN_ELEM_NAME];/* Name of the AppSrc element */
    int32_t     in_width;/* Width of the buffers input to the CodecPipeline */
    int32_t     in_height;/* Height of the buffers input to the CodecPipeline */
    char        in_format[8];/* Format of the buffers input to the CodecPipeline (eg:"NV12") */
    uint8_t     in_num_planes;/* Number of planes of the buffers input to the CodecPipeline */
    uint8_t     in_num_channels;/* Number of channels input to the CodecPipeline */
    uint8_t     in_buffer_depth;/* The input buffer pool size for the CodecPipeline */

    char        m_AppSinkNameArr[CODEC_MAX_NUM_CHANNELS][CODEC_MAX_LEN_ELEM_NAME];/* Name of the AppSink element */
    int32_t     out_width;/* Width of the buffers output from the CodecPipeline */
    int32_t     out_height;/* Height of the buffers output from the CodecPipeline */
    char        out_format[8];/* Format of the buffers output from the CodecPipeline (eg:"NV12") */
    uint8_t     out_num_planes;/* Number of planes of the buffers output from the CodecPipeline */
    uint8_t     out_num_channels;/* Number of channels output from the CodecPipeline */
    uint8_t     out_buffer_depth;/* The output buffer pool size for the CodecPipeline */

    char        m_cmdString[CODEC_MAX_LEN_CMD_STR];/* The command string that describes the CodecPipeline, in a format accepted by gst_parse_launch() */

    int32_t appEncode;
    int32_t appDecode;
} app_codec_wrapper_params_t;

/**
 * \brief
 *
 * Launches the CODECPipeline described by user defined parameters.
 * Initializes the codec_wrapper and calls gst_parse_launch() on the command string.
 *
 * \param [in]      prm         Init parameters
 */
int32_t appCodecInit(app_codec_wrapper_params_t *prm);

/**
 * \brief
 *
 * Initializes the AppSrc elements to be able to push buffers to the CODECPipeline.
 * Wraps the given allocated memory as CODECBuffers to push.
 * The input parameters (in_*) must be properly initialised.
 *
 * \param [in]      data_ptr        memory that the CODECBuffers will map to
 */
int32_t appCodecSrcInit(void* data_ptr[CODEC_MAX_BUFFER_DEPTH][CODEC_MAX_NUM_CHANNELS][CODEC_MAX_NUM_PLANES]);

/**
 * \brief
 *
 * Initializes the AppSink elements to be able to pull buffers from the CODECPipeline.
 * Registers the data_pointers that will map to the bufferpool of pulled CODECBuffers.
 * The output parameters (out_*) must be properly initialised.
 *
 * \param [in]      data_ptr        memory that the CODECBuffers will map to
 */
int32_t appCodecSinkInit(void* data_ptr[CODEC_MAX_BUFFER_DEPTH][CODEC_MAX_NUM_CHANNELS][CODEC_MAX_NUM_PLANES]);

/**
 * \brief
 *
 * Starts the CODECPipeline that was launched previously.
 */
int32_t appCodecStart();

/**
 * \brief
 *
 * Push a buffer from the bufferpool to the AppSrc element.
 *
 * \param [in]     idx             the buffer id, from the bufferpool, to be pushed to the CODECPipeline
 */
int32_t appCodecEnqAppSrc(uint8_t idx);

/**
 * \brief
 *
 * Wait for a previously pushed buffer to be consumed by the CODECPipeline.
 *
 * \param [in]     idx             the buffer id, from the bufferpool, to wait on
 */
int32_t appCodecDeqAppSrc(uint8_t idx);

/**
 * \brief
 *
 * Push EOS (End Of Stream) to the AppSrc element.
 * Signals the elements to stop processing buffers after this.
 */
int32_t appCodecEnqEosAppSrc();

/**
 * \brief
 *
 * Pull a buffer from the AppSink element into the bufferpool.
 *
 * \param [in]     idx             the slot into which the pulled data is stored
 *                                 in the pulled_data_ptr array, while maintaining
 *                                 a reference to the pulled CODECBuffer
 */
int32_t appCodecDeqAppSink(uint8_t idx);

/**
 * \brief
 *
 * Release a previously pulled CODECBuffer back to GStreamer and cleanup.
 *
 * \param [in]     idx             the slot from which the buffer is released
 *                                 back to CODECreamer. The data pointer for this
 *                                 slot is no longer valid
 */
int32_t appCodecEnqAppSink(uint8_t idx);

/**
 * \brief
 *
 * Stops the CODECPipeline that was in playing state.
 * If there is no AppSink element, waits to recieve EOS before cleaning up.
 */
int32_t appCodecStop();

/**
 * \brief
 *
 * Unmaps all buffers that were mapped and destroys all GStreamer or OpenMAX objects.
 */
void    appCodecDeInit();


/**
 * \brief
 *
 * Prints the current count of the numbers of buffers pushed/pulled to/from the CODECPipeline.
 * Not multi-thread safe.
 */
void appCodecPrintStats();

/* @} */

#endif /* _TI_CODEC_WRAPPER_H_ */

