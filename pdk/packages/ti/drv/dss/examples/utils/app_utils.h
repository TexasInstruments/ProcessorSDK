/*
 *  Copyright (c) Texas Instruments Incorporated 2018
 *  All rights reserved.
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
 *  \file app_utils.h
 *
 *  \brief DSS example utility header file.
 */

#ifndef APP_UTILS_H_
#define APP_UTILS_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
/* Display output type */
#if defined (SOC_AM65XX)
#define APP_OUTPUT_OLDI          ((uint32_t) 0)
#else
#define APP_OUTPUT_HDMI          ((uint32_t) 0)
#define APP_OUTPUT_EDP           ((uint32_t) 1)
#define APP_OUTPUT_DSI           ((uint32_t) 2)
#endif

#if defined (SOC_J721S2)
/* Multilink Serdes enables USB or PCI alongside DP during dp init, which 
*  requires the serdes reference clock to be 100Mhz*/
#define DO_MULTILINK             (0U)
#endif

#if defined (SOC_J721S2) || defined (SOC_J784S4)
/*DSI has two instances on J721s2 and J784s4 , these macros will be used to 
configure the clock for the required instance.*/
#define DSI_INSTANCE_0           (1U)
#define DSI_INSTANCE_1           (0U)

#if (1U == DSI_INSTANCE_0)
#define UTILS_TISCI_DEV_DSS_DSI     TISCI_DEV_DSS_DSI0
#define UTILS_TISCI_DEV_DPHY_TX     TISCI_DEV_DPHY_TX0
#elif (1U == DSI_INSTANCE_1)
#define UTILS_TISCI_DEV_DSS_DSI     TISCI_DEV_DSS_DSI1
#define UTILS_TISCI_DEV_DPHY_TX     TISCI_DEV_DPHY_TX1
#endif
#endif

#if defined (SOC_J742S2)
#define UTILS_TISCI_DEV_DSS_DSI     TISCI_DEV_DSS_DSI0
#define UTILS_TISCI_DEV_DPHY_TX     TISCI_DEV_DPHY_TX0
#endif

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */
/**
 *  \brief LCD configuration parameters.
 */
typedef struct
{
    uint32_t outType;
    /**< Display output type */
    uint64_t pixelClk;
    /**< Pixel Clock in Hz */
} App_utilsLcdCfgParams;
/* ========================================================================== */
/*                  Internal/Private Function Declarations                   */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

void App_configureLCD(App_utilsLcdCfgParams cfgParams);
void App_configureSoC(void);

/* ========================================================================== */
/*                              Global Variables                              */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*      Internal Function Declarations (Needed for other static inlines)      */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef APP_UTILS_H_ */

/* @} */
