/*
 *  Copyright (C) 2026 Texas Instruments Incorporated - https://www.ti.com/
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

#include <utils/drm_wrapper/include/drm_wrapper.h>
#include <utils/mem/include/app_mem.h>

#include <drm.h>
#include <xf86drm.h>
#include <xf86drmMode.h>
#include <drm_fourcc.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <poll.h>

#define APP_DRM_WRAPPER_LOG_ERROR(fmt, ...) \
    printf("[DRM_WRAPPER][ERROR] " fmt "\n", ##__VA_ARGS__)

#define APP_DRM_WRAPPER_LOG_INFO(fmt, ...) \
    printf("[DRM_WRAPPER][INFO] " fmt "\n", ##__VA_ARGS__)

/**
 * \brief Internal structure for DRM wrapper handle
 */
struct _app_drm_wrapper_handle
{
    /** \brief Configuration parameters */
    app_drm_wrapper_cfg_t cfg;

    /** \brief DRM device file descriptor */
    int drm_fd;

    /** \brief Framebuffer IDs for registered buffers */
    uint32_t fb_ids[APP_DRM_MAX_BUFQ_DEPTH];

    /** \brief CRTC information */
    drmModeCrtcPtr crtc_info;

    /** \brief Flag indicating if modeset has been done */
    int modeset_done;

    /** \brief Flag indicating first frame */
    int first_frame;

    /** \brief Flag indicating a page flip is in flight (cleared by VBlank callback) */
    volatile int page_flip_pending;
};

/**
 * \brief Page flip handler callback
 */
static void app_drm_page_flip_handler(int fd, uint32_t sequence,
                                      uint32_t tv_sec, uint32_t tv_usec,
                                      void *user_data)
{
    app_drm_wrapper_handle_t *handle = (app_drm_wrapper_handle_t *)user_data;
    handle->page_flip_pending = 0;
}

/**
 * \brief DRM event context for page flip handling
 */
static drmEventContext g_drm_event_ctx = {
    .version = DRM_EVENT_CONTEXT_VERSION,
    .page_flip_handler = app_drm_page_flip_handler,
};

void appDrmWrapperInitCfg(app_drm_wrapper_cfg_t *cfg)
{
    if (cfg == NULL)
    {
        return;
    }

    memset(cfg, 0, sizeof(app_drm_wrapper_cfg_t));

    cfg->width        = APP_DRM_DEFAULT_WIDTH;
    cfg->height       = APP_DRM_DEFAULT_HEIGHT;
    cfg->pix_format   = APP_DRM_DEFAULT_PIX_FMT;
    cfg->crtc_id      = APP_DRM_DEFAULT_CRTC_ID;
    cfg->connector_id = APP_DRM_DEFAULT_CONN_ID;
    cfg->bufq_depth   = APP_DRM_DEFAULT_BUFQ_DEPTH;
}

app_drm_wrapper_handle_t* appDrmWrapperCreate(app_drm_wrapper_cfg_t *cfg)
{
    app_drm_wrapper_handle_t *handle = NULL;

    if (cfg == NULL)
    {
        APP_DRM_WRAPPER_LOG_ERROR("Invalid config pointer");
        return NULL;
    }

    if (cfg->bufq_depth > APP_DRM_MAX_BUFQ_DEPTH)
    {
        APP_DRM_WRAPPER_LOG_ERROR("Buffer queue depth %u exceeds maximum %u",
                                  cfg->bufq_depth, APP_DRM_MAX_BUFQ_DEPTH);
        return NULL;
    }

    handle = (app_drm_wrapper_handle_t *)malloc(sizeof(app_drm_wrapper_handle_t));
    if (handle == NULL)
    {
        APP_DRM_WRAPPER_LOG_ERROR("Failed to allocate handle memory");
        return NULL;
    }

    memset(handle, 0, sizeof(app_drm_wrapper_handle_t));
    memcpy(&handle->cfg, cfg, sizeof(app_drm_wrapper_cfg_t));

    handle->drm_fd            = -1;
    handle->modeset_done      = 0;
    handle->first_frame       = 1;
    handle->page_flip_pending = 0;

    /* Open DRM device - tidss is the TI Display Subsystem driver */
    handle->drm_fd = drmOpen("tidss", NULL);
    if (handle->drm_fd < 0)
    {
        APP_DRM_WRAPPER_LOG_ERROR("Failed to open DRM device 'tidss': %d (%s)",
                                  errno, strerror(errno));
        goto error_free_handle;
    }

    /* Get CRTC information */
    handle->crtc_info = drmModeGetCrtc(handle->drm_fd, handle->cfg.crtc_id);
    if (handle->crtc_info == NULL)
    {
        APP_DRM_WRAPPER_LOG_ERROR("Failed to get CRTC info for ID %u",
                                  handle->cfg.crtc_id);
        goto error_close_drm;
    }

    APP_DRM_WRAPPER_LOG_INFO("DRM wrapper created: %ux%u, CRTC=%u, Connector=%u",
                             handle->cfg.width, handle->cfg.height,
                             handle->cfg.crtc_id, handle->cfg.connector_id);

    return handle;

error_close_drm:
    close(handle->drm_fd);

error_free_handle:
    free(handle);
    return NULL;
}

int32_t appDrmWrapperGetImageDmaBufFd(vx_image image,
                                      int32_t *fd,
                                      uint32_t *pitch,
                                      uint64_t *size,
                                      uint32_t *offset,
                                      uint32_t max_planes)
{
    vx_status status;
    vx_uint32 img_width;
    vx_uint32 img_height;
    vx_size num_planes;
    vx_df_image img_format;
    vx_rectangle_t rect;
    vx_imagepatch_addressing_t image_addr;
    vx_map_id map_id;
    void *data_ptr;
    uint32_t plane;

    if (image == NULL || fd == NULL || pitch == NULL || size == NULL || offset == NULL)
    {
        APP_DRM_WRAPPER_LOG_ERROR("Invalid parameters");
        return -1;
    }

    status = vxQueryImage(image, VX_IMAGE_WIDTH, &img_width, sizeof(vx_uint32));
    if (status != VX_SUCCESS)
    {
        APP_DRM_WRAPPER_LOG_ERROR("Failed to query image width");
        return -1;
    }

    status = vxQueryImage(image, VX_IMAGE_HEIGHT, &img_height, sizeof(vx_uint32));
    if (status != VX_SUCCESS)
    {
        APP_DRM_WRAPPER_LOG_ERROR("Failed to query image height");
        return -1;
    }

    status = vxQueryImage(image, VX_IMAGE_PLANES, &num_planes, sizeof(vx_size));
    if (status != VX_SUCCESS)
    {
        APP_DRM_WRAPPER_LOG_ERROR("Failed to query image planes");
        return -1;
    }

    status = vxQueryImage(image, VX_IMAGE_FORMAT, &img_format, sizeof(vx_df_image));
    if (status != VX_SUCCESS)
    {
        APP_DRM_WRAPPER_LOG_ERROR("Failed to query image format");
        return -1;
    }

    if (num_planes > max_planes)
    {
        APP_DRM_WRAPPER_LOG_ERROR("Number of planes %zu exceeds max %u",
                                  num_planes, max_planes);
        return -1;
    }

    rect.start_x = 0;
    rect.start_y = 0;
    rect.end_x   = img_width;
    rect.end_y   = img_height;

    for (plane = 0; plane < num_planes; plane++)
    {
        status = vxMapImagePatch(image,
                                 &rect,
                                 plane,
                                 &map_id,
                                 &image_addr,
                                 &data_ptr,
                                 VX_WRITE_ONLY,
                                 VX_MEMORY_TYPE_HOST,
                                 VX_NOGAP_X);
        if (status != VX_SUCCESS)
        {
            APP_DRM_WRAPPER_LOG_ERROR("Failed to map image plane %u", plane);
            return -1;
        }

        pitch[plane] = image_addr.stride_y;
        size[plane]  = (image_addr.dim_y / image_addr.step_y) *
                       ((image_addr.dim_x * image_addr.stride_x) / image_addr.step_x);

        /* Get DMA-BUF FD from TIOVX memory */
        fd[plane] = appMemGetDmaBufFd(data_ptr, &offset[plane]);

        vxUnmapImagePatch(image, map_id);

        if (fd[plane] < 0)
        {
            APP_DRM_WRAPPER_LOG_ERROR("Failed to get DMA-BUF FD for plane %u", plane);
            return -1;
        }
    }

    return (int32_t)num_planes;
}

int32_t appDrmWrapperRegisterBuffer(app_drm_wrapper_handle_t *handle,
                                    vx_image image,
                                    uint32_t buf_idx)
{
    int32_t status = 0;
    int32_t fd[APP_DRM_MAX_PLANES];
    uint32_t pitch[APP_DRM_MAX_PLANES];
    uint64_t size[APP_DRM_MAX_PLANES];
    uint32_t offset[APP_DRM_MAX_PLANES];
    uint32_t gem_handles[APP_DRM_MAX_PLANES];
    int32_t num_planes;
    int ret;
    uint32_t i;

    if (handle == NULL || image == NULL)
    {
        APP_DRM_WRAPPER_LOG_ERROR("Invalid handle or image");
        return -1;
    }

    if (buf_idx >= handle->cfg.bufq_depth)
    {
        APP_DRM_WRAPPER_LOG_ERROR("Buffer index %u >= bufq_depth %u",
                                  buf_idx, handle->cfg.bufq_depth);
        return -1;
    }

    /* Get DMA-BUF FDs from TIOVX image */
    num_planes = appDrmWrapperGetImageDmaBufFd(image, fd, pitch, size, offset,
                                               APP_DRM_MAX_PLANES);
    if (num_planes < 0)
    {
        APP_DRM_WRAPPER_LOG_ERROR("Failed to get DMA-BUF FDs");
        return -1;
    }

    /* Convert DMA-BUF FDs to GEM handles */
    memset(gem_handles, 0, sizeof(gem_handles));
    for (i = 0; i < (uint32_t)num_planes; i++)
    {
        ret = drmPrimeFDToHandle(handle->drm_fd, fd[i], &gem_handles[i]);
        if (ret < 0)
        {
            APP_DRM_WRAPPER_LOG_ERROR("Failed to convert FD to GEM handle for plane %u", i);
            /* Clean up previously created GEM handles */
            for (uint32_t j = 0; j < i; j++) {
                struct drm_gem_close gem_close = { .handle = gem_handles[j] };
                drmIoctl(handle->drm_fd, DRM_IOCTL_GEM_CLOSE, &gem_close);
            }
            return -1;
        }
    }

    /* Create DRM framebuffer */
    ret = drmModeAddFB2(handle->drm_fd,
                        handle->cfg.width,
                        handle->cfg.height,
                        handle->cfg.pix_format,
                        gem_handles,
                        pitch,
                        offset,
                        &handle->fb_ids[buf_idx],
                        0);
    if (ret < 0)
    {
        APP_DRM_WRAPPER_LOG_ERROR("Failed to add framebuffer: %d (%s)",
                                  errno, strerror(errno));
        return -1;
    }

    /* Perform modeset on first buffer registration */
    if (handle->modeset_done == 0)
    {
        ret = drmModeSetCrtc(handle->drm_fd,
                             handle->cfg.crtc_id,
                             handle->fb_ids[buf_idx],
                             0, 0,
                             &handle->cfg.connector_id, 1,
                             &handle->crtc_info->mode);
        if (ret < 0)
        {
            APP_DRM_WRAPPER_LOG_ERROR("Failed to set CRTC: %d (%s)",
                                      errno, strerror(errno));
            return -1;
        }
        handle->modeset_done = 1;
        APP_DRM_WRAPPER_LOG_INFO("Modeset completed");
    }

    APP_DRM_WRAPPER_LOG_INFO("Registered buffer %u, FB ID=%u",
                             buf_idx, handle->fb_ids[buf_idx]);

    return status;
}

int32_t appDrmWrapperWaitFlipDone(app_drm_wrapper_handle_t *handle)
{
    struct pollfd pfd;
    int ret;

    if (handle == NULL)
    {
        APP_DRM_WRAPPER_LOG_ERROR("Invalid handle");
        return -1;
    }

    if (!handle->page_flip_pending)
    {
        return 0;
    }

    memset(&pfd, 0, sizeof(pfd));
    pfd.fd     = handle->drm_fd;
    pfd.events = POLLIN;

    while (handle->page_flip_pending)
    {
        ret = poll(&pfd, 1, APP_DRM_PAGE_FLIP_TIMEOUT);
        if (ret > 0 && (pfd.revents & POLLIN))
        {
            drmHandleEvent(handle->drm_fd, &g_drm_event_ctx);
        }
        else if (ret == 0)
        {
            APP_DRM_WRAPPER_LOG_ERROR("Page flip timed out");
            handle->page_flip_pending = 0;
            break;
        }
        else
        {
            APP_DRM_WRAPPER_LOG_ERROR("poll() error: %d (%s)",
                                      errno, strerror(errno));
            break;
        }
    }

    return 0;
}

int32_t appDrmWrapperRender(app_drm_wrapper_handle_t *handle, uint32_t buf_idx)
{
    int32_t status = 0;
    struct pollfd pfd;
    int ret;

    if (handle == NULL)
    {
        APP_DRM_WRAPPER_LOG_ERROR("Invalid handle");
        return -1;
    }

    if (buf_idx >= handle->cfg.bufq_depth)
    {
        APP_DRM_WRAPPER_LOG_ERROR("Buffer index %u >= bufq_depth %u",
                                  buf_idx, handle->cfg.bufq_depth);
        return -1;
    }

    if (handle->fb_ids[buf_idx] == 0)
    {
        APP_DRM_WRAPPER_LOG_ERROR("Buffer %u not registered", buf_idx);
        return -1;
    }

    /*
     * Wait for the previous page flip to complete before touching the
     * back buffer.  The VBlank callback clears page_flip_pending; we must
     * not proceed until it does so the DSS has finished scanning out the
     * buffer we are about to overwrite.  Skip on first frame since no
     * flip has been submitted yet.
     */
    if (handle->first_frame == 0)
    {
        memset(&pfd, 0, sizeof(pfd));
        pfd.fd     = handle->drm_fd;
        pfd.events = POLLIN;

        while (handle->page_flip_pending)
        {
            ret = poll(&pfd, 1, APP_DRM_PAGE_FLIP_TIMEOUT);
            if (ret > 0 && (pfd.revents & POLLIN))
            {
                drmHandleEvent(handle->drm_fd, &g_drm_event_ctx);
            }
            else if (ret == 0)
            {
                APP_DRM_WRAPPER_LOG_ERROR("Page flip timed out");
                handle->page_flip_pending = 0;
                break;
            }
            else
            {
                APP_DRM_WRAPPER_LOG_ERROR("poll() error: %d (%s)",
                                          errno, strerror(errno));
                break;
            }
        }
    }
    else
    {
        handle->first_frame = 0;
    }

    /* Mark flip in-flight before submitting; callback clears it at VBlank */
    handle->page_flip_pending = 1;
    ret = drmModePageFlip(handle->drm_fd,
                          handle->cfg.crtc_id,
                          handle->fb_ids[buf_idx],
                          DRM_MODE_PAGE_FLIP_EVENT,
                          handle);
    if (ret < 0)
    {
        handle->page_flip_pending = 0;
        APP_DRM_WRAPPER_LOG_ERROR("Page flip failed: %d (%s)",
                                  errno, strerror(errno));
        return -1;
    }

    return status;
}

int32_t appDrmWrapperDelete(app_drm_wrapper_handle_t *handle)
{
    uint32_t i;

    if (handle == NULL)
    {
        return -1;
    }

    /* Remove framebuffers */
    for (i = 0; i < handle->cfg.bufq_depth; i++)
    {
        if (handle->fb_ids[i] != 0)
        {
            drmModeRmFB(handle->drm_fd, handle->fb_ids[i]);
            handle->fb_ids[i] = 0;
        }
    }

    /* Free CRTC info */
    if (handle->crtc_info != NULL)
    {
        drmModeFreeCrtc(handle->crtc_info);
        handle->crtc_info = NULL;
    }

    /* Close DRM device */
    if (handle->drm_fd >= 0)
    {
        close(handle->drm_fd);
        handle->drm_fd = -1;
    }

    free(handle);

    APP_DRM_WRAPPER_LOG_INFO("DRM wrapper deleted");

    return 0;
}
