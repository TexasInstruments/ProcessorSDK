/*
 *  Copyright (C) 2023 Texas Instruments Incorporated
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
 */
#ifndef CSLR_SOC_IN_H_
#define CSLR_SOC_IN_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <ti/csl/soc/am62lx/src/csl_psilcfg_thread_map.h>
#include <ti/csl/soc/am62lx/src/cslr_intr_adc0.h>
#include <ti/csl/soc/am62lx/src/cslr_intr_main_gpiomux_introuter0.h>
#include <ti/csl/soc/am62lx/src/cslr_intr_timesync_introuter0.h>
#include <ti/csl/soc/am62lx/src/cslr_intr_cpsw0.h>
#include <ti/csl/soc/am62lx/src/cslr_intr_epwm0.h>
#include <ti/csl/soc/am62lx/src/cslr_intr_epwm1.h>
#include <ti/csl/soc/am62lx/src/cslr_intr_epwm2.h>
#include <ti/csl/soc/am62lx/src/cslr_intr_gicss0.h>
#include <ti/csl/soc/am62lx/src/cslr_intr_gluelogic_epwm0_sync_muxglue.h>
#include <ti/csl/soc/am62lx/src/cslr_intr_gluelogic_intrglue_pbist_cpu_main.h>
#include <ti/csl/soc/am62lx/src/cslr_intr_gluelogic_soc_cbass_err_intr_glue.h>
#include <ti/csl/soc/am62lx/src/cslr_intr_gluelogic_socresettriggerglue.h>
#include <ti/csl/soc/am62lx/src/cslr_intr_main_vgpio_mux.h>
#include <ti/csl/soc/am62lx/src/cslr_intr_pinfunction_cp_gemac_cpts0_ts_compout.h>
#include <ti/csl/soc/am62lx/src/cslr_intr_pinfunction_cp_gemac_cpts0_ts_syncout.h>
#include <ti/csl/soc/am62lx/src/cslr_intr_pinfunction_sync0_outout.h>
#include <ti/csl/soc/am62lx/src/cslr_intr_pinfunction_sync1_outout.h>
#include <ti/csl/soc/am62lx/src/cslr_intr_pinfunction_sync2_outout.h>
#include <ti/csl/soc/am62lx/src/cslr_intr_pinfunction_sync3_outout.h>
#include <ti/csl/soc/am62lx/src/cslr_intr_pdma0.h>
#include <ti/csl/soc/am62lx/src/cslr_intr_pdma1.h>
#include <ti/csl/soc/am62lx/src/cslr_intr_pdma2.h>
#include <ti/csl/soc/am62lx/src/cslr_intr_pdma3.h>
#include <ti/csl/soc/am62lx/src/cslr_intr_usb0.h>
#include <ti/csl/soc/am62lx/src/cslr_intr_usb1.h>
#include <ti/csl/soc/am62lx/src/cslr_intr_wkup_deepsleep_sources0.h>
#include <ti/csl/soc/am62lx/src/cslr_soc_baseaddress.h>
#include <ti/csl/soc/am62lx/src/cslr_dmass0_baseaddress.h>
#include <ti/csl/soc/am62lx/src/cslr_wkup_dmass0_baseaddress.h>
#include <ti/csl/soc/am62lx/src/cslr_soc_ecc_aggr.h>
#include <ti/csl/soc/am62lx/src/cslr_soc.h>
#include <ti/csl/soc/am62lx/src/cslr_soc_intr.h>
#include <ti/csl/soc/am62lx/src/cslr_soc_pbist.h>
#include <ti/csl/soc/am62lx/src/csl_soc_firewalls.h>
#include <ti/csl/soc/am62lx/src/csl_soc_isc.h>
#include <ti/csl/soc/am62lx/src/csl_soc_psc.h>
#include <ti/csl/soc/am62lx/src/csl_soc_qos.h>
#ifndef ROM_BUILD
#include <ti/csl/soc/am62lx/src/cslr_soc_ctrl_mmr.h>
#endif

#ifdef __cplusplus
}
#endif
#endif /* CSLR_SOC_IN_H_ */
