/*
 *
 * Copyright (C) 2015-2019 Texas Instruments Incorporated - http://www.ti.com/
 *
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

#ifndef PCIEV3__H
#define PCIEV3__H

#ifdef __cplusplus
extern "C" {
#endif

/* System level header files */
#include <stdint.h>
#include <stdlib.h>

/* enable both BAR_0 and BAR_1 as 32 bit non-prefetch, 4MB */
#define RC_BAR_CFG    0x80012914

/* ============================================================== */
/**
 *   @file  ti/drv/pcie/src/v3/pcie.h
 *
 *   @brief PCIe hw rev 0 sub-system API and Data Definitions
 *
 */

/** revision dependant config (in soc/device.c) for one device instance */
typedef struct
{
  volatile uint32_t        *pcieCtrlAddr; /** address of PCIECTRL register */
  volatile uint32_t        *pcieRefClkAddr; /** address of PCIEREFCLK register */
  void                     *userCfgBase;    /** base address of user config */
  void                     *intCfgBase;    /** base address of int config */
  uint8_t                   numLane; /** Lane Number **/
  uint8_t                   linkSpeed;  /** Link Speed **/
  uint8_t                   pfNum;  /** Physical Function Number **/
} Pciev3_DevParams;

/**
 * v3 does not have multiple config addresses, it only has a common base.
 * However it uses a remoteOffset.
 */
typedef struct
{
  void              *cfgBase;      /** @brief base address of RC/EP config */
  uint32_t           remoteOffset; /** @brief offset relative to data area for remote config */
} Pciev3_DeviceCfgBaseAddrs;

/** v3 version of @ref Pcie_open */
pcieRet_e Pciev3_open
(
  int32_t        deviceNum,  /**< [in] PCIe device number (0,1,...) */
  Pcie_Handle   *pHandle     /**< [out] Resulting instance handle */
);

/** v3 version of @ref Pcie_close */
pcieRet_e Pciev3_close
(
  Pcie_Handle *pHandle /**< [in] The PCIE LLD instance indentifier */
);

/** v3 version of @ref Pcie_readRegs */
pcieRet_e Pciev3_readRegs
(
  Pcie_Handle      handle,   /**< [in] The PCIE LLD instance identifier */
  pcieLocation_e   location, /**< [in] Local or remote peripheral */
  pcieRegisters_t *readRegs  /**< [in/out] List of registers to read */
);

/** v3 version of @ref Pcie_writeRegs */
pcieRet_e Pciev3_writeRegs
(
  Pcie_Handle      handle,   /**< [in] The PCIE LLD instance identifier */
  pcieLocation_e   location, /**< [in] Local or remote peripheral */
  pcieRegisters_t *writeRegs /**< [in] List of registers to write */
);

/** v3 version of @ref Pcie_setInterfaceMode */
pcieRet_e Pciev3_setInterfaceMode
(
  Pcie_Handle handle, /**< [in] specified interface */
  pcieMode_e mode     /**< [in] PCIE Mode */
);

/** v3 version of @ref Pcie_getMemSpaceReserved */
pcieRet_e Pciev3_getMemSpaceReserved 
(
  Pcie_Handle  handle,     /**< [in]  The PCIE LLD instance identifier */
  uint32_t    *resSize     /**< [out] Reserved space */
);

/** v3 version of @ref Pcie_getMemSpaceRange */
pcieRet_e Pciev3_getMemSpaceRange
(
  Pcie_Handle  handle,   /**< [in] The PCIE LLD instance identifier */
  void         **base,   /**< [out] The memory space base address */
  uint32_t      *size    /**< [out] Total size of the memory space [bytes] */
);

/** v3 version of @ref Pcie_cfgBar */
pcieRet_e Pciev3_cfgBar
(
  Pcie_Handle            handle,   /**< [in] The PCIE LLD instance identifier */
  const pcieBarCfg_t    *barCfg    /**< [in] BAR configuration parameters */
);

/** v3 version of @ref Pcie_atuRegionConfig */
pcieRet_e Pciev3_atuRegionConfig 
(
  Pcie_Handle      handle,   /**< [in] The PCIE LLD instance identifier */
  pcieLocation_e   location, /**< [in] local/remote */
  uint32_t         atuRegionIndex, /**< [in] index number to configure */
  const            pcieAtuRegionParams_t *atuRegionParams /**< [in] config structure */
);

/** v3 version of @ref Pcie_getPendingFuncInts */
pcieRet_e Pciev3_getPendingFuncInts
(
  Pcie_Handle      handle,   /**< [in] The PCIE LLD instance identifier */
  void            *pendingBits,/**< [out] rev-specific pending bits */
  int32_t          sizeMsiBits,/**< [in] size of msiBits in MAU */
  void            *msiBits /**< [out] rev-specific msi pending bits */
);

/** v3 version of @ref Pcie_clrPendingFuncInts */
pcieRet_e Pciev3_clrPendingFuncInts
(
  Pcie_Handle      handle,   /**< [in] The PCIE LLD instance identifier */
  void            *pendingBits,/**< [in] rev-specific pending bits to ack */
  int32_t          sizeMsiBits,/**< [in] size of msiBits in MAU */
  void            *msiBits /**< [in] rev-specific msi pending bits to ack */
);

#ifdef __cplusplus
}
#endif

#endif  /* _PCIEV3_H */

/* Nothing past this point */

