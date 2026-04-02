/**
 *  \file   j7200/GPIO_soc.c
 *
 *  \brief  J7200 SOC specific GPIO hardware attributes.
 *
 *   This file contains the GPIO hardware attributes like base address and
 *   interrupt ids.
 */

/*
 * Copyright (C) 2019 - 2022 Texas Instruments Incorporated - http://www.ti.com/
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

#include <string.h>
#include <ti/csl/csl_utils.h>
#include <ti/csl/soc/j7200/src/cslr_soc.h>
#include <ti/csl/csl_types.h>
#include <ti/csl/hw_types.h>
#include <ti/csl/soc.h>
#include <ti/csl/arch/csl_arch.h>
#include <ti/drv/gpio/soc/GPIO_soc.h>
#include <ti/drv/sciclient/sciclient.h>

/** \brief Number of gpio pins for each port */
#define GPIO_NUM_PINS_PER_PORT           (128U)

/** \brief Number of gpio ports present in the soc */
#define GPIO_NUM_PORTS                   (4U)

static uint16_t GPIO_socGetCoreSciId();
static int32_t GPIO_socGetIrqRange(uint16_t ir_id, uint16_t dst_id, uint16_t *irq_range_start, uint16_t *irq_range_num);

/* GPIO Pin interrupt configurations */
GPIO_IntCfg GPIO_intCfgs[GPIO_NUM_PORTS][GPIO_NUM_PINS_PER_PORT] =
{
    {
        /* GPIO port 0 pin 0 */
        {
            0, /* Interrrupt Num - Query from BoardCfg */
            0, /* GPIO pin interrupt event - Query from BoardCfg*/
            INVALID_INTC_MUX_NUM,        /* CIC not used for GPIO pin 0 */
            0,
            0
        },
        /* GPIO port 0 pin 1 */
        {
            0, /* Interrrupt Num - Query from BoardCfg */
            0, /* GPIO pin interrupt event - Query from BoardCfg*/
            INVALID_INTC_MUX_NUM,
            0,
            0
        },
        /* GPIO port 0 pin 2 */
        {

            0, /* Interrrupt Num - Query from BoardCfg */

            0, /* GPIO pin interrupt event - Query from BoardCfg*/
            INVALID_INTC_MUX_NUM,
            0,
            0
        },
        /* GPIO port 0 pin 3 */
        {
            0, /* Interrrupt Num - Query from BoardCfg */
            0, /* GPIO pin interrupt event - Query from BoardCfg*/
            INVALID_INTC_MUX_NUM,
            0,
            0
        },
        /* GPIO port 0 pin 4 */
        {
            0, /* Interrrupt Num - Query from BoardCfg */
            0, /* GPIO pin interrupt event - Query from BoardCfg*/
            INVALID_INTC_MUX_NUM,
            0,
            0
        },
        /* GPIO port 0 pin 5 */
        {
            0, /* Interrrupt Num - Query from BoardCfg */
            0, /* GPIO pin interrupt event - Query from BoardCfg*/
            INVALID_INTC_MUX_NUM,
            0,
            0
        },
        /* GPIO port 0 pin 6 */
        {
            0, /* Interrrupt Num - Query from BoardCfg */
            0, /* GPIO pin interrupt event - Query from BoardCfg*/
            INVALID_INTC_MUX_NUM,
            0,
            0
        },
        /* GPIO port 0 pin 7 */
        {
            0, /* Interrrupt Num - Query from BoardCfg */
            0, /* GPIO pin interrupt event - Query from BoardCfg*/
            INVALID_INTC_MUX_NUM,
            0,
            0
        },
        /* GPIO port 0 pin 8 */
        {
            0, /* Interrrupt Num - Query from BoardCfg */
            0, /* GPIO pin interrupt event - Query from BoardCfg*/
            INVALID_INTC_MUX_NUM,
            0,
            0
        },
        /* GPIO port 0 pin 9 */
        {
            0, /* Interrrupt Num - Query from BoardCfg */
            0, /* GPIO pin interrupt event - Query from BoardCfg*/
            INVALID_INTC_MUX_NUM,
            0,
            0
        },
        /* GPIO port 0 pin 10 */
        {
            0, /* Interrrupt Num - Query from BoardCfg */
            0, /* GPIO pin interrupt event - Query from BoardCfg*/
            INVALID_INTC_MUX_NUM,
            0,
            0
        },
        /* GPIO port 0 pin 11 */
        {
            0, /* Interrrupt Num - Query from BoardCfg */
            0, /* GPIO pin interrupt event - Query from BoardCfg*/
            INVALID_INTC_MUX_NUM,
            0,
            0
        },
        /* GPIO port 0 pin 12 */
        {
            0, /* Interrrupt Num - Query from BoardCfg */
            0, /* GPIO pin interrupt event - Query from BoardCfg*/
            INVALID_INTC_MUX_NUM,
            0,
            0
        },
        /* GPIO port 0 pin 13 */
        {
            0, /* Interrrupt Num - Query from BoardCfg */
            0, /* GPIO pin interrupt event - Query from BoardCfg*/
            INVALID_INTC_MUX_NUM,
            0,
            0
        },
        /* GPIO port 0 pin 14 */
        {
            0, /* Interrrupt Num - Query from BoardCfg */
            0, /* GPIO pin interrupt event - Query from BoardCfg*/
            INVALID_INTC_MUX_NUM,
            0,
            0
        },
        /* GPIO port 0 pin 15 */
        {
            0, /* Interrrupt Num - Query from BoardCfg */
            0, /* GPIO pin interrupt event - Query from BoardCfg*/
            INVALID_INTC_MUX_NUM,
            0,
            0
        },
    },
};

/* GPIO Driver hardware attributes */
GPIO_v0_hwAttrsList GPIO_v0_hwAttrs =
{
    {
        (uint32_t)CSL_GPIO0_BASE,
        &GPIO_intCfgs[0][0],
        &GPIO_socConfigIntrPath
    },
    {
        (uint32_t)CSL_GPIO2_BASE,
        &GPIO_intCfgs[1][0],
        &GPIO_socConfigIntrPath
    },
    {
        (uint32_t)CSL_GPIO4_BASE,
        &GPIO_intCfgs[2][0],
        &GPIO_socConfigIntrPath
    },
    {
        (uint32_t)CSL_GPIO6_BASE,
        &GPIO_intCfgs[3][0],
        &GPIO_socConfigIntrPath
    },
    {
        0U,
        NULL,
        NULL
    },
    {
        0U,
        NULL,
        NULL
    },
    {
        0U,
        NULL,
        NULL
    },
    {
        0U,
        NULL,
        NULL
    }
};

/* GPIO configuration structure */
CSL_PUBLIC_CONST GPIOConfigList GPIO_config =
{
    {
        &GPIO_FxnTable_v0,
        NULL,
        &GPIO_v0_hwAttrs[0]
    },

    {
        &GPIO_FxnTable_v0,
        NULL,
        &GPIO_v0_hwAttrs[1]
    },

    {
        NULL,
        NULL,
        NULL
    }
};

/**
 * \brief  This API gets the SoC level of GPIO intial configuration
 *
 * \param  idx       GPIO instance index.
 * \param  cfg       Pointer to GPIO SOC initial config.
 *
 * \return 0 success: -1: error
 *
 */
int32_t GPIO_socGetInitCfg(uint32_t idx, GPIO_v0_HwAttrs *cfg)
{
    int32_t ret = 0;

    if (idx < GPIO_NUM_PORTS)
    {
        *cfg = GPIO_v0_hwAttrs[idx];
    }
    else
    {
        ret = -1;
    }

    return ret;
}

/**
 * \brief  This API sets the SoC level of GPIO intial configuration
 *
 * \param  idx       GPIO instance index.
 * \param  cfg       Pointer to GPIO SOC initial config.
 *
 * \return           0 success: -1: error
 *
 */
int32_t GPIO_socSetInitCfg(uint32_t idx, const GPIO_v0_HwAttrs *cfg)
{
    int32_t ret = 0;

    if (idx < GPIO_NUM_PORTS)
    {
        GPIO_v0_hwAttrs[idx] = *cfg;
    }
    else
    {
        ret = -1;
    }

    return ret;
}

/**
 * \brief  This API gets the number of GPIO pins per port
 *         and GPIO number of ports
 *
 * \param  numPins     pointer to numPins variable.
 * \param  numPorts    pointer to numPorts variable.
 *
 * \return none
 *
 */
void GPIO_socGetNumPinsPorts(uint32_t *numPins, uint32_t *numPorts)
{
    *numPins = GPIO_NUM_PINS_PER_PORT;
    *numPorts = GPIO_NUM_PORTS;
}

void MuxIntcP_clearInEvent(int32_t muxNum, int32_t muxInEvent)
{
    (void)muxNum;
    (void)muxInEvent;

    return;
}


MuxIntcP_Status MuxIntcP_create(MuxIntcP_inParams *inParams, MuxIntcP_outParams *outParams)
{

    return (MuxIntcP_OK);
}

/* A count kept for each bank usage/ 16 pins share a bank */
#define GPIO_NUM_BANKS        ((GPIO_NUM_PINS_PER_PORT + 15U) / 16U)
uint32_t GPIO_PinBankUsageCount[GPIO_NUM_PORTS][GPIO_NUM_BANKS] = {{0U}};

/**
 * \brief  This function sets/clears the soc interrupt path including the \n
 *         interrupt routers (GPIO_RTR,MAIN2MCU_RTR) etc and GPIO number of ports
 *
 * \param  portNum     The GPIO port number to configure the interrupt path.
 * \param  pinNum      The GPIO pin number to configure the interrupt path.
 * \param  hwAttrs     The GPIO_v0_HwAttrs for this SOC
 *  \param  setIntrPath TRUE - set the interrupt path, FALSE - clear the interrupt path
 * \return CSL_PASS if successful, CSL_FAIL if failed
 *
 */
int32_t GPIO_socConfigIntrPath(uint32_t portNum, uint32_t pinNum,void *hwAttrs,bool setIntrPath)
{

    GPIO_v0_HwAttrs   *cfg = (GPIO_v0_HwAttrs *)hwAttrs;
    GPIO_IntCfg       *intCfg;
    uint32_t           bankNum;
	int32_t retVal=CSL_PASS;
    struct tisci_msg_rm_irq_set_req     rmIrqReq;
    struct tisci_msg_rm_irq_set_resp    rmIrqResp;
    struct tisci_msg_rm_irq_release_req rmIrqRelease;

    memset(&rmIrqReq, 0, sizeof(rmIrqReq));
    memset(&rmIrqResp, 0, sizeof(rmIrqResp));
    memset(&rmIrqRelease, 0, sizeof(rmIrqRelease));
    
    uint16_t ir_id = 0U, src_id = 0U, src_index = 0U, dst_id, dst_host_irq, irq_range_start, irq_range_num;
    intCfg = cfg->intCfg;

    /* Input parameter validation */
    bankNum = pinNum/16U; /* Each GPIO bank has 16 pins */

    /* We route bank interrupts to the cpu interrupts */
    switch (cfg->baseAddr)
    {
        case (uint32_t)CSL_WKUP_GPIO0_BASE:
            src_id = TISCI_DEV_WKUP_GPIO0;
            src_index = (uint16_t)bankNum;  /* This is the bus_gpio_bank (0-5) mentioned in DMSC firmware guide for J7200_DEV_WKUP_GPIO0 */
            break;
        case (uint32_t)CSL_WKUP_GPIO1_BASE:
            src_id = TISCI_DEV_WKUP_GPIO1;
            src_index = (uint16_t)bankNum;  /* This is the bus_gpio_bank (0-5) mentioned in DMSC firmware guide for J7200_DEV_WKUP_GPIO1 */
            break;
        case (uint32_t)CSL_GPIO0_BASE:
            src_id = TISCI_DEV_GPIO0;
            src_index = (uint16_t)bankNum;  /* This is the bus_gpio_bank (0-7) mentioned in DMSC firmware guide  for J7200_DEV_GPIO0 */
            break;
        case (uint32_t)CSL_GPIO2_BASE:
            src_id = TISCI_DEV_GPIO2;
            src_index = (uint16_t)bankNum;  /* This is the bus_gpio_bank (0-7) mentioned in DMSC firmware guide  for J7200_DEV_GPIO2 */
            break;
        case (uint32_t)CSL_GPIO4_BASE:
            src_id = TISCI_DEV_GPIO4;
            src_index = (uint16_t)bankNum;  /* This is the bus_gpio_bank (0-7) mentioned in DMSC firmware guide  for J7200_DEV_GPIO4 */
            break;
        case (uint32_t)CSL_GPIO6_BASE:
            src_id = TISCI_DEV_GPIO6;
            src_index = (uint16_t)bankNum;  /* This is the bus_gpio_bank (0-7) mentioned in DMSC firmware guide  for J7200_DEV_GPIO6 */
            break;
        default:
            retVal = CSL_EFAIL;
            break;
    }

    /* Get TISCI Destination Idx for the core. */
    dst_id = GPIO_socGetCoreSciId();

    if(CSL_PASS == retVal)
    {
        /* Check if the source is a GPIO in the wakeup domain. */
        if (TISCI_DEV_WKUP_GPIO0 == src_id || TISCI_DEV_WKUP_GPIO1 == src_id)
        {
            /* Wakeup GPIO is connected to single router, which routes to
             * both MCU cores, and the MAIN cores and A72 cores as well.
             */
            ir_id = TISCI_DEV_WKUP_GPIOMUX_INTRTR0;
        }
        /* Check if the source is a GPIO in the Main domain. */
        else
        {
            switch(dst_id)
            {
                /* Check if the interrupt is going to the MCU domain. */
                case TISCI_DEV_MCU_R5FSS0_CORE0:
                case TISCI_DEV_MCU_R5FSS0_CORE1:
                    ir_id = TISCI_DEV_MAIN2MCU_PLS_INTRTR0;
                    break;
                /* Check if the interrupt is staying in the MAIN domain. */
                case TISCI_DEV_R5FSS0_CORE0:
                case TISCI_DEV_R5FSS0_CORE1:
                case TISCI_DEV_COMPUTE_CLUSTER0_GIC500SS:
                    ir_id = TISCI_DEV_GPIOMUX_INTRTR0;
                    break;
                default:
                    break;

            }
        }
    }

    /* Get the valid range of valid interrupts based on the ir_id. */
    retVal = GPIO_socGetIrqRange(ir_id, dst_id, &irq_range_start, &irq_range_num);
    

    if(CSL_PASS == retVal) 
    {
        if(bankNum < irq_range_num)
        {
            dst_host_irq = irq_range_start + bankNum;

            intCfg[pinNum].intNum   = dst_host_irq; /* Core Interrupt Idx - Use Range from BoardCfg */ 

        }
        else
        {
            retVal = CSL_EFAIL;
        }
    }

    if(CSL_PASS == retVal) 
    {
        if(setIntrPath)
        {
            (void)memset (&rmIrqReq,0,sizeof(rmIrqReq));

            rmIrqReq.secondary_host         = TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST;
            rmIrqReq.src_id = src_id;
            rmIrqReq.src_index = src_index;  /* This is the event coming out of
            the peripheral */

            /* Set the destination interrupt */
            rmIrqReq.valid_params |= TISCI_MSG_VALUE_RM_DST_ID_VALID;
            rmIrqReq.valid_params |= TISCI_MSG_VALUE_RM_DST_HOST_IRQ_VALID;

            /* Set the destination based on the core */
            rmIrqReq.dst_id       = dst_id;
            rmIrqReq.dst_host_irq       = dst_host_irq;
            if(GPIO_PinBankUsageCount[portNum][bankNum] == 0U) {
                retVal = Sciclient_rmIrqSet(
                            (const struct tisci_msg_rm_irq_set_req *)&rmIrqReq,
                            &rmIrqResp,
                            SCICLIENT_SERVICE_WAIT_FOREVER);

                if(retVal==CSL_PASS) {
                    /* Increase the bank usage count for this port */
                    GPIO_PinBankUsageCount[portNum][bankNum]++;
                }
                } else {
                    /* The interrupt path is already allocated, no need to re-allocate it */
                    retVal = CSL_PASS;
                }
        }
        else
        {
            (void)memset (&rmIrqRelease,0,sizeof(rmIrqRelease));

            rmIrqRelease.secondary_host         = TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST;
            rmIrqRelease.src_id = src_id;
            rmIrqRelease.src_index = src_index;  /* This is the event coming out of the peripheral */

            /* Set the destination interrupt */
            rmIrqRelease.valid_params |= TISCI_MSG_VALUE_RM_DST_ID_VALID;
            rmIrqRelease.valid_params |= TISCI_MSG_VALUE_RM_DST_HOST_IRQ_VALID;

            /* Set the destination based on the core */
            rmIrqRelease.dst_id       = dst_id;
            rmIrqRelease.dst_host_irq = dst_host_irq;

            retVal = Sciclient_rmIrqRelease(
                            (const struct tisci_msg_rm_irq_release_req *)&rmIrqRelease,
                            SCICLIENT_SERVICE_WAIT_FOREVER);

            if(retVal==CSL_PASS)
            {
                /* Increase the bank usage count for this port */
                GPIO_PinBankUsageCount[portNum][bankNum]--;
            }
        }
    }

  return(retVal);
}


static uint16_t GPIO_socGetCoreSciId()
{ 
    uint16_t devId = 0U;

#if defined (BUILD_MPU)
    devId = TISCI_DEV_COMPUTE_CLUSTER0_GIC500SS;
#elif defined (BUILD_MCU)
    CSL_ArmR5CPUInfo info;

    CSL_armR5GetCpuID(&info);
    if (info.grpId == (uint32_t)CSL_ARM_R5_CLUSTER_GROUP_ID_0)
    {
        /* MCU SS Pulsar R5 SS */
        devId = (info.cpuID == CSL_ARM_R5_CPU_ID_0)?
                                    TISCI_DEV_MCU_R5FSS0_CORE0:
                                        TISCI_DEV_MCU_R5FSS0_CORE1;
    }
    else if (info.grpId == (uint32_t)CSL_ARM_R5_CLUSTER_GROUP_ID_1)
    {
        /* MAIN SS Pulsar R5 SS0 */
        devId = (info.cpuID == CSL_ARM_R5_CPU_ID_0)?
                                    TISCI_DEV_R5FSS0_CORE0:
                                        TISCI_DEV_R5FSS0_CORE1;
    }
#endif
    return devId;
}

static int32_t GPIO_socGetIrqRange(uint16_t ir_id, uint16_t dst_id, uint16_t *irq_range_start, uint16_t *irq_range_num)
{
    int32_t         retVal = CSL_PASS;
    uint16_t        irIntrIdx;
    struct tisci_msg_rm_get_resource_range_resp res;
    struct tisci_msg_rm_get_resource_range_req  req;

    memset(&res, 0, sizeof(res));
    memset(&req, 0, sizeof(req));

    req.type           = ir_id;
    req.subtype        = (uint8_t)TISCI_RESASG_SUBTYPE_IR_OUTPUT;
    req.secondary_host = (uint8_t)TISCI_MSG_VALUE_RM_UNUSED_SECONDARY_HOST;

    res.range_num = 0;
    res.range_start = 0;

    /* Get interrupt number range */
    retVal =  Sciclient_rmGetResourceRange(
                &req,
                &res,
                SCICLIENT_SERVICE_WAIT_FOREVER);
    if (CSL_PASS != retVal || res.range_num == 0) {
        /* Try with HOST_ID_ALL */
        req.type           = ir_id;
        req.subtype        = (uint8_t)TISCI_RESASG_SUBTYPE_IR_OUTPUT;
        req.secondary_host = TISCI_HOST_ID_ALL;

        retVal = Sciclient_rmGetResourceRange(
                &req,
                &res,
                SCICLIENT_SERVICE_WAIT_FOREVER);
    }
    if ((CSL_PASS == retVal) && (res.range_num != 0))
    {
        *irq_range_num = res.range_num;
        /* Translate IR Idx to Core Interrupt Idx */
        irIntrIdx = res.range_start;
        retVal = Sciclient_rmIrqTranslateIrOutput(ir_id,
                                                  irIntrIdx,
                                                  dst_id,
                                                  irq_range_start);
                    
    }
    else
    {
        retVal = CSL_EFAIL;
    }

    return retVal;
}
