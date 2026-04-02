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

#ifndef _APP_DRM_WRAPPER_H_
#define _APP_DRM_WRAPPER_H_

/* Standard headers */
#include <stdint.h>

/* TIOVX headers */
#include <VX/vx.h>
#include <TI/tivx.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup group_vision_apps_utils_drm_wrapper DRM/KMS Display Wrapper APIs
 *
 * \brief This section contains APIs to use Linux DRM/KMS display framework
 *        for rendering TIOVX image buffers with zero-copy via DMA-BUF
 *
 * \ingroup group_vision_apps_utils
 *
 * @{
 */

/** \brief Maximum buffer queue depth supported */
#define APP_DRM_MAX_BUFQ_DEPTH      (8u)

/** \brief Maximum number of planes per buffer */
#define APP_DRM_MAX_PLANES          (4u)

/** \brief Default display width */
#define APP_DRM_DEFAULT_WIDTH       (1920u)

/** \brief Default display height */
#define APP_DRM_DEFAULT_HEIGHT      (1080u)

/** \brief Default pixel format (NV12) - DRM_FORMAT_NV12 = 0x3231564E */
#define APP_DRM_DEFAULT_PIX_FMT     (0x3231564E)

/** \brief Default CRTC ID for AM62A */
#define APP_DRM_DEFAULT_CRTC_ID     (39u)

/** \brief Default Connector ID for AM62A */
#define APP_DRM_DEFAULT_CONN_ID     (41u)

/** \brief Default buffer queue depth */
#define APP_DRM_DEFAULT_BUFQ_DEPTH  (4u)

/** \brief Page flip timeout in milliseconds */
#define APP_DRM_PAGE_FLIP_TIMEOUT   (50u)

/**
 * \brief DRM wrapper configuration parameters
 */
typedef struct
{
    /** \brief Width of the image to render */
    uint32_t    width;

    /** \brief Height of the image to render */
    uint32_t    height;

    /** \brief DRM pixel format (e.g., DRM_FORMAT_NV12, DRM_FORMAT_ARGB8888) */
    uint32_t    pix_format;

    /** \brief CRTC ID of the DRM connector to use */
    uint32_t    crtc_id;

    /** \brief Connector ID of the DRM connector to use */
    uint32_t    connector_id;

    /** \brief Number of buffers in the buffer queue */
    uint32_t    bufq_depth;

} app_drm_wrapper_cfg_t;

/**
 * \brief Opaque handle for DRM wrapper instance
 */
typedef struct _app_drm_wrapper_handle app_drm_wrapper_handle_t;

/**
 * \brief Initialize DRM wrapper configuration with defaults
 *
 * This function sets the configuration structure to default values.
 * Call this before modifying specific parameters.
 *
 * \param [out] cfg     Pointer to configuration structure to initialize
 */
void appDrmWrapperInitCfg(app_drm_wrapper_cfg_t *cfg);

/**
 * \brief Create a DRM wrapper handle
 *
 * Opens the DRM device and initializes the display pipeline.
 *
 * \param [in] cfg      Pointer to configuration structure
 *
 * \return Handle to DRM wrapper instance, or NULL on failure
 */
app_drm_wrapper_handle_t* appDrmWrapperCreate(app_drm_wrapper_cfg_t *cfg);

/**
 * \brief Register a TIOVX image buffer for display
 *
 * Extracts DMA-BUF file descriptor from the TIOVX image and creates
 * a DRM framebuffer for it. This enables zero-copy display.
 *
 * \param [in] handle       DRM wrapper handle
 * \param [in] image        TIOVX image object to register
 * \param [in] buf_idx      Buffer index in the queue (0 to bufq_depth-1)
 *
 * \return 0 on success, negative value on failure
 */
int32_t appDrmWrapperRegisterBuffer(app_drm_wrapper_handle_t *handle,
                                    vx_image image,
                                    uint32_t buf_idx);

/**
 * \brief Render a registered buffer to the display
 *
 * Performs a page flip to display the specified buffer.
 * This function will wait for the previous page flip to complete
 * before submitting a new one.
 *
 * \param [in] handle       DRM wrapper handle
 * \param [in] buf_idx      Buffer index to display (must be registered first)
 *
 * \return 0 on success, negative value on failure
 */
int32_t appDrmWrapperRender(app_drm_wrapper_handle_t *handle, uint32_t buf_idx);

/**
 * \brief Delete DRM wrapper handle and release resources
 *
 * Closes the DRM device and frees all associated resources.
 *
 * \param [in] handle       DRM wrapper handle to delete
 *
 * \return 0 on success, negative value on failure
 */
int32_t appDrmWrapperDelete(app_drm_wrapper_handle_t *handle);

/**
 * \brief Get DMA-BUF file descriptor from a TIOVX image
 *
 * Utility function to extract DMA-BUF file descriptors from TIOVX image
 * for external use (e.g., with other DRM/V4L2 subsystems).
 *
 * \param [in]  image       TIOVX image object
 * \param [out] fd          Array to store file descriptors for each plane
 * \param [out] pitch       Array to store pitch (stride) for each plane
 * \param [out] size        Array to store size for each plane
 * \param [out] offset      Array to store offset for each plane
 * \param [in]  max_planes  Maximum number of planes to extract
 *
 * \return Number of planes extracted, or negative value on failure
 */
int32_t appDrmWrapperGetImageDmaBufFd(vx_image image,
                                      int32_t *fd,
                                      uint32_t *pitch,
                                      uint64_t *size,
                                      uint32_t *offset,
                                      uint32_t max_planes);

/* @} */

#ifdef __cplusplus
}
#endif

#endif /* _APP_DRM_WRAPPER_H_ */
