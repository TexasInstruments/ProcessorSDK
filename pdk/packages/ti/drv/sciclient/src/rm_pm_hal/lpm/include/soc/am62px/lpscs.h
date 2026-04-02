/*
 * DM Stub Firmware
 *
 * am62px soc lpscs.h
 *
 * Copyright (C) 2023-2024, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
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

#ifndef LPSCS_H_
#define LPSCS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <types/short_types.h>
#include <psc_raw.h>

#define PD_GP_CORE_CTL                   0       /* Main GP CORE CTL PD and its LPSCs */
#define LPSC_MAIN_ALWAYS_ON              0
#define LPSC_MAIN_DM                     1
#define LPSC_DM_PBIST                    2
#define LPSC_MAIN2DM_ISO                 3
#define LPSC_DM2MAIN_ISO                 4
#define LPSC_DM2MAIN_INFRA_ISO           5
#define LPSC_DM2CENTRAL_ISO              6
#define LPSC_CENTRAL2DM_ISO              7
#define LPSC_DM_PBIST_1                  8
#define LPSC_CSI_RX1                     9
#define LPSC_DPHY_RX1                    10
#define LPSC_CSI_TX0                     11
#define LPSC_USB0_ISO                    12
#define LPSC_USB2_ISO                    13
#define LPSC_MAIN_TEST                   14
#define LPSC_GPMC                        15
#define LPSC_MCAN1                       16
#define LPSC_MAIN_MCASP_0                17
#define LPSC_MAIN_MCASP_1                18
#define LPSC_MAIN_MCASP_2                19
#define LPSC_EMMC_8B                     20
#define LPSC_EMMC_4B_0                   21
#define LPSC_EMMC_4B_1                   22
#define LPSC_USB_0                       23
#define LPSC_USB_2                       24
#define LPSC_CSI_RX_0                    25
#define LPSC_DPHY_RX_0                   26
#define LPSC_SMS_COMMON                  27
#define LPSC_FSS_OSPI                    28
#define LPSC_TIFS                        29
#define LPSC_HSM                         30
#define LPSC_SA3UL                       31
#define LPSC_HSM_ISO                     32
#define LPSC_DEBUGSS                     33
#define LPSC_MAIN_IP                     34
#define LPSC_MAIN_MCANSS_0               35
#define LPSC_GIC                         36
#define LPSC_MAIN_PBIST                  37
#define LPSC_DPHY_TX0                    38
#define LPSC_JPEG                        39
#define LPSC_SERDES0                     40
#define LPSC_SERDES1                     41
#define LPSC_CPSW                        42
#define LPSC_GPCORE_RSVD2                43
#define LPSC_CSI_RX3                     44
#define LPSC_DPHY_RX3                    45
#define LPSC_PBIST1                      46
#define LPSC_USB1                        47
#define LPSC_CSI_RX2                     48
#define LPSC_DPHY_RX2                    49
#define LPSC_GPCORE_RSVD3                50
#define PD_GPU_CORE                      1      /* Main GPU CORE PD and its LPSCs */
#define LPSC_GPU_CORE                    51
#define PD_CPSW                          2      /* Main CPSW PD and its LPSCs */
#define LPSC_GPCORE_RSVD5                52
#define PD_A53_CLUSTER_0                 3      /* Main A53 CLUSTER0 PD and its LPSCs */
#define LPSC_A53_CLUSTER_0               53
#define LPSC_A53_CLUSTER_0_PBIST_0       54
#define LPSC_A53_CLUSTER_0_PBIST_1       55
#define PD_A53_CLUSTER_0_CORE_0          4      /* Main A53_0 PD and its LPSCs */
#define LPSC_A53_CLUSTER_0_CORE_0        56
#define PD_A53_CLUSTER_0_CORE_1          5      /* Main A53_1 PD and its LPSCs */
#define LPSC_A53_CLUSTER_0_CORE_1        57
#define PD_A53_CLUSTER_0_CORE_2          6      /* Main A53_2 PD and its LPSCs */
#define LPSC_A53_CLUSTER_0_CORE_2        58
#define PD_A53_CLUSTER_0_CORE_3          7      /* Main A53_3 PD and its LPSCs */
#define LPSC_A53_CLUSTER_0_CORE_3        59
#define PD_GPU                           8      /* Main GPU PD and its LPSCs */
#define LPSC_GPU                         60
#define LPSC_GPU_PBIST                   61
#define LPSC_PDRSVD2_RSVD0               62
#define LPSC_PDRSVD2_RSVD1               63
#define LPSC_PDRSVD2_RSVD2               64
#define PD_CODEC                         10             /* Main CODEC PD and its LPSCs */
#define LPSC_CODEC                       65
#define LPSC_CODEC_PBIST                 66
#define PD_C7X                           11             /* Main C7X PD and its LPSCs */
#define LPSC_C7X_CORE                    67
#define LPSC_C7X_PBIST                   68
#define LPSC_C7X_COMMON                  69
#define PD_VPAC                          12     /* Main VPAC PD and its LPSCs */
#define LPSC_VPAC_CORE                   70
#define LPSC_VPAC_PBIST                  71
#define PD_DDR                           13     /* Main DDR PD and its LPSCs */
#define LPSC_EMIF_LOCAL                  72
#define LPSC_EMIF_CFG_ISO                73
#define LPSC_EMIF_DATA_ISO               74
#define PD_PCIE                          14     /* Main PCIE PD and its LPSCs */
#define LPSC_PCIE0                       75
#define PD_CSDSP_1                       15     /* Main CSDSP1 CORE PD and its LPSCs */
#define LPSC_CSDSP_1_CORE                76
#define LPSC_CSDSP_1_PBIST               77
#define LPSC_CSDSP_1_COMMON              78
#define PD_MAIN_MCUSS0                   16             /* Main MCUSS0 PD and its LPSCs */
#define LPSC_MAIN_MCUSS0_CORE0           79
#define LPSC_MAIN_MCUSS0_PBIST           80
#define PD_MAIN_SRAM0                    17             /* Main SRAM0 MSRAM PD and its LPSCs */
#define LPSC_MAIN_SRAM0_MSRAM            81
#define LPSC_MAIN_SRAM0_PBIST            82
#define PD_MAIN_SRAM1                    18     /* Main SRAM1 MSRAM PD and its LPSCs */
#define LPSC_MAIN_SRAM1_MSRAM            83
#define LPSC_MAIN_SRAM1_PBIST            84
#define PD_RSVD0                         19     /* RSVD0 PD and its LPSCs */
#define LPSC_PDRSVD0_RSVD0               85
#define LPSC_PDRSVD0_RSVD1               86
#define LPSC_PDRSVD0_RSVD2               87
#define PD_RSVD1                         20     /* RSVD1 PD and its LPSCs */
#define LPSC_PDRSVD1_RSVD0               88
#define LPSC_RSVD1_RSVD1                 89
#define LPSC_RSVD1_RSVD2                 90
#define PD_DSS                           21     /* DSS PD and its LPSCs */
#define LPSC_DSS0                        91
#define LPSC_DSS1                        92
#define LPSC_DSS_DSI0                    93
#define LPSC_OLDI0                       94
#define LPSC_OLDI1                       95

#define PD_GP_CORE_CTL_MCU               0      /* MCU GP CORE CTL PD and its LPSCs */
#define LPSC_MCU_ALWAYSON                0
#define LPSC_MAIN2MCU_ISO                1
#define LPSC_DM2MCU_ISO                  2
#define LPSC_DM2SAFE_ISO                 3
#define LPSC_MCU2DM_ISO                  4
#define LPSC_MCU_TEST                    5
#define PD_MCUSS                         1      /* MCUSS PD and its LPSCs */
#define LPSC_MCU_R5                      6
#define LPSC_MCU_MCANSS0                 7
#define LPSC_MCU_MCANSS1                 8
#define LPSC_MCU_COMMON                  9
#define LPSC_MCU_PBIST                   10

#define DDR_PD                           PD_DDR       /* DDR power domain */

enum lpscs_main_phase1 {
	LPSC_PHASE1_A53_0,
	LPSC_PHASE1_A53_CLUSTER_0,
	LPSC_PHASE1_HSM,
	LPSC_PHASE1_TIFS,
	LPSC_PHASE1_DM2CENTRAL_ISO,
	LPSC_PHASE1_MAIN2DM_ISO,
	LPSC_PHASE1_DM2MAIN_ISO,
	LPSC_PHASE1_CENTRAL2DM_ISO,
	LPSC_PHASE1_MAX,
};

enum lpscs_main_phase2 {
	LPSC_PHASE2_DEBUGSS,
	LPSC_PHASE2_MAX,
};

enum lpscs_mcu {
	LPSCS_MAIN2MCU_ISO,
	LPSCS_MCU2DM_ISO,
	LPSCS_DM2MCU_ISO,
	LPSCS_MCU_TEST,
	LPSCS_MCU_COMMON,
	LPSCS_DM2SAFE_ISO,
	LPSCS_MCU_MAX,
};

enum mcu_pds {
	GP_CORE_CTL_MCU_PDS,
	MCUSS_PDS,
	MCU_PDS_MAX,
};

enum usb_lpscs {
	USB_LPSC_0,
	USB_LPSC_2,
	USB_LPSCS_MAX,
};

extern u32 num_main_lpscs_phase1;
extern u32 num_main_lpscs_phase2;
extern u32 num_mcu_lpscs;
extern u32 num_mcu_pds;
extern u32 num_usb_lpscs;

extern const struct pd_lpsc main_lpscs_phase1[LPSC_PHASE1_MAX];
extern const struct pd_lpsc main_lpscs_phase2[LPSC_PHASE2_MAX];
extern const struct pd_lpsc mcu_lpscs[LPSCS_MCU_MAX];
extern const u32 mcu_pds[MCU_PDS_MAX];
extern struct usb_lpsc usb_lpscs[USB_LPSCS_MAX];

#ifdef __cplusplus
}
#endif
#endif /* LPSCS_H_ */
