/******************************************************************************
Copyright (c) [2012 - 2017] Texas Instruments Incorporated

All rights reserved not granted herein.

Limited License.

 Texas Instruments Incorporated grants a world-wide, royalty-free, non-exclusive
 license under copyrights and patents it now or hereafter owns or controls to
 make,  have made, use, import, offer to sell and sell ("Utilize") this software
 subject to the terms herein.  With respect to the foregoing patent license,
 such license is granted  solely to the extent that any such patent is necessary
 to Utilize the software alone.  The patent license shall not apply to any
 combinations which include this software, other than combinations with devices
 manufactured by or for TI ("TI Devices").  No hardware patent is licensed
 hereunder.

 Redistributions must preserve existing copyright notices and reproduce this
 license (including the above copyright notice and the disclaimer and
 (if applicable) source code license limitations below) in the documentation
 and/or other materials provided with the distribution

 Redistribution and use in binary form, without modification, are permitted
 provided that the following conditions are met:

 * No reverse engineering, decompilation, or disassembly of this software
   is permitted with respect to any software provided in binary form.

 * Any redistribution and use are licensed by TI for use only with TI Devices.

 * Nothing shall obligate TI to provide you with source code for the software
   licensed and provided to you in object code.

 If software source code is provided to you, modification and redistribution of
 the source code are permitted provided that the following conditions are met:

 * Any redistribution and use of the source code, including any resulting
   derivative works, are licensed by TI for use only with TI Devices.

 * Any redistribution and use of any object code compiled from the source code
   and any resulting derivative works, are licensed by TI for use only with TI
   Devices.

 Neither the name of Texas Instruments Incorporated nor the names of its
 suppliers may be used to endorse or promote products derived from this software
 without specific prior written permission.

 DISCLAIMER.

 THIS SOFTWARE IS PROVIDED BY TI AND TI�S LICENSORS "AS IS" AND ANY EXPRESS OR
 IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 IN NO EVENT SHALL TI AND TI�S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
******************************************************************************/

/**
 *******************************************************************************
 *
 * \defgroup DRAW_2D_API Font and 2D Drawing API
 *
 * \brief  This module has the interface for drawing fonts and 2D primitives
 *         like lines
 *
 *         NOTE: This is limited demo API and not a comprehensive 2D drawing
 *               library
 *
 * \ingroup group_vision_apps_utils
 * @{
 *
 *******************************************************************************
 */

/**
 *******************************************************************************
 *
 * \file draw2d.h
 *
 * \brief Font and 2D Drawing API
 *
 * \version 0.0 (Oct 2013) : [KC] First version
 *
 *******************************************************************************
 */

#ifndef _DRAW_2D_H_
#define _DRAW_2D_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 *  INCLUDE FILES
 *******************************************************************************
 */
#include <TI/tivx.h>

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <math.h>
/*******************************************************************************
 *  Defines
 *******************************************************************************
 */

 #define DRAW2D_MAX_PLANES   (3)

/**
 * \brief Enums for data format.
 *
 *  All data formats may not be supported by all links. For supported data
 *  formats please look link header file.
 *
 *
 */
 typedef enum
 {
     DRAW2D_DF_YUV422I_UYVY = 0x0000,
     /**< YUV 422 Interleaved format - UYVY. */
     DRAW2D_DF_YUV422I_YUYV,
     /**< YUV 422 Interleaved format - YUYV. */
     DRAW2D_DF_YUV422I_YVYU,
     /**< YUV 422 Interleaved format - YVYU. */
     DRAW2D_DF_YUV422I_VYUY,
     /**< YUV 422 Interleaved format - VYUY. */
     DRAW2D_DF_YUV422SP_UV,
     /**< YUV 422 Semi-Planar - Y separate, UV interleaved. */
     DRAW2D_DF_YUV422SP_VU,
     /**< YUV 422 Semi-Planar - Y separate, VU interleaved. */
     DRAW2D_DF_YUV422P,
     /**< YUV 422 Planar - Y, U and V separate. */
     DRAW2D_DF_YUV420SP_UV,
     /**< YUV 420 Semi-Planar - Y separate, UV interleaved. */
     DRAW2D_DF_YUV420SP_VU,
     /**< YUV 420 Semi-Planar - Y separate, VU interleaved. */
     DRAW2D_DF_YUV420P,
     /**< YUV 420 Planar - Y, U and V separate. */
     DRAW2D_DF_YUV444P,
     /**< YUV 444 Planar - Y, U and V separate. */
     DRAW2D_DF_YUV444I,
     /**< YUV 444 interleaved - YUVYUV... */
     DRAW2D_DF_RGB16_565,
     /**< RGB565 16-bit - 5-bits R, 6-bits G, 5-bits B. */
     DRAW2D_DF_BGRA16_4444,
     /**< BGRA 16-bit - 4-bits R, 4-bits G, 4-bits B, 4-bits Alpha (LSB). */
     DRAW2D_DF_RGB24_888,
     /**< RGB24 24-bit - 8-bits R, 8-bits G, 8-bits B. */
     DRAW2D_DF_ARGB32_8888,
     /**< ARGB32 32-bit - 8-bits R, 8-bits G, 8-bits B, 8-bit Alpha (MSB). */
     DRAW2D_DF_RGBA32_8888,
     /**< RGBA32 32-bit - 8-bits R, 8-bits G, 8-bits B, 8-bit Alpha (LSB). */
     DRAW2D_DF_BGR16_565,
     /**< BGR565 16-bit -   5-bits B, 6-bits G, 5-bits R. */
     DRAW2D_DF_BGR24_888,
     /**< BGR888 24-bit - 8-bits B, 8-bits G, 8-bits R. */
     DRAW2D_DF_ABGR32_8888,
     /**< ABGR8888 32-bit - 8-bits B, 8-bits G, 8-bits R, 8-bit Alpha (MSB). */
     DRAW2D_DF_BGRA32_8888,
     /**< BGRA8888 32-bit - 8-bits B, 8-bits G, 8-bits R, 8-bit Alpha (LSB). */
     DRAW2D_DF_MISC,
     /**< For future purpose. */
     DRAW2D_DF_INVALID,
     /**< Invalid data format. Could be used to initialize variables. */
     DRAW2D_DF_FORCE32BITS = 0xFFFFFFFF
     /**< This should be the last value after the max enumeration value.
     *   This is to make sure enum size defaults to 32 bits always regardless
     *   of compiler.
     */
 }Draw2D_DataFormat;

 /**
  *
  * \brief Macro that converts RGB888 to RGB565
  *
  */

#define RGB888_TO_RGB565(r,g,b)     ((((uint32_t)(r>>3) & 0x1F) << 11) | (((uint32_t)(g>>2) & 0x3F) << 5) | (((uint32_t)(b>>3) & 0x1F)))
#define RGB888_TO_BGRA444(r,g,b,a)  ((((uint32_t)(r>>4) & 0xF) << 0) | (((uint32_t)(g>>4) & 0xF) << 4) | (((uint32_t)(b>>4) & 0xF)<<8)| (((uint32_t)(a>>4) & 0xF)<<12))

#define DRAW2D_TRANSPARENT_COLOR            (RGB888_TO_RGB565(0, 0, 0))
#define DRAW2D_TRANSPARENT_COLOR_FORMAT     (DRAW2D_DF_BGR16_565)

/* white logo with transperency */
#define DRAW2D_BMP_IDX_TI_LOGO_0            (0)
/* red+white logo with black background */
#define DRAW2D_BMP_IDX_TI_LOGO_1            (1)
/* white logo with black background */
#define DRAW2D_BMP_IDX_TI_LOGO_2            (2)
/* white logo with black background */
#define DRAW2D_BMP_IDX_TI_LOGO_3            (3)
/* red+white logo (small) with black background */
#define DRAW2D_BMP_IDX_TI_LOGO_4            (4)
/* white logo (small) with black background */
#define DRAW2D_BMP_IDX_TI_LOGO_5            (5)
/* DOF Colour Map */
#define DRAW2D_BMP_IDX_DOF_COLOUR_MAP       (6)
/* Stereo Vision Colour Map */
#define DRAW2D_BMP_IDX_SDE_COLOUR_MAP       (7)


/*
 *  \brief  Font property
 */
typedef struct {

    uint8_t *addr;
    /**< memory address location of the font */

    uint32_t width;
    /**< Width of a character of font in pixels */

    uint32_t height;
    /**< Height of a character of font in lines */

    uint32_t bpp;
    /**< bytes per pixel of the font in bytes */

    uint32_t lineOffset;
    /**< line offset of the font in bytes */

    uint32_t num;
    /**< Number of characters in font */

    uint32_t colorFormat;
    /**<
     *   Currently below data formats are supported
     *   - DRAW2D_DF_BGR16_565
     */

} Draw2D_FontProperty;

/*
 *  \brief  Font property
 */
typedef struct {

    uint8_t *addr;
    /**< memory address location of the bitmap */

    uint32_t width;
    /**< Width of a bitmap data in pixels */

    uint32_t height;
    /**< Height of a bitmap data in lines */

    uint32_t bpp;
    /**< bytes per pixel of the bitmap in bytes */

    uint32_t lineOffset;
    /**< line offset of the bitmap in bytes */

    uint32_t colorFormat;
    /**<
     *   Currently below data formats are supported
     *   - DRAW2D_DF_BGR16_565
     */

} Draw2D_BmpProperty;

/**
 *  \brief  Buffer information into which font and 2d primitives will be draw
 */
typedef struct
{
    uint8_t *bufAddr[DRAW2D_MAX_PLANES];
    /**< Address of buffer memory */

    uint32_t bufWidth;
    /**< Width of buffer in pixels */

    uint32_t bufHeight;
    /**< Height of buffer in lines */

    uint32_t bufPitch[DRAW2D_MAX_PLANES];
    /**< Pitch of buffer in Bytes */

    uint32_t dataFormat;
    /**< Valid values of type System_VideoDataFormat
     *   Currently below data formats are supported
     *   - DRAW2D_DF_BGR16_565
     *   - DRAW2D_DF_YUV422I_YVYU
     *   - DRAW2D_DF_YUV420SP_UV
     *   - DRAW2D_DF_BGRA16_4444
     */

    uint32_t transperentColor;
    /**< Color used to represent transperent color
     *   Buffer will be filled with this color initially and when
     *   Draw2D_clearBuf() is called
     *
     *   Representation of color in this field depends on the data format
     *   ex, 16-bit value for RGB565 data format
     */

    uint32_t transperentColorFormat;
    /**<
     *   Currently below data formats are supported
     *   - DRAW2D_DF_BGR16_565
     */

} Draw2D_BufInfo;

/**
 *  \brief Font parameters
 */
typedef struct
{
    uint32_t fontIdx;
    /**< Font index is used to select the font to use for drawing.
     */

} Draw2D_FontPrm;

/**
 *  \brief Bitmap parameters
 */
typedef struct
{
    uint32_t bmpIdx;
    /**< Bitmap index is used to select the bitmap to use for drawing.
     */

} Draw2D_BmpPrm;

/**
 *  \brief Line draw parameters
 */
typedef struct
{
    uint32_t lineColor;
    /**< Color used to draw a line
     *
     *   Representation of color in this field depends on the data format
     *   set during Draw2D_setBufInfo()
     *
     *   ex, 16-bit value for RGB565 data format
     */

    uint32_t lineSize;
    /**< Size of line in pixels to draw
     */

    uint32_t lineColorFormat;
    /**<
     *   Currently below data formats are supported
     *   - DRAW2D_DF_BGR16_565
     *   - DRAW2D_DF_YUV422I_YVYU
     *   - DRAW2D_DF_YUV420SP_UV
     *   - DRAW2D_DF_BGRA16_4444
     */

} Draw2D_LinePrm;

/**
 *******************************************************************************
 *
 *  \brief Region Params
 *
 *******************************************************************************
 */
typedef struct
{
    uint32_t startX;
    /**< X Position where region starts */
    uint32_t startY;
    /**< Y position where region starts */
    uint32_t height;
    /**< Height of the region */
    uint32_t width;
    /**< Width of the region */
    uint32_t color;
    /**< Color to be filled in this region */
    uint32_t colorFormat;
    /**<
     *   Currently below data formats are supported
     *   - DRAW2D_DF_BGR16_565
     *   - DRAW2D_DF_YUV422I_YVYU
     *   - DRAW2D_DF_YUV420SP_UV
     *   - DRAW2D_DF_BGRA16_4444
     */
}Draw2D_RegionPrm;

/**
 *  \brief Draw 2D object handle
 */
typedef void *Draw2D_Handle;


/*
 *  Functions
 */

/**
 * \brief Create a context for drawing
 *
 *        MUST be called before calling any other Draw2D API
 *
 * \param  pCtx    [OUT] Created Draw 2D context
 *
 * \return  VX_SUCCESS on success
 */
int32_t Draw2D_create(Draw2D_Handle *pCtx);

/**
 * \brief Delete a previously created drawing context
 *
 * \param  pCtx    [IN] Draw 2D context
 *
 * \return  VX_SUCCESS on success
 */
int32_t Draw2D_delete(Draw2D_Handle pCtx);


/**
 * \brief Associated a drawing buffer with a drawing context
 *
 *        This API MUST be called after Draw2D_create() and before any drawing
 *        API
 *
 * \param  pCtx     [IN] Draw 2D context
 * \param  pBufInfo [IN] Buffer information
 *
 * \return  VX_SUCCESS on success
 */
int32_t Draw2D_setBufInfo(Draw2D_Handle pCtx, Draw2D_BufInfo *pBufInfo);

/**
 * \brief Update drawing buffer
 *
 *        This API can be used to update the buffer address when needed
 *
 * \param  pCtx     [IN] Draw 2D context
 * \param  bufAddr  [IN] Array of buffer addresses
 *
 * \return  VX_SUCCESS on success
 */
void Draw2D_updateBufAddr(Draw2D_Handle pCtx, uint8_t **bufAddr);

/**
 * \brief Fill buffer with transperency color
 *
 * \param  pCtx     [IN] Draw 2D context
 *
 * \return  VX_SUCCESS on success
 */
int32_t Draw2D_clearBuf(Draw2D_Handle pCtx);

/**
 * \brief Draw string of character into the drawing buffer
 *
 * \param  pCtx     [IN] Draw 2D context
 * \param  startX   [IN] X-position in the buffer
 * \param  startY   [IN] Y-position in the buffer
 * \param  str      [IN] Ascii string to draw
 * \param  pPrm     [IN] Font to use when drawing,
 *                       when set to NULL default properties used
 *
 * \return  VX_SUCCESS on success
 */
int32_t Draw2D_drawString(Draw2D_Handle pCtx,
                        uint32_t startX,
                        uint32_t startY,
                        char *str,
                        Draw2D_FontPrm *pPrm);

/**
 * \brief Draw string of character into the drawing buffer
 *
 * \param  pCtx     [IN] Draw 2D context
 * \param  startX   [IN] X-position in the buffer
 * \param  startY   [IN] Y-position in the buffer
 * \param  str      [IN] Ascii string to draw
 * \param  pPrm     [IN] Font to use when drawing,
 *                       when set to NULL default properties used
 * \param  rotate   [IN] Set 1 to rotate
 *
 * \return  VX_SUCCESS on success
 */
int32_t Draw2D_drawString_rot(Draw2D_Handle pCtx,
                        uint32_t startX,
                        uint32_t startY,
                        char *str,
                        Draw2D_FontPrm *pPrm,
                        uint32_t rotate);

/**
 * \brief Clear a area equal to stringLength in the drawing buffer
 *
 *        This is used to erase a string of previously written characters.
 *        Internal this draws the 'SPACE' stringLength times
 *
 * \param  pCtx         [IN] Draw 2D context
 * \param  startX       [IN] start X-position in the buffer
 * \param  startY       [IN] start Y-position in the buffer
 * \param  stringLength [IN] Length of string to clear
 * \param  pPrm         [IN] Font to use when drawing,
 *                       when set to NULL default properties used
 *
 * \return  VX_SUCCESS on success
 */
int32_t Draw2D_clearString(Draw2D_Handle pCtx,
                            uint32_t startX,
                            uint32_t startY,
                            uint32_t stringLength,
                            Draw2D_FontPrm *pPrm);


/**
 * \brief Get properties of a given font
 *
 * \param  pPrm         [IN] Font to use when drawing,
 *                       when set to NULL default properties used
 * \param  pProp        [OUT] Font properties like width x height, bpp etc
 *
 * \return  VX_SUCCESS on success
 */
int32_t Draw2D_getFontProperty(Draw2D_FontPrm *pPrm, Draw2D_FontProperty *pProp);

/**
 * \brief Get properties of a given bitmap
 *
 * \param  pPrm         [IN] Bitmap to use when drawing,
 *                       when set to NULL default properties used
 * \param  pProp        [OUT] Bitmap properties like width x height, bpp etc
 *
 * \return  VX_SUCCESS on success
 */
int32_t Draw2D_getBmpProperty(Draw2D_BmpPrm *pPrm, Draw2D_BmpProperty *pProp);

/**
 * \brief Draw a line in the drawing buffer
 *
 *        Currently only horizontal or vertical lines can be drawn
 *        So make sure endX or endY  is equal to startX or startY
 *
 *        To clear a line, call the same API with color as transperency
 *        color in line properties
 *
 * \param  pCtx         [IN] Draw 2D context
 * \param  startX       [IN] start X-position in the buffer
 * \param  startY       [IN] start Y-position in the buffer
 * \param  endX         [IN] end X-position in the buffer
 * \param  endY         [IN] end Y-position in the buffer
 * \param  pPrm         [IN] Line properties to use when drawing,
 *                       when set to NULL default properties used
 *
 * \return  VX_SUCCESS on success
 */
int32_t Draw2D_drawLine(Draw2D_Handle pCtx,
                        uint32_t startX,
                        uint32_t startY,
                        uint32_t endX,
                        uint32_t endY,
                        Draw2D_LinePrm *pPrm);

/**
 * \brief Draw a rectangle in the drawing buffer
 *
 *        Internally uses Draw2D_drawLine() 4 times to draw the rectangle
 *
 *        To clear a rectangle, call the same API with color as transperency
 *        color in line properties
 *
 * \param  pCtx         [IN] Draw 2D context
 * \param  startX       [IN] start X-position in the buffer
 * \param  startY       [IN] start Y-position in the buffer
 * \param  width        [IN] width of rectangle in pixels
 * \param  height       [IN] height of rectangle in lines
 * \param  pPrm         [IN] Line properties to use when drawing,
 *                       when set to NULL default properties used
 *
 * \return  VX_SUCCESS on success
 */
int32_t Draw2D_drawRect(Draw2D_Handle pCtx,
                        uint32_t startX,
                        uint32_t startY,
                        uint32_t width,
                        uint32_t height,
                        Draw2D_LinePrm *pPrm);

/**
 * \brief Clear a region in the drawing buffer with transperency color
 *
 * \param  pCtx         [IN] Draw 2D context
 * \param  startX       [IN] start X-position in the buffer
 * \param  startY       [IN] start Y-position in the buffer
 * \param  width        [IN] width of region in pixels
 * \param  height       [IN] height of region in lines
 *
 * \return  VX_SUCCESS on success
 */
int32_t Draw2D_clearRegion(Draw2D_Handle pCtx,
                            uint32_t startX,
                            uint32_t startY,
                            uint32_t width,
                            uint32_t height);


/**
 * \brief Draw a region in the drawing buffer with custom color
 *
 * \param  pCtx         [IN] Draw 2D context
 * \param  prm          [IN] Region Parameters
 *
 * \return  VX_SUCCESS on success
 */
int32_t Draw2D_fillRegion(Draw2D_Handle pCtx, Draw2D_RegionPrm *prm);


/**
 * \brief Draw pixel of a given color
 *
 *        Representation of color in this function depends on the data format
 *        set during Draw2D_setBufInfo()
 *
 *        ex, 16-bit value for RGB565 data format
 *
 * \param  pCtx         [IN] Draw 2D context
 * \param  px           [IN] X-position in the buffer
 * \param  py           [IN] Y-position in the buffer
 * \param  color        [IN] Color used to draw the pixel
 * \param  colorFormat  [IN] Color format of the color
 *
 * \return  VX_SUCCESS on success
 */
void Draw2D_drawPixel(Draw2D_Handle pCtx, uint32_t px, uint32_t py, uint32_t color, uint32_t colorFormat);


/**
 * \brief Draw a bitmap into the drawing buffer
 *
 * \param  pCtx     [IN] Draw 2D context
 * \param  startX   [IN] X-position in the buffer
 * \param  startY   [IN] Y-position in the buffer
 * \param  pPrm     [IN] Bitmap to use when drawing,
 *                       when set to NULL default properties used
 *
 * \return  VX_SUCCESS on success
 */
int32_t Draw2D_drawBmp(Draw2D_Handle pCtx,
                        uint32_t startX,
                        uint32_t startY,
                        Draw2D_BmpPrm *pPrm);

/**
 * \brief Draw a bitmap into the drawing buffer
 *
 * \param  pCtx     [IN] Draw 2D context
 * \param  startX   [IN] X-position in the buffer
 * \param  startY   [IN] Y-position in the buffer
 * \param  pPrm     [IN] Bitmap to use when drawing,
 *                       when set to NULL default properties used
 * \param  rotate   [IN] rotation angle, 0: 90 deg, 1: 180 deg, 2: 270 deg
 *
 * \return  VX_SUCCESS on success
 */
int32_t Draw2D_drawBmp_rot(Draw2D_Handle pCtx,
                        uint32_t startX,
                        uint32_t startY,
                        Draw2D_BmpPrm *pPrm,
                        uint32_t rotate);

/**
 * \brief Draw a bitmap from BMP file at a specified display buffer location
 *
 * \param pCtx       [IN] Draw 2D context
 * \param input_file [IN] input .BMP file name
 * \param startX     [IN] X position in display buffer
 * \param startY     [IN] Y position in display buffer
 */
int32_t Draw2D_insertBmp(Draw2D_Handle pCtx,
                         char *input_file,
                         int32_t startX,
                         int32_t startY);

/**
 * \brief Draw a bitmap from BMP file (pre-copied at a memory location) at a specified display buffer location
 *
 * \param pCtx       [IN] Draw 2D context
 * \param buf        [IN] .BMP file in memory location
 * \param buf_size   [IN] size of BMP file buffer
 * \param startX     [IN] X position in display buffer
 * \param startY     [IN] Y position in display buffer
 *
 */
int32_t Draw2D_insertBmpFromMemory(Draw2D_Handle pCtx,
                         void *buf,
                         uint32_t buf_size,
                         int32_t startX,
                         int32_t startY);

/** \brief Sets a global color to use for fonts in RGB565 format
 *
 *  \param colorText    [IN] Color of text
 *  \param colorBorder  [IN] Color of text border
 *  \param colorBg      [IN] Color of text background
 */
void Draw2D_setFontColor(uint16_t colorText, uint16_t colorBorder, uint16_t colorBg );

/** \brief Restore font to default */
void Draw2D_resetFontColor();

/**
 *******************************************************************************
 *
 * \brief Initialize In queue parameters of a link
 *
 * \param val         [IN]   Value to be rounded off
 *
 * \param align       [IN]   Value to which val to be rounded
 *
 * \return aligned value of val
 *
 *******************************************************************************
 */
static inline uint32_t Draw2D_floor(uint32_t val, uint32_t align)
{
    return (((val) / (align)) * (align));
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* _DRAW_2D_H_ */

/* @} */

/* Nothing beyond this point */
