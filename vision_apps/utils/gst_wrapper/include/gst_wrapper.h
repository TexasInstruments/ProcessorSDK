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

#ifndef _TI_GST_WRAPPER_H_
#define _TI_GST_WRAPPER_H_


#include <utils/codec_wrapper/include/codec_wrapper.h>


/**
 * \defgroup group_vision_apps_utils_gst_wrapper GStreamer Wrapper APIs
 *
 * \brief This section contains APIs to use GStreamer CODEC Pipelines within demos
 *
 * \ingroup group_vision_apps_utils
 *
 * @{
 */

/**
 * \brief 
 * 
 * Launches the GstPipeline described by user defined parameters.
 * Initializes the gst_wrapper and calls gst_parse_launch() on the command string.
 * 
 * \param [in]      prm         Init parameters
 */
int32_t appGstInit(app_codec_wrapper_params_t *prm);

/**
 * \brief 
 * 
 * Initializes the AppSrc elements to be able to push buffers to the GstPipeline.
 * Wraps the given allocated memory as GstBuffers to push. 
 * The input parameters (in_*) must be properly initialised.
 * 
 * \param [in]      data_ptr        memory that the GstBuffers will map to
 */
int32_t appGstSrcInit(void* data_ptr[CODEC_MAX_BUFFER_DEPTH][CODEC_MAX_NUM_CHANNELS][CODEC_MAX_NUM_PLANES]);

/**
 * \brief 
 * 
 * Initializes the AppSink elements to be able to pull buffers from the GstPipeline.
 * Registers the data_pointers that will map to the bufferpool of pulled GstBuffers.
 * The output parameters (out_*) must be properly initialised.
 * 
 * \param [in]      data_ptr        pointers to hold the pulled buffers
 */
int32_t appGstSinkInit(void* (*data_ptr)[CODEC_MAX_NUM_CHANNELS][CODEC_MAX_NUM_PLANES]);

/**
 * \brief 
 * 
 * Starts the GstPipeline that was launched previously.
 */
int32_t appGstStart();

/**
 * \brief 
 * 
 * Push a buffer from the bufferpool to the AppSrc element.
 * 
 * \param [in]     idx             the buffer id, from the bufferpool, to be pushed to the GstPipeline
 */
int32_t appGstEnqAppSrc(uint8_t idx);

/**
 * \brief 
 * 
 * Wait for a previously pushed buffer to be consumed by the GstPipeline.
 * 
 * \param [in]     idx             the buffer id, from the bufferpool, to wait on
 */
int32_t appGstDeqAppSrc(uint8_t idx);

/**
 * \brief 
 * 
 * Push EOS (End Of Stream) to the AppSrc element.
 * Signals the elements to stop processing buffers after this.
 */
int32_t appGstEnqEosAppSrc();

/**
 * \brief 
 * 
 * Pull a buffer from the AppSink element into the bufferpool.
 * 
 * \param [in]     idx             the slot into which the pulled data is stored  
 *                                 in the pulled_data_ptr array, while maintaining 
 *                                 a reference to the pulled GstBuffer
 */
int32_t appGstDeqAppSink(uint8_t idx);

/**
 * \brief 
 * 
 * Release a previously pulled GstBuffer back to GStreamer and cleanup.
 * 
 * \param [in]     idx             the slot from which the buffer is released 
 *                                 back to Gstreamer. The data pointer for this
 *                                 slot is no longer valid
 */
int32_t appGstEnqAppSink(uint8_t idx);

/**
 * \brief 
 * 
 * Stops the GstPipeline that was in playing state.
 * If there is no AppSink element, waits to recieve EOS before cleaning up.
 */
int32_t appGstStop(); 

/**
 * \brief 
 * 
 * Unmaps all buffers that were mapped and destroys all GStreamer objects.
 */
void    appGstDeInit();


/**
 * \brief 
 * 
 * Prints the current count of the numbers of buffers pushed/pulled to/from the GstPipeline.
 * Not multi-thread safe.
 */
void appGstPrintStats();

/* @} */

#endif /* _TI_GST_WRAPPER_H_ */

