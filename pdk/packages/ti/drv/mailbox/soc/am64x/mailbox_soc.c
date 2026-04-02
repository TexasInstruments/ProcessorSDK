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
#include <ti/csl/soc.h>
#include <ti/csl/csl_types.h>
#include <ti/drv/mailbox/mailbox.h>
#include <ti/drv/mailbox/src/mailbox_internal.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/*!
 * @brief
 *  Mailbox Entry Information
 *
 *  This structure is used to store the cluser/user/fifo information used for communication between a core pair
 *
 *  \ingroup MAILBOX_DRIVER_INTERNAL_DATA_STRUCTURE
 */
typedef struct Mailbox_Entry_s
{
    /**
     * @brief  Cluster id.
     */
    uint32_t    cluster;

    /**
     * @brief  User id.
     */
    uint32_t    user;

    /**
     * @brief  Fifo id.
     */
    uint32_t    fifo;

} Mailbox_Entry;

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
    /**
     * @brief  Entry for the mailbox info for receiving messages
     */
    Mailbox_Entry    rx;
    /**
     * @brief  Entry for the mailbox info for sending messages
     */
    Mailbox_Entry    tx;
    /**
     * @brief  Flag to indicate if there is exclusive access to the mailbox
     *         for receiving messages from a single remote core. For example, if
     *         the interrupt line for mailbox cluster 0 is only used for
     *         receiving messages from a single remote core. Some optimiation
     *         can be done when handling the incoming message in this case.
     */
    bool             exclusive;
    /**
     * @brief  The associated eventId for the interrupt.
     */
    uint32_t         eventId;
} Mailbox_HwCfg;


/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

#define  MAILBOX_MAXFIFO_CNT    (16U)
#define  MAILBOX_CLUSTER_CNT     (8U)
#define  MAILBOX_USER_CNT        (4U)

/**
 * @brief
 *  Global Variable for tracking information required by the mailbox driver.
 */
extern Mailbox_MCB gMailboxMCB;

/**
 * \brief Maps mBoxData allocated to a given Remote Processor
 */
uintptr_t           gInstToMBoxDataMap[MAILBOX_MAX_INST];

typedef struct Mailbox_Fifo_s
{
    int32_t                 cfgNdx;
    Mailbox_Callback        func;
    uint32_t                arg;
    uint32_t                queueId;
    void                   *handle;
} Mailbox_Fifo;

/* mboxData */
typedef struct Mailbox_Data_s
{
    uint32_t          baseAddr;
    uint32_t          fifoCnt;
    Mailbox_Fifo      fifoTable[MAILBOX_MAXFIFO_CNT];
    uint32_t          noMsgCnt;
    uint32_t          intCnt;
    uint32_t          userId;

} Mailbox_Data;

/* ========================================================================== */
/*                             Globals                                        */
/* ========================================================================== */
uint32_t               g_mBoxCnt = 0U;
Mailbox_Data           g_mBoxData[MAILBOX_MAX_INST];
Mailbox_Callback       g_VimCallback[MAILBOX_MAX_INST];
Mbox_Handle            g_VimCallbackArg[MAILBOX_MAX_INST];
Mailbox_Callback       g_FastCallback[MAILBOX_MAX_INST];
Mbox_Handle            g_FastCallbackArg[MAILBOX_MAX_INST];
Mailbox_Driver         g_mBoxDrivers[MAILBOX_MAX_INST];

/* Mailbox Cluster Base Address */
static uint32_t  g_Mailbox_BaseAddr[MAILBOX_CLUSTER_CNT] =
{
    CSL_MAILBOX0_REGS0_BASE,
    CSL_MAILBOX0_REGS1_BASE,
    CSL_MAILBOX0_REGS2_BASE,
    CSL_MAILBOX0_REGS3_BASE,
    CSL_MAILBOX0_REGS4_BASE,
    CSL_MAILBOX0_REGS5_BASE,
    CSL_MAILBOX0_REGS6_BASE,
    CSL_MAILBOX0_REGS7_BASE,
};

Mailbox_HwCfg g_Mailbox_HwCfg[MAILBOX_MAX_INST][MAILBOX_MAX_INST] =
{
    /* Host Processor - A53-vm0 */
    {
        { { 0xFFU, 0xFFU,  0U}, { 0xFFU, 0xFFU,  0U}, true, 0 },  /* Self - A53-vm0 */
        { {    2U,    2U,  0U}, {    2U,    2U,  1U}, false, 0 },  /* mcu-r5f0 */
        { {    2U,    2U,  2U}, {    2U,    2U,  3U}, false, 0 },  /* mcu-r5f1 */
        { {    4U,    2U,  0U}, {    4U,    2U,  1U}, false, 0 },  /* pulsar1_cr5f_0 */
        { {    4U,    2U,  2U}, {    4U,    2U,  3U}, false, 0 },  /* pulsar1_cr5f_1 */
        { {    6U,    2U,  0U}, {    6U,    2U,  1U}, true, 0 },  /* m4f_0 */
    },
    /* Host Processor - mcu1_0  */
    {
        { {    2U,    0U,  1U }, {    2U,    0U,  0U}, true, 0 },  /* A53-vm0 */
        { { 0xFFU, 0xFFU,  0U }, { 0xFFU, 0xFFU,  0U}, true, 0 },  /* Self - mcu-r5f0 */
        { {    0U,    0U,  0U }, {    0U, 0xFFU,  1U}, true, 0 },  /* mcu-r5f1 */
        { {    1U,    0U,  0U }, {    0U, 0xFFU,  2U}, true, 0 },  /* pulsar1_cr5f_0 */
        { {    3U,    0U,  0U }, {    0U, 0xFFU,  3U}, true, 0 },  /* pulsar1_cr5f_1 */
        { {    6U,    0U,  2U }, {    6U, 0xFFU,  3U}, true, 0 },  /* m4f_0 */
    },
    /* Host Processor - mcu1_1 */
    {
        { {    2U,    1U,  3U }, {    2U,    1U,  2U}, true, 0 },  /* A53-vm0 */
        { {    0U,    1U,  1U }, {    0U, 0xFFU,  0U}, true, 0 },  /* mcu-r5f0 */
        { { 0xFFU, 0xFFU,  0U }, { 0xFFU, 0xFFU,  0U}, true, 0 },  /* Self - mcu-r5f1 */
        { {    1U,    1U,  1U }, {    1U, 0xFFU,  2U}, true, 0 },  /* pulsar1_cr5f_0 */
        { {    3U,    1U,  1U }, {    1U, 0xFFU,  3U}, true, 0 },  /* pulsar1_cr5F_1 */
        { {    6U,    1U,  4U }, {    6U, 0xFFU,  5U}, true, 0 },  /* m4f_0 */
    },
    /* Host Processor - pulsar1_cr5f_0 */
    {
        { {    4U,    0U,  1U }, {    4U,    0U,  0U}, true, 0 },  /* A53-vm0 */
        { {    0U,    2U,  2U }, {    1U, 0xFFU,  0U}, true, 0 },  /* mcu-r5f0 */
        { {    1U,    2U,  2U }, {    1U, 0xFFU,  1U}, true, 0 },  /* mcu-r5f1 */
        { { 0xFFU, 0xFFU,  0U }, { 0xFFU, 0xFFU,  0U}, true, 0 },  /* Self - pulsar1_cr5f_0 */
        { {    5U,    0U,  2U }, {    5U, 0xFFU,  3U}, true, 0 },  /* pulsar1_cr5f_1 */
        { {    7U,    0U,  2U }, {    7U, 0xFFU,  4U}, true, 0 },  /* m4f_0 */
    },
    /* Host Processor - pulsar1_cr5f_1 */
    {
        { {    4U,    1U,  3U }, {    4U,    1U,  2U}, true, 0 },  /* A53-vm0 */
        { {    0U,    3U,  3U }, {    3U, 0xFFU,  0U}, true, 0 },  /* mcu-r5f0 */
        { {    1U,    3U,  3U }, {    3U, 0xFFU,  1U}, true, 0 },  /* mcu-r5f1 */
        { {    5U,    1U,  3U }, {    5U, 0xFFU,  2U}, true, 0 },  /* pulsar1_cr5f_0 */
        { { 0xFFU, 0xFFU,  0U }, { 0xFFU, 0xFFU,  0U}, true, 0 },  /* Self - pulsar1_cr5f_1 */
        { {    7U,    1U,  3U }, {    7U, 0xFFU,  5U}, true, 0 },  /* m4f_0 */
    },
    /* Host Processor - m4f_0 */
    {
        { {    6U,    3U,  1U }, {    6U,    3U,  0U}, false, 0 }, /* A53-vm0 */
        { {    6U,    3U,  3U }, {    6U, 0xFFU,  2U}, false, 0 }, /* mcu-r5f0 */
        { {    6U,    3U,  5U }, {    6U, 0xFFU,  4U}, false, 0 }, /* mcu-r5f1 */
        { {    7U,    3U,  4U }, {    7U, 0xFFU,  2U}, false, 0 }, /* pulsar1_cr5f_0 */
        { {    7U,    3U,  5U }, {    7U, 0xFFU,  3U}, false, 0 }, /* pulsar1_cr5f_1 */
        { { 0xFFU, 0xFFU,  0U }, { 0xFFU, 0xFFU,  0U}, false, 0 }, /* Self - m4f_0 */
    }
};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

Mailbox_Instance Mailbox_getLocalEndPoint(void)
{
    Mailbox_Instance localEndpoint = MAILBOX_INST_INVALID;

#if defined (BUILD_MPU1_0)
    localEndpoint = MAILBOX_INST_MPU1_0;
#elif defined (BUILD_MCU1_0)
    localEndpoint = MAILBOX_INST_MCU1_0;
#elif defined (BUILD_MCU1_1)
    localEndpoint = MAILBOX_INST_MCU1_1;
#elif defined (BUILD_MCU2_0)
    localEndpoint = MAILBOX_INST_MCU2_0;
#elif defined (BUILD_MCU2_1)
    localEndpoint = MAILBOX_INST_MCU2_1;
#elif defined (BUILD_M4F_0)
    localEndpoint = MAILBOX_INST_M4F_0;
#endif

    return localEndpoint;
}

int32_t Mailbox_validateLocalEndPoint(Mailbox_Instance localEndpoint)
{
    int32_t retVal = MAILBOX_SOK;
    /* Validate local End point based on the Core. */
#if defined (BUILD_MPU1_0)
    if (localEndpoint != MAILBOX_INST_MPU1_0)
    {
        retVal = MAILBOX_EINVAL;
    }
#elif defined (BUILD_MCU1_0)
    if (localEndpoint != MAILBOX_INST_MCU1_0)
    {
        retVal = MAILBOX_EINVAL;
    }
#elif defined (BUILD_MCU1_1)
    if (localEndpoint != MAILBOX_INST_MCU1_1)
    {
        retVal = MAILBOX_EINVAL;
    }
#elif defined (BUILD_MCU2_0)
    if (localEndpoint != MAILBOX_INST_MCU2_0)
    {
        retVal = MAILBOX_EINVAL;
    }
#elif defined (BUILD_MCU2_1)
    if (localEndpoint != MAILBOX_INST_MCU2_1)
    {
        retVal = MAILBOX_EINVAL;
    }
#elif defined (BUILD_M4F_0)
    if (localEndpoint != MAILBOX_INST_M4F_0)
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
    if ((remoteEndpoint == localEndpoint) || (remoteEndpoint > MAILBOX_INST_LAST))
    {
        retVal = MAILBOX_EINVAL;
    }
    return retVal;
}

int32_t Mailbox_validateDataTransferMode(Mailbox_DataTransferMode dataTransferMode)
{
    /* We ignore data transfer mode for this Mailbox IP. Only way to get data is with register read */
    return MAILBOX_SOK;
}

int32_t Mailbox_validateReadWriteMode(Mailbox_Mode readMode, Mailbox_Mode writeMode)
{
    int32_t retVal = MAILBOX_SOK;
    if ((writeMode == MAILBOX_MODE_CALLBACK) || (writeMode == MAILBOX_MODE_BLOCKING) ||
        (readMode == MAILBOX_MODE_BLOCKING))
    {
        retVal = MAILBOX_EINVAL;
    }
    return retVal;
}

int32_t Mailbox_isMultiChannelSupported(Mailbox_Instance localEndpoint, Mailbox_Instance remoteEndpoint)
{
    /* We do not support Multi-channel for this Mailbox IP */
    return MAILBOX_EINVAL;
}

Mbox_Handle Mailbox_allocDriver(Mailbox_Instance remoteEndpoint)
{
    Mailbox_Driver *driver = NULL;

    driver = &g_mBoxDrivers[remoteEndpoint];

    return (Mbox_Handle)driver;
}

int32_t Mailbox_freeDriver(Mbox_Handle handle)
{
    return MAILBOX_SOK;
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
    Mailbox_Instance localEndpoint = Mailbox_getLocalEndPoint();

    if ((remoteEndpoint < MAILBOX_MAX_INST) && (localEndpoint < MAILBOX_MAX_INST))
    {
        hwCfg = &g_Mailbox_HwCfg[localEndpoint][remoteEndpoint];
    }

    return (void *)hwCfg;
}

int32_t Mailbox_initHw(Mbox_Handle handle)
{
    int32_t             retVal = MAILBOX_SOK;

    if (handle == NULL)
    {
        retVal = MAILBOX_EINVAL;
    }

    if (retVal == MAILBOX_SOK)
    {
        // TODO
    }
    return retVal;
}

uint32_t Mailbox_GetMessageCount(Mbox_Handle handle)
{
    int32_t count = 0;

    if (handle != NULL)
    {
        count = MailboxGetMessageCount(((Mailbox_Driver *)handle)->baseAddrRx,
                                       ((Mailbox_HwCfg *)(((Mailbox_Driver *)handle)->hwCfg))->rx.fifo);
    }
    return count;
}

/*
 *  ======== Mailbox_write ========
 */
int32_t Mailbox_write(Mbox_Handle handle, const uint8_t *buffer, uint32_t size)
{
    int32_t retVal = MAILBOX_SOK;

    if ((handle != NULL) &&
        (((Mailbox_Driver *)handle)->hwCfg != NULL) &&
        (buffer != NULL) &&
        (((Mailbox_Driver *)handle)->cfg.writeMode == MAILBOX_MODE_FAST))
    {
        MailboxWriteMessage(((Mailbox_Driver *)handle)->baseAddrTx,
                            ((Mailbox_HwCfg *)(((Mailbox_Driver *)handle)->hwCfg))->tx.fifo,
                            *((uint32_t *)buffer));
    }
    else
    {
        Mailbox_Driver* driver = (Mailbox_Driver*)handle;
        Mailbox_HwCfg *hwCfg = NULL;
        int32_t key = 0U;
        uint32_t cnt;

        if (handle == NULL)
        {
            retVal = MAILBOX_EINVAL;
        }
        else if (driver->hwCfg == NULL)
        {
            retVal = MAILBOX_EINVAL;
        }
        else if (buffer == NULL)
        {
            retVal = MAILBOX_EINVAL;
        }
        else
        {
            hwCfg = (Mailbox_HwCfg *)driver->hwCfg;
            cnt = driver->cfg.writeTimeout;

            key = gMailboxMCB.initParam.osalPrms.disableAllIntr();

            do
            {
                retVal = MailboxSendMessage(driver->baseAddrTx, hwCfg->tx.fifo, *((uint32_t *)buffer));
                cnt--;
            } while( (cnt != 0U) && (retVal == MESSAGE_INVALID));

            if (MESSAGE_INVALID == retVal)
            {
                retVal = MAILBOX_EINVAL;
            }

            gMailboxMCB.initParam.osalPrms.restoreAllIntr(key);
        }
    }

    return retVal;
}

int32_t Mailbox_read(Mbox_Handle handle, uint8_t *buffer, uint32_t size)
{
   int32_t retVal = MAILBOX_SOK;

    if ((handle != NULL) &&
        (((Mailbox_Driver *)handle)->hwCfg != NULL) &&
        (buffer != NULL) &&
        (((Mailbox_Driver *)handle)->cfg.readMode == MAILBOX_MODE_FAST))
    {
        MailboxReadMessage(((Mailbox_Driver *)handle)->baseAddrRx,
                           ((Mailbox_HwCfg *)(((Mailbox_Driver *)handle)->hwCfg))->rx.fifo,
                           (uint32_t *)buffer);
    }
    else
    {
        Mailbox_Driver* driver = (Mailbox_Driver*)handle;
        Mailbox_HwCfg* hwCfg = NULL;

        if (handle == NULL)
        {
            retVal = MAILBOX_EINVAL;
        }
        else if (driver->hwCfg == NULL)
        {
            retVal = MAILBOX_EINVAL;
        }
        else if (buffer == NULL)
        {
            retVal = MAILBOX_EINVAL;
        }
        else
        {
            hwCfg = (Mailbox_HwCfg *)driver->hwCfg;

            /* Get the message from Mailbox fifo */
            retVal = MailboxGetMessage(driver->baseAddrRx, hwCfg->rx.fifo, (uint32_t *)buffer);
            if (retVal == MESSAGE_INVALID)
            {
                retVal = MAILBOX_EINVAL;
            }
        }
    }

    return retVal;
}

uint32_t Mailbox_getBaseAddr(uint32_t clusterId)
{
    uint32_t baseAddrPhy = 0x00000000U;
    uint32_t baseAddrVirt = 0x00000000U;

    if( clusterId < MAILBOX_CLUSTER_CNT)
    {
        baseAddrPhy = g_Mailbox_BaseAddr[clusterId];
        if (gMailboxMCB.initParam.phyToVirtFxn)
        {
            baseAddrVirt = (uint32_t)gMailboxMCB.initParam.phyToVirtFxn(baseAddrPhy);
        }
        else
        {
            baseAddrVirt = baseAddrPhy;
        }
    }

    return baseAddrVirt;
}

int32_t Mailbox_getMailboxIntrRouterCfg(uint32_t selfId, uint32_t clusterId, uint32_t userId,
            Mailbox_MbConfig* cfg, uint32_t cnt)
{
    int32_t    retVal         = MAILBOX_SOK;

    switch(selfId)
    {
        case MAILBOX_INST_MPU1_0:
            if (clusterId == 2 && userId == 2)
            {
                cfg->eventId = CSLR_GICSS0_SPI_MAILBOX0_MAILBOX_CLUSTER_2_MAILBOX_CLUSTER_PEND_2;
            }
            else if (clusterId == 3 && userId == 2)
            {
                cfg->eventId = CSLR_GICSS0_SPI_MAILBOX0_MAILBOX_CLUSTER_3_MAILBOX_CLUSTER_PEND_2;
            }
            else if (clusterId == 4 && userId == 2)
            {
                cfg->eventId = CSLR_GICSS0_SPI_MAILBOX0_MAILBOX_CLUSTER_4_MAILBOX_CLUSTER_PEND_2;
            }
            else if (clusterId == 5 && userId == 2)
            {
                cfg->eventId = CSLR_GICSS0_SPI_MAILBOX0_MAILBOX_CLUSTER_5_MAILBOX_CLUSTER_PEND_2;
            }
            else if (clusterId == 6 && userId == 2)
            {
                cfg->eventId = CSLR_GICSS0_SPI_MAILBOX0_MAILBOX_CLUSTER_6_MAILBOX_CLUSTER_PEND_2;
            }
            else
            {
                retVal = MAILBOX_EINVAL;
            }
            break;
        case MAILBOX_INST_MCU1_0:
            if (clusterId == 0 && userId == 0)
            {
                cfg->eventId = CSLR_R5FSS0_CORE0_INTR_MAILBOX0_MAILBOX_CLUSTER_0_MAILBOX_CLUSTER_PEND_0;
            }
            else if (clusterId == 1 && userId == 0)
            {
                cfg->eventId = CSLR_R5FSS0_CORE0_INTR_MAILBOX0_MAILBOX_CLUSTER_1_MAILBOX_CLUSTER_PEND_0;
            }
            else if (clusterId == 2 && userId == 0)
            {
                cfg->eventId = CSLR_R5FSS0_CORE0_INTR_MAILBOX0_MAILBOX_CLUSTER_2_MAILBOX_CLUSTER_PEND_0;
            }
            else if (clusterId == 3 && userId == 0)
            {
                cfg->eventId = CSLR_R5FSS0_CORE0_INTR_MAILBOX0_MAILBOX_CLUSTER_3_MAILBOX_CLUSTER_PEND_0;
            }
            else if (clusterId == 6 && userId == 0)
            {
                cfg->eventId = CSLR_R5FSS0_CORE0_INTR_MAILBOX0_MAILBOX_CLUSTER_6_MAILBOX_CLUSTER_PEND_0;
            }
            else
            {
                retVal = MAILBOX_EINVAL;
            }
            break;
        case MAILBOX_INST_MCU1_1:
            if (clusterId == 0 && userId == 1)
            {
                cfg->eventId = CSLR_R5FSS0_CORE1_INTR_MAILBOX0_MAILBOX_CLUSTER_0_MAILBOX_CLUSTER_PEND_1;
            }
            else if (clusterId == 1 && userId == 1)
            {
                cfg->eventId = CSLR_R5FSS0_CORE1_INTR_MAILBOX0_MAILBOX_CLUSTER_1_MAILBOX_CLUSTER_PEND_1;
            }
            else if (clusterId == 2 && userId == 1)
            {
                cfg->eventId = CSLR_R5FSS0_CORE1_INTR_MAILBOX0_MAILBOX_CLUSTER_2_MAILBOX_CLUSTER_PEND_1;
            }
            else if (clusterId == 3 && userId == 1)
            {
                cfg->eventId = CSLR_R5FSS0_CORE1_INTR_MAILBOX0_MAILBOX_CLUSTER_3_MAILBOX_CLUSTER_PEND_1;
            }
            else if (clusterId == 6 && userId == 1)
            {
                cfg->eventId = CSLR_R5FSS0_CORE1_INTR_MAILBOX0_MAILBOX_CLUSTER_6_MAILBOX_CLUSTER_PEND_1;
            }
            else
            {
                retVal = MAILBOX_EINVAL;
            }
            break;
        case MAILBOX_INST_MCU2_0:
            if (clusterId == 0 && userId == 2)
            {
                cfg->eventId = CSLR_R5FSS1_CORE0_INTR_MAILBOX0_MAILBOX_CLUSTER_0_MAILBOX_CLUSTER_PEND_2;
            }
            else if (clusterId == 1 && userId == 2)
            {
                cfg->eventId = CSLR_R5FSS1_CORE0_INTR_MAILBOX0_MAILBOX_CLUSTER_1_MAILBOX_CLUSTER_PEND_2;
            }
            else if (clusterId == 4 && userId == 0)
            {
                cfg->eventId = CSLR_R5FSS1_CORE0_INTR_MAILBOX0_MAILBOX_CLUSTER_4_MAILBOX_CLUSTER_PEND_0;
            }
            else if (clusterId == 5 && userId == 0)
            {
                cfg->eventId = CSLR_R5FSS1_CORE0_INTR_MAILBOX0_MAILBOX_CLUSTER_5_MAILBOX_CLUSTER_PEND_0;
            }
            else if (clusterId == 7 && userId == 0)
            {
                cfg->eventId = CSLR_R5FSS1_CORE0_INTR_MAILBOX0_MAILBOX_CLUSTER_7_MAILBOX_CLUSTER_PEND_0;
            }
            else
            {
                retVal = MAILBOX_EINVAL;
            }
            break;
        case MAILBOX_INST_MCU2_1:
            if (clusterId == 0 && userId == 3)
            {
                cfg->eventId = CSLR_R5FSS1_CORE1_INTR_MAILBOX0_MAILBOX_CLUSTER_0_MAILBOX_CLUSTER_PEND_3;
            }
            else if (clusterId == 1 && userId == 3)
            {
                cfg->eventId = CSLR_R5FSS1_CORE1_INTR_MAILBOX0_MAILBOX_CLUSTER_1_MAILBOX_CLUSTER_PEND_3;
            }
            else if (clusterId == 4 && userId == 1)
            {
                cfg->eventId = CSLR_R5FSS1_CORE1_INTR_MAILBOX0_MAILBOX_CLUSTER_4_MAILBOX_CLUSTER_PEND_1;
            }
            else if (clusterId == 5 && userId == 1)
            {
                cfg->eventId = CSLR_R5FSS1_CORE1_INTR_MAILBOX0_MAILBOX_CLUSTER_5_MAILBOX_CLUSTER_PEND_1;
            }
            else if (clusterId == 7 && userId == 1)
            {
                cfg->eventId = CSLR_R5FSS1_CORE1_INTR_MAILBOX0_MAILBOX_CLUSTER_7_MAILBOX_CLUSTER_PEND_1;
            }
            else
            {
                retVal = MAILBOX_EINVAL;
            }
            break;
        case MAILBOX_INST_M4F_0:
            if (clusterId == 6 && userId == 3)
            {
                cfg->eventId = CSLR_MCU_M4FSS0_CORE0_NVIC_MAILBOX0_MAILBOX_CLUSTER_6_MAILBOX_CLUSTER_PEND_3;
            }
            else if (clusterId == 7 && userId == 3)
            {
                cfg->eventId = CSLR_MCU_M4FSS0_CORE0_NVIC_MAILBOX0_MAILBOX_CLUSTER_7_MAILBOX_CLUSTER_PEND_3;
            }
            else
            {
                retVal = MAILBOX_EINVAL;
            }
            break;
        default:
            retVal = MAILBOX_EINVAL;
            break;
    }

    cfg->priority      = 1U;

    return retVal;
}

#if defined(BUILD_MCU) && defined(VIM_DIRECT_REGISTRATION)
static inline void Mailbox_directClrNewMsgStatus(Mbox_Handle handle)
{
    MailboxClrNewMsgStatus(((Mailbox_Driver *)handle)->baseAddrRx,
                           ((Mailbox_HwCfg *)((Mailbox_Driver *)handle)->hwCfg)->rx.user,
                           ((Mailbox_HwCfg *)((Mailbox_Driver *)handle)->hwCfg)->rx.fifo);

    CSL_vimClrIntrPending((CSL_vimRegs *)(uintptr_t)TEST_VIM_BASE_ADDR,
                          ((Mailbox_HwCfg *)((Mailbox_Driver *)handle)->hwCfg)->eventId);
    /* Acknowledge interrupt servicing */
    CSL_vimAckIntr((CSL_vimRegs *)(uintptr_t)TEST_VIM_BASE_ADDR, \
                    (CSL_VimIntrMap)0u );
}

__attribute__((interrupt("IRQ")))     void mailboxIsr_0(void);
__attribute__((interrupt("IRQ")))     void mailboxIsr_1(void);
__attribute__((interrupt("IRQ")))     void mailboxIsr_2(void);
__attribute__((interrupt("IRQ")))     void mailboxIsr_3(void);
__attribute__((interrupt("IRQ")))     void mailboxIsr_4(void);
__attribute__((interrupt("IRQ")))     void mailboxIsr_5(void);



#ifdef __cplusplus
#pragma CODE_STATE (32)
#else
#pragma CODE_STATE (mailboxIsr_0,32)
#endif  /* #ifdef __cplusplus */
void mailboxIsr_0(void)
{
    (g_VimCallback[MAILBOX_INST_MPU1_0])(g_VimCallbackArg[MAILBOX_INST_MPU1_0], MAILBOX_INST_MPU1_0);
    Mailbox_directClrNewMsgStatus(g_VimCallbackArg[MAILBOX_INST_MPU1_0]);
}

#ifdef __cplusplus
#pragma CODE_STATE (32)
#else
#pragma CODE_STATE (mailboxIsr_1,32)
#endif  /* #ifdef __cplusplus */
void mailboxIsr_1(void)
{
    (g_VimCallback[MAILBOX_INST_MCU1_0])(g_VimCallbackArg[MAILBOX_INST_MCU1_0], MAILBOX_INST_MCU1_0);
    Mailbox_directClrNewMsgStatus(g_VimCallbackArg[MAILBOX_INST_MCU1_0]);
}

#ifdef __cplusplus
#pragma CODE_STATE (32)
#else
#pragma CODE_STATE (mailboxIsr_2,32)
#endif  /* #ifdef __cplusplus */
void mailboxIsr_2(void)
{
    (g_VimCallback[MAILBOX_INST_MCU1_1])(g_VimCallbackArg[MAILBOX_INST_MCU1_1], MAILBOX_INST_MCU1_1);
    Mailbox_directClrNewMsgStatus(g_VimCallbackArg[MAILBOX_INST_MCU1_1]);
}

#ifdef __cplusplus
#pragma CODE_STATE (32)
#else
#pragma CODE_STATE (mailboxIsr_3,32)
#endif  /* #ifdef __cplusplus */
void mailboxIsr_3(void)
{
    (g_VimCallback[MAILBOX_INST_MCU2_0])(g_VimCallbackArg[MAILBOX_INST_MCU2_0], MAILBOX_INST_MCU2_0);
    Mailbox_directClrNewMsgStatus(g_VimCallbackArg[MAILBOX_INST_MCU2_0]);
}

#ifdef __cplusplus
#pragma CODE_STATE (32)
#else
#pragma CODE_STATE (mailboxIsr_4,32)
#endif  /* #ifdef __cplusplus */
void mailboxIsr_4(void)
{
    (g_VimCallback[MAILBOX_INST_MCU2_1])(g_VimCallbackArg[MAILBOX_INST_MCU2_1], MAILBOX_INST_MCU2_1);
    Mailbox_directClrNewMsgStatus(g_VimCallbackArg[MAILBOX_INST_MCU2_1]);
}

#ifdef __cplusplus
#pragma CODE_STATE (32)
#else
#pragma CODE_STATE (mailboxIsr_5,32)
#endif  /* #ifdef __cplusplus */
void mailboxIsr_5(void)
{
    (g_VimCallback[MAILBOX_INST_M4F_0])(g_VimCallbackArg[MAILBOX_INST_M4F_0], MAILBOX_INST_M4F_0);
    Mailbox_directClrNewMsgStatus(g_VimCallbackArg[MAILBOX_INST_M4F_0]);
}

uintptr_t mailboxIsrArray[6] =
{
    (uintptr_t)&mailboxIsr_0,
    (uintptr_t)&mailboxIsr_1,
    (uintptr_t)&mailboxIsr_2,
    (uintptr_t)&mailboxIsr_3,
    (uintptr_t)&mailboxIsr_4,
    (uintptr_t)&mailboxIsr_5
};
#endif

void Mailbox_InternalCallbackFast(uintptr_t arg)
{
    uint32_t idx = (uint32_t)arg;
    Mbox_Handle handle = g_FastCallbackArg[idx];

    (g_FastCallback[idx])(g_FastCallbackArg[idx], idx);

    MailboxClrNewMsgStatus(((Mailbox_Driver *)handle)->baseAddrRx,
                           ((Mailbox_HwCfg *)((Mailbox_Driver *)handle)->hwCfg)->rx.user,
                           ((Mailbox_HwCfg *)((Mailbox_Driver *)handle)->hwCfg)->rx.fifo);
}

void Mailbox_InternalCallback(uintptr_t arg)
{
    uint32_t              n;
    Mailbox_Data         *mbox;
    Mailbox_Fifo         *fifo;

    mbox = (Mailbox_Data *)arg;
    if(mbox != NULL)
    {
        mbox->intCnt++;
        /* Optimization to save time checking the fifo if only one is being used */
        if (mbox->fifoCnt == 1U)
        {
            fifo = &mbox->fifoTable[0];
            if (mbox->fifoTable[0].func)
            {
                /* Call the function with handle and arg */
                (mbox->fifoTable[0].func)(fifo->handle, fifo->arg);
            }
            /* Clear new message status of Mailbox */
            MailboxClrNewMsgStatus(mbox->baseAddr, mbox->userId, fifo->queueId);
        }
        else
        {
            for(n = 0; n < mbox->fifoCnt; n++)
            {
                fifo = &mbox->fifoTable[n];

                if(0U != MailboxGetRawNewMsgStatus(mbox->baseAddr, mbox->userId, fifo->queueId))
                {
                    if (mbox->fifoTable[n].func)
                    {
                        /* Call the function with handle and arg */
                        (mbox->fifoTable[n].func)(fifo->handle, fifo->arg);
                    }
                    /* Clear new message status of Mailbox */
                    MailboxClrNewMsgStatus(mbox->baseAddr, mbox->userId, fifo->queueId);
                }
            }
        }
        mbox->noMsgCnt++;
    }
}

int32_t Mailbox_registerInterrupts(Mbox_Handle handle)
{
    Mailbox_Driver        *driver;
    int32_t               retVal = MAILBOX_SOK;
    Mailbox_HwCfg         *hwCfg = NULL;
    uint32_t              n;
    Mailbox_Data          *mbox = NULL;
    Mailbox_MbConfig      cfg;
    Mailbox_Instance      localEndpoint;
    void                  *hwiHandle = NULL;
    uint32_t              i = 0;
    Mailbox_Instance remoteEndpoint;
    uint32_t              q = 0;

    driver = (Mailbox_Driver *)handle;
    localEndpoint = driver->localEndpoint;
    remoteEndpoint = driver->remoteEndpoint;

    hwCfg = (Mailbox_HwCfg *)driver->hwCfg;

    /* Get the mailbox base addr for RX cluster */
    driver->baseAddrRx = Mailbox_getBaseAddr(hwCfg->rx.cluster);
    driver->baseAddrTx = Mailbox_getBaseAddr(hwCfg->tx.cluster);

    if ((driver->cfg.readMode != MAILBOX_MODE_POLLING) && (driver->cfg.readCallback != NULL))
    {
        if (gMailboxMCB.initParam.osalPrms.registerIntr == NULL)
        {
            retVal = MAILBOX_EINVAL;
        }

        if (retVal == MAILBOX_SOK)
        {
            /*
             * check if this cluster and user ID is already registered, and if so,
             * skip interrupt registration and just store callback/arg data for this
             * fifo to be handled in the callback
             */
            for (n = 0; n < g_mBoxCnt; n++)
            {
                if ((driver->baseAddrRx == g_mBoxData[n].baseAddr) &&
                    (hwCfg->rx.user == g_mBoxData[n].userId))
                        break;
            }

            /* Get the MailBox Data */
            mbox = &g_mBoxData[n];

            if (n == g_mBoxCnt)
            {
                /* Could not find one, this is new entry */
                mbox->baseAddr = driver->baseAddrRx;
                mbox->fifoCnt  = 0;
                mbox->userId   = hwCfg->rx.user;

                /*
                 * Before we register for the interrupt, make sure that all the fifo
                 * new msg interrupts are disabled. This helps in case there is
                 * some stale state from previous run.
                 */
                for (q = 0; q < MAILBOX_MAXFIFO_CNT; q++)
                {
                    MailboxDisableNewMsgInt(driver->baseAddrRx, hwCfg->rx.user, q);
                    MailboxClrNewMsgStatus(driver->baseAddrRx, hwCfg->rx.user, q);
                }

                /* clear new msg status */
                MailboxClrNewMsgStatus(driver->baseAddrRx, hwCfg->rx.user, hwCfg->rx.fifo);

                /* Get the interrupt configuration */
                retVal = Mailbox_getMailboxIntrRouterCfg(localEndpoint, hwCfg->rx.cluster, hwCfg->rx.user, &cfg, g_mBoxCnt);
                if (retVal == MAILBOX_SOK)
                {
                    hwCfg->eventId = cfg.eventId;

                    /* Register interrupts */
                    if ((hwCfg->exclusive == TRUE) && ((driver->cfg.readMode == MAILBOX_MODE_FAST)))
                    {
#if defined(BUILD_MCU) && defined(VIM_DIRECT_REGISTRATION)
                        if (driver->cfg.enableVIMDirectInterrupt == true)
                        {
                            g_VimCallback[remoteEndpoint] = driver->cfg.readCallback;
                            g_VimCallbackArg[remoteEndpoint] = driver;

                            hwiHandle = gMailboxMCB.initParam.osalPrms.registerDirectIntr((Mbox_OsalDirectIsrFxn)mailboxIsrArray[remoteEndpoint],
                                                                                      cfg.eventId,
                                                                                      cfg.priority);
                        }
#endif
                        if (hwiHandle == NULL)
                        {
                            /* Direct interrupt registration failed or isn't enabled, try regular registration */
                            g_FastCallback[remoteEndpoint] = driver->cfg.readCallback;
                            g_FastCallbackArg[remoteEndpoint] = driver;
                            hwiHandle = gMailboxMCB.initParam.osalPrms.registerIntr(Mailbox_InternalCallbackFast,
                                                                                    cfg.eventId,
                                                                                    cfg.priority,
                                                                                    (void *)remoteEndpoint,
                                                                                    (char *)"MAILBOX_NEW_MSG");
                            gMailboxMCB.hwiHandles.mailboxFull = hwiHandle;
                            if(hwiHandle == NULL)
                            {
                                //SystemP_printf("Mailbox_plugInterrupt : Failed to register ISR...\n");
                            }
                        }
                    }
                    else
                    {
                        hwiHandle = gMailboxMCB.initParam.osalPrms.registerIntr(Mailbox_InternalCallback,
                                                                                cfg.eventId,
                                                                                cfg.priority,
                                                                                (void *)mbox,
                                                                                (char *)"MAILBOX_NEW_MSG");
                        gMailboxMCB.hwiHandles.mailboxFull = hwiHandle;
                        if(hwiHandle == NULL)
                        {
                            //SystemP_printf("Mailbox_plugInterrupt : Failed to register ISR...\n");
                        }
                    }
                    g_mBoxCnt++;
                }
            }
        }

        if (retVal == MAILBOX_SOK)
        {
            /* Check if there is empty space in our fifoTable (for example if handle is closed and re-opened) */
            for (i = 0; i < mbox->fifoCnt; i++)
            {
                if (mbox->fifoTable[i].func == NULL)
                {
                    break;
                }
            }

            /* Add the fifo data for the remoteProcId. */
            mbox->fifoTable[i].cfgNdx  = n;
            mbox->fifoTable[i].func    = driver->cfg.readCallback;
            mbox->fifoTable[i].arg     = driver->remoteEndpoint;
            mbox->fifoTable[i].queueId = hwCfg->rx.fifo;
            mbox->fifoTable[i].handle  = driver;
            gInstToMBoxDataMap[driver->remoteEndpoint] = (uintptr_t)mbox;

            if (i == mbox->fifoCnt)
            {
                mbox->fifoCnt++;
            }

            if (driver->cfg.enableInterrupts == true)
            {
                /* enable the mailbox interrupt */
                MailboxEnableNewMsgInt(driver->baseAddrRx, hwCfg->rx.user, hwCfg->rx.fifo);
            }
        }
    }

    return retVal;
}

int32_t Mailbox_unregisterInterrupts(Mbox_Handle handle)
{
    Mailbox_Driver*    driver;
    int32_t            retVal = MAILBOX_SOK;
    Mailbox_HwCfg      *hwCfg = NULL;
    Mailbox_Data       *mbox = NULL;
    uint8_t            i = 0;

    driver = (Mailbox_Driver *)handle;
    hwCfg = (Mailbox_HwCfg *)driver->hwCfg;

    if ((driver->cfg.readMode != MAILBOX_MODE_POLLING) && (driver->cfg.readCallback != NULL))
    {
        mbox = (Mailbox_Data *)gInstToMBoxDataMap[driver->remoteEndpoint];

        /* Disable the mailbox interrupt */
        MailboxDisableNewMsgInt(driver->baseAddrRx, hwCfg->rx.user, hwCfg->rx.fifo);

        /* Remove the callback info from the fifo table */
        gInstToMBoxDataMap[driver->remoteEndpoint] = (uintptr_t)NULL;

        for (i = 0; i < mbox->fifoCnt; i++)
        {
            if (mbox->fifoTable[i].queueId == hwCfg->rx.fifo)
            {
                /* clear the fifo info */
                mbox->fifoTable[i].cfgNdx  = 0;
                mbox->fifoTable[i].func    = NULL;
                mbox->fifoTable[i].arg     = 0;
                mbox->fifoTable[i].queueId = 0;
                mbox->fifoTable[i].handle  = NULL;
            }
        }
    }

    return retVal;
}

int32_t Mailbox_enableInterrupts(Mbox_Handle handle)
{
    Mailbox_Driver*    driver;
    int32_t            retVal = MAILBOX_SOK;
    Mailbox_HwCfg      *hwCfg = NULL;

    if (handle != NULL)
    {
        driver = (Mailbox_Driver *)handle;
        hwCfg = (Mailbox_HwCfg *)driver->hwCfg;

        if (hwCfg != NULL)
        {
            MailboxEnableNewMsgInt(driver->baseAddrRx, hwCfg->rx.user, hwCfg->rx.fifo);
	}
        else
        {
            retVal = MAILBOX_EINVAL;
        }
    }
    else
    {
        retVal = MAILBOX_EINVAL;
    }

    return retVal;
}

int32_t Mailbox_disableInterrupts(Mbox_Handle handle)
{
    Mailbox_Driver*    driver;
    int32_t            retVal = MAILBOX_SOK;
    Mailbox_HwCfg      *hwCfg = NULL;

    if (handle != NULL)
    {
        driver = (Mailbox_Driver *)handle;
        hwCfg = (Mailbox_HwCfg *)driver->hwCfg;

        if (hwCfg != NULL)
        {
            MailboxDisableNewMsgInt(driver->baseAddrRx, hwCfg->rx.user, hwCfg->rx.fifo);
        }
        else
        {
            retVal = MAILBOX_EINVAL;
        }
    }
    else
    {
        retVal = MAILBOX_EINVAL;
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

#if defined (BUILD_M4F_0)
    temp = phyAddr + 0x40000000;
#endif

    return ((void *) temp);
}
