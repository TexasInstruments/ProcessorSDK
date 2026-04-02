/******************************************************************************
 * Copyright (c) 2021 Texas Instruments Incorporated - http://www.ti.com
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
 *
 *****************************************************************************/

/** 
 *  \file board_lld_init.c
 *
 *  \brief This file initializes UART and I2C LLD modules 
 *
 */

#include <ti/csl/arch/csl_arch.h>
#include "board_internal.h"
#include "board_utils.h"
#include "board_cfg.h"
#include "board_control.h"

extern Board_I2cInitCfg_t gBoardI2cInitCfg;
extern Board_initParams_t gBoardInitParams;
static uint32_t gUARTBaseAddr = 0;
static uint32_t gUARTClkFreq = 0;

uint32_t gBoardI2cBaseAddr[BOARD_SOC_DOMAIN_MAX][I2C_HWIP_MAX_CNT] =
    {{CSL_I2C0_CFG_BASE, CSL_I2C1_CFG_BASE, CSL_I2C2_CFG_BASE, CSL_I2C3_CFG_BASE,
      CSL_I2C4_CFG_BASE, CSL_I2C5_CFG_BASE, CSL_I2C6_CFG_BASE},
     {CSL_WKUP_I2C0_CFG_BASE, 0, 0, 0, 0, 0, 0},
     {CSL_MCU_I2C0_CFG_BASE, CSL_MCU_I2C1_CFG_BASE, 0, 0, 0, 0, 0}};

Board_I2cObj_t gBoardI2cObj[BOARD_I2C_PORT_CNT] = {
    {NULL, BOARD_SOC_DOMAIN_MAIN, 0, 0},
    {NULL, BOARD_SOC_DOMAIN_MAIN, 1, 0},
    {NULL, BOARD_SOC_DOMAIN_MAIN, 2, 0},
    {NULL, BOARD_SOC_DOMAIN_MAIN, 3, 0},
    {NULL, BOARD_SOC_DOMAIN_MAIN, 4, 0},
    {NULL, BOARD_SOC_DOMAIN_MAIN, 5, 0},
    {NULL, BOARD_SOC_DOMAIN_MAIN, 6, 0}
};

uint32_t gBoardUartBaseAddr[BOARD_SOC_DOMAIN_MAX][CSL_UART_MAIN_CNT] =
    {{CSL_UART0_BASE, CSL_UART1_BASE, CSL_UART2_BASE, CSL_UART3_BASE, CSL_UART4_BASE,
      CSL_UART5_BASE, CSL_UART6_BASE, CSL_UART7_BASE, CSL_UART8_BASE, CSL_UART9_BASE},
     {CSL_WKUP_UART0_BASE, 0, 0, 0, 0, 0, 0, 0, 0, 0},
     {CSL_MCU_UART0_BASE, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

/**
  *  \brief   Returns base address of given I2C instance
  *
  *  \param   instNum [IN] I2C instance
  *
  *  \param   domain  [IN] Domain of I2C controller
  *                           BOARD_SOC_DOMAIN_MAIN - Main Domain
  *                           BOARD_SOC_DOMAIN_WKUP - Wakeup domain
  *                           BOARD_SOC_DOMAIN_MCU - MCU domain
  *
  *  \return  Valid base address in case of success or 0
  *
  */
static uint32_t Board_getI2cBaseAddr(uint8_t instNum,
                                     uint8_t domain)
{
    uint32_t baseAddr = 0;

    if((instNum < I2C_HWIP_MAX_CNT) &&
       (domain <= BOARD_SOC_DOMAIN_MCU))
    {
        baseAddr = gBoardI2cBaseAddr[domain][instNum];
    }

    return (baseAddr);
}

/**
  *  \brief   Returns base address of given UART instance
  *
  *  \param   instNum [IN] UART instance
  *
  *  \param   domain  [IN] Domain of UART controller
  *                           BOARD_SOC_DOMAIN_MAIN - Main Domain
  *                           BOARD_SOC_DOMAIN_WKUP - Wakeup domain
  *                           BOARD_SOC_DOMAIN_MCU - MCU domain
  *
  *  \return  Valid base address in case of success or 0
  *
  */
static uint32_t Board_getUartBaseAddr(uint8_t instNum,
                                      uint8_t domain)
{
    uint32_t baseAddr = 0;

    if((instNum < CSL_UART_MAIN_CNT) &&
       (domain <= BOARD_SOC_DOMAIN_MCU))
    {
        baseAddr = gBoardUartBaseAddr[domain][instNum];
    }

    return (baseAddr);
}

/**
 *  \brief   This function initializes the default UART instance for use for
 *           console operations.
 *
 *  \return  Board_STATUS in case of success or appropriate error code.
 *
 */
Board_STATUS Board_uartStdioInit(void)
{
    UART_HwAttrs uart_cfg;
    uint32_t uartInst;
    uint32_t uartBaseAddr;
    uint32_t socDomainUART;
    uint32_t socDomainCore;

#ifdef BUILD_MCU
    CSL_ArmR5CPUInfo info = {0U, 0U, 0U};

    CSL_armR5GetCpuID(&info);
    if (CSL_ARM_R5_CLUSTER_GROUP_ID_0 != info.grpId)
    {
        /* Main R5 cores will use main domain UART instances */
        socDomainUART = BOARD_SOC_DOMAIN_MAIN;
        uartInst      = gBoardInitParams.uartInst;
    }
    else
    {
        socDomainUART = gBoardInitParams.uartSocDomain;
        if(BOARD_SOC_DOMAIN_MCU == socDomainUART)
        {
            uartInst = BOARD_MCU_UART_INSTANCE;
        }
        else
        {
            uartInst = gBoardInitParams.uartInst;
        }
    }
#else
    socDomainUART = gBoardInitParams.uartSocDomain;
    uartInst      = gBoardInitParams.uartInst;
#endif

    socDomainCore = Board_getSocDomain();

    /* Disable the UART interrupt */
    UART_socGetInitCfg(uartInst, &uart_cfg);

    if(socDomainUART != socDomainCore)
    {
        uartBaseAddr = Board_getUartBaseAddr(uartInst, socDomainUART);
        if(0U != uartBaseAddr)
        {
            gUARTBaseAddr = uart_cfg.baseAddr;
            uart_cfg.baseAddr = uartBaseAddr;
        }
        else
        {
            return BOARD_INVALID_PARAM;
        }

        gUARTClkFreq = uart_cfg.frequency;
        if(BOARD_SOC_DOMAIN_MAIN == socDomainUART)
        {
            uart_cfg.frequency = BOARD_UART_CLK_MAIN;
        }
        else
        {
            uart_cfg.frequency = BOARD_UART_CLK_WKUP;
        }
    }

    uart_cfg.enableInterrupt = UFALSE;
    UART_socSetInitCfg(uartInst, &uart_cfg);

    UART_stdioInit(uartInst);

    return BOARD_SOK;
}

/**
 *  \brief   This function is to get the i2c handle of the requested
 *           instance of the specifed domain
 *
 *  \param   domainType [IN] Domain of I2C controller
 *                             BOARD_SOC_DOMAIN_MAIN - Main Domain
 *                             BOARD_SOC_DOMAIN_WKUP - Wakeup domain
 *                             BOARD_SOC_DOMAIN_MCU - MCU domain
 *
 *  \param   i2cInst    [IN]        I2C instance
 *
 *  \return  Valid I2C handle in case of success or NULL in case of failure.
 *
 */
I2C_Handle Board_getI2CHandle(uint8_t domainType,
                              uint32_t i2cInst)
{
    Board_STATUS status;
    Board_I2cInitCfg_t i2cCfg;

    i2cCfg.i2cInst    = i2cInst;
    i2cCfg.socDomain  = domainType;
    i2cCfg.enableIntr = BFALSE;
    Board_setI2cInitConfig(&i2cCfg);

    status = Board_i2cInit();
    if(BOARD_SOK != status)
    {
        return NULL;
    }

    return (gBoardI2cObj[i2cInst].i2cHandle);
}

/**
 *  \brief   This function is to release the i2c handle acquired using
 *           Board_getI2CHandle function
 *
 *  \param   hI2c [IN] I2C handle
 *
 *  \return  Board_STATUS in case of success or appropriate error code.
 *
 */
Board_STATUS Board_releaseI2CHandle(I2C_Handle hI2c)
{
    Board_STATUS status = BOARD_FAIL;
    I2C_HwAttrs i2c_cfg;
    uint32_t coreDomain;
    uint32_t i2cInst;

    if(hI2c != NULL)
    {
        for (i2cInst = 0; i2cInst < BOARD_I2C_PORT_CNT; i2cInst++)
        {
            if(hI2c == gBoardI2cObj[i2cInst].i2cHandle)
            {
                break;
            }
        }

        if(i2cInst != BOARD_I2C_PORT_CNT)
        {
            I2C_close(gBoardI2cObj[i2cInst].i2cHandle);
            gBoardI2cObj[i2cInst].i2cHandle = NULL;

            coreDomain = Board_getSocDomain();

            if(gBoardI2cObj[i2cInst].i2cDomain != coreDomain)
            {
                I2C_socGetInitCfg(i2cInst, &i2c_cfg);
                i2c_cfg.baseAddr = gBoardI2cObj[i2cInst].i2cBaseAddr;
                I2C_socSetInitCfg(i2cInst, &i2c_cfg);
            }

            status = BOARD_SOK;
        }
    }

    return status;
}

/**
  *  \brief   This function initializes the i2c instance set using
  *           Board_setI2cInitConfig API.
  *
  *  \return  Board_STATUS in case of success or appropriate error code.
  *
  */
Board_STATUS Board_i2cInit(void)
{
    I2C_Params i2cParams;
    I2C_HwAttrs i2c_cfg;
    uint32_t i2cInst;
    uint32_t i2cBaseAddr;
    uint32_t i2cDomain;
    uint32_t coreDomain;

    i2cInst   = gBoardI2cInitCfg.i2cInst;
    i2cDomain = gBoardI2cInitCfg.socDomain;

    if(gBoardI2cObj[i2cInst].i2cHandle == NULL)
    {
        coreDomain = Board_getSocDomain();

        I2C_init();
        I2C_socGetInitCfg(i2cInst, &i2c_cfg);

        if(i2cDomain != coreDomain)
        {
            i2cBaseAddr = Board_getI2cBaseAddr(i2cInst, i2cDomain);
            if(i2cBaseAddr != 0)
            {
                gBoardI2cObj[i2cInst].i2cBaseAddr = i2c_cfg.baseAddr;
                i2c_cfg.baseAddr                  = i2cBaseAddr;
            }
            else
            {
                return BOARD_INVALID_PARAM;
            }
        }

        i2c_cfg.enableIntr = gBoardI2cInitCfg.enableIntr;

        I2C_socSetInitCfg(i2cInst, &i2c_cfg);
        I2C_Params_init(&i2cParams);

        gBoardI2cObj[i2cInst].i2cHandle = I2C_open(i2cInst, &i2cParams);
        if (gBoardI2cObj[i2cInst].i2cHandle == NULL)
        {
            return BOARD_I2C_OPEN_FAIL;
        }

        gBoardI2cObj[i2cInst].i2cDomain = i2cDomain;
        gBoardI2cObj[i2cInst].instNum   = i2cInst;
    }
    else
    {
        if(gBoardI2cObj[i2cInst].i2cDomain != i2cDomain)
        {
            return BOARD_I2C_OPEN_FAIL;
        }
    }

    return BOARD_SOK;
}

/**
 *  \brief   This function is used to close the initialized board I2C handle.
 */
Board_STATUS Board_i2cDeInit(void)
{
    uint32_t coreDomain;
    I2C_HwAttrs i2c_cfg;
    uint32_t i2cInst;

    i2cInst = gBoardI2cInitCfg.i2cInst;

    if(gBoardI2cObj[i2cInst].i2cHandle != NULL)
    {
        I2C_close(gBoardI2cObj[i2cInst].i2cHandle);
        gBoardI2cObj[i2cInst].i2cHandle = NULL;

        coreDomain = Board_getSocDomain();

        if(gBoardI2cObj[i2cInst].i2cDomain != coreDomain)
        {
            I2C_socGetInitCfg(i2cInst, &i2c_cfg);
            i2c_cfg.baseAddr = gBoardI2cObj[i2cInst].i2cBaseAddr;
            I2C_socSetInitCfg(i2cInst, &i2c_cfg);
        }
    }

    return BOARD_SOK;
}

/**
 *  \brief   This function is used to de-initialize board UART handles.
 */
Board_STATUS Board_uartDeInit(void)
{
    UART_HwAttrs uart_cfg;
    uint32_t socDomainCore;

    UART_stdioDeInit();

    socDomainCore = Board_getSocDomain();

    if(gBoardInitParams.uartSocDomain != socDomainCore)
    {
        UART_socGetInitCfg(gBoardInitParams.uartInst, &uart_cfg);
        uart_cfg.baseAddr  = gUARTBaseAddr;
        uart_cfg.frequency = gUARTClkFreq;
        UART_socSetInitCfg(gBoardInitParams.uartInst, &uart_cfg);
    }

    return BOARD_SOK;
}
