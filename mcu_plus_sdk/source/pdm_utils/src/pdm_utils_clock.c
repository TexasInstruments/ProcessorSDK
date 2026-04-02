/*
 * Copyright (c) 2025, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/**
 *  \file pdm_utils_clock.c
 *
 *  \brief This file implements APIs to provide clock info.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include "pdm_utils.h"
#include "pdm_utils_soc.h"

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                       Internal Function Declarations                       */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Array that holds the device clock names in their respective clock IDs
 * This is defined in pdm_utils_soc.c file, used here to populate the clock name
 * for a given clock ID
 */
extern char *gPdmUtilsClockNames[PDM_UTILS_NUM_MAX_CLOCKS+1];

/* Array that holds the device names in their respective device IDs
 * This is defined in pdm_utils_soc.c file, used here to populate the device name
 * for a given clock ID
 */
extern char *gPdmUtilsDevNames[PDM_UTILS_NUM_MAX_DEVICES+1];

/* Array that holds the device clock count.
 * This is defined in devices.c file, used here to get number of clocks connected to each device
 */
extern const struct devgroup soc_devgroups[PDM_UTILS_DEVGRP_RANGE_ID_MAX];

/* Array that holds device clock ID and frequency info.
 * This is defined in devices.c file, used here for get information on each
 * clock connected to each device
 */
extern const struct dev_data *const soc_device_data_arr[PDM_UTILS_NUM_MAX_DEVICES + 1U];

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t PdmUtils_getSocClockTree(void *buf, uint32_t size)
{
    int32_t retVal=0;
    const struct dev_data *deviceData=NULL;
    uint32_t deviceNum=0U;
    int32_t deviceCount=-1;

    PdmUtils_SocClockTree *socClockTree;

    /* Validate the given buffer size */
    if((size < sizeof(PdmUtils_SocClockTree)) || (NULL == buf))
    {
        retVal = PDM_UTILS_EFAIL;
    }
    else
    {
        socClockTree = (PdmUtils_SocClockTree*)buf;
        for(deviceNum=0U; deviceNum<=PDM_UTILS_NUM_MAX_DEVICES; deviceNum++)
        {
            deviceData = soc_device_data_arr[deviceNum];
            /* Check if the obtained device data object is null */
            if(NULL != deviceData)
            {
                 deviceCount++;
                 socClockTree->device[deviceCount].devId = deviceNum;
                 retVal += PdmUtils_getDeviceClockTree(&(socClockTree->device[deviceCount]));
            }
        }
        socClockTree->numDevices = deviceCount+1;
    }

    return retVal;
}

int32_t PdmUtils_getDeviceClockTree(PdmUtils_Device *device)
{
    int32_t retVal=0U;
    struct clk *clkOutput=NULL;
    const struct clk_parent *clkParentNode;
    const struct dev_data *deviceData=NULL;
    const struct dev_clk_data *deviceClkData=NULL;
    uint16_t deviceClkGrp;
    uint32_t clkParentCount=0U, clkCount=0U, iterator=0U;
    PdmUtils_ClockTree *clockTree;

    /* Validate the given pointer */
    if((NULL == device))
    {
        retVal = PDM_UTILS_EFAIL;
    }
    else
    {
       deviceData = soc_device_data_arr[device->devId];
       /* Check if the obtained device data object is null */
       if(deviceData == NULL)
       {
          retVal = PDM_UTILS_EFAIL;
       }
       else
       {
           clkCount = 0U;
           /* Populate device name */
           snprintf(device->devName, sizeof(device->devName), "%s", gPdmUtilsDevNames[device->devId]);
           clockTree = device->clockTree;

           deviceClkData = soc_devgroups[deviceData->pm_devgrp].dev_clk_data;
           deviceClkGrp = deviceData->dev_clk_idx;

           for(iterator=0U; iterator<deviceData->n_clocks; iterator++)
           {
               /* Check if clk object is null or of type mux parent */
               if((0U != deviceClkData[deviceClkGrp+iterator].clk) &&
                  (DEV_CLK_TABLE_TYPE_PARENT !=
                   deviceClkData[deviceClkGrp+iterator].type))
               {
                    clkParentCount=0U;
                    /* With clockID, get the clk object */
                    clkOutput = clk_lookup(deviceClkData[deviceClkGrp+iterator].clk);
                    if (NULL != clkOutput)
                    {
                         /* Obtain the clockID and clockName */
                         clockTree[clkCount].clkId = deviceClkData[deviceClkGrp+iterator].clk;
                         snprintf(clockTree[clkCount].clkName,
                                  sizeof(clockTree[clkCount].clkName), "%s", gPdmUtilsClockNames[deviceClkData[deviceClkGrp+iterator].clk]);
                         clockTree[clkCount].clkTreeData[clkParentCount].clkId = deviceClkData[deviceClkGrp+iterator].clk;
                         snprintf(clockTree[clkCount].clkTreeData[clkParentCount].clkName,
                                  sizeof(clockTree[clkCount].clkTreeData[clkParentCount].clkName),
                                  "%s", gPdmUtilsClockNames[deviceClkData[deviceClkGrp+iterator].clk]);
                         clockTree[clkCount].clkTreeData[clkParentCount].freq = clk_get_freq(clkOutput)/deviceClkData[deviceClkGrp+iterator].cdiv;
                         /* With clk object, obtain clock parent */
                         clkParentNode = clk_get_parent(clkOutput);
                         /* Iterate till the last parent node */
                         while(NULL != clkParentNode)
                         {
                             clkParentCount++;
                             /* Obtain the clockID and clockName */
                             clockTree[clkCount].clkTreeData[clkParentCount].clkId = clkParentNode->clk;
                             snprintf(clockTree[clkCount].clkTreeData[clkParentCount].clkName,
                                      sizeof(clockTree[clkCount].clkTreeData[clkParentCount].clkName),
                                      "%s", gPdmUtilsClockNames[clkParentNode->clk]);
                             /* With clockID, get the clk object */
                             clkOutput = clk_lookup((clk_idx_t)clkParentNode->clk);
                             if(NULL == clkOutput)
                             {
                                 break;
                             }
                             /* From clk object, obtain frequency of the clock */
                             clockTree[clkCount].clkTreeData[clkParentCount].freq = clk_get_freq(clkOutput)/clkParentNode->cdiv;
                             /* With clk object, obtain clock parent */
                             clkParentNode = clk_get_parent(clkOutput);
                         }
                         clockTree[clkCount].clkTreeDepth = clkParentCount;
                         clkCount++;
                    }
                }
           }

           device->numClocks = clkCount;
        }
    }
    return retVal;
}



