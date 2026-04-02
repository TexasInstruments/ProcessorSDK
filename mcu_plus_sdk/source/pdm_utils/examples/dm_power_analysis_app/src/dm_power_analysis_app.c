/*
 *  Copyright (c) Texas Instruments Incorporated 2025
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 *  \file  dm_power_analysis_app.c
 *
 *  \brief Example implementation file for DM power analysis app
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <string.h>
#if defined (SOC_J722S)
#include <kernel/dpl/DebugP.h>
#else
#include <ti/drv/uart/UART_stdio.h>
#endif
#include <pdm_utils.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#if defined (SOC_J722S)
/* Redirecting UART_printf to DebugP_log in MCU+SDK */
#define UART_printf                                  DebugP_log
#endif

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 * \brief This API prints PSC, PD and LPSC status of the device
 *
 * \param buf  Pointer to the data object to hold PSC, PD and LPSC status
 * \param size Size of the data object
 *
 */
static void PdmUtils_printPscDataBook(void* buf, uint32_t size);

/**
 * \brief API to print given PLL data object
 *
 * \param pllData Pointer to data object for PLL data
 *
 */
static void PdmUtils_printPllData(PdmUtils_PllData *pllData);

/**
 * \brief API to print PLL data for all PLLs present on the SOC
 *
 * \param buf  Pointer to the data object to hold PSC, PD and LPSC status
 * \param size Size of the data object
 *
 */
static void PdmUtils_printPllDataBook(void* buf, uint32_t size);

/**
 * \brief This API prints the given string with right alignment in the given length
 *
 * \param buf String to be printed
 * \param len Length for alignment
 *
 */
static void PdmUtils_printStrAlign(char *buf, uint32_t len);

#if !defined (SOC_J721E) && !defined (SOC_J7200)
/**
 * \brief This API prints PET clock table
 *
 * \param buf  Pointer to the data object to hold PET clocks and its frequencies
 * \param size Size of the data object
 *
 */
static void PdmUtils_printPetClockTable(void *buf, uint32_t size);
#endif

#if defined (BUILD_MCU1_0) || defined (BUILD_WKUP_R5)
/**
 * \brief This API prints clock tree of the device
 *
 * \param buf  Pointer to the data object to hold clock tree information
 * \param size Size of the data object
 *
 */
static void PdmUtils_printClockTree(void *buf, uint32_t size);
#endif

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Data object to hold PLL data */
PdmUtils_PllDataBook gPllDatabook;
/* Data object to hold PSC data */
PdmUtils_PscDataBook gPscDataBook;
#if !defined(SOC_J721E) && !defined(SOC_J7200)
/* Data object to hold PET clock data */
PdmUtils_PetClockDataBook gPetClockBook;
#endif
#if defined (BUILD_MCU1_0) || defined (BUILD_WKUP_R5)
/* Data object to hold soc clock tree */
PdmUtils_SocClockTree gSocClockTree;
#endif

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

static void PdmUtils_printPllDataBook(void* buf, uint32_t size)
{
    uint32_t count;
    PdmUtils_PllDataBook *pllDataBook;
    PdmUtils_PllData *pllData;

    UART_printf("\n\n----------------PLL INFO STARTS-----------------\n\r");
    UART_printf("========================================================================================================================\n\r");
    UART_printf("|  PLL Name   | status  |         config        |     postdiv    | VCO       | Num of |  HSDIV  |  status  | Output    |\n\r");
    UART_printf("|             |         |                       |                | Freq(MHz) | HSDIVs |         |          | freq(MHz) |\n\r");
    UART_printf("========================================================================================================================\n\r");

    /* Validate the given buffer size */
    if((NULL == buf) || (size < (sizeof(PdmUtils_PllDataBook))))
    {
        UART_printf("DM Power Analysis App: Insufficient Buffer size for PLL data book\r\n");
    }
    else
    {
        pllDataBook = (PdmUtils_PllDataBook*)buf;
        pllData = pllDataBook->pllData;

        /* Print MCU and Main PLL data */
        for(count=0U; count<(pllDataBook->numMcuPll + pllDataBook->numMainPll); count++)
        {
            PdmUtils_printPllData(pllData);
            pllData++;
        }

        UART_printf("\n\n----------------PLL INFO ENDS-----------------\n\r");
    }
}

static void PdmUtils_printPllData(PdmUtils_PllData *pllData)
{
    char stringBuf[32];
    char statusStr[32];
    uint32_t count;

    if(NULL == pllData)
    {
        UART_printf("DM Power Analysis App: Invalid pointer for PLL data object\n\r");
    }
    else
    {
        snprintf(statusStr, sizeof(statusStr), "%s", pllData->domainID ? "MAIN" : "MCU");
        strcat(statusStr,"_PLL_");
        sprintf(stringBuf, "%s%d",statusStr, pllData->pllId);
        UART_printf("|");
        PdmUtils_printStrAlign(stringBuf, 13);
        sprintf(stringBuf, "%s", pllData->enabled ? "ENABLED" : "DISABLED");
        PdmUtils_printStrAlign(stringBuf, 9);
        sprintf(stringBuf, "M=%d, FracM=%d",pllData->pllDivData.integerDiv, pllData->pllDivData.fracDiv);
        PdmUtils_printStrAlign(stringBuf, 23);
        sprintf(stringBuf, "%s, M2=%d",(pllData->postDivClkEnabled ? "ENABLED" :"DISABLED"),pllData->pllDivData.postDiv1);
        PdmUtils_printStrAlign(stringBuf, 16);
        sprintf (stringBuf, "%d", (uint32_t)((pllData->vcoClkFreq)/1000000));
        PdmUtils_printStrAlign(stringBuf, 11);
        sprintf(stringBuf,"%d", pllData->numHsDivs);
        PdmUtils_printStrAlign(stringBuf, 8);

        /* Print all HSDivs info */
        for(count=0U; count<pllData->numHsDivs; count++)
        {
            if(count > 0)
            {
                UART_printf("|");
                sprintf(stringBuf,"%s", "");
                PdmUtils_printStrAlign(stringBuf, 13);
                PdmUtils_printStrAlign(stringBuf, 9);
                PdmUtils_printStrAlign(stringBuf, 23);
                PdmUtils_printStrAlign(stringBuf, 16);
                PdmUtils_printStrAlign(stringBuf, 11);
                PdmUtils_printStrAlign(stringBuf, 8);
            }
            UART_printf(" HSDIV%2d |", pllData->pllHsDivData[count].id);
            sprintf(stringBuf, "%s",(pllData->pllHsDivData[count].enabled ? "ENABLED" : "DISABLED"));
            PdmUtils_printStrAlign(stringBuf, 10);
            sprintf(stringBuf, "%d", (uint32_t)((pllData->pllHsDivData[count].freq)));
            PdmUtils_printStrAlign(stringBuf, 11);
            UART_printf("\n\r");
        }

        UART_printf("------------------------------------------------------------------------------------------------------------------------\n\r");
    }
}

static void PdmUtils_printStrAlign(char *str, uint32_t len)
{
    uint32_t strLen, count=0, numSpaces;

    strLen = strlen(str);
    numSpaces = len -strLen;
    UART_printf("%s",str);
    for(count=0U; count<numSpaces; count++)
    {
        UART_printf(" ");
    }

    UART_printf("|");
}

#if defined (BUILD_MCU1_0) || defined (BUILD_WKUP_R5)
/* Obtained PSC info for MCU1_0 and other cores differ due to inaccessiblity of
 * PM driver. Please refer documentation */
static void PdmUtils_printPscDataBook(void *buffer, uint32_t size)
{
    PdmUtils_PscDataBook *pscDatabook;
    PdmUtils_PscData *pscData;
    PdmUtils_PdData *pdData;
    PdmUtils_LpscData *lpscData;
    uint32_t pscIdx, pdIdx, lpscIdx, devIdx;

    /* Validate the given buffer size */
    if((size < sizeof(PdmUtils_PscDataBook)) || (NULL == buffer))
    {
        UART_printf("DM Power Analysis App: Insufficient Buffer size for PSC data book\r\n");
    }
    else
    {
        UART_printf("\n\n----------------PSC INFO STARTS-----------------\n\r");
        UART_printf("====================================================================================================================================================================\n\r");
        UART_printf("           PSC Name        |     Power Domain    |   PD status   |             LPSC name          |   LPSC Status   |                 Connected IPs                |\n\r");
        UART_printf("====================================================================================================================================================================\n\r");
        UART_printf("-------------------------------------------------------------------------------------------------------------------------------------------------------------------\n\r");

        pscDatabook = (PdmUtils_PscDataBook*)buffer;
        for(pscIdx=0U; pscIdx<pscDatabook->numPsc; pscIdx++)
        {
            pscData = &(pscDatabook->psc[pscIdx]);
            UART_printf("%25s  |", pscData->pscName);
            for(pdIdx=0U; pdIdx<pscData->numPD; pdIdx++)
            {
                if(pdIdx > 0U)
                {
                    UART_printf("%25s  |","");
                }

                UART_printf("%19s  |", pscData->pd[pdIdx].pdName);
                UART_printf("%13s  |", pscData->pd[pdIdx].state?"ENABLED":"DISABLED");

                pdData = (&pscData->pd[pdIdx]);
                for(lpscIdx=0U; lpscIdx<pdData->numLpsc; lpscIdx++)
                {
                    lpscData = (&pdData->lpsc[lpscIdx]);
                    if(lpscIdx > 0U)
                    {
                        UART_printf("%25s  |","");
                        UART_printf("%19s  |%13s  |","","");
                    }

                    UART_printf("%30s  |", lpscData->lpscName);
                    UART_printf("%15s  |", lpscData->state?"ENABLED":"DISABLED");
                    for(devIdx=0U; devIdx<lpscData->numDevices; devIdx++)
                    {
                        if(devIdx > 0U)
                        {
                            UART_printf("%25s  |","");
                            UART_printf("%19s  |%13s  |","","");
                            UART_printf("%30s  |%15s  |","","");
                        }
                        UART_printf("%44s  |\n\r" , lpscData->devNames[devIdx]);
                    }

                    if(lpscData->numDevices == 0U)
                    {
                        UART_printf("%44s  |\n\r","");
                    }
                }
            }
        }
        UART_printf("-------------------------------------------------------------------------------------------------------------------------------------------------------------------\n\r");
        UART_printf("\n\n----------------PSC INFO ENDS-----------------\n\r");
    }
}

static void PdmUtils_printClockTree(void *buf, uint32_t size)
{
    PdmUtils_SocClockTree *socClockTree;
    PdmUtils_ClockTree *clockTree;
    uint32_t deviceNum=0U, iterator=0U;
    int32_t count=0;

    /* Validate the given buffer size */
    if((NULL == buf) || (size < (sizeof(PdmUtils_PetClock))))
    {
        UART_printf("DM Power Analysis App: Insufficient Buffer size for clock tree data object\r\n");
    }
    else
    {
        socClockTree = (PdmUtils_SocClockTree*)buf;
        UART_printf("\n\n--------------------------------CLOCK TREE STARTS--------------------------------\n\r");
        for(deviceNum=0U; deviceNum<=socClockTree->numDevices; deviceNum++)
        {

            UART_printf("MODULE %s \n\r",socClockTree->device[deviceNum].devName);
            UART_printf("  Number of clocks for device :%d\n\r",socClockTree->device[deviceNum].numClocks);
            clockTree = socClockTree->device[deviceNum].clockTree;
            /* Iterate through all the device clocks */
            for(iterator=0U; iterator<socClockTree->device[deviceNum].numClocks; iterator++)
            {
                UART_printf("\tModule clock %3d:%s\n\r", iterator, clockTree[iterator].clkName);
                UART_printf("\tClock tree      :");
                /* Print clock tree starting from first parent  */
                for(count=clockTree[iterator].clkTreeDepth; count>=0 ; count--)
                {
                    if(0U == clockTree[iterator].clkTreeDepth)
                    {
                        UART_printf("Refer to Clock Architecture");
                    }
                    else
                    {
                        UART_printf("%10s", clockTree[iterator].clkTreeData[count].clkName);
                        UART_printf("(%d MHz)",(uint32_t)(clockTree[iterator].clkTreeData[count].freq/1000000));
                    }

                    if(count > 0U)
                    {
                        UART_printf(" -> ", clockTree[iterator].clkTreeData[count].clkName);
                    }
                }
                UART_printf("\n\r");
                UART_printf("------------------------------------------------------------------------\n\r");
            }
            if(deviceNum == (socClockTree->numDevices))
            {
                UART_printf("All devices done\n\r");
                UART_printf("\n\n\r----------------------------CLOCK TREE ENDS-----------------------------------------------\n\r");
            }
        }
    }
}

#else
static void PdmUtils_printPscDataBook(void *buffer, uint32_t size)
{
    PdmUtils_PscDataBook *pscDatabook;
    uint32_t pdIdx, pscIdx, lpscIdx;

    /* Validate the given buffer size */
    if((size < sizeof(PdmUtils_PscDataBook)) || (NULL == buffer))
    {
        UART_printf("DM Power Analysis App: Insufficient Buffer size for PSC data book\n\r");
    }
    else
    {
        UART_printf("\n\n----------------PSC INFO STARTS-----------------\n\r");
        UART_printf("==============================================\n\r");
        UART_printf("|          Device Name        |     status   |\n\r");
        UART_printf("==============================================\n\r");
        pscDatabook = (PdmUtils_PscDataBook*)buffer;
        for(pscIdx=0U; pscIdx<pscDatabook->numPsc; pscIdx++)
        {
            for(pdIdx=0U; pdIdx<pscDatabook->psc[pscIdx].numPD ; pdIdx++)
            {
                UART_printf("|%27s  |", pscDatabook->psc[pscIdx].pd[pdIdx].pdName);
                UART_printf("%12s  |\n\r", pscDatabook->psc[pscIdx].pd[pdIdx].state?"ENABLED":"DISABLED");
                for(lpscIdx=0U; lpscIdx<pscDatabook->psc[pscIdx].pd[pdIdx].numLpsc ; lpscIdx++)
                {
                    UART_printf("|%27s  |", pscDatabook->psc[pscIdx].pd[pdIdx].lpsc[lpscIdx].lpscName);
                    UART_printf("%12s  |\n\r", pscDatabook->psc[pscIdx].pd[pdIdx].lpsc[lpscIdx].state?"ENABLED":"DISABLED");
                }
            }
        }

        UART_printf("==============================================\n\r");
        UART_printf("\n\n----------------PSC INFO ENDS-----------------\n\r");
    }
}
#endif

#if !defined (SOC_J721E) && !defined (SOC_J7200)
static void PdmUtils_printPetClockTable(void *buf, uint32_t size)
{
    int32_t count;
    PdmUtils_PetClockDataBook *petClockBook;

    /* Validate the given buffer size */
    if((NULL == buf) || (size < (sizeof(PdmUtils_PetClock))))
    {
        UART_printf("DM Power Analysis App: Insufficient Buffer size for PET clock table data object\r\n");
    }
    else
    {
         petClockBook = (PdmUtils_PetClockDataBook*)buf;
         UART_printf("\n\n==============PET CLOCK TABLE START=============\r\n");
         UART_printf("========================================\r\n");
         UART_printf("|       Frequency of selected IPs      |\r\n");
         UART_printf("========================================\r\n");
         UART_printf("|       IP Name           |  Freq(MHz) |\r\n");
         UART_printf("========================================\r\n");
         for(count=0U; count<petClockBook->numClocks; count++)
         {
             UART_printf("|%25s", petClockBook->petClocks[count].clockName);
             UART_printf("|%12d|\n\r", petClockBook->petClocks[count].freq);
         }

         UART_printf("========================================\r\n");
         UART_printf("==============PET CLOCK TABLE END===============\r\n");
     }
}
#endif

/* Function to print PLL, Clock tree and PSC status */
void DmPowerAnalysisApp_main(void)
{
    int32_t retVal = PDM_UTILS_PASS;

    memset(&gPllDatabook, 0x0, sizeof(gPllDatabook));
#if !defined(SOC_J721E) && !defined(SOC_J7200)
    memset(&gPetClockBook, 0x0, sizeof(gPetClockBook));
#endif
    memset(&gPscDataBook, 0x0, sizeof(gPscDataBook));

    retVal = PdmUtils_getPllDataBook(&gPllDatabook, sizeof(PdmUtils_PllDataBook));
    if(PDM_UTILS_PASS == retVal)
    {
        PdmUtils_printPllDataBook(&gPllDatabook, sizeof(PdmUtils_PllDataBook));
    }
    else
    {
        UART_printf("DM Power Analysis App: Failed to get PLL Data of the device\n");
    }

    /*  PET table support not added for J721e */
#if !defined (SOC_J721E) && !defined (SOC_J7200)
    retVal = PdmUtils_getPetClockBook((void*)&gPetClockBook, sizeof(PdmUtils_PetClockDataBook));
    if(PDM_UTILS_PASS == retVal)
    {
        PdmUtils_printPetClockTable((void*)&gPetClockBook, sizeof(PdmUtils_PetClockDataBook));
    }
    else
    {
        UART_printf("DM Power Analysis App: Failed to get PET clock Data of the device\n");
    }
#endif

    retVal =  PdmUtils_getPscDataBook((void*)&gPscDataBook, sizeof(PdmUtils_PscDataBook));
    if(PDM_UTILS_PASS == retVal)
    {
        PdmUtils_printPscDataBook((void*)&gPscDataBook, sizeof(PdmUtils_PscDataBook));
    }
    else
    {
        UART_printf("DM Power Analysis App: Failed to get PSC Data of the device\n");
    }
#if defined (BUILD_MCU1_0) || defined (BUILD_WKUP_R5)
    memset(&gSocClockTree, 0x0, sizeof(gSocClockTree));
    retVal = PdmUtils_getSocClockTree((void*)&gSocClockTree, sizeof(gSocClockTree));
    if(PDM_UTILS_PASS == retVal)
    {
        PdmUtils_printClockTree((void*)&gSocClockTree, sizeof(gSocClockTree));
    }
    else
    {
        UART_printf("DM Power Analysis App: Failed to get SOC clock tree of the device\n");
    }

#endif
    return;
}

#if defined (BUILD_C7X)
extern void Osal_initMmuDefault(void);
void InitMmu(void)
{
    Osal_initMmuDefault();
}
#endif

