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

#ifndef ITT_SERVER_LINUX_PRIV_H_
#define ITT_SERVER_LINUX_PRIV_H_

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include <pthread.h>
#include <utils/ipc/include/app_ipc.h>
#include <utils/remote_service/include/app_remote_service.h>
#include <sensor_drv/include/iss_sensors.h>
#include <itt_server_remote/include/itt_srvr_remote.h>
#include <utils/console_io/include/app_log.h>
#include <itt_server.h>


//#define ENABLE_DEBUG_ITTSERVER 


#define NETWORK_CTRL_HEADER         (0x1234ABCDU)
#define NETWORK_RX_HEADER           (0x5678ABCDU)
#define NETWORK_TX_HEADER           (0xABCD4321U)
#define TRUE    1
#define FALSE    0

#if defined (SOC_AM62A)
#define APP_IPC_RTOS_CPU APP_IPC_CPU_MCU1_0
#else
#define APP_IPC_RTOS_CPU APP_IPC_CPU_MCU2_0
#endif

#define NETWORKCTRL_ISS_DCC_FILE_SIZE                           (128U*1024U)
#define ALGORITHM_AEWB_LINK_CMD_GET_DCC_BUF_PARAMS                 (0x1003)
#define ALGORITHM_AEWB_LINK_CMD_PARSE_AND_SET_DCC_PARAMS         (0x1002)


int32_t IttCtrl_readParams(uint8_t *pPrm, uint32_t prmSize);
int32_t IttCtrl_writeParams(uint8_t *pPrm, uint32_t prmSize, uint32_t returnStatus);


/**
 *******************************************************************************
 *  \brief Information of saved RAW data frame
 *******************************************************************************
 */
typedef struct
{
    uint32_t isSaveFrameComplete;
    /**< TRUE: Frame is saved at address mentioned in 'bufAddr'
     *   FALSE: Frame is not yet saved, try after some time
     */

    uint8_t *bufAddr;
    /**< Address where frame is saved */

    uint32_t bufSize;
    /**< Size of buffer where frame is saved */

} AlgItt_GetSaveFrameStatus;



/**
 *******************************************************************************
 *   \brief Structure containing control parameters for DCC
 *******************************************************************************
*/
typedef struct
{
    AlgItt_ControlParams baseClassControl;
    /**< Base class control params */

    uint8_t       *dccBuf;
    /**< Pointer to the DCC File */

    uint32_t      dccBufSize;
    /**< DCC Buffer Size */
} AlgItt_IssAewbDccControlParams;

/* Initialize I2C bus for camera register read/write */
int32_t i2cInit(void);

/**
 *******************************************************************************
 *   \brief Structure containing necessary objects for EdgeAI live tuning
 *******************************************************************************
*/
typedef struct
{
    vx_context context;
    vx_node *node;
    tivx_raw_image *raw_image_handle;
    vx_image *yuv_image_handle;

} module_obj_t;

/**
 *******************************************************************************
 *   \brief Structure for EdgeAI ITT Server
 *******************************************************************************
*/
typedef struct itt_server_obj {

    /*! VISS module object */
    module_obj_t obj_viss;

    /*! LDC module object */
    module_obj_t obj_ldc; 

} ITTServerEdgeAIObj;

typedef enum {
    VISS = 0,
    AEWB,
    LDC,
} EDGEAI_NODES;

#endif /* ITT_SERVER_LINUX_PRIV_H_ */
