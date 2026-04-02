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
#ifndef _APP_OBJ_ARR_SPLIT_MODULE
#define _APP_OBJ_ARR_SPLIT_MODULE

/**
 * \defgroup group_vision_apps_modules_obj_arr_split Object Array Split Node Module
 *
 * \brief This section contains module APIs for the Node tivxObjArraySplitNode
 *
 * \ingroup group_vision_apps_modules
 *
 * @{
 */

#include "app_modules.h"

/** \brief Obj Array Split Module Data Structure
 *
 * Contains the data objects required to use tivxObjArraySplitNode
 *
 */
typedef struct {
  /*! Obj Arr Split node object */
  vx_node  node;

  /*! Input object array to obj arr split node */
  vx_object_array input_arr;

  /*! Output0 object array to obj arr split node */
  vx_object_array output0_arr;

  /*! Output1 object array to obj arr split node */
  vx_object_array output1_arr;

  /*! Output2 object array to obj arr split node (optional) */
  vx_object_array output2_arr;

  /*! Output3 object array to obj arr split node (optional) */
  vx_object_array output3_arr;

  /*! Number of object array outputs used by module */
  uint32_t num_outputs;

  /*! Number of object array elements split into output0 */
  uint32_t output0_num_elements;

  /*! Number of object array elements split into output1 */
  uint32_t output1_num_elements;

  /*! Number of object array elements split into output2 */
  uint32_t output2_num_elements;

  /*! Number of object array elements split into output3 */
  uint32_t output3_num_elements;

} ObjArrSplitObj;

/** \brief Obj Arr Split module init helper function
 *
 * This obj arr split init helper function will create all the data objects required to create the obj arr split
 * node
 *
 * \param [in]  context        OpenVX context which must be created using \ref vxCreateContext
 * \param [out] ObjArrSplitObj Obj Arr Split module object which gets populated with obj arr split node data objects
 * \param [in]  objName        String of the name of this object
 * \param [in]  numOutputs     Number of outputs that the input should be split into.  The minimum is 2 and maximum is 4
 *
 */
vx_status app_init_obj_arr_split(vx_context context, ObjArrSplitObj *objArrSplitObj, char *objName);

/** \brief Obj Arr Split module deinit helper function
 *
 * This obj arr split deinit helper function will release all the data objects created during the \ref app_init_obj_arr_split call
 *
 * \param [in,out] ObjArrSplitObj  Obj Arr Split module object which contains obj arr split node data objects which are released in this function
 *
 */
void app_deinit_obj_arr_split(ObjArrSplitObj *objArrSplitObj);

/** \brief Obj Arr Split module delete helper function
 *
 * This obj arr split delete helper function will delete the obj arr split node that is created during the \ref app_create_graph_obj_arr_split call
 *
 * \param [in,out] objArrSplitObj   Obj Arr Split module object which contains obj arr split node objects which are released in this function
 *
 */
void app_delete_obj_arr_split(ObjArrSplitObj *objArrSplitObj);

/** \brief Obj Arr Split module create helper function
 *
 * This obj arr split create helper function will create the node using all the data objects created during the \ref app_init_obj_arr_split call.
 *
 * \param [in]     graph           OpenVX graph that has been created using \ref vxCreateGraph and where the obj arr split node is created
 * \param [in,out] ObjArrSplitObj  Obj Arr Split module object which contains obj arr split node which is created in this function
 *
 */
vx_status app_create_graph_obj_arr_split(vx_graph graph, ObjArrSplitObj *objArrSplitObj);

/* @} */

#endif
