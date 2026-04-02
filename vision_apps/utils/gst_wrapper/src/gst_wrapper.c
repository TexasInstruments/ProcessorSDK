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


#include "gst_wrapper_priv.h"

#define GST_TIMEOUT  400*GST_MSECOND

app_gst_wrapper_obj_t g_app_gst_wrapper_obj;


static GstElement *findElementByName(GstElement* pipeline, const char* name)
{
    GstElement *elem;

    elem = gst_bin_get_by_name(GST_BIN(pipeline), name);

    if (elem == NULL)
    {
        printf("gst_wrapper: Could not find element <%s> in the pipeline.\n", name);
    }

    return elem;
}

static int32_t exportgsttiovxbuffer(GstBuffer* buf, void* data_ptr[CODEC_MAX_NUM_PLANES])
{
    vx_status status = VX_SUCCESS;
    void* p_status = NULL;
    GstTIOVXImageMeta *tiovxmeta = NULL;
    vx_reference img1;
    vx_enum img1_type = VX_TYPE_INVALID;
    uint32_t img1_num_planes = 0;
    uint32_t sizes[CODEC_MAX_NUM_PLANES] = { 0 };

    tiovxmeta = (GstTIOVXImageMeta *) gst_buffer_iterate_meta(buf, &p_status);
    if (!tiovxmeta)
    {
        printf("gst_wrapper: ERROR: TIOVX meta not found in pulled buffer!\n");
        return -1;
    }
    
    img1 = vxGetObjectArrayItem (tiovxmeta->array, 0);
    status = vxGetStatus((vx_reference) img1 );
    if ( status != VX_SUCCESS)
    {
        printf("gst_wrapper: ERROR: Could not get vx_reference from TIOVX meta!\n");
        return status;
    }
 
    status = vxQueryReference ((vx_reference) img1, (vx_enum) VX_REFERENCE_TYPE, &img1_type, sizeof (vx_enum));
    if (VX_SUCCESS != status) {
        printf("gst_wrapper: ERROR: Failed to verify VX_REFERENCE_TYPE!\n");
        vxReleaseReference (&img1);
        return status;
    }
    else if(VX_TYPE_IMAGE != img1_type) {
        printf("gst_wrapper: ERROR: vx_reference is not a vx_image!\n");
        vxReleaseReference (&img1);
        return VX_ERROR_INVALID_TYPE;
    }

    status = tivxReferenceExportHandle(
                (vx_reference) img1,
                data_ptr,
                sizes,
                CODEC_MAX_NUM_PLANES,
                &img1_num_planes);
    if (VX_SUCCESS != status) {
        printf("gst_wrapper: ERROR: Could not export handles from vx_image!\n");
        vxReleaseReference (&img1);
        return status;
    }
    
    vxReleaseReference (&img1);
    return status;
}

int32_t appGstInit(app_codec_wrapper_params_t* params)
{
    int32_t status = 0;
    app_gst_wrapper_obj_t* p_gst_pipe_obj = &g_app_gst_wrapper_obj;

    if ( strcmp(params->in_format, "NV12") || strcmp(params->out_format, "NV12") )
    {
        printf("gst_wrapper: Error: Only NV12 format supported!\n");
        status=-1;
    }

    if ( status==0 && (sizeof(params->m_cmdString)!=0) )
    {
        printf("gst_wrapper: GstCmdString:\n%s\n",params->m_cmdString);
        p_gst_pipe_obj->params = *params;
    }
    else status=-1;

    p_gst_pipe_obj->m_pipeline = NULL;
    if (status==0)
    {
        /* GStreamer INIT  */
        gst_init(NULL, NULL);

        p_gst_pipe_obj->m_pipeline = gst_parse_launch(p_gst_pipe_obj->params.m_cmdString, NULL);
        if (p_gst_pipe_obj->m_pipeline == NULL)
        {
            printf("gst_wrapper: gst_parse_launch() failed:\n%s\n",p_gst_pipe_obj->params.m_cmdString);
            status = -1;
        }
    }

    p_gst_pipe_obj->isAppSrc = 0;
    p_gst_pipe_obj->isAppSink = 0;
    p_gst_pipe_obj->push_count = -1;
    p_gst_pipe_obj->pull_count = -1;

    return status;
}

int32_t appGstSrcInit(void* data_ptr[CODEC_MAX_BUFFER_DEPTH][CODEC_MAX_NUM_CHANNELS][CODEC_MAX_NUM_PLANES])
{
    int32_t status = 0;
    GstCaps* caps = NULL;
    app_gst_wrapper_obj_t* p_gst_pipe_obj = &g_app_gst_wrapper_obj;
    uint32_t plane_size = p_gst_pipe_obj->params.in_width * p_gst_pipe_obj->params.in_height;

    /* Setup AppSrc Elements */
    for (uint8_t ch = 0; ch < p_gst_pipe_obj->params.in_num_channels; ch++)
    {
        p_gst_pipe_obj->m_srcElemArr[ch]  = findElementByName(p_gst_pipe_obj->m_pipeline, 
                                                    p_gst_pipe_obj->params.m_AppSrcNameArr[ch]);
        if (p_gst_pipe_obj->m_srcElemArr[ch] == NULL)
        {
            printf("gst_wrapper: findElementByName() FAILED! %s not found\n", p_gst_pipe_obj->params.m_AppSrcNameArr[ch]);
            status = -1;
        }
        else
        {
            p_gst_pipe_obj->isAppSrc = GST_IS_APP_SRC(p_gst_pipe_obj->m_srcElemArr[ch]);
            if (p_gst_pipe_obj->isAppSrc)
            {
                caps = gst_caps_new_simple("video/x-raw",
                                        "width", G_TYPE_INT, p_gst_pipe_obj->params.in_width,
                                        "height", G_TYPE_INT, p_gst_pipe_obj->params.in_height,
                                        "format", G_TYPE_STRING, p_gst_pipe_obj->params.in_format,
                                        NULL);
                if (caps == NULL)
                {
                    printf("gst_wrapper: gst_caps_new_simple() FAILED!\n");
                    status = -1;
                }
                gst_app_src_set_caps (GST_APP_SRC(p_gst_pipe_obj->m_srcElemArr[ch]), caps);
                gst_caps_unref (caps);
            }
            else 
            {
                printf("gst_wrapper: %s not an AppSrc element\n", p_gst_pipe_obj->params.m_AppSrcNameArr[ch]);
                status = -1;
            }
        }
    }

    /* Setup GstBuffers to push to AppSrc Elements using given data_ptrs */
    if (status==0)
    {
        for (uint8_t idx = 0; idx < p_gst_pipe_obj->params.in_buffer_depth && status==0; idx++)
        {
            for (uint8_t ch = 0; ch < p_gst_pipe_obj->params.in_num_channels && status==0; ch++)
            {
                p_gst_pipe_obj->buff[idx][ch] = gst_buffer_new();

                p_gst_pipe_obj->mem[idx][ch][0] = gst_memory_new_wrapped (0, data_ptr[idx][ch][0], plane_size, 0, plane_size, NULL, NULL);
                p_gst_pipe_obj->mem[idx][ch][1] = gst_memory_new_wrapped (0, data_ptr[idx][ch][1], plane_size/2, 0, plane_size/2, NULL, NULL);

                gst_buffer_append_memory (p_gst_pipe_obj->buff[idx][ch], p_gst_pipe_obj->mem[idx][ch][0]);
                gst_buffer_append_memory (p_gst_pipe_obj->buff[idx][ch], p_gst_pipe_obj->mem[idx][ch][1]);

                p_gst_pipe_obj->mem[idx][ch][0] = gst_buffer_get_memory (p_gst_pipe_obj->buff[idx][ch], 0);
                p_gst_pipe_obj->mem[idx][ch][1] = gst_buffer_get_memory (p_gst_pipe_obj->buff[idx][ch], 1);

                gst_memory_map(p_gst_pipe_obj->mem[idx][ch][0],&p_gst_pipe_obj->map_info[idx][ch][0], GST_MAP_WRITE);
                gst_memory_map(p_gst_pipe_obj->mem[idx][ch][1],&p_gst_pipe_obj->map_info[idx][ch][1], GST_MAP_WRITE);
            }
        }
    }

    if (status==0) p_gst_pipe_obj->push_count = 0;

    return status;
}

int32_t appGstSinkInit(void* (*data_ptr)[CODEC_MAX_NUM_CHANNELS][CODEC_MAX_NUM_PLANES])
{
    int32_t status = 0;
    app_gst_wrapper_obj_t* p_gst_pipe_obj = &g_app_gst_wrapper_obj;

    /* Setup AppSink Elements */
    for (uint8_t ch = 0; ch < p_gst_pipe_obj->params.out_num_channels; ch++)
    {
        p_gst_pipe_obj->m_sinkElemArr[ch] = findElementByName(p_gst_pipe_obj->m_pipeline, 
                                                p_gst_pipe_obj->params.m_AppSinkNameArr[ch]);
        if (p_gst_pipe_obj->m_sinkElemArr[ch] == NULL)
        {
            printf("gst_wrapper: findElementByName() FAILED! %s not found\n", p_gst_pipe_obj->params.m_AppSinkNameArr[ch]);
            status = -1;
        }
        else
        {
            p_gst_pipe_obj->isAppSink = GST_IS_APP_SINK(p_gst_pipe_obj->m_sinkElemArr[ch]);
            if (p_gst_pipe_obj->isAppSink==0)
            {
                printf("gst_wrapper: %s not an AppSink element\n", p_gst_pipe_obj->params.m_AppSinkNameArr[ch]);
                status = -1;
            }
        }
    }
    
    /* Setup internal pointers to point to given data_ptrs, where the pulled data will be made available */
    if (status == 0)
    {
        p_gst_pipe_obj->pulled_data_ptr = data_ptr;
    }

    if (status==0) p_gst_pipe_obj->pull_count = 0;

    return status;
}

int32_t appGstStart()
{
    app_gst_wrapper_obj_t* p_gst_pipe_obj = &g_app_gst_wrapper_obj;

    /* Set pipeline state to PLAYING */
    GstStateChangeReturn ret = gst_element_set_state (p_gst_pipe_obj->m_pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE)
    {
        printf("gst_wrapper: gst_element_set_state() FAILED! ... GST pipe not playing.\n");
        return -1;
    }
    return 0;
}

int32_t appGstEnqAppSrc(uint8_t idx)
{
    int32_t status = 0;
    app_gst_wrapper_obj_t* p_gst_pipe_obj = &g_app_gst_wrapper_obj;
    
    if(p_gst_pipe_obj->isAppSrc==0){
        printf("Gst Pipeline not initialised correctly: isAppSrc = 0 !\n");
        return -1;
    }
    for (uint8_t ch = 0; ch < p_gst_pipe_obj->params.in_num_channels; ch++)
    {
        gst_memory_unmap(p_gst_pipe_obj->mem[idx][ch][0],&p_gst_pipe_obj->map_info[idx][ch][0]);
        gst_memory_unmap(p_gst_pipe_obj->mem[idx][ch][1],&p_gst_pipe_obj->map_info[idx][ch][1]);

        if (status == 0)
        {
            GstFlowReturn ret;

            gst_buffer_ref(p_gst_pipe_obj->buff[idx][ch]);
            ret = gst_app_src_push_buffer(GST_APP_SRC(p_gst_pipe_obj->m_srcElemArr[ch]), p_gst_pipe_obj->buff[idx][ch]);
            if (ret != GST_FLOW_OK)
            {
                printf("gst_wrapper: Pushing buffer to AppSrc returned %d instead of GST_FLOW_OK:%d\n", ret, GST_FLOW_OK);
                status = -1;
            }
        }
    }

    if (status==0) p_gst_pipe_obj->push_count++;

    return status;
}

int32_t appGstDeqAppSrc(uint8_t idx)
{
    int32_t status = 0;
    app_gst_wrapper_obj_t* p_gst_pipe_obj = &g_app_gst_wrapper_obj;
    uint8_t refcount;

    if(p_gst_pipe_obj->isAppSrc==0){
        printf("Gst Pipeline not initialised correctly: isAppSrc = 0 !\n");
        return -1;
    }
    for (uint8_t ch = 0; ch < p_gst_pipe_obj->params.in_num_channels; ch++)
    {
        refcount = GST_MINI_OBJECT_REFCOUNT_VALUE(&p_gst_pipe_obj->buff[idx][ch]->mini_object);
        while (refcount > 1)
        {
            refcount = GST_MINI_OBJECT_REFCOUNT_VALUE(&p_gst_pipe_obj->buff[idx][ch]->mini_object);
        }
        refcount = GST_MINI_OBJECT_REFCOUNT_VALUE(&p_gst_pipe_obj->mem[idx][ch][1]->mini_object);
        while (refcount > 2)
        {
            refcount = GST_MINI_OBJECT_REFCOUNT_VALUE(&p_gst_pipe_obj->mem[idx][ch][1]->mini_object);
        }
        refcount = GST_MINI_OBJECT_REFCOUNT_VALUE(&p_gst_pipe_obj->mem[idx][ch][0]->mini_object);
        while (refcount > 2)
        {
            refcount = GST_MINI_OBJECT_REFCOUNT_VALUE(&p_gst_pipe_obj->mem[idx][ch][0]->mini_object);
        }

        if(!gst_memory_map(p_gst_pipe_obj->mem[idx][ch][0],&p_gst_pipe_obj->map_info[idx][ch][0], GST_MAP_WRITE)) 
        { 
            status = -1; 
            break;
        }
        if(!gst_memory_map(p_gst_pipe_obj->mem[idx][ch][1],&p_gst_pipe_obj->map_info[idx][ch][1], GST_MAP_WRITE)) 
        {
            status = -1;
            break;
        }
    }

    return status;
}

int32_t appGstEnqEosAppSrc()
{
    GstFlowReturn ret;
    int32_t status = 0;
    app_gst_wrapper_obj_t* p_gst_pipe_obj = &g_app_gst_wrapper_obj;

    if(p_gst_pipe_obj->isAppSrc==0){
        printf("Gst Pipeline not initialised correctly: isAppSrc = 0 !\n");
        return -1;
    }
    for (uint8_t ch = 0; ch < p_gst_pipe_obj->params.in_num_channels; ch++)
    {
        ret = gst_app_src_end_of_stream(GST_APP_SRC(p_gst_pipe_obj->m_srcElemArr[ch]   ));
        if (ret != GST_FLOW_OK)
        {
            printf("gst_wrapper: Pushing EOS to AppSrc returned %d instead of GST_FLOW_OK:%d\n", ret, GST_FLOW_OK);
            status = -1;
        }
    }

    return status;
}

int32_t appGstDeqAppSink(uint8_t idx)
{
    GstSample *out_sample = NULL;
    int32_t status = 0;
    app_gst_wrapper_obj_t* p_gst_pipe_obj = &g_app_gst_wrapper_obj;

    if(p_gst_pipe_obj->isAppSink==0){
        printf("Gst Pipeline not initialised correctly: isAppSink = 0 !\n");
        return -1;
    }
    for (uint8_t ch = 0; ch < p_gst_pipe_obj->params.out_num_channels; ch++)
    {
        /* Pull Sample from AppSink element */
        out_sample = gst_app_sink_try_pull_sample(GST_APP_SINK(p_gst_pipe_obj->m_sinkElemArr[ch]),GST_TIMEOUT);
        if(out_sample)
        {
            p_gst_pipe_obj->pulled_buff[idx][ch] = gst_sample_get_buffer(out_sample);

            status = exportgsttiovxbuffer(p_gst_pipe_obj->pulled_buff[idx][ch], (p_gst_pipe_obj->pulled_data_ptr)[idx][ch]);
            if (status != 0){
                printf("gst_wrapper: exportgsttiovxbuffer FAILED!\n");
                break;
            }

            gst_buffer_ref(p_gst_pipe_obj->pulled_buff[idx][ch]);
            gst_sample_unref(out_sample);
        }
        else if(gst_app_sink_is_eos(GST_APP_SINK(p_gst_pipe_obj->m_sinkElemArr[ch])))
        {
            status = 1;
            break;
        }
        else
        {
            printf("gst_wrapper: WARNING: gst_app_sink_pull_sample() FAILED!\n");
            status = -1;
            break;
        }
    }

    if (status==0) p_gst_pipe_obj->pull_count++;

    return status;
}

int32_t appGstEnqAppSink(uint8_t idx)
{
    app_gst_wrapper_obj_t* p_gst_pipe_obj = &g_app_gst_wrapper_obj;

    if(p_gst_pipe_obj->isAppSink==0){
        printf("Gst Pipeline not initialised correctly: isAppSink = 0 !\n");
        return -1;
    }
    for (uint8_t ch = 0; ch < p_gst_pipe_obj->params.out_num_channels; ch++)
    {
        if ( p_gst_pipe_obj->pulled_buff[idx][ch] != NULL )
        {
            for(int32_t p = 0; p < p_gst_pipe_obj->params.out_num_planes; p++)
            {
                (p_gst_pipe_obj->pulled_data_ptr)[idx][ch][p] = NULL;
            }
            gst_buffer_unref(p_gst_pipe_obj->pulled_buff[idx][ch]);
            p_gst_pipe_obj->pulled_buff[idx][ch] = NULL;
        }
    }
    return 0;
}

int32_t appGstStop()
{
    GstStateChangeReturn ret;
    app_gst_wrapper_obj_t* p_gst_pipe_obj = &g_app_gst_wrapper_obj;

    if(p_gst_pipe_obj->isAppSink==0)
    {
        GstBus     *bus;
        GstMessage *msg;
        bus = gst_element_get_bus (p_gst_pipe_obj->m_pipeline);
        msg =
            gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE,
            GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

        if (GST_MESSAGE_TYPE (msg) == GST_MESSAGE_EOS) {
            printf("gst_wrapper: Got EOS from pipeline!\n");
        }
        else if (GST_MESSAGE_TYPE (msg) == GST_MESSAGE_ERROR) {
            printf("gst_wrapper: An error occurred! Re-run with the GST_DEBUG=*:WARN environment "
                "variable set for more details.");
        }

        /* Free resources */
        gst_message_unref (msg);
        gst_object_unref (bus);
    }

    /* Set pipeline state to NULL */
    ret = gst_element_set_state (p_gst_pipe_obj->m_pipeline, GST_STATE_NULL);
    if (ret == GST_STATE_CHANGE_FAILURE)
    {
        printf("gst_wrapper: GST pipe set state NULL failed.\n");
        return -1;
    }
    return 0;
}

void appGstDeInit()
{
    app_gst_wrapper_obj_t* p_gst_pipe_obj = &g_app_gst_wrapper_obj;

    if(p_gst_pipe_obj->isAppSink)
    {
        for (uint8_t ch = 0; ch < p_gst_pipe_obj->params.out_num_channels; ch++)
        {
            gst_object_unref (p_gst_pipe_obj->m_sinkElemArr[ch]);
        }
    }
    if(p_gst_pipe_obj->isAppSrc)
    {
        for (uint8_t ch = 0; ch < p_gst_pipe_obj->params.in_num_channels; ch++)
        {
            for (uint8_t idx = 0; idx < p_gst_pipe_obj->params.in_buffer_depth; idx++)
            {
                gst_memory_unmap(p_gst_pipe_obj->mem[idx][ch][0],&p_gst_pipe_obj->map_info[idx][ch][0]);
                gst_memory_unmap(p_gst_pipe_obj->mem[idx][ch][1],&p_gst_pipe_obj->map_info[idx][ch][1]);

                gst_memory_unref(p_gst_pipe_obj->mem[idx][ch][0]);
                gst_memory_unref(p_gst_pipe_obj->mem[idx][ch][1]);

                gst_buffer_unref(p_gst_pipe_obj->buff[idx][ch]);

            }
            gst_object_unref (p_gst_pipe_obj->m_srcElemArr[ch]);
        }
    }
    gst_object_unref (p_gst_pipe_obj->m_pipeline);
}

void appGstPrintStats()
{
    app_gst_wrapper_obj_t* p_gst_pipe_obj = &g_app_gst_wrapper_obj;

    printf("GST_WRAPPER PUSH/PULL COUNTS:\n");
    printf("\n");
    printf("Push count : %d\n", p_gst_pipe_obj->push_count);
    printf("Pull count : %d\n", p_gst_pipe_obj->pull_count);
    printf("\n");
}
