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
 *  \file pdm_utils.h
 *
 *  \brief Header for PDM utils library.
 *
 */

#ifndef PDM_UTILS_H_
#define PDM_UTILS_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#if defined (SOC_J722S)
#include <drivers/hw_include/csl_types.h>
#else
#include <ti/csl/csl_types.h>
#endif
#include "pdm_utils_soc.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* Status Pass */
#define PDM_UTILS_PASS                         (0)
/* Status Fail */                              
#define PDM_UTILS_EFAIL                        (-1)
/* State: LPSC Disabled */                     
#define PDM_UTILS_LPSC_DISABLED                (0U)
/* State: LPSC Enabled */                      
#define PDM_UTILS_LPSC_ENABLED                 (1U)
/* State: LPSC Transition */                   
#define PDM_UTILS_LPSC_TRANSITION              (2U)
/* State: PD Disabled */                       
#define PDM_UTILS_PD_DISABLED                  (0U)
/* State: PD Enabled */                        
#define PDM_UTILS_PD_ENABLED                   (1U)
/* State: PLL Disabled */                      
#define PDM_UTILS_PLL_DISABLED                 (0U)
/* State: PLL Enabled */                       
#define PDM_UTILS_PLL_ENABLED                  (1U)
/* Macro to represent Invalid Base address */
#define PDM_UTILS_INVALID_VAL                  (0xDEADBEEFU)
/* Maximum number of HSDIVs as per PLL IP Spec */
#define PDM_UTILS_NUM_MAX_HSDIV                (0x10U)
/*  Maximum number of PDs present on this Device */
#define PDM_UTILS_NUM_MAX_PD                   (PDM_UTILS_NUM_MAX_PD_MAIN + PDM_UTILS_NUM_MAX_PD_MCU)
/* Maximum number of LPSCs present on this Device */
#define PDM_UTILS_NUM_MAX_LPSC                 (PDM_UTILS_NUM_MAX_LPSC_MAIN + PDM_UTILS_NUM_MAX_LPSC_MCU)
/* Macro for maximum number of PLLs in a device */
#define PDM_UTILS_NUM_MAX_PLL                  (PDM_UTILS_NUM_MAIN_PLL + PDM_UTILS_NUM_MCU_PLL )

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* Structure to hold PLL divider data */
typedef struct PdmUtils_PllDividerData_t
{
    /* Integer divider */
    uint32_t integerDiv;
    /* Fractional divider */
    uint32_t fracDiv;
    /* Post divider1 */
    uint32_t postDiv1;
    /* Post divider2 */
    uint32_t postDiv2;
    /* Pre divider */
    uint32_t preDiv;
} PdmUtils_PllDividerData;

/* Structure to hold PLL HSDIV data */
typedef struct PdmUtils_PllHsDivData_t
{
    /* HSDIV ID */
    uint32_t id;
    /* State: enabled/disabled */
    uint32_t enabled;
    /* Divider value of this HSDIV */
    uint32_t hsdiv;
    /* Output Frequency of this HSDIV */
    uint32_t freq;
} PdmUtils_PllHsDivData;

/* Structure to hold PLL data */
typedef struct PdmUtils_PllData_t
{
    /* Index of PLL */
    uint32_t                pllId;
    /* MAIN/MCU domain */
    uint32_t                domainID;
    /* State: whenther PLL is enabled or not */
    uint32_t                enabled;
    /* Parent clock Frequency */
    uint32_t                parentClk;
    /* Output VCO Frequency */
    uint64_t                vcoClkFreq;
    /* State: Whether post divider clock is enabled or not */
    uint32_t                postDivClkEnabled;
    /* Refer to PdmUtils_PllDividerData structure definition */
    PdmUtils_PllDividerData pllDivData;
    /* Number of HSDIVs connected to this PLL */
    uint32_t                numHsDivs;
    /* Refer to PdmUtils_PllHsDivData structure definition */
    PdmUtils_PllHsDivData   pllHsDivData[PDM_UTILS_NUM_MAX_HSDIV];
} PdmUtils_PllData;

/* Structure to hold PLL data book */
typedef struct PdmUtils_PllDataBook_t
{
    /* Number of MCU PLLs */
    uint32_t         numMcuPll;
    /* Number of Main PLLs */
    uint32_t         numMainPll;
    /* Structure to hold PLL data */
    PdmUtils_PllData pllData[PDM_UTILS_NUM_MAX_PLL];
} PdmUtils_PllDataBook;

/* Structure to hold LPSC data */
typedef struct PdmUtils_LpscData_t
{
    /* Index of PSC */
    uint32_t pscIdx;
    /* Index of LPSC */
    uint32_t lpscIdx;
    /* State of LPSC */
    uint32_t state;
    /* Name of LPSC */
    char lpscName[PDM_UTILS_NAME_MAX_LENGTH];
    /* Number of devices connected */
    uint32_t numDevices;
    /* IDs of the devices connected */
    uint32_t deviceId[PDM_UTILS_NUM_MAX_DEV_CONNECTED];
    /* Names of devices connected */
    char devNames[PDM_UTILS_NUM_MAX_DEV_CONNECTED][PDM_UTILS_NAME_MAX_LENGTH];
} PdmUtils_LpscData;

/* Structure to hold PD data */
typedef struct PdmUtils_PdData_t
{
    /* Index of PSC */
    uint32_t pscIdx;
    /* Index of PD */
    uint32_t pdIdx;
    /* State of PD */
    uint32_t state;
    /* Name of PD */
    char pdName[PDM_UTILS_NAME_MAX_LENGTH];
    /* Number of LPSCs connected */
    uint32_t numLpsc;
    /* Object to hold LPSC data */
    PdmUtils_LpscData lpsc[PDM_UTILS_NUM_MAX_LPSC];
} PdmUtils_PdData;

/* Structure to hold PSC data */
typedef struct PdmUtils_PscData_t
{
    /* Index of PSC */
    uint32_t pscIdx;
    /* Number of PDs connected */
    uint32_t numPD;
    /* Name of PSC */
    char pscName[PDM_UTILS_NAME_MAX_LENGTH];
    /* Object to hold PD data */
    PdmUtils_PdData pd[PDM_UTILS_NUM_MAX_PD];
} PdmUtils_PscData;

/* Structure to hold meta data for PSCs */
typedef struct PdmUtils_PscDataBook_t
{
    /* Number of PSCs */
    uint32_t numPsc;
    /* Object to hold PSC data*/
    PdmUtils_PscData psc[PDM_UTILS_NUM_MAX_PSC];
} PdmUtils_PscDataBook;

/* Structure to store clock info needed to replicate PET tool */
typedef struct PdmUtils_PetClock_t
{
    /* Device Id for the clock */
    uint32_t deviceId;
    /* Clock ID for the clock */
    uint32_t clockId;
    /* Name of the clock as per PET tool */
    char     clockName[PDM_UTILS_NAME_MAX_LENGTH];
    /* Frequency of the clock */
    uint32_t freq;
} PdmUtils_PetClock;

#if !defined(SOC_J721E) && !defined (SOC_J7200)
/* Structure to hold PET clock data book */
typedef struct PdmUtils_PetClockDataBook_t
{
    /* Number of PET clocks */
    uint32_t numClocks;
    /* Structure to hold each PET clock data */
    PdmUtils_PetClock petClocks[PDM_UTILS_PET_NUM_CLOCKS];
} PdmUtils_PetClockDataBook;
#endif

/* Structure to hold clock Id ane name of a particular clock */
typedef struct PdmUtils_DevClkData_t
{
    /* clock ID as per clock_ids.h file */
    uint32_t clkId;
    /* Name of the clock, refer soc header */
    char clkName[PDM_UTILS_NAME_MAX_LENGTH];
    /* Clock Frequency */
    uint64_t freq;
} PdmUtils_DevClkData;

/* Structure to hold clock tree info of a device*/
typedef struct PdmUtils_ClockTree_t
{
    /* TISCI clock ID */
    uint32_t clkId;
    /* clock name */
    char clkName[PDM_UTILS_NAME_MAX_LENGTH];
    /* Number of parent clocks till terminal parent */
    uint32_t clkTreeDepth;
    /* Clock tree for this device clock */
    PdmUtils_DevClkData clkTreeData[PDM_UTILS_NUM_MAX_CLOCK_TREE_DEPTH];
} PdmUtils_ClockTree;

/* Structure to hold device clocks */
typedef struct PdmUtils_Device_t
{
    /* Device ID */
    uint32_t devId;
    /* Device name */
    char     devName[PDM_UTILS_NAME_MAX_LENGTH];
    /* Number of imput clocks connected to this device */
    uint32_t numClocks;
    /* Structure to hold clock tree info of this device */
    PdmUtils_ClockTree clockTree[PDM_UTILS_NUM_MAX_DEVICE_CLOCKS];
} PdmUtils_Device;

/* Structure to hold clock tree of the entire SOC */
typedef struct PdmUtils_SocClockTree_t
{
    /* Number of devices present in the SOC */
    uint32_t numDevices;
    /* Structure to hold device info */
    PdmUtils_Device device[PDM_UTILS_NUM_MAX_DEVICES];
} PdmUtils_SocClockTree;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/**
 * \brief This API updates the value of HSDIVs connected to a PLL in PLL data structure. 
 *        Note that PLL data structure should have the valid PLL number and domain values for this API to work.
 *
 * \param pllData PLL data structure
 *
 * \return PDM_UTILS_PASS on success, else PDM_UTILS_FAIL
 */
int32_t PdmUtils_getNumHsdiv(PdmUtils_PllData *pllData);

/**
 * \brief This API updates the PLL status in the PLL data structure.
 *        Note that PLL data structure should have the valid PLL number and domain values for this API to work.
 *
 * \param pllData PLL data structure
 *
 * \return PDM_UTILS_PASS if enabled, else PDM_UTILS_FAIL
 */
int32_t PdmUtils_getPllStatus(PdmUtils_PllData *pllData);

/**
 * \brief This API updates the POSTDIV clock status in the PLL data structure.
 *        Note that PLL data structure should have the valid PLL number and domain values for this API to work.
 *
 * \param pllData PLL data structure
 *
 * \return PDM_UTILS_PASS on success, else PDM_UTILS_FAIL
 */
int32_t PdmUtils_getPllPostDivClkStatus(PdmUtils_PllData *pllData);

/**
 * \brief This API updates PLL divider data (M, FracM, M2 values) in the PLL data structure. 
 *        Note that PLL data structure should have the valid PLL number and domain values for this API to work.
 *
 * \param pllData PLL data structure
 *
 * \return PDM_UTILS_PASS on success, else PDM_UTILS_FAIL
 */
int32_t PdmUtils_getPllDividerData(PdmUtils_PllData *pllData);

/**
 * \brief This API updates HSDIV divider value in the PLL data structure.
 *        Note that PLL data structure should have the valid PLL number and domain values for this API to work.
 *
 * \param pllData PLL data structure
 *
 * \return PDM_UTILS_PASS on success, else PDM_UTILS_FAIL
 */
int32_t PdmUtils_getPllHsdivInfo(PdmUtils_PllData *pllData);

/**
 * \brief This API updates PLL data i.e., divider data, status etc in the PLL data structure.
 *        Note that PLL data structure should have the valid PLL number and domain values for this API to work.
 *
 * \param pllData PLL data structure
 *
 * \return PDM_UTILS_PASS on success, else PDM_UTILS_FAIL
 */
int32_t PdmUtils_getPllInfo(PdmUtils_PllData *pllData);

/**
 * \brief API to get SOC PLL data
 *
 * \param buf  Pointer to the data object to hold entire SOC PLL data 
 * \param size Size of the data object
 *
 * \return PDM_UTILS_PASS on success, else PDM_UTILS_FAIL
 */
int32_t PdmUtils_getPllDataBook(void *buf, uint32_t size);

/**
 * \brief API to get PSC data
 *
 * \param buf  Pointer to the data object to hold PSC, PD and LPSC status 
 * \param size Size of the data object
 *
 * \return PDM_UTILS_PASS on success, else PDM_UTILS_FAIL
 */
int32_t PdmUtils_getPscDataBook(void *buf, uint32_t size);

/**
 * \brief API to get Clock tree information of the SOC
 *
 * \param buf  Pointer to the data object to hold clock tree information
 * \param size Size of the buffer 
 *
 * \return PDM_UTILS_PASS on success, else PDM_UTILS_FAIL
 */
int32_t PdmUtils_getSocClockTree(void *buf, uint32_t size);

/**
 * \brief API to get Clock tree information of the device
 *
 * \param device Pointer to the data object to hold clock tree information of the device
 *
 * \return PDM_UTILS_PASS on success, else PDM_UTILS_FAIL
 */
int32_t PdmUtils_getDeviceClockTree(PdmUtils_Device *device);

/**
 * \brief API to get IP clocks as per PET tool
 *
 * \param buf  Pointer to the data object to hold PET clocks and its frequencies
 * \param size Size of the data object 
 *
 * \return PDM_UTILS_PASS on success, else PDM_UTILS_FAIL
 */
int32_t PdmUtils_getPetClockBook(void *buf, uint32_t size);

#endif /* PDM_UTILS_H_ */

