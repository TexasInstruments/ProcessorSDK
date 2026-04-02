/*
 *
 * Copyright (c) 2018 Texas Instruments Incorporated
 *
 * All rights reserved not granted herein.
 *
 * Limited License.
 *
 * Texas Instruments Incorporated grants a world-wide, royalty-free, non-exclusive
 * license under copyrights and patents it now or hereafter owns or controls to make,
 * have made, use, import, offer to sell and sell ("Utilize") this software subject to the
 * terms herein.  With respect to the foregoing patent license, such license is granted
 * solely to the extent that any such patent is necessary to Utilize the software alone.
 * The patent license shall not apply to any combinations which include this software,
 * other than combinations with devices manufactured by or for TI ("TI Devices").
 * No hardware patent is licensed hereunder.
 *
 * Redistributions must preserve existing copyright notices and reproduce this license
 * (including the above copyright notice and the disclaimer and (if applicable) source
 * code license limitations below) in the documentation and/or other materials provided
 * with the distribution
 *
 * Redistribution and use in binary form, without modification, are permitted provided
 * that the following conditions are met:
 *
 * *       No reverse engineering, decompilation, or disassembly of this software is
 * permitted with respect to any software provided in binary form.
 *
 * *       any redistribution and use are licensed by TI for use only with TI Devices.
 *
 * *       Nothing shall obligate TI to provide you with source code for the software
 * licensed and provided to you in object code.
 *
 * If software source code is provided to you, modification and redistribution of the
 * source code are permitted provided that the following conditions are met:
 *
 * *       any redistribution and use of the source code, including any resulting derivative
 * works, are licensed by TI for use only with TI Devices.
 *
 * *       any redistribution and use of any object code compiled from the source code
 * and any resulting derivative works, are licensed by TI for use only with TI Devices.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of its suppliers
 *
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * DISCLAIMER.
 *
 * THIS SOFTWARE IS PROVIDED BY TI AND TI'S LICENSORS "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL TI AND TI'S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "app_grpx_priv.h"

int32_t appGrpxDrawDdrLoad(app_grpx_obj_t *obj,
                    uint32_t x, uint32_t y,
                    uint32_t barHeight
                    )
{
    Draw2D_FontPrm fontPrm;
    Draw2D_FontPrm fontPrm2;
    char string1[APP_GRPX_STRING_MAX];
    char string2[APP_GRPX_STRING_MAX];
    Draw2D_FontProperty fontProp;
    Draw2D_FontProperty fontProp2;
    uint32_t startX, startY1, startY2;
    uint32_t barWidth, padY;
    uint32_t rd_load, wr_load, total_load;

    fontPrm.fontIdx = 2;
    fontPrm2.fontIdx = 3;

    Draw2D_getFontProperty(&fontPrm, &fontProp);
    Draw2D_getFontProperty(&fontPrm2, &fontProp2);

    barWidth = fontProp.width*3;
    padY = APP_GRPX_LOAD_PAD_Y;

    /* name start X, Y */
    startY1 =  y + fontProp.height + padY + barHeight + padY;
    startY2 =  startY1 + fontProp.height + padY/2;
    startX =  x;

    snprintf(string1, APP_GRPX_STRING_MAX,
              "%s",
              "DDR BW"
              );
    snprintf(string2, APP_GRPX_STRING_MAX,
              "%4d MB/s",
              obj->read_bw_avg+obj->write_bw_avg
              );

    /* draw name - line 1 */
    Draw2D_drawString(obj->draw2d_obj,
              startX,
              startY1,
              string1,
              &fontPrm
              );

    Draw2D_setFontColor(
        RGB888_TO_RGB565(0, 255, 255),
        RGB888_TO_RGB565(16, 16, 16),
        DRAW2D_TRANSPARENT_COLOR
        );

    /* draw performance - line 2 */
    Draw2D_drawString(obj->draw2d_obj,
              startX,
              startY2,
              string2,
              &fontPrm2
              );

    Draw2D_resetFontColor();

    rd_load = (obj->read_bw_avg*100)/obj->total_available_bw;
    wr_load = (obj->write_bw_avg*100)/obj->total_available_bw;
    total_load = ((obj->read_bw_avg+obj->write_bw_avg)*100)/obj->total_available_bw;

    /* limit to 100 */
    if(rd_load>100)
        rd_load=100;
    if(wr_load>100)
        wr_load=100;
    if(total_load>100)
        total_load=100;

    appGrpxDrawLoadBar2
        (
            obj,
            wr_load,
            rd_load,
            startX,
            y + fontProp.height + padY,
            barWidth,
            barHeight,
            "WR",
            "RD"
        );

    /* draw CPU load as text */
    snprintf(string1, APP_GRPX_STRING_MAX,
                  "%2d%%",
                  total_load
                  );
    Draw2D_drawString(obj->draw2d_obj,
              startX,
              y,
              string1,
              &fontPrm
              );

    return 0;
}

void appGrpxGetDdrLoad(app_grpx_obj_t *obj)
{
    int32_t status=0;
    app_perf_stats_ddr_stats_t ddr_stats;

    obj->read_bw_avg = 0;
    obj->write_bw_avg = 0;

    status = appPerfStatsDdrStatsGet(&ddr_stats);
    if(status==0)
    {
        obj->read_bw_avg = ddr_stats.read_bw_avg;
        obj->write_bw_avg = ddr_stats.write_bw_avg;
        obj->total_available_bw = ddr_stats.total_available_bw;
    }
}

int32_t appGrpxGetDimDdrLoad(uint16_t *width, uint16_t *height)
{
    int32_t status = 0;

    Draw2D_FontPrm font;
    Draw2D_FontProperty prop;

    Draw2D_FontPrm font2;
    Draw2D_FontProperty prop2;

    font.fontIdx = 2;
    font2.fontIdx = 3;

    *width = 0;
    *height = 0;

    status  = Draw2D_getFontProperty(&font, &prop);
    status |= Draw2D_getFontProperty(&font2, &prop2);
    if(status==0)
    {
        *height = APP_GRPX_LOAD_BAR_HEIGHT + prop.height*2 + prop2.height + APP_GRPX_LOAD_PAD_Y*2 + APP_GRPX_LOAD_PAD_Y/2;
        *width  = (prop2.width*10 + prop.width/2);
    }
    return status;
}

int32_t appGrpxShowDdrLoad(uint16_t startx, uint16_t starty)
{
    int32_t status = 0;
    app_grpx_obj_t *obj = (app_grpx_obj_t*)&g_app_grpx_obj;

    appGrpxGetDdrLoad(obj);
    appGrpxDrawDdrLoad(obj, startx, starty,
        APP_GRPX_LOAD_BAR_HEIGHT /* bar height */
        );
    return status;
}
