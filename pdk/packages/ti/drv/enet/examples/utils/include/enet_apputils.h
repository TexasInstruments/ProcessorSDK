/*
 *  Copyright (c) Texas Instruments Incorporated 2020-2021
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

/*!
 * \file     enet_apputils.h
 *
 * \brief    This file contains the function prototypes of the Enet
 *           application utility functions used in the Enet examples.
 *
 * NOTE: This library is meant only for Enet examples. Customers are not
 * encouraged to use this layer as these are very specific to the examples
 * written and the API behaviour and signature can change at any time to
 * suit the examples.
 */

#ifndef ENET_APPUTILS_H_
#define ENET_APPUTILS_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/drv/enet/enet.h>
#include <ti/drv/udma/udma.h>
#include <ti/drv/enet/include/core/enet_rm.h>
#include <ti/drv/enet/include/mod/cpsw_stats.h>
#include <ti/drv/enet/include/mod/cpsw_macport.h>
#include <ti/drv/enet/include/core/enet_dma.h>

#include "enet_ethutils.h"
#include "enet_ioctlutils.h"
#include "enet_apputils_k3.h"
#include "enet_udmautils.h"

#if defined(__KLOCWORK__)
#include <stdlib.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/** \brief Size of an array */
#define ENETAPPUTILS_ROUND_UP(x, y)     ((((x) + ((y) - 1U)) / (y)) * (y))
#define ENETAPPUTILS_ALIGN(x)           (ENETAPPUTILS_ROUND_UP((x), 128U))

/* \brief Octets of payload per console row */
#define OCTETS_PER_ROW                  (16U)
/** \brief Max frame length */
#define ETH_MAX_FRAME_LEN               (1522U)

/* \brief Support macros for MMR lock/unlock functions */
#define MMR_KICK0_UNLOCK_VAL            (0x68EF3490U)
#define MMR_KICK1_UNLOCK_VAL            (0xD172BC5AU)
#define MMR_KICK_LOCK_VAL               (0x00000000U)

#define CSL_MMR_KICK_UNLOCKED_MASK          (0x00000001U)
#define CSL_MMR_KICK_UNLOCKED_SHIFT         (0x0U)

#define ENET_CTRL_RGMII_ID_SHIFT            (4U)
#define ENET_CTRL_RGMII_ID_INTTXDLY         (0U)
#define ENET_CTRL_RGMII_ID_NODELAY          (1U)

#if defined(SOC_J7200)
#define ENET_UTILS_MCU2_0_UART_INSTANCE     (3U)
#elif defined(SOC_J721S2)
#define ENET_UTILS_MCU2_0_UART_INSTANCE     (2U)
#else
#define ENET_UTILS_MCU2_0_UART_INSTANCE     (2U)
#endif

#if defined(__KLOCWORK__)
#define EnetAppUtils_assert(cond)       do { if (!(cond)) abort(); } while (0)
#else
#define EnetAppUtils_assert(cond)                                     \
    (EnetAppUtils_assertLocal((bool) (cond), (const char *) # cond,   \
                    (const char *) __FILE__, (int32_t) __LINE__))
#endif

#define ENET_UTILS_CACHELINE_SIZE        (ENETDMA_CACHELINE_ALIGNMENT)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/**
 *  \brief  Lock/unlock enum
 */
typedef enum
{
    ENETAPPUTILS_LOCK_MMR = 0U,
    /**< lock control MMR */
    ENETAPPUTILS_UNLOCK_MMR,
    /**< unlock control MMR */
} EnetAppUtils_MmrLockState;

/**
 *  \brief CTRL MMR enum for MCU and Main domain
 */
typedef enum
{
    ENETAPPUTILS_MMR_LOCK0 = 0U,
    /**< control MMR lock 0*/
    ENETAPPUTILS_MMR_LOCK1,
    /**< control MMR lock 1*/
    ENETAPPUTILS_MMR_LOCK2,
    /**< control MMR lock 2*/
    ENETAPPUTILS_MMR_LOCK3,
    /**< control MMR lock 3*/
    ENETAPPUTILS_MMR_LOCK4,
    /**< control MMR lock 4*/
    ENETAPPUTILS_MMR_LOCK5,
    /**< control MMR lock 5*/
    ENETAPPUTILS_MMR_LOCK6,
    /**< control MMR lock 6*/
    ENETAPPUTILS_MMR_LOCK7,
    /**< control MMR lock 7*/
} EnetAppUtils_CtrlMmrType;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/*!
 * \brief Perform cache writeback operation
 *
 * Performs cache write-back operation based on whether cache is coherent
 * or not. This internally uses the OSAL API.
 *
 * \param addr  Start address of the cache line/s
 * \param size  Size (in bytes) of the memory to be written back
 *
 */
void EnetAppUtils_cacheWb(const void *addr,
                          int32_t size);

/*!
 * \brief Perform cache invalidate operation
 *
 * Perform cache invalidate operation based on whether cache is
 * coherent or not. This internally uses the OSAL API.
 *
 * \param addr  Start address of the cache line/s
 * \param size  Size (in bytes) of the memory to invalidate
 *
 */
void EnetAppUtils_cacheInv(const void *addr,
                           int32_t size);

/*!
 * \brief Perform cache writeback and invalidate operation
 *
 * Performs cache write-back and invalidation operation based on whether
 * cache is coherent or not. This internally uses the OSAL API.
 *
 * \param addr  Start address of the cache line/s
 * \param size  Size (in bytes) of the memory to writeback and invalidate
 */
void EnetAppUtils_cacheWbInv(const void *addr,
                             int32_t size);

/*!
 * \brief Prints the va_list on UART or console.
 *
 * Print the passed va_list on UART or console.  This API is required by apps
 * implementing functions which originally take a variadic argument, hence
 * the body of such functions require helper APIs that takes va_list.
 */
void EnetAppUtils_vprint(const char *pcString,
                         va_list args);

/*!
 * \brief Prints the string on UART or console
 */
void EnetAppUtils_print(const char *pcString,
                        ...);

/*!
 * \brief Input utility used to accept a character from terminal
 */
char EnetAppUtils_getChar(void);

/*!
 * \brief Input utility used to accept a character from terminal using interrupt mode
 */
char EnetAppUtils_getChar2(void);

/*!
 * \brief Input utility used to accept a number from terminal
 */
int32_t EnetAppUtils_getNum(void);

/*!
 * \brief Input utility used to accept a hexa decimal number from terminal
 */
uint32_t EnetAppUtils_getHex(void);

/*!
 * \brief Rand number function
 */
uint32_t EnetAppUtils_randFxn(uint32_t min,
                              uint32_t max);

/*!
 * \brief Wait for emulator connection
 */
void EnetAppUtils_waitEmuConnect(void);

/*!
 * \brief Wait function utility. This uses timer created in EnetAppUtils_timerInit
 *        function for busy wait for app passed wait time.
 */
void EnetAppUtils_wait(uint32_t waitTime);

/**
 *  \brief Returns if print can be supported for a platform/build
 *
 *  \return UTRUE if print can be supported for a platform. Else UFALSE
 */
uint32_t EnetAppUtils_isPrintSupported(void);

/**
 *  \brief Utils function for printing "non-zero" MAC port statistics for CPSW2G
 */
void EnetAppUtils_printMacPortStats2G(CpswStats_MacPort_2g *st);

/**
 *  \brief Utils function for printing "non-zero" Host port statistics for CPSW2G
 */
void EnetAppUtils_printHostPortStats2G(CpswStats_HostPort_2g *st);

/**
 *  \brief Utils function for printing "non-zero" MAC port statistics for CPSW9G
 */
void EnetAppUtils_printMacPortStats9G(CpswStats_MacPort_Ng *st);

/**
 *  \brief Utils function for printing "non-zero" Host port statistics for CPSW9G
 */
void EnetAppUtils_printHostPortStats9G(CpswStats_HostPort_Ng *st);

/**
 *  \brief Utils function for printing Ethernet frame with src MAC, dest MAC,
 *         VLAN type
 */
void EnetAppUtils_printFrame(EthFrame *frame,
                             uint32_t len);

/**
 *  \brief Utils function for printing Ethernet frame with src MAC, dest MAC,
 *         VLAN type that is in form of scatter-gather list
 */
void EnetAppUtils_printSGFrame(EnetDma_Pkt *pktInfo);

/**
 *  \brief Utils function for printing MAC address
 */
void EnetAppUtils_printMacAddr(uint8_t macAddr[]);

/**
 *  \brief Validates the state of all packets in the given
 *         queue with the expectedState and changes it to
 *         newState.
 */
void EnetAppUtils_validatePacketState(EnetDma_PktQ *pQueue,
                                      uint32_t expectedState,
                                      uint32_t newState);

/**
 *  \brief lock/unlock the MCU MMR CNTR register
 */
EnetAppUtils_MmrLockState EnetAppUtils_mcuMmrCtrl(EnetAppUtils_CtrlMmrType mmrNum,
                                                  EnetAppUtils_MmrLockState lock);

/**
 *  \brief Lock/unlock the Main MMR CNTR register
 */
EnetAppUtils_MmrLockState EnetAppUtils_mainMmrCtrl(EnetAppUtils_CtrlMmrType mmrNum,
                                                   EnetAppUtils_MmrLockState lock);

/**
 *  \brief Set loopback configuration for RGMII mode
 */
void EnetAppUtils_setNoPhyCfgRgmii(EnetMacPort_Interface *interface,
                                   EnetPhy_Cfg *phyCfg);

/**
 *  \brief Set loopback configuration for RMII mode
 */
void EnetAppUtils_setNoPhyCfgRmii(EnetMacPort_Interface *interface,
                                   EnetPhy_Cfg *phyCfg);

/**
 *  \brief Set loopback configuration for SGMII mode
 */
void EnetAppUtils_setNoPhyCfgSgmii(EnetMacPort_Interface *interface,
                                   CpswMacPort_Cfg *macCfg,
                                   EnetPhy_Cfg *phyCfg);

/**
 *  \brief Utils assert function
 */
#if !defined(__KLOCWORK__)
static inline void EnetAppUtils_assertLocal(bool condition,
                                            const char *str,
                                            const char *fileName,
                                            int32_t lineNum);
#endif

int32_t EnetAppUtils_showRxFlowStats(EnetDma_RxChHandle hRxFlow);

int32_t EnetAppUtils_showTxChStats(EnetDma_TxChHandle hTxCh);

int32_t EnetAppUtils_showRxChStats(EnetDma_RxChHandle hRxCh);

/**
 *  \brief Utility function to init resource config structure part of Cpsw_Cfg
 */
void EnetAppUtils_initResourceConfig(Enet_Type enetType,
                                     uint32_t instId,
                                     uint32_t selfCoreId,
                                     EnetRm_ResCfg *resCfg);

/*!
 * \brief Converts given character in Hexadecimal to Decimal value
 *
 * \param hex  Hexadecimal character to be converted
 * \return decimal value if a valid hexadecimal character
 * \return -1 if invalid hexadecimal character
 */
int8_t  EnetAppUtils_hex2Num(char hex);

/*!
 * \brief Converts given MAC address in string format to byte array
 *
 * \param txt  Pointer to MAC address string
 * \param addr Pointer to array of size 6 bytes to store octets of the MAC address
 *
 * \return ENET_SOK if input is a valid MAC address format
 *         ENET_EFAIL if input is invalid MAC address format
 */
int32_t EnetAppUtils_macAddrAtoI(const char *txt, uint8_t *addr);

/*!
 * \brief Converts given IP address in string format to byte array
 *
 * \param txt  Pointer to IP address string
 * \param addr Pointer to array of size 4 bytes to store octets of the IPv4 address
 *
 * \return ENET_SOK if input is a valid IPv4 address format
 *         ENET_EFAIL if input is invalid IPv4 address format
 */
int32_t EnetAppUtils_ipAddrAtoI(const char* txt, uint8_t *addr);

/*!
 * \brief Configure CPSW clocks and enable CPSW device via sciclient.
 *
 * Sets CPPI, 5/50/250-MHz RGMII reference clock, CPTS clock (MAIN_SYSCLK0),
 * and set device state to SW_STATE_ON.
 *
 * \param enetType  Enet Peripheral type
 * \param instId    Enet Peripheral instance id
 */
void EnetAppUtils_enableClocks(Enet_Type enetType, uint32_t instId);

/*!
 * \brief Disable CPSW clocks and disable CPSW device via sciclient.
 *
 * Disable CPPI, 5/50/250-MHz RGMII reference clock, CPTS clock (MAIN_SYSCLK0),
 * and set device state to SW_STATE_AUTO_OFF.
 *
 * \param enetType  Enet Peripheral type
 * \param instId    Enet Peripheral instance id
 */
void EnetAppUtils_disableClocks(Enet_Type enetType, uint32_t instId);

/*!
 * \brief Turn CPSW OFF via sciclient.
 *
 * Turn CPSW OFF via sciclient.
 */
void EnetAppUtils_turnCpswOff(void);

/*!
 * \brief Turn CPSW ON via sciclient.
 *
 * Turn CPSW ON via sciclient.
 */
void EnetAppUtils_turnCpswOn(void);

int32_t EnetAppUtils_showRxFlowStats(EnetDma_RxChHandle hRxFlow);

int32_t EnetAppUtils_showTxChStats(EnetDma_TxChHandle hTxCh);

/*!
 * \brief Delay in microseconds.
 *
 * \param delayInUsecs   Time in microseconds
 */
void EnetAppUtils_delayInUsec(uint32_t delayInUsecs);

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

#if !defined(__KLOCWORK__)
static inline void EnetAppUtils_assertLocal(bool condition,
                                            const char *str,
                                            const char *fileName,
                                            int32_t lineNum)
{
    volatile static bool gCpswAssertWaitInLoop = BTRUE;

    if (!(condition))
    {
        EnetAppUtils_print("Assertion @ Line: %d in %s: %s : failed !!!\n",
                           lineNum, fileName, str);
        while (gCpswAssertWaitInLoop)
        {
        }
    }

    return;
}
#endif

#ifdef __cplusplus
}
#endif

#endif  /* ENET_APPUTILS_H_ */
