/********************************************************************
 * Copyright (C) 2020 Texas Instruments Incorporated.
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
 *  Name        : cslr_spi_mibspiram.h
*/
#ifndef CSLR_SPI_MIBSPIRAM_H_
#define CSLR_SPI_MIBSPIRAM_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include <ti/csl/cslr.h>
#include <stdint.h>

/**************************************************************************
* Hardware Region  :
**************************************************************************/
#define CSL_MIBSPIRAM_MAX_ELEMENTS                                   (128U)

/**************************************************************************
* Register Overlay Structure
**************************************************************************/
/**
 * @struct MIBSPIRam_t
 * @brief
 *   MIBSPI Buffer RAM Definition
 * @details
 *   This structure is used to access the MSS_MIBSPIA buffer memory.
 */
/**
 * @typedef MIBSPIRam
 * @brief
 *   MIBSPI Buffer RAM Definition
 * @details
 *   This structure is used to access the MSS_MIBSPIA buffer memory.
 */
typedef volatile struct CSL_mibspiRam_t
{
    uint32_t tx[CSL_MIBSPIRAM_MAX_ELEMENTS];
    uint32_t rx[CSL_MIBSPIRAM_MAX_ELEMENTS];
} CSL_mibspiRam;



/**************************************************************************
* Register Macros
**************************************************************************/


/**************************************************************************
* Field Definition Macros
**************************************************************************/
#define CSL_MIBSPIRAM_TX_BUFMODE_SHIFT        (29U)
#define CSL_MIBSPIRAM_TX_BUFMODE_MASK        (0xE0000000U)

#define CSL_MIBSPIRAM_TX_CSHOLD_SHIFT        (28U)
#define CSL_MIBSPIRAM_TX_CSHOLD_MASK        (0x10000000U)

#define CSL_MIBSPIRAM_TX_LOCK_SHIFT        (27U)
#define CSL_MIBSPIRAM_TX_LOCK_MASK        (0x08000000U)

#define CSL_MIBSPIRAM_TX_WDEL_SHIFT        (26U)
#define CSL_MIBSPIRAM_TX_WDEL_MASK        (0x04000000U)

#define CSL_MIBSPIRAM_TX_DFSEL_SHIFT        (24U)
#define CSL_MIBSPIRAM_TX_DFSEL_MASK        (0x03000000U)

#define CSL_MIBSPIRAM_TX_CSNR_SHIFT        (16U)
#define CSL_MIBSPIRAM_TX_CSNR_MASK        (0x00FF0000U)

#define CSL_MIBSPIRAM_TX_TXDATA_SHIFT        (0U)
#define CSL_MIBSPIRAM_TX_TXDATA_MASK        (0x0000FFFFU)

#define CSL_MIBSPIRAM_RX_RXEMPTY_SHIFT        (31U)
#define CSL_MIBSPIRAM_RX_RXEMPTY_MASK        (0x80000000U)

#define CSL_MIBSPIRAM_RX_RXOVR_SHIFT        (30U)
#define CSL_MIBSPIRAM_RX_RXOVR_MASK        (0x40000000U)

#define CSL_MIBSPIRAM_RX_TXFULL_SHIFT        (29U)
#define CSL_MIBSPIRAM_RX_TXFULL_MASK        (0x20000000U)

#define CSL_MIBSPIRAM_RX_BITERR_SHIFT        (28U)
#define CSL_MIBSPIRAM_RX_BITERR_MASK        (0x10000000U)

#define CSL_MIBSPIRAM_RX_DESYNC_SHIFT        (27U)
#define CSL_MIBSPIRAM_RX_DESYNC_MASK        (0x08000000U)

#define CSL_MIBSPIRAM_RX_PARITYERR_SHIFT        (26U)
#define CSL_MIBSPIRAM_RX_PARITYERR_MASK        (0x04000000U)

#define CSL_MIBSPIRAM_RX_TIMEOUT_SHIFT        (25U)
#define CSL_MIBSPIRAM_RX_TIMEOUT_MASK        (0x02000000U)

#define CSL_MIBSPIRAM_RX_DLENERR_SHIFT        (24U)
#define CSL_MIBSPIRAM_RX_DLENERR_MASK        (0x01000000U)

#define CSL_MIBSPIRAM_RX_LCSNR_SHIFT        (16U)
#define CSL_MIBSPIRAM_RX_LCSNR_MASK        (0x00FF0000U)

#define CSL_MIBSPIRAM_RX_RXDATA_SHIFT        (0U)
#define CSL_MIBSPIRAM_RX_RXDATA_MASK        (0x0000FFFFU)

#define CSL_MIBSPIRAM_RX_RXFLAGS_SHIFT        (16U)
#define CSL_MIBSPIRAM_RX_RXFLAGS_MASK        (0xFFFF0000U)



#define CSL_MIBSPIRAM_GET_TX_BUFMODE(ramBase,idx)    (CSL_FEXT((((CSL_mibspiRam *)(ramBase))->tx[idx]),MIBSPIRAM_TX_BUFMODE))
#define CSL_MIBSPIRAM_GET_TX_CSHOLD(ramBase,idx)    (CSL_FEXT((((CSL_mibspiRam *)(ramBase))->tx[idx]),MIBSPIRAM_TX_CSHOLD))
#define CSL_MIBSPIRAM_GET_TX_LOCK(ramBase,idx)    (CSL_FEXT((((CSL_mibspiRam *)(ramBase))->tx[idx]),MIBSPIRAM_TX_LOCK))
#define CSL_MIBSPIRAM_GET_TX_WDEL(ramBase,idx)    (CSL_FEXT((((CSL_mibspiRam *)(ramBase))->tx[idx]),MIBSPIRAM_TX_WDEL))
#define CSL_MIBSPIRAM_GET_TX_DFSEL(ramBase,idx)    (CSL_FEXT((((CSL_mibspiRam *)(ramBase))->tx[idx]),MIBSPIRAM_TX_DFSEL))
#define CSL_MIBSPIRAM_GET_TX_CSNR(ramBase,idx)    (CSL_FEXT((((CSL_mibspiRam *)(ramBase))->tx[idx]),MIBSPIRAM_TX_CSNR))
#define CSL_MIBSPIRAM_GET_TX_TXDATA(ramBase,idx)    (CSL_FEXT((((CSL_mibspiRam *)(ramBase))->tx[idx]),MIBSPIRAM_TX_TXDATA))


#define CSL_MIBSPIRAM_SET_TX_BUFMODE(ramBase,idx,val)    (CSL_FINS((((CSL_mibspiRam *)(ramBase))->tx[idx]),MIBSPIRAM_TX_BUFMODE,(val)))
#define CSL_MIBSPIRAM_SET_TX_CSHOLD(ramBase,idx,val)    (CSL_FINS((((CSL_mibspiRam *)(ramBase))->tx[idx]),MIBSPIRAM_TX_CSHOLD,(val)))
#define CSL_MIBSPIRAM_SET_TX_LOCK(ramBase,idx,val)    (CSL_FINS((((CSL_mibspiRam *)(ramBase))->tx[idx]),MIBSPIRAM_TX_LOCK,(val)))
#define CSL_MIBSPIRAM_SET_TX_WDEL(ramBase,idx,val)    (CSL_FINS((((CSL_mibspiRam *)(ramBase))->tx[idx]),MIBSPIRAM_TX_WDEL,(val)))
#define CSL_MIBSPIRAM_SET_TX_DFSEL(ramBase,idx,val)    (CSL_FINS((((CSL_mibspiRam *)(ramBase))->tx[idx]),MIBSPIRAM_TX_DFSEL,(val)))
#define CSL_MIBSPIRAM_SET_TX_CSNR(ramBase,idx,val)    (CSL_FINS((((CSL_mibspiRam *)(ramBase))->tx[idx]),MIBSPIRAM_TX_CSNR,(val)))
#define CSL_MIBSPIRAM_SET_TX_TXDATA(ramBase,idx,val)    (CSL_FINS((((CSL_mibspiRam *)(ramBase))->tx[idx]),MIBSPIRAM_TX_TXDATA,(val)))

#define CSL_MIBSPIRAM_GET_RX_RXEMPTY(ramBase,idx)    (CSL_FEXT((((CSL_mibspiRam *)(ramBase))->rx[idx]),MIBSPIRAM_RX_RXEMPTY))
#define CSL_MIBSPIRAM_GET_RX_RXOVR(ramBase,idx)    (CSL_FEXT((((CSL_mibspiRam *)(ramBase))->rx[idx]),MIBSPIRAM_RX_RXOVR))
#define CSL_MIBSPIRAM_GET_RX_TXFULL(ramBase,idx)    (CSL_FEXT((((CSL_mibspiRam *)(ramBase))->rx[idx]),MIBSPIRAM_RX_TXFULL))
#define CSL_MIBSPIRAM_GET_RX_BITERR(ramBase,idx)    (CSL_FEXT((((CSL_mibspiRam *)(ramBase))->rx[idx]),MIBSPIRAM_RX_BITERR))
#define CSL_MIBSPIRAM_GET_RX_DESYNC(ramBase,idx)    (CSL_FEXT((((CSL_mibspiRam *)(ramBase))->rx[idx]),MIBSPIRAM_RX_DESYNC))
#define CSL_MIBSPIRAM_GET_RX_PARITYERR(ramBase,idx)    (CSL_FEXT((((CSL_mibspiRam *)(ramBase))->rx[idx]),MIBSPIRAM_RX_PARITYERR))
#define CSL_MIBSPIRAM_GET_RX_TIMEOUT(ramBase,idx)    (CSL_FEXT((((CSL_mibspiRam *)(ramBase))->rx[idx]),MIBSPIRAM_RX_TIMEOUT))
#define CSL_MIBSPIRAM_GET_RX_DLENERR(ramBase,idx)    (CSL_FEXT((((CSL_mibspiRam *)(ramBase))->rx[idx]),MIBSPIRAM_RX_DLENERR))
#define CSL_MIBSPIRAM_GET_RX_LCSNR(ramBase,idx)    (CSL_FEXT((((CSL_mibspiRam *)(ramBase))->rx[idx]),MIBSPIRAM_RX_LCSNR))
#define CSL_MIBSPIRAM_GET_RX_RXDATA(ramBase,idx)    (CSL_FEXT((((CSL_mibspiRam *)(ramBase))->rx[idx]),MIBSPIRAM_RX_RXDATA))
#define CSL_MIBSPIRAM_GET_RX_RXFLAGS(ramBase,idx)    (CSL_FEXT((((CSL_mibspiRam *)(ramBase))->rx[idx]),MIBSPIRAM_RX_RXFLAGS))

#define CSL_MIBSPIRAM_SET_RX_RXEMPTY(ramBase,idx,val)    (CSL_FINS((((CSL_mibspiRam *)(ramBase))->rx[idx]),MIBSPIRAM_RX_RXEMPTY,(val)))
#define CSL_MIBSPIRAM_SET_RX_RXOVR(ramBase,idx,val)    (CSL_FINS((((CSL_mibspiRam *)(ramBase))->rx[idx]),MIBSPIRAM_RX_RXOVR,(val)))
#define CSL_MIBSPIRAM_SET_RX_TXFULL(ramBase,idx,val)    (CSL_FINS((((CSL_mibspiRam *)(ramBase))->rx[idx]),MIBSPIRAM_RX_TXFULL,(val)))
#define CSL_MIBSPIRAM_SET_RX_BITERR(ramBase,idx,val)    (CSL_FINS((((CSL_mibspiRam *)(ramBase))->rx[idx]),MIBSPIRAM_RX_BITERR,(val)))
#define CSL_MIBSPIRAM_SET_RX_DESYNC(ramBase,idx,val)    (CSL_FINS((((CSL_mibspiRam *)(ramBase))->rx[idx]),MIBSPIRAM_RX_DESYNC,(val)))
#define CSL_MIBSPIRAM_SET_RX_PARITYERR(ramBase,idx,val)    (CSL_FINS((((CSL_mibspiRam *)(ramBase))->rx[idx]),MIBSPIRAM_RX_PARITYERR,(val)))
#define CSL_MIBSPIRAM_SET_RX_TIMEOUT(ramBase,idx,val)    (CSL_FINS((((CSL_mibspiRam *)(ramBase))->rx[idx]),MIBSPIRAM_RX_TIMEOUT,(val)))
#define CSL_MIBSPIRAM_SET_RX_DLENERR(ramBase,idx,val)    (CSL_FINS((((CSL_mibspiRam *)(ramBase))->rx[idx]),MIBSPIRAM_RX_DLENERR,(val)))
#define CSL_MIBSPIRAM_SET_RX_LCSNR(ramBase,idx,val)    (CSL_FINS((((CSL_mibspiRam *)(ramBase))->rx[idx]),MIBSPIRAM_RX_LCSNR,(val)))
#define CSL_MIBSPIRAM_SET_RX_RXDATA(ramBase,idx,val)    (CSL_FINS((((CSL_mibspiRam *)(ramBase))->rx[idx]),MIBSPIRAM_RX_RXDATA,(val)))
#define CSL_MIBSPIRAM_SET_RX_RXFLAGS(ramBase,idx,val)    (CSL_FINS((((CSL_mibspiRam *)(ramBase))->rx[idx]),MIBSPIRAM_RX_RXFLAGS,(val)))


#ifdef __cplusplus
}
#endif
#endif
