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

#define COLOR_00    (0)    /* background color */
#define COLOR_01    (1)    /* border color     */
#define COLOR_02    (2)    /* font color       */

uint16_t gDraw2D_font_color_bg = DRAW2D_TRANSPARENT_COLOR;
uint16_t gDraw2D_font_color_border = ((uint16_t)(RGB888_TO_RGB565(16,16,16)))    /* border color     */;
uint16_t gDraw2D_font_color_text = (0xE71C)    /* font color       */;


int32_t Draw2D_getFontProperty(Draw2D_FontPrm *pPrm, Draw2D_FontProperty *pProp)
{

    if(pProp==NULL)
        return VX_FAILURE;

    /* default */
    Draw2D_getFontProperty00(pProp); /* default */

    if(pPrm!=NULL)
    {
        if(pPrm->fontIdx==0)
            Draw2D_getFontProperty00(pProp);
        else
        if(pPrm->fontIdx==1)
            Draw2D_getFontProperty01(pProp);
        else
        if(pPrm->fontIdx==2)
            Draw2D_getFontProperty02(pProp);
        else
        if(pPrm->fontIdx==3)
            Draw2D_getFontProperty03(pProp);
        else
        if(pPrm->fontIdx==10)
            Draw2D_getFontProperty10(pProp);
        else
        if(pPrm->fontIdx==11)
            Draw2D_getFontProperty11(pProp);
        else
        if(pPrm->fontIdx==12)
            Draw2D_getFontProperty12(pProp);
        else
        if(pPrm->fontIdx==13)
            Draw2D_getFontProperty13(pProp);
    }

    return VX_SUCCESS;
}

int32_t Draw2D_getBmpProperty(Draw2D_BmpPrm *pPrm, Draw2D_BmpProperty *pProp)
{
    if(pProp==NULL)
        return VX_FAILURE;

    /* default */
    Draw2D_getBmpProperty00(pProp); /* default */

    if(pPrm!=NULL)
    {
        if(pPrm->bmpIdx==DRAW2D_BMP_IDX_TI_LOGO_0)
            Draw2D_getBmpProperty00(pProp);
        else
        if(pPrm->bmpIdx==DRAW2D_BMP_IDX_TI_LOGO_1)
            Draw2D_getBmpProperty01(pProp);
        else
        if(pPrm->bmpIdx==DRAW2D_BMP_IDX_TI_LOGO_2)
            Draw2D_getBmpProperty02(pProp);
        else
        if(pPrm->bmpIdx==DRAW2D_BMP_IDX_TI_LOGO_3)
            Draw2D_getBmpProperty03(pProp);
        else
        if(pPrm->bmpIdx==DRAW2D_BMP_IDX_TI_LOGO_4)
            Draw2D_getBmpProperty04(pProp);
        else
        if(pPrm->bmpIdx==DRAW2D_BMP_IDX_TI_LOGO_5)
            Draw2D_getBmpProperty05(pProp);
        else
        if(pPrm->bmpIdx==DRAW2D_BMP_IDX_DOF_COLOUR_MAP)
            Draw2D_getBmpProperty_DofColourMap(pProp);
        else
        if(pPrm->bmpIdx==DRAW2D_BMP_IDX_SDE_COLOUR_MAP)
            Draw2D_getBmpProperty_SdeColourMap(pProp);
    }

    return VX_SUCCESS;
}

uint8_t * Draw2D_getFontCharAddr(Draw2D_FontProperty *font, char c)
{
    if(font==NULL)
        return 0;

    if(c<' ' || c>'~')
        c = ' '; /* if out of bound draw 'space' char */

    c = c - ' ';

    return ((uint8_t *)font->addr + (c * font->width * font->bpp));
}

uint16_t Draw2D_getFontColor(uint16_t key)
{
    uint16_t color = gDraw2D_font_color_bg;

    if(key==COLOR_01)
        color = gDraw2D_font_color_border;
    else
    if(key==COLOR_02)
        color = gDraw2D_font_color_text;

    return color;
}

static int32_t Draw2D_initializeFontProperty(Draw2D_FontProperty *pProp)
{
    if(pProp==NULL)
        return VX_FAILURE;

    pProp->addr        = NULL;
    pProp->width       = 0;
    pProp->height      = 0;
    pProp->bpp         = 0;
    pProp->lineOffset  = 0;
    pProp->num         = 0;
    pProp->colorFormat = 0;

    return 0;
}

int32_t Draw2D_drawString_rot(Draw2D_Handle pCtx,
                        uint32_t startX,
                        uint32_t startY,
                        char *str,
                        Draw2D_FontPrm *pPrm,
                        uint32_t rotate)
{
    int32_t status = VX_SUCCESS;
    Draw2D_Obj *pObj = (Draw2D_Obj *)pCtx;
    uint32_t len, width, height, h, i, w, px, py;
    uint8_t *fontAddr;
    uint16_t *fontAddr16, color;
    Draw2D_FontProperty font;

    if(pObj==NULL || str==NULL)
        return VX_FAILURE;

    Draw2D_initializeFontProperty(&font);
    Draw2D_getFontProperty(pPrm, &font);

    len = strlen(str);

    width = font.width*len;
    height = font.height;

    if(startX >= pObj->bufInfo.bufWidth)
        return 0;

    if(startY >= pObj->bufInfo.bufHeight)
        return 0;

    if(0 == rotate)
    {
        if((startX + width)> pObj->bufInfo.bufWidth)
        {
            width = pObj->bufInfo.bufWidth - startX;
        }

        if((startY + height)> pObj->bufInfo.bufHeight)
        {
            height = pObj->bufInfo.bufHeight - startY;
        }
    }
    else if(1 == rotate)
    {
        if((startX + height)> pObj->bufInfo.bufWidth)
        {
            height = pObj->bufInfo.bufWidth - startX;
        }

        if(startY < width/2)
        {
            width = startY*2;
        }
    }
    else if(2 == rotate)
    {
        if(startX < height)
        {
            height = startX;
        }

        if((startY + width/2) > pObj->bufInfo.bufHeight)
        {
            width = 2*(pObj->bufInfo.bufHeight - startY);
        }
    }


    len = width/font.width;

    /* draw 'len' char's from string 'str' */
    if(0 == rotate)
    {
        if(font.colorFormat==DRAW2D_DF_YUV420SP_UV)
        {
            for(i=0; i<len; i++)
            {
                px  = startX + i*font.width;
                py  = startY;

                Draw2D_drawCharYuv420SP(pCtx, px, py, str[i], &font);
            }
        }
        else
        {
            for(i=0; i<len; i++)
            {
                fontAddr = Draw2D_getFontCharAddr(&font, str[i]);
                px  = startX + i*font.width;
                py  = startY;

                /* draw font char */
                for(h=0; h<height; h++)
                {
                    fontAddr16 = (uint16_t*)fontAddr;
                    for(w=0; w<font.width; w++)
                    {
                        /* Assume color format is 2 bytes per pixel */
                        color = Draw2D_getFontColor(*fontAddr16);
                        Draw2D_drawPixel(
                            pCtx,
                            px+w,
                            py+h,
                            color,
                            font.colorFormat
                            );
                        fontAddr16++;
                    }
                    fontAddr += font.lineOffset;
                }
            }
        }
    }
    else if(1 == rotate)
    {
        for(i=0; i<len; i++)
        {
            fontAddr = Draw2D_getFontCharAddr(&font, str[i]);
            px  = startX;
            py  = startY - i*font.width;

            /* draw font char */
            for(h=0; h<height; h++)
            {
                fontAddr16 = (uint16_t*)fontAddr;
                for(w=0; w<font.width; w++)
                {
                    /* Assume color format is 2 bytes per pixel */
                    color = Draw2D_getFontColor(*fontAddr16);
                    Draw2D_drawPixel(
                        pCtx,
                        px+h,
                        py-w,
                        color,
                        font.colorFormat
                        );
                    fontAddr16++;
                }
                fontAddr += font.lineOffset;
            }
        }
    }
    else if(2 == rotate)
    {
        for(i=0; i<len; i++)
        {
            fontAddr = Draw2D_getFontCharAddr(&font, str[i]);
            px  = startX;
            py  = startY + i*font.width;

            /* draw font char */
            for(h=0; h<height; h++)
            {
                fontAddr16 = (uint16_t*)fontAddr;
                for(w=0; w<font.width; w++)
                {
                    /* Assume color format is 2 bytes per pixel */
                    color = Draw2D_getFontColor(*fontAddr16);
                    Draw2D_drawPixel(
                        pCtx,
                        px-h,
                        py+w,
                        color,
                        font.colorFormat
                        );
                    fontAddr16++;
                }
                fontAddr += font.lineOffset;
            }
        }
    }

    return status;
}

int32_t Draw2D_drawString(Draw2D_Handle pCtx,
                        uint32_t startX,
                        uint32_t startY,
                        char *str,
                        Draw2D_FontPrm *pPrm)
{
    return Draw2D_drawString_rot(pCtx,
                          startX,
                          startY,
                          str,
                          pPrm,
                          0);
}

int32_t Draw2D_clearString(Draw2D_Handle pCtx,
                            uint32_t startX,
                            uint32_t startY,
                            uint32_t stringLength,
                            Draw2D_FontPrm *pPrm)
{
    int32_t status = VX_SUCCESS;
    char tmpString[80];
    uint32_t len, clearLen;
    Draw2D_FontProperty font;

    if(pCtx==NULL)
        return VX_FAILURE;

    Draw2D_initializeFontProperty(&font);
    Draw2D_getFontProperty(pPrm, &font);

    len = sizeof(tmpString)-1;
    memset(tmpString, ' ', len);

    while(stringLength)
    {
        if(stringLength<len)
            clearLen = stringLength;
        else
            clearLen = len;

        tmpString[clearLen] = 0;
        Draw2D_drawString(pCtx, startX, startY, tmpString, pPrm);

        startX += clearLen*font.width;

        stringLength -= clearLen;
    }
    return status;
}

int32_t Draw2D_drawBmp(Draw2D_Handle pCtx,
                        uint32_t startX,
                        uint32_t startY,
                        Draw2D_BmpPrm *pPrm)
{
    return Draw2D_drawBmp_rot(pCtx,
                        startX,
                        startY,
                        pPrm,
                        0);
}


static int32_t Draw2D_initializeBmpProperty(Draw2D_BmpProperty *pProp)
{
    if(pProp==NULL)
        return VX_FAILURE;

    pProp->addr        = NULL;
    pProp->width       = 0;
    pProp->height      = 0;
    pProp->bpp         = 0;
    pProp->lineOffset  = 0;
    pProp->colorFormat = 0;

    return 0;
}

int32_t Draw2D_drawBmp_rot(Draw2D_Handle pCtx,
                        uint32_t startX,
                        uint32_t startY,
                        Draw2D_BmpPrm *pPrm,
                        uint32_t rotate)
{
    int32_t status = VX_SUCCESS;
    Draw2D_Obj *pObj = (Draw2D_Obj *)pCtx;
    uint32_t width, height, h, w;
    uint8_t *bmpAddr;
    uint16_t color, *bmpAddr16;
    Draw2D_BmpProperty bmp;

    if(pObj==NULL)
        return VX_FAILURE;

    Draw2D_initializeBmpProperty(&bmp);
    Draw2D_getBmpProperty(pPrm, &bmp);

    width = bmp.width;
    height = bmp.height;

    if (startX >= pObj->bufInfo.bufWidth)
        return 0;

    if (startY >= pObj->bufInfo.bufHeight)
        return 0;

    if(0 == rotate)
    {
        if((startX + width)> pObj->bufInfo.bufWidth)
        {
            width = pObj->bufInfo.bufWidth - startX;
        }

        if((startY + height)> pObj->bufInfo.bufHeight)
        {
            height = pObj->bufInfo.bufHeight - startY;
        }
    }
    else if(1 == rotate)
    {
        if((startX + height)> pObj->bufInfo.bufWidth)
        {
            height = pObj->bufInfo.bufWidth - startX;
        }

        if(startY < width/2)
        {
            width = startY*2;
        }
    }
    else if(2 == rotate)
    {
        if(startX < height)
        {
            height = startX;
        }

        if((startY + width/2) > pObj->bufInfo.bufHeight)
        {
            width = 2*(pObj->bufInfo.bufHeight - startY);
        }
    }

    /* draw bitmap */
    bmpAddr = bmp.addr;

    if(0 == rotate)
    {
        /* draw bmp */
        for(h=0; h<height; h++)
        {
            bmpAddr16 = (uint16_t*)bmpAddr;
            for(w=0; w<bmp.width; w++)
            {
                /* Assume color format is 2 bytes per pixel */
                color = *bmpAddr16;
                Draw2D_drawPixel(
                    pCtx,
                    startX+w,
                    startY+h,
                    color,
                    bmp.colorFormat
                    );
                bmpAddr16++;
            }
            bmpAddr += bmp.lineOffset;
        }
    }
    else if(1 == rotate)
    {
        /* draw bmp */
        for(h=0; h<height; h++)
        {
            bmpAddr16 = (uint16_t*)bmpAddr;
            for(w=0; w<bmp.width; w++)
            {
                /* Assume color format is 2 bytes per pixel */
                color = *bmpAddr16;
                Draw2D_drawPixel(
                    pCtx,
                    startX+h,
                    startY-w,
                    color,
                    bmp.colorFormat
                    );
                bmpAddr16++;
            }
            bmpAddr += bmp.lineOffset;
        }
    }
    else if(2 == rotate)
    {
        /* draw bmp */
        for(h=0; h<height; h++)
        {
            bmpAddr16 = (uint16_t*)bmpAddr;
            for(w=0; w<bmp.width; w++)
            {
                /* Assume color format is 2 bytes per pixel */
                color = *bmpAddr16;
                Draw2D_drawPixel(
                    pCtx,
                    startX-h,
                    startY+w,
                    color,
                    bmp.colorFormat
                    );
                bmpAddr16++;
            }
            bmpAddr += bmp.lineOffset;
        }
    }

    return status;
}

void Draw2D_setFontColor(uint16_t colorText, uint16_t colorBorder, uint16_t colorBg )
{
    gDraw2D_font_color_text = colorText;
    gDraw2D_font_color_border = colorBorder;
    gDraw2D_font_color_bg = colorBg;
}

void Draw2D_resetFontColor()
{
    Draw2D_setFontColor(
        (0xE71C),    /* font color       */
        ((uint16_t)(RGB888_TO_RGB565(16,16,16))),    /* border color     */
        DRAW2D_TRANSPARENT_COLOR
            );
}

