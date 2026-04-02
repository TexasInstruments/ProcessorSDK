/*
 *
 * Copyright (c) 2017 Texas Instruments Incorporated
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
#ifndef _APP_DISPLAY_MODULE
#define _APP_DISPLAY_MODULE

/**
 * \defgroup group_vision_apps_modules_display Display Node Module
 *
 * \brief This section contains module APIs for the TIOVX Display Node tivxHwaDisplay
 *
 * \ingroup group_vision_apps_modules
 *
 * @{
 */

#include "app_modules.h"
#include <TI/video_io_display.h>

/** \brief Display width value, can be modified as needed
 *
 */
#define DISPLAY_WIDTH  (1920)

/** \brief Display height value, can be modified as needed
 *
 */
#define DISPLAY_HEIGHT (1080)

/** \brief Display Module Data Structure
 *
 * Contains the data objects required to use the tivxHwaDisplay node
 *
 */
typedef struct {
    /*! Display node object */
    vx_node  disp_node;

    /*! Display node user data object for configuration of display node */
    vx_user_data_object disp_params_obj;

    /*! Display node params structure to initialize disp_params_obj object */
    tivx_display_params_t disp_params;

    /*! Display option value; must be set to 1 to use the display */
    vx_int32 display_option;

    /*! Display node graph parameter index */
    vx_int32 graph_parameter_index;

    /*! Name of display module */
    vx_char objName[APP_MODULES_MAX_OBJ_NAME_SIZE];

} DisplayObj;

/** \brief Display module init helper function
 *
 * This display init helper function will create all the data objects required to create the display
 * node
 *
 * \param [in]  context    OpenVX context which must be created using \ref vxCreateContext
 * \param [out] displayObj Display Module object which gets populated with display node data objects
 * \param [in]  objName    String of the name of this object
 *
 */
vx_status app_init_display(vx_context context, DisplayObj *displayObj, char *objName);

/** \brief Display module deinit helper function
 *
 * This display deinit helper function will release all the data objects created during the \ref app_init_display call
 *
 * \param [in,out] displayObj    Display Module object which contains display node data objects which are released in this function
 *
 */
void app_deinit_display(DisplayObj *displayObj);

/** \brief Display module delete helper function
 *
 * This display delete helper function will delete the display node that is created during the \ref app_create_graph_display call
 *
 * \param [in,out] displayObj   Display Module object which contains display node object which is released in this function
 *
 */
void app_delete_display(DisplayObj *displayObj);

/** \brief Display module create helper function
 *
 * This display create helper function will create the node using all the data objects created during the \ref app_init_display call.
 *
 * \param [in]     graph       OpenVX graph that has been created using \ref vxCreateGraph and where the display node is created
 * \param [in,out] displayObj  Display Module object which contains display node which is created in this function
 * \param [in]     disp_image  Image which will be displayed
 *
 */
vx_status app_create_graph_display(vx_graph graph, DisplayObj *displayObj, vx_image disp_image);

/* @} */

#endif
