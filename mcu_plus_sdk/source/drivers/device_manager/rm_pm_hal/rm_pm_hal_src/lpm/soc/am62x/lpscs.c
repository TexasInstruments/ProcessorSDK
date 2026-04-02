/*
 * DM Stub Firmware
 *
 * am62x soc lpscs.c
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

#include <lpscs.h>

/* MAIN LPSCs to be disabled during Deepsleep phase 1 */
const struct pd_lpsc main_lpscs_phase1[LPSC_PHASE1_MAX] = {
	{ PD_A53_0,	    LPSC_A53_0		    },
	{ PD_A53_CLUSTER_0, LPSC_A53_CLUSTER_0	    },
	{ PD_GP_CORE_CTL,   LPSC_HSM		    },
	{ PD_GP_CORE_CTL,   LPSC_TIFS		    },
	{ PD_GP_CORE_CTL,   LPSC_DM2CENTRAL_ISO	    },
	{ PD_GP_CORE_CTL,   LPSC_MAIN2DM_ISO	    },
	{ PD_GP_CORE_CTL,   LPSC_DM2MAIN_ISO	    },
	{ PD_GP_CORE_CTL,   LPSC_CENTRAL2DM_ISO	    },
};

u32 num_main_lpscs_phase1 = sizeof(main_lpscs_phase1) / sizeof(struct pd_lpsc);

/* MAIN LPSCs to be disabled during Deepsleep phase 2 */
const struct pd_lpsc main_lpscs_phase2[LPSC_PHASE2_MAX] = {
	{ PD_GP_CORE_CTL, LPSC_DEBUGSS	  },
};

u32 num_main_lpscs_phase2 = sizeof(main_lpscs_phase2) / sizeof(struct pd_lpsc);

/* MCU LPSCs to be disabled during Deepsleep */
const struct pd_lpsc mcu_lpscs[LPSCS_MCU_MAX] = {
	{ PD_GP_CORE_CTL_MCU, LPSC_MAIN2MCU_ISO	   },
	{ PD_GP_CORE_CTL_MCU, LPSC_MCU2MAIN_ISO	   },
	{ PD_GP_CORE_CTL_MCU, LPSC_MCU2DM_ISO	   },
	{ PD_GP_CORE_CTL_MCU, LPSC_MCU_TEST	   },
	{ PD_MCU_M4F,	      LPSC_MCU_COMMON	   },
	{ PD_GP_CORE_CTL_MCU, LPSC_DM2SAFE_ISO	   },
};

u32 num_mcu_lpscs = sizeof(mcu_lpscs) / sizeof(struct pd_lpsc);

/* MCU PDs to be disabled during Deepsleep */
const u32 mcu_pds[MCU_PDS_MAX] = { PD_GP_CORE_CTL_MCU, PD_MCU_M4F };

u32 num_mcu_pds = sizeof(mcu_pds) / sizeof(u32);

/* USB LPSCs to be saved and restored during Deepsleep */
struct usb_lpsc usb_lpscs[USB_LPSCS_MAX] = {
	{ PD_GP_CORE_CTL, LPSC_USB_0, LPSC_MAIN_USB0_ISO, 0U	},
	{ PD_GP_CORE_CTL, LPSC_USB_1, LPSC_MAIN_USB1_ISO, 0U	},
};

u32 num_usb_lpscs = sizeof(usb_lpscs) / sizeof(struct usb_lpsc);
