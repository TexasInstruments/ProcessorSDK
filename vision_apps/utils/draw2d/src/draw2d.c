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

#include <utils/draw2d/src/draw2d_priv.h>
#include <tivx_utils_file_rd_wr.h>

#define RGB565_TO_BGRA444(x)        ((((uint32_t)(x>>1) & 0xF) << 0) | (((uint32_t)(x>>7) & 0xF) << 4) | (((uint32_t)(x>>12) & 0xF)<<8)| (((uint32_t)(0xF) & 0xF)<<12))

int32_t Draw2D_create(Draw2D_Handle *pHndl)
{
    int32_t status = VX_SUCCESS;
    Draw2D_Obj *pObj;

    *pHndl = NULL;

    pObj = malloc(sizeof(*pObj));
    if(pObj==NULL)
        return VX_FAILURE;

    memset(pObj, 0, sizeof(*pObj));

    *pHndl = pObj;

    return status;
}

int32_t Draw2D_delete(Draw2D_Handle pHndl)
{
    int32_t status = VX_SUCCESS;

    if(pHndl)
        free(pHndl);

    return status;
}

int32_t Draw2D_setBufInfo(Draw2D_Handle pHndl, Draw2D_BufInfo *pBufInfo)
{
    int32_t status = VX_SUCCESS;
    Draw2D_Obj *pObj = (Draw2D_Obj *)pHndl;

    if(pHndl==NULL || pBufInfo == NULL)
        return VX_FAILURE;

    pObj->bufInfo = *pBufInfo;

    if(pObj->bufInfo.dataFormat==DRAW2D_DF_BGR16_565
        ||
       pObj->bufInfo.dataFormat==DRAW2D_DF_BGRA16_4444
        ||
       pObj->bufInfo.dataFormat==DRAW2D_DF_YUV422I_YUYV
        ||
       pObj->bufInfo.dataFormat==DRAW2D_DF_YUV420SP_UV
        )
    {

    }
    else
    {
        return VX_FAILURE;
    }

    if(pObj->bufInfo.bufAddr[0] == (uint8_t *)NULL
        ||
       pObj->bufInfo.bufWidth == 0
        ||
       pObj->bufInfo.bufPitch[0] == 0
        ||
       pObj->bufInfo.bufHeight == 0
        )
    {
        return VX_FAILURE;
    }

    if (pObj->bufInfo.dataFormat == DRAW2D_DF_YUV420SP_UV
         &&
        (pObj->bufInfo.bufAddr[1] == (uint8_t *)NULL
         ||
        pObj->bufInfo.bufPitch[1] == 0)
        )
    {
        return VX_FAILURE;
    }

    return status;
}

void Draw2D_updateBufAddr(Draw2D_Handle pHndl, uint8_t **bufAddr)
{
    Draw2D_Obj *pObj = (Draw2D_Obj *)pHndl;

    pObj->bufInfo.bufAddr[0] = bufAddr[0];
    pObj->bufInfo.bufAddr[1] = bufAddr[1];
    pObj->bufInfo.bufAddr[2] = bufAddr[2];

}

int32_t Draw2D_clearBuf(Draw2D_Handle pCtx)
{
    int32_t status;
    Draw2D_Obj *pObj = (Draw2D_Obj *)pCtx;

    if(pObj==NULL)
        return VX_FAILURE;

    status = Draw2D_clearRegion(
                pCtx,
                0,
                0,
                pObj->bufInfo.bufWidth,
                pObj->bufInfo.bufHeight);

    return status;
}

int32_t Draw2D_clearRegion(Draw2D_Handle pCtx,
                            uint32_t startX,
                            uint32_t startY,
                            uint32_t width,
                            uint32_t height)
{
    Draw2D_RegionPrm regionPrm;
    Draw2D_Obj *pObj = (Draw2D_Obj *)pCtx;

    if(pObj==NULL)
        return VX_FAILURE;

    regionPrm.startX = startX;
    regionPrm.startY = startY;
    regionPrm.width  = width;
    regionPrm.height = height;
    regionPrm.color  = pObj->bufInfo.transperentColor;
    regionPrm.colorFormat  = pObj->bufInfo.transperentColorFormat;

    return Draw2D_fillRegion(pCtx, &regionPrm);
}

int32_t Draw2D_fillRegion(Draw2D_Handle pCtx, Draw2D_RegionPrm *regionPrm)
{
    int32_t status = VX_SUCCESS;
    Draw2D_Obj *pObj = (Draw2D_Obj *)pCtx;
    uint32_t x, y;

    if(pObj==NULL)
        return VX_FAILURE;

    if(regionPrm->startX >= pObj->bufInfo.bufWidth)
        return 0;

    if(regionPrm->startY >= pObj->bufInfo.bufHeight)
        return 0;

    if((regionPrm->startX + regionPrm->width)> pObj->bufInfo.bufWidth)
    {
        regionPrm->width = pObj->bufInfo.bufWidth - regionPrm->startX;
    }

    if((regionPrm->startY + regionPrm->height)> pObj->bufInfo.bufHeight)
    {
        regionPrm->height = pObj->bufInfo.bufHeight - regionPrm->startY;
    }

    for(x=regionPrm->startX; x< regionPrm->startX+regionPrm->width; x++)
    {
        for(y=regionPrm->startY; y< regionPrm->startY+regionPrm->height; y++)
        {
            Draw2D_drawPixel(
                pCtx,
                x,
                y,
                regionPrm->color,
                regionPrm->colorFormat
                );
        }
    }

    return status;
}

void Draw2D_drawCharYuv420SP(Draw2D_Handle pCtx, uint32_t px, uint32_t py, char value, Draw2D_FontProperty *pProp)
{
    Draw2D_Obj *pObj = (Draw2D_Obj *)pCtx;
    uint32_t width, height, h;
    uint8_t *addr;
    uint8_t *fontAddr;

    if(pCtx==NULL)
        return ;

    py = Draw2D_floor(py, 2);
    px = Draw2D_floor(px, 2);

    if(px >= pObj->bufInfo.bufWidth)
        return ;

    if(py >= pObj->bufInfo.bufHeight)
        return ;

    width = pProp->width;
    height= pProp->height;

    if( py+height >= pObj->bufInfo.bufHeight)
    {
        height = pObj->bufInfo.bufHeight-py;
    }
    if( px+width >= pObj->bufInfo.bufWidth)
    {
        width = pObj->bufInfo.bufWidth-px;
    }

    fontAddr = (uint8_t*)Draw2D_getFontCharAddr(pProp, value);

    addr = pObj->bufInfo.bufAddr[0]
        + pObj->bufInfo.bufPitch[0]*py + px;

    for(h=0; h<height; h++)
    {
        memcpy(addr, fontAddr, pProp->width);
        fontAddr += pProp->lineOffset;
        addr += pObj->bufInfo.bufPitch[0];
    }

    fontAddr = (uint8_t *)Draw2D_getFontCharAddr(pProp, value);

    fontAddr += pProp->lineOffset*pProp->height;

    addr = pObj->bufInfo.bufAddr[1]
        + pObj->bufInfo.bufPitch[1]*py/2 + px;

    for(h=0; h<height/2; h++)
    {
        memcpy(addr, fontAddr, pProp->width);
        fontAddr += pProp->lineOffset;
        addr += pObj->bufInfo.bufPitch[1];
    }
}

void Draw2D_drawPixel(Draw2D_Handle pCtx, uint32_t px, uint32_t py, uint32_t color, uint32_t colorFormat)
{
    Draw2D_Obj *pObj = (Draw2D_Obj *)pCtx;
    uint8_t *addr;

    if(pCtx==NULL)
        return ;

    if(px >= pObj->bufInfo.bufWidth)
        return ;

    if(py >= pObj->bufInfo.bufHeight)
        return ;

    if(pObj->bufInfo.dataFormat==DRAW2D_DF_BGRA16_4444)
    {
        if(colorFormat==DRAW2D_DF_BGR16_565)
            color = RGB565_TO_BGRA444(color);

        addr = pObj->bufInfo.bufAddr[0]
            + pObj->bufInfo.bufPitch[0]*py + 2*px;

        *(uint16_t*)addr = (color & 0xFFFF);
    }
    else
    if(pObj->bufInfo.dataFormat==DRAW2D_DF_BGR16_565)
    {
        /* color in BGR565 is represented as,
         * bit  0.. 4 = B
         * bit  5..10 = G
         * bit 11..15 = R
         * bit 16..23 = NOT USED
         * bit 24..31 = NOT USED
         */

        addr = pObj->bufInfo.bufAddr[0]
            + pObj->bufInfo.bufPitch[0]*py + 2*px;

        *(uint16_t*)addr = (color & 0xFFFF);
    }
    else
    if(pObj->bufInfo.dataFormat==DRAW2D_DF_YUV422I_YUYV)
    {
        /* color in YUV422 is represented as,
         * bit  0.. 7 = Y
         * bit  8..15 = U
         * bit 16..23 = Y
         * bit 24..31 = V
         */

        /* x MUST be multiple of 2 */
        px = Draw2D_floor(px, 2);

        addr = pObj->bufInfo.bufAddr[0]
            + pObj->bufInfo.bufPitch[0]*py + 2*px;

        *(uint32_t*)addr = color;
    }
    else
    if(pObj->bufInfo.dataFormat==DRAW2D_DF_YUV420SP_UV)
    {
        /* color in YUV420 is represented as,
         * bit  0.. 7 = V
         * bit  8..15 = U
         * bit 16..23 = Y
         * bit 24..31 = NOT USED
         */

        if(colorFormat==DRAW2D_DF_BGR16_565)
            color = Draw2D_rgb565ToYuv444(color);

        /* x, y MUST be multiple of 2 */
        px = Draw2D_floor(px, 2);
        py = Draw2D_floor(py, 2);

        /* Put Y color */
        addr = pObj->bufInfo.bufAddr[0]
            + pObj->bufInfo.bufPitch[0]*py + px;

        *(uint8_t*)addr = ((color & 0xFF0000) >> 16);

        addr = pObj->bufInfo.bufAddr[0]
            + pObj->bufInfo.bufPitch[0]*py + (px+1);

        *(uint8_t*)addr = ((color & 0xFF0000) >> 16);

        addr = pObj->bufInfo.bufAddr[0]
            + pObj->bufInfo.bufPitch[0]*(py+1) + px;

        *(uint8_t*)addr = ((color & 0xFF0000) >> 16);

        addr = pObj->bufInfo.bufAddr[0]
            + pObj->bufInfo.bufPitch[0]*(py+1) + (px+1);

        *(uint8_t*)addr = ((color & 0xFF0000) >> 16);

        /* Put CbCr color */
        addr = pObj->bufInfo.bufAddr[1]
            + pObj->bufInfo.bufPitch[1]*py/2 + px;

        *(uint16_t*)addr = ((color & 0xFF00) >> 8) | ((color & 0xFF) << 8);
    }
}


#define SIGN(x) ((x<0)?-1:((x>0)?1:0)) /* macro to return the sign of a
                                         number */

#define ABS(x)  (x<0?-x:x) /* macro to find abs of a value */

int32_t Draw2D_drawLine(Draw2D_Handle pCtx,
                        uint32_t x1,
                        uint32_t y1,
                        uint32_t x2,
                        uint32_t y2,
                        Draw2D_LinePrm *pPrm)
{
    int32_t status = VX_SUCCESS;
    Draw2D_Obj *pObj = (Draw2D_Obj *)pCtx;
    int32_t i,dx,dy,sdx,sdy,dxabs,dyabs,x,y,px,py;
    int32_t lineSize, k;
    uint32_t lineColor, lineColorFormat;

    if(pPrm==NULL)
    {
        lineSize  = 1;
        lineColor = 0xFFFFFFFF;
        lineColorFormat = DRAW2D_DF_BGR16_565;
    }
    else
    {
        lineSize  = pPrm->lineSize;
        lineColor = pPrm->lineColor;
        lineColorFormat = pPrm->lineColorFormat;
    }

    if(x1 > pObj->bufInfo.bufWidth)
        x1 = pObj->bufInfo.bufWidth;

    if(x2 > pObj->bufInfo.bufWidth)
        x2 = pObj->bufInfo.bufWidth;

    if(y1 >= pObj->bufInfo.bufHeight)
        y1 = pObj->bufInfo.bufHeight;

    if(y2 >= pObj->bufInfo.bufHeight)
        y2 = pObj->bufInfo.bufHeight;

    dx=x2-x1;      /* the horizontal distance of the line */
    dy=y2-y1;      /* the vertical distance of the line */
    dxabs=ABS(dx);
    dyabs=ABS(dy);
    sdx=SIGN(dx);
    sdy=SIGN(dy);
    x=dyabs>>1;
    y=dxabs>>1;
    px=x1;
    py=y1;

    if (dxabs>=dyabs) /* the line is more horizontal than vertical */
    {
        for(k=(-lineSize/2); k<(lineSize/2); k++)
        {
            Draw2D_drawPixel(pObj, px, py+k, lineColor, lineColorFormat);
        }
        for(i=0;i<dxabs;i++)
        {
            y+=dyabs;
            if (y>=dxabs)
            {
                y-=dxabs;
                py+=sdy;
            }
            px+=sdx;
            for(k=(-lineSize/2); k<(lineSize/2); k++)
            {
                Draw2D_drawPixel(pObj, px, py+k, lineColor, lineColorFormat);
            }
        }
    }
    else /* the line is more vertical than horizontal */
    {
        for(k=(-lineSize/2); k<(lineSize/2); k++)
        {
            Draw2D_drawPixel(pObj, px+k, py, lineColor, lineColorFormat);
        }
        for(i=0;i<dyabs;i++)
        {
            x+=dxabs;
            if (x>=dyabs)
            {
                x-=dyabs;
                px+=sdx;
            }
            py+=sdy;
            for(k=(-lineSize/2); k<(lineSize/2); k++)
            {
                Draw2D_drawPixel(pObj, px+k, py, lineColor, lineColorFormat);
            }
        }
    }

    return status;
}

int32_t Draw2D_drawHorizontalLine(Draw2D_Handle pCtx,
                                    uint32_t startX,
                                    uint32_t startY,
                                    uint32_t width,
                                    Draw2D_LinePrm * pPrm)
{
    Draw2D_Obj *pObj = (Draw2D_Obj *)pCtx;
    uint32_t lineSize, lineColor, lineColorFormat;
    Draw2D_RegionPrm regionPrm;

    if(pObj==NULL)
        return VX_FAILURE;

    if(pPrm==NULL)
    {
        lineSize  = 2;
        lineColor = 0xFFFFFFFF;
        lineColorFormat = DRAW2D_DF_BGR16_565;
    }
    else
    {
        lineSize  = pPrm->lineSize;
        lineColor = pPrm->lineColor;
        lineColorFormat = pPrm->lineColorFormat;
    }

    regionPrm.startX = startX;
    regionPrm.startY = startY;
    regionPrm.width  = width;
    regionPrm.height = lineSize;
    regionPrm.color  = lineColor;
    regionPrm.colorFormat = lineColorFormat;

    return Draw2D_fillRegion(pCtx, &regionPrm);
}

int32_t Draw2D_drawVerticalLine(Draw2D_Handle pCtx,
                                    uint32_t startX,
                                    uint32_t startY,
                                    uint32_t height,
                                    Draw2D_LinePrm * pPrm)
{
    Draw2D_Obj *pObj = (Draw2D_Obj *)pCtx;
    uint32_t lineSize, lineColor, lineColorFormat;;
    Draw2D_RegionPrm regionPrm;

    if(pObj==NULL)
        return VX_FAILURE;

    if(pPrm==NULL)
    {
        lineSize  = 2;
        lineColor = 0xFFFFFFFF;
        lineColorFormat = DRAW2D_DF_BGR16_565;
    }
    else
    {
        lineSize  = pPrm->lineSize;
        lineColor = pPrm->lineColor;
        lineColorFormat = pPrm->lineColorFormat;
    }

    regionPrm.startX = startX;
    regionPrm.startY = startY;
    regionPrm.width  = lineSize;
    regionPrm.height = height;
    regionPrm.color  = lineColor;
    regionPrm.colorFormat = lineColorFormat;

    return Draw2D_fillRegion(pCtx, &regionPrm);
}

int32_t Draw2D_drawRect(Draw2D_Handle pCtx,
                        uint32_t startX,
                        uint32_t startY,
                        uint32_t width,
                        uint32_t height,
                        Draw2D_LinePrm * pPrm)
{
    int32_t status = VX_SUCCESS;

    Draw2D_drawHorizontalLine(pCtx,startX,startY,width,pPrm);
    Draw2D_drawHorizontalLine(pCtx,startX,startY + height,width+pPrm->lineSize,pPrm);
    Draw2D_drawVerticalLine(pCtx,startX,startY,height,pPrm);
    Draw2D_drawVerticalLine(pCtx,startX + width,startY,height,pPrm);

    return status;
}

#ifndef EXCLUDE_BMP_LOAD
int32_t Draw2D_insertBmp(Draw2D_Handle pCtx,
                         char *input_file,
                         int32_t startX,
                         int32_t startY)
{
   vx_status status = VX_SUCCESS;

   tivx_utils_bmp_image_params_t   imgParams;
   void *dataPtr = NULL;
   uint8_t *pIn;
   uint32_t imgWidth;
   uint32_t imgHeight;
   int32_t i, j;

   Draw2D_Obj *pObj = (Draw2D_Obj *)pCtx;

   if(pObj==NULL)
       return VX_FAILURE;

   status = tivx_utils_bmp_file_read(
               input_file,
               vx_false_e,
               &imgParams);

   if(status != VX_SUCCESS)
       return status;

   imgWidth  = imgParams.width;
   imgHeight = imgParams.height;
   dataPtr   = imgParams.data;

   if(((startX + imgWidth) > pObj->bufInfo.bufWidth) ||
      ((startY + imgHeight) > pObj->bufInfo.bufHeight))
      {
        return VX_FAILURE;
      }

   pIn = (uint8_t *)dataPtr;

   for(i = 0; i < imgHeight; i++)
   {
     uint16_t *pOut = (uint16_t *)(pObj->bufInfo.bufAddr[0] + ((i + startY) * pObj->bufInfo.bufPitch[0])) + startX;

     for(j = 0; j < imgWidth; j++)
     {
       uint16_t val_565;
       uint8_t R, G, B;

       B = pIn[0];
       G = pIn[1];
       R = pIn[2];

       val_565 = RGB888_TO_RGB565(R, G, B);

       pOut[j] = val_565;
       pIn += 3;
     }
   }

   /* Release the bmp buffer created in readInput() */
   tivx_utils_bmp_read_release(&imgParams);

   return VX_SUCCESS;
 }

 int32_t Draw2D_insertBmpFromMemory(Draw2D_Handle pCtx,
                         void *buf,
                         uint32_t buf_size,
                         int32_t startX,
                         int32_t startY)
{
   vx_status status = VX_SUCCESS;

   tivx_utils_bmp_image_params_t   imgParams;
   void *dataPtr = NULL;
   uint8_t *pIn;
   uint32_t imgWidth;
   uint32_t imgHeight;
   int32_t i, j;

   Draw2D_Obj *pObj = (Draw2D_Obj *)pCtx;

   if(pObj==NULL)
       return VX_FAILURE;

   status = tivx_utils_bmp_file_read_from_memory(
               buf,
               buf_size,
               vx_false_e,
               &imgParams);

   if(status != VX_SUCCESS)
       return status;

   imgWidth  = imgParams.width;
   imgHeight = imgParams.height;
   dataPtr   = imgParams.data;

   if(((startX + imgWidth) > pObj->bufInfo.bufWidth) ||
      ((startY + imgHeight) > pObj->bufInfo.bufHeight))
      {
        return VX_FAILURE;
      }

   pIn = (uint8_t *)dataPtr;

   for(i = 0; i < imgHeight; i++)
   {
     uint16_t *pOut = (uint16_t *)(pObj->bufInfo.bufAddr[0] + ((i + startY) * pObj->bufInfo.bufPitch[0])) + startX;

     for(j = 0; j < imgWidth; j++)
     {
       uint16_t val_565;
       uint8_t R, G, B;

       B = pIn[0];
       G = pIn[1];
       R = pIn[2];

       val_565 = RGB888_TO_RGB565(R, G, B);

       pOut[j] = val_565;
       pIn += 3;
     }
   }

   /* Release the bmp buffer created in readInput() */
   tivx_utils_bmp_read_release(&imgParams);

   return VX_SUCCESS;
 }
#endif /* EXCLUDE_BMP_LOAD */

/*
 Formula used for RGB to YUV

 Y = 0.299R + 0.587G + 0.114B
 U = 0.492 (B-Y) + 128
 V = 0.877 (R-Y) + 128
*/
#define RGB2YUV_MUL_YR  0x132
#define RGB2YUV_MUL_YG  0x259
#define RGB2YUV_MUL_YB  0x074
#define RGB2YUV_MUL_U   0x1F7
#define RGB2YUV_MUL_V   0x382

#define RGB2YUV_OFFSET_UV 128
#define RGB2YUV_QSHIFT     10

uint32_t Draw2D_rgb565ToYuv444(uint16_t rgb565)
{
  int32_t r, g, b;
  int32_t y, u, v;

  r  = (rgb565 & 0xF800) >> (11-3);
  g  = (rgb565 & 0x07E0) >> (5-2);
  b  = (rgb565 & 0x001F) << (3);

  y = ( ( r*RGB2YUV_MUL_YR + g*RGB2YUV_MUL_YG + b*RGB2YUV_MUL_YB) >> (RGB2YUV_QSHIFT) );
  u = (( ( RGB2YUV_MUL_U * (b-y)) >> (RGB2YUV_QSHIFT) ) + RGB2YUV_OFFSET_UV);
  v = (( ( RGB2YUV_MUL_V * (r-y)) >> (RGB2YUV_QSHIFT) ) + RGB2YUV_OFFSET_UV);

  if(v>255)
    v = 255;
  if(v<0)
    v = 0;

  return ( (uint32_t)y  << 16 ) +
         ( (uint32_t)u  <<  8 ) +
         ( (uint32_t)v  <<  0 ) ;

}
