/*
 *
 * Copyright (c) 2020 Texas Instruments Incorporated
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
#ifndef _APP_CAPTURE_MODULE
#define _APP_CAPTURE_MODULE

/**
 * \defgroup group_vision_apps_modules_capture Capture Node Module
 *
 * \brief This section contains module APIs for the TIOVX Capture node tivxHwaCapture
 *
 * \ingroup group_vision_apps_modules
 *
 * @{
 */

#include "app_modules.h"
#include "app_sensor_module.h"
#include <TI/video_io_capture.h>

/** \brief Number of capture channels that are used by this module.  Can be modified as needed
 *
 */
#define NUM_CAPT_CHANNELS   (4u)

/** \brief Capture Module Data Structure
 *
 * Contains the data objects required to use the tivxHwaCapture node
 *
 */
typedef struct {
    /*! Capture node object */
    vx_node node;

    /*! User data object for config parameter, used as node parameter of Capture node */
    vx_user_data_object config;

    /*! Capture node params structure to initialize config object */
    tivx_capture_params_t params;

    /*! Capture node object array output */
    vx_object_array raw_image_arr[APP_MODULES_MAX_BUFQ_DEPTH];

    /*! Capture node graph parameter index */
    vx_int32 graph_parameter_index;

    /*! Flag to indicate whether or not the intermediate output is written */
    vx_int32 en_out_capture_write;

    /*! Flag to enable test mode */
    vx_int32 test_mode;

    /*! Node used to write capture output */
    vx_node write_node;

    /*! File path used to write capture node output */
    vx_array file_path;

    /*! File path prefix used to write capture node output */
    vx_array file_prefix;

    /*! User data object containing write cmd parameters */
    vx_user_data_object write_cmd;

    /*! Output file path for capture node output */
    vx_char output_file_path[TIVX_FILEIO_FILE_PATH_LENGTH];

    /*! Name of capture module */
    vx_char objName[APP_MODULES_MAX_OBJ_NAME_SIZE];

    /*! Raw image used when camera gets disconnected */
    tivx_raw_image error_frame_raw_image;

    /*! Flag to enable detection of camera disconnection */
    vx_uint8 enable_error_detection;

    /*! Capture node format, taken from sensor_out_format */
    vx_uint32 capture_format;
}CaptureObj;

/** \brief Capture module init helper function
 *
 * This capture init helper function will create all the data objects required to create the capture
 * node
 *
 * \param [in]  context    OpenVX context which must be created using \ref vxCreateContext
 * \param [out] captureObj Capture Module object which gets populated with capture node data objects
 * \param [in]  sensorObj  Sensor Module object used to initialize capture data object parameters;
 *                         must be initialized prior to passing to this function
 * \param [in]  objName    String of the name of this object
 * \param [in]  bufq_depth Capture output buffer queue depth
 *
 */
vx_status app_init_capture(vx_context context, CaptureObj *captureObj, SensorObj *sensorObj, char *objName, int32_t bufq_depth);


/** \brief Capture module deinit helper function
 *
 * This capture deinit helper function will release all the data objects created during the \ref app_init_capture call
 *
 * \param [in,out] captureObj    Capture Module object which contains capture node data objects which are released in this function
 * \param [in]     bufq_depth    Capture output buffer queue depth
 *
 */
void app_deinit_capture(CaptureObj *captureObj, vx_int32 bufq_depth);


/** \brief Capture module delete helper function
 *
 * This capture delete helper function will delete the capture node and write node that is created during the \ref app_create_graph_capture call
 *
 * \param [in,out] captureObj   Capture Module object which contains capture node objects which are released in this function
 *
 */
void app_delete_capture(CaptureObj *captureObj);


/** \brief Capture module create helper function
 *
 * This capture create helper function will create the node using all the data objects created during the \ref app_init_capture call.
 * Internally calls \ref app_create_graph_capture_write_output if en_out_capture_write is set
 *
 * \param [in]     graph       OpenVX graph that has been created using \ref vxCreateGraph and where the capture node is created
 * \param [in,out] captureObj  Capture Module object which contains capture node and write node which are created in this function
 *
 */
vx_status app_create_graph_capture(vx_graph graph, CaptureObj *captureObj);

/** \brief Capture module write output helper function
 *
 * This capture create helper function will create the node for writing the capture output
 *
 * \param [in]     graph       OpenVX graph
 * \param [in,out] captureObj  Capture Module object which contains capture node and write node which are created in this function
 *
 */
vx_status app_create_graph_capture_write_output(vx_graph graph, CaptureObj *captureObj);

/** \brief Capture module write output helper function
 *
 * This capture create helper function will create the node for writing the capture output
 *
 * \param [in] captureObj    Capture Module object which contains the write node used in this function
 * \param [in] start_frame   Starting frame to write
 * \param [in] num_frames    Total number of frames to write
 * \param [in] num_skip      Number of capture frames to skip writing
 *
 */
vx_status app_send_cmd_capture_write_node(CaptureObj *captureObj, vx_uint32 start_frame, vx_uint32 num_frames, vx_uint32 num_skip);


/** \brief Capture module write output helper function
 *
 * This capture helper function send the blank frame to the capture node over a control command.  This frame is
 * used as the output for when a camera is disconnected
 *
 * \param [in] captureObj   Capture Module object which contains the error frame object for using as blank frame
 *
 */
vx_status app_send_error_frame(CaptureObj *captureObj);

/** \brief Capture module write output helper function
 *
 * This capture helper function takes a raw_image that is unpopulated and populates it with the provided path
 *
 * \param [in]  context       OpenVX context which must be created using \ref vxCreateContext
 * \param [in]  sensorInfo    Capture sensor information
 * \param [in]  file_path     Path to image
 * \param [out] bytes_read    Number of bytes read from file
 *
 */
tivx_raw_image read_error_image_raw(vx_context context, IssSensor_Info *sensorInfo, char file_path[], vx_int32 *bytes_read);

/* @} */

#endif
