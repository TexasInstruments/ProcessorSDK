/*
 *  Copyright (c) Texas Instruments Incorporated 2020
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
 *   \file mailbox_soc.c
 *
 *   \brief The file implements the Platform specific Mailbox Driver Interface
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <string.h>
#include <ti/csl/soc.h>
#include <ti/drv/mailbox/mailbox.h>
#include <ti/drv/mailbox/src/mailbox_internal.h>
#include <ti/csl/hw_types.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/*! \brief Mailbox receive buffer size */

/*! \brief Mailbox receive buffer size 512 bytes */
#define MAILBOX_512B                 (0U)

/*! \brief Mailbox receive buffer size 1024 bytes */
#define MAILBOX_1024B                (1U)

/*! \brief Mailbox receive buffer size 1536 bytes */
#define MAILBOX_1536B                (2U)

/*! \brief Mailbox receive buffer size 2048 bytes */
#define MAILBOX_2048B                (3U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/*!
 *  @brief Mailbox hardware addresses for one mailbox in one direction
 *
 *  This Structure contains the register and data memory addresses for one logical mailbox in one direction.
 *
 *  \ingroup MAILBOX_DRIVER_INTERNAL_DATA_STRUCTURE
 */
typedef struct Mailbox_Base_t
{
    /**
     * @brief  Register base.
     */
    // MAILBOXRegs   *reg;

    /**
     * @brief  Data memory base.
     */
    uint8_t        *data;

} Mailbox_Base;

/**
 * @brief
 *  Mailbox Driver HW configuration
 *
 * @details
 *  The structure is used to store the hardware specific configuration which is
 *  passed to each driver instance
 *
 */
typedef struct Mailbox_HwCfg_t
{
    CSL_mboxRegAddr   *mbxReg;
    /**
     * @brief  Register and memory base for localEndpoint to remoteEndpoint communication.
     */
    Mailbox_Base       baseLocalToRemote;
    /**
     * @brief  Register and memory base for remoteEndpoint to localEndpoint communication.
     */
    Mailbox_Base       baseRemoteToLocal;
    /**
     * @brief  Mailbox Full Interrupt Number
     */
    uint32_t           boxFullIntNum;
    /**
     * @brief  Mailbox Empty Interrupt Number
     */
    uint32_t           boxEmptyIntNum;
    /**
     * @brief  Remote End Point processor Number
     */
    uint32_t           remoteProcNum;
}Mailbox_HwCfg;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static void Mailbox_boxFullISR(uintptr_t arg);
static void Mailbox_boxEmptyISR(uintptr_t arg);
static int32_t Mailbox_validateLocalEndPointRxMem(void);
static int32_t Mailbox_writeReset(Mbox_Handle handle);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
/*!
 *  @brief    Semaphore structure
 */
typedef struct Mailbox_driverPoolElem_s {
    bool            used;
    Mailbox_Driver  mbxDrvObj;
} Mailbox_driverPoolElem;

/* global pool of statically allocated semaphore pools */
static Mailbox_driverPoolElem gMailboxDriversPool[MAILBOX_DRIVER_POOL_NUM_ELEMENTS];

/**
 * @brief
 *  Global Variable for tracking information required by the mailbox driver.
 */
extern Mailbox_MCB gMailboxMCB;

#if defined (BUILD_MCU1_0)
CSL_mboxRegAddr gMboxReg =
{
    .memInit        = (uintptr_t)(CSL_MSS_CTRL_U_BASE + CSL_MSS_CTRL_MSS_MAILBOX_MEM_INIT),
    .memInitDone    = (uintptr_t)(CSL_MSS_CTRL_U_BASE + CSL_MSS_CTRL_MSS_MAILBOX_MEM_INIT_DONE),
    .memInitStatus  = (uintptr_t)(CSL_MSS_CTRL_U_BASE + CSL_MSS_CTRL_MSS_MAILBOX_MEM_INIT_STATUS),
    .mboxWriteDone  = (uintptr_t)(CSL_MSS_CTRL_U_BASE + CSL_MSS_CTRL_MSS_CR5A_MBOX_WRITE_DONE),
    .mboxReadReq    = (uintptr_t)(CSL_MSS_CTRL_U_BASE + CSL_MSS_CTRL_MSS_CR5A_MBOX_READ_REQ),
    .mboxReadDone   = (uintptr_t)(CSL_MSS_CTRL_U_BASE + CSL_MSS_CTRL_MSS_CR5A_MBOX_READ_DONE),
    .mboxReadDoneAck = (uintptr_t)(CSL_MSS_CTRL_U_BASE + CSL_MSS_CTRL_MSS_CR5A_B_MBOX_READ_DONE_ACK)
};


/**
 * @brief   This is SOC specific configuration and should *NOT* be modified by the customer.
 */
Mailbox_HwCfg   gRemoteHwCfg[] =
{
     /* Communication between MSS and DSS (MSS master). */
    {
        .mbxReg                 = &gMboxReg,
        .baseLocalToRemote.data = (uint8_t *)(CR5A_TO_DSS_MBX_MEM),
        .baseRemoteToLocal.data = (uint8_t *)(DSS_TO_CR5A_MBX_MEM),
        .boxFullIntNum          = CSL_MSS_INTR_MSS_CR5A_MBOX_READ_REQ,
        .boxEmptyIntNum         = CSL_MSS_INTR_MSS_CR5A_MBOX_READ_ACK,
        .remoteProcNum          = MAILBOX_INST_DSP
    },
    /* Communication between MSS and RSS (MSS master). */
    {
        .mbxReg                 = &gMboxReg,
        .baseLocalToRemote.data = (uint8_t *)(CR5A_TO_RSS_MBX_MEM),
        .baseRemoteToLocal.data = (uint8_t *)(RSS_TO_CR5A_MBX_MEM),
        .boxFullIntNum          = CSL_MSS_INTR_MSS_CR5A_MBOX_READ_REQ,
        .boxEmptyIntNum         = CSL_MSS_INTR_MSS_CR5A_MBOX_READ_ACK,
        .remoteProcNum          = MAILBOX_INST_RSS
    },
    /* Communication between CR5A and CR5B (MSS master). */
    {
        .mbxReg                 = &gMboxReg,
        .baseLocalToRemote.data = (uint8_t *)(CR5A_TO_CR5B_MBX_MEM),
        .baseRemoteToLocal.data = (uint8_t *)(CR5B_TO_CR5A_MBX_MEM),
        .boxFullIntNum          = CSL_MSS_INTR_MSS_CR5A_MBOX_READ_REQ,
        .boxEmptyIntNum         = CSL_MSS_INTR_MSS_CR5A_MBOX_READ_ACK,
        .remoteProcNum          = MAILBOX_INST_MSS_CR5B
    },

};

#endif

#if defined (BUILD_MCU1_1)
CSL_mboxRegAddr gMboxReg =
{
    .memInit        = (uintptr_t)(CSL_MSS_CTRL_U_BASE + CSL_MSS_CTRL_MSS_MAILBOX_MEM_INIT),
    .memInitDone    = (uintptr_t)(CSL_MSS_CTRL_U_BASE + CSL_MSS_CTRL_MSS_MAILBOX_MEM_INIT_DONE),
    .memInitStatus  = (uintptr_t)(CSL_MSS_CTRL_U_BASE + CSL_MSS_CTRL_MSS_MAILBOX_MEM_INIT_STATUS),
    .mboxWriteDone  = (uintptr_t)(CSL_MSS_CTRL_U_BASE + CSL_MSS_CTRL_MSS_CR5B_MBOX_WRITE_DONE),
    .mboxReadReq    = (uintptr_t)(CSL_MSS_CTRL_U_BASE + CSL_MSS_CTRL_MSS_CR5B_MBOX_READ_REQ),
    .mboxReadDone   = (uintptr_t)(CSL_MSS_CTRL_U_BASE + CSL_MSS_CTRL_MSS_CR5B_MBOX_READ_DONE),
    .mboxReadDoneAck = (uintptr_t)(CSL_MSS_CTRL_U_BASE + CSL_MSS_CTRL_MSS_CR5A_B_MBOX_READ_DONE_ACK)
};


/**
 * @brief   This is SOC specific configuration and should *NOT* be modified by the customer.
 */
Mailbox_HwCfg   gRemoteHwCfg[] =
{
     /* Communication between MSS and DSS (MSS master). */
    {
        .mbxReg                 = &gMboxReg,
        .baseLocalToRemote.data = (uint8_t *)(CR5B_TO_DSS_MBX_MEM),
        .baseRemoteToLocal.data = (uint8_t *)(DSS_TO_CR5B_MBX_MEM),
        .boxFullIntNum          = CSL_MSS_INTR_MSS_CR5B_MBOX_READ_REQ,
        .boxEmptyIntNum         = CSL_MSS_INTR_MSS_CR5B_MBOX_READ_ACK,
        .remoteProcNum          = MAILBOX_INST_DSP
    },
    /* Communication between CR5A and CR5B (MSS master). */
    {
        .mbxReg                 = &gMboxReg,
        .baseLocalToRemote.data = (uint8_t *)(CR5B_TO_CR5A_MBX_MEM),
        .baseRemoteToLocal.data = (uint8_t *)(CR5A_TO_CR5B_MBX_MEM),
        .boxFullIntNum          = CSL_MSS_INTR_MSS_CR5B_MBOX_READ_REQ,
        .boxEmptyIntNum         = CSL_MSS_INTR_MSS_CR5B_MBOX_READ_ACK,
        .remoteProcNum          = MAILBOX_INST_MSS_CR5B
    }
};

#endif

#if defined (BUILD_DSP_1)
CSL_mboxRegAddr gMboxReg =
{
    .memInit        = (uintptr_t)(CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_MAILBOX_MEMINIT_START),
    .memInitDone    = (uintptr_t)(CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_MAILBOX_MEMINIT_DONE),
    .memInitStatus  = (uintptr_t)(CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_MAILBOX_MEMINIT_STATUS),
    .mboxWriteDone  = (uintptr_t)(CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_DSP_MBOX_WRITE_DONE),
    .mboxReadReq    = (uintptr_t)(CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_DSP_MBOX_READ_REQ),
    .mboxReadDone   = (uintptr_t)(CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_DSP_MBOX_READ_DONE),
    .mboxReadDoneAck   = (uintptr_t)(CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_DSP_MBOX_READ_DONE_ACK),
};


/**
 * @brief   This is SOC specific configuration and should *NOT* be modified by the customer.
 */
Mailbox_HwCfg   gRemoteHwCfg[] =
{
    /* Communication between MSS and DSS (MSS master). */
    {
        .mbxReg                 = &gMboxReg,
        .baseLocalToRemote.data = (uint8_t *)(DSS_TO_CR5A_MBX_MEM),
        .baseRemoteToLocal.data = (uint8_t *)(CR5A_TO_DSS_MBX_MEM),
        .boxFullIntNum          = CSL_DSS_INTR_DSS_DSP_MBOX_READ_REQ,
        .boxEmptyIntNum         = CSL_DSS_INTR_DSS_DSP_MBOX_READ_ACK,
        .remoteProcNum          = MAILBOX_INST_MSS_CR5A
    },
    /* Communication between RSS and DSS (RSS master). */
    {
        .mbxReg                 = &gMboxReg,
        .baseLocalToRemote.data = (uint8_t *)(DSS_TO_RSS_MBX_MEM),
        .baseRemoteToLocal.data = (uint8_t *)(RSS_TO_DSS_MBX_MEM),
        .boxFullIntNum          = CSL_DSS_INTR_DSS_DSP_MBOX_READ_REQ,
        .boxEmptyIntNum         = CSL_DSS_INTR_DSS_DSP_MBOX_READ_ACK,
        .remoteProcNum          = MAILBOX_INST_RSS
    },
    /* Communication between CR5B and DSS (MSS master). */
    {
        .mbxReg                 = &gMboxReg,
        .baseLocalToRemote.data = (uint8_t *)(DSS_TO_CR5B_MBX_MEM),
        .baseRemoteToLocal.data = (uint8_t *)(CR5B_TO_DSS_MBX_MEM),
        .boxFullIntNum          = CSL_DSS_INTR_DSS_DSP_MBOX_READ_REQ,
        .boxEmptyIntNum         = CSL_DSS_INTR_DSS_DSP_MBOX_READ_ACK,
        .remoteProcNum          = MAILBOX_INST_MSS_CR5B
    }
};
#endif

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int32_t Mailbox_validateLocalEndPoint(Mailbox_Instance localEndpoint)
{
    int32_t retVal = MAILBOX_SOK;
    /* Validate local End point based on the Core. */
#if defined (BUILD_MCU1_0)
    if (localEndpoint != MAILBOX_INST_MSS_CR5A)
    {
        retVal = MAILBOX_EINVAL;
    }
#elif defined (BUILD_MCU1_1)
    if (localEndpoint != MAILBOX_INST_MSS_CR5B)
    {
        retVal = MAILBOX_EINVAL;
    }
#elif defined (BUILD_DSP_1)
    if (localEndpoint != MAILBOX_INST_DSP)
    {
        retVal = MAILBOX_EINVAL;
    }
#else
    /* Currently not supported for other cores. */
    retVal = MAILBOX_EINVAL;
#endif
    return retVal;
}

int32_t Mailbox_validateRemoteEndPoint(Mailbox_Instance localEndpoint, Mailbox_Instance remoteEndpoint)
{
    int32_t retVal = MAILBOX_SOK;

    /* Configure Rx memory size and offset only if remoteEndpoint is RSS */
    if ((remoteEndpoint == MAILBOX_INST_RSS) && ((localEndpoint == MAILBOX_INST_MSS_CR5A) || 
        (localEndpoint == MAILBOX_INST_DSP)))
    {
        retVal = Mailbox_validateLocalEndPointRxMem();
    }

    if ((remoteEndpoint == localEndpoint) || (remoteEndpoint > MAILBOX_INST_LAST))
    {
        retVal = MAILBOX_EINVAL;
    }
    return retVal;
}

int32_t Mailbox_validateDataTransferMode(Mailbox_DataTransferMode dataTransferMode)
{
    int32_t retVal = MAILBOX_SOK;
    if (dataTransferMode != MAILBOX_DATA_TRANSFER_MEMCPY)
    {
        retVal = MAILBOX_EINVAL;
    }
    return retVal;
}

int32_t Mailbox_validateReadWriteMode(Mailbox_Mode readMode, Mailbox_Mode writeMode)
{
    int32_t retVal = MAILBOX_SOK;
    if (writeMode == MAILBOX_MODE_CALLBACK)
    {
        retVal = MAILBOX_EINVAL;
    }
    return retVal;
}

int32_t Mailbox_isMultiChannelSupported(Mailbox_Instance localEndpoint, Mailbox_Instance remoteEndpoint)
{
    int32_t retVal = MAILBOX_EINVAL;
#if defined (BUILD_MCU1_0)
    /* Multichannel supported only between DSS and MSS. */
    if ((localEndpoint == MAILBOX_INST_MSS_CR5A) &&
        ((remoteEndpoint == MAILBOX_INST_DSP) || (remoteEndpoint == MAILBOX_INST_MSS_CR5B)))
    {
        retVal = MAILBOX_SOK;
    }
#elif defined (BUILD_MCU1_1)
    /* Multichannel supported only between DSS and MSS. */
    if ((localEndpoint == MAILBOX_INST_MSS_CR5B) &&
        ((remoteEndpoint == MAILBOX_INST_MSS_CR5A) || (remoteEndpoint == MAILBOX_INST_DSP)))
    {
        retVal = MAILBOX_SOK;
    }
#elif defined (BUILD_DSP_1)
    /* Multichannel supported only between DSS and MSS. */
    if ((localEndpoint == MAILBOX_INST_DSP) &&
        ((remoteEndpoint == MAILBOX_INST_MSS_CR5A) || (remoteEndpoint == MAILBOX_INST_MSS_CR5B)))
    {
        retVal = MAILBOX_SOK;
    }
#else
    /* Currently not supported for other cores. */
    retVal = MAILBOX_EINVAL;
#endif
    return retVal;
}

Mbox_Handle Mailbox_allocDriver(Mailbox_Instance remoteEndpoint)
{
    Mailbox_Driver *driver = NULL;
    Mailbox_driverPoolElem  *mboxPool;
    uint32_t                 i, maxMboxDrvElem;
    uintptr_t         key;

    mboxPool = &gMailboxDriversPool[0];
    maxMboxDrvElem = MAILBOX_DRIVER_POOL_NUM_ELEMENTS;

    key = HwiP_disable();

     for (i = 0; i < maxMboxDrvElem; i++)
     {
         if (mboxPool[i].used == FALSE)
         {
             mboxPool[i].used = TRUE;
             break;
         }
     }
     HwiP_restore(key);

    if (i < maxMboxDrvElem)
    {
        /* Grab the memory */
        driver = (Mailbox_Driver *) &mboxPool[i].mbxDrvObj;
        memset(driver, 0, sizeof(Mailbox_Driver));
    }

    return (Mbox_Handle)driver;
}

int32_t Mailbox_freeDriver(Mbox_Handle handle)
{
    Mailbox_Driver *driver = (Mailbox_Driver *) handle;
    Mailbox_driverPoolElem  *mboxPool;
    uint32_t                 i, maxMboxDrvElem;
    int32_t         retVal = MAILBOX_EINVAL;
    uintptr_t         key;

    mboxPool = &gMailboxDriversPool[0];
    maxMboxDrvElem = MAILBOX_DRIVER_POOL_NUM_ELEMENTS;

    key = HwiP_disable();
    for (i = 0; i < maxMboxDrvElem; i++)
    {
        if ((mboxPool[i].used == TRUE) &&
            (&mboxPool[i].mbxDrvObj == driver))
        {
            mboxPool[i].used = FALSE;
            memset(driver, 0, sizeof(Mailbox_Driver));
            retVal = MAILBOX_SOK;
            break;
        }
    }
    HwiP_restore(key);

    return retVal;
}

/* TODO: Proc Num itself can be used as Mailbox type. */
int32_t getMailboxTypeFromProcNum(uint32_t procNum, Mailbox_Instance *remoteEndpoint)
{
    int32_t retVal = MAILBOX_SOK;

    switch (procNum)
    {
        case MAILBOX_INST_MSS_CR5A:
            *remoteEndpoint = MAILBOX_INST_MSS_CR5A;
            break;
        case MAILBOX_INST_MSS_CR5B:
            *remoteEndpoint = MAILBOX_INST_MSS_CR5B;
            break;
        case MAILBOX_INST_RSS:
            *remoteEndpoint = MAILBOX_INST_RSS;
            break;
        case MAILBOX_INST_DSP:
            *remoteEndpoint = MAILBOX_INST_DSP;
            break;
        default:
            retVal = MAILBOX_EINVAL;
    }
    return retVal;
}

int32_t Mailbox_getBoxFullRemoteInst(Mailbox_Instance *remoteEndpoint)
{
    uint32_t regVal;
    int32_t retVal = MAILBOX_EINVAL;
    uint32_t i;

    regVal = CSL_Mbox_getBoxFullIntr(&gMboxReg);
    if (regVal != 0)
    {
        for (i=0; i<MAILBOX_INST_LAST; i++)
        {
            uint32_t bits = (i << 2U);
            if ((regVal & (1 << bits)) != 0U)
            {
                retVal = getMailboxTypeFromProcNum(i, remoteEndpoint);
                break;
            }
        }
    }
    return retVal;
}

int32_t Mailbox_getBoxEmptyRemoteInst(Mailbox_Instance *remoteEndpoint)
{
    uint32_t regVal;
    int32_t retVal = MAILBOX_EINVAL;
    uint32_t i;

    regVal = CSL_Mbox_getBoxEmptyIntr(&gMboxReg);
    if (regVal != 0)
    {
        for (i=0; i<MAILBOX_INST_LAST; i++)
        {
            uint32_t bits = (i << 2U);
            if ((regVal & (1 << bits)) != 0U)
            {
                retVal = getMailboxTypeFromProcNum(i, remoteEndpoint);
                break;
            }
        }
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function configures the hardware base addresses and interrupt numbers
 *      It populates the driver with hard coded information derived from the platform
 *      files.
 *
 *  @param[in]  driver handle
 *
 *
 *  @retval
 *      Not applicable
 *
 *  \ingroup MAILBOX_DRIVER_INTERNAL_FUNCTION
 *
 */
void* Mailbox_getHwCfg(Mailbox_Instance remoteEndpoint)
{
    Mailbox_HwCfg *hwCfg = NULL;
    uint32_t numRemote, i;
    volatile uint32_t rssCtrlBootInfoReg3 = HW_RD_REG32((CSL_RSS_PROC_CTRL_U_BASE + CSL_RSS_PROC_CTRL_RSS_CR4_BOOT_INFO_REG3));
    uint32_t mssOffsetIndex = HW_GET_FIELD(rssCtrlBootInfoReg3, CSL_RSS_PROC_CTRL_RSS_CR4_BOOT_INFO_REG3_MSS_OFFSET_INDEX);
    uint32_t mssBuffSize    = HW_GET_FIELD(rssCtrlBootInfoReg3, CSL_RSS_PROC_CTRL_RSS_CR4_BOOT_INFO_REG3_MSS_BUFF_SIZE);
    uint32_t mssSysSelect   = HW_GET_FIELD(rssCtrlBootInfoReg3, CSL_RSS_PROC_CTRL_RSS_CR4_BOOT_INFO_REG3_MSS_SYS_SELECT);
    uint32_t mssEnable      = HW_GET_FIELD(rssCtrlBootInfoReg3, CSL_RSS_PROC_CTRL_RSS_CR4_BOOT_INFO_REG3_MSS_ENABLE);
    uint32_t dssOffsetIndex = HW_GET_FIELD(rssCtrlBootInfoReg3, CSL_RSS_PROC_CTRL_RSS_CR4_BOOT_INFO_REG3_DSS_OFFSET_INDEX);
    uint32_t dssBuffSize    = HW_GET_FIELD(rssCtrlBootInfoReg3, CSL_RSS_PROC_CTRL_RSS_CR4_BOOT_INFO_REG3_DSS_BUFF_SIZE);
    uint32_t dssSysSelect   = HW_GET_FIELD(rssCtrlBootInfoReg3, CSL_RSS_PROC_CTRL_RSS_CR4_BOOT_INFO_REG3_DSS_SYS_SELECT);
    uint32_t dssEnable      = HW_GET_FIELD(rssCtrlBootInfoReg3, CSL_RSS_PROC_CTRL_RSS_CR4_BOOT_INFO_REG3_DSS_ENABLE);

    numRemote = sizeof(gRemoteHwCfg)/sizeof(Mailbox_HwCfg);
    for (i=0; i<numRemote; i++)
    {
        if (gRemoteHwCfg[i].remoteProcNum == remoteEndpoint)
        {
            hwCfg= &gRemoteHwCfg[i];
        }
    }

#if defined (BUILD_MCU1_0)
    /*This is local endpoint MSS*/
    if(remoteEndpoint == MAILBOX_INST_DSP)
    {
        /* If MSS-RSS communication enabled*/
        if(mssEnable && (!mssSysSelect))
        {
            /* If MSS memory offset is Zero. */
            if(mssOffsetIndex == 0u)
            {
                /* Remote to local memory starts after mss buffer size
                 * Mailbox memory is divided statically based on the current SBL configuration
                 * of 512 Byte RSS Buffer. Validate the same if the RSS Mailbox is initialized.
                 * hwCfg->baseRemoteToLocal.data += ((mssBuffSize + 1) * MAILBOX_BUFF_UNIT_SIZE); */
                if (mssBuffSize != 0)
                {
                    hwCfg = NULL;
                }
            }
        }
        
        /* If DSS-RSS communication enabled*/
        if(dssEnable && (!dssSysSelect))
        {
            /* If DSS memory offset is Zero. */
            if(dssOffsetIndex == 0u)
            {
                /* Remote to local memory starts after mss buffer size
                 * Mailbox memory is divided statically based on the current SBL configuration
                 * of 512 Byte RSS Buffer. Validate the same if the RSS Mailbox is initialized.
                 * hwCfg->baseLocalToRemote.data += ((dssBuffSize + 1) * MAILBOX_BUFF_UNIT_SIZE); */
                if (dssBuffSize != 0)
                {
                    hwCfg = NULL;
                }
            }
        }
    }

    if(remoteEndpoint == MAILBOX_INST_RSS)
    {
        /* Mailbox memory is divided statically based on the current SBL configuration
         * of 512 Byte RSS Buffer. Validate the same if the RSS Mailbox is initialized.
         * hwCfg->baseRemoteToLocal.data += (mssOffsetIndex * ((mssBuffSize + 1) * MAILBOX_BUFF_UNIT_SIZE)); */
        if ((mssOffsetIndex != 0) || (mssBuffSize != 0))
        {
            hwCfg = NULL;
        }
    }
#endif

#if defined (BUILD_DSP_1)
    /* This is local endpoint DSS */
    if(remoteEndpoint == MAILBOX_INST_MSS_CR5A)
    {
        /* If MSS-RSS communication enabled*/
        if(mssEnable && (!mssSysSelect))
        {
            /* If MSS memory offset is Zero. */
            if(mssOffsetIndex == 0u)
            {
                /* Remote to local memory starts after mss buffer size
                 * Mailbox memory is divided statically based on the current SBL configuration
                 * of 512 Byte RSS Buffer. Validate the same if the RSS Mailbox is initialized.
                 * hwCfg->baseLocalToRemote.data += ((mssBuffSize + 1) * MAILBOX_BUFF_UNIT_SIZE); */
                if (mssBuffSize != 0)
                {
                    hwCfg = NULL;
                }
            }
        }
        
        /* If DSS-RSS communication enabled*/
        if(dssEnable && (!dssSysSelect))
        {
            /* If MSS memory offset is Zero. */
            if(dssOffsetIndex == 0u)
            {
                /* Remote to local memory starts after DSS buffer size
                 * Mailbox memory is divided statically based on the current SBL configuration
                 * of 512 Byte RSS Buffer. Validate the same if the RSS Mailbox is initialized.
                 * hwCfg->baseRemoteToLocal.data += ((dssBuffSize + 1) * MAILBOX_BUFF_UNIT_SIZE); */
                if (dssBuffSize != 0)
                {
                    hwCfg = NULL;
                }
            }
        }
    }
    if(remoteEndpoint == MAILBOX_INST_RSS)
    {
        /* Mailbox memory is divided statically based on the current SBL configuration
         * of 512 Byte RSS Buffer. Validate the same if the RSS Mailbox is initialized.
         * hwCfg->baseRemoteToLocal.data += (dssOffsetIndex * ((dssBuffSize + 1) * MAILBOX_BUFF_UNIT_SIZE)); */
        if ((mssOffsetIndex != 0) || (mssBuffSize != 0))
        {
            hwCfg = NULL;
        }
    }
#endif
    return (void *)hwCfg;
}

int32_t Mailbox_initHw(Mbox_Handle handle)
{
    Mailbox_Driver*     driver;
    int32_t             retVal = MAILBOX_SOK;
    Mailbox_HwCfg       *hwCfg = NULL;

    driver = (Mailbox_Driver*)handle;

    if (handle == NULL)
    {
        retVal = MAILBOX_EINVAL;
    }

    if (retVal == MAILBOX_SOK)
    {
        hwCfg = (Mailbox_HwCfg *)driver->hwCfg;

        /* Clear interrupt status register. */
        CSL_Mbox_clearTxAckInterrupt(hwCfg->mbxReg, hwCfg->remoteProcNum);

        /* Init mailbox. */
        CSL_Mbox_memInit(hwCfg->mbxReg);

        while (CSL_Mbox_isMeminitDone(hwCfg->mbxReg) != 0x01)
        {
            /* Wait till init is complete. */
        }
    }
    return retVal;
}

int32_t Mailbox_registerInterrupts(Mbox_Handle handle)
{
    Mailbox_Driver*          driver;
    int32_t                  retVal = MAILBOX_SOK;
    Mailbox_HwCfg            *hwCfg = NULL;
    uint32_t                 priority;
    void                     *hwiHandle;

    driver = (Mailbox_Driver *)handle;

    if ((gMailboxMCB.initParam.osalPrms.enableIntr == NULL) ||
        (gMailboxMCB.initParam.osalPrms.disableIntr == NULL) ||
        (gMailboxMCB.initParam.osalPrms.registerIntr == NULL) ||
        (gMailboxMCB.initParam.osalPrms.unRegisterIntr == NULL))
    {
        retVal = MAILBOX_EINVAL;
    }

    if ((retVal == MAILBOX_SOK) && (gMailboxMCB.totalInstCnt == 0U))
    {
        /* Initialize the mailbox */
        Mailbox_initHw(handle);

        hwCfg = (Mailbox_HwCfg *)driver->hwCfg;

        /* Register the Interrupt Handler: Every mailbox has 2 interrupts: "mailbox full" and "mailbox empty"*/
        /************** Mailbox full ***********/
#if defined(BUILD_DSP_1)
        priority = MAILBOX_OSAL_DEFAULT_PRIORITY;
#else
        priority = 0x1U;
#endif
        hwiHandle = gMailboxMCB.initParam.osalPrms.registerIntr(Mailbox_boxFullISR,
                                                                (uint32_t)(hwCfg)->boxFullIntNum,
                                                                priority, NULL,
                                                                (char *)("MAILBOX_FULL"));
        gMailboxMCB.hwiHandles.mailboxFull = hwiHandle;

        /* Debug Message: */
        DebugP_log2 ("MAILBOX: Mailbox Driver Registering Mailbox Full HWI ISR [%p] for Interrupt %d\n",
                      (uintptr_t)gMailboxMCB.hwiHandles.mailboxFull, (hwCfg)->boxFullIntNum);
        gMailboxMCB.initParam.osalPrms.enableIntr((uint32_t)(hwCfg)->boxFullIntNum);

        /************** Mailbox empty ***********/
        hwiHandle = gMailboxMCB.initParam.osalPrms.registerIntr(Mailbox_boxEmptyISR,
                                                                (uint32_t)(hwCfg)->boxEmptyIntNum,
                                                                priority, NULL,
                                                                (char *)("MAILBOX_EMPTY"));
        gMailboxMCB.hwiHandles.mailboxEmpty = hwiHandle;

        /* Debug Message: */
        DebugP_log2 ("MAILBOX: Mailbox Driver Registering Mailbox Full to Core HWI ISR [%p] for Interrupt %d\n",
                      (uintptr_t)gMailboxMCB.hwiHandles.mailboxEmpty, (hwCfg)->boxEmptyIntNum);
        gMailboxMCB.initParam.osalPrms.enableIntr((uint32_t)(hwCfg)->boxEmptyIntNum);
    }
    return retVal;
}

int32_t Mailbox_unregisterInterrupts(Mbox_Handle handle)
{
    Mailbox_Driver*    driver;
    int32_t            retVal = MAILBOX_SOK;
    Mailbox_HwCfg      *hwCfg = NULL;

    driver = (Mailbox_Driver *)handle;
    hwCfg = (Mailbox_HwCfg *)driver->hwCfg;

    if(gMailboxMCB.totalInstCnt == 0)
    {
        /* Was the Full HWI registered?  */
        if(gMailboxMCB.hwiHandles.mailboxFull != NULL)
        {
            gMailboxMCB.initParam.osalPrms.disableIntr((uint32_t)(hwCfg)->boxFullIntNum);

            /* YES: Delete and unregister the interrupt handler. */
            gMailboxMCB.initParam.osalPrms.unRegisterIntr(gMailboxMCB.hwiHandles.mailboxFull,
                                                          (uint32_t)(hwCfg)->boxFullIntNum);
            gMailboxMCB.hwiHandles.mailboxFull = NULL;
            gMailboxMCB.errCnt.mailboxFull = 0;
        }

        /* Was the Empty HWI registered?  */
        if(gMailboxMCB.hwiHandles.mailboxEmpty != NULL)
        {
            gMailboxMCB.initParam.osalPrms.disableIntr((uint32_t)(hwCfg)->boxEmptyIntNum);

            /* YES: Delete and unregister the interrupt handler. */
            gMailboxMCB.initParam.osalPrms.unRegisterIntr(gMailboxMCB.hwiHandles.mailboxEmpty,
                                                          (uint32_t)(hwCfg)->boxEmptyIntNum);
            gMailboxMCB.hwiHandles.mailboxEmpty = NULL;
            gMailboxMCB.errCnt.mailboxEmpty = 0;
        }
    }

    return retVal;
}

/*
 *  ======== Mailbox_read ========
 */
int32_t Mailbox_read(Mbox_Handle handle, uint8_t *buffer, uint32_t size)
{
    Mailbox_Driver*     driver;
    Mailbox_HwCfg*      hwCfg  = NULL;
    int32_t             retVal = 0;

    driver = (Mailbox_Driver*)handle;

    /* Sanity Check: Validate the arguments */
    if((size == 0) || (buffer == NULL) || (handle == NULL))
    {
        /* Error: Invalid Arguments */
        DebugP_log3 ("MAILBOX: Mailbox_read Error! Invalid param. Size=%d Buffer=(%p) handle=(%p)\n", size, (uintptr_t)buffer, (uintptr_t)handle);
        retVal = MAILBOX_EINVAL;
    }
    else
    {
        hwCfg = (Mailbox_HwCfg *)driver->hwCfg;

        /* If the size is bigger than maximum mailbox buffer size, read/copy the max available size */
        if(size + driver->numBytesRead > MAILBOX_DATA_BUFFER_SIZE)
        {
            size = MAILBOX_DATA_BUFFER_SIZE - driver->numBytesRead;
        }

        /* First check if this is the first message because it will only block if this is a first message */
        if(driver->numBytesRead == 0)
        {
            /* If mailbox read mode is "blocking", need to pend on semaphore*/
            if(driver->cfg.readMode == MAILBOX_MODE_BLOCKING)
            {
                /* Pend on semaphore until message arrives in mailbox ("mailbox_full" interrupt is received)*/
                retVal = gMailboxMCB.initParam.osalPrms.lockMutex(driver->readSem, driver->cfg.readTimeout);
                if(retVal == MAILBOX_ETIMEOUT)
                {
                    /* Set error code */
                    retVal = MAILBOX_EREADTIMEDOUT;

                    /* Report the error condition: */
                    DebugP_log2 ("MAILBOX:(%p) Mailbox_read timed out. Number of RX messages = %d.\n",
                                 (uintptr_t)driver, driver->rxCount);
                }
            }

            /* In polling or callback mode the newMessageFlag indicates that a message was received.
               In blocking mode, a posted semaphore indicates that a new message was received.
               If any of these are true, the driver should read the new message.*/
            if( ((driver->cfg.readMode == MAILBOX_MODE_POLLING)  && (driver->newMessageFlag == MAILBOX_NEW_MESSAGE_RECEIVED)) ||
                ((driver->cfg.readMode == MAILBOX_MODE_CALLBACK) && (driver->newMessageFlag == MAILBOX_NEW_MESSAGE_RECEIVED)) ||
                ((driver->cfg.readMode == MAILBOX_MODE_BLOCKING) && (retVal != MAILBOX_EREADTIMEDOUT)) )
            {
                driver->newMessageFlag = MAILBOX_NEW_MESSAGE_NOT_RECEIVED;
                /* Increment RX count */
                driver->rxCount++;

                /* Copy data from mailbox buffer into application buffer */
                if(driver->cfg.dataTransferMode == MAILBOX_DATA_TRANSFER_MEMCPY)
                {
                    if(driver->cfg.chType == MAILBOX_CHTYPE_MULTI)
                    {
                        /*Read message. Need to account for internal header*/
                        memcpy((void *)buffer, (void *)((uint8_t *)((hwCfg)->baseRemoteToLocal.data) + driver->numBytesRead + MAILBOX_MULTI_CH_HEADER_SIZE), size);
                    }
                    else
                    {
                        /*Read message.*/
                        memcpy((void *)buffer, (void *)((uint8_t *)((hwCfg)->baseRemoteToLocal.data) + driver->numBytesRead), size);
                    }
                }
                else
                {
                    DebugP_log0("MAILBOX: Mailbox_read Error! Only memcpy dataTransferMode is supported\n");
                    retVal = MAILBOX_EINVALCFG;
                    goto exit;
                }

                /* Set return value */
                retVal = (int32_t)size;

                /* Update number of bytes read for this message*/
                driver->numBytesRead = driver->numBytesRead + size;
            }
        }
        else
        {
            /* This is a subsequent read for a message that is already in the mailbox buffer */
            /* Copy data from mailbox buffer into application buffer */
            if(driver->cfg.dataTransferMode == MAILBOX_DATA_TRANSFER_MEMCPY)
            {
                if(driver->cfg.chType == MAILBOX_CHTYPE_MULTI)
                {
                    /*Read message. Need to account for internal header*/
                    memcpy((void *)buffer, (void *)((uint8_t *)((hwCfg)->baseRemoteToLocal.data) + driver->numBytesRead + MAILBOX_MULTI_CH_HEADER_SIZE), size);
                }
                else
                {
                    /*Read message.*/
                    memcpy((void *)buffer, (void *)((uint8_t *)((hwCfg)->baseRemoteToLocal.data) + driver->numBytesRead), size);
                }
            }
            else
            {
                DebugP_log0("MAILBOX: Mailbox_read Error! Only memcpy dataTransferMode is supported\n");
                retVal = MAILBOX_EINVALCFG;
                goto exit;
            }

            /* Set return value */
            retVal = (int32_t)size;

            /* Update number of bytes read for this message*/
            driver->numBytesRead = driver->numBytesRead + size;
        }
    }

exit:
    return retVal;
}

/*
 *  ======== Mailbox_readFlush ========
 */
int32_t Mailbox_readFlush(Mbox_Handle handle)
{
    Mailbox_Driver*     driver;
    Mailbox_HwCfg*      hwCfg  = NULL;
    int32_t             retVal = 0;
    int32_t             key;

    /* Sanity Check: Validate the arguments */
    if(handle == NULL)
    {
        /* Error: Invalid Arguments */
        DebugP_log0("MAILBOX: Mailbox_readFlush Error! Null handle\n");
        retVal = MAILBOX_EINVAL;
    }
    else
    {
        driver = (Mailbox_Driver*)handle;
        hwCfg = (Mailbox_HwCfg*)driver->hwCfg;

        /* If this is multi-channel, make sure mailbox is not being used by another instance */
        if(driver->cfg.chType == MAILBOX_CHTYPE_MULTI)
        {
            /* Critical Section Protection*/
            key = gMailboxMCB.initParam.osalPrms.disableAllIntr();
            if(driver->remoteCfgPtr->readChIDInUse != driver->cfg.chId)
            {
                retVal = MAILBOX_ECHINUSE;
                /* Release the critical section: */
                gMailboxMCB.initParam.osalPrms.restoreAllIntr(key);
                goto exit;
            }
            else
            {
                /* Indicate that channel is no longer in use */
                driver->remoteCfgPtr->readChIDInUse = MAILBOX_UNUSED_CHANNEL_ID;
                /* Release the critical section: */
                gMailboxMCB.initParam.osalPrms.restoreAllIntr(key);
            }
        }

        driver->readFlushCount++;

        /* Reset number of bytes read*/
        driver->numBytesRead = 0;

        if(hwCfg != NULL)
        {
            /* Send acknowledgement to remote endpoint */
            CSL_Mbox_triggerAckInterrupt(hwCfg->mbxReg, hwCfg->remoteProcNum);
        }
        else
        {
            /* Error: Invalid Arguments */
            DebugP_log0("MAILBOX: Mailbox_readFlush Error! Null hardware configuration.\n");
            retVal = MAILBOX_EINVAL;
        }
    }

exit:
    return retVal;
}

/*
 *  ======== Mailbox_write ========
 */
int32_t Mailbox_write(Mbox_Handle handle, const uint8_t *buffer, uint32_t size)
{
    Mailbox_Driver*     driver;
    Mailbox_HwCfg*      hwCfg = NULL;
    int32_t             retVal = 0;
    uint32_t            header = 0;
    int32_t             key;

    driver = (Mailbox_Driver*)handle;

    /* Sanity Check: Validate the arguments */
    if((size == 0) || (size > MAILBOX_DATA_BUFFER_SIZE) || (buffer == NULL) || (handle == NULL) || (driver->hwCfg == NULL))
    {
        /* Error: Invalid Arguments */
        DebugP_log4 ("MAILBOX: Mailbox_write Error! Invalid param. Size=%d Buffer=(%p) handle=(%p) hwCfgPtr=(%p)\n",
                     size, (uintptr_t)buffer, (uintptr_t)handle, ((handle) ? (uintptr_t)driver->hwCfg : NULL));
        retVal = MAILBOX_EINVAL;
        goto exit;
    }

    hwCfg = (Mailbox_HwCfg *)driver->hwCfg;

    if(driver->cfg.chType == MAILBOX_CHTYPE_MULTI)
    {
        /* Critical Section Protection*/
        key = gMailboxMCB.initParam.osalPrms.disableAllIntr();
        if(driver->remoteCfgPtr->writeChIDInUse == MAILBOX_UNUSED_CHANNEL_ID)
        {
            /* Mark that the TX mailbox is now in use*/
            driver->remoteCfgPtr->writeChIDInUse = driver->cfg.chId;
            /* Release the critical section: */
            gMailboxMCB.initParam.osalPrms.restoreAllIntr(key);
        }
        else
        {
            /* Error: TX mailbox is being used by another mailbox instance*/
            DebugP_log2 ("MAILBOX: Mailbox_write Error! handle=(%p). Write attempt with TX box in use by channel ID %d\n",(uintptr_t)driver, driver->remoteCfgPtr->writeChIDInUse);
            retVal = MAILBOX_ECHINUSE;
            /* Release the critical section: */
            gMailboxMCB.initParam.osalPrms.restoreAllIntr(key);
            goto exit;
        }
    }

    if(driver->txBoxStatus == MAILBOX_TX_BOX_FULL)
    {
        /* Error: TX mailbox is full, can not write new message until acknowledge is received from remote endpoint */
        /* Note that this should take care that the DMA has been completed as well because this flag is cleaned only after
           copy is done */
        DebugP_log1("MAILBOX: Mailbox_write Error! handle=(%p). Write attempt with txBoxStatus == MAILBOX_TX_BOX_FULL\n", (uintptr_t)handle);
        retVal = MAILBOX_ETXFULL;
        goto exit;
    }

    /* A write operation is starting, need to set TXbox flag to full to block any other write to this instance of mailbox*/
    driver->txBoxStatus = MAILBOX_TX_BOX_FULL;

    /* Copy data from application buffer to mailbox buffer */
    if(driver->cfg.dataTransferMode == MAILBOX_DATA_TRANSFER_MEMCPY)
    {
        if(driver->cfg.chType == MAILBOX_CHTYPE_MULTI)
        {
            /*Write internal header*/
            header = driver->cfg.chId;
            memcpy((void *)(hwCfg)->baseLocalToRemote.data, (void *)(&header), sizeof(header));
            /*Write message. Need to account for internal header size*/
            memcpy((void *)((uint8_t *)((hwCfg)->baseLocalToRemote.data) + MAILBOX_MULTI_CH_HEADER_SIZE), (const void *)buffer, size);
        }
        else
        {
            /*Write message.*/
            memcpy((void *)(hwCfg)->baseLocalToRemote.data, (const void *)buffer, size);
        }

    }
    else
    {
        DebugP_log0("MAILBOX: Mailbox_write Error! Only memcpy dataTransferMode is supported\n");
        retVal = MAILBOX_EINVALCFG;
        goto exit;
    }

    /* Store handle of instance */
    driver->remoteCfgPtr->lastMsgSentHandle = handle;

    /* Trigger "mailbox full" interrupt to remote endpoint*/
    CSL_Mbox_triggerTxInterrupt(hwCfg->mbxReg, hwCfg->remoteProcNum);

    /* Next action depends on the mailbox write mode*/
    if(driver->cfg.writeMode == MAILBOX_MODE_BLOCKING)
    {
        /* Pend on semaphore until acknowledge ("mailbox_empty" interrupt) from remote endpoint is received*/
        retVal = gMailboxMCB.initParam.osalPrms.lockMutex(driver->writeSem, driver->cfg.writeTimeout);
        if(retVal == MAILBOX_ETIMEOUT)
        {
            /* Set error code */
            retVal = MAILBOX_ETXACKTIMEDOUT;

            /* Report the error condition: */
            DebugP_log2 ("MAILBOX:(%p) Write acknowledge timed out. Ack was never received. Number of received TX messages = %d.\n",
                         (uintptr_t)driver, driver->txCount);
            /* Reset the driver status. */
            /* Critical Section Protection*/
            key = gMailboxMCB.initParam.osalPrms.disableAllIntr();

            /* update txBox status flag */
            driver->txBoxStatus = MAILBOX_TX_BOX_EMPTY;

            /* update TX box multichannel status */
            if(driver->cfg.chType == MAILBOX_CHTYPE_MULTI)
            {
                driver->remoteCfgPtr->writeChIDInUse = MAILBOX_UNUSED_CHANNEL_ID;
            }
            gMailboxMCB.initParam.osalPrms.restoreAllIntr(key);
        }
    }

    /* If write is blocking mode and semaphore did not timeout, write succeeded and ack received.
       If write is polling mode and we reached this point, write was done but not sure if ack has been received. */
    if( ((driver->cfg.writeMode == MAILBOX_MODE_BLOCKING) && (retVal != MAILBOX_ETXACKTIMEDOUT)) ||
        (driver->cfg.writeMode == MAILBOX_MODE_POLLING) )
    {
        /* Increment TX count */
        driver->txCount++;

        /* Set return value */
        retVal = (int32_t)size;
    }

exit:
    if(retVal == MAILBOX_ETXFULL)
    {
        Mailbox_writeReset(handle);
    }
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered ISR for the "mailbox full" interrupt.
 *
 *  @param[in]  driver
 *      Driver handle
 *
 *  @retval
 *      Not applicable
 *
 *  \ingroup MAILBOX_DRIVER_INTERNAL_FUNCTION
 */
static void Mailbox_boxFullISRProcessing(Mailbox_Driver* driver)
{
    Mailbox_HwCfg *hwCfg = NULL;

    if(driver != NULL)
    {
        hwCfg = (Mailbox_HwCfg *)driver->hwCfg;

        driver->boxFullIsrCount++;

        /* set flag to inform that new message received */
        driver->newMessageFlag = MAILBOX_NEW_MESSAGE_RECEIVED;

        /* Action depends on the read mode */
        if(driver->cfg.readMode == MAILBOX_MODE_POLLING)
        {
            /* Do nothing. Flag set above is enough */
        }
        else if(driver->cfg.readMode == MAILBOX_MODE_BLOCKING)
        {
            /* Post semaphore */
            if(driver->readSem)
            {
                /* Post the semaphore to unblock calling thread. */
                gMailboxMCB.initParam.osalPrms.unlockMutex(driver->readSem, true);
            }
        }
        else /* Call back read mode */
        {
            /* Check if call back function is implemented */
            if(driver->cfg.readCallback != NULL)
            {
                (*(driver->cfg.readCallback))(driver, driver->remoteEndpoint);
            }
        }

        /* Clear the status register */
        if(hwCfg != NULL)
        {
            CSL_Mbox_clearBoxFullInterrupt(hwCfg->mbxReg, hwCfg->remoteProcNum);
        }
    }
}

/**
 *  @b Description
 *  @n
 *      The function is the registered ISR for the "mailbox full" interrupt.
 *
 *  @param[in]  arg
 *      Argument which is registered with the OS while registering
 *      the ISR
 *
 *  @retval
 *      Not applicable
 *
 *  \ingroup MAILBOX_DRIVER_INTERNAL_FUNCTION
 */
static void Mailbox_boxFullISR(uintptr_t arg)
{
    Mailbox_HwCfg    *hwCfg = NULL;
    uint32_t         header;
    uint8_t          id = MAILBOX_UNUSED_CHANNEL_ID;
    Mailbox_Instance     remoteEndpoint;
    int32_t          retVal;
    Mailbox_RemoteCfg *remoteCfg;
    uint32_t            i;

    retVal = Mailbox_getBoxFullRemoteInst(&remoteEndpoint);

    if (retVal == MAILBOX_SOK)
    {
        /* Get the remote Cfg */
        for (i=0; i<MAILBOX_MAX_NUM_REMOTES_ENDPOINTS; i++)
        {
            if (gMailboxMCB.remoteConfig[i].remoteEndpoint == remoteEndpoint)
            {
                break;
            }
        }
        if (i != MAILBOX_MAX_NUM_REMOTES_ENDPOINTS)
        {
            remoteCfg = &gMailboxMCB.remoteConfig[i];
            if (remoteCfg->chType == MAILBOX_CHTYPE_SINGLE)
            {
                Mailbox_Driver *driver = (Mailbox_Driver*) remoteCfg->handleArray[0];
                if (driver == NULL)
                {
                    /* driver not opened. Clear the interrupt. */
                    CSL_Mbox_clearBoxFullInterrupt(&gMboxReg, remoteEndpoint);
                }
                else
                {
                    Mailbox_boxFullISRProcessing(driver);
                }
            }
            else
            {
                /* Get the Channel Id from the header. */
                /* First need to find the ID in the received message*/
                if(remoteCfg->hwCfgPtr != NULL)
                {
                    hwCfg = (Mailbox_HwCfg *)remoteCfg->hwCfgPtr;
                    memcpy((void *)&header, (void *)(hwCfg->baseRemoteToLocal.data), sizeof(header));
                    id = (uint8_t)(header & 0x7U);
                    if(id > MAILBOX_CH_ID_MAX)
                    {
                        /*error*/
                        gMailboxMCB.errCnt.mailboxFull++;
                        /* clear the interrupt. */
                        CSL_Mbox_clearBoxFullInterrupt(&gMboxReg, remoteEndpoint);
                    }
                    else
                    {
                        /* get the driver handle for particular channel. */
                        Mailbox_Driver* driver = (Mailbox_Driver*) remoteCfg->handleArray[id];
                        if (driver == NULL)
                        {
                            /* driver not opened. Clear the interrupt. */
                            CSL_Mbox_clearBoxFullInterrupt(&gMboxReg, remoteEndpoint);
                        }
                        else
                        {
                            driver->remoteCfgPtr->readChIDInUse = driver->cfg.chId;
                            Mailbox_boxFullISRProcessing(driver);
                        }
                    }
                }
                else
                {
                    /* driver not opened for any channel. Clear the interrupt. */
                    CSL_Mbox_clearBoxFullInterrupt(&gMboxReg, remoteEndpoint);
                }
            }
        }
        else
        {
            /* Spurious interrupt, Not registered remote proc. Disable it. */
            gMailboxMCB.errCnt.mailboxFull++;
            /* driver not opened for any channel. Clear the interrupt. */
            CSL_Mbox_clearBoxFullInterrupt(&gMboxReg, remoteEndpoint);
        }
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is the registered ISR for the "mailbox empty" interrupt.
 *      This interrupt indicates that an acknowledge for the last write operation was received.
 *
 *  @param[in]  driver
 *      Driver handle.
 *
 *  @retval
 *      Not applicable
 *
 *  \ingroup MAILBOX_DRIVER_INTERNAL_FUNCTION
 */
static void Mailbox_boxEmptyISRProcessing(Mailbox_Driver* driver)
{
    Mailbox_HwCfg *hwCfg = NULL;

    if(driver != NULL)
    {
        hwCfg = (Mailbox_HwCfg *)driver->hwCfg;

        /* Acknowldedgement from remote endpoint has been received */
        /* Local endpoint clears the "mailbox empty" interrupt */
        if(hwCfg != NULL)
        {
            CSL_Mbox_clearTxAckInterrupt(hwCfg->mbxReg, hwCfg->remoteProcNum);
        }

        driver->boxEmptyIsrCount++;

        /* update txBox status flag */
        driver->txBoxStatus = MAILBOX_TX_BOX_EMPTY;

        /* update TX box multichannel status */
        if(driver->cfg.chType == MAILBOX_CHTYPE_MULTI)
        {
            driver->remoteCfgPtr->writeChIDInUse = MAILBOX_UNUSED_CHANNEL_ID;
        }

        /* Action depends on the write mode */
        if(driver->cfg.writeMode == MAILBOX_MODE_BLOCKING)
        {
            if(driver->writeSem)
            {
                /* Post the semaphore to unblock calling thread. */
                gMailboxMCB.initParam.osalPrms.unlockMutex(driver->writeSem, true);
            }
        }
    }
}

/**
 *  @b Description
 *  @n
 *      The function is the registered ISR for the "mailbox empty" interrupt.
 *      This interrupt indicates that an acknowledge for the last write operation was received.
 *
 *  @param[in]  arg
 *      Argument which is registered with the OS while registering
 *      the ISR
 *
 *  @retval
 *      Not applicable
 *
 *  \ingroup MAILBOX_DRIVER_INTERNAL_FUNCTION
 */
static void Mailbox_boxEmptyISR(uintptr_t arg)
{
    Mailbox_Instance     remoteEndpoint;
    int32_t          retVal;
    Mailbox_RemoteCfg *remoteCfg;
    uint32_t            i;

    retVal = Mailbox_getBoxEmptyRemoteInst(&remoteEndpoint);

    if (retVal == MAILBOX_SOK)
    {
        /* Get the remote Cfg */
        for (i=0; i<MAILBOX_MAX_NUM_REMOTES_ENDPOINTS; i++)
        {
            if (gMailboxMCB.remoteConfig[i].remoteEndpoint == remoteEndpoint)
            {
                break;
            }
        }
        if (i != MAILBOX_MAX_NUM_REMOTES_ENDPOINTS)
        {
            remoteCfg = &gMailboxMCB.remoteConfig[i];
            if (remoteCfg->lastMsgSentHandle != NULL)
            {
                Mailbox_boxEmptyISRProcessing((Mailbox_Driver*) remoteCfg->lastMsgSentHandle);
            }
            else
            {
                /* TODO: Spurious interrupt, clear it. */
                gMailboxMCB.errCnt.mailboxEmpty++;
            }
        }
        else
        {
            /* TODO: Spurious interrupt, Not registered remote proc. Disable it. */
            gMailboxMCB.errCnt.mailboxFull++;
        }
    }
    return;
}

static int32_t Mailbox_validateLocalEndPointRxMem()
{
    int32_t retVal = MAILBOX_SOK;
    #if defined (BUILD_MCU1_0) || defined (BUILD_DSP_1)
    uint32_t endAddress = 0ul;
    uint32_t startAddress = 0ul;
    #endif
    volatile uint32_t rssCtrlBootInfoReg3 = HW_RD_REG32((CSL_RSS_PROC_CTRL_U_BASE + CSL_RSS_PROC_CTRL_RSS_CR4_BOOT_INFO_REG3));
    uint32_t mssOffsetIndex = HW_GET_FIELD(rssCtrlBootInfoReg3, CSL_RSS_PROC_CTRL_RSS_CR4_BOOT_INFO_REG3_MSS_OFFSET_INDEX);
    uint32_t mssBuffSize    = HW_GET_FIELD(rssCtrlBootInfoReg3, CSL_RSS_PROC_CTRL_RSS_CR4_BOOT_INFO_REG3_MSS_BUFF_SIZE);
    uint32_t dssOffsetIndex = HW_GET_FIELD(rssCtrlBootInfoReg3, CSL_RSS_PROC_CTRL_RSS_CR4_BOOT_INFO_REG3_DSS_OFFSET_INDEX);
    uint32_t dssBuffSize    = HW_GET_FIELD(rssCtrlBootInfoReg3, CSL_RSS_PROC_CTRL_RSS_CR4_BOOT_INFO_REG3_DSS_BUFF_SIZE);

    /* Validate local End point Mailbox size and offset. */
    if ((mssBuffSize <= MAILBOX_2048B) && (dssBuffSize <= MAILBOX_2048B))
    {
        #if defined (BUILD_MCU1_0)
        startAddress = CSL_MSS_MBOX_U_BASE;
        #elif defined (BUILD_DSP_1)
        startAddress = CSL_DSS_MAILBOX_U_BASE;
        #endif

        #if defined (BUILD_MCU1_0)
        endAddress = startAddress + mssBuffSize * (1 + mssOffsetIndex);
        if(endAddress > (CSL_MSS_MBOX_U_BASE + CSL_MSS_MAILBOX_U_SIZE))
        {
            retVal = MAILBOX_EINVAL;
        }
        #elif defined (BUILD_DSP_1)
        endAddress = startAddress + dssBuffSize * (1 + dssOffsetIndex);
        if(endAddress > (CSL_DSS_MAILBOX_U_BASE + CSL_DSS_MAILBOX_U_SIZE))
        {
            retVal = MAILBOX_EINVAL;
        }
        #endif
    }
    else
    {
        retVal = MAILBOX_EINVAL;
    }

    return retVal;
}

/*
 *  ======== Mailbox_writeReset ========
 *  Used when a write is never acknowledged by the receiver,
 *  do a writeReset so a new message can be sent.
 */
 
static int32_t Mailbox_writeReset(Mbox_Handle handle)
{
    Mailbox_Driver*     driver;
    int32_t             retVal = 0;
    Mailbox_HwCfg       *hwCfg = NULL;

    driver = (Mailbox_Driver*)handle;

    /* Sanity Check: Validate the arguments */
    if ((handle == NULL) || (driver->hwCfg == NULL))
    {
        /* Error: Invalid Arguments */
        DebugP_log0 ("MAILBOX: Mailbox_writeReset Error! Null handle");
        retVal = MAILBOX_EINVAL;
    }
    else
    {
        hwCfg = (Mailbox_HwCfg *)driver->hwCfg;

        /* make sure "mailbox empty" interrupt is clear */
        CSL_Mbox_clearTxAckInterrupt(hwCfg->mbxReg, hwCfg->remoteProcNum);

        /* Reset mailbox TX status to empty so a new message can be sent*/
        driver->txBoxStatus = MAILBOX_TX_BOX_EMPTY;
    }

    return retVal;
}

uint64_t Mailbox_defaultVirtToPhyFxn(const void *virtAddr)
{
    return ((uint64_t) virtAddr);
}

void *Mailbox_defaultPhyToVirtFxn(uint64_t phyAddr)
{
#if defined (__aarch64__)
    uint64_t temp = phyAddr;
#else
    /* R5 is 32-bit machine, need to truncate to avoid void * typecast error */
    uint32_t temp = (uint32_t) phyAddr;
#endif

    return ((void *) temp);
}

