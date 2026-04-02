/*
*
* Copyright (c) {2015 - 2020} Texas Instruments Incorporated
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

/**
 *  \file tidl_custom.c
 *
 *  \brief File containing the TIDL custom layer APIs.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include "stdio.h"
#include "stdlib.h"
#include <itidl_ti.h>
#include "tidl_custom.h"

#include "tidl_custom_maxpooling.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define CUSTOM_SUCCESS        (0)
#define CUSTOM_FAIL           (-1)


/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                  Internal/Private Function Declarations                    */
/* ========================================================================== */


/**
 *  \brief API to update the output tensor scale depending on the min and max value.
 *
 *  \param  outputScale       Output tensor scale.
 *  \param  inputScale        Input tensor scale.
 *  \param  minTensorValue    Minimum tensor value. Could be current minimum value or
 *                            running minimum value depending upon whether this API is
 *                            used while stats collection or inference respectively.
 *  \param  maxTensorValue    Maximum tensor value. Could be current maximum value or
 *                            running maximum value depending upon whether this API is
 *                            used while stats collection or inference respectively.
 *
 *  \return None
 */


/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t TIDL_customLayerProcess(void * tidlHandle,
                                sTIDL_Layer_t *tidlLayer,
                                void *inPtrs[],
                                void *outPtrs[],
                                void *params,
                                void *dmaUtilsContext,
                                const sTIDL_sysMemHandle_t sysMems[TIDL_SYSMEM_MAX],
                                int32_t execMode)
{
  int32_t status = CUSTOM_SUCCESS;

  if(TIDL_CUSTOM_TYPE_0 == tidlLayer->layerParams.customParams.customLayerType)
  {
    status = TIDL_customMaxPoolingProcess(tidlHandle,
                                          tidlLayer,
                                          inPtrs,
                                          outPtrs,
                                          params,
                                          dmaUtilsContext,
                                          sysMems,
                                          execMode);
  }
  else if(TIDL_CUSTOM_TYPE_1 == tidlLayer->layerParams.customParams.customLayerType)
  {
    /* Call the corresponding custom layer's process function */
  }
  else if(TIDL_CUSTOM_TYPE_2 == tidlLayer->layerParams.customParams.customLayerType)
  {
    /* Call the corresponding custom layer's process function */
  }
  else if(TIDL_CUSTOM_TYPE_3 == tidlLayer->layerParams.customParams.customLayerType)
  {
    /* Call the corresponding custom layer's process function */
  }
  else
  {
    status = CUSTOM_FAIL;
  }

  return status;
}

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */


