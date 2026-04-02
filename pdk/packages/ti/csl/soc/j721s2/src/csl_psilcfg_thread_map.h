/* =============================================================================
 *   Copyright (c) 2020-2021 Texas Instruments Incorporated
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

#ifndef CSL_PSILCFG_THREAD_MAP_H_
#define CSL_PSILCFG_THREAD_MAP_H_

/*-----------------------------------------------------------------------------
 *  The following nomenclature is used in this file:
 *
 *  o #define CSL_PSILCFG_NAVSS_MAIN_SAUL0_PSILS_THREAD_OFFSET      (0x4000U)
 *                                             -
 *      "S" means a thread source (from the perspective of navss)
 *
 *  o #define CSL_PSILCFG_NAVSS_MAIN_SAUL0_PSILD_THREAD_CNT         (2U)
 *                                             -
 *      "D" means a thread destination (from the perspective of navss)
 *
 *  o #define CSL_PSILCFG_NAVSS_MAIN_SAUL0_PSILS_THREAD_CNT         (4U)
 *                                               ----------
 *      "THREAD_CNT" is the thread count for the specified thread
 *      source or destination
 *
 *  o #define CSL_PSILCFG_NAVSS_MAIN_SAUL0_PSILD_THREAD_OFFSET      (CSL_PSILCFG_DEST_THREAD_OFFSET | CSL_PSILCFG_NAVSS_MAIN_SAUL0_PSILS_THREAD_OFFSET)
 *                                               -------------
 *      "THREAD_OFFSET" is the starting thread # for the specified thread
 *      source or destination of the PSI-L endpoint
 *---------------------------------------------------------------------------*/

/** \brief Destination thread offset */
#define CSL_PSILCFG_DEST_THREAD_OFFSET      (0x8000U)

/**
 *  \anchor CSL_PsilcfgThreadMap
 *  \name PSIL thread map for main and mcu navss
 *
 *  Map of all PSIL threads
 *
 *  @{
 */
/*=============================================================================
 * navss_main thread map
 *===========================================================================*/
/*-----------------------------------------------------------------------------
 * PSILCFG0_CFGSTRM PSIL Map
 *---------------------------------------------------------------------------*/
#define CSL_PSILCFG_NAVSS_MAIN_PSILCFG0_CFGSTRM_PSILS_THREAD_OFFSET             (0x0000U)
#define CSL_PSILCFG_NAVSS_MAIN_PSILCFG0_CFGSTRM_PSILS_THREAD_CNT                (1U)
#define CSL_PSILCFG_NAVSS_MAIN_PSILCFG0_CFGSTRM_PSILD_THREAD_OFFSET             (CSL_PSILCFG_DEST_THREAD_OFFSET | CSL_PSILCFG_NAVSS_MAIN_PSILCFG0_CFGSTRM_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MAIN_PSILCFG0_CFGSTRM_PSILD_THREAD_CNT                (0U)
/* UDMAC0_TCFGSTRM Map (for backwards compatibility with prior thread map constants) */
#define CSL_PSILCFG_NAVSS_MAIN_UDMAC0_TCFGSTRM_THREAD_OFFSET                    (CSL_PSILCFG_NAVSS_MAIN_PSILCFG0_CFGSTRM_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MAIN_UDMAC0_TCFGSTRM_THREAD_CNT                       (CSL_PSILCFG_NAVSS_MAIN_PSILCFG0_CFGSTRM_PSILS_THREAD_CNT)
/* UDMAC0_RCFGSTRM Map (for backwards compatibility with prior thread map constants) */
#define CSL_PSILCFG_NAVSS_MAIN_UDMAC0_RCFGSTRM_THREAD_OFFSET                    (CSL_PSILCFG_NAVSS_MAIN_PSILCFG0_CFGSTRM_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MAIN_UDMAC0_RCFGSTRM_THREAD_CNT                       (CSL_PSILCFG_NAVSS_MAIN_PSILCFG0_CFGSTRM_PSILS_THREAD_CNT)
/*-----------------------------------------------------------------------------
 * UDMAP0_TRSTRM PSIL Map
 *---------------------------------------------------------------------------*/
#define CSL_PSILCFG_NAVSS_MAIN_UDMAP0_TRSTRM_PSILS_THREAD_OFFSET                (0x0008U)
#define CSL_PSILCFG_NAVSS_MAIN_UDMAP0_TRSTRM_PSILS_THREAD_CNT                   (1U)
#define CSL_PSILCFG_NAVSS_MAIN_UDMAP0_TRSTRM_PSILD_THREAD_OFFSET                (CSL_PSILCFG_DEST_THREAD_OFFSET | CSL_PSILCFG_NAVSS_MAIN_UDMAP0_TRSTRM_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MAIN_UDMAP0_TRSTRM_PSILD_THREAD_CNT                   (0U)
/* UDMAC0_TTRSTRM Map (for backwards compatibility with prior thread map constants) */
#define CSL_PSILCFG_NAVSS_MAIN_UDMAC0_TTRSTRM_THREAD_OFFSET                     (CSL_PSILCFG_NAVSS_MAIN_UDMAP0_TRSTRM_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MAIN_UDMAC0_TTRSTRM_THREAD_CNT                        (CSL_PSILCFG_NAVSS_MAIN_UDMAP0_TRSTRM_PSILS_THREAD_CNT)
/* UDMAC0_RTRSTRM Map (for backwards compatibility with prior thread map constants) */
#define CSL_PSILCFG_NAVSS_MAIN_UDMAC0_RTRSTRM_THREAD_OFFSET                     (CSL_PSILCFG_NAVSS_MAIN_UDMAP0_TRSTRM_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MAIN_UDMAC0_RTRSTRM_THREAD_CNT                        (CSL_PSILCFG_NAVSS_MAIN_UDMAP0_TRSTRM_PSILS_THREAD_CNT)
/*-----------------------------------------------------------------------------
 * UDMAP0_CFGSTRM PSIL Map
 *---------------------------------------------------------------------------*/
#define CSL_PSILCFG_NAVSS_MAIN_UDMAP0_CFGSTRM_PSILS_THREAD_OFFSET               (0x0020U)
#define CSL_PSILCFG_NAVSS_MAIN_UDMAP0_CFGSTRM_PSILS_THREAD_CNT                  (1U)
#define CSL_PSILCFG_NAVSS_MAIN_UDMAP0_CFGSTRM_PSILD_THREAD_OFFSET               (CSL_PSILCFG_DEST_THREAD_OFFSET | CSL_PSILCFG_NAVSS_MAIN_UDMAP0_CFGSTRM_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MAIN_UDMAP0_CFGSTRM_PSILD_THREAD_CNT                  (0U)
/*-----------------------------------------------------------------------------
 * UDMAP0_STRM PSIL Map
 *---------------------------------------------------------------------------*/
#define CSL_PSILCFG_NAVSS_MAIN_UDMAP0_STRM_PSILS_THREAD_OFFSET                  (0x1000U)
#define CSL_PSILCFG_NAVSS_MAIN_UDMAP0_STRM_PSILS_THREAD_CNT                     (140U)
#define CSL_PSILCFG_NAVSS_MAIN_UDMAP0_STRM_PSILD_THREAD_OFFSET                  (CSL_PSILCFG_DEST_THREAD_OFFSET | CSL_PSILCFG_NAVSS_MAIN_UDMAP0_STRM_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MAIN_UDMAP0_STRM_PSILD_THREAD_CNT                     (140U)
/* UDMAP0_TSTRM Map (for backwards compatibility with prior thread map constants) */
#define CSL_PSILCFG_NAVSS_MAIN_UDMAP0_TSTRM_THREAD_OFFSET                       (CSL_PSILCFG_NAVSS_MAIN_UDMAP0_STRM_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MAIN_UDMAP0_TSTRM_THREAD_CNT                          (CSL_PSILCFG_NAVSS_MAIN_UDMAP0_STRM_PSILS_THREAD_CNT)
/* UDMAP0_RSTRM Map for backwards compatibility with prior thread map constants) */
#define CSL_PSILCFG_NAVSS_MAIN_UDMAP0_RSTRM_THREAD_OFFSET                       (CSL_PSILCFG_NAVSS_MAIN_UDMAP0_STRM_PSILD_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MAIN_UDMAP0_RSTRM_THREAD_CNT                          (CSL_PSILCFG_NAVSS_MAIN_UDMAP0_STRM_PSILD_THREAD_CNT)
/*-----------------------------------------------------------------------------
 * BCDMA0_CFGSTRM PSIL Map
 *---------------------------------------------------------------------------*/
#define CSL_PSILCFG_NAVSS_MAIN_BCDMA0_CFGSTRM_PSILS_THREAD_OFFSET               (0x0022U)
#define CSL_PSILCFG_NAVSS_MAIN_BCDMA0_CFGSTRM_PSILS_THREAD_CNT                  (1U)
#define CSL_PSILCFG_NAVSS_MAIN_BCDMA0_CFGSTRM_PSILD_THREAD_OFFSET               (CSL_PSILCFG_DEST_THREAD_OFFSET | CSL_PSILCFG_NAVSS_MAIN_BCDMA0_CFGSTRM_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MAIN_BCDMA0_CFGSTRM_PSILD_THREAD_CNT                  (0U)
/*-----------------------------------------------------------------------------
 * BCDMA0_STRM PSIL Map
 *---------------------------------------------------------------------------*/
#define CSL_PSILCFG_NAVSS_MAIN_BCDMA0_STRM_PSILS_THREAD_OFFSET                  (0x2000U)
#define CSL_PSILCFG_NAVSS_MAIN_BCDMA0_STRM_PSILS_THREAD_CNT                     (16U)
#define CSL_PSILCFG_NAVSS_MAIN_BCDMA0_STRM_PSILD_THREAD_OFFSET                  (CSL_PSILCFG_DEST_THREAD_OFFSET | CSL_PSILCFG_NAVSS_MAIN_BCDMA0_STRM_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MAIN_BCDMA0_STRM_PSILD_THREAD_CNT                     (32U)
/*-----------------------------------------------------------------------------
 * PDMA_MAIN_DEBUG PSIL Map
 *---------------------------------------------------------------------------*/
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_DEBUG_PSILS_THREAD_OFFSET              (0x4300U)
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_DEBUG_PSILS_THREAD_CNT                 (4U)
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_DEBUG_PSILD_THREAD_OFFSET              (CSL_PSILCFG_DEST_THREAD_OFFSET | CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_DEBUG_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_DEBUG_PSILD_THREAD_CNT                 (0U)
/*-----------------------------------------------------------------------------
 * PDMA_MAIN_DEBUG_G0 PSIL Map
 *---------------------------------------------------------------------------*/
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_DEBUG_G0_PSILS_THREAD_OFFSET           (0x4300U)
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_DEBUG_G0_PSILS_THREAD_CNT              (2U)
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_DEBUG_G0_PSILD_THREAD_OFFSET           (CSL_PSILCFG_DEST_THREAD_OFFSET | CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_DEBUG_G0_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_DEBUG_G0_PSILD_THREAD_CNT              (0U)
/*-----------------------------------------------------------------------------
 * PDMA_MAIN_DEBUG_G1 PSIL Map
 *---------------------------------------------------------------------------*/
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_DEBUG_G1_PSILS_THREAD_OFFSET           (0x4304U)
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_DEBUG_G1_PSILS_THREAD_CNT              (2U)
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_DEBUG_G1_PSILD_THREAD_OFFSET           (CSL_PSILCFG_DEST_THREAD_OFFSET | CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_DEBUG_G1_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_DEBUG_G1_PSILD_THREAD_CNT              (0U)
/*-----------------------------------------------------------------------------
 * PDMA_MAIN_MCASP PSIL Map
 *---------------------------------------------------------------------------*/
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_MCASP_PSILS_THREAD_OFFSET              (0x4400U)
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_MCASP_PSILS_THREAD_CNT                 (5U)
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_MCASP_PSILD_THREAD_OFFSET              (CSL_PSILCFG_DEST_THREAD_OFFSET | CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_MCASP_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_MCASP_PSILD_THREAD_CNT                 (5U)
/*-----------------------------------------------------------------------------
 * PDMA_MAIN_MISC PSIL Map
 *---------------------------------------------------------------------------*/
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_MISC_PSILS_THREAD_OFFSET               (0x4600U)
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_MISC_PSILS_THREAD_CNT                  (256U)
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_MISC_PSILD_THREAD_OFFSET               (CSL_PSILCFG_DEST_THREAD_OFFSET | CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_MISC_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_MISC_PSILD_THREAD_CNT                  (256U)
/*-----------------------------------------------------------------------------
 * PDMA_MAIN_SPI_G0 PSIL Map
 *---------------------------------------------------------------------------*/
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G0_PSILS_THREAD_OFFSET             (0x4600U)
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G0_PSILS_THREAD_CNT                (16U)
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G0_PSILD_THREAD_OFFSET             (CSL_PSILCFG_DEST_THREAD_OFFSET | CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G0_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G0_PSILD_THREAD_CNT                (16U)
/*-----------------------------------------------------------------------------
 * PDMA_MAIN_SPI_G1 PSIL Map
 *---------------------------------------------------------------------------*/
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G1_PSILS_THREAD_OFFSET             (0x4620U)
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G1_PSILS_THREAD_CNT                (16U)
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G1_PSILD_THREAD_OFFSET             (CSL_PSILCFG_DEST_THREAD_OFFSET | CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G1_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G1_PSILD_THREAD_CNT                (16U)
/*-----------------------------------------------------------------------------
 * CPSW2 PSIL Map
 *---------------------------------------------------------------------------*/
#define CSL_PSILCFG_NAVSS_MAIN_CPSW2_PSILS_THREAD_OFFSET                        (0x4640U)
#define CSL_PSILCFG_NAVSS_MAIN_CPSW2_PSILS_THREAD_CNT                           (1U)
#define CSL_PSILCFG_NAVSS_MAIN_CPSW2_PSILD_THREAD_OFFSET                        (CSL_PSILCFG_DEST_THREAD_OFFSET | CSL_PSILCFG_NAVSS_MAIN_CPSW2_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MAIN_CPSW2_PSILD_THREAD_CNT                           (8U)
/*-----------------------------------------------------------------------------
 * PDMA_MAIN_USART PSIL Map
 *---------------------------------------------------------------------------*/
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET              (0x4700U)
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_CNT                 (64U)
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET              (CSL_PSILCFG_DEST_THREAD_OFFSET | CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_CNT                 (64U)
/*-----------------------------------------------------------------------------
 * PDMA_MAIN_USART_G0 PSIL Map
 *---------------------------------------------------------------------------*/
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_G0_PSILS_THREAD_OFFSET           (0x4700U)
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_G0_PSILS_THREAD_CNT              (2U)
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_G0_PSILD_THREAD_OFFSET           (CSL_PSILCFG_DEST_THREAD_OFFSET | CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_G0_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_G0_PSILD_THREAD_CNT              (2U)
/*-----------------------------------------------------------------------------
 * PDMA_MAIN_USART_G1 PSIL Map
 *---------------------------------------------------------------------------*/
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_G1_PSILS_THREAD_OFFSET           (0x4702U)
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_G1_PSILS_THREAD_CNT              (2U)
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_G1_PSILD_THREAD_OFFSET           (CSL_PSILCFG_DEST_THREAD_OFFSET | CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_G1_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_G1_PSILD_THREAD_CNT              (2U)
/*-----------------------------------------------------------------------------
 * PDMA_MAIN_USART_G2 PSIL Map
 *---------------------------------------------------------------------------*/
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_G2_PSILS_THREAD_OFFSET           (0x4704U)
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_G2_PSILS_THREAD_CNT              (6U)
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_G2_PSILD_THREAD_OFFSET           (CSL_PSILCFG_DEST_THREAD_OFFSET | CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_G2_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_G2_PSILD_THREAD_CNT              (6U)
/*-----------------------------------------------------------------------------
 * PDMA_MAIN_MCAN PSIL Map
 *---------------------------------------------------------------------------*/
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_MCAN_PSILS_THREAD_OFFSET               (0x470AU)
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_MCAN_PSILS_THREAD_CNT                  (54U)
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_MCAN_PSILD_THREAD_OFFSET               (CSL_PSILCFG_DEST_THREAD_OFFSET | CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_MCAN_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_MCAN_PSILD_THREAD_CNT                  (54U)
/*-----------------------------------------------------------------------------
 * MSMC0 PSIL Map
 *---------------------------------------------------------------------------*/
#define CSL_PSILCFG_NAVSS_MAIN_MSMC0_PSILS_THREAD_OFFSET                        (0x4800U)
#define CSL_PSILCFG_NAVSS_MAIN_MSMC0_PSILS_THREAD_CNT                           (32U)
#define CSL_PSILCFG_NAVSS_MAIN_MSMC0_PSILD_THREAD_OFFSET                        (CSL_PSILCFG_DEST_THREAD_OFFSET | CSL_PSILCFG_NAVSS_MAIN_MSMC0_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MAIN_MSMC0_PSILD_THREAD_CNT                           (32U)
/*-----------------------------------------------------------------------------
 * VPAC_DMPAC PSIL Map
 *---------------------------------------------------------------------------*/
#define CSL_PSILCFG_NAVSS_MAIN_VPAC_DMPAC_PSILS_THREAD_OFFSET                   (0x4820U)
#define CSL_PSILCFG_NAVSS_MAIN_VPAC_DMPAC_PSILS_THREAD_CNT                      (224U)
#define CSL_PSILCFG_NAVSS_MAIN_VPAC_DMPAC_PSILD_THREAD_OFFSET                   (CSL_PSILCFG_DEST_THREAD_OFFSET | CSL_PSILCFG_NAVSS_MAIN_VPAC_DMPAC_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MAIN_VPAC_DMPAC_PSILD_THREAD_CNT                      (224U)
/*-----------------------------------------------------------------------------
 * VPAC_TC0_CC PSIL Map (VPAC0)
 *---------------------------------------------------------------------------*/
#define CSL_PSILCFG_NAVSS_MAIN_VPAC_TC0_CC_PSILS_THREAD_OFFSET                  (0x4820U)
#define CSL_PSILCFG_NAVSS_MAIN_VPAC_TC0_CC_PSILS_THREAD_CNT                     (32U)
#define CSL_PSILCFG_NAVSS_MAIN_VPAC_TC0_CC_PSILD_THREAD_OFFSET                  (CSL_PSILCFG_DEST_THREAD_OFFSET | CSL_PSILCFG_NAVSS_MAIN_VPAC_TC0_CC_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MAIN_VPAC_TC0_CC_PSILD_THREAD_CNT                     (32U)
/*-----------------------------------------------------------------------------
 * VPAC_TC1_CC PSIL Map (VPAC0)
 *---------------------------------------------------------------------------*/
#define CSL_PSILCFG_NAVSS_MAIN_VPAC_TC1_CC_PSILS_THREAD_OFFSET                  (0x4840U)
#define CSL_PSILCFG_NAVSS_MAIN_VPAC_TC1_CC_PSILS_THREAD_CNT                     (64U)
#define CSL_PSILCFG_NAVSS_MAIN_VPAC_TC1_CC_PSILD_THREAD_OFFSET                  (CSL_PSILCFG_DEST_THREAD_OFFSET | CSL_PSILCFG_NAVSS_MAIN_VPAC_TC1_CC_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MAIN_VPAC_TC1_CC_PSILD_THREAD_CNT                     (64U)
/*-----------------------------------------------------------------------------
 * VPAC1_TC0_CC PSIL Map
 *---------------------------------------------------------------------------*/
#define CSL_PSILCFG_NAVSS_MAIN_VPAC1_TC0_CC_PSILS_THREAD_OFFSET                 (0x4880U)
#define CSL_PSILCFG_NAVSS_MAIN_VPAC1_TC0_CC_PSILS_THREAD_CNT                    (32U)
#define CSL_PSILCFG_NAVSS_MAIN_VPAC1_TC0_CC_PSILD_THREAD_OFFSET                 (CSL_PSILCFG_DEST_THREAD_OFFSET | CSL_PSILCFG_NAVSS_MAIN_VPAC1_TC0_CC_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MAIN_VPAC1_TC0_CC_PSILD_THREAD_CNT                    (32U)
/*-----------------------------------------------------------------------------
 * VPAC1_TC1_CC PSIL Map
 *---------------------------------------------------------------------------*/
#define CSL_PSILCFG_NAVSS_MAIN_VPAC1_TC1_CC_PSILS_THREAD_OFFSET                 (0x48A0U)
#define CSL_PSILCFG_NAVSS_MAIN_VPAC1_TC1_CC_PSILS_THREAD_CNT                    (64U)
#define CSL_PSILCFG_NAVSS_MAIN_VPAC1_TC1_CC_PSILD_THREAD_OFFSET                 (CSL_PSILCFG_DEST_THREAD_OFFSET | CSL_PSILCFG_NAVSS_MAIN_VPAC1_TC1_CC_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MAIN_VPAC1_TC1_CC_PSILD_THREAD_CNT                    (64U)
/*-----------------------------------------------------------------------------
 * DMPAC_TC0_CC PSIL Map
 *---------------------------------------------------------------------------*/
#define CSL_PSILCFG_NAVSS_MAIN_DMPAC_TC0_CC_PSILS_THREAD_OFFSET                 (0x48E0U)
#define CSL_PSILCFG_NAVSS_MAIN_DMPAC_TC0_CC_PSILS_THREAD_CNT                    (32U)
#define CSL_PSILCFG_NAVSS_MAIN_DMPAC_TC0_CC_PSILD_THREAD_OFFSET                 (CSL_PSILCFG_DEST_THREAD_OFFSET | CSL_PSILCFG_NAVSS_MAIN_DMPAC_TC0_CC_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MAIN_DMPAC_TC0_CC_PSILD_THREAD_CNT                    (32U)
/*-----------------------------------------------------------------------------
 * CSI PSIL Map
 *---------------------------------------------------------------------------*/
#define CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET                          (0x4900U)
#define CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_CNT                             (256U)
#define CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET                          (CSL_PSILCFG_DEST_THREAD_OFFSET | CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_CNT                             (256U)
/*-----------------------------------------------------------------------------
 * CPSW PSIL Map
 *---------------------------------------------------------------------------*/
#define CSL_PSILCFG_NAVSS_MAIN_CPSW_PSILS_THREAD_OFFSET                         (0x4A00U)
#define CSL_PSILCFG_NAVSS_MAIN_CPSW_PSILS_THREAD_CNT                            (256U)
#define CSL_PSILCFG_NAVSS_MAIN_CPSW_PSILD_THREAD_OFFSET                         (CSL_PSILCFG_DEST_THREAD_OFFSET | CSL_PSILCFG_NAVSS_MAIN_CPSW_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MAIN_CPSW_PSILD_THREAD_CNT                            (256U)
/*-----------------------------------------------------------------------------
 * SAUL0 PSIL Map
 *---------------------------------------------------------------------------*/
#define CSL_PSILCFG_NAVSS_MAIN_SAUL0_PSILS_THREAD_OFFSET                        (0x4A40U)
#define CSL_PSILCFG_NAVSS_MAIN_SAUL0_PSILS_THREAD_CNT                           (16U)
#define CSL_PSILCFG_NAVSS_MAIN_SAUL0_PSILD_THREAD_OFFSET                        (CSL_PSILCFG_DEST_THREAD_OFFSET | CSL_PSILCFG_NAVSS_MAIN_SAUL0_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MAIN_SAUL0_PSILD_THREAD_CNT                           (8U)
/*-----------------------------------------------------------------------------
 * CPSW5 PSIL Map
 *---------------------------------------------------------------------------*/
#define CSL_PSILCFG_NAVSS_MAIN_CPSW5_PSILS_THREAD_OFFSET                        (0x4A00U)
#define CSL_PSILCFG_NAVSS_MAIN_CPSW5_PSILS_THREAD_CNT                           (1U)
#define CSL_PSILCFG_NAVSS_MAIN_CPSW5_PSILD_THREAD_OFFSET                        (CSL_PSILCFG_DEST_THREAD_OFFSET | CSL_PSILCFG_NAVSS_MAIN_CPSW5_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MAIN_CPSW5_PSILD_THREAD_CNT                           (8U)
/*-----------------------------------------------------------------------------
 * NAVSS_MCU PSIL Map (for backwards compatibility with prior thread map constants)
 *---------------------------------------------------------------------------*/
#define CSL_PSILCFG_NAVSS_MAIN_NAVSS_MCU_PSILS_THREAD_OFFSET                    (0x6000U)
#define CSL_PSILCFG_NAVSS_MAIN_NAVSS_MCU_PSILS_THREAD_CNT                       (8192U)
#define CSL_PSILCFG_NAVSS_MAIN_NAVSS_MCU_PSILD_THREAD_OFFSET                    (CSL_PSILCFG_DEST_THREAD_OFFSET | CSL_PSILCFG_NAVSS_MAIN_NAVSS_MCU_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MAIN_NAVSS_MCU_PSILD_THREAD_CNT                       (8192U)

/*=============================================================================
 * navss_mcu thread map
 *===========================================================================*/
/*-----------------------------------------------------------------------------
 * PSILCFG0_CFGSTRM PSIL Map
 *---------------------------------------------------------------------------*/
#define CSL_PSILCFG_NAVSS_MCU_PSILCFG0_CFGSTRM_PSILS_THREAD_OFFSET              (0x0001U)
#define CSL_PSILCFG_NAVSS_MCU_PSILCFG0_CFGSTRM_PSILS_THREAD_CNT                 (1U)
#define CSL_PSILCFG_NAVSS_MCU_PSILCFG0_CFGSTRM_PSILD_THREAD_OFFSET              (CSL_PSILCFG_DEST_THREAD_OFFSET | CSL_PSILCFG_NAVSS_MCU_PSILCFG0_CFGSTRM_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MCU_PSILCFG0_CFGSTRM_PSILD_THREAD_CNT                 (0U)
/* CFG_PROXY Map (for backwards compatibility with prior thread map constants) */
#define CSL_PSILCFG_NAVSS_MCU_CFG_PROXY_THREAD_OFFSET                           (CSL_PSILCFG_NAVSS_MCU_PSILCFG0_CFGSTRM_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MCU_CFG_PROXY_THREAD_CNT                              (CSL_PSILCFG_NAVSS_MCU_PSILCFG0_CFGSTRM_PSILS_THREAD_CNT)
/*-----------------------------------------------------------------------------
 * PSILCFG0_CFGSTRM PSIL Map
 *---------------------------------------------------------------------------*/
#define CSL_PSILCFG_NAVSS_MCU_UDMAP0_CFGSTRM_PSILS_THREAD_OFFSET                (0x0021U)
#define CSL_PSILCFG_NAVSS_MCU_UDMAP0_CFGSTRM_PSILS_THREAD_CNT                   (1U)
#define CSL_PSILCFG_NAVSS_MCU_UDMAP0_CFGSTRM_PSILD_THREAD_OFFSET                (CSL_PSILCFG_DEST_THREAD_OFFSET | CSL_PSILCFG_NAVSS_MCU_UDMAP0_CFGSTRM_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MCU_UDMAP0_CFGSTRM_PSILD_THREAD_CNT                   (0U)
/* UDMAC0_TTRSTRM Map (for backwards compatibility with prior thread map constants) */
#define CSL_PSILCFG_NAVSS_MCU_UDMAC0_TTRSTRM_THREAD_OFFSET                      (CSL_PSILCFG_NAVSS_MCU_UDMAP0_CFGSTRM_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MCU_UDMAC0_TTRSTRM_THREAD_CNT                         (CSL_PSILCFG_NAVSS_MCU_UDMAP0_CFGSTRM_PSILS_THREAD_CNT)
/* UDMAC0_RTRSTRM Map (for backwards compatibility with prior thread map constants) */
#define CSL_PSILCFG_NAVSS_MCU_UDMAC0_RTRSTRM_THREAD_OFFSET                      (CSL_PSILCFG_NAVSS_MCU_UDMAP0_CFGSTRM_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MCU_UDMAC0_RTRSTRM_THREAD_CNT                         (CSL_PSILCFG_NAVSS_MCU_UDMAP0_CFGSTRM_PSILS_THREAD_CNT)
/*-----------------------------------------------------------------------------
 * NAVSS PSIL Map (for backwards compatibility with prior thread map constants)
 *---------------------------------------------------------------------------*/
#define CSL_PSILCFG_NAVSS_MCU_NAVSS_PSILS_THREAD_OFFSET                         (0x1000U)
#define CSL_PSILCFG_NAVSS_MCU_NAVSS_PSILS_THREAD_CNT                            (16384U)
#define CSL_PSILCFG_NAVSS_MCU_NAVSS_PSILD_THREAD_OFFSET                         (CSL_PSILCFG_DEST_THREAD_OFFSET | CSL_PSILCFG_NAVSS_MCU_NAVSS_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MCU_NAVSS_PSILD_THREAD_CNT                            (16384U)
/*-----------------------------------------------------------------------------
 * UDMAP0_STRM PSIL Map
 *---------------------------------------------------------------------------*/
#define CSL_PSILCFG_NAVSS_MCU_UDMAP0_STRM_PSILS_THREAD_OFFSET                   (0x6000U)
#define CSL_PSILCFG_NAVSS_MCU_UDMAP0_STRM_PSILS_THREAD_CNT                      (48U)
#define CSL_PSILCFG_NAVSS_MCU_UDMAP0_STRM_PSILD_THREAD_OFFSET                   (CSL_PSILCFG_DEST_THREAD_OFFSET | CSL_PSILCFG_NAVSS_MCU_UDMAP0_STRM_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MCU_UDMAP0_STRM_PSILD_THREAD_CNT                      (48U)
/* UDMAP0_TSTRM (for backwards compatibility with prior thread map constants) / UDMAP0_STRM PSIL Slave Map */
#define CSL_PSILCFG_NAVSS_MCU_UDMAP0_TSTRM_THREAD_OFFSET                        (CSL_PSILCFG_NAVSS_MCU_UDMAP0_STRM_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MCU_UDMAP0_TSTRM_THREAD_CNT                           (CSL_PSILCFG_NAVSS_MCU_UDMAP0_STRM_PSILS_THREAD_CNT)
/* UDMAP0 RSTRM (for backwards compatibility with prior thread map constants) / UDMAP0_STRM PSIL Master Map */
#define CSL_PSILCFG_NAVSS_MCU_UDMAP0_RSTRM_THREAD_OFFSET                        (CSL_PSILCFG_NAVSS_MCU_UDMAP0_STRM_PSILD_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MCU_UDMAP0_RSTRM_THREAD_CNT                           (CSL_PSILCFG_NAVSS_MCU_UDMAP0_STRM_PSILD_THREAD_CNT)
/*-----------------------------------------------------------------------------
 * CPSW0 PSIL Map
 *---------------------------------------------------------------------------*/
#define CSL_PSILCFG_NAVSS_MCU_CPSW0_PSILS_THREAD_OFFSET                         (0x7000U)
#define CSL_PSILCFG_NAVSS_MCU_CPSW0_PSILS_THREAD_CNT                            (1U)
#define CSL_PSILCFG_NAVSS_MCU_CPSW0_PSILD_THREAD_OFFSET                         (CSL_PSILCFG_DEST_THREAD_OFFSET | CSL_PSILCFG_NAVSS_MCU_CPSW0_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MCU_CPSW0_PSILD_THREAD_CNT                            (8U)
/*-----------------------------------------------------------------------------
 * PDMA_MCU0 PSIL Map
 *---------------------------------------------------------------------------*/
#define CSL_PSILCFG_NAVSS_MCU_PDMA_MCU0_PSILS_THREAD_OFFSET                     (0x7100U)
#define CSL_PSILCFG_NAVSS_MCU_PDMA_MCU0_PSILS_THREAD_CNT                        (7U)
#define CSL_PSILCFG_NAVSS_MCU_PDMA_MCU0_PSILD_THREAD_OFFSET                     (CSL_PSILCFG_DEST_THREAD_OFFSET | CSL_PSILCFG_NAVSS_MCU_PDMA_MCU0_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MCU_PDMA_MCU0_PSILD_THREAD_CNT                        (7U)
/*-----------------------------------------------------------------------------
 * PDMA_MCU1 PSIL Map
 *---------------------------------------------------------------------------*/
#define CSL_PSILCFG_NAVSS_MCU_PDMA_MCU1_PSILS_THREAD_OFFSET                     (0x7200U)
#define CSL_PSILCFG_NAVSS_MCU_PDMA_MCU1_PSILS_THREAD_CNT                        (8U)
#define CSL_PSILCFG_NAVSS_MCU_PDMA_MCU1_PSILD_THREAD_OFFSET                     (CSL_PSILCFG_DEST_THREAD_OFFSET | CSL_PSILCFG_NAVSS_MCU_PDMA_MCU1_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MCU_PDMA_MCU1_PSILD_THREAD_CNT                        (8U)
/*-----------------------------------------------------------------------------
 * PDMA_MCU2 PSIL Map
 *---------------------------------------------------------------------------*/
#define CSL_PSILCFG_NAVSS_MCU_PDMA_MCU2_PSILS_THREAD_OFFSET                     (0x7300U)
#define CSL_PSILCFG_NAVSS_MCU_PDMA_MCU2_PSILS_THREAD_CNT                        (4U)
#define CSL_PSILCFG_NAVSS_MCU_PDMA_MCU2_PSILD_THREAD_OFFSET                     (CSL_PSILCFG_DEST_THREAD_OFFSET | CSL_PSILCFG_NAVSS_MCU_PDMA_MCU2_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MCU_PDMA_MCU2_PSILD_THREAD_CNT                        (4U)
/*-----------------------------------------------------------------------------
 * PDMA_ADC PSIL Map
 *---------------------------------------------------------------------------*/
#define CSL_PSILCFG_NAVSS_MCU_PDMA_ADC_PSILS_THREAD_OFFSET                      (0x7400U)
#define CSL_PSILCFG_NAVSS_MCU_PDMA_ADC_PSILS_THREAD_CNT                         (4U)
#define CSL_PSILCFG_NAVSS_MCU_PDMA_ADC_PSILD_THREAD_OFFSET                      (CSL_PSILCFG_DEST_THREAD_OFFSET | CSL_PSILCFG_NAVSS_MCU_PDMA_ADC_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_NAVSS_MCU_PDMA_ADC_PSILD_THREAD_CNT                         (0U)
/*-----------------------------------------------------------------------------
 * SAUL0 PSIL Map
 *---------------------------------------------------------------------------*/
#define CSL_PSILCFG_NAVSS_MCU_SAUL0_PSILS_THREAD_OFFSET                         (0x7504U)
#define CSL_PSILCFG_NAVSS_MCU_SAUL0_PSILS_THREAD_CNT                            (8U)
#define CSL_PSILCFG_NAVSS_MCU_SAUL0_PSILD_THREAD_OFFSET                         (CSL_PSILCFG_DEST_THREAD_OFFSET | 0x7500U)
#define CSL_PSILCFG_NAVSS_MCU_SAUL0_PSILD_THREAD_CNT                            (4U)

/* @} */

/**
 *  \anchor CSL_PsilcfgDmssThreadMap
 *  \name PSIL thread map for DMSS_HSM
 *
 *  Map of all DMSS_HSM PSIL threads
 *
 *  @{
 */
/*=============================================================================
 * DMSS_HSM thread map
 *===========================================================================*/
/*-----------------------------------------------------------------------------
 * PSILCFG0_CFGSTRM PSIL Map
 *---------------------------------------------------------------------------*/
#define CSL_PSILCFG_DMSS_PSILCFG0_CFGSTRM_PSILS_THREAD_OFFSET             (0x0002U)
#define CSL_PSILCFG_DMSS_PSILCFG0_CFGSTRM_PSILS_THREAD_CNT                (1U)
#define CSL_PSILCFG_DMSS_PSILCFG0_CFGSTRM_PSILD_THREAD_OFFSET             (CSL_PSILCFG_DEST_THREAD_OFFSET | CSL_PSILCFG_DMSS_PSILCFG0_CFGSTRM_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_DMSS_PSILCFG0_CFGSTRM_PSILD_THREAD_CNT                (0U)
/*-----------------------------------------------------------------------------
 * PKTDMA_CFGSTRM PSIL Map
 *---------------------------------------------------------------------------*/
#define CSL_PSILCFG_DMSS_PKTDMA_CFGSTRM_PSILS_THREAD_OFFSET               (0x0022U)
#define CSL_PSILCFG_DMSS_PKTDMA_CFGSTRM_PSILS_THREAD_CNT                  (1U)
#define CSL_PSILCFG_DMSS_PKTDMA_CFGSTRM_PSILD_THREAD_OFFSET               (CSL_PSILCFG_DEST_THREAD_OFFSET | CSL_PSILCFG_DMSS_PKTDMA_CFGSTRM_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_DMSS_PKTDMA_CFGSTRM_PSILD_THREAD_CNT                  (0U)
/*-----------------------------------------------------------------------------
 * PKTDMA_STRM PSIL Map
 *---------------------------------------------------------------------------*/
#define CSL_PSILCFG_DMSS_PKTDMA_STRM_PSILS_THREAD_OFFSET                  (0x3000U)
#define CSL_PSILCFG_DMSS_PKTDMA_STRM_PSILS_THREAD_CNT                     (2U)
#define CSL_PSILCFG_DMSS_PKTDMA_STRM_PSILD_THREAD_OFFSET                  (CSL_PSILCFG_DEST_THREAD_OFFSET | CSL_PSILCFG_DMSS_PKTDMA_STRM_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_DMSS_PKTDMA_STRM_PSILD_THREAD_CNT                     (4U)
/*-----------------------------------------------------------------------------
 * SAUL0 PSIL Map
 *---------------------------------------------------------------------------*/
#define CSL_PSILCFG_DMSS_SAUL0_PSILS_THREAD_OFFSET                        (0x74FCU)
#define CSL_PSILCFG_DMSS_SAUL0_PSILS_THREAD_CNT                           (4U)
#define CSL_PSILCFG_DMSS_SAUL0_PSILD_THREAD_OFFSET                        (CSL_PSILCFG_DEST_THREAD_OFFSET | CSL_PSILCFG_DMSS_SAUL0_PSILS_THREAD_OFFSET)
#define CSL_PSILCFG_DMSS_SAUL0_PSILD_THREAD_CNT                           (2U)

/* @} */

/**
 *  \anchor CSL_PdmaCh
 *  \name PDMA Channels
 *
 *  List of all PDMA channels across main and mcu domains
 *
 *  @{
 */

/**
 *  \anchor CSL_PdmaChMainTx
 *  \name Main TX PDMA Channels
 *
 *  List of all Main PDMA TX channels
 *
 *  @{
 */

/*
 * PDMA MAIN MCASP TX Channels
 */
#define CSL_PDMA_CH_MAIN_MCASP0_CH0_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_MCASP_PSILD_THREAD_OFFSET + 0U)
#define CSL_PDMA_CH_MAIN_MCASP1_CH0_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_MCASP_PSILD_THREAD_OFFSET + 1U)
#define CSL_PDMA_CH_MAIN_MCASP2_CH0_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_MCASP_PSILD_THREAD_OFFSET + 2U)
#define CSL_PDMA_CH_MAIN_MCASP3_CH0_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_MCASP_PSILD_THREAD_OFFSET + 3U)
#define CSL_PDMA_CH_MAIN_MCASP4_CH0_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_MCASP_PSILD_THREAD_OFFSET + 4U)
/*
 * PDMA MAIN UART TX Channels
 */
#define CSL_PDMA_CH_MAIN_UART0_CH0_TX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 0U)
#define CSL_PDMA_CH_MAIN_UART1_CH0_TX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 1U)
#define CSL_PDMA_CH_MAIN_UART2_CH0_TX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 2U)
#define CSL_PDMA_CH_MAIN_UART3_CH0_TX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 3U)
#define CSL_PDMA_CH_MAIN_UART4_CH0_TX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 4U)
#define CSL_PDMA_CH_MAIN_UART5_CH0_TX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 5U)
#define CSL_PDMA_CH_MAIN_UART6_CH0_TX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 6U)
#define CSL_PDMA_CH_MAIN_UART7_CH0_TX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 7U)
#define CSL_PDMA_CH_MAIN_UART8_CH0_TX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 8U)
#define CSL_PDMA_CH_MAIN_UART9_CH0_TX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 9U)
/*
 * PDMA MAIN MCSPI TX Channels
 */
#define CSL_PDMA_CH_MAIN_MCSPI0_CH0_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G0_PSILD_THREAD_OFFSET + 0U)
#define CSL_PDMA_CH_MAIN_MCSPI0_CH1_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G0_PSILD_THREAD_OFFSET + 1U)
#define CSL_PDMA_CH_MAIN_MCSPI0_CH2_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G0_PSILD_THREAD_OFFSET + 2U)
#define CSL_PDMA_CH_MAIN_MCSPI0_CH3_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G0_PSILD_THREAD_OFFSET + 3U)
#define CSL_PDMA_CH_MAIN_MCSPI1_CH0_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G0_PSILD_THREAD_OFFSET + 4U)
#define CSL_PDMA_CH_MAIN_MCSPI1_CH1_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G0_PSILD_THREAD_OFFSET + 5U)
#define CSL_PDMA_CH_MAIN_MCSPI1_CH2_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G0_PSILD_THREAD_OFFSET + 6U)
#define CSL_PDMA_CH_MAIN_MCSPI1_CH3_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G0_PSILD_THREAD_OFFSET + 7U)
#define CSL_PDMA_CH_MAIN_MCSPI2_CH0_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G0_PSILD_THREAD_OFFSET + 8U)
#define CSL_PDMA_CH_MAIN_MCSPI2_CH1_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G0_PSILD_THREAD_OFFSET + 9U)
#define CSL_PDMA_CH_MAIN_MCSPI2_CH2_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G0_PSILD_THREAD_OFFSET + 10U)
#define CSL_PDMA_CH_MAIN_MCSPI2_CH3_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G0_PSILD_THREAD_OFFSET + 11U)
#define CSL_PDMA_CH_MAIN_MCSPI3_CH0_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G0_PSILD_THREAD_OFFSET + 12U)
#define CSL_PDMA_CH_MAIN_MCSPI3_CH1_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G0_PSILD_THREAD_OFFSET + 13U)
#define CSL_PDMA_CH_MAIN_MCSPI3_CH2_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G0_PSILD_THREAD_OFFSET + 14U)
#define CSL_PDMA_CH_MAIN_MCSPI3_CH3_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G0_PSILD_THREAD_OFFSET + 15U)
#define CSL_PDMA_CH_MAIN_MCSPI4_CH0_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G1_PSILD_THREAD_OFFSET + 0U)
#define CSL_PDMA_CH_MAIN_MCSPI4_CH1_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G1_PSILD_THREAD_OFFSET + 1U)
#define CSL_PDMA_CH_MAIN_MCSPI4_CH2_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G1_PSILD_THREAD_OFFSET + 2U)
#define CSL_PDMA_CH_MAIN_MCSPI4_CH3_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G1_PSILD_THREAD_OFFSET + 3U)
#define CSL_PDMA_CH_MAIN_MCSPI5_CH0_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G1_PSILD_THREAD_OFFSET + 4U)
#define CSL_PDMA_CH_MAIN_MCSPI5_CH1_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G1_PSILD_THREAD_OFFSET + 5U)
#define CSL_PDMA_CH_MAIN_MCSPI5_CH2_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G1_PSILD_THREAD_OFFSET + 6U)
#define CSL_PDMA_CH_MAIN_MCSPI5_CH3_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G1_PSILD_THREAD_OFFSET + 7U)
#define CSL_PDMA_CH_MAIN_MCSPI6_CH0_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G1_PSILD_THREAD_OFFSET + 8U)
#define CSL_PDMA_CH_MAIN_MCSPI6_CH1_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G1_PSILD_THREAD_OFFSET + 9U)
#define CSL_PDMA_CH_MAIN_MCSPI6_CH2_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G1_PSILD_THREAD_OFFSET + 10U)
#define CSL_PDMA_CH_MAIN_MCSPI6_CH3_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G1_PSILD_THREAD_OFFSET + 11U)
#define CSL_PDMA_CH_MAIN_MCSPI7_CH0_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G1_PSILD_THREAD_OFFSET + 12U)
#define CSL_PDMA_CH_MAIN_MCSPI7_CH1_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G1_PSILD_THREAD_OFFSET + 13U)
#define CSL_PDMA_CH_MAIN_MCSPI7_CH2_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G1_PSILD_THREAD_OFFSET + 14U)
#define CSL_PDMA_CH_MAIN_MCSPI7_CH3_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G1_PSILD_THREAD_OFFSET + 15U)
/*
 * PDMA MAIN MCAN TX Channels
 */
#define CSL_PDMA_CH_MAIN_MCAN0_CH0_TX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 10U)
#define CSL_PDMA_CH_MAIN_MCAN0_CH1_TX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 11U)
#define CSL_PDMA_CH_MAIN_MCAN0_CH2_TX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 12U)
#define CSL_PDMA_CH_MAIN_MCAN1_CH0_TX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 13U)
#define CSL_PDMA_CH_MAIN_MCAN1_CH1_TX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 14U)
#define CSL_PDMA_CH_MAIN_MCAN1_CH2_TX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 15U)
#define CSL_PDMA_CH_MAIN_MCAN2_CH0_TX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 16U)
#define CSL_PDMA_CH_MAIN_MCAN2_CH1_TX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 17U)
#define CSL_PDMA_CH_MAIN_MCAN2_CH2_TX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 18U)
#define CSL_PDMA_CH_MAIN_MCAN3_CH0_TX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 19U)
#define CSL_PDMA_CH_MAIN_MCAN3_CH1_TX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 20U)
#define CSL_PDMA_CH_MAIN_MCAN3_CH2_TX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 21U)
#define CSL_PDMA_CH_MAIN_MCAN4_CH0_TX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 22U)
#define CSL_PDMA_CH_MAIN_MCAN4_CH1_TX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 23U)
#define CSL_PDMA_CH_MAIN_MCAN4_CH2_TX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 24U)
#define CSL_PDMA_CH_MAIN_MCAN5_CH0_TX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 25U)
#define CSL_PDMA_CH_MAIN_MCAN5_CH1_TX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 26U)
#define CSL_PDMA_CH_MAIN_MCAN5_CH2_TX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 27U)
#define CSL_PDMA_CH_MAIN_MCAN6_CH0_TX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 28U)
#define CSL_PDMA_CH_MAIN_MCAN6_CH1_TX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 29U)
#define CSL_PDMA_CH_MAIN_MCAN6_CH2_TX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 30U)
#define CSL_PDMA_CH_MAIN_MCAN7_CH0_TX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 31U)
#define CSL_PDMA_CH_MAIN_MCAN7_CH1_TX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 32U)
#define CSL_PDMA_CH_MAIN_MCAN7_CH2_TX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 33U)
#define CSL_PDMA_CH_MAIN_MCAN8_CH0_TX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 34U)
#define CSL_PDMA_CH_MAIN_MCAN8_CH1_TX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 35U)
#define CSL_PDMA_CH_MAIN_MCAN8_CH2_TX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 36U)
#define CSL_PDMA_CH_MAIN_MCAN9_CH0_TX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 37U)
#define CSL_PDMA_CH_MAIN_MCAN9_CH1_TX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 38U)
#define CSL_PDMA_CH_MAIN_MCAN9_CH2_TX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 39U)
#define CSL_PDMA_CH_MAIN_MCAN10_CH0_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 40U)
#define CSL_PDMA_CH_MAIN_MCAN10_CH1_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 41U)
#define CSL_PDMA_CH_MAIN_MCAN10_CH2_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 42U)
#define CSL_PDMA_CH_MAIN_MCAN11_CH0_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 43U)
#define CSL_PDMA_CH_MAIN_MCAN11_CH1_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 44U)
#define CSL_PDMA_CH_MAIN_MCAN11_CH2_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 45U)
#define CSL_PDMA_CH_MAIN_MCAN12_CH0_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 46U)
#define CSL_PDMA_CH_MAIN_MCAN12_CH1_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 47U)
#define CSL_PDMA_CH_MAIN_MCAN12_CH2_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 48U)
#define CSL_PDMA_CH_MAIN_MCAN13_CH0_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 49U)
#define CSL_PDMA_CH_MAIN_MCAN13_CH1_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 50U)
#define CSL_PDMA_CH_MAIN_MCAN13_CH2_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 51U)
#define CSL_PDMA_CH_MAIN_MCAN14_CH0_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 52U)
#define CSL_PDMA_CH_MAIN_MCAN14_CH1_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 53U)
#define CSL_PDMA_CH_MAIN_MCAN14_CH2_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 54U)
#define CSL_PDMA_CH_MAIN_MCAN15_CH0_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 55U)
#define CSL_PDMA_CH_MAIN_MCAN15_CH1_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 56U)
#define CSL_PDMA_CH_MAIN_MCAN15_CH2_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 57U)
#define CSL_PDMA_CH_MAIN_MCAN16_CH0_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 58U)
#define CSL_PDMA_CH_MAIN_MCAN16_CH1_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 59U)
#define CSL_PDMA_CH_MAIN_MCAN16_CH2_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 60U)
#define CSL_PDMA_CH_MAIN_MCAN17_CH0_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 61U)
#define CSL_PDMA_CH_MAIN_MCAN17_CH1_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 62U)
#define CSL_PDMA_CH_MAIN_MCAN17_CH2_TX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILD_THREAD_OFFSET + 63U)

/*
 * PDMA MAIN DEBUG TX Channels
 */
/* None */

/* @} */

/**
 *  \anchor CSL_PdmaChMcuTx
 *  \name MCU TX PDMA Channels
 *
 *  List of all MCU PDMA TX channels
 *
 *  @{
 */

/*
 * PDMA MCU ADC TX Channels
 */
/* None */
/*
 * PDMA MCU MCSPI TX Channels
 */
#define CSL_PDMA_CH_MCU_MCSPI0_CH0_TX       (CSL_PSILCFG_NAVSS_MCU_PDMA_MCU0_PSILD_THREAD_OFFSET + 0U)
#define CSL_PDMA_CH_MCU_MCSPI0_CH1_TX       (CSL_PSILCFG_NAVSS_MCU_PDMA_MCU0_PSILD_THREAD_OFFSET + 1U)
#define CSL_PDMA_CH_MCU_MCSPI0_CH2_TX       (CSL_PSILCFG_NAVSS_MCU_PDMA_MCU0_PSILD_THREAD_OFFSET + 2U)
#define CSL_PDMA_CH_MCU_MCSPI0_CH3_TX       (CSL_PSILCFG_NAVSS_MCU_PDMA_MCU0_PSILD_THREAD_OFFSET + 3U)
#define CSL_PDMA_CH_MCU_MCSPI1_CH0_TX       (CSL_PSILCFG_NAVSS_MCU_PDMA_MCU1_PSILD_THREAD_OFFSET + 0U)
#define CSL_PDMA_CH_MCU_MCSPI1_CH1_TX       (CSL_PSILCFG_NAVSS_MCU_PDMA_MCU1_PSILD_THREAD_OFFSET + 1U)
#define CSL_PDMA_CH_MCU_MCSPI1_CH2_TX       (CSL_PSILCFG_NAVSS_MCU_PDMA_MCU1_PSILD_THREAD_OFFSET + 2U)
#define CSL_PDMA_CH_MCU_MCSPI1_CH3_TX       (CSL_PSILCFG_NAVSS_MCU_PDMA_MCU1_PSILD_THREAD_OFFSET + 3U)
#define CSL_PDMA_CH_MCU_MCSPI2_CH0_TX       (CSL_PSILCFG_NAVSS_MCU_PDMA_MCU1_PSILD_THREAD_OFFSET + 4U)
#define CSL_PDMA_CH_MCU_MCSPI2_CH1_TX       (CSL_PSILCFG_NAVSS_MCU_PDMA_MCU1_PSILD_THREAD_OFFSET + 5U)
#define CSL_PDMA_CH_MCU_MCSPI2_CH2_TX       (CSL_PSILCFG_NAVSS_MCU_PDMA_MCU1_PSILD_THREAD_OFFSET + 6U)
#define CSL_PDMA_CH_MCU_MCSPI2_CH3_TX       (CSL_PSILCFG_NAVSS_MCU_PDMA_MCU1_PSILD_THREAD_OFFSET + 7U)
/*
 * PDMA MCU MCAN TX Channels
 */
#define CSL_PDMA_CH_MCU_MCAN0_CH0_TX        (CSL_PSILCFG_NAVSS_MCU_PDMA_MCU0_PSILD_THREAD_OFFSET + 4U)
#define CSL_PDMA_CH_MCU_MCAN0_CH1_TX        (CSL_PSILCFG_NAVSS_MCU_PDMA_MCU0_PSILD_THREAD_OFFSET + 5U)
#define CSL_PDMA_CH_MCU_MCAN0_CH2_TX        (CSL_PSILCFG_NAVSS_MCU_PDMA_MCU0_PSILD_THREAD_OFFSET + 6U)
#define CSL_PDMA_CH_MCU_MCAN1_CH0_TX        (CSL_PSILCFG_NAVSS_MCU_PDMA_MCU2_PSILD_THREAD_OFFSET + 1U)
#define CSL_PDMA_CH_MCU_MCAN1_CH1_TX        (CSL_PSILCFG_NAVSS_MCU_PDMA_MCU2_PSILD_THREAD_OFFSET + 2U)
#define CSL_PDMA_CH_MCU_MCAN1_CH2_TX        (CSL_PSILCFG_NAVSS_MCU_PDMA_MCU2_PSILD_THREAD_OFFSET + 3U)
/*
 * PDMA MCU UART TX Channels
 */
#define CSL_PDMA_CH_MCU_UART0_CH0_TX        (CSL_PSILCFG_NAVSS_MCU_PDMA_MCU2_PSILD_THREAD_OFFSET + 0U)

/* @} */

/**
 *  \anchor CSL_PdmaChMainRx
 *  \name Main RX PDMA Channels
 *
 *  List of all Main PDMA RX channels
 *
 *  @{
 */

/*
 * PDMA MAIN MCASP RX Channels
 */
#define CSL_PDMA_CH_MAIN_MCASP0_CH0_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_MCASP_PSILS_THREAD_OFFSET + 0U)
#define CSL_PDMA_CH_MAIN_MCASP1_CH0_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_MCASP_PSILS_THREAD_OFFSET + 1U)
#define CSL_PDMA_CH_MAIN_MCASP2_CH0_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_MCASP_PSILS_THREAD_OFFSET + 2U)
#define CSL_PDMA_CH_MAIN_MCASP3_CH0_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_MCASP_PSILS_THREAD_OFFSET + 3U)
#define CSL_PDMA_CH_MAIN_MCASP4_CH0_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_MCASP_PSILS_THREAD_OFFSET + 4U)
/*
 * PDMA MAIN UART RX Channels
 */
#define CSL_PDMA_CH_MAIN_UART0_CH0_RX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 0U)
#define CSL_PDMA_CH_MAIN_UART1_CH0_RX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 1U)
#define CSL_PDMA_CH_MAIN_UART2_CH0_RX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 2U)
#define CSL_PDMA_CH_MAIN_UART3_CH0_RX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 3U)
#define CSL_PDMA_CH_MAIN_UART4_CH0_RX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 4U)
#define CSL_PDMA_CH_MAIN_UART5_CH0_RX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 5U)
#define CSL_PDMA_CH_MAIN_UART6_CH0_RX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 6U)
#define CSL_PDMA_CH_MAIN_UART7_CH0_RX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 7U)
#define CSL_PDMA_CH_MAIN_UART8_CH0_RX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 8U)
#define CSL_PDMA_CH_MAIN_UART9_CH0_RX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 9U)
/*
 * PDMA MAIN MCSPI RX Channels
 */
#define CSL_PDMA_CH_MAIN_MCSPI0_CH0_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G0_PSILS_THREAD_OFFSET + 0U)
#define CSL_PDMA_CH_MAIN_MCSPI0_CH1_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G0_PSILS_THREAD_OFFSET + 1U)
#define CSL_PDMA_CH_MAIN_MCSPI0_CH2_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G0_PSILS_THREAD_OFFSET + 2U)
#define CSL_PDMA_CH_MAIN_MCSPI0_CH3_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G0_PSILS_THREAD_OFFSET + 3U)
#define CSL_PDMA_CH_MAIN_MCSPI1_CH0_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G0_PSILS_THREAD_OFFSET + 4U)
#define CSL_PDMA_CH_MAIN_MCSPI1_CH1_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G0_PSILS_THREAD_OFFSET + 5U)
#define CSL_PDMA_CH_MAIN_MCSPI1_CH2_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G0_PSILS_THREAD_OFFSET + 6U)
#define CSL_PDMA_CH_MAIN_MCSPI1_CH3_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G0_PSILS_THREAD_OFFSET + 7U)
#define CSL_PDMA_CH_MAIN_MCSPI2_CH0_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G0_PSILS_THREAD_OFFSET + 8U)
#define CSL_PDMA_CH_MAIN_MCSPI2_CH1_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G0_PSILS_THREAD_OFFSET + 9U)
#define CSL_PDMA_CH_MAIN_MCSPI2_CH2_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G0_PSILS_THREAD_OFFSET + 10U)
#define CSL_PDMA_CH_MAIN_MCSPI2_CH3_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G0_PSILS_THREAD_OFFSET + 11U)
#define CSL_PDMA_CH_MAIN_MCSPI3_CH0_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G0_PSILS_THREAD_OFFSET + 12U)
#define CSL_PDMA_CH_MAIN_MCSPI3_CH1_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G0_PSILS_THREAD_OFFSET + 13U)
#define CSL_PDMA_CH_MAIN_MCSPI3_CH2_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G0_PSILS_THREAD_OFFSET + 14U)
#define CSL_PDMA_CH_MAIN_MCSPI3_CH3_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G0_PSILS_THREAD_OFFSET + 15U)
#define CSL_PDMA_CH_MAIN_MCSPI4_CH0_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G1_PSILS_THREAD_OFFSET + 0U)
#define CSL_PDMA_CH_MAIN_MCSPI4_CH1_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G1_PSILS_THREAD_OFFSET + 1U)
#define CSL_PDMA_CH_MAIN_MCSPI4_CH2_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G1_PSILS_THREAD_OFFSET + 2U)
#define CSL_PDMA_CH_MAIN_MCSPI4_CH3_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G1_PSILS_THREAD_OFFSET + 3U)
#define CSL_PDMA_CH_MAIN_MCSPI5_CH0_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G1_PSILS_THREAD_OFFSET + 4U)
#define CSL_PDMA_CH_MAIN_MCSPI5_CH1_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G1_PSILS_THREAD_OFFSET + 5U)
#define CSL_PDMA_CH_MAIN_MCSPI5_CH2_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G1_PSILS_THREAD_OFFSET + 6U)
#define CSL_PDMA_CH_MAIN_MCSPI5_CH3_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G1_PSILS_THREAD_OFFSET + 7U)
#define CSL_PDMA_CH_MAIN_MCSPI6_CH0_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G1_PSILS_THREAD_OFFSET + 8U)
#define CSL_PDMA_CH_MAIN_MCSPI6_CH1_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G1_PSILS_THREAD_OFFSET + 9U)
#define CSL_PDMA_CH_MAIN_MCSPI6_CH2_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G1_PSILS_THREAD_OFFSET + 10U)
#define CSL_PDMA_CH_MAIN_MCSPI6_CH3_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G1_PSILS_THREAD_OFFSET + 11U)
#define CSL_PDMA_CH_MAIN_MCSPI7_CH0_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G1_PSILS_THREAD_OFFSET + 12U)
#define CSL_PDMA_CH_MAIN_MCSPI7_CH1_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G1_PSILS_THREAD_OFFSET + 13U)
#define CSL_PDMA_CH_MAIN_MCSPI7_CH2_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G1_PSILS_THREAD_OFFSET + 14U)
#define CSL_PDMA_CH_MAIN_MCSPI7_CH3_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_SPI_G1_PSILS_THREAD_OFFSET + 15U)
/*
 * PDMA MAIN MCAN RX Channels
 */
#define CSL_PDMA_CH_MAIN_MCAN0_CH0_RX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 10U)
#define CSL_PDMA_CH_MAIN_MCAN0_CH1_RX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 11U)
#define CSL_PDMA_CH_MAIN_MCAN0_CH2_RX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 12U)
#define CSL_PDMA_CH_MAIN_MCAN1_CH0_RX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 13U)
#define CSL_PDMA_CH_MAIN_MCAN1_CH1_RX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 14U)
#define CSL_PDMA_CH_MAIN_MCAN1_CH2_RX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 15U)
#define CSL_PDMA_CH_MAIN_MCAN2_CH0_RX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 16U)
#define CSL_PDMA_CH_MAIN_MCAN2_CH1_RX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 17U)
#define CSL_PDMA_CH_MAIN_MCAN2_CH2_RX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 18U)
#define CSL_PDMA_CH_MAIN_MCAN3_CH0_RX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 19U)
#define CSL_PDMA_CH_MAIN_MCAN3_CH1_RX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 20U)
#define CSL_PDMA_CH_MAIN_MCAN3_CH2_RX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 21U)
#define CSL_PDMA_CH_MAIN_MCAN4_CH0_RX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 22U)
#define CSL_PDMA_CH_MAIN_MCAN4_CH1_RX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 23U)
#define CSL_PDMA_CH_MAIN_MCAN4_CH2_RX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 24U)
#define CSL_PDMA_CH_MAIN_MCAN5_CH0_RX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 25U)
#define CSL_PDMA_CH_MAIN_MCAN5_CH1_RX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 26U)
#define CSL_PDMA_CH_MAIN_MCAN5_CH2_RX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 27U)
#define CSL_PDMA_CH_MAIN_MCAN6_CH0_RX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 28U)
#define CSL_PDMA_CH_MAIN_MCAN6_CH1_RX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 29U)
#define CSL_PDMA_CH_MAIN_MCAN6_CH2_RX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 30U)
#define CSL_PDMA_CH_MAIN_MCAN7_CH0_RX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 31U)
#define CSL_PDMA_CH_MAIN_MCAN7_CH1_RX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 32U)
#define CSL_PDMA_CH_MAIN_MCAN7_CH2_RX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 33U)
#define CSL_PDMA_CH_MAIN_MCAN8_CH0_RX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 34U)
#define CSL_PDMA_CH_MAIN_MCAN8_CH1_RX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 35U)
#define CSL_PDMA_CH_MAIN_MCAN8_CH2_RX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 36U)
#define CSL_PDMA_CH_MAIN_MCAN9_CH0_RX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 37U)
#define CSL_PDMA_CH_MAIN_MCAN9_CH1_RX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 38U)
#define CSL_PDMA_CH_MAIN_MCAN9_CH2_RX       (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 39U)
#define CSL_PDMA_CH_MAIN_MCAN10_CH0_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 40U)
#define CSL_PDMA_CH_MAIN_MCAN10_CH1_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 41U)
#define CSL_PDMA_CH_MAIN_MCAN10_CH2_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 42U)
#define CSL_PDMA_CH_MAIN_MCAN11_CH0_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 43U)
#define CSL_PDMA_CH_MAIN_MCAN11_CH1_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 44U)
#define CSL_PDMA_CH_MAIN_MCAN11_CH2_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 45U)
#define CSL_PDMA_CH_MAIN_MCAN12_CH0_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 46U)
#define CSL_PDMA_CH_MAIN_MCAN12_CH1_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 47U)
#define CSL_PDMA_CH_MAIN_MCAN12_CH2_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 48U)
#define CSL_PDMA_CH_MAIN_MCAN13_CH0_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 49U)
#define CSL_PDMA_CH_MAIN_MCAN13_CH1_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 50U)
#define CSL_PDMA_CH_MAIN_MCAN13_CH2_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 51U)
#define CSL_PDMA_CH_MAIN_MCAN14_CH0_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 52U)
#define CSL_PDMA_CH_MAIN_MCAN14_CH1_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 53U)
#define CSL_PDMA_CH_MAIN_MCAN14_CH2_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 54U)
#define CSL_PDMA_CH_MAIN_MCAN15_CH0_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 55U)
#define CSL_PDMA_CH_MAIN_MCAN15_CH1_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 56U)
#define CSL_PDMA_CH_MAIN_MCAN15_CH2_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 57U)
#define CSL_PDMA_CH_MAIN_MCAN16_CH0_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 58U)
#define CSL_PDMA_CH_MAIN_MCAN16_CH1_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 59U)
#define CSL_PDMA_CH_MAIN_MCAN16_CH2_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 60U)
#define CSL_PDMA_CH_MAIN_MCAN17_CH0_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 61U)
#define CSL_PDMA_CH_MAIN_MCAN17_CH1_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 62U)
#define CSL_PDMA_CH_MAIN_MCAN17_CH2_RX      (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_USART_PSILS_THREAD_OFFSET + 63U)
/*
 * PDMA MAIN DEBUG RX Channels
 */
#define CSL_PDMA_CH_MAIN_DEBUG_CELL0_RX     (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_DEBUG_PSILS_THREAD_OFFSET + 0U)
#define CSL_PDMA_CH_MAIN_DEBUG_CELL1_RX     (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_DEBUG_PSILS_THREAD_OFFSET + 1U)
#define CSL_PDMA_CH_MAIN_DEBUG_CELL2_RX     (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_DEBUG_PSILS_THREAD_OFFSET + 2U)
#define CSL_PDMA_CH_MAIN_DEBUG_CELL3_RX     (CSL_PSILCFG_NAVSS_MAIN_PDMA_MAIN_DEBUG_PSILS_THREAD_OFFSET + 3U)

/* @} */

/**
 *  \anchor CSL_PdmaChMcuRx
 *  \name MCU RX PDMA Channels
 *
 *  List of all MCU PDMA RX channels
 *
 *  @{
 */
/*
 * PDMA MCU ADC RX Channels
 */
#define CSL_PDMA_CH_MCU_ADC0_CH0_RX         (CSL_PSILCFG_NAVSS_MCU_PDMA_ADC_PSILS_THREAD_OFFSET + 0U)
#define CSL_PDMA_CH_MCU_ADC0_CH1_RX         (CSL_PSILCFG_NAVSS_MCU_PDMA_ADC_PSILS_THREAD_OFFSET + 1U)
#define CSL_PDMA_CH_MCU_ADC1_CH0_RX         (CSL_PSILCFG_NAVSS_MCU_PDMA_ADC_PSILS_THREAD_OFFSET + 2U)
#define CSL_PDMA_CH_MCU_ADC1_CH1_RX         (CSL_PSILCFG_NAVSS_MCU_PDMA_ADC_PSILS_THREAD_OFFSET + 3U)
/*
 * PDMA MCU MCSPI RX Channels
 */
#define CSL_PDMA_CH_MCU_MCSPI0_CH0_RX       (CSL_PSILCFG_NAVSS_MCU_PDMA_MCU0_PSILS_THREAD_OFFSET + 0U)
#define CSL_PDMA_CH_MCU_MCSPI0_CH1_RX       (CSL_PSILCFG_NAVSS_MCU_PDMA_MCU0_PSILS_THREAD_OFFSET + 1U)
#define CSL_PDMA_CH_MCU_MCSPI0_CH2_RX       (CSL_PSILCFG_NAVSS_MCU_PDMA_MCU0_PSILS_THREAD_OFFSET + 2U)
#define CSL_PDMA_CH_MCU_MCSPI0_CH3_RX       (CSL_PSILCFG_NAVSS_MCU_PDMA_MCU0_PSILS_THREAD_OFFSET + 3U)
#define CSL_PDMA_CH_MCU_MCSPI1_CH0_RX       (CSL_PSILCFG_NAVSS_MCU_PDMA_MCU1_PSILS_THREAD_OFFSET + 0U)
#define CSL_PDMA_CH_MCU_MCSPI1_CH1_RX       (CSL_PSILCFG_NAVSS_MCU_PDMA_MCU1_PSILS_THREAD_OFFSET + 1U)
#define CSL_PDMA_CH_MCU_MCSPI1_CH2_RX       (CSL_PSILCFG_NAVSS_MCU_PDMA_MCU1_PSILS_THREAD_OFFSET + 2U)
#define CSL_PDMA_CH_MCU_MCSPI1_CH3_RX       (CSL_PSILCFG_NAVSS_MCU_PDMA_MCU1_PSILS_THREAD_OFFSET + 3U)
#define CSL_PDMA_CH_MCU_MCSPI2_CH0_RX       (CSL_PSILCFG_NAVSS_MCU_PDMA_MCU1_PSILS_THREAD_OFFSET + 4U)
#define CSL_PDMA_CH_MCU_MCSPI2_CH1_RX       (CSL_PSILCFG_NAVSS_MCU_PDMA_MCU1_PSILS_THREAD_OFFSET + 5U)
#define CSL_PDMA_CH_MCU_MCSPI2_CH2_RX       (CSL_PSILCFG_NAVSS_MCU_PDMA_MCU1_PSILS_THREAD_OFFSET + 6U)
#define CSL_PDMA_CH_MCU_MCSPI2_CH3_RX       (CSL_PSILCFG_NAVSS_MCU_PDMA_MCU1_PSILS_THREAD_OFFSET + 7U)
/*
 * PDMA MCU MCAN RX Channels
 */
#define CSL_PDMA_CH_MCU_MCAN0_CH0_RX        (CSL_PSILCFG_NAVSS_MCU_PDMA_MCU0_PSILS_THREAD_OFFSET + 4U)
#define CSL_PDMA_CH_MCU_MCAN0_CH1_RX        (CSL_PSILCFG_NAVSS_MCU_PDMA_MCU0_PSILS_THREAD_OFFSET + 5U)
#define CSL_PDMA_CH_MCU_MCAN0_CH2_RX        (CSL_PSILCFG_NAVSS_MCU_PDMA_MCU0_PSILS_THREAD_OFFSET + 6U)
#define CSL_PDMA_CH_MCU_MCAN1_CH0_RX        (CSL_PSILCFG_NAVSS_MCU_PDMA_MCU2_PSILS_THREAD_OFFSET + 1U)
#define CSL_PDMA_CH_MCU_MCAN1_CH1_RX        (CSL_PSILCFG_NAVSS_MCU_PDMA_MCU2_PSILS_THREAD_OFFSET + 2U)
#define CSL_PDMA_CH_MCU_MCAN1_CH2_RX        (CSL_PSILCFG_NAVSS_MCU_PDMA_MCU2_PSILS_THREAD_OFFSET + 3U)
/*
 * PDMA MCU UART RX Channels
 */
#define CSL_PDMA_CH_MCU_UART0_CH0_RX        (CSL_PSILCFG_NAVSS_MCU_PDMA_MCU2_PSILS_THREAD_OFFSET + 0U)

/* @} */

/* @} */

/**
 *  \anchor CSL_PsilCsiCh
 *  \name PSIL channels for CSI TX and CSI RX
 *
 *  List of all CSI channels
 *
 *  @{
 */
#define CSL_PSIL_CSI_TX_CH(inst,chan)       (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + (((uint32_t)(inst))*32U) + ((uint32_t)(chan)))

#define CSL_PSIL_CSI_TX0_CH0                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x00U + 0U)
#define CSL_PSIL_CSI_TX0_CH1                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x00U + 1U)
#define CSL_PSIL_CSI_TX0_CH2                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x00U + 2U)
#define CSL_PSIL_CSI_TX0_CH3                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x00U + 3U)
#define CSL_PSIL_CSI_TX0_CH4                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x00U + 4U)
#define CSL_PSIL_CSI_TX0_CH5                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x00U + 5U)
#define CSL_PSIL_CSI_TX0_CH6                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x00U + 6U)
#define CSL_PSIL_CSI_TX0_CH7                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x00U + 7U)
#define CSL_PSIL_CSI_TX0_CH8                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x00U + 8U)
#define CSL_PSIL_CSI_TX0_CH9                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x00U + 9U)
#define CSL_PSIL_CSI_TX0_CH10               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x00U + 10U)
#define CSL_PSIL_CSI_TX0_CH11               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x00U + 11U)
#define CSL_PSIL_CSI_TX0_CH12               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x00U + 12U)
#define CSL_PSIL_CSI_TX0_CH13               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x00U + 13U)
#define CSL_PSIL_CSI_TX0_CH14               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x00U + 14U)
#define CSL_PSIL_CSI_TX0_CH15               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x00U + 15U)
#define CSL_PSIL_CSI_TX0_CH16               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x00U + 16U)
#define CSL_PSIL_CSI_TX0_CH17               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x00U + 17U)
#define CSL_PSIL_CSI_TX0_CH18               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x00U + 18U)
#define CSL_PSIL_CSI_TX0_CH19               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x00U + 19U)
#define CSL_PSIL_CSI_TX0_CH20               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x00U + 20U)
#define CSL_PSIL_CSI_TX0_CH21               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x00U + 21U)
#define CSL_PSIL_CSI_TX0_CH22               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x00U + 22U)
#define CSL_PSIL_CSI_TX0_CH23               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x00U + 23U)
#define CSL_PSIL_CSI_TX0_CH24               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x00U + 24U)
#define CSL_PSIL_CSI_TX0_CH25               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x00U + 25U)
#define CSL_PSIL_CSI_TX0_CH26               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x00U + 26U)
#define CSL_PSIL_CSI_TX0_CH27               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x00U + 27U)
#define CSL_PSIL_CSI_TX0_CH28               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x00U + 28U)
#define CSL_PSIL_CSI_TX0_CH29               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x00U + 29U)
#define CSL_PSIL_CSI_TX0_CH30               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x00U + 30U)
#define CSL_PSIL_CSI_TX0_CH31               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x00U + 31U)

#define CSL_PSIL_CSI_TX1_CH0                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x20U + 0U)
#define CSL_PSIL_CSI_TX1_CH1                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x20U + 1U)
#define CSL_PSIL_CSI_TX1_CH2                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x20U + 2U)
#define CSL_PSIL_CSI_TX1_CH3                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x20U + 3U)
#define CSL_PSIL_CSI_TX1_CH4                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x20U + 4U)
#define CSL_PSIL_CSI_TX1_CH5                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x20U + 5U)
#define CSL_PSIL_CSI_TX1_CH6                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x20U + 6U)
#define CSL_PSIL_CSI_TX1_CH7                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x20U + 7U)
#define CSL_PSIL_CSI_TX1_CH8                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x20U + 8U)
#define CSL_PSIL_CSI_TX1_CH9                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x20U + 9U)
#define CSL_PSIL_CSI_TX1_CH10               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x20U + 10U)
#define CSL_PSIL_CSI_TX1_CH11               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x20U + 11U)
#define CSL_PSIL_CSI_TX1_CH12               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x20U + 12U)
#define CSL_PSIL_CSI_TX1_CH13               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x20U + 13U)
#define CSL_PSIL_CSI_TX1_CH14               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x20U + 14U)
#define CSL_PSIL_CSI_TX1_CH15               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x20U + 15U)
#define CSL_PSIL_CSI_TX1_CH16               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x20U + 16U)
#define CSL_PSIL_CSI_TX1_CH17               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x20U + 17U)
#define CSL_PSIL_CSI_TX1_CH18               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x20U + 18U)
#define CSL_PSIL_CSI_TX1_CH19               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x20U + 19U)
#define CSL_PSIL_CSI_TX1_CH20               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x20U + 20U)
#define CSL_PSIL_CSI_TX1_CH21               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x20U + 21U)
#define CSL_PSIL_CSI_TX1_CH22               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x20U + 22U)
#define CSL_PSIL_CSI_TX1_CH23               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x20U + 23U)
#define CSL_PSIL_CSI_TX1_CH24               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x20U + 24U)
#define CSL_PSIL_CSI_TX1_CH25               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x20U + 25U)
#define CSL_PSIL_CSI_TX1_CH26               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x20U + 26U)
#define CSL_PSIL_CSI_TX1_CH27               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x20U + 27U)
#define CSL_PSIL_CSI_TX1_CH28               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x20U + 28U)
#define CSL_PSIL_CSI_TX1_CH29               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x20U + 29U)
#define CSL_PSIL_CSI_TX1_CH30               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x20U + 30U)
#define CSL_PSIL_CSI_TX1_CH31               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILD_THREAD_OFFSET + 0x20U + 31U)

#define CSL_PSIL_CSI_RX_CH(inst,chan)       (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x40U + (((uint32_t)(inst))*32U) + ((uint32_t)(chan)))

#define CSL_PSIL_CSI_RX0_CH0                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x40U  + 0U)
#define CSL_PSIL_CSI_RX0_CH1                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x40U  + 1U)
#define CSL_PSIL_CSI_RX0_CH2                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x40U  + 2U)
#define CSL_PSIL_CSI_RX0_CH3                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x40U  + 3U)
#define CSL_PSIL_CSI_RX0_CH4                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x40U  + 4U)
#define CSL_PSIL_CSI_RX0_CH5                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x40U  + 5U)
#define CSL_PSIL_CSI_RX0_CH6                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x40U  + 6U)
#define CSL_PSIL_CSI_RX0_CH7                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x40U  + 7U)
#define CSL_PSIL_CSI_RX0_CH8                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x40U  + 8U)
#define CSL_PSIL_CSI_RX0_CH9                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x40U  + 9U)
#define CSL_PSIL_CSI_RX0_CH10               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x40U  + 10U)
#define CSL_PSIL_CSI_RX0_CH11               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x40U  + 11U)
#define CSL_PSIL_CSI_RX0_CH12               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x40U  + 12U)
#define CSL_PSIL_CSI_RX0_CH13               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x40U  + 13U)
#define CSL_PSIL_CSI_RX0_CH14               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x40U  + 14U)
#define CSL_PSIL_CSI_RX0_CH15               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x40U  + 15U)
#define CSL_PSIL_CSI_RX0_CH16               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x40U  + 16U)
#define CSL_PSIL_CSI_RX0_CH17               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x40U  + 17U)
#define CSL_PSIL_CSI_RX0_CH18               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x40U  + 18U)
#define CSL_PSIL_CSI_RX0_CH19               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x40U  + 19U)
#define CSL_PSIL_CSI_RX0_CH20               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x40U  + 20U)
#define CSL_PSIL_CSI_RX0_CH21               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x40U  + 21U)
#define CSL_PSIL_CSI_RX0_CH22               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x40U  + 22U)
#define CSL_PSIL_CSI_RX0_CH23               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x40U  + 23U)
#define CSL_PSIL_CSI_RX0_CH24               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x40U  + 24U)
#define CSL_PSIL_CSI_RX0_CH25               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x40U  + 25U)
#define CSL_PSIL_CSI_RX0_CH26               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x40U  + 26U)
#define CSL_PSIL_CSI_RX0_CH27               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x40U  + 27U)
#define CSL_PSIL_CSI_RX0_CH28               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x40U  + 28U)
#define CSL_PSIL_CSI_RX0_CH29               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x40U  + 29U)
#define CSL_PSIL_CSI_RX0_CH30               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x40U  + 30U)
#define CSL_PSIL_CSI_RX0_CH31               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x40U  + 31U)

#define CSL_PSIL_CSI_RX1_CH0                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x60U  + 0U)
#define CSL_PSIL_CSI_RX1_CH1                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x60U  + 1U)
#define CSL_PSIL_CSI_RX1_CH2                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x60U  + 2U)
#define CSL_PSIL_CSI_RX1_CH3                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x60U  + 3U)
#define CSL_PSIL_CSI_RX1_CH4                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x60U  + 4U)
#define CSL_PSIL_CSI_RX1_CH5                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x60U  + 5U)
#define CSL_PSIL_CSI_RX1_CH6                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x60U  + 6U)
#define CSL_PSIL_CSI_RX1_CH7                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x60U  + 7U)
#define CSL_PSIL_CSI_RX1_CH8                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x60U  + 8U)
#define CSL_PSIL_CSI_RX1_CH9                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x60U  + 9U)
#define CSL_PSIL_CSI_RX1_CH10               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x60U  + 10U)
#define CSL_PSIL_CSI_RX1_CH11               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x60U  + 11U)
#define CSL_PSIL_CSI_RX1_CH12               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x60U  + 12U)
#define CSL_PSIL_CSI_RX1_CH13               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x60U  + 13U)
#define CSL_PSIL_CSI_RX1_CH14               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x60U  + 14U)
#define CSL_PSIL_CSI_RX1_CH15               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x60U  + 15U)
#define CSL_PSIL_CSI_RX1_CH16               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x60U  + 16U)
#define CSL_PSIL_CSI_RX1_CH17               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x60U  + 17U)
#define CSL_PSIL_CSI_RX1_CH18               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x60U  + 18U)
#define CSL_PSIL_CSI_RX1_CH19               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x60U  + 19U)
#define CSL_PSIL_CSI_RX1_CH20               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x60U  + 20U)
#define CSL_PSIL_CSI_RX1_CH21               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x60U  + 21U)
#define CSL_PSIL_CSI_RX1_CH22               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x60U  + 22U)
#define CSL_PSIL_CSI_RX1_CH23               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x60U  + 23U)
#define CSL_PSIL_CSI_RX1_CH24               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x60U  + 24U)
#define CSL_PSIL_CSI_RX1_CH25               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x60U  + 25U)
#define CSL_PSIL_CSI_RX1_CH26               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x60U  + 26U)
#define CSL_PSIL_CSI_RX1_CH27               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x60U  + 27U)
#define CSL_PSIL_CSI_RX1_CH28               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x60U  + 28U)
#define CSL_PSIL_CSI_RX1_CH29               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x60U  + 29U)
#define CSL_PSIL_CSI_RX1_CH30               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x60U  + 30U)
#define CSL_PSIL_CSI_RX1_CH31               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x60U  + 31U)

#define CSL_PSIL_CSI_RX2_CH0                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x80U  + 0U)
#define CSL_PSIL_CSI_RX2_CH1                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x80U  + 1U)
#define CSL_PSIL_CSI_RX2_CH2                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x80U  + 2U)
#define CSL_PSIL_CSI_RX2_CH3                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x80U  + 3U)
#define CSL_PSIL_CSI_RX2_CH4                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x80U  + 4U)
#define CSL_PSIL_CSI_RX2_CH5                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x80U  + 5U)
#define CSL_PSIL_CSI_RX2_CH6                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x80U  + 6U)
#define CSL_PSIL_CSI_RX2_CH7                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x80U  + 7U)
#define CSL_PSIL_CSI_RX2_CH8                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x80U  + 8U)
#define CSL_PSIL_CSI_RX2_CH9                (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x80U  + 9U)
#define CSL_PSIL_CSI_RX2_CH10               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x80U  + 10U)
#define CSL_PSIL_CSI_RX2_CH11               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x80U  + 11U)
#define CSL_PSIL_CSI_RX2_CH12               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x80U  + 12U)
#define CSL_PSIL_CSI_RX2_CH13               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x80U  + 13U)
#define CSL_PSIL_CSI_RX2_CH14               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x80U  + 14U)
#define CSL_PSIL_CSI_RX2_CH15               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x80U  + 15U)
#define CSL_PSIL_CSI_RX2_CH16               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x80U  + 16U)
#define CSL_PSIL_CSI_RX2_CH17               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x80U  + 17U)
#define CSL_PSIL_CSI_RX2_CH18               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x80U  + 18U)
#define CSL_PSIL_CSI_RX2_CH19               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x80U  + 19U)
#define CSL_PSIL_CSI_RX2_CH20               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x80U  + 20U)
#define CSL_PSIL_CSI_RX2_CH21               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x80U  + 21U)
#define CSL_PSIL_CSI_RX2_CH22               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x80U  + 22U)
#define CSL_PSIL_CSI_RX2_CH23               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x80U  + 23U)
#define CSL_PSIL_CSI_RX2_CH24               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x80U  + 24U)
#define CSL_PSIL_CSI_RX2_CH25               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x80U  + 25U)
#define CSL_PSIL_CSI_RX2_CH26               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x80U  + 26U)
#define CSL_PSIL_CSI_RX2_CH27               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x80U  + 27U)
#define CSL_PSIL_CSI_RX2_CH28               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x80U  + 28U)
#define CSL_PSIL_CSI_RX2_CH29               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x80U  + 29U)
#define CSL_PSIL_CSI_RX2_CH30               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x80U  + 30U)
#define CSL_PSIL_CSI_RX2_CH31               (CSL_PSILCFG_NAVSS_MAIN_CSI_PSILS_THREAD_OFFSET + 0x80U  + 31U)
/* @} */

/**
 *  \anchor CSL_PsilVpacTcCh
 *  \name PSIL channels for VPAC TC TX and RX
 *
 *  Macros for VPAC TC channels
 *
 *  @{
 */
#define CSL_PSIL_VPAC_TC_TX_CH(vpac_inst, tc_inst, chan)    (CSL_PSILCFG_NAVSS_MAIN_VPAC_DMPAC_PSILD_THREAD_OFFSET + (((uint32_t)(vpac_inst))*0x60U) + (((uint32_t)(tc_inst))*0x20U) + ((uint32_t)(chan)))
#define CSL_PSIL_VPAC_TC_RX_CH(vpac_inst, tc_inst, chan)    (CSL_PSILCFG_NAVSS_MAIN_VPAC_DMPAC_PSILS_THREAD_OFFSET + (((uint32_t)(vpac_inst))*0x60U) + (((uint32_t)(tc_inst))*0x20U) + ((uint32_t)(chan)))
/* @} */

/**
 *  \anchor CSL_PsilDmpacCh
 *  \name PSIL channels for DMPAC TX and RX
 *
 *  Macros for DMAC channels
 *
 *  @{
 */
#define CSL_PSIL_DMPAC_TX_CH(chan)  (CSL_PSILCFG_NAVSS_MAIN_VPAC_DMPAC_PSILD_THREAD_OFFSET + 0xC0U + ((uint32_t)(chan)))
#define CSL_PSIL_DMPAC_RX_CH(chan)  (CSL_PSILCFG_NAVSS_MAIN_VPAC_DMPAC_PSILS_THREAD_OFFSET + 0xC0U + ((uint32_t)(chan)))
/* @} */

#endif
