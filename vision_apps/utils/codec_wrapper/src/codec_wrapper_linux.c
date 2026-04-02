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


#include <utils/codec_wrapper/include/codec_wrapper.h>
#include <utils/gst_wrapper/src/gst_wrapper_priv.h>


#define GST_TIMEOUT  400*GST_MSECOND


int32_t appCodecInit(app_codec_wrapper_params_t* params)
{
    return appGstInit(params);
}

int32_t appCodecSrcInit(void* data_ptr[CODEC_MAX_BUFFER_DEPTH][CODEC_MAX_NUM_CHANNELS][CODEC_MAX_NUM_PLANES])
{
    return appGstSrcInit(data_ptr);
}

int32_t appCodecSinkInit(void* data_ptr[CODEC_MAX_BUFFER_DEPTH][CODEC_MAX_NUM_CHANNELS][CODEC_MAX_NUM_PLANES])
{
    return appGstSinkInit(data_ptr);
}

int32_t appCodecStart()
{
    return appGstStart();
}

int32_t appCodecEnqAppSrc(uint8_t idx)
{
    return appGstEnqAppSrc(idx);
}

int32_t appCodecDeqAppSrc(uint8_t idx)
{
    return appGstDeqAppSrc(idx);
}

int32_t appCodecEnqEosAppSrc()
{
    return appGstEnqEosAppSrc();
}

int32_t appCodecDeqAppSink(uint8_t idx)
{
    return appGstDeqAppSink(idx);
}

int32_t appCodecEnqAppSink(uint8_t idx)
{
    return appGstEnqAppSink(idx);
}

int32_t appCodecStop()
{
    return appGstStop();
}

void appCodecDeInit()
{
    appGstDeInit();
}

void appCodecPrintStats()
{
    appGstPrintStats();
}
