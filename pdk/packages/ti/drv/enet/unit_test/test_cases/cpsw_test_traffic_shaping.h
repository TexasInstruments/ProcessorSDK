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
 *  \file cpsw_test_traffic_shaping.h
 *
 *  \brief cpsw_test_traffic_shaping test private header file.
 */

#ifndef ENET_TEST_TRAFFICSHAPING_H_
#define ENET_TEST_TRAFFICSHAPING_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
#define ENETTESTTRAFFICSHAPING_DEFAULTFLOWID                       (0)
#define ENETTESTTRAFFICSHAPING1_RXFLOWID                           (1)
#define ENETTESTTRAFFICSHAPING2_RXFLOWID                           (2)
#define ENETTESTTRAFFICSHAPING3_RXFLOWID                           (3)
#define ENETTESTTRAFFICSHAPING_DEFAULTCHID                         (0)
#define ENETTESTTRAFFICSHAPING1_TXCHID                             (1)
#define ENETTESTTRAFFICSHAPING2_TXCHID                             (2)
#define ENETTESTTRAFFICSHAPING3_TXCHID                             (3)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
int32_t EnetTestTrafficShaping_Run(EnetTestTaskObj *taskObj);

void EnetTestTrafficShaping_setOpenPrms(EnetTestTaskObj *taskObj,
                                        Cpsw_Cfg *pCpswCfg,
                                        EnetUtils_Cfg *pUtilsPrms);

void EnetTestTrafficShaping_updatePortLinkCfg(EnetPer_PortLinkCfg *pLinkArgs,
                                              Enet_MacPort portNum);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef ENET_TEST_TRAFFICSHAPING_H_ */
