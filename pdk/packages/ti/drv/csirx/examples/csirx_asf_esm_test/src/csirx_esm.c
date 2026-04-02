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
 *  \file csirx_esm.c
 *
 *  \brief utility functions for csirx esm.
 *
 */
/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include "csirx_esm.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
/**
 * \brief Configuring ESM bitmap for CSIRX 
 *        Bit 200U to 205U are used for CSIRX interrupts
 *        200U/32 = 6U, 205U/32 = 6U (Bit Map Index - Group Number)
 *        200U%32 = 8U, 205U%32 = 13U (Bit Offsets - Bit Number)
 */
SDL_ESM_config CsirxAsf_Test_esmInitConfig_MAIN =
{
    .esmErrorConfig = {1u, 8u}, /* Self test error config - Don't Care for CSIRX Application */
    .enableBitmap = {0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00003f00u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u,
                },
     /**< Enable CSIRX Fatal and Non-Fatal Errors */
    .priorityBitmap = {0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00003f00u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u,
                        },
    /**< Set CSIRX Fatal and Non-Fatal Errors are High Priority */
    .errorpinBitmap = {0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u,
                 0x00000000u,
                      },
    /**< Disable CSIRX Fatal and Non-Fatal Errors from driving error pin */
};

static SDL_CSIRX_ESM_Instance_t SDL_ESM_instance_MAIN;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
SDL_Result ESM_init (const SDL_ESM_Inst esmInstType,
                        const SDL_ESM_config *esmInitConfig,
                        SDL_ESM_applicationCallback callback,
                        void *arg)
{
    SDL_Result result = SDL_PASS;
    int32_t SDLRet;
    uint32_t intNum;
    uint32_t i,j;
    uint32_t intStatus;
    esmIntrPriorityLvl_t intrPriorityLvlWr, intrPriorityLvlRd;
    bool enableWr, enableRd;
    uint32_t influence;
    uint32_t esmInstBaseAddr;
    uint32_t esmMaxNumevents;
    SDL_CSIRX_ESM_Instance_t *SDL_ESM_Instance;

    if(CSIRX_ESM_selectEsmInst(esmInstType, &SDL_ESM_Instance) == ((bool)false)){
        result = SDL_EBADARGS;
    }
    /* Check for valid esmInstConfig and esmInstType, and initialize appropriate
     * esmInstBaseAddr for register base and SDM_ESM_instance for SW instance structure. Also get Maximum number of events corresponding to the instance */
    if (( esmInitConfig == ((void *)0u)) ||
        ( CSIRX_ESM_getBaseAddr(esmInstType, &esmInstBaseAddr) == ((bool)false)))
    {

            result = SDL_EBADARGS;
    }
    else
    {
        CSIRX_ESM_getMaxNumEvents(esmInstType, &esmMaxNumevents);
        /* Record init config in instance */
        SDL_ESM_Instance->esmInitConfig.esmErrorConfig.bitNumber = esmInitConfig->esmErrorConfig.bitNumber;
        SDL_ESM_Instance->esmInitConfig.esmErrorConfig.groupNumber = esmInitConfig->esmErrorConfig.groupNumber;
        for(i=INIT_VAL;i<ARRAY_SIZE;i++){
            SDL_ESM_Instance->esmInitConfig.enableBitmap[i] = esmInitConfig->enableBitmap[i];
            SDL_ESM_Instance->esmInitConfig.priorityBitmap[i] = esmInitConfig->priorityBitmap[i];
            SDL_ESM_Instance->esmInitConfig.errorpinBitmap[i] = esmInitConfig->errorpinBitmap[i];
        }
        SDL_ESM_Instance->callback = callback;
        SDL_ESM_Instance->arg = arg;

        /* ESM reset and configure */
        (void)SDL_ESM_reset(esmInstBaseAddr);

        /* Enable interrupt for all events from init configuration*/
        for(i=((uint32_t)NULL); i <= (esmMaxNumevents/BITS_PER_WORD); i++)
        {
            uint32_t remainingBits = esmMaxNumevents - (i*BITS_PER_WORD);

            /* Enable the configuration interrupt for this group */
            SDLRet = SDL_ESM_clearCfgIntrStatus(esmInstBaseAddr, i);
            if (SDLRet == SDL_PASS)
            {
                SDLRet = SDL_ESM_getCfgIntrStatus(esmInstBaseAddr, i, &intStatus);
            }
            if (SDLRet == SDL_PASS)
            {
                if (intStatus != ((uint32_t)0))
                {
                    SDLRet = SDL_EFAIL;
                }
            }
            if (SDLRet == SDL_PASS)
            {
                SDLRet = SDL_ESM_enableCfgIntr(esmInstBaseAddr, i);
                if (SDLRet == SDL_PASS)
                {
                    SDLRet = SDL_ESM_isEnableCfgIntr(esmInstBaseAddr, i, &intStatus);
                }
            }

            if (remainingBits > BITS_PER_WORD){
                remainingBits = BITS_PER_WORD;
            }

            for(j=((uint32_t)NULL); j< remainingBits; j++)
            {
                intNum = (i*BITS_PER_WORD)+j;

                /* Clear interrupt status, so that we start with clean state */
                (void)SDL_ESM_clearIntrStatus(esmInstBaseAddr, intNum);

                (void)SDL_ESM_getIntrStatus(esmInstBaseAddr, intNum, &intStatus);


                /* Depending on the bitmap configuration enable interrupt and set priority level */
                if(((esmInitConfig->enableBitmap[i]) & (((uint32_t)MASK_BIT)<<j)) != 0u)
                {
                    /* Enable interrupt and verifiy if interrupt status is enabled */

                    (void)SDL_ESM_enableIntr(esmInstBaseAddr, intNum);

                    SDLRet = SDL_ESM_isEnableIntr(esmInstBaseAddr, intNum, &intStatus);

                    if (intStatus != ((uint32_t)STATUS_NUM))
                    {
                        SDLRet = SDL_EFAIL;
                    }

                    /* Set interrupt priority level and verifiy if priority level is set */
                    if (SDLRet == SDL_PASS)
                    {
                        intrPriorityLvlWr = ((esmInitConfig->priorityBitmap[i]
                                            & (((uint32_t)1u)<<j)) != 0u)?1u:0u;
                        SDLRet = SDL_ESM_setIntrPriorityLvl(esmInstBaseAddr,
                                                            intNum,
                                                            intrPriorityLvlWr);
                    }
                    if (SDLRet == SDL_PASS)
                    {
                        SDLRet = SDL_ESM_getIntrPriorityLvl(esmInstBaseAddr,
                                                            intNum,
                                                            &intrPriorityLvlRd);
                    }
                    if (SDLRet == SDL_PASS)
                    {
                        if (intrPriorityLvlWr != intrPriorityLvlRd)
                        {
                            SDLRet = SDL_EFAIL;
                        }
                    }
                }

                /*
                 * Depending on the bitmap configuration set error output
                 * and verify if error output is set */
                if (SDLRet == SDL_PASS)
                {
                    enableWr = ((esmInitConfig->errorpinBitmap[i]
                                & (((uint32_t)MASK_BIT)<<j)) != 0u)?(bool)true:(bool)false;
                    SDLRet = SDL_ESM_setInfluenceOnErrPin(esmInstBaseAddr,
                                                            intNum,
                                                            enableWr);
                }
                if (SDLRet == SDL_PASS)
                {
                    SDLRet = SDL_ESM_getInfluenceOnErrPin(esmInstBaseAddr,
                                                            intNum,
                                                            &influence);
                }
                if (SDLRet == SDL_PASS)
                {
                    enableRd = (influence != 0u)?(bool)true:(bool)false;
                    if (enableWr != enableRd)
                    {
                        SDLRet = SDL_EFAIL;
                    }
                }
                if (SDLRet != SDL_PASS)
                {
                    break;
                }
            }
        }

        /* Enable Global interrupt and verify if global interrupt is enabled for ESM */
        if (SDLRet == SDL_PASS)
        {
            SDLRet = SDL_ESM_enableGlobalIntr(esmInstBaseAddr);
        }
        if (SDLRet == SDL_PASS)
        {
            SDLRet = SDL_ESM_getGlobalIntrEnabledStatus(esmInstBaseAddr, &intStatus);
        }
        if (SDLRet == SDL_PASS)
        {
            if (intStatus != SDL_ESM_EN_KEY_ENBALE_VAL)
            {
                SDLRet = SDL_EFAIL;
            }
        }
        if (SDLRet != SDL_PASS)
        {
            result = SDL_EFAIL;
        }
    }
    return result;
}

bool CSIRX_ESM_selectEsmInst(const SDL_ESM_Inst esmInstType,
                           SDL_CSIRX_ESM_Instance_t **pEsmInstancePtr)
{
    bool instValid = ((bool)true);

    switch(esmInstType)
    {
        case SDL_ESM_INST_MAIN_ESM0:
            *pEsmInstancePtr = (SDL_CSIRX_ESM_Instance_t *)(&SDL_ESM_instance_MAIN);
            break;
        default:
            /* Invalid instance input parameter */
            instValid = ((bool)false);
            break;
    }

    return (instValid);
}

bool CSIRX_ESM_getBaseAddr(const SDL_ESM_Inst esmInstType, uint32_t *esmBaseAddr)
{
    bool instValid = ((bool)false);

    if (esmBaseAddr != NULL)
    {
        switch(esmInstType)
        {
            case SDL_ESM_INST_MAIN_ESM0:
                instValid = ((bool)true);
                *esmBaseAddr = SOC_MAIN_ESM_BASE;
                break;
            default:
                break;
        }
    }

    return (instValid);
}

bool CSIRX_ESM_getMaxNumEvents(const SDL_ESM_Inst esmInstType,
                             uint32_t *esmMaxNumEvents)
{
    bool instValid = ((bool)false);

    if (esmMaxNumEvents != NULL)
    {
        switch(esmInstType)
        {
            case SDL_ESM_INST_MAIN_ESM0:
                instValid = ((bool)true);
                *esmMaxNumEvents = SOC_MAIN_ESM_MAX_NUM_EVENTS;
                break;
            default:
                break;
        }
    }

    return (instValid);
}

void CSIRX_ESM_appCallbackIntrHandler (esmIntrPriorityLvl_t esmIntrPriorityLvlType,
                                uintptr_t arg)
{
    uint32_t intSrc;
    esmGroupIntrStatus_t localEsmGroupIntrStatus;
    SDL_ESM_IntType esmIntType;
    (void)arg;

    if (esmIntrPriorityLvlType == ESM_INTR_PRIORITY_LEVEL_HIGH) {
        esmIntType = SDL_ESM_INT_TYPE_HI;
    }
    else {
        esmIntType = SDL_ESM_INT_TYPE_LO;
    }
    /* Check on the highest priority event and handle it */
    do {
        (void)SDL_ESM_getGroupIntrStatus(SOC_MAIN_ESM_BASE,
                                         (uint32_t)esmIntrPriorityLvlType,
                                         &localEsmGroupIntrStatus);
        intSrc = localEsmGroupIntrStatus.highestPendLvlIntNum;
        CSIRX_ESM_processInterruptSource(SOC_MAIN_ESM_BASE, esmIntType, intSrc);
    } while ((intSrc != (uint32_t)(NO_EVENT_VALUE)));

    return;
}

void CSIRX_ESM_processInterruptSource(uint32_t esmInstBaseAddr,
                                           SDL_ESM_IntType esmIntType,
                                           uint32_t intSrc)
{
    SDL_CSIRX_ESM_Instance_t *SDL_ESM_instance;
    SDL_ESM_Inst esmInstType;
    uint32_t groupNumber, intIndex;
    CSIRX_ESM_selectEsmInstFromAddr(esmInstBaseAddr,
                                  &esmInstType,
                                  &SDL_ESM_instance);
    if (intSrc != NO_EVENT_VALUE) {
        if (intSrc < (BITS_PER_WORD*SDL_ESM_MAX_EVENT_MAP_NUM_WORDS)) {
            CSIRX_ESM_getGroupNumberIndex(intSrc, &groupNumber, &intIndex);

            if((SDL_ESM_instance->esmInitConfig.enableBitmap[groupNumber]
               & (((uint32_t)MASK_BIT)<<intIndex)) != INVALID_BIT) {

                if (SDL_ESM_instance->callback != NULL) {
                    (void)SDL_ESM_instance->callback(esmInstType, esmIntType,
                                    groupNumber, intIndex,
                                    intSrc, SDL_ESM_instance->arg);
                }
            }
            (void)SDL_ESM_clearIntrStatus(esmInstBaseAddr, intSrc);
        }
    }
    return;
}

void CSIRX_ESM_getGroupNumberIndex(uint32_t intSrc, uint32_t *groupNumber,
                                    uint32_t *intIndex)
{
    *groupNumber = intSrc >> GROUP_NUMBER_BIT_SHIFT;
    *intIndex = intSrc-((*groupNumber) << GROUP_NUMBER_BIT_SHIFT);

    return;
}

int32_t CSIRX_MainESM_writeEOI(esmIntrType_t intrType)
{
    int32_t  retVal = SDL_EBADARGS;
    if (SOC_MAIN_ESM_BASE != ((uint32_t) (0u)))
    {
        HW_WR_FIELD32(SOC_MAIN_ESM_BASE + SDL_ESM_EOI, SDL_ESM_EOI_KEY, intrType);
        retVal = SDL_PASS;
    }
    return retVal;
}

void CSIRX_ESM_selectEsmInstFromAddr(uint32_t esmInstBaseAddr,
                                   SDL_ESM_Inst *pEsmInstType,
                                   SDL_CSIRX_ESM_Instance_t **pEsmInstancePtr)
{
    switch(esmInstBaseAddr)
    {
        case SOC_MAIN_ESM_BASE:
            *pEsmInstancePtr = &SDL_ESM_instance_MAIN;
            *pEsmInstType = SDL_ESM_INST_MAIN_ESM0;
            break;
        default:
            break;
    }
}
