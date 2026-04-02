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
 *  \file pdm_utils_psc.c
 *
 *  \brief This file implements APIs to provide PSC info
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include "string.h"
#include "pdm_utils.h"
#include "pdm_utils_soc.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* Offset for configuration registers of a given LPSC id */
#define PSC_MDSTAT(id)                         (0x800U + (4U * (id)))
/* Offset for configuration registers of a given PD id */
#define PSC_PDSTAT(domain)                     (0x200U + (4U * (domain)))

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Array that holds the LPSC names in their respective LPSC index
 * This is defined in pdm_utils_soc.c file, used here to populate the LPSC name
 * corresponding to the LPSC Index
 */
extern char *gPdmUtilsLpscNames[PDM_UTILS_NUM_MAX_PSC][PDM_UTILS_NUM_MAX_LPSC];

/* Array that holds the PSC names in their respective PSC index
 * This is defined in pdm_utils_soc.c file, used here to populate the PSC name
 * corresponding to the PSC Index
 */
extern char *gPdmUtilsPscNames[PDM_UTILS_NUM_MAX_PSC];

/* Array that holds the PD names in their respective PD index
 * This is defined in pdm_utils_soc.c file, used here to populate the PD name
 * corresponding to the PD Index
 */
extern char *gPdmUtilsPdNames[PDM_UTILS_NUM_MAX_PSC][PDM_UTILS_NUM_MAX_PD];

/* Array that holds the device names in their respective device ID Index
 * This is defined in pdm_utils_soc.c file, used here to populate the device name
 * corresponding to the device ID
 */
extern char *gPdmUtilsDevNames[PDM_UTILS_NUM_MAX_DEVICES+1];

/* ========================================================================== */
/*                       Function Declarations                                */
/* ========================================================================== */

/**
 * \brief API to get whether LPSC of given index and given psc is enabled or not
 *
 * \param lpsc LPSC data structure
 *
 * \return PDM_UTILS_PASS on success, else PDM_UTILS_FAIL
 */
static int32_t PdmUtils_getLpscState(PdmUtils_LpscData *lpsc);

/**
 * \brief API to get whether PD of a given pd index and psc index is enabled or not
 *
 * \param pd PD data structure
 *
 * \return PDM_UTILS_PASS on success, else PDM_UTILS_FAIL
 */
static int32_t PdmUtils_getPDState(PdmUtils_PdData *pd);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

#if defined(BUILD_MCU1_0) || defined (BUILD_WKUP_R5)
int32_t PdmUtils_getPscDataBook(void *buffer, uint32_t size)
{
    int32_t ret = PDM_UTILS_PASS, pdCount=-1;
    PdmUtils_PscDataBook *pscDatabook;
    struct device *pscDev;
    const struct psc_drv_data *psc;
    const struct lpsc_module_data	*lpscData;
    int32_t pdIdx=-1, pdIdxLocal=-1, iterator=0;
    uint32_t lpscCount=0U, pscIdx=0U, numDevices, count;

    if((size < sizeof(PdmUtils_PscDataBook)) || (NULL == buffer))
    {
        ret = PDM_UTILS_EFAIL;
    }
    else
    {
        pscDatabook = (PdmUtils_PscDataBook*)buffer;
        pscDatabook->numPsc = PDM_UTILS_NUM_MAX_PSC;
        /* Iterate through all PSCs in the SOC */
        for(pscIdx=0U; pscIdx<pscDatabook->numPsc; pscIdx++)
        {
             pscDatabook->psc[pscIdx].pscIdx = pscIdx;
             pdCount = -1;
             snprintf(pscDatabook->psc[pscIdx].pscName, sizeof(pscDatabook->psc[pscIdx].pscName), "%s", gPdmUtilsPscNames[pscIdx]);
             pscDev = psc_lookup(pscIdx);

             if(pscDev != NULL)
             {
                 /* Get PSC driver object */
                 psc = to_psc_drv_data(get_drv_data(pscDev));
                 lpscData = psc->mod_data;
                 /* Iterate through number of modules connected to this lpsc */
                 for(count=0U; count<psc->module_count; count++)
                 {
                     if(lpscData[count].flags & LPSC_MODULE_EXISTS)
                     {
                         pdIdx = lpscData[count].powerdomain;
                         if(pdIdx != pdIdxLocal)
                         {
                             pdCount++;
                             /* Get PSC and PD index */
                             pscDatabook->psc[pscIdx].pd[pdCount].pscIdx = pscIdx;
                             pscDatabook->psc[pscIdx].pd[pdCount].pdIdx = pdIdx;
                             /* Get PD state */
                             PdmUtils_getPDState(&(pscDatabook->psc[pscIdx].pd[pdCount]));
                             lpscCount = 0;
                         }
                         else
                         {
                             lpscCount++;
                         }
                         pscDatabook->psc[pscIdx].pd[pdCount].lpsc[lpscCount].pscIdx = pscIdx;
                         pscDatabook->psc[pscIdx].pd[pdCount].lpsc[lpscCount].lpscIdx = count;
                         /* Get LPSC state */
                         PdmUtils_getLpscState(&(pscDatabook->psc[pscIdx].pd[pdCount].lpsc[lpscCount]));
                         iterator = 0U;
                         if(lpscData[count].flags & LPSC_DEVICES_LIST)
                         {
                             numDevices=0U;
                             /* For all the available devices, get the deviceID and names */
                             while(lpscData[count].lpsc_dev.dev_list[numDevices] != DEV_ID_NONE)
                             {
                                 pscDatabook->psc[pscIdx].pd[pdCount].lpsc[lpscCount].deviceId[numDevices] =
                                                                      lpscData[count].lpsc_dev.dev_list[numDevices];
                                 snprintf(pscDatabook->psc[pscIdx].pd[pdCount].lpsc[lpscCount].devNames[numDevices],
                                          sizeof(pscDatabook->psc[pscIdx].pd[pdCount].lpsc[lpscCount].devNames[numDevices]),
                                          "%s",
                                          gPdmUtilsDevNames[lpscData[count].lpsc_dev.dev_list[numDevices]]);
                                 numDevices++;
                             }
                             pscDatabook->psc[pscIdx].pd[pdCount].lpsc[lpscCount].numDevices = numDevices;
                         }
                         else
                         {
                             numDevices=0U;
                             /* For all the available devices, get the deviceID and names */
                             for(iterator=0U;
                                (iterator<4U &&
                                 (lpscData[count].lpsc_dev.dev_array[iterator]!=DEV_ID_NONE) &&
                                 (lpscData[count].lpsc_dev.dev_array[iterator]!=0U));
				iterator++)
                                {
                                    pscDatabook->psc[pscIdx].pd[pdCount].lpsc[lpscCount].deviceId[iterator] =
                                                                         lpscData[count].lpsc_dev.dev_array[iterator];
                                    snprintf(pscDatabook->psc[pscIdx].pd[pdCount].lpsc[lpscCount].devNames[iterator],
                                             sizeof(pscDatabook->psc[pscIdx].pd[pdCount].lpsc[lpscCount].devNames[iterator]),
                                             "%s", gPdmUtilsDevNames[lpscData[count].lpsc_dev.dev_array[iterator]]);
                                    numDevices++;

                                }
                                pscDatabook->psc[pscIdx].pd[pdCount].lpsc[lpscCount].numDevices = numDevices;
                         }
                         pdIdxLocal = pdIdx;
                         pscDatabook->psc[pscIdx].pd[pdCount].numLpsc =
                         lpscCount+1;
                    }
                }
            }
            pscDatabook->psc[pscIdx].numPD = pdCount+1;
        }
    }
    return ret;
}

#else
int32_t PdmUtils_getPscDataBook(void *buffer, uint32_t size)
{
    int32_t ret = PDM_UTILS_PASS;
    PdmUtils_PscDataBook *pscDatabook;
    uint32_t pdIdx=0, lpscIdx=0, pscIdx=0;

    if((size < sizeof(PdmUtils_PscDataBook)) || (NULL == buffer))
    {
        ret = PDM_UTILS_EFAIL;
    }
    else
    {
        PdmUtils_getNumPsc(buffer, size);
        pscDatabook = (PdmUtils_PscDataBook*)buffer;

        for (pscIdx=0U; pscIdx<pscDatabook->numPsc; pscIdx++)
        {
             pscDatabook->psc[pscIdx].pscIdx = pscIdx;
             snprintf(pscDatabook->psc[pscIdx].pscName,
                      sizeof(pscDatabook->psc[pscIdx].pscName), 
                      "%s", gPdmUtilsPscNames[pscIdx]);
             for (pdIdx=0U; pdIdx<pscDatabook->psc[pscIdx].numPD ; pdIdx++)
             {
                 pscDatabook->psc[pscIdx].pd[pdIdx].pscIdx = pscIdx;
                 pscDatabook->psc[pscIdx].pd[pdIdx].pdIdx = pdIdx;

                 ret += PdmUtils_getPDState(&(pscDatabook->psc[pscIdx].pd[pdIdx]));
                 for (lpscIdx=0U; lpscIdx<pscDatabook->psc[pscIdx].pd[pdIdx].numLpsc; lpscIdx++)
                 {
                     pscDatabook->psc[pscIdx].pd[pdIdx].lpsc[lpscIdx].pscIdx = pscIdx;
                     pscDatabook->psc[pscIdx].pd[pdIdx].lpsc[lpscIdx].lpscIdx = lpscIdx;
                     ret += PdmUtils_getLpscState(&(pscDatabook->psc[pscIdx].pd[pdIdx].lpsc[lpscIdx]));
                 }
             }
        }
    }
    return ret;
}
#endif

static int32_t PdmUtils_getLpscState(PdmUtils_LpscData *lpsc)
{
    uint32_t readVal;
    int32_t ret = PDM_UTILS_PASS;
    uint32_t baseAddr;

    baseAddr = PdmUtils_getLpscBaseAddr(lpsc->pscIdx);
    if(baseAddr == PDM_UTILS_INVALID_VAL)
    {
        ret = PDM_UTILS_EFAIL;
    }

    snprintf(lpsc->lpscName, sizeof(lpsc->lpscName), "%s", gPdmUtilsLpscNames[lpsc->pscIdx][lpsc->lpscIdx]);
    readVal = CSL_REG32_RD(baseAddr + PSC_MDSTAT(lpsc->lpscIdx)) & CSL_PSC_MDSTAT_STATE_MASK;
    if(readVal == CSL_PSC_MDSTAT_STATE_SWRSTDISABLE)
    {
        lpsc->state = PDM_UTILS_LPSC_DISABLED; /* Disabled */
    }
    else if((readVal == CSL_PSC_MDSTAT_STATE_DISABLE) ||
             (readVal == CSL_PSC_MDSTAT_STATE_ENABLE) ||
             (readVal == CSL_PSC_MDSTAT_STATE_SYNCRST))
    {
        lpsc->state = PDM_UTILS_LPSC_ENABLED;       /* Enabled or retention */
    }
    else
    {
        lpsc->state = PDM_UTILS_LPSC_TRANSITION;       /* Transition (other) */
    }

    return ret;
}

static int32_t PdmUtils_getPDState(PdmUtils_PdData *pd)
{
    uint32_t readVal;
    int32_t ret = PDM_UTILS_PASS;
    uint32_t baseAddr;

    baseAddr = PdmUtils_getLpscBaseAddr(pd->pscIdx);
    if(baseAddr == PDM_UTILS_INVALID_VAL)
    {
        ret = PDM_UTILS_EFAIL;
    }

    snprintf(pd->pdName, sizeof(pd->pdName), "%s", gPdmUtilsPdNames[pd->pscIdx][pd->pdIdx]);
    readVal = CSL_REG32_RD(baseAddr + PSC_PDSTAT(pd->pdIdx)) & CSL_PSC_PDSTAT_STATE_MASK;
    if(readVal != CSL_PSC_PDSTAT_STATE_ON)
    {
        pd->state = PDM_UTILS_PD_DISABLED; /* Disabled */
    }
    else
    {
        pd->state = PDM_UTILS_PD_ENABLED;       /* Enabled or retention */
    }

    return ret;
}

