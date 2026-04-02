/******************************************************************************
 * FILE PURPOSE:  SA LLD Common Interface APIs
 ******************************************************************************
 * FILE NAME:   salldcom.c
 *
 * DESCRIPTION: SA LLD Common Interface API implementations
 *
 * FUNCTION               DESCRIPTION
 * ========               ===========
 *
 * REVISION HISTORY:
 *
 * (C) Copyright 2009-2013, Texas Instruments Inc.
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
/* System level header files */
#include <stdlib.h>
#include <string.h>

/* SALLD header files */
#include <ti/drv/sa/salld.h>
#include <ti/drv/sa/sa_osal.h>
#include <ti/csl/cslr_cp_ace.h>
#include <ti/drv/sa/src/salldloc.h>
#include "ti/drv/sa/src/salldproto.h"


/* Define SA related base address */

/* Global Variable which describes the SA LLD Version Information */
const char sa_lld_version_str[] = SA_LLD_VERSION_STR ":" __DATE__  ":" __TIME__;

/* SA PHP IDs */
#define SA_PHP_ID_IPSEC1                  0
#define SA_PHP_ID_SRTP_AC                 1
#define SA_PHP_ID_IPSEC2                  2

/* SA PHP System Statistics */
#define SA_PHP_SRAM_SIZE              0x1000
#define SA_PHP_SRAM_SIZE_IN_UINT32     0x400
#define SA_SYS_STATS_BASE              0x200

/* SA PHP Command Label offset */
#define SA_CMD_LABEL_TBUF_OFFSET       ((0x100)/4)

/* SA Internal Buffer 6 offset */
#define SA_INT_BUF6_OFFSET             ((0xE00)/4)

/* SA PKT INFO Buffer Offset */
#define SA_PKT_INFO_BUF_OFFSET         ((0x3400)/4)

/* Offsets to system statistics block */
#define SA_SYS_STATS_ERR_OFFSET         0x00     /* PHP all */
#define SA_SYS_STATS_ESP_OFFSET         0x40     /* PHP 0/2 only */
#define SA_SYS_STATS_AH_OFFSET          0x80     /* PHP 0/2 only */
#define SA_SYS_STATS_SRTP_OFFSET        0x40     /* PHP 1 only */
#define SA_SYS_STATS_AC_OFFSET          0x80     /* PHP 1 only */

/* System Error Statistic */
/* Packet Instance Buffers not available */
#define SA_SYS_ERR_NOMEM_OFFSET       ((SA_SYS_STATS_BASE + SA_SYS_STATS_ERR_OFFSET + 0)/4)
/* Context Cache related errors */
#define SA_SYS_ERR_CTX_OFFSET         ((SA_SYS_STATS_BASE + SA_SYS_STATS_ERR_OFFSET + 4)/4)
/* SA processing engine related error */
#define SA_SYS_ERR_ENGINE_OFFSET      ((SA_SYS_STATS_BASE + SA_SYS_STATS_ERR_OFFSET + 8)/4)
/* Packet Procotol related error such as Invalid IP types */
#define SA_SYS_ERR_PROTO_OFFSET       ((SA_SYS_STATS_BASE + SA_SYS_STATS_ERR_OFFSET + 12)/4)

/* IPSEC ESP system statistics */
/* Number of replay failures with old packets */
#define SA_SYS_ESP_REPLAY_OLD_OFFSET  ((SA_SYS_STATS_BASE + SA_SYS_STATS_ESP_OFFSET + 0)/4)
/* Number of replay failures with duplicated packets */
#define SA_SYS_ESP_REPLAY_DUP_OFFSET  ((SA_SYS_STATS_BASE + SA_SYS_STATS_ESP_OFFSET + 4)/4)
/* Number of Authentication failure */
#define SA_SYS_ESP_AUTH_FAIL_OFFSET   ((SA_SYS_STATS_BASE + SA_SYS_STATS_ESP_OFFSET + 8)/4)
/* Total Number of Packets encrypted */
#define SA_SYS_ESP_ENC_HI_OFFSET      ((SA_SYS_STATS_BASE + SA_SYS_STATS_ESP_OFFSET + 12)/4)
#define SA_SYS_ESP_ENC_LO_OFFSET      ((SA_SYS_STATS_BASE + SA_SYS_STATS_ESP_OFFSET + 16)/4)
/* Total Number of Packets decrypted */
#define SA_SYS_ESP_DEC_HI_OFFSET      ((SA_SYS_STATS_BASE + SA_SYS_STATS_ESP_OFFSET + 20)/4)
#define SA_SYS_ESP_DEC_LO_OFFSET      ((SA_SYS_STATS_BASE + SA_SYS_STATS_ESP_OFFSET + 24)/4)

#define SA_SYS_ESP_OFFSET   SA_SYS_ESP_REPLAY_OLD_OFFSET


/* IPSEC AH system statistics */
/* Number of replay failures with old packets */
#define SA_SYS_AH_REPLAY_OLD_OFFSET   ((SA_SYS_STATS_BASE + SA_SYS_STATS_AH_OFFSET + 0)/4)
/* Number of replay failures with duplicated packets */
#define SA_SYS_AH_REPLAY_DUP_OFFSET   ((SA_SYS_STATS_BASE + SA_SYS_STATS_AH_OFFSET + 4)/4)
/* Number of Authentication failure */
#define SA_SYS_AH_AUTH_FAIL_OFFSET    ((SA_SYS_STATS_BASE + SA_SYS_STATS_AH_OFFSET + 8)/4)
/* Total Number of Packets encrypted */
#define SA_SYS_AH_ENC_HI_OFFSET       ((SA_SYS_STATS_BASE + SA_SYS_STATS_AH_OFFSET + 12)/4)
#define SA_SYS_AH_ENC_LO_OFFSET       ((SA_SYS_STATS_BASE + SA_SYS_STATS_AH_OFFSET + 16)/4)
/* Total Number of Packets decrypted */
#define SA_SYS_AH_DEC_HI_OFFSET       ((SA_SYS_STATS_BASE + SA_SYS_STATS_AH_OFFSET + 20)/4)
#define SA_SYS_AH_DEC_LO_OFFSET       ((SA_SYS_STATS_BASE + SA_SYS_STATS_AH_OFFSET + 24)/4)

#define SA_SYS_AH_OFFSET   SA_SYS_AH_REPLAY_OLD_OFFSET


/* SRTP system statistics */
/* Number of replay failures with old packets */
#define SA_SYS_SRTP_REPLAY_OLD_OFFSET ((SA_SYS_STATS_BASE + SA_SYS_STATS_SRTP_OFFSET + 0)/4)
/* Number of replay failures with duplicated packets */
#define SA_SYS_SRTP_REPLAY_DUP_OFFSET ((SA_SYS_STATS_BASE + SA_SYS_STATS_SRTP_OFFSET + 4)/4)
/* Number of Authentication failure */
#define SA_SYS_SRTP_AUTH_FAIL_OFFSET  ((SA_SYS_STATS_BASE + SA_SYS_STATS_SRTP_OFFSET + 8)/4)
/* Total Number of Packets encrypted */
#define SA_SYS_SRTP_ENC_HI_OFFSET     ((SA_SYS_STATS_BASE + SA_SYS_STATS_SRTP_OFFSET + 12)/4)
#define SA_SYS_SRTP_ENC_LO_OFFSET     ((SA_SYS_STATS_BASE + SA_SYS_STATS_SRTP_OFFSET + 16)/4)
/* Total Number of Packets decrypted */
#define SA_SYS_SRTP_DEC_HI_OFFSET     ((SA_SYS_STATS_BASE + SA_SYS_STATS_SRTP_OFFSET + 20)/4)
#define SA_SYS_SRTP_DEC_LO_OFFSET     ((SA_SYS_STATS_BASE + SA_SYS_STATS_SRTP_OFFSET + 24)/4)

#define SA_SYS_SRTP_OFFSET   SA_SYS_SRTP_REPLAY_OLD_OFFSET


/* Air Ciphering system statistics */
/* Number of Authentication failure */
#define SA_SYS_AC_AUTH_FAIL_OFFSET    ((SA_SYS_STATS_BASE + SA_SYS_STATS_AC_OFFSET + 0)/4)
/* Total Number of Packets encrypted */
#define SA_SYS_AC_ENC_HI_OFFSET       ((SA_SYS_STATS_BASE + SA_SYS_STATS_AC_OFFSET + 4)/4)
#define SA_SYS_AC_ENC_LO_OFFSET       ((SA_SYS_STATS_BASE + SA_SYS_STATS_AC_OFFSET + 8)/4)
/* Total Number of Packets decrypted */
#define SA_SYS_AC_DEC_HI_OFFSET       ((SA_SYS_STATS_BASE + SA_SYS_STATS_AC_OFFSET + 12)/4)
#define SA_SYS_AC_DEC_LO_OFFSET       ((SA_SYS_STATS_BASE + SA_SYS_STATS_AC_OFFSET + 16)/4)

#define SA_SYS_AC_OFFSET   SA_SYS_AC_AUTH_FAIL_OFFSET


/* SA PHP PDSP Version Number */
#define SA_PHP_VERSION_BASE           0x2f0

#define SA_PHP_VERSION_OFFSET         ((SA_PHP_VERSION_BASE + 0)/4)

/* SA TRNG related definitions */
#define SA_RNG_STARTUP_CYCLES_DEFAULT     (1 << 8)
#define SA_RNG_STARTUP_CYCLES_MIN         (1 << 8)
#define SA_RNG_STARTUP_CYCLES_MAX         (1 << 24)
#define SA_RNG_STARTUP_CYCLES_SHIFT       8

#define SA_RNG_MIN_REFILL_CYCLES_DEFAULT  (1 << 6)
#define SA_RNG_MIN_REFILL_CYCLES_MIN      (1 << 6)
#define SA_RNG_MIN_REFILL_CYCLES_MAX      (1 << 14)
#define SA_RNG_MIN_REFILL_CYCLES_SHIFT    6

#define SA_RNG_MAX_REFILL_CYCLES_DEFAULT  (1 << 24)
#define SA_RNG_MAX_REFILL_CYCLES_MIN      (1 << 8)
#define SA_RNG_MAX_REFILL_CYCLES_MAX      (1 << 24)
#define SA_RNG_MAX_REFILL_CYCLES_SHIFT    8

#define SA_RNG_CLOCK_DIVIDER_DEFAULT       1
#define SA_RNG_CLOCK_DIVIDER_MIN           1
#define SA_RNG_CLOCK_DIVIDER_MAX          16
#define SA_RNG_CLOCK_DIVIDER_SHIFT         0

#ifdef  NSS_LITE2
#define SA_RNG2_SAMPLE_CYCLES_DEFAULT  (512)
#define SA_RNG2_SAMPLE_CYCLES_MIN      (1)
#define SA_RNG2_SAMPLE_CYCLES_MAX      (65535)
#define SA_RNG2_SAMPLE_CYCLES_SHIFT    (0)

#define SA_RNG2_ALARMCNT_ALARM_THRESHOLD_RESETVAL (255)

/* RNG2 KNOWN ANSWER TEST KEYS */
#define SA_RNG2_KNOWN_ANSWER_KEYS_0  (0x603deb10)
#define SA_RNG2_KNOWN_ANSWER_KEYS_1  (0x15ca71be)
#define SA_RNG2_KNOWN_ANSWER_KEYS_2  (0x2b73aef0)
#define SA_RNG2_KNOWN_ANSWER_KEYS_3  (0x857d7781)
#define SA_RNG2_KNOWN_ANSWER_KEYS_4  (0x1f352c07)
#define SA_RNG2_KNOWN_ANSWER_KEYS_5  (0x3b6108d7)
#define SA_RNG2_KNOWN_ANSWER_KEYS_6  (0x2d9810a3)
#define SA_RNG2_KNOWN_ANSWER_KEYS_7  (0x0914dff4)

/* TRNG2 KNOWN ANSWER TEST INPUT */
#define SA_RNG2_KNOWN_ANSWER_TRNG_INPUT_0  (0x6bc1bee2)
#define SA_RNG2_KNOWN_ANSWER_TRNG_INPUT_1  (0x2e409f96)
#define SA_RNG2_KNOWN_ANSWER_TRNG_INPUT_2  (0xe93d7e11)
#define SA_RNG2_KNOWN_ANSWER_TRNG_INPUT_3  (0x7393172a)


/* TRNG2 KNOWN ANSWER TEST EXPECTED OUTPUT */
#define SA_RNG2_KNOWN_ANSWER_TRNG_OUTPUT_0  (0x6bc1bee2)
#define SA_RNG2_KNOWN_ANSWER_TRNG_OUTPUT_1  (0x2e409f96)
#define SA_RNG2_KNOWN_ANSWER_TRNG_OUTPUT_2  (0xe93d7e11)
#define SA_RNG2_KNOWN_ANSWER_TRNG_OUTPUT_3  (0x7393172a)

#endif

/***********************************************************************************************
 * FUNCTION PURPOSE: Convert SA TRNG cycle parameters
 ***********************************************************************************************
 * DESCRIPTION: Convert SA TRNG cycle parameters based on min, max, default and divider
 ***********************************************************************************************/
static uint16_t sap_rng_conv_cycle_params(uint32_t param, uint32_t max, uint32_t min,
                                          uint32_t default_value, uint16_t shift)
{
    if (param == 0)
    {
        param = default_value;
    }
    else if (param > max)
    {
        param = max;
    }
    else if (param < min)
    {
        param = min;
    }

    return ((uint16_t)(param >> shift));
}

/******************************************************************************
 * FUNCTION PURPOSE:  Obtains memory buffer requirements for an SALLD
 *                    instance
 ******************************************************************************
 * DESCRIPTION: This function obtains memory buffer requirements for an SALLD
 *         instance. the memory buffer requirements are in terms of the size and
 *         alignment array which must not be changed by external software.
 *         Worst case memory size requirements are computed using parameters
 *         provided by Sa_SizeCfg_t.
 *
 * int16_t  Sa_getBufferReq (
 *              Sa_SizeCfg_t *sizeCfg,   - configuration information to be used in
 *                                         estimating the worst case buffer sizes
 *              int          *sizes      - size requirement array
 *              int          *aligns)    - alignemnt requirement array
 *
 *  Assumptions:
 *      --All the memory is from heap.
 *      --There is no special requirement for memory.
 *****************************************************************************/
int16_t  Sa_getBufferReq (Sa_SizeCfg_t *sizeCfg, int sizes[], int aligns[])
{
  int alignment = max(8, sizeCfg->cacheLineSize);

  sizes[0] = SALLD_ROUND_UP(sizeof(salldObj_t), alignment);
  if (sizeCfg->ctrlBitMap & sa_SIZE_CONFIG_CREATE_SHADOW_INST)
  {
    sizes[0] *= 2;  /* Double the instance size to include the shadow */
  }
  aligns[0] = alignment;

  return(sa_ERR_OK);
}  /* Sa_getBufferReq */
#if !defined(NSS_LITE) && !defined(NSS_LITE2)
#define SA_CSL_SASS_ENABLE_ALL      (CSL_CP_ACE_CMD_STATUS_ENCSS_EN_MASK    | \
                                     CSL_CP_ACE_CMD_STATUS_ENCSS1_EN_MASK   | \
                                     CSL_CP_ACE_CMD_STATUS_AUTHSS_EN_MASK   | \
                                     CSL_CP_ACE_CMD_STATUS_AUTHSS1_EN_MASK  | \
                                     CSL_CP_ACE_CMD_STATUS_AIRSS_EN_MASK    | \
                                     CSL_CP_ACE_CMD_STATUS_TRNG_EN_MASK     | \
                                     CSL_CP_ACE_CMD_STATUS_PHP1SS_EN_MASK   | \
                                     CSL_CP_ACE_CMD_STATUS_PHP2SS_EN_MASK   | \
                                     CSL_CP_ACE_CMD_STATUS_PHP3SS_EN_MASK   | \
                                     CSL_CP_ACE_CMD_STATUS_CTXCACH_EN_MASK  | \
                                     CSL_CP_ACE_CMD_STATUS_PA_IN_PORT_EN_MASK       | \
                                     CSL_CP_ACE_CMD_STATUS_CDMA_IN_PORT_EN_MASK     | \
                                     CSL_CP_ACE_CMD_STATUS_PA_OUT_PORT_EN_MASK      | \
                                     CSL_CP_ACE_CMD_STATUS_CDMA_OUT_PORT_EN_MASK      \
                                     )
#else
#define SA_CSL_SASS_ENABLE_ALL      (CSL_CP_ACE_CMD_STATUS_ENCSS_EN_MASK    | \
                                     CSL_CP_ACE_CMD_STATUS_AUTHSS_EN_MASK   | \
                                     CSL_CP_ACE_CMD_STATUS_TRNG_EN_MASK     | \
                                     CSL_CP_ACE_CMD_STATUS_CTXCACH_EN_MASK  | \
                                     CSL_CP_ACE_CMD_STATUS_CDMA_IN_PORT_EN_MASK     | \
                                     CSL_CP_ACE_CMD_STATUS_CDMA_OUT_PORT_EN_MASK      \
                                     )
#endif                                     

/******************************************************************************
 * FUNCTION PURPOSE: Create and initialize an SALLD instance
 ******************************************************************************
 * DESCRIPTION: This function initialize an SALLD instance and its
 *         corresponding instance structure based on stystem configuration data
 *         such as the the call-out table and etc.
 *
 * int16_t Sa_create (
 *          Sa_Config_t *cfg,      - a pointer to configuration structure
 *          void*        bases,    - Array of the memory buffer base addresses
 *          Sa_Handle   *pHandle)  - store Instance handle which is the pointer
 *                                   to an initialized instance structures
 *
 * Return values:  sa_ERR_OK
 *                 sa_ERR_GEN
 *                 sa_ERR_PARAMS
 *                 sa_ERR_NOMEM
 *                 sa_ERR_INV_BUF
 *                 sa_ERR_INV_PROTO_TYPE
 *
 *****************************************************************************/
int16_t Sa_create (Sa_Config_t *cfg, void* bases[], Sa_Handle *pHandle)
{
  int  i;
  int  instSize, alignment;
  Bool fShadowInst;

  salldObj_t *inst = (salldObj_t *)bases[SALLD_BUFN];
  salldObj_t *shadowInst = NULL;
  CSL_Cp_aceRegs*  pSaRegs;

  /* Test instance pointer */
  if (pHandle == NULL || inst == NULL || cfg == NULL || cfg->sizeConfig == NULL)
    return(sa_ERR_PARAMS);

  alignment = max(8, cfg->sizeConfig->cacheLineSize);
  instSize = SALLD_ROUND_UP(sizeof(salldObj_t), alignment);

  if (cfg->sizeConfig->ctrlBitMap & sa_SIZE_CONFIG_CREATE_SHADOW_INST)
  {
    fShadowInst = TRUE;
    instSize += instSize; /* Double instance size for shadow instance */
  }
  else
  {
    fShadowInst = FALSE;
  }

  /* Invalidate the Cache Contents */
  Sa_osalBeginMemAccess(inst, instSize);
  memset(inst, 0, sizeof(salldObj_t));

  /* Allocate all dynamic buffers (base address != NULL ?)   */
  for (i = 0; i < SALLD_NBUF; i++) {
    if (bases[i] == NULL) {
      return (sa_ERR_NOMEM);
    }
    inst->memBaseOffsets[i] = sa_CONV_ADDR_TO_OFFSET(cfg->instPoolBaseAddr, bases[i]);
  }

  /* Verify all mandatory call-out APIs are implemented */
  if ((cfg->callTable == NULL)                    ||
      (cfg->callTable->ScAlloc == NULL)           ||
      (cfg->callTable->ScFree == NULL)            ||
      (cfg->callTable->ChanRegister == NULL)      ||
      (cfg->callTable->ChanUnRegister == NULL)    ||
      (cfg->callTable->ChanSendNullPkt == NULL))  {

    return(sa_ERR_PARAMS);
  }

  /* Default Instance Initialization */
  inst->magic          = SALLD_INST_MAGIC_WORD;
  inst->ID             = cfg->ID;
  salldLObj.baseAddr   = (void *) (uintptr_t) cfg->baseAddr;
  inst->stateBitfield  = 0;    /* clear all states  */
  SALLD_SET_STATE_ENABLE(inst, 1);

#if !defined(NSS_LITE) && !defined(NSS_LITE2)

  /* Record if halt on system error is set */
  if (cfg->ctrlBitMap & sa_CONFIG_CTRL_BITMAP_TRIGGER_SYS_ERR_HALT)
  {
    SALLD_SET_STATE_DBG_L1_COLLECT (inst, 1);
  }

  /* Record if packet information logging is set */
  if (cfg->ctrlBitMap & sa_CONFIG_CTRL_BITMAP_TRIGGER_PKT_INFO_LOG)
  {
    SALLD_SET_STATE_DBG_L2_COLLECT (inst, 1);
  }

  /* Is it the second generation SASS? */
  if (cfg->sizeConfig->ctrlBitMap & sa_SIZE_CONFIG_SASS_GEN2)
  {
    SALLD_SET_SASS_GEN2(inst, 1);
  }

  /* Initialize Instance Engine Pair Selector */
  if (SALLD_TEST_SASS_GEN2(inst))
  {
    if(cfg->engSelMode > sa_EngSelMode_USERSPECIFIED)
		    return(sa_ERR_PARAMS);
    else
        SALLD_SET_STATE_ENG_SEL_MODE(inst, cfg->engSelMode);
  }
  else
  {
    SALLD_SET_STATE_ENG_SEL_MODE(inst, SALLD_ENG_SELECT_MODE_NA);
  }
#else
  /* Is it the second generation (SA2_UL)*/
  if (cfg->sizeConfig->ctrlBitMap & sa_SIZE_CONFIG_SASS_UL_GEN2)
  {
    SALLD_SET_SASS_UL_GEN2(inst, 1);
  }

    if (cfg->ctrlBitMap & sa_CONFIG_CTRL_BITMAP_LIMIT_ACCESS)
    {
        SALLD_SET_LIMIT_ACCESS(inst, 1);
    }
#endif  

  /* Record the call-out table */
  SALLD_SET_LOC_INST_READY(&salldLObj, 1);
  salldLObj.callOutFuncs = *cfg->callTable;

  /* Copy special security context buffer pointer if necessary */
  salldLObj.intBuf = cfg->intBuf;

  /* Return the instance handle */
  *pHandle = (Sa_Handle)sa_CONV_ADDR_TO_OFFSET(cfg->instPoolBaseAddr, inst);

  /* Record the base addresses in the local object */
  salldLObj.instPoolBaseAddr = cfg->instPoolBaseAddr;
  salldLObj.scPoolBaseAddr   = cfg->scPoolBaseAddr;

  /* Enable all the sud-modules expect PKA in the SA */
  pSaRegs = (CSL_Cp_aceRegs*)(uintptr_t)cfg->baseAddr;
  #ifdef NSS_LITE2
    /*
     * Enable the RNG module if not enabled already. Note that on HS devices,
     * ENGINE_ENABLE already configured for all engines on SA2UL instance
     * managed by DMSC. The register is firewall-protected beyond this, so the
     * driver must not write the register. A check to the engine status will
     * confirm if the engine is already enabled. If so, do not reconfigure.
     */
    if((pSaRegs->MMR.CMD_STATUS & SA_CSL_SASS_ENABLE_ALL) !=
          SA_CSL_SASS_ENABLE_ALL)
    {
        if (SALLD_TEST_LIMIT_ACCESS(inst))
        {
            /* This is a critical error */
            return(sa_ERR_ENGINE_STATE);
        }
        else
        {
            pSaRegs->UPDATES.ENGINE_ENABLE |= SA_CSL_SASS_ENABLE_ALL;
        }
    }

    /* set the range for the security context if instructed to set */
    if ((cfg->ctrlBitMap & sa_CONFIG_CTRL_BITMAP_SET_SCPTR_RANGE) &&
            !(SALLD_TEST_LIMIT_ACCESS(inst)))
    {
        pSaRegs->UPDATES.SCPTR_PROMOTE_HI_RANGE_H = cfg->scPtrRange.scPtrPromoteHighRangeH;
        pSaRegs->UPDATES.SCPTR_PROMOTE_HI_RANGE_L = cfg->scPtrRange.scPtrPromoteHighRangeL;
        pSaRegs->UPDATES.SCPTR_PROMOTE_LOW_RANGE_H = cfg->scPtrRange.scPtrPromoteLowRangeH;
        pSaRegs->UPDATES.SCPTR_PROMOTE_LOW_RANGE_L = cfg->scPtrRange.scPtrPromoteLowRangeL;
    }
  #else
  pSaRegs->MMR.CMD_STATUS |= SA_CSL_SASS_ENABLE_ALL;
  #endif
  /* Populate the shadow Instance which will be used at another processer with different Endian mode if required */
  if (fShadowInst)
  {
    /* Shadow instance is created just after system instance in the memory
       hence the shadowoffset = system offset + size of system structure */
    inst->shadowInstOffset = (uint32_t) inst->memBaseOffsets[SALLD_BUFN] + sizeof (salldObj_t);
    shadowInst = (salldObj_t *)(sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, inst->shadowInstOffset));

    memset(shadowInst, 0, sizeof(salldObj_t));
    shadowInst->magic = salld_swiz32(SALLD_INST_MAGIC_WORD);
    shadowInst->ID = SALLD_SWIZ(inst->ID);
    shadowInst->stateBitfield = SALLD_SWIZ(inst->stateBitfield);
    shadowInst->shadowInstOffset = SALLD_SWIZ(inst->shadowInstOffset);

    /* Note: the local instance can not be used by the processors in different endian mode */
    /* The callout table can not be shared among ARM and DSP cores */
  }

  /* Once the SA LLD has been created we need to writeback the contents of its instance data */
  Sa_osalEndMemAccess(inst, instSize);

  return(sa_ERR_OK);
} /*  Sa_create  */

/******************************************************************************
 * FUNCTION PURPOSE: Activate the SALLD local instance
 ******************************************************************************
 * DESCRIPTION: This function activates the SALLD local instance. This function
 *              should be called once at each core which shares the SA LLD handle
 *              with the master core where the SA LLD instance was created.
 *
 * int16_t Sa_start (
 *
 *          Sa_Handle handle,      - SALLD channel identifier
 *          Sa_Config_t *cfg)      - a pointer to configuration structure
 *
 * Return values:  sa_ERR_OK
 *                 sa_ERR_GEN
 *                 sa_ERR_PARAMS
 *
 *****************************************************************************/
int16_t Sa_start (Sa_Handle handle, Sa_Config_t *cfg)
{
  salldObj_t *inst;
  uint32_t mtCsKey;
  int16_t ret = sa_ERR_OK;

  /* Test instance pointer */
  if (cfg == NULL || (inst = (salldObj_t *)sa_CONV_OFFSET_TO_ADDR(cfg->instPoolBaseAddr, handle)) == NULL)
    return(sa_ERR_PARAMS);

  /* Verify all mandatory call-out APIs are implemented */
  if ((cfg->callTable == NULL)                    ||
      (cfg->callTable->ScAlloc == NULL)           ||
      (cfg->callTable->ScFree == NULL)            ||
      (cfg->callTable->ChanRegister == NULL)      ||
      (cfg->callTable->ChanUnRegister == NULL)    ||
      (cfg->callTable->ChanSendNullPkt == NULL))  {

    return(sa_ERR_PARAMS);
  }
  /* Record the call-out table */
  /* CRITICAL Section Start: The global instance is a shared resource which needs to be
   * protected from the following:
   *  a) Multiple Cores acccess
   */
  Sa_osalMtCsEnter(&mtCsKey);

  /* Invalidate the Cache Contents */
  Sa_osalBeginMemAccess(inst, sizeof(salldObj_t));

  if (inst->magic != SALLD_INST_MAGIC_WORD)
  {
    if(inst->magic == salld_swiz32(SALLD_INST_MAGIC_WORD))
        ret = sa_ERR_INV_ENDIAN_MODE;
    else
        ret = sa_ERR_INV_HANDLE;
  }
  else
  {
    salldLObj.callOutFuncs = *cfg->callTable;
    /* Copy special security context buffer pointer if necessary */
    salldLObj.intBuf = cfg->intBuf;

	/* Record the base addresses in the local object */
    salldLObj.instPoolBaseAddr = cfg->instPoolBaseAddr;
    salldLObj.scPoolBaseAddr   = cfg->scPoolBaseAddr;
    salldLObj.baseAddr         = (void *) (uintptr_t)cfg->baseAddr;
  }

  /* Writeback the instance updates */
  Sa_osalEndMemAccess(inst, sizeof(salldObj_t));

  /* Critical Section End */
  Sa_osalMtCsExit(mtCsKey);

  return(ret);
} /*  Sa_start  */


/******************************************************************************
 * FUNCTION PURPOSE:  Delete an SALLD instance
 ******************************************************************************
 * DESCRIPTION: This function clears the SALLD instance and returns the base
 *         address of all buffers previously allocated for the SA LLD instance.
 *
 *  int16_t  Sa_close  (
 *              Sa_Handle handle,      - SALLD channel identifier
 *              void*     bases[])     - Output array of memory buffer base addresses
 *
 * Return values:  sa_ERR_OK
 *
 ******************************************************************************/
int16_t  Sa_close  (Sa_Handle handle, void* bases[])
{
  salldObj_t *inst = (salldObj_t *)sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, handle);
  int i;
  salldObj_t *shadowInst = NULL;
#if !defined(NSS_LITE) && !defined(NSS_LITE2)
  int numPdsps = SALLD_TEST_SASS_GEN2(inst)?3:2;
#endif
  CSL_Cp_aceRegs*  pSaRegs = (CSL_Cp_aceRegs*)salldLObj.baseAddr;
  CSL_Cp_aceTrngRegs* pRngRegs = &pSaRegs->TRNG;

  uint32_t mtKey;

  /* Non zero shadow instance offset indicates the presense of shadow instance */
  if (inst->shadowInstOffset)
  	 shadowInst = (salldObj_t *)(sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, inst->shadowInstOffset));

  /* Invalidate the Cache Contents */
  Sa_osalBeginMemAccess(inst, sizeof(salldObj_t));

  if (inst->magic != SALLD_INST_MAGIC_WORD)
  {
    if(inst->magic == salld_swiz32(SALLD_INST_MAGIC_WORD))
        return(sa_ERR_INV_ENDIAN_MODE);
    else
        return(sa_ERR_INV_HANDLE);
  }

  Sa_osalMtCsEnter(&mtKey);

  /* Reset all components at SA */
#if !defined(NSS_LITE) && !defined(NSS_LITE2)
  /* Disable and reset PDSPs */
  for (i = 0; i < numPdsps; i++)  {
    pSaRegs->PDSP_CONTROL_STATUS[i].PDSP_CONTROL =  0;
  }
#endif  
  pRngRegs->TRNG_CONTROL = 0; /* Disable RNG */

#ifdef NSS_LITE2
    /* Only disable engines when full access is available */
    if (!SALLD_TEST_LIMIT_ACCESS(inst))
    {
        pSaRegs->UPDATES.ENGINE_ENABLE = 0;
    }
#else
  pSaRegs->MMR.CMD_STATUS = 0;
#endif

  inst->stateBitfield = 0;  /* indicate SALLD instance is closed */
  inst->magic = 0;

  if (shadowInst)
  {
    shadowInst->magic = 0;
  }

  for (i = 0; i < SALLD_NBUF; i++) {
    bases[i] = (void *) sa_CONV_OFFSET_TO_ADDR (salldLObj.instPoolBaseAddr, inst->memBaseOffsets[i]);
  }

  /* Writeback the instance updates */
  Sa_osalEndMemAccess(inst, sizeof(salldObj_t));

  Sa_osalMtCsExit(mtKey);

  return(sa_ERR_OK);

} /* Sa_close */

#if !defined(NSS_LITE) && !defined(NSS_LITE2)
/******************************************************************************
 * FUNCTION PURPOSE: IPSEC ESP Send Data Utility
 ******************************************************************************
 * DESCRIPTION:
 *
 * void salld_esp_pre_proc_util
 *    (
 *      salldIpsecEspTxRxInfo_t *info,      - A pointer to SALLD IPSEC Tx/Rx Info
 *      Sa_PktDesc_t *pktDesc               - packet descriptor pointer
 *    )
 *
 * here's the format of ESP
 *
 *              |<-------- ESP HEADER ------>|<--------------- ENCRYPTED PAYLOAD ------------------>|
 *  *-----------*----------------------------*-------------*---------*------------------------------*-----*
 *  * IP HEADER * SPI | SEQUENCE NUMBER | IV * IP DATAGRAM * PADDING * PADDING LENGTH | NEXT HEADER * ICV *
 *  *-----------*----------------------------*-------------*---------*------------------------------*-----*
 *                                                                   |<---------- ESP TAIL -------->|
 *              |<----------------------------- AUTHENTICATED BY ICV ------------------------------>|
 *
 *
 *  Perform the following actions:
 *   - Reserve room for the ESP Header (SPI and sequence number) following
 *     the external IP header for Tunnel mode or the original IP header
 *     for transport mode.
 *   - Generate and insert the Initialization Vector (IV) in front of the ESP payload
 *   - Extract the protocol (next header) from the original IP header, and
 *     replace it with ESP Transport (50).
 *   - Calculate the ESP padding size, insert ESP padding and the ESP Trail
 *     (padding size + next header)
 *   - Adjust the payload length of the external IP header
 *   - Update the packet size and protocol (IP) payload size in the header
 *     parsing information to reserve room for the ESN and authentication data
 *     if necessary
 *   - Prepare the CPPI Software information as defined at section 5.1.12.
 *   - Update statistics
 *
 * Note: Assume that enough space is reserved in front of the segment bugffer
 *       to insert the ESP header. And enough room is reserved for the ESP
 *       padding and hash location.
 *****************************************************************************/
int16_t salld_esp_pre_proc_util (salldIpsecEspTxRxInfo_t *info,   Sa_PktDesc_t* pktDesc)
{
	Sa_ipsecNatTInfo_t* pNatTInfo = &info->natTInfo;
	Sa_PktDesc_t* pPktDesc = pktDesc;
	tword *pktIn, *pktSegTail;
	tword *pIP, *pOrig;
	tword *pIPNew, *pNew;
	int16_t espHdrSize, ipVerLen, espMiscSize, udpHdrSize;
	int16_t ipLen, payloadLen, paddingLen, encryptedLen, i, offset, oldPayloadLen;
	/* Segmentation support */
	int16_t ipSegmentIndex, ipSegmentOffset, tailSegmentOffset, payloadOffset, segOffsetTmp, segOffset, segLast;
	uint8_t nextHdr, nextHdrOffset;
	uint16_t f_ipV6;
	uint16_t *segUsedSizes;
	int extHdrLen, maxIpLen;

	/* Sanity Check */
	if(pPktDesc->nSegments < 1)
	  return (sa_ERR_PARAMS);

	/* Encrypt and Authenticate Packet(s) */
	payloadOffset = pPktDesc->payloadOffset;
	ipSegmentIndex = tailSegmentOffset = segOffsetTmp = i = segOffset = 0;

	segUsedSizes = pPktDesc->segUsedSizes;

	/* Calculate segment containing IP header as well as segment offset to IP Header*/
	segOffsetTmp = segUsedSizes[ipSegmentIndex];
	while(segOffset + segOffsetTmp < payloadOffset)
	{
	  segOffset += segOffsetTmp;
	  tailSegmentOffset += segOffsetTmp;
	  segOffsetTmp = segUsedSizes[++ipSegmentIndex];
	}
	ipSegmentOffset = payloadOffset - segOffset;

	/* Calculate segment containing ESP padding as well as segment offset to ESP padding*/
	for(i = ipSegmentIndex; i < (segLast = pPktDesc->nSegments-1);i++)
	{
	  tailSegmentOffset += segUsedSizes[i];
	}


	pOrig = pIP = (tword *)pPktDesc->segments[ipSegmentIndex];
	pIP += SALLD_BYTE_TO_WORD(ipSegmentOffset);
	pktIn = pIP;

	/*
	 * Calculate the size of authentictaion header and retrieve the pointer to the authentication header
	 */
	espHdrSize = IPSEC_ESP_HDR_BASIC_SIZE_BYTES + info->ivSize;
	ipVerLen = pktRead8bits_m(pktIn, IPV4_BYTE_OFFSET_VER_HLEN);

	if ((f_ipV6 = ((ipVerLen & IPV4_VER_MASK) == IPV6_VER_VALUE)))
	{
	  /* IPV6 requires that All header to be 8-byte aligned */
	  espHdrSize = SALLD_ROUND_UP(espHdrSize, 8);
	  ipLen = IPV6_HDR_SIZE_BYTES;

	  /* Extract payloadlen from the IP header */
	  payloadLen = pktRead16bits_m(pIP, IPV6_BYTE_OFFSET_PLEN) + IPV6_HDR_SIZE_BYTES;

	  nextHdr = pktRead8bits_m(pIP, IPV6_BYTE_OFFSET_PROTO);
	  nextHdrOffset = IPV6_BYTE_OFFSET_PROTO;

	  maxIpLen = pPktDesc->segUsedSizes[ipSegmentIndex] - ipSegmentOffset;

	  while ((nextHdr == IP_PROTO_IPV6_HOP_BY_HOP) ||
			 (nextHdr == IP_PROTO_IPV6_ROUTE)	   ||
			 (nextHdr == IP_PROTO_IPV6_DEST_OPT))
	  {
		  nextHdrOffset = ipLen + IPV6_OPT_HEADER_OFFSET_PROTO;
		  nextHdr = pktRead8bits_m(pIP, ipLen + IPV6_OPT_HEADER_OFFSET_PROTO);
		  extHdrLen = pktRead8bits_m(pIP, ipLen + IPV6_OPT_HEADER_OFFSET_LEN);
		  ipLen += (extHdrLen + 1) * IPV6_OPT_HEADER_LEN_UNIT_IN_BYTES;

		  if(ipLen > maxIpLen)
			  return (sa_ERR_PACKET);
	  }
	}
	else
	{
	  ipLen = (ipVerLen & IPV4_HLEN_MASK) << 2;
	  /* Extract payloadlen from the IP header */
	  payloadLen = pktRead16bits_m(pIP, IPV4_BYTE_OFFSET_LEN);
	  nextHdr = pktRead8bits_m(pIP, IPV4_BYTE_OFFSET_PROTO);
	}

	udpHdrSize = (info->validBitMap & sa_ESP_TXRX_VALID_IPSEC_NAT_T_INFO)?UDP_HDR_SIZE_BYTES:0;

	pIPNew = pIP - SALLD_BYTE_TO_WORD(espHdrSize + udpHdrSize);
	pNew = pOrig - SALLD_BYTE_TO_WORD(espHdrSize + udpHdrSize);

	misc_utlCopy((uint16_t *)pOrig, (uint16_t *)pNew,  SALLD_BYTE_TO_TUINT(ipLen+ipSegmentOffset));

	/*
	 * Calculate padding length
	 */
	encryptedLen = payloadLen - ipLen;

	if (info->encryptionBlockSize == 1)
	{
	  paddingLen = 0;
	}
	else
	{
	  paddingLen = (encryptedLen + IPSEC_ESP_TAIL_SIZE_BYTES) % info->encryptionBlockSize;
	  if (paddingLen)
	  {
		  paddingLen = info->encryptionBlockSize - paddingLen;
	  }
	}


	espMiscSize = paddingLen + IPSEC_ESP_TAIL_SIZE_BYTES + info->macSize;
	/* Extract the next header and update the IP header for ESP */
	oldPayloadLen = payloadLen;
	payloadLen += espHdrSize + espMiscSize + udpHdrSize;
	if (f_ipV6)
	{
	  pktWrite8bits_m(pIPNew, nextHdrOffset, udpHdrSize?IP_PROTO_UDP:IP_PROTO_ESP);
	  pktWrite16bits_m(pIPNew, IPV6_BYTE_OFFSET_PLEN, payloadLen - IPV6_HDR_SIZE_BYTES);
	}
	else
	{
	  pktWrite8bits_m(pIPNew, IPV4_BYTE_OFFSET_PROTO,  udpHdrSize?IP_PROTO_UDP:IP_PROTO_ESP);
	  pktWrite16bits_m(pIPNew, IPV4_BYTE_OFFSET_LEN, payloadLen);

	  /* Reclaculate the IPV4 header checksum */
	  salld_set_ipv4_chksum(pIPNew);
	}

	/* Populate the UDP header */
	pktIn = pIPNew + SALLD_BYTE_TO_WORD(ipLen);
	if (udpHdrSize)
	{
	  pktWrite16bits_m(pktIn, UDP_BYTE_OFFSET_SRC_PORT, pNatTInfo->srcPort);
	  pktWrite16bits_m(pktIn, UDP_BYTE_OFFSET_DEST_PORT, pNatTInfo->dstPort);
	  pktWrite16bits_m(pktIn, UDP_BYTE_OFFSET_LEN, payloadLen - ipLen);
	  pktWrite16bits_m(pktIn, UDP_BYTE_OFFSET_CKSUM, 0);
	}

	/* Populate the ESP header */
	pktIn += SALLD_BYTE_TO_WORD(udpHdrSize);
//	pktWrite32bits_m(pktIn, IPSEC_ESP_HDR_OFFSET_SPI, pConfig->spi);

	pktIn += SALLD_BYTE_TO_WORD(IPSEC_ESP_HDR_BASIC_SIZE_BYTES);

	/* Reserve room for IV */
	if (info->ivSize)
	{
	  pktIn += SALLD_BYTE_TO_WORD(info->ivSize);
	}

	/* Assumption here is ESP header will be in same segment as IP header
	 * IP Header must be in its own segment
	 */

	pktSegTail = (tword *) pPktDesc->segments[segLast];

	/*
	 * set padding values - the first padding byte should be 0x01, the second one should be 0x02 ...
	 */

	offset = oldPayloadLen + payloadOffset - tailSegmentOffset;
	for (i = 1; i <= paddingLen; i++, offset++)
	{
	  pktWrite8bits_m(pktSegTail, offset, i);
	}

	/* Write Padding Len and next payload type */
	pktWrite8bits_m(pktSegTail, offset,  (tword)(paddingLen));
	pktWrite8bits_m(pktSegTail, offset + 1, nextHdr);

	/* Adjust the packet length */
	pPktDesc->payloadLen = payloadLen - ipLen - udpHdrSize;
	pPktDesc->payloadOffset += (ipLen + udpHdrSize);
	pPktDesc->segUsedSizes[ipSegmentIndex] += (espHdrSize + udpHdrSize);
	pPktDesc->segUsedSizes[segLast] += espMiscSize;
	pPktDesc->size += espHdrSize + espMiscSize + udpHdrSize;
	pPktDesc->segments[ipSegmentIndex] = pNew;

	return (sa_ERR_OK);

}
/******************************************************************************
 * FUNCTION PURPOSE: IPSEC ESP Receive Data
 ******************************************************************************
 * DESCRIPTION:
 *
 * void salld_esp_post_proc_util (
 *      salldIpsecEspTxRxInfo_t *info,      - A pointer to SALLD IPSEC Tx/Rx Info
 *      Sa_PktDesc_t *pktDesc               - packet descriptor pointer
 *
 * Perform the following actions:
 *  - Update the corresponding statistics and return Error if the SASS packet
 *    error occurs.
 *  - Verify the ESP padding bytes
 *  - Extract the next header from the ESP Trailer and replace the one in the
 *    IP header with it.
 *  - Update the packet size and protocol (IP) payload size in the header
 *    parsing information by removing the size of the authentication data,
 *    the ESP header, padding and the ESP trailer.
 *  - Remove the ESP header, Trailer and the Authentication data
 *  - Update statistics
 */
int16_t salld_esp_post_proc_util (salldIpsecEspTxRxInfo_t *info,   Sa_PktDesc_t* pktDesc)
{
	Sa_PktDesc_t* pPktDesc = pktDesc;

	tword *pktSegTail, *pktPadding;
	tword *pHdr, *pHdrNew;
	tword *pIP;
	tword *pIPNew;
	int16_t espHdrSize, ipVerLen, espMiscSize, udpHdrSize;
	int16_t ipLen, payloadLen, paddingLen, i, offset;
	/* Segmentation support */
	int16_t ipSegmentIndex, ipSegmentOffset,
			tailSegmentIndex, tailSegmentOffset,
			paddingSegmentIndex, paddingSegmentOffset, firstPaddingLen,
			payloadOffset, segOffsetTmp, segOffset;
	uint16_t *segUsedSizes;
	uint8_t nextHdr, nextHdrOffset;
	uint16_t  f_ipV6, f_procEspTrail = TRUE;
	int16_t ret = sa_ERR_OK;
	uint16_t tempBuf[128];
	int extHdrLen, maxIpLen;

	/* Encrypt and Authenticate Packet(s) */
	payloadOffset = pPktDesc->payloadOffset;
	ipSegmentIndex = tailSegmentIndex = tailSegmentOffset = segOffsetTmp = i = segOffset = 0;
	segUsedSizes = pPktDesc->segUsedSizes;

	/* Calculate segment containing IP header as well as segment offset to IP Header */
	segOffsetTmp = segUsedSizes[ipSegmentIndex];
	while(segOffset + segOffsetTmp < payloadOffset)
	{
	  segOffset += segOffsetTmp;
	  segOffsetTmp = segUsedSizes[++ipSegmentIndex];
	}
	ipSegmentOffset = payloadOffset - segOffset;

	pHdr = (tword *)pPktDesc->segments[ipSegmentIndex];
	pIP = pHdr + SALLD_BYTE_TO_WORD(ipSegmentOffset);

	ipVerLen = pktRead8bits_m(pIP, IPV4_BYTE_OFFSET_VER_HLEN);

	if ((f_ipV6 = ((ipVerLen & IPV4_VER_MASK) == IPV6_VER_VALUE)))
	{
	  ipLen = IPV6_HDR_SIZE_BYTES;

	  /* Extract payloadlen from the IP header */
	  payloadLen = pktRead16bits_m(pIP, IPV6_BYTE_OFFSET_PLEN) + IPV6_HDR_SIZE_BYTES;

	  nextHdr = pktRead8bits_m(pIP, IPV6_BYTE_OFFSET_PROTO);
	  nextHdrOffset = IPV6_BYTE_OFFSET_PROTO;

	  maxIpLen = pPktDesc->segUsedSizes[ipSegmentIndex] - ipSegmentOffset;

	  while ((nextHdr == IP_PROTO_IPV6_HOP_BY_HOP) ||
			 (nextHdr == IP_PROTO_IPV6_ROUTE)	   ||
			 (nextHdr == IP_PROTO_IPV6_DEST_OPT))
	  {
		  nextHdrOffset = ipLen + IPV6_OPT_HEADER_OFFSET_PROTO;
		  nextHdr = pktRead8bits_m(pIP, ipLen + IPV6_OPT_HEADER_OFFSET_PROTO);
		  extHdrLen = pktRead8bits_m(pIP, ipLen + IPV6_OPT_HEADER_OFFSET_LEN);
		  ipLen += (extHdrLen + 1) * IPV6_OPT_HEADER_LEN_UNIT_IN_BYTES;

		  if(ipLen > maxIpLen)
			  return (sa_ERR_PACKET);
	  }

	}
	else
	{
	  ipLen = (ipVerLen & IPV4_HLEN_MASK) << 2;
	  /* Extract payloadlen from the IP header */
	  payloadLen = pktRead16bits_m(pIP, IPV4_BYTE_OFFSET_LEN);
	  nextHdr = pktRead8bits_m(pIP, IPV4_BYTE_OFFSET_PROTO);
	}

	if((nextHdr != IP_PROTO_ESP) && (nextHdr != IP_PROTO_UDP))
	  return (sa_ERR_PACKET);

	espHdrSize = IPSEC_ESP_HDR_BASIC_SIZE_BYTES + info->ivSize;
	udpHdrSize = (nextHdr == IP_PROTO_UDP)?UDP_HDR_SIZE_BYTES:0;

    /*
    * May need to adjust the payload length of outer IP if (hardware) RA-based inner IP reassembly occurs.
    * The inner IP reassembly will remove the extra outer IP paylaod such as ESP padding, trailer and
    * authentication tag and L2 CRC. Therefore, the total packet size should match the IP length within
    * inner IP header.
    *
    * Assumption: All the headers except L2 header reside at the same data segment.
    */
    if (info->validBitMap & sa_ESP_TXRX_VALID_RX_PAYLOAD_INFO)
    {
      Sa_RxPayloadInfo_t* pRxPayloadInfo = &info->rxPayloadInfo;
      int16_t ipVerLen2, payloadLen2, ipSegmentOffset2;
      tword *pIPin;

      if (pRxPayloadInfo->ipOffset2)
      {
          ipSegmentOffset2 = pRxPayloadInfo->ipOffset2 - segOffset;
          pIPin = pHdr + SALLD_BYTE_TO_WORD(ipSegmentOffset2);

          ipVerLen2 = pktRead8bits_m(pIPin, IPV4_BYTE_OFFSET_VER_HLEN);

          if ((ipVerLen2 & IPV4_VER_MASK) == IPV6_VER_VALUE )
          {
              payloadLen2 = pktRead16bits_m(pIPin, IPV6_BYTE_OFFSET_PLEN) + IPV6_HDR_SIZE_BYTES;
              nextHdr = IP_PROTO_IPV6_IN_IPV4;
          }
          else if ((ipVerLen2 & IPV4_VER_MASK) == IPV4_VER_VALUE )
          {
              payloadLen2 = pktRead16bits_m(pIPin, IPV4_BYTE_OFFSET_LEN);
              nextHdr = IP_PROTO_IP_IN_IP;
          }
          else
          {
              return (sa_ERR_PACKET);
          }

          if ((pRxPayloadInfo->ipOffset2 + payloadLen2) == (pPktDesc->payloadLen + payloadOffset))
          {
              /* Adjust the payload length of the outer IP */
              payloadLen = pPktDesc->payloadLen;
              f_procEspTrail = FALSE;
          }
      }
    }

	pHdrNew = pHdr + SALLD_BYTE_TO_WORD(espHdrSize + udpHdrSize);
	pIPNew = pIP + SALLD_BYTE_TO_WORD(espHdrSize + udpHdrSize);

	/* Overwrite to ESP header */
	misc_utlCopy((uint16_t *)pHdr, tempBuf,
				  SALLD_BYTE_TO_TUINT(ipSegmentOffset + ipLen));
	misc_utlCopy(tempBuf, (uint16_t *)pHdrNew,
				 SALLD_BYTE_TO_TUINT(ipSegmentOffset + ipLen));

	if (f_procEspTrail)
    {
	  /* Calculate segment containing ESP padding as well as segment offset to ESP padding */
	  tailSegmentIndex = ipSegmentIndex;
	  while((segOffset + segOffsetTmp) < (payloadOffset + payloadLen - info->macSize- IPSEC_ESP_TAIL_SIZE_BYTES))
	  {
		  segOffset+=segOffsetTmp;
		  segOffsetTmp = segUsedSizes[++tailSegmentIndex];
	  }
	  tailSegmentOffset = payloadOffset + payloadLen - info->macSize - IPSEC_ESP_TAIL_SIZE_BYTES - segOffset;

	  /* Get to ESP Tail */
	  /* Note: The tailSegmentOffset is always even-byte aligned due to the nature of padding */
	  pktSegTail = (tword *) pPktDesc->segments[tailSegmentIndex];
	  pktSegTail += SALLD_BYTE_TO_WORD(tailSegmentOffset);

	  /* Verify boundary conditions since in receive no assumptions about where
	   * packet data may cross over to next segment, each "|" below shows possible
	   * crossover boundaries, with the exception of padding, since padding could
	   * have split within itself
	   *
	   * | padding* | padding length(1 byte) | next header(1 byte) | tag
	   *
	   * */
	  paddingLen = pktRead8bits_m(pktSegTail, IPSEC_ESP_TAIL_OFFSET_PADDING_LEN);
	  nextHdr    = pktRead8bits_m(pktSegTail, IPSEC_ESP_TAIL_OFFSET_NEXT_HEADER);
	  offset	   = paddingLen & 0x01;

	  if (paddingLen < tailSegmentOffset)
	  {
	    /* All padding bytes resides at the same data segment which contains the ESP Tail */
	    paddingSegmentIndex = tailSegmentIndex;
	    paddingSegmentOffset = tailSegmentOffset - paddingLen;
	    pktPadding = pktSegTail - SALLD_BYTE_TO_WORD(paddingLen + offset);

	  }
	  else
	  {
	    /* The padding bytes are across from the previous data segment */
	    paddingSegmentIndex = tailSegmentIndex - 1;
	    paddingSegmentOffset = segUsedSizes[paddingSegmentIndex] - (paddingLen - tailSegmentOffset);
	    /* copy the first padding segment */
	    firstPaddingLen = (paddingLen - tailSegmentOffset) + offset;
	    pktPadding	  = (tword *) pPktDesc->segments[paddingSegmentIndex];
	    pktPadding	 += SALLD_BYTE_TO_WORD(paddingSegmentOffset - offset);
	    misc_utlCopy((uint16_t *)pktPadding,
				     (uint16_t *)tempBuf,
				     SALLD_BYTE_TO_TUINT(firstPaddingLen));
	    /* copy the remaining padding segment */
	    pktPadding = (tword *)tempBuf;
	    misc_utlCopy((uint16_t *)pPktDesc->segments[tailSegmentIndex], (uint16_t *)&pktPadding[SALLD_BYTE_TO_WORD(firstPaddingLen)],
				     SALLD_BYTE_TO_TUINT(tailSegmentOffset));

	  }

	  /* Padding Verification */
	  for (i = 1; i <= paddingLen; i++, offset++)
	  {
	    if ( i != pktRead8bits_m(pktPadding, offset))
	    {
		    ret = sa_ERR_PADDING_FAIL;
		    break;
	    }
	  }

	  espMiscSize = paddingLen + IPSEC_ESP_TAIL_SIZE_BYTES + info->macSize;

    }
    else
    {
      espMiscSize = 0;
    }

	/* Replace the next header and update the IP header for ESP */
	payloadLen -= (espHdrSize + espMiscSize + udpHdrSize);
	if (f_ipV6)
	{
	  pktWrite8bits_m(pIPNew, nextHdrOffset, nextHdr);
	  pktWrite16bits_m(pIPNew, IPV6_BYTE_OFFSET_PLEN, payloadLen - IPV6_HDR_SIZE_BYTES);

	}
	else
	{
	  pktWrite8bits_m(pIPNew, IPV4_BYTE_OFFSET_PROTO, nextHdr);
	  pktWrite16bits_m(pIPNew, IPV4_BYTE_OFFSET_LEN, payloadLen);

	  /* Reclaculate the IPV4 haeder checksum */
	  salld_set_ipv4_chksum(pIPNew);
	}

	/* Adjust the packet length */
	pPktDesc->size = payloadLen + payloadOffset;
	pPktDesc->payloadLen = payloadLen;
	pPktDesc->segUsedSizes[ipSegmentIndex] -= (espHdrSize + udpHdrSize);
#if defined(NSS_LITE)  || defined (NSS_LITE2)
	pPktDesc->segUsedSizes[paddingSegmentIndex] = paddingSegmentOffset;
#endif
	pPktDesc->segments[ipSegmentIndex] = pHdrNew;
    if (f_procEspTrail)
    {
	  pPktDesc->segUsedSizes[paddingSegmentIndex] = paddingSegmentOffset;
	  pPktDesc->nSegments = paddingSegmentIndex+1;
    }
    return (ret);
}

/******************************************************************************
 * FUNCTION PURPOSE: Sum of two IPSEC stats
 ******************************************************************************
 * DESCRIPTION: This function adds two set of IPSEC stats
 *****************************************************************************/
static void sa_ipsec_stats_add(Sa_IpsecSysStats_t *sum,  Sa_IpsecSysStats_t *src1,   Sa_IpsecSysStats_t *src2)
{
    sum->replayOld = src1->replayOld + src2->replayOld;
    sum->replayDup = src1->replayDup + src2->replayDup;
    sum->authFail  = src1->authFail  + src2->authFail;
    sum->pktEncHi  = src1->pktEncHi  + src2->pktEncHi;
    sum->pktEncLo  = src1->pktEncLo  + src2->pktEncLo;
    sum->pktDecHi  = src1->pktDecHi  + src2->pktDecHi;
    sum->pktDecLo  = src1->pktDecLo  + src2->pktDecLo;

    if((sum->pktEncLo < src1->pktEncLo) || (sum->pktEncLo < src2->pktEncLo))
        sum->pktEncHi++;

    if((sum->pktDecLo < src1->pktDecLo) || (sum->pktDecLo < src2->pktDecLo))
        sum->pktDecHi++;
}
/******************************************************************************
 * FUNCTION PURPOSE: Trigger the debug levels in SA LLD
 ******************************************************************************
 * DESCRIPTION: This function triggers the debug levels
 *****************************************************************************/
static void sa_trigger_halt_on_err(salldObj_t *inst, int numPdsps)
{
     int              i;
     uint32_t         flag;
     uint32_t         reg27, mask = 0x00008000;
     CSL_Cp_aceRegs*  pSaRegs = (CSL_Cp_aceRegs*)salldLObj.baseAddr;

     flag = SALLD_FW_CTRL_LEVEL1_DEBUG_MASK;

     if (SALLD_TEST_STATE_DBG_L2_COLLECT(inst)) {
         flag |= SALLD_FW_CTRL_LEVEL2_DEBUG_MASK;
     }

     /* Set the flag for firmware */
     for (i = 0; i < numPdsps; i++)  {
       /* Halt the PDSP */
       pSaRegs->PDSP_CONTROL_STATUS[i].PDSP_CONTROL =  1;

       /* Wait for halt - bit 15 to clear */
       while (pSaRegs->PDSP_CONTROL_STATUS[i].PDSP_CONTROL & mask);

       /* Read 32 bit value of register 27 */
       reg27 = pSaRegs->PDSP_DEBUG[i].PDSP_IGP[27];

       /* Clear the two debug flag bits (bit-15, bit-14) before setting */
       reg27 &= (uint32_t) 0xFFFF3FFF;

       /* Or the flag */
       reg27 |= flag;

       /* Write back the read value to register 27 */
       pSaRegs->PDSP_DEBUG[i].PDSP_IGP[27] = reg27;

       /* Run the PDSP */
       pSaRegs->PDSP_CONTROL_STATUS[i].PDSP_CONTROL =  3;
     }
}

/******************************************************************************
 * FUNCTION PURPOSE: Collect the debug log based on levels
 ******************************************************************************
 * DESCRIPTION: This function collects the debug info
 *****************************************************************************/
static void sa_collect_debug_info(int id, salldCoreDumpInfo_t *dbgInfo, int gen2)
{
  CSL_Cp_aceRegs*  pSaRegs = (CSL_Cp_aceRegs*)salldLObj.baseAddr;
  int              i, offset;
  uint32_t         *src, *dst;

  /* Derive the SRAM0 offset based on Gen2/Gen1 and PDSP id */
  if (gen2)
  {
     offset = id * SA_PHP_SRAM_SIZE_IN_UINT32;
  }
  else
  {
     switch (id) {
       case 0:
         offset = 0;
         break;
       case 1:
         offset = 2 * SA_PHP_SRAM_SIZE_IN_UINT32;
         break;
     }
  }

  salldL1DbgInfo_t *dbgLevel1 = &dbgInfo->dbgLevel1[id];

  /* Collect PDSP Control and Status snap shots */
  dbgLevel1->ctrl_status[0] = pSaRegs->PDSP_CONTROL_STATUS[id].PDSP_CONTROL;
  for ( i = 0; i < SA_DBG_COLLECT_NUM_STATUS; i++)
  {
    dbgLevel1->ctrl_status[i+1] = pSaRegs->PDSP_CONTROL_STATUS[id].PDSP_STATUS;
  }

  /* Halt the PDSP before reading the debug registers */
  pSaRegs->PDSP_CONTROL_STATUS[id].PDSP_CONTROL =  1;

  /* Collect 32 debug registers */
  src = (uint32_t *) &pSaRegs->PDSP_DEBUG[id].PDSP_IGP[0];
  dst = (uint32_t *) &dbgLevel1->dbgregs[0];
  for ( i = 0; i < (SA_DBG_COLLECT_DBGREGS_SIZE/4); i++)
  {
      dst[i] = src[i];
  }

  /* Collect Last command label and temporary buffers */
  offset += SA_CMD_LABEL_TBUF_OFFSET;
  src = (uint32_t *) &pSaRegs->SRAM0[offset];
  dst = (uint32_t *) &dbgLevel1->cmdLblTmpBuf[0];
  for ( i = 0; i < (SA_DBG_COLLECT_DBGREGS_SIZE/4); i++)
  {
      dst[i] = src[i];
  }

  /* Collect Internal Buf area */
  offset += SA_INT_BUF6_OFFSET;
  src = (uint32_t *) &pSaRegs->SRAM0[offset];
  dst = (uint32_t *) &dbgLevel1->intBuf[0];
  for ( i = 0; i < (SA_DBG_COLLECT_DBGREGS_SIZE/4); i++)
  {
      dst[i] = src[i];
  }

  return;
}
#endif

/******************************************************************************
 * FUNCTION PURPOSE:  SALLD Get Sysyem Stats
 ******************************************************************************
 * DESCRIPTION: This function obtains SALLD system statistics
 *
 *  int16_t  Sa_getSysStats (
 *   Sa_Handle        handle      - SALLD instance handle
 *   Sa_SysStats_t    *stats)     - a pointer to system statistics
 *
 * Return values:  sa_ERR_OK
 *                 sa_ERR_PARAMS
 *
 ******************************************************************************/
int16_t Sa_getSysStats(Sa_Handle handle, Sa_SysStats_t *stats)
{
#if !defined(NSS_LITE) && !defined(NSS_LITE2)
  salldObj_t *inst = (salldObj_t *)sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, handle);
  Sa_IpsecSysStats_t esp1, esp2, ah1, ah2;
  uint32_t           *src, *dst;
  uint32_t            i, len;

  CSL_Cp_aceRegs*  pSaRegs = (CSL_Cp_aceRegs*)salldLObj.baseAddr;

  if (inst->magic != SALLD_INST_MAGIC_WORD)
  {
    if(inst->magic == salld_swiz32(SALLD_INST_MAGIC_WORD))
        return(sa_ERR_INV_ENDIAN_MODE);
    else
        return(sa_ERR_INV_HANDLE);
  }

  if (!stats)
  {
    return(sa_ERR_PARAMS);
  }

  if (SALLD_TEST_SASS_GEN2(inst))
  {
    /*
     * The second generation SASS contains three PDSPs and two sets of IPSEC enhines.
     * Three statistics blocks:
     *   0xC000: IPSEC1
     *   0xD000: SRTP/Air Ciphering
     *   0xE000: IPSEC2
     *
     */
    stats->err.errNoMem  = pSaRegs->SRAM0[SA_SYS_ERR_NOMEM_OFFSET] +
                            pSaRegs->SRAM0[SA_SYS_ERR_NOMEM_OFFSET + SA_PHP_SRAM_SIZE_IN_UINT32] +
                            pSaRegs->SRAM0[SA_SYS_ERR_NOMEM_OFFSET + 2*SA_PHP_SRAM_SIZE_IN_UINT32];
    stats->err.errCtx    = pSaRegs->SRAM0[SA_SYS_ERR_CTX_OFFSET] +
                            pSaRegs->SRAM0[SA_SYS_ERR_CTX_OFFSET + SA_PHP_SRAM_SIZE_IN_UINT32] +
                            pSaRegs->SRAM0[SA_SYS_ERR_CTX_OFFSET + 2*SA_PHP_SRAM_SIZE_IN_UINT32];
    stats->err.errEngine = pSaRegs->SRAM0[SA_SYS_ERR_ENGINE_OFFSET] +
                            pSaRegs->SRAM0[SA_SYS_ERR_ENGINE_OFFSET + SA_PHP_SRAM_SIZE_IN_UINT32] +
                            pSaRegs->SRAM0[SA_SYS_ERR_ENGINE_OFFSET + 2*SA_PHP_SRAM_SIZE_IN_UINT32];
    stats->err.errProto  = pSaRegs->SRAM0[SA_SYS_ERR_PROTO_OFFSET] +
                            pSaRegs->SRAM0[SA_SYS_ERR_PROTO_OFFSET + SA_PHP_SRAM_SIZE_IN_UINT32] +
                            pSaRegs->SRAM0[SA_SYS_ERR_PROTO_OFFSET + 2*SA_PHP_SRAM_SIZE_IN_UINT32];

    src = (uint32_t *) &pSaRegs->SRAM0[SA_SYS_ESP_OFFSET];
    dst = (uint32_t* ) &esp1;
    len = (sizeof(Sa_IpsecSysStats_t)/4);
    for (i = 0; i < len; i++)
    {
        dst[i] = src[i];
    }

    src = (uint32_t *) &pSaRegs->SRAM0[SA_SYS_AH_OFFSET];
    dst = (uint32_t* ) &ah1;
    len = (sizeof(Sa_IpsecSysStats_t)/4);
    for (i = 0; i < len; i++)
    {
        dst[i] = src[i];
    }

    src = (uint32_t *) &pSaRegs->SRAM0[SA_SYS_ESP_OFFSET + 2*SA_PHP_SRAM_SIZE_IN_UINT32];
    dst = (uint32_t* ) &esp2;
    len = (sizeof(Sa_IpsecSysStats_t)/4);
    for (i = 0; i < len; i++)
    {
        dst[i] = src[i];
    }

    src = (uint32_t *) &pSaRegs->SRAM0[SA_SYS_AH_OFFSET  + 2*SA_PHP_SRAM_SIZE_IN_UINT32];
    dst = (uint32_t* ) &ah2;
    len = (sizeof(Sa_IpsecSysStats_t)/4);
    for (i = 0; i < len; i++)
    {
        dst[i] = src[i];
    }

    src = (uint32_t *) &pSaRegs->SRAM0[SA_SYS_SRTP_OFFSET + SA_PHP_SRAM_SIZE_IN_UINT32];
    dst = (uint32_t* ) &stats->srtp;
    len = (sizeof(Sa_SrtpSysStats_t)/4);
    for (i = 0; i < len; i++)
    {
        dst[i] = src[i];
    }

    src = (uint32_t *) &pSaRegs->SRAM0[SA_SYS_AC_OFFSET + SA_PHP_SRAM_SIZE_IN_UINT32];
    dst = (uint32_t* ) &stats->ac;
    len = (sizeof(Sa_AcSysStats_t)/4);
    for (i = 0; i < len; i++)
    {
        dst[i] = src[i];
    }

    sa_ipsec_stats_add(&stats->esp, &esp1, &esp2);
    sa_ipsec_stats_add(&stats->ah, &ah1, &ah2);

  }
  else
  {
    /*
     * The first generation SASS contains two PDSPs and one set of IPSEC enhines.
     * Two statistics blocks:
     *   0xC000: IPSEC1
     *   0xE000: SRTP/Air Ciphering
     */

    stats->err.errNoMem  = pSaRegs->SRAM0[SA_SYS_ERR_NOMEM_OFFSET] +
                            pSaRegs->SRAM0[SA_SYS_ERR_NOMEM_OFFSET + 2*SA_PHP_SRAM_SIZE_IN_UINT32];
    stats->err.errCtx    = pSaRegs->SRAM0[SA_SYS_ERR_CTX_OFFSET] +
                            pSaRegs->SRAM0[SA_SYS_ERR_CTX_OFFSET + 2*SA_PHP_SRAM_SIZE_IN_UINT32];
    stats->err.errEngine = pSaRegs->SRAM0[SA_SYS_ERR_ENGINE_OFFSET] +
                            pSaRegs->SRAM0[SA_SYS_ERR_ENGINE_OFFSET + 2*SA_PHP_SRAM_SIZE_IN_UINT32];
    stats->err.errProto  = pSaRegs->SRAM0[SA_SYS_ERR_PROTO_OFFSET] +
                            pSaRegs->SRAM0[SA_SYS_ERR_PROTO_OFFSET + 2*SA_PHP_SRAM_SIZE_IN_UINT32];


    src = (uint32_t *) &pSaRegs->SRAM0[SA_SYS_ESP_OFFSET];
    dst = (uint32_t* ) &stats->esp;
    len = (sizeof(Sa_IpsecSysStats_t)/4);
    for (i = 0; i < len; i++)
    {
        dst[i] = src[i];
    }

    src = (uint32_t *) &pSaRegs->SRAM0[SA_SYS_AH_OFFSET];
    dst = (uint32_t* ) &stats->ah;
    len = (sizeof(Sa_IpsecSysStats_t)/4);
    for (i = 0; i < len; i++)
    {
        dst[i] = src[i];
    }

    src = (uint32_t *) &pSaRegs->SRAM0[SA_SYS_SRTP_OFFSET + (2*SA_PHP_SRAM_SIZE_IN_UINT32)];
    dst = (uint32_t* ) &stats->srtp;
    len = (sizeof(Sa_SrtpSysStats_t)/4);
    for (i = 0; i < len; i++)
    {
        dst[i] = src[i];
    }

    src = (uint32_t *) &pSaRegs->SRAM0[SA_SYS_AC_OFFSET + (2*SA_PHP_SRAM_SIZE_IN_UINT32)];
    dst = (uint32_t* ) &stats->ac;
    len = (sizeof(Sa_AcSysStats_t)/4);
    for (i = 0; i < len; i++)
    {
        dst[i] = src[i];
    }

  }
                  
  return(sa_ERR_OK);
#else
  return (sa_ERR_API_UNSUPPORTED);
#endif  
  
}


/******************************************************************************
 * FUNCTION PURPOSE: Create and initialize the TRNG module
 ******************************************************************************
 * DESCRIPTION: The function is called to initialize and configure the TRNG
 *          (True Random Number Generator) module inside SA
 *
 * int16_t Sa_rngInit (
 *          Sa_Handle            handle  - salld instance handle
 *          Sa_RngConfigParams_t *cfg)   - a pointer to configuration structure
 *
 * Return values:  sa_ERR_OK
 *                 sa_ERR_PARAMS
 *
 *****************************************************************************/
int16_t Sa_rngInit (Sa_Handle handle, Sa_RngConfigParams_t* pCfg)
{
#if defined (NSS_LITE2)
  return (sa_ERR_API_UNSUPPORTED);
#else
  salldObj_t *inst = (salldObj_t *)sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, handle);
  CSL_Cp_aceRegs*  pSaRegs = (CSL_Cp_aceRegs*)salldLObj.baseAddr;
  CSL_Cp_aceTrngRegs* pRngRegs = &pSaRegs->TRNG;
  uint32_t mtKey;
  uint16_t startup_cycles, min_refill_cycles, max_refill_cycles, clock_divider;

  if(pCfg == NULL)
    return(sa_ERR_PARAMS);

  /* Convert the configuration parameters */
  startup_cycles = sap_rng_conv_cycle_params(pCfg->startupCycles, SA_RNG_STARTUP_CYCLES_MAX,
                                             SA_RNG_STARTUP_CYCLES_MIN, SA_RNG_STARTUP_CYCLES_DEFAULT,
                                             SA_RNG_STARTUP_CYCLES_SHIFT);
  min_refill_cycles = sap_rng_conv_cycle_params(pCfg->minRefillCycles, SA_RNG_MIN_REFILL_CYCLES_MAX,
                                                    SA_RNG_MIN_REFILL_CYCLES_MIN, SA_RNG_MIN_REFILL_CYCLES_DEFAULT,
                                                    SA_RNG_MIN_REFILL_CYCLES_SHIFT);
  max_refill_cycles = sap_rng_conv_cycle_params(pCfg->maxRefillCycles, SA_RNG_MAX_REFILL_CYCLES_MAX,
                                                SA_RNG_MAX_REFILL_CYCLES_MIN, SA_RNG_MAX_REFILL_CYCLES_DEFAULT,
                                                SA_RNG_MAX_REFILL_CYCLES_SHIFT);

  clock_divider = sap_rng_conv_cycle_params(pCfg->clockDiv, SA_RNG_CLOCK_DIVIDER_MAX,
                                            SA_RNG_CLOCK_DIVIDER_MIN, SA_RNG_CLOCK_DIVIDER_DEFAULT,
                                            SA_RNG_CLOCK_DIVIDER_SHIFT);

  Sa_osalMtCsEnter(&mtKey);

  /* Invalidate the Cache Contents */
  Sa_osalBeginMemAccess(inst, sizeof(salldObj_t));

  if (inst->magic != SALLD_INST_MAGIC_WORD)
  {

    Sa_osalMtCsExit(mtKey);

    if(inst->magic == salld_swiz32(SALLD_INST_MAGIC_WORD))
        return(sa_ERR_INV_ENDIAN_MODE);
    else
        return(sa_ERR_INV_HANDLE);
  }

  /*
   * Initialize the TRNG only if it has not been initialized or
   * the sa_RNG_CTRL_REINIT flag is set
   */
  if(SALLD_TEST_STATE_RNG(inst) && !(pCfg->ctrlBitfield & sa_RNG_CTRL_REINIT))
  {
    Sa_osalMtCsExit(mtKey);
    return (sa_ERR_OK);
  }

  /* Enable the RNG module */
  pSaRegs->MMR.CMD_STATUS |= CSL_CP_ACE_CMD_STATUS_TRNG_EN_MASK;

  /* Configure the RNG module */
  pRngRegs->TRNG_CONTROL = 0; /* Disable RNG */
  pRngRegs->TRNG_CONTROL = startup_cycles << CSL_CP_ACE_TRNG_CONTROL_STARTUP_CYCLES_SHIFT;
  pRngRegs->TRNG_CONFIG = (min_refill_cycles << CSL_CP_ACE_TRNG_CONFIG_MIN_REFILL_CYCLES_SHIFT) |
                          ((clock_divider - 1) << CSL_CP_ACE_TRNG_CONFIG_SAMPLE_DIV_SHIFT)      |
                          (max_refill_cycles << CSL_CP_ACE_TRNG_CONFIG_MAX_REFILL_CYCLES_SHIFT);
  if (pCfg->ctrlBitfield & sa_RNG_CTRL_ENABLE_INT)
  {
    pRngRegs->TRNG_INTMASK |= CSL_CP_ACE_TRNG_INTMASK_READY_MASK;
  }
  else
  {
    pRngRegs->TRNG_INTMASK &= ~CSL_CP_ACE_TRNG_INTMASK_READY_MASK;

  }

  /* Enable the RNG module */
  pRngRegs->TRNG_CONTROL |= CSL_CP_ACE_TRNG_CONTROL_ENABLE_TRNG_MASK;
  SALLD_SET_STATE_RNG(inst, 1);

  /* Writeback the instance updates */
  Sa_osalEndMemAccess(inst, sizeof(salldObj_t));

  Sa_osalMtCsExit(mtKey);
  return (sa_ERR_OK);
#endif
}

/******************************************************************************
 * FUNCTION PURPOSE: Create and initialize the TRNG2 module
 ******************************************************************************
 * DESCRIPTION: The function is called to initialize and configure the TRNG2
 *          (True Random Number Generator-2) module inside SA2UL
 *
 * int16_t Sa_rng2Init (
 *          Sa_Handle            handle  - salld instance handle
 *          Sa_Rng2ConfigParams_t *cfg)   - a pointer to configuration structure
 *
 * Return values:  sa_ERR_OK
 *                 sa_ERR_PARAMS
 *
 *****************************************************************************/
int16_t Sa_rng2Init (Sa_Handle handle, Sa_Rng2ConfigParams_t* pCfg)
{
#if !defined (NSS_LITE2)
      return (sa_ERR_API_UNSUPPORTED);
#else
  salldObj_t *inst = (salldObj_t *)sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, handle);
  CSL_Cp_aceRegs*  pSaRegs = (CSL_Cp_aceRegs*)salldLObj.baseAddr;
  CSL_Cp_aceTrngRegs* pRngRegs = &pSaRegs->TRNG;
  uint32_t mtKey;
  uint16_t sample_cycles, clock_divider;
  uint8_t alarm_thr = (uint8_t) SA_RNG2_ALARMCNT_ALARM_THRESHOLD_RESETVAL;
  uint32_t  bitmask, proceed;
  uint32_t  i;

  if(pCfg == NULL)
    return(sa_ERR_PARAMS);

  /* Convert the configuration parameters */
  sample_cycles = sap_rng_conv_cycle_params(pCfg->sampleCycles, SA_RNG2_SAMPLE_CYCLES_MAX,
                                                SA_RNG2_SAMPLE_CYCLES_MIN, SA_RNG2_SAMPLE_CYCLES_DEFAULT,
                                                SA_RNG2_SAMPLE_CYCLES_SHIFT);

  /* Using the same min and max values */
  clock_divider = sap_rng_conv_cycle_params(pCfg->clockDiv, SA_RNG_CLOCK_DIVIDER_MAX,
                                            SA_RNG_CLOCK_DIVIDER_MIN, SA_RNG_CLOCK_DIVIDER_DEFAULT,
                                            SA_RNG_CLOCK_DIVIDER_SHIFT);

  Sa_osalMtCsEnter(&mtKey);

  /* Invalidate the Cache Contents */
  Sa_osalBeginMemAccess(inst, sizeof(salldObj_t));

  if (inst->magic != SALLD_INST_MAGIC_WORD)
  {

    Sa_osalMtCsExit(mtKey);

    if(inst->magic == salld_swiz32(SALLD_INST_MAGIC_WORD))
        return(sa_ERR_INV_ENDIAN_MODE);
    else
        return(sa_ERR_INV_HANDLE);
  }

  /*
   * Initialize the TRNG only if it has not been initialized or
   * the sa_RNG_CTRL_REINIT flag is set
   */
  if(SALLD_TEST_STATE_RNG(inst) && !(pCfg->ctrlBitfield & sa_RNG2_CTRL_REINIT))
  {
    Sa_osalMtCsExit(mtKey);
    return (sa_ERR_OK);
  }

    /*
     * Enable the RNG module if not enabled already. Note that on HS devices,
     * ENGINE_ENABLE already configured for all engines on SA2UL instance
     * managed by DMSC. The register is firewall-protected beyond this, so the
     * driver must not write the register. A check to the engine status will
     * confirm if the engine is already enabled. If so, do not reconfigure.
     */
    if((pSaRegs->MMR.CMD_STATUS & CSL_CP_ACE_CMD_STATUS_TRNG_EN_MASK) !=
        CSL_CP_ACE_CMD_STATUS_TRNG_EN_MASK)
    {
        if (SALLD_TEST_LIMIT_ACCESS(inst))
        {
            /* This is a critical error */
            return(sa_ERR_ENGINE_STATE);
        }
        else
        {
            pSaRegs->UPDATES.ENGINE_ENABLE |= CSL_CP_ACE_CMD_STATUS_TRNG_EN_MASK;
        }
    }

  /* Configure the RNG module */
  /* 1. Make sure the engine is idle by writing zeroes to the TRNG_CONTROL register twice. */
  pRngRegs->TRNG_CONTROL = 0; /* Disable RNG */

  /* 2. Write all configuration values in the TRNG_CONFIG and TRNG_ALARMCNT registers, 
   * write zeroes to the TRNG_ALARMMASK and TRNG_ALARMSTOP registers.*/

  pRngRegs->TRNG_ALARMCNT  = (alarm_thr   << CSL_CP_ACE_TRNG_ALARMCNT_ALARM_THRESHOLD_SHIFT);

  pRngRegs->TRNG_CONFIG = (sample_cycles  << CSL_CP_ACE_TRNG_CONFIG_SAMPLE_CYCLES_SHIFT) |
                          (clock_divider  << CSL_CP_ACE_TRNG_CONFIG_SAMPLE_DIV_SHIFT);

  /* 3. Enable all FROs in the TRNG_FROENABLE register 
   * (note that this can only be done after clearing the TRNG_ALARMSTOP register). */
  /* Initialize FRO de-tune for all the configured FRO's,
   * set the default value,
   * this must be done before the FRO's are enabled */
  pRngRegs->TRNG_FRODETUNE = (uint32_t) 0;
  /* Clear alarm mask */
  pRngRegs->TRNG_ALARMMASK = 0;
  /* Clear alarm stop */
  pRngRegs->TRNG_ALARMSTOP = 0;
  /* Enable the configured FRO's */
  pRngRegs->TRNG_FROENABLE = (uint32_t) 0x00FFFFFFU;

  /* 4. Start the actual engine by setting the 'enable_trng' and 'drbg_en' bits 
   * in the TRNG_CONTROL register (also a nice point to set the interrupt mask bits).*/
  bitmask =  CSL_CP_ACE_TRNG_CONTROL_ENABLE_TRNG_MASK;

  if (pCfg->ctrlBitfield & sa_RNG2_CTRL_DRBG_USE)
  {
      /* set drbg_en bit */
      pRngRegs->TRNG_CONTROL |= CSL_CP_ACE_TRNG_CONTROL_DRBG_EN_MASK;
  }

  /* Clear all pending events */
  pRngRegs->TRNG_STATUS = 0x63FF;

  if (pCfg->ctrlBitfield & sa_RNG2_CTRL_DRBG_KNOWN_TESTS)
  {
      /* set drbg_en bit */
      pRngRegs->TRNG_CONTROL |= CSL_CP_ACE_TRNG_CONTROL_DRBG_EN_MASK;
  }

  pRngRegs->TRNG_CONTROL |= bitmask;
  /* Now the engine is ready to handle the first Generate request using the 
   * request data bit of the TRNG_CONTROL register for DRBG Use case
   */
  if (pCfg->ctrlBitfield & sa_RNG2_CTRL_DRBG_USE)
  {
      /*5. Wait until the 'reseed_ai' bit in the TRNG_STATUS register indicates a '1' */
      do
      {
        /* dummy operations */
        proceed = 1;
      } while ((pRngRegs->TRNG_STATUS & CSL_CP_ACE_TRNG_STATUS_RESEED_AI_MASK) == 0);
  
      /* 5. Mandatory: Write a 384 bits (48 Bytes) concatenation of a 'Personalization String' and
       *   'Nonce' in the TRNG_PS_AI_... registers. The first Byte of the 'Nonce' must be written to
       *    bits [31:24] of TRNG_PS_AI_0 while the last Byte of the 'Personalization String'
       *    must be written to bits [7:0] of TRNG_PS_AI_11. Using a 'Personalization String' is not
       *    a hard requirement; if it is not used, then the 'Nonce' must be 48 Bytes long to
       *    fill these registers. All of the 48 Bytes must be used as they are fed through the
       *    BC_DF function (and contribute to the 'L' length value used in that algorithm)
       */
       pRngRegs->TRNG_PS_AI_0 = pCfg->pStringData[0];
       pRngRegs->TRNG_PS_AI_1 = pCfg->pStringData[1];
       pRngRegs->TRNG_PS_AI_2 = pCfg->pStringData[2];
       pRngRegs->TRNG_PS_AI_3 = pCfg->pStringData[3];
       pRngRegs->TRNG_PS_AI_4 = pCfg->pStringData[4];
       pRngRegs->TRNG_PS_AI_5 = pCfg->pStringData[5];
       pRngRegs->TRNG_PS_AI_6 = pCfg->pStringData[6];
       pRngRegs->TRNG_PS_AI_7 = pCfg->pStringData[7];
       pRngRegs->TRNG_PS_AI_8 = pCfg->pStringData[8];
       pRngRegs->TRNG_PS_AI_9 = pCfg->pStringData[9];
       pRngRegs->TRNG_PS_AI_10 = pCfg->pStringData[10];
       pRngRegs->TRNG_PS_AI_11 = pCfg->pStringData[11];

      /* Set the number of 128 bit data blocks that must be generated by the 
       * [SP 800-90A] AES-256 DRBG in a single generate function */
      bitmask  = (128 << CSL_CP_ACE_TRNG_CONTROL_DATA_BLOCKS_SHIFT) | 
                 (1   << CSL_CP_ACE_TRNG_CONTROL_REQUEST_DATA_SHIFT);
      pRngRegs->TRNG_CONTROL |= bitmask;
  }

  if (pCfg->ctrlBitfield & sa_RNG2_CTRL_DRBG_KNOWN_TESTS)
  {
      /* Read the TRNG_CONTROL to make sure drbg_en is set */
      bitmask = (pRngRegs->TRNG_CONTROL & CSL_CP_ACE_TRNG_CONTROL_DRBG_EN_MASK);
      /* If DRBG is not enabled */
      if (bitmask == 0U)
      {
          /* Writeback the instance updates */
          Sa_osalEndMemAccess(inst, sizeof(salldObj_t));

          Sa_osalMtCsExit(mtKey);
          return (sa_ERR_GEN);
      }

      /* Set the test_aes_256 bit in the TRNG_TEST register to 1 */
      pRngRegs->TRNG_TEST |= (1 << CSL_CP_ACE_TRNG_TEST_TEST_AES_256_SHIFT);

      /* Allow hardware to make ready bit to 0 and test_ready to be set */
      for (i=0; i<100;)
      {
          i++;
      }

      /* make sure 'ready' bit is clear and 'test_ready' bit is set in TRNG_STATUS */
      proceed = 0U;
      if ( (pRngRegs->TRNG_STATUS & CSL_CP_ACE_TRNG_STATUS_READY_MASK) == 0U)
      {
         /* Check 'test_ready' */
         if ( (pRngRegs->TRNG_STATUS & CSL_CP_ACE_TRNG_STATUS_TEST_READY_MASK) != 0U)
         {
            proceed = 1U;
         }
      }
      /* Error check */
      if (proceed == 0U)
      {
          /* Writeback the instance updates */
          Sa_osalEndMemAccess(inst, sizeof(salldObj_t));

          Sa_osalMtCsExit(mtKey);
          return (sa_ERR_GEN);
      }

      /* Write the 256 bits to key0 through key 7 */
      pRngRegs->TRNG_PS_AI_0 = SA_RNG2_KNOWN_ANSWER_KEYS_0;
      pRngRegs->TRNG_PS_AI_1 = SA_RNG2_KNOWN_ANSWER_KEYS_1;
      pRngRegs->TRNG_PS_AI_2 = SA_RNG2_KNOWN_ANSWER_KEYS_2;
      pRngRegs->TRNG_PS_AI_3 = SA_RNG2_KNOWN_ANSWER_KEYS_3;
      pRngRegs->TRNG_PS_AI_4 = SA_RNG2_KNOWN_ANSWER_KEYS_4;
      pRngRegs->TRNG_PS_AI_5 = SA_RNG2_KNOWN_ANSWER_KEYS_5;
      pRngRegs->TRNG_PS_AI_6 = SA_RNG2_KNOWN_ANSWER_KEYS_6;
      pRngRegs->TRNG_PS_AI_7 = SA_RNG2_KNOWN_ANSWER_KEYS_7;

      /* Write the TRNG_INPUT_0 … TRNG_INPUT_3 registers with
         the 128 bits test value to encrypt.
         Upon writing the (highest Byte of the) TRNG_INPUT_3 register,
         the 'test_ready' bit in the TRNG_STATUS register will 
         drop to '0' to indicate processing has started
      */
      pRngRegs->TRNG_INPUT_0 = SA_RNG2_KNOWN_ANSWER_TRNG_INPUT_0;
      pRngRegs->TRNG_INPUT_1 = SA_RNG2_KNOWN_ANSWER_TRNG_INPUT_1;
      pRngRegs->TRNG_INPUT_2 = SA_RNG2_KNOWN_ANSWER_TRNG_INPUT_2;
      pRngRegs->TRNG_INPUT_3 = SA_RNG2_KNOWN_ANSWER_TRNG_INPUT_3; /* (starts processing) */

      /* Allow hardware to make test_ready bit to drop to 0  */
      for (i=0; i<100;)
      {
          i++;
      }

      /* wait until test_ready the tranisition from 0 to 1 */
      proceed = 1;
      i       = 0;
      do 
      {
           i++;
           if (i == UINT32_MAX)
           {
               proceed = 0;
               break;
           }
      } while ((pRngRegs->TRNG_STATUS & CSL_CP_ACE_TRNG_STATUS_TEST_READY_MASK) == 0U);

      /* Output is ready verify the result */
      if (proceed == 1)
      {
          proceed = 0;
          if (
               (pRngRegs->TRNG_INPUT_0 == SA_RNG2_KNOWN_ANSWER_TRNG_OUTPUT_0) &&
               (pRngRegs->TRNG_INPUT_1 == SA_RNG2_KNOWN_ANSWER_TRNG_OUTPUT_1) &&
               (pRngRegs->TRNG_INPUT_2 == SA_RNG2_KNOWN_ANSWER_TRNG_OUTPUT_2) &&
               (pRngRegs->TRNG_INPUT_3 == SA_RNG2_KNOWN_ANSWER_TRNG_OUTPUT_3)
              )
          {
              proceed = 1;
          }
      }

      if (proceed == 0)
      {
          /* Writeback the instance updates */
          Sa_osalEndMemAccess(inst, sizeof(salldObj_t));
          Sa_osalMtCsExit(mtKey);
          return (sa_ERR_GEN);
      }
  }


  SALLD_SET_STATE_RNG(inst, 1);

  /* Writeback the instance updates */
  Sa_osalEndMemAccess(inst, sizeof(salldObj_t));

  Sa_osalMtCsExit(mtKey);
  return (sa_ERR_OK);
#endif
}

/******************************************************************************
 * FUNCTION PURPOSE:  SALLD Get a 64-bit true random number
 ******************************************************************************
 * DESCRIPTION: This function is called to get a 64-bit true random number.
 *              It is a non-blocking function call with return value indicates
 *              whetehr the random number is available
 *
 *  int16_t  Sa_getRandonNum (
 *   Sa_Handle        handle      - SALLD instance handle
 *   Bool             f_isr       - Flag to indicate whether it is called from ISR
 *   Sa_RngData_t*    rnd)        - a pointer to 64-bit random number
 *
 * Return values:  sa_ERR_OK
 *                 sa_ERR_PARAMS
 *                 sa_ERR_MOUDLE_BUSY
 *                 sa_ERR_MODULE_UNAVAIL
 *
 ******************************************************************************/
int16_t Sa_getRandomNum (Sa_Handle handle, uint16_t f_isr, Sa_RngData_t* rnd)
{
#if defined (NSS_LITE2)
  int16_t  retCode = sa_ERR_API_UNSUPPORTED;
#else
  int16_t  retCode = sa_ERR_OK;

  salldObj_t *inst = (salldObj_t *)sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, handle);
  CSL_Cp_aceRegs*  pSaRegs = (CSL_Cp_aceRegs*)salldLObj.baseAddr;
  CSL_Cp_aceTrngRegs* pRngRegs = &pSaRegs->TRNG;
  uint32_t mtKey;

  if (!rnd)
  {
    return(sa_ERR_PARAMS);
  }

  if (inst->magic != SALLD_INST_MAGIC_WORD)
  {

    if(inst->magic == salld_swiz32(SALLD_INST_MAGIC_WORD))
        return(sa_ERR_INV_ENDIAN_MODE);
    else
        return(sa_ERR_INV_HANDLE);
  }

  Sa_osalMtCsEnter(&mtKey);

  if(!SALLD_TEST_STATE_RNG(inst))
  {
    retCode = sa_ERR_MODULE_UNAVAIL;
  }

  else if (!(pRngRegs->TRNG_STATUS & CSL_CP_ACE_TRNG_STATUS_READY_MASK))
  {
    retCode = sa_ERR_MODULE_BUSY;
  }
  else
  {
    rnd->hi = pRngRegs->TRNG_OUTPUT_H;   /* read randon number */
    rnd->lo = pRngRegs->TRNG_OUTPUT_L;
    pRngRegs->TRNG_INTACK = CSL_CP_ACE_TRNG_INTACK_READY_MASK; /* interrupt acknowledge */
  }
  Sa_osalMtCsExit(mtKey);
#endif

  return(retCode);
}

/******************************************************************************
 * FUNCTION PURPOSE:  SALLD Get a 128-bit true random number
 ******************************************************************************
 * DESCRIPTION: This function is called to get a 64-bit true random number.
 *              It is a non-blocking function call with return value indicates
 *              whetehr the random number is available
 *
 *  int16_t  Sa_getRandonNum (
 *   Sa_Handle        handle      - SALLD instance handle
 *   Bool             f_isr       - Flag to indicate whether it is called from ISR
 *   Sa_Rng2Data_t*    rnd)        - a pointer to 128-bit random number
 *
 * Return values:  sa_ERR_OK
 *                 sa_ERR_PARAMS
 *                 sa_ERR_MOUDLE_BUSY
 *                 sa_ERR_MODULE_UNAVAIL
 *
 ******************************************************************************/
int16_t Sa_getRandomNum2 (Sa_Handle handle, uint16_t f_isr, Sa_Rng2Data_t* rnd)
{
#if !defined (NSS_LITE2)
  int16_t  retCode = sa_ERR_API_UNSUPPORTED;
#else
  int16_t  retCode = sa_ERR_OK;

  salldObj_t *inst = (salldObj_t *)sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, handle);
  CSL_Cp_aceRegs*  pSaRegs = (CSL_Cp_aceRegs*)salldLObj.baseAddr;
  CSL_Cp_aceTrngRegs* pRngRegs = &pSaRegs->TRNG;
  uint32_t mtKey;

  if (!rnd)
  {
    return(sa_ERR_PARAMS);
  }

  if (inst->magic != SALLD_INST_MAGIC_WORD)
  {

    if(inst->magic == salld_swiz32(SALLD_INST_MAGIC_WORD))
        return(sa_ERR_INV_ENDIAN_MODE);
    else
        return(sa_ERR_INV_HANDLE);
  }

  Sa_osalMtCsEnter(&mtKey);

  if(!SALLD_TEST_STATE_RNG(inst))
  {
    retCode = sa_ERR_MODULE_UNAVAIL;
  }

  else if (!(pRngRegs->TRNG_STATUS & CSL_CP_ACE_TRNG_STATUS_READY_MASK))
  {
    retCode = sa_ERR_MODULE_BUSY;
  }
  else
  {
    rnd->hihi = pRngRegs->TRNG_INPUT_3;   /* read randon number */
    rnd->hilo = pRngRegs->TRNG_INPUT_2;
    rnd->hi   = pRngRegs->TRNG_INPUT_1;
    rnd->lo   = pRngRegs->TRNG_INPUT_0;
    pRngRegs->TRNG_STATUS = CSL_CP_ACE_TRNG_INTACK_READY_ACK_MASK; /* interrupt acknowledge */
  }
  Sa_osalMtCsExit(mtKey);
#endif

  return(retCode);
}


/******************************************************************************
 * FUNCTION PURPOSE:  Deactivate the TRNG module (optional)
 ******************************************************************************
 * DESCRIPTION: This function deactivates the TRNG module and clears its
 *              corresponding LLD internal state
 *
 *  int16_t  Sa_rngClose  (
 *              Sa_Handle handle)      - SALLD instance
 *
 * Return values:  sa_ERR_OK
 *
 ******************************************************************************/
int16_t  Sa_rngClose  (Sa_Handle handle)
{
  salldObj_t *inst = (salldObj_t *)sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, handle);
  CSL_Cp_aceRegs*  pSaRegs = (CSL_Cp_aceRegs*)salldLObj.baseAddr;
  CSL_Cp_aceTrngRegs* pRngRegs = &pSaRegs->TRNG;
  uint32_t mtKey;

  Sa_osalMtCsEnter(&mtKey);

  /* Invalidate the Cache Contents */
  Sa_osalBeginMemAccess(inst, sizeof(salldObj_t));

  if (inst->magic != SALLD_INST_MAGIC_WORD)
  {

    Sa_osalMtCsExit(mtKey);

    if(inst->magic == salld_swiz32(SALLD_INST_MAGIC_WORD))
        return(sa_ERR_INV_ENDIAN_MODE);
    else
        return(sa_ERR_INV_HANDLE);
  }

  if (SALLD_TEST_STATE_RNG(inst))
  {

    pRngRegs->TRNG_CONTROL = 0; /* Disable RNG */

    SALLD_SET_STATE_RNG(inst, 0);

    /* Writeback the instance updates */
    Sa_osalEndMemAccess(inst, sizeof(salldObj_t));

    /* Disable the RNG module */
#ifdef NSS_LITE2
    /* Only disable engines when full access is available */
    if (!SALLD_TEST_LIMIT_ACCESS(inst))
    {
        pSaRegs->UPDATES.ENGINE_ENABLE &= ~CSL_CP_ACE_CMD_STATUS_TRNG_EN_MASK;
    }
#else
    pSaRegs->MMR.CMD_STATUS &= ~CSL_CP_ACE_CMD_STATUS_TRNG_EN_MASK;
#endif    
  }
  
  Sa_osalMtCsExit(mtKey);
  return(sa_ERR_OK);  

} /* Sa_rngClose */

/* RNG2 Close */
#if defined(NSS_LITE2)
int16_t  Sa_rng2Close  (Sa_Handle handle)
{
    CSL_Cp_aceRegs*     pSaRegs = (CSL_Cp_aceRegs*)salldLObj.baseAddr;
    CSL_Cp_aceTrngRegs* pRngRegs = &pSaRegs->TRNG;
    uint32_t            bitmask;

    /* Disable the DRBG if it is already enabled */
    /* Read the TRNG_CONTROL to make sure drbg_en is set */
    bitmask = (pRngRegs->TRNG_CONTROL & CSL_CP_ACE_TRNG_CONTROL_DRBG_EN_MASK);
    /* If DRBG enabled */
    if (bitmask == 1U)
    {
        /* Clear enable_trng bit */
        pRngRegs->TRNG_CONTROL &= ~CSL_CP_ACE_TRNG_CONTROL_ENABLE_TRNG_MASK;
        pRngRegs->TRNG_CONTROL &= ~CSL_CP_ACE_TRNG_CONTROL_DRBG_EN_MASK;
    }
   return (Sa_rngClose(handle));
}
#else
int16_t  Sa_rng2Close  (Sa_Handle handle)
{
   return(sa_ERR_API_UNSUPPORTED);
}
#endif

/******************************************************************************
 * FUNCTION PURPOSE:  Obtain the SA system ID
 ******************************************************************************
 * DESCRIPTION: This function returns the SA system ID associated with the
 *      specified handle.
 *         SALLD instance.
 *
 *****************************************************************************/
uint32_t  Sa_getID (Sa_Handle handle)
{
  salldObj_t *inst = (salldObj_t *)sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, handle);

  if (inst->magic != SALLD_INST_MAGIC_WORD)
  {
    return(0);
  }
  else
  {
    return (inst->ID);
  }
}

/******************************************************************************
 * FUNCTION PURPOSE:  Obtain the SA shadow system handle
 ******************************************************************************
 * DESCRIPTION: This function returns the shaddow system handle. The shaddow system
 *           handle is set to NULL if it does not exist.
 *
 *****************************************************************************/
int16_t  Sa_getShadowHandle  (Sa_Handle handle, Sa_Handle *shandle)
{
  salldObj_t *inst = (salldObj_t *)sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, handle);

  if (inst->magic != SALLD_INST_MAGIC_WORD)
  {

    if(inst->magic == salld_swiz32(SALLD_INST_MAGIC_WORD))
        return(sa_ERR_INV_ENDIAN_MODE);
    else
        return(sa_ERR_INV_HANDLE);
  }

  *shandle = (Sa_Handle)(uintptr_t)inst->shadowInstOffset;

  return (sa_ERR_OK);

}

#if !defined(NSS_LITE) && !defined(NSS_LITE2)
/****************************************************************************************
 * FUNCTION PURPOSE: Enable a PDSP
 ****************************************************************************************
 * DESCRIPTION:  The PDSP is put into the run state if it is currently in the reset state.
 *               If taken out of reset the function waits for the PDSP to indicate 
 *               that it is ready. 
 *
 *               Return value:
 *                  SALLD_PDSP_ALREADY_ACTIVE - the PDSP was already out of reset
 *                  SALLD_PDSP_RESET_RELEASED - the PDSP was in reset and successfully taken out of reset
 *                  SALLD_PDSP_NO_RESTART - the PDSP was taken out of reset but timed out on the handshake
 ****************************************************************************************/

#define SALLD_PDSP_ALREADY_ACTIVE      0
#define SALLD_PDSP_RESET_RELEASED      1
#define SALLD_PDSP_NO_RESTART          2

static int salld_pdsp_run (CSL_Cp_acePdsp_control_statusRegs *pRegs)
{
  /* Check for enabled PDSP */
  if ( (pRegs->PDSP_CONTROL & CSL_CP_ACE_PDSP_CONTROL_ENABLE_MASK) == CSL_CP_ACE_PDSP_CONTROL_ENABLE_MASK)
    return (SALLD_PDSP_ALREADY_ACTIVE);

  /* Enable the PDSP */
  pRegs->PDSP_CONTROL |= CSL_CP_ACE_PDSP_CONTROL_ENABLE_MASK;

  return (SALLD_PDSP_RESET_RELEASED);

} /* salld_pdsp_run */
#endif

/*****************************************************************************************
 * FUNCTION PURPOSE: Set the state of the Sub-System
 *****************************************************************************************
 * DESCRIPTION: The Sub-System state can be set or queried.
 *****************************************************************************************/
Sa_State_t Sa_resetControl (Sa_Handle handle, Sa_State_t newState)
{

#if !defined(NSS_LITE) && !defined(NSS_LITE2)
  salldObj_t *inst = (salldObj_t *)sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, handle);
  CSL_Cp_aceRegs*  pSaRegs = (CSL_Cp_aceRegs*)salldLObj.baseAddr;
  Sa_State_t  ret           = sa_STATE_ENABLE;
  int numPdsps;
  int i;
  uint32_t mtKey;

  if (inst->magic != SALLD_INST_MAGIC_WORD)
  {
    return (sa_STATE_INVALID_REQUEST);
  }

  Sa_osalMtCsEnter(&mtKey);

  numPdsps = SALLD_TEST_SASS_GEN2(inst)?3:2;


  if (newState == sa_STATE_RESET)  {

    /* Disable and reset PDSPs */
    for (i = 0; i < numPdsps; i++)  {
      pSaRegs->PDSP_CONTROL_STATUS[i].PDSP_CONTROL =  0;
    }

    Sa_osalMtCsExit(mtKey);
    return (sa_STATE_RESET);

  }

  if (newState == sa_STATE_ENABLE)  {
    int         res;

    /* Do nothing if a pdsp is already out of reset. If any PDSPs are out of reset
     * a global init is not performed */
    for (i = 0; i < numPdsps; i++)  {

      res = salld_pdsp_run (&pSaRegs->PDSP_CONTROL_STATUS[i]);

      if (res == SALLD_PDSP_NO_RESTART)  {
        ret = sa_STATE_ENABLE_FAILED;
      }
    }

    /* Check if we need any debug info collect enabled, if yes, set the configurations */
    res  = 0;
    res |= SALLD_TEST_STATE_DBG_L1_COLLECT(inst);
    res |= SALLD_TEST_STATE_DBG_L2_COLLECT(inst);

    /* Non zero value indicates to enable the debug info in PDSPs */
    if (res)  {
      /* wait for a micro second to make sure firmware init sequence is complete */
      for (i = 0; i < 1000; i++)
        asm (" nop ");

      sa_trigger_halt_on_err(inst, numPdsps);
    }


    Sa_osalMtCsExit(mtKey);
    return (ret);

  }

  if (newState == sa_STATE_QUERY)  {

    uint16_t enable  = FALSE;
    uint16_t reset   = FALSE;

    for (i = 0; i < numPdsps; i++)  {
      if ( (pSaRegs->PDSP_CONTROL_STATUS[i].PDSP_CONTROL & CSL_CP_ACE_PDSP_CONTROL_ENABLE_MASK) ==
                     CSL_CP_ACE_PDSP_CONTROL_ENABLE_MASK )
        enable = TRUE;
      else
        reset = TRUE;
    }

    if ((reset == TRUE) && (enable == TRUE))
      ret = sa_STATE_INCONSISTENT;
    else if (reset == TRUE)
       ret = sa_STATE_RESET;
    else
       ret = sa_STATE_ENABLE;

    Sa_osalMtCsExit(mtKey);
    return (ret);

  }

  Sa_osalMtCsExit(mtKey);
  return (sa_STATE_INVALID_REQUEST);
  
#else
  return (sa_ERR_API_UNSUPPORTED);
#endif

} /* Sa_resetControl */

/***********************************************************************************************
 * FUNCTION PURPOSE: Download a PDSP image
 ***********************************************************************************************
 * DESCRIPTION: A PDSP image is downloaded. The PDSP remains in reset.
 ***********************************************************************************************/
int16_t Sa_downloadImage (Sa_Handle handle, int modId, void* image, int sizeBytes)
{

#if !defined(NSS_LITE) && !defined(NSS_LITE2)
  salldObj_t *inst = (salldObj_t *)sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, handle);
  CSL_Cp_aceRegs*  pSaRegs = (CSL_Cp_aceRegs*)salldLObj.baseAddr;
  int numPdsps = SALLD_TEST_SASS_GEN2(inst)?3:2;
  volatile uint32_t *src;
  uint32_t mtKey;
  volatile uint32_t *iram;
  uint32_t i;

  /* Verify the specified PDSP is valid */
  if ((modId < 0)  || (modId >= numPdsps))
    return (sa_ERR_PARAMS);

  /* Verify the image size is in range */
  if ((sizeBytes < 0)  || (sizeBytes >= 0x4000))
    return (sa_ERR_PARAMS);

  if (inst->magic != SALLD_INST_MAGIC_WORD)
  {

    if(inst->magic == salld_swiz32(SALLD_INST_MAGIC_WORD))
        return(sa_ERR_INV_ENDIAN_MODE);
    else
        return(sa_ERR_INV_HANDLE);
  }

  /* set IRAM location */
  if (modId == 0)
  {
    iram = (volatile uint32_t *)pSaRegs->PDSP0_IRAM;
  }
  else if (modId == 1)
  {
    iram = (volatile uint32_t *)pSaRegs->PDSP1_IRAM;
  }
  else
  {
    iram = (volatile uint32_t *)pSaRegs->PDSP2_IRAM;
  }

  Sa_osalMtCsEnter(&mtKey);

  /* Make sure the PDSP is disabled */
  if ( (pSaRegs->PDSP_CONTROL_STATUS[modId].PDSP_CONTROL & CSL_CP_ACE_PDSP_CONTROL_ENABLE_MASK)  ==
                CSL_CP_ACE_PDSP_CONTROL_ENABLE_MASK)
  {
    Sa_osalMtCsExit(mtKey);
    return (sa_STATE_INVALID);
  }
  /* Copy the image */
  src = (uint32_t *) image;
  for ( i = 0; i < (sizeBytes/4); i++)
  {
      iram[i] = src[i];
  }
  Sa_osalMtCsExit(mtKey);
  return (sa_ERR_OK);
#else
  return (sa_ERR_API_UNSUPPORTED);
#endif  

} /* Sa_downloadImage */

/***********************************************************************************************
 * FUNCTION PURPOSE: Inquire PDSP Version Number
 ***********************************************************************************************
 * DESCRIPTION: Extract the SA PDSP  version number.
 ***********************************************************************************************/
int16_t Sa_getPDSPVersion (Sa_Handle handle, int modId, uint32_t *pVersion)
{
#if !defined(NSS_LITE) && !defined(NSS_LITE2)
  salldObj_t *inst = (salldObj_t *)sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, handle);
  CSL_Cp_aceRegs*  pSaRegs = (CSL_Cp_aceRegs*)salldLObj.baseAddr;
  int numPdsps, phpSramSize;

  if (SALLD_TEST_SASS_GEN2(inst))
  {
    numPdsps = 3;
    phpSramSize = SA_PHP_SRAM_SIZE_IN_UINT32;
  }
  else
  {
    numPdsps = 2;
    phpSramSize = 2*SA_PHP_SRAM_SIZE_IN_UINT32;
  }

  /* Verify the specified PDSP is valid */
  if ((modId < 0)  || (modId >= numPdsps))
    return (sa_ERR_PARAMS);

  if (inst->magic != SALLD_INST_MAGIC_WORD)
  {
    if(inst->magic == salld_swiz32(SALLD_INST_MAGIC_WORD))
        return(sa_ERR_INV_ENDIAN_MODE);
    else
        return(sa_ERR_INV_HANDLE);
  }

  *pVersion = pSaRegs->SRAM0[SA_PHP_VERSION_OFFSET + modId*phpSramSize];

  return (sa_ERR_OK);
  
#else
  return (sa_ERR_API_UNSUPPORTED);
#endif

} /* Sa_getPDSPVersion */


/***********************************************************************************************
 * FUNCTION PURPOSE: Inquire Version Number
 ***********************************************************************************************
 * DESCRIPTION: Return the SA LLD version number.
 ***********************************************************************************************/
uint32_t Sa_getVersion (void)
{
    return SA_LLD_VERSION_ID;
}

/***********************************************************************************************
 * FUNCTION PURPOSE: Inquire Version String
 ***********************************************************************************************
 * DESCRIPTION: Return the SA LLD version string.
 ***********************************************************************************************/
const char* Sa_getVersionStr (void)
{
    return sa_lld_version_str;
}

/***********************************************************************************************
 * FUNCTION PURPOSE: Inquire Version String
 ***********************************************************************************************
 * DESCRIPTION: Return the SA LLD version string.
 ***********************************************************************************************/
int16_t Sa_coreDump(Sa_Handle handle, uint32_t *dbgBuf, uint16_t bufSz)
{
#if !defined(NSS_LITE) && !defined(NSS_LITE2)
  salldObj_t *inst = (salldObj_t *)(sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, handle));
  CSL_Cp_aceRegs*  pSaRegs = (CSL_Cp_aceRegs*)salldLObj.baseAddr;
  uint32_t mtCsKey;
  salldCoreDumpInfo_t *dbgInfo;
  int16_t ret = sa_ERR_OK;
  int     modId, numPdsps;
  uint32_t  *src, *dst;
  uint32_t   i;

  /* Zero overhead check that Debug SIZE published is consistent */
  SA_COMPILE_TIME_SIZE_CHECK (sa_CORE_DUMP_BUF_SIZE >= (sizeof(salldCoreDumpInfo_t) >> 2));

  /* Test instance pointer */
  if (dbgBuf == NULL )
    return(sa_ERR_PARAMS);

  /* CRITICAL Section Start: The global instance is a shared resource which needs to be
   * protected from the following:
   *  a) Multiple Cores acccess
   */
  Sa_osalMtCsEnter(&mtCsKey);

  dbgInfo = (salldCoreDumpInfo_t *) dbgBuf;

  dbgInfo->debug_level = 0;

  if (bufSz < sa_CORE_DUMP_BUF_SIZE)
     return (sa_ERR_PARAMS);

  /* Check the buffer size passed and also
     record the debug info levels */
  if (SALLD_TEST_STATE_DBG_L1_COLLECT(inst)) {
    dbgInfo->debug_level = 1;
  }

  if (SALLD_TEST_STATE_DBG_L2_COLLECT(inst) ){
    dbgInfo->debug_level = 2;
  }

  /* record number of PDSPs information */
  numPdsps = SALLD_TEST_SASS_GEN2(inst)?3:2;
  dbgInfo->numPdsps = numPdsps;

  /* Error check on handle using existing APIs */
  for ( modId = 0; modId < numPdsps; modId++ )
  {
    ret = Sa_getPDSPVersion(handle, modId, &dbgInfo->phpVer[modId]);

    if (ret != sa_ERR_OK)
      return(ret);
  }

  /* Collect the MMR dump */
  src = (uint32_t *) &pSaRegs->MMR.PID;
  dst = (uint32_t *) &dbgInfo->mmr[0];
  for ( i = 0 ; i < (SA_DBG_COLLECT_MMR_SIZE/4); i++)
  {
      dst[i] = src[i];
  }

  /* Collect the debug information per valid PDSPs */
  for ( modId = 0; modId < numPdsps; modId++)
  {
    sa_collect_debug_info(modId, dbgInfo, numPdsps == 3);
  }

  /* Snap shot the system statistics */
  ret = Sa_getSysStats(handle, &dbgInfo->sysStats);

  if (ret != sa_ERR_OK)
      return(ret);


  /* More specific debug log collection for Gen 2 Air Cipher case */
  if (dbgInfo->debug_level == 2)
  {
    memcpy ( (void *) &dbgInfo->dbgLevel2[0], (void *) &pSaRegs->SRAM0[SA_PKT_INFO_BUF_OFFSET], SA_DBG_COLLECT_PKT_INFO_SIZE);
  }

  /* Critical Section End */
  Sa_osalMtCsExit(mtCsKey);

  return(ret);
#else
    return (sa_ERR_API_UNSUPPORTED);
#endif  

}

/* Nothing past this point */

