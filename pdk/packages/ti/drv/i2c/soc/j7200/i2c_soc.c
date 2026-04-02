/*
 * Copyright (C) 2020-2024 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/**
 *  \file   j7200/i2c_soc.c
 *
 *  \brief  J7200 SoC specific I2C hardware attributes.
 *
 *   This file contains the hardware attributes of I2C peripheral like
 *   base address, interrupt number etc.
 *
 *      Following instances are supported
 *      Main Domain I2C instance 0 to 6 on MPU A72 & MCU 20, MCU 21
 *      MCU Domain I2C instance 0 to 1 on MCU 10, MCU 11
 *
 *      To add support for additional I2C instances in main/mcu domain
 *          please update "i2cInitCfg"
 *      If the main domain instances are to be supported in mcu domain,
 *          implement function I2C_socCfgIntrPathFxn and use sciclient apis to
 *          route interrupts
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <string.h>
#include <ti/csl/arch/csl_arch.h>
#include <ti/csl/soc.h>
#include <ti/drv/i2c/i2c.h>
#include <ti/drv/i2c/soc/i2c_soc.h>
#include <ti/drv/sciclient/sciclient.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define I2C_INPUT_CLK        (96000000U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* MAIN Domain A72 */
#if defined (BUILD_MPU)

/* I2C configuration structure */
I2C_HwAttrs i2cInitCfg[I2C_HWIP_MAX_CNT] =
{
    {
        /* default configuration for I2C instance and MPU core on Main domain*/
        (uint32_t)CSL_I2C0_CFG_BASE,        /* baseAddr */
        CSLR_COMPUTE_CLUSTER0_MSMC_1MB_GIC_SPI_I2C0_POINTRPEND_0,  /* intNum */
        0,                                  /* eventId */
        I2C_INPUT_CLK,
        BTRUE,
        {
            /* default own slave addresses */
            0x70, 0x0, 0x0, 0x0
        },
        (I2C_socCfgIntrPathFxn)NULL         /* No routing of ints required */
    },
    {
        (uint32_t)CSL_I2C1_CFG_BASE,
        CSLR_COMPUTE_CLUSTER0_MSMC_1MB_GIC_SPI_I2C1_POINTRPEND_0,
        0,
        I2C_INPUT_CLK,
        BTRUE,
        {
            0x71, 0x0, 0x0, 0x0
        },
        (I2C_socCfgIntrPathFxn)NULL         /* No routing of ints required */
    },
    {
        (uint32_t)CSL_I2C2_CFG_BASE,
        CSLR_COMPUTE_CLUSTER0_MSMC_1MB_GIC_SPI_I2C2_POINTRPEND_0,
        0,
        I2C_INPUT_CLK,
        BTRUE,
        {
            0x72, 0x0, 0x0, 0x0
        },
        (I2C_socCfgIntrPathFxn)NULL         /* No routing of ints required */
    },
    {
        (uint32_t)CSL_I2C3_CFG_BASE,
        CSLR_COMPUTE_CLUSTER0_MSMC_1MB_GIC_SPI_I2C3_POINTRPEND_0,
        0,
        I2C_INPUT_CLK,
        BTRUE,
        {
            0x73, 0x0, 0x0, 0x0
        },
        (I2C_socCfgIntrPathFxn)NULL         /* No routing of ints required */
    },
    {
        (uint32_t)CSL_I2C4_CFG_BASE,
        CSLR_COMPUTE_CLUSTER0_MSMC_1MB_GIC_SPI_I2C4_POINTRPEND_0,
        0,
        I2C_INPUT_CLK,
        BTRUE,
        {
            0x74, 0x0, 0x0, 0x0
        },
        (I2C_socCfgIntrPathFxn)NULL         /* No routing of ints required */
    },
    {
        (uint32_t)CSL_I2C5_CFG_BASE,
        CSLR_COMPUTE_CLUSTER0_MSMC_1MB_GIC_SPI_I2C5_POINTRPEND_0,
        0,
        I2C_INPUT_CLK,
        BTRUE,
        {
            0x75, 0x0, 0x0, 0x0
        },
        (I2C_socCfgIntrPathFxn)NULL         /* No routing of ints required */
    },
    {
        (uint32_t)CSL_I2C6_CFG_BASE,
        CSLR_COMPUTE_CLUSTER0_MSMC_1MB_GIC_SPI_I2C6_POINTRPEND_0,
        0,
        I2C_INPUT_CLK,
        BTRUE,
        {
            0x76, 0x0, 0x0, 0x0
        },
        (I2C_socCfgIntrPathFxn)NULL         /* No routing of ints required */
    },
};
#endif /* MPU Build */

/* MCU Domain R5F */
#if defined (BUILD_MCU)
/* I2C configuration structure */
I2C_HwAttrs i2cInitCfg[I2C_HWIP_MAX_CNT] =
{
    {
        /*
         * default configuration for I2C instances and R5 cores on MCU domain
         * I2C_socInit() is called during initialization to change the default
         * configurations to use Main Domain's I2C instances if R5 is running
         * on Main domain
         */
        (uint32_t)CSL_MCU_I2C0_CFG_BASE,    /* baseAddr */
        CSLR_MCU_R5FSS0_CORE0_INTR_MCU_I2C0_POINTRPEND_0,
        0,                                  /* eventId */
        I2C_INPUT_CLK,
        BTRUE,
        {
            /* default own slave addresses */
            0x70, 0x0, 0x0, 0x0
        },
        (I2C_socCfgIntrPathFxn)NULL         /* No routing of ints required */
    },
    {
        (uint32_t)CSL_MCU_I2C1_CFG_BASE,
        CSLR_MCU_R5FSS0_CORE0_INTR_MCU_I2C1_POINTRPEND_0,
        0,
        I2C_INPUT_CLK,
        BTRUE,
        {
            0x71, 0x0, 0x0, 0x0
        },
        (I2C_socCfgIntrPathFxn)NULL         /* No routing of ints required */
    },
    {
        0,
        0,
        0,
        I2C_INPUT_CLK,
        BTRUE,
        {
            0x72, 0x0, 0x0, 0x0
        },
        (I2C_socCfgIntrPathFxn)NULL         /* No routing of ints required */
    },
    {
        0,
        0,
        0,
        I2C_INPUT_CLK,
        BTRUE,
        {
            0x73, 0x0, 0x0, 0x0
        },
        (I2C_socCfgIntrPathFxn)NULL         /* No routing of ints required */
    },
    {
        0,
        0,
        0,
        I2C_INPUT_CLK,
        BTRUE,
        {
            0x74, 0x0, 0x0, 0x0
        },
        (I2C_socCfgIntrPathFxn)NULL         /* No routing of ints required */
    },
    {
        0,
        0,
        0,
        I2C_INPUT_CLK,
        BTRUE,
        {
            0x75, 0x0, 0x0, 0x0
        },
        (I2C_socCfgIntrPathFxn)NULL         /* No routing of ints required */
    },
    {
        0,
        0,
        0,
        I2C_INPUT_CLK,
        BTRUE,
        {
            0x76, 0x0, 0x0, 0x0
        },
        (I2C_socCfgIntrPathFxn)NULL         /* No routing of ints required */
    },
};
#endif /* MCU Build */



/* I2C objects */
I2C_v1_Object I2cObjects[I2C_HWIP_MAX_CNT];


/* I2C configuration structure */
I2C_config_list I2C_config = {
    {
        &I2C_v1_FxnTable,
        &I2cObjects[0],
        &i2cInitCfg[0]
    },

    {
        &I2C_v1_FxnTable,
        &I2cObjects[1],
        &i2cInitCfg[1]
    },

    {
        &I2C_v1_FxnTable,
        &I2cObjects[2],
        &i2cInitCfg[2]
    },

    {
        &I2C_v1_FxnTable,
        &I2cObjects[3],
        &i2cInitCfg[3]
    },

    {
        &I2C_v1_FxnTable,
        &I2cObjects[4],
        &i2cInitCfg[4]
    },

    {
        &I2C_v1_FxnTable,
        &I2cObjects[5],
        &i2cInitCfg[5]
    },

    {
        &I2C_v1_FxnTable,
        &I2cObjects[6],
        &i2cInitCfg[6]
    },
    {NULL, NULL, NULL},
    {NULL, NULL, NULL},
    {NULL, NULL, NULL},
    {NULL, NULL, NULL},
    {NULL, NULL, NULL},
    {NULL, NULL, NULL},
    {NULL, NULL, NULL}
};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/**
 * \brief  This API gets the SoC level of I2C intial configuration
 *
 * \param  index       I2C instance index.
 * \param  cfg       Pointer to I2C SOC initial config.
 *
 * \return 0 success: -1: error
 *
 */
int32_t I2C_socGetInitCfg(uint32_t idx, I2C_HwAttrs *cfg)
{
    int32_t ret = 0;

    if (idx < I2C_HWIP_MAX_CNT)
    {
        *cfg = i2cInitCfg[idx];
    }
    else
    {
        ret = (int32_t)(-1);
    }

    return ret;
}

/**
 * \brief  This API sets the SoC level of I2C intial configuration
 *
 * \param  index       I2C instance index.
 * \param  cfg       Pointer to I2C SOC initial config.
 *
 * \return           0 success: -1: error
 *
 */
int32_t I2C_socSetInitCfg(uint32_t idx, const I2C_HwAttrs *cfg)
{
    int32_t ret = I2C_STATUS_SUCCESS;

    if (I2C_HWIP_MAX_CNT > idx)
    {
        i2cInitCfg[idx] = *cfg;
    }
    else
    {
        ret = I2C_STATUS_ERROR;
    }

    return ret;
}

#if defined (BUILD_MCU)
/**
 * \brief  This API update the default SoC level of configurations
 *         based on the core and domain
 *
 *         i2cInitCfg table configures MCU domain's I2C instances by
 *         default for R5, I2C_socInit() is called in I2C_init() to
 *         overwrite the defaut configurations with the configurations
 *         of Main domain's UART instances if R5 is on the Main domain
 *
 * \param  none
 *
 * \return none
 *
 */
void I2C_socInit(void);
void I2C_socInit(void)
{
    uint32_t         i;
    CSL_ArmR5CPUInfo info;

    CSL_armR5GetCpuID(&info);
    if ((uint32_t)CSL_ARM_R5_CLUSTER_GROUP_ID_0 != info.grpId)
    {
        /* Pulsar R5 core is on the Main domain */
        for (i = 0U; i < I2C_HWIP_MAX_CNT; i++)
        {
            /* Configure the Main SS UART instances for Main SS Pulsar R5 */
            i2cInitCfg[i].baseAddr = (uint32_t)CSL_I2C0_CFG_BASE + (0x10000U * i);

            if (2U > i)
            {
                i2cInitCfg[i].intNum = CSLR_R5FSS0_CORE0_INTR_I2C0_POINTRPEND_0 + i;
            }
            else
            {
                i2cInitCfg[i].intNum = CSLR_R5FSS0_CORE0_INTR_I2C2_POINTRPEND_0 + (i - 2U);
            }
        }
    }
}
#endif

#if defined (NOT_REQUIRED_YET)
static int32_t I2C_configSocIntrPath(const void *pHwAttrs, bool setIntrPath)
{
   int32_t ret = I2C_STATUS_SUCCESS;

#if defined(BUILD_MCU)
    struct tisci_msg_rm_irq_set_req      rmIrqReq;
    struct tisci_msg_rm_irq_set_resp     rmIrqResp;
    struct tisci_msg_rm_irq_release_req  rmIrqRelease;
    uint16_t                             src_id;
    uint16_t                             src_index;
    uint16_t                             dst_id;
    CSL_ArmR5CPUInfo                     r5CpuInfo;
    int32_t                              retVal;
    I2C_HwAttrs const                   *hwAttrs = (I2C_HwAttrs const *)(pHwAttrs);

    if (CSL_WKUP_I2C0_CFG_BASE == hwAttrs->baseAddr)
    {
        /*
         * The interrupt path is established using the DMSC firmware
         * for I2C instance 0 on wakeup domin
         */
        src_id = TISCI_DEV_WKUP_I2C0;
        src_index = (uint16_t)hwAttrs->eventId;  /* set to 0 for non-event based interrupt */

        CSL_armR5GetCpuID(&r5CpuInfo);
        if ((uint32_t)CSL_ARM_R5_CLUSTER_GROUP_ID_1 == r5CpuInfo.grpId)
        {
            if(0U == r5CpuInfo.cpuID)
            {
                dst_id = TISCI_DEV_R5FSS0_CORE0;
            }
            else
            {
                dst_id = TISCI_DEV_R5FSS0_CORE1;
            }
        }
        else if ((uint32_t)CSL_ARM_R5_CLUSTER_GROUP_ID_2 == r5CpuInfo.grpId)
        {
            if(0U == r5CpuInfo.cpuID)
            {
                dst_id = TISCI_DEV_R5FSS1_CORE0;
            }
            else
            {
                dst_id = TISCI_DEV_R5FSS1_CORE1;
            }
        }
        else
        {
            ret = I2C_STATUS_ERROR;
        }

        if (I2C_STATUS_SUCCESS == ret)
        {
            if(setIntrPath)
            {
                (void)memset (&rmIrqReq, 0, sizeof(rmIrqReq));
                rmIrqReq.secondary_host = TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST;
                rmIrqReq.src_id = src_id;
                rmIrqReq.src_index = src_index;

                /* Set the destination interrupt */
                rmIrqReq.valid_params |= TISCI_MSG_VALUE_RM_DST_ID_VALID;
                rmIrqReq.valid_params |= TISCI_MSG_VALUE_RM_DST_HOST_IRQ_VALID;

                /* Set the destination based on the core */
                rmIrqReq.dst_id       = dst_id;
                rmIrqReq.dst_host_irq = (uint16_t)hwAttrs->intNum;
            }
            else
            {
                (void)memset (&rmIrqRelease,0,sizeof(rmIrqRelease));
                rmIrqRelease.secondary_host  = TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST;
                rmIrqRelease.src_id = src_id;
                rmIrqRelease.src_index = src_index;

                /* Set the destination interrupt */
                rmIrqRelease.valid_params |= TISCI_MSG_VALUE_RM_DST_ID_VALID;
                rmIrqRelease.valid_params |= TISCI_MSG_VALUE_RM_DST_HOST_IRQ_VALID;

                /* Set the destination based on the core */
                rmIrqRelease.dst_id       = dst_id;
                rmIrqRelease.dst_host_irq = (uint16_t)hwAttrs->intNum;
            }

            /* Config event */
            if(setIntrPath)
            {
                retVal = Sciclient_rmIrqSet(
                            (const struct tisci_msg_rm_irq_set_req *)&rmIrqReq,
                            &rmIrqResp,
                            SCICLIENT_SERVICE_WAIT_FOREVER);
             }
            else
            {
                retVal = Sciclient_rmIrqRelease(
                            (const struct tisci_msg_rm_irq_release_req *)&rmIrqRelease,
                             SCICLIENT_SERVICE_WAIT_FOREVER);
            }
            if (CSL_PASS != retVal)
            {
               ret = I2C_STATUS_ERROR;
            }
        }
    }
#endif

    return(ret);
}
#endif /* NOT_REQUIRED_YET */
