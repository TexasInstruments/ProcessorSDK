/*
 * DM Stub Firmware
 *
 * am62x soc lpscs.h
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
#define LPSC_MAIN_DM                     1
#define LPSC_DM_PBIST                    2
#define LPSC_MAIN2DM_ISO                 3
#define LPSC_DM2MAIN_ISO                 4
#define LPSC_DM2MAIN_INFRA_ISO           5
#define LPSC_DM2CENTRAL_ISO              6
#define LPSC_CENTRAL2DM_ISO              7
#define LPSC_GP_SPARE0                   8
#define LPSC_EMIF_LOCAL                  9
#define LPSC_EMIF_CFG_ISO                10
#define LPSC_EMIF_DATA_ISO               11
#define LPSC_MAIN_USB0_ISO               12
#define LPSC_MAIN_USB1_ISO               13
#define LPSC_MAIN_TEST                   14
#define LPSC_GPMC                        15
#define LPSC_GP_SPARE1                   16
#define LPSC_MAIN_MCASP_0                17
#define LPSC_MAIN_MCASP_1                18
#define LPSC_MAIN_MCASP_2                19
#define LPSC_EMMC_8B                     20
#define LPSC_EMMC_4B_0                   21
#define LPSC_EMMC_4B_1                   22
#define LPSC_USB_0                       23
#define LPSC_USB_1                       24
#define LPSC_CSI_RX_0                    25
#define LPSC_DPHY_0                      26
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
#define LPSC_MAIN_SPARE0                 38
#define LPSC_MAIN_SPARE1                 39
#define PD_ICSSM                         1              /* Main ISCCM PD and its LPSCs */
#define LPSC_ICSSM                       40
#define PD_CPSW                          2              /* Main CPSW PD and its LPSCs */
#define LPSC_CPSW3G                      41
#define PD_A53_CLUSTER_0                 3              /* Main A53 CLUSTER0 PD and its LPSCs */
#define LPSC_A53_CLUSTER_0               42
#define LPSC_A53_CLUSTER_0_PBIST_0       43
#define LPSC_A53_CLUSTER_0_PBIST_1       44
#define PD_A53_0                         4              /* Main A53_0 PD and its LPSCs */
#define LPSC_A53_0                       45
#define PD_A53_1                         5              /* Main A53_1 PD and its LPSCs */
#define LPSC_A53_1                       46
#define PD_A53_2                         6              /* Main A53_2 PD and its LPSCs */
#define LPSC_A53_2                       47
#define PD_A53_3                         7              /* Main A53_3 PD and its LPSCs */
#define LPSC_A53_3                       48
#define PD_GPU                           8              /* Main GPU PD and its LPSCs */
#define LPSC_GPU                         49
#define LPSC_GPU_PBIST                   50
#define PD_DSS                           9       /* Main DSS PD and its LPSCs */
#define LPSC_DSS                         51

#define PD_GP_CORE_CTL_MCU               0      /* MCU GP CORE CTL PD and its LPSCs */
#define LPSC_MCU_ALWAYSON                0
#define LPSC_MAIN2MCU_ISO                1
#define LPSC_MCU2MAIN_ISO                2
#define LPSC_DM2SAFE_ISO                 3
#define LPSC_MCU2DM_ISO                  4
#define LPSC_MCU_TEST                    5
#define PD_MCU_M4F                       1      /* MCU M4F PD and its LPSCs */
#define LPSC_MCU_M4F                     6
#define LPSC_MCU_MCANSS0                 7
#define LPSC_MCU_MCANSS1                 8
#define LPSC_MCU_COMMON                  9

#define DDR_PD                           PD_GP_CORE_CTL       /* DDR power domain */

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
	LPSCS_MCU2MAIN_ISO,
	LPSCS_MCU2DM_ISO,
	LPSCS_MCU_TEST,
	LPSCS_MCU_COMMON,
	LPSCS_DM2SAFE_ISO,
	LPSCS_MCU_MAX,
};

enum mcu_pds {
	GP_CORE_CTL_MCU_PDS,
	MCU_M4F_PDS,
	MCU_PDS_MAX,
};

enum usb_lpscs {
	USB_LPSC_0,
	USB_LPSC_1,
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
