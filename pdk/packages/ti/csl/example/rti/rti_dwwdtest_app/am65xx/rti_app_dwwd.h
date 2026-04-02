/*
 *   Copyright (c) Texas Instruments Incorporated 2020
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
#ifndef RTI_APP_DWWD_H_
#define RTI_APP_DWWD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <ti/csl/soc/cslr_soc_intr.h>
#include <ti/csl/soc/cslr_soc_ctrl_mmr.h>

#ifdef BUILD_MPU1_0
/**< RTI Instance */
#define APP_RTI_DWWD_WINDOW_SIZE         (RTI_RTIDWWDSIZECTRL_DWWDSIZE_100_PERCENT)
/**< DWWD Window Size */
#define APP_RTI_DWWD_TIMEOUT_VALUE       (10000U)
/**< DWWD Time Out Value */
#define APP_RTI_DWWD_REACTION            (RTI_RTIDWWDRXNCTRL_DWWDRXN_INTERRUPT)
/**< DWWD Reaction After Timeout */
#define RTI_CLOCK_SOURCE_32KHZ_FREQ_KHZ  (32U)
/**< RTI Clock Source Selection */

#define RTI_APP_CTRL_MMR_CFG_BASE        (CSL_CTRL_MMR0_CFG0_BASE)
/**< RTI CTRL MMR Config Base address */

#define RTI_APP_RTI_CFG_BASE             (CSL_RTI0_CFG_BASE)
/**< RTI Config Base address */

#define RTI_APP_CTRL_MMR_CLKSEL_OFFSET   (CSL_MAIN_CTRL_MMR_CFG0_RTI0_CLKSEL)
/**< RTI Clock select Register offset */

#define RTI_APP_RTI_CLK_SEL_FIELD_MASK   (CSL_MAIN_CTRL_MMR_CFG0_RTI0_CLKSEL_CLK_SEL_MASK)
/**< RTI CLock select Register field mask */

#define RTI_APP_RTI_CLK_SEL_FIELD_SHIFT  (CSL_MAIN_CTRL_MMR_CFG0_RTI0_CLKSEL_CLK_SEL_SHIFT)
/**< RTI CLock select Register field shift */

#define RTI_APP_WDT_INT_NUM              (CSL_GIC0_INTR_RTI0_BUS_INTR_WWD)
/**< RTI WWD interrupt number*/

#endif

#ifdef BUILD_MCU1_0
/**< RTI Instance */
#define APP_RTI_DWWD_WINDOW_SIZE         (RTI_RTIDWWDSIZECTRL_DWWDSIZE_100_PERCENT)
/**< DWWD Window Size */
#define APP_RTI_DWWD_TIMEOUT_VALUE       (10000U)
/**< DWWD Time Out Value */
#define APP_RTI_DWWD_REACTION            (RTI_RTIDWWDRXNCTRL_DWWDRXN_INTERRUPT)
/**< DWWD Reaction After Timeout */
#define RTI_CLOCK_SOURCE_32KHZ_FREQ_KHZ  (32U)
/**< RTI Clock Source Selection */

#define RTI_APP_CTRL_MMR_CFG_BASE        (CSL_MCU_CTRL_MMR0_CFG0_BASE)
/**< RTI CTRL MMR Config Base address */

#define RTI_APP_RTI_CFG_BASE             (CSL_MCU_RTI1_CFG_BASE)
/**< RTI Config Base address */

#if (RTI_APP_RTI_CFG_BASE == CSL_MCU_RTI0_CFG_BASE)
#define RTI_APP_CTRL_MMR_CLKSEL_OFFSET   (CSL_MCU_CTRL_MMR_CFG0_MCU_RTI0_CLKSEL)
/**< RTI Clock select Register offset */

#define RTI_APP_RTI_CLK_SEL_FIELD_MASK   (CSL_MCU_CTRL_MMR_CFG0_MCU_RTI0_CLKSEL_CLK_SEL_MASK
/**< RTI CLock select Register mask */

#define RTI_APP_RTI_CLK_SEL_FIELD_SHIFT  (CSL_MCU_CTRL_MMR_CFG0_MCU_RTI0_CLKSEL_CLK_SEL_SHIFT)
/**< RTI CLock select Register field shift */

#define RTI_APP_WDT_INT_NUM              (CSL_MCU0_INTR_RTI0_INTR_WWD)
/**< RTI WWD interrupt number*/
#endif

#if (RTI_APP_RTI_CFG_BASE == CSL_MCU_RTI1_CFG_BASE)
#define RTI_APP_CTRL_MMR_CLKSEL_OFFSET   (CSL_MCU_CTRL_MMR_CFG0_MCU_RTI1_CLKSEL)
/**< RTI Clock select Register offset */

#define RTI_APP_RTI_CLK_SEL_FIELD_MASK   (CSL_MCU_CTRL_MMR_CFG0_MCU_RTI1_CLKSEL_CLK_SEL_MASK)
/**< RTI CLock select Register field mask*/

#define RTI_APP_RTI_CLK_SEL_FIELD_SHIFT  (CSL_MCU_CTRL_MMR_CFG0_MCU_RTI1_CLKSEL_CLK_SEL_SHIFT)
/**< RTI CLock select Register field shift */

#define RTI_APP_WDT_INT_NUM              (CSL_MCU0_INTR_RTI1_INTR_WWD)
/**< RTI WWD interrupt number*/
#endif
#endif

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif /* RTI_APP_DWWD_H_ */
