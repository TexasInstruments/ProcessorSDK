/*
 *  Copyright (c) Texas Instruments Incorporated 2020-2024
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
 * \file     enet_apputils.c
 *
 * \brief    Common Enet application utility used in all Enet examples.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <ti/csl/soc.h>
#include <ti/csl/csl_cpswitch.h>

#include <ti/osal/osal.h>
#include <ti/board/board.h>

#include <ti/drv/enet/enet.h>
#include <ti/drv/enet/include/per/cpsw.h>

#include <ti/drv/uart/UART.h>
#include <ti/drv/uart/UART_stdio.h>
#include <ti/drv/pm/pmlib.h>

#if (defined(BUILD_MCU1_0) && (defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)))
#include <ti/drv/sciclient/sciserver_tirtos.h>
#endif

#include "include/enet_apputils.h"
#include "include/enet_board.h"
#include "include/enet_apprm.h"

#include <ti/osal/src/nonos/Nonos_config.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#if (defined(BUILD_MCU1_0) && (defined(SOC_J721E) || defined(SOC_J7200) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)))
/**< High Priority for SCI Server */
#define ENET_SETUP_SCISERVER_TASK_PRI_HIGH   (3 + 1)
/**< Low Priority for SCI Server */
#define ENET_SETUP_SCISERVER_TASK_PRI_LOW    (1)
#endif

#define __NOP  asm (" NOP ")

#define NOP5   do { __NOP; __NOP; __NOP; __NOP; __NOP; } while (0)

#define NOP10  NOP5; \
               NOP5

#define NOP50  NOP10; \
               NOP10; \
               NOP10; \
               NOP10; \
               NOP10

/* Second to microsecs conversion factor */
#define ENET_APPUTILS_SEC2MICROSEC           (1000000ULL)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void EnetAppUtils_cacheWb(const void *addr,
                          int32_t size)
{
    bool isCacheCoherent = Enet_isCacheCoherent();

    if (isCacheCoherent == BFALSE)
    {
        CacheP_wb(addr, size);
    }
}

void EnetAppUtils_cacheInv(const void *addr,
                           int32_t size)
{
    bool isCacheCoherent = Enet_isCacheCoherent();

    if (isCacheCoherent == BFALSE)
    {
        CacheP_Inv(addr, size);
    }
}

void EnetAppUtils_cacheWbInv(const void *addr,
                             int32_t size)
{
    bool isCacheCoherent = Enet_isCacheCoherent();

    if (isCacheCoherent == BFALSE)
    {
        CacheP_wbInv(addr, size);
    }
}

void EnetAppUtils_vprint(const char *pcString,
                         va_list args)
{
    char printBuffer[ENET_CFG_PRINT_BUF_LEN];

    vsnprintf(printBuffer, sizeof(printBuffer), pcString, args);

#if defined(UART_ENABLED) && defined(ENETAPPUTILS_UART_ALLOWED)
    UART_printf("%s", printBuffer);
#else
    if (UTRUE == EnetAppUtils_isPrintSupported())
    {
        printf("%s", printBuffer);
    }
#endif
}

void EnetAppUtils_print(const char *pcString,
                        ...)
{
    char printBuffer[ENET_CFG_PRINT_BUF_LEN];
    va_list arguments;

#if ENET_CFG_IS_ON(DEV_ERROR)
    if (ENET_CFG_PRINT_BUF_LEN < strlen(pcString))
    {
        /* Don't use EnetAppUtils_assert as it uses EnetAppUtils_print function  */
        assert(BFALSE);
    }
#endif

    /* Start the varargs processing */
    va_start(arguments, pcString);
    vsnprintf(printBuffer, sizeof(printBuffer), pcString, arguments);

    {
#if defined(UART_ENABLED) && defined(ENETAPPUTILS_UART_ALLOWED)
        UART_printf("%s", printBuffer);
#else
        if (UTRUE == EnetAppUtils_isPrintSupported())
        {
            printf("%s", printBuffer);
        }
#endif
    }

    /* End the varargs processing */
    va_end(arguments);
}

char EnetAppUtils_getChar(void)
{
    char ch;

#if defined(UART_ENABLED) && defined(ENETAPPUTILS_UART_ALLOWED)
    ch = UART_getc();
#else
    scanf("%c", &ch);
#endif

    return ch;
}

char EnetAppUtils_getChar2(void)
{
    char ch;

#if defined(UART_ENABLED) && defined(ENETAPPUTILS_UART_ALLOWED)
    ch = UART_getByteFromConsole();
#else
    scanf("%c", &ch);
#endif

    return ch;
}

int32_t EnetAppUtils_getNum(void)
{
    int32_t num;

#if defined(UART_ENABLED) && defined(ENETAPPUTILS_UART_ALLOWED)
    UART_scanFmt("%d", &num);
#else
    scanf("%d", &num);
#endif

    return num;
}

uint32_t EnetAppUtils_getHex(void)
{
    uint32_t num;

#if defined(UART_ENABLED) && defined(ENETAPPUTILS_UART_ALLOWED)
    UART_scanFmt("%x", &num);
#else
    scanf("%x", &num);
#endif

    return num;
}

uint32_t EnetAppUtils_randFxn(uint32_t min,
                              uint32_t max)
{
    return (rand() % (max - min + 1)) + min;
}

void EnetAppUtils_waitEmuConnect(void)
{
    volatile uint32_t emuWaitFlag = 0x1U;

    while (emuWaitFlag)
    {
        ;
    }
}

uint32_t EnetAppUtils_isPrintSupported(void)
{
    uint32_t retVal = UTRUE;

#if defined(BUILD_MPU)
    retVal = UFALSE;
#endif

    return(retVal);
}

static void EnetAppUtils_printStatsNonZero(const char *pcString,
                                           uint64_t statVal)
{
    if (0U != statVal)
    {
        EnetAppUtils_print(pcString, statVal);
    }
}

static void EnetAppUtils_printStatsWithIdxNonZero(const char *pcString,
                                                  uint32_t idx,
                                                  uint64_t statVal)
{
    if (0U != statVal)
    {
        EnetAppUtils_print(pcString, idx, statVal);
    }
}


static void EnetAppUtils_printDmaDescStats(EnetDma_DmaDescStats *descstats)
{
    // TODO when DMA stats are enabled.
}

static void EnetAppUtils_printCbStats(EnetDma_CbStats *cbStats)
{
    uint32_t i;

    EnetAppUtils_printStatsNonZero("Data Notify Count          = %llu\n", cbStats->dataNotifyCnt);
    EnetAppUtils_printStatsNonZero("Zero Notify Count          = %llu\n", cbStats->zeroNotifyCnt);
    EnetAppUtils_printStatsNonZero("Total Packets Count        = %llu\n", cbStats->totalPktCnt);
    EnetAppUtils_printStatsNonZero("Total Cycles Count         = %llu\n", cbStats->totalCycleCnt);
    EnetAppUtils_printStatsNonZero("Packets per Notify Max     = %llu\n", cbStats->pktsPerNotifyMax);
    for (i = 0U; i < ENET_DMA_STATS_HISTORY_CNT; i++)
    {
        EnetAppUtils_printStatsWithIdxNonZero("Packets per Notify[%d] = %llu\n", i, cbStats->pktsPerNotify[i]);
    }

    EnetAppUtils_printStatsNonZero("Cycles per Notify Max      = %llu\n", cbStats->cycleCntPerNotifyMax);
    for (i = 0U; i < ENET_DMA_STATS_HISTORY_CNT; i++)
    {
        EnetAppUtils_printStatsWithIdxNonZero("Cycles per Notify[%d]  = %llu\n", i, cbStats->cycleCntPerNotify[i]);
    }

    EnetAppUtils_printStatsNonZero("Cycles per Packet Max      = %llu\n", cbStats->cycleCntPerPktMax);
    for (i = 0U; i < ENET_DMA_STATS_HISTORY_CNT; i++)
    {
        EnetAppUtils_printStatsWithIdxNonZero("Cycles per Packet[%d]  = %llu\n", i, cbStats->cycleCntPerPkt[i]);
    }

    for (i = 0U; i < ENET_DMA_STATS_HISTORY_CNT; i++)
    {
        EnetAppUtils_printStatsWithIdxNonZero("Ready Desc Q Count[%d] = %llu\n", i, cbStats->readyDmaDescQCnt[i]);
    }
}

static void EnetAppUtils_printTxChStats(EnetDma_TxChStats *stats)
{
    EnetAppUtils_print(" TX Channel Statistics\n");
    EnetAppUtils_print("-----------------------------------------\n");
    EnetAppUtils_printCbStats(&stats->submitPktStats);
    EnetAppUtils_printCbStats(&stats->retrievePktStats);
    EnetAppUtils_printDmaDescStats(&stats->dmaDescStats);

    EnetAppUtils_printStatsNonZero("TX Submit Packet EnQ count           = %llu\n", stats->txSubmitPktEnq);
    EnetAppUtils_printStatsNonZero("TX Submit Packet Underflow           = %llu\n", stats->txSubmitPktOverFlowCnt);
    EnetAppUtils_printStatsNonZero("TX Submit Packet DeQ count           = %llu\n", stats->txRetrievePktDeq);
}

static void EnetAppUtils_printRxChStats(EnetDma_RxChStats *stats)
{
    EnetAppUtils_print(" RX Channel Statistics\n");
    EnetAppUtils_print("-----------------------------------------\n");
    EnetAppUtils_printCbStats(&stats->submitPktStats);
    EnetAppUtils_printCbStats(&stats->retrievePktStats);
    EnetAppUtils_printDmaDescStats(&stats->dmaDescStats);

    EnetAppUtils_printStatsNonZero("RX Submit Packet EnQ count           = %llu\n", stats->rxSubmitPktEnq);
    EnetAppUtils_printStatsNonZero("RX Submit Packet Underflow           = %llu\n", stats->rxSubmitPktUnderFlowCnt);
    EnetAppUtils_printStatsNonZero("RX Submit Packet DeQ count           = %llu\n", stats->rxRetrievePktDeq);
}

int32_t EnetAppUtils_showTxChStats(EnetDma_TxChHandle hTxCh)
{
    int32_t retVal = ENET_SOK;

    EnetDma_TxChStats txChStats;

    retVal = EnetDma_getTxChStats(hTxCh, &txChStats);
    if (ENET_ENOTSUPPORTED != retVal)
    {
        EnetAppUtils_printTxChStats(&txChStats);
        EnetDma_resetTxChStats(hTxCh);
    }

    return retVal;
}

int32_t EnetAppUtils_showRxChStats(EnetDma_RxChHandle hRxCh)
{
    int32_t retVal = ENET_SOK;

    EnetDma_RxChStats rxChStats;

    retVal = EnetDma_getRxChStats(hRxCh, &rxChStats);
    if (ENET_ENOTSUPPORTED != retVal)
    {
        EnetAppUtils_printRxChStats(&rxChStats);
        EnetDma_resetRxChStats(hRxCh);
    }

    return retVal;
}

void EnetAppUtils_printMacAddr(uint8_t macAddr[])
{
    EnetAppUtils_print("%02x:%02x:%02x:%02x:%02x:%02x\n",
                       macAddr[0] & 0xFF,
                       macAddr[1] & 0xFF,
                       macAddr[2] & 0xFF,
                       macAddr[3] & 0xFF,
                       macAddr[4] & 0xFF,
                       macAddr[5] & 0xFF);
}

void EnetAppUtils_printFrame(EthFrame *frame,
                             uint32_t len)
{
    uint8_t *payload;
    uint32_t i;

    EnetAppUtils_print("Dst addr : ");
    EnetAppUtils_printMacAddr(&frame->hdr.dstMac[0]);

    EnetAppUtils_print("Src addr : ");
    EnetAppUtils_printMacAddr(&frame->hdr.srcMac[0]);

    if (frame->hdr.etherType == Enet_htons(ETHERTYPE_VLAN_TAG))
    {
        EthVlanFrame *vlanFrame = (EthVlanFrame *)frame;

        EnetAppUtils_print("TPID     : 0x%04x\n", Enet_ntohs(vlanFrame->hdr.tpid) & 0xFFFFU);
        EnetAppUtils_print("Priority : %d\n", (Enet_ntohs(vlanFrame->hdr.tci) & 0xFFFFU) >> 13);
        EnetAppUtils_print("VLAN Id  : %d\n", Enet_ntohs(vlanFrame->hdr.tci) & 0xFFFU);
        EnetAppUtils_print("EtherType: 0x%04x\n", Enet_ntohs(vlanFrame->hdr.etherType) & 0xFFFFU);
        payload = vlanFrame->payload;
        len    -= ETH_VLAN_TAG_LEN;
    }
    else
    {
        EnetAppUtils_print("EtherType: 0x%04x\n", Enet_ntohs(frame->hdr.etherType) & 0xFFFFU);
        payload = frame->payload;
    }

    EnetAppUtils_print("Payload  : ");
    for (i = 0; i < len; i++)
    {
        EnetAppUtils_print("0x%02x ", payload[i]);
        if (i && (((i + 1) % OCTETS_PER_ROW) == 0))
        {
            EnetAppUtils_print("\n           ");
        }
    }

    if (len && ((len % OCTETS_PER_ROW) != 0))
    {
        EnetAppUtils_print("\n");
    }

    EnetAppUtils_print("\n");
}

void EnetAppUtils_printSGFrame(EnetDma_Pkt *pktInfo)
{
    uint8_t *payload;
    uint32_t i;
    EthFrame *frame = (EthFrame *)pktInfo->sgList.list[0].bufPtr;
    uint32_t headerLen;
    uint32_t segmentIndex;
    uint32_t payLoadLen;
    uint32_t totalPacketFilledLen = 0U;

    EnetAppUtils_print("Dst addr : ");
    EnetAppUtils_printMacAddr(&frame->hdr.dstMac[0]);

    EnetAppUtils_print("Src addr : ");
    EnetAppUtils_printMacAddr(&frame->hdr.srcMac[0]);

    if (frame->hdr.etherType == Enet_htons(ETHERTYPE_VLAN_TAG))
    {
        EthVlanFrame *vlanFrame = (EthVlanFrame *)frame;

        EnetAppUtils_print("TPID     : 0x%04x\n", Enet_ntohs(vlanFrame->hdr.tpid) & 0xFFFFU);
        EnetAppUtils_print("Priority : %d\n", (Enet_ntohs(vlanFrame->hdr.tci) & 0xFFFFU) >> 13);
        EnetAppUtils_print("VLAN Id  : %d\n", Enet_ntohs(vlanFrame->hdr.tci) & 0xFFFU);
        EnetAppUtils_print("EtherType: 0x%04x\n", Enet_ntohs(vlanFrame->hdr.etherType) & 0xFFFFU);
        payload = vlanFrame->payload;
    }
    else
    {
        EnetAppUtils_print("EtherType: 0x%04x\n", Enet_ntohs(frame->hdr.etherType) & 0xFFFFU);
        payload = frame->payload;
    }
    headerLen = (payload - pktInfo->sgList.list[0].bufPtr);
    EnetAppUtils_assert(pktInfo->sgList.list[0].segmentFilledLen >= headerLen);
    EnetAppUtils_print("Payload  : ");
    for (segmentIndex = 0U; segmentIndex < pktInfo->sgList.numScatterSegments; segmentIndex++)
    {
        uint32_t segmentFillLength = pktInfo->sgList.list[segmentIndex].segmentFilledLen;
        totalPacketFilledLen += segmentFillLength;
        if (segmentIndex == 0)
        {
            segmentFillLength -= headerLen;
        }
        else
        {
            payload = pktInfo->sgList.list[segmentIndex].bufPtr;
        }
        for (i = 0; i < segmentFillLength; i++)
        {
            EnetAppUtils_print("0x%02x ", payload[i]);
            if (i && (((i + 1) % OCTETS_PER_ROW) == 0))
            {
                EnetAppUtils_print("\n           ");
            }
        }
    }
    payLoadLen = totalPacketFilledLen - headerLen;
    if ((payLoadLen) && (((payLoadLen + 1) % OCTETS_PER_ROW) != 0))
    {
        EnetAppUtils_print("\n");
    }

    EnetAppUtils_print("\n");
}

void EnetAppUtils_printHostPortStats2G(CpswStats_HostPort_2g *st)
{
    EnetAppUtils_printStatsNonZero("  rxGoodFrames            = %llu\n", st->rxGoodFrames);
    EnetAppUtils_printStatsNonZero("  rxBcastFrames           = %llu\n", st->rxBcastFrames);
    EnetAppUtils_printStatsNonZero("  rxMcastFrames           = %llu\n", st->rxMcastFrames);
    EnetAppUtils_printStatsNonZero("  rxCrcErrors             = %llu\n", st->rxCrcErrors);
    EnetAppUtils_printStatsNonZero("  rxOversizedFrames       = %llu\n", st->rxOversizedFrames);
    EnetAppUtils_printStatsNonZero("  rxUndersizedFrames      = %llu\n", st->rxUndersizedFrames);
    EnetAppUtils_printStatsNonZero("  rxFragments             = %llu\n", st->rxFragments);
    EnetAppUtils_printStatsNonZero("  aleDrop                 = %llu\n", st->aleDrop);
    EnetAppUtils_printStatsNonZero("  aleOverrunDrop          = %llu\n", st->aleOverrunDrop);
    EnetAppUtils_printStatsNonZero("  rxOctets                = %llu\n", st->rxOctets);
    EnetAppUtils_printStatsNonZero("  txGoodFrames            = %llu\n", st->txGoodFrames);
    EnetAppUtils_printStatsNonZero("  txBcastFrames           = %llu\n", st->txBcastFrames);
    EnetAppUtils_printStatsNonZero("  txMcastFrames           = %llu\n", st->txMcastFrames);
    EnetAppUtils_printStatsNonZero("  txOctets                = %llu\n", st->txOctets);
    EnetAppUtils_printStatsNonZero("  octetsFrames64          = %llu\n", st->octetsFrames64);
    EnetAppUtils_printStatsNonZero("  octetsFrames65to127     = %llu\n", st->octetsFrames65to127);
    EnetAppUtils_printStatsNonZero("  octetsFrames128to255    = %llu\n", st->octetsFrames128to255);
    EnetAppUtils_printStatsNonZero("  octetsFrames256to511    = %llu\n", st->octetsFrames256to511);
    EnetAppUtils_printStatsNonZero("  octetsFrames512to1023   = %llu\n", st->octetsFrames512to1023);
    EnetAppUtils_printStatsNonZero("  octetsFrames1024        = %llu\n", st->octetsFrames1024);
    EnetAppUtils_printStatsNonZero("  netOctets               = %llu\n", st->netOctets);
    EnetAppUtils_printStatsNonZero("  rxBottomOfFifoDrop      = %llu\n", st->rxBottomOfFifoDrop);
    EnetAppUtils_printStatsNonZero("  portMaskDrop            = %llu\n", st->portMaskDrop);
    EnetAppUtils_printStatsNonZero("  rxTopOfFifoDrop         = %llu\n", st->rxTopOfFifoDrop);
    EnetAppUtils_printStatsNonZero("  aleRateLimitDrop        = %llu\n", st->aleRateLimitDrop);
    EnetAppUtils_printStatsNonZero("  aleVidIngressDrop       = %llu\n", st->aleVidIngressDrop);
    EnetAppUtils_printStatsNonZero("  aleDAEqSADrop           = %llu\n", st->aleDAEqSADrop);
    EnetAppUtils_printStatsNonZero("  aleBlockDrop            = %llu\n", st->aleBlockDrop);
    EnetAppUtils_printStatsNonZero("  aleSecureDrop           = %llu\n", st->aleSecureDrop);
    EnetAppUtils_printStatsNonZero("  aleAuthDrop             = %llu\n", st->aleAuthDrop);
    EnetAppUtils_printStatsNonZero("  aleUnknownUcast         = %llu\n", st->aleUnknownUcast);
    EnetAppUtils_printStatsNonZero("  aleUnknownUcastBcnt     = %llu\n", st->aleUnknownUcastBcnt);
    EnetAppUtils_printStatsNonZero("  aleUnknownMcast         = %llu\n", st->aleUnknownMcast);
    EnetAppUtils_printStatsNonZero("  aleUnknownMcastBcnt     = %llu\n", st->aleUnknownMcastBcnt);
    EnetAppUtils_printStatsNonZero("  aleUnknownBcast         = %llu\n", st->aleUnknownBcast);
    EnetAppUtils_printStatsNonZero("  aleUnknownBcastBcnt     = %llu\n", st->aleUnknownBcastBcnt);
    EnetAppUtils_printStatsNonZero("  alePolicyMatch          = %llu\n", st->alePolicyMatch);
    EnetAppUtils_printStatsNonZero("  alePolicyMatchRed       = %llu\n", st->alePolicyMatchRed);
    EnetAppUtils_printStatsNonZero("  alePolicyMatchYellow    = %llu\n", st->alePolicyMatchYellow);
    EnetAppUtils_printStatsNonZero("  txMemProtectError       = %llu\n", st->txMemProtectError);
}

void EnetAppUtils_printMacPortStats2G(CpswStats_MacPort_2g *st)
{
    uint32_t i;

    EnetAppUtils_printStatsNonZero("  rxGoodFrames            = %llu\n", st->rxGoodFrames);
    EnetAppUtils_printStatsNonZero("  rxBcastFrames           = %llu\n", st->rxBcastFrames);
    EnetAppUtils_printStatsNonZero("  rxMcastFrames           = %llu\n", st->rxMcastFrames);
    EnetAppUtils_printStatsNonZero("  rxPauseFrames           = %llu\n", st->rxPauseFrames);
    EnetAppUtils_printStatsNonZero("  rxCrcErrors             = %llu\n", st->rxCrcErrors);
    EnetAppUtils_printStatsNonZero("  rxAlignCodeErrors       = %llu\n", st->rxAlignCodeErrors);
    EnetAppUtils_printStatsNonZero("  rxOversizedFrames       = %llu\n", st->rxOversizedFrames);
    EnetAppUtils_printStatsNonZero("  rxJabberFrames          = %llu\n", st->rxJabberFrames);
    EnetAppUtils_printStatsNonZero("  rxUndersizedFrames      = %llu\n", st->rxUndersizedFrames);
    EnetAppUtils_printStatsNonZero("  rxFragments             = %llu\n", st->rxFragments);
    EnetAppUtils_printStatsNonZero("  aleDrop                 = %llu\n", st->aleDrop);
    EnetAppUtils_printStatsNonZero("  aleOverrunDrop          = %llu\n", st->aleOverrunDrop);
    EnetAppUtils_printStatsNonZero("  rxOctets                = %llu\n", st->rxOctets);
    EnetAppUtils_printStatsNonZero("  txGoodFrames            = %llu\n", st->txGoodFrames);
    EnetAppUtils_printStatsNonZero("  txBcastFrames           = %llu\n", st->txBcastFrames);
    EnetAppUtils_printStatsNonZero("  txMcastFrames           = %llu\n", st->txMcastFrames);
    EnetAppUtils_printStatsNonZero("  txPauseFrames           = %llu\n", st->txPauseFrames);
    EnetAppUtils_printStatsNonZero("  txDeferredFrames        = %llu\n", st->txDeferredFrames);
    EnetAppUtils_printStatsNonZero("  txCollisionFrames       = %llu\n", st->txCollisionFrames);
    EnetAppUtils_printStatsNonZero("  txSingleCollFrames      = %llu\n", st->txSingleCollFrames);
    EnetAppUtils_printStatsNonZero("  txMultipleCollFrames    = %llu\n", st->txMultipleCollFrames);
    EnetAppUtils_printStatsNonZero("  txExcessiveCollFrames   = %llu\n", st->txExcessiveCollFrames);
    EnetAppUtils_printStatsNonZero("  txLateCollFrames        = %llu\n", st->txLateCollFrames);
    EnetAppUtils_printStatsNonZero("  rxIPGError              = %llu\n", st->rxIPGError);
    EnetAppUtils_printStatsNonZero("  txCarrierSenseErrors    = %llu\n", st->txCarrierSenseErrors);
    EnetAppUtils_printStatsNonZero("  txOctets                = %llu\n", st->txOctets);
    EnetAppUtils_printStatsNonZero("  octetsFrames64          = %llu\n", st->octetsFrames64);
    EnetAppUtils_printStatsNonZero("  octetsFrames65to127     = %llu\n", st->octetsFrames65to127);
    EnetAppUtils_printStatsNonZero("  octetsFrames128to255    = %llu\n", st->octetsFrames128to255);
    EnetAppUtils_printStatsNonZero("  octetsFrames256to511    = %llu\n", st->octetsFrames256to511);
    EnetAppUtils_printStatsNonZero("  octetsFrames512to1023   = %llu\n", st->octetsFrames512to1023);
    EnetAppUtils_printStatsNonZero("  octetsFrames1024        = %llu\n", st->octetsFrames1024);
    EnetAppUtils_printStatsNonZero("  netOctets               = %llu\n", st->netOctets);
    EnetAppUtils_printStatsNonZero("  rxBottomOfFifoDrop      = %llu\n", st->rxBottomOfFifoDrop);
    EnetAppUtils_printStatsNonZero("  portMaskDrop            = %llu\n", st->portMaskDrop);
    EnetAppUtils_printStatsNonZero("  rxTopOfFifoDrop         = %llu\n", st->rxTopOfFifoDrop);
    EnetAppUtils_printStatsNonZero("  aleRateLimitDrop        = %llu\n", st->aleRateLimitDrop);
    EnetAppUtils_printStatsNonZero("  aleVidIngressDrop       = %llu\n", st->aleVidIngressDrop);
    EnetAppUtils_printStatsNonZero("  aleDAEqSADrop           = %llu\n", st->aleDAEqSADrop);
    EnetAppUtils_printStatsNonZero("  aleBlockDrop            = %llu\n", st->aleBlockDrop);
    EnetAppUtils_printStatsNonZero("  aleSecureDrop           = %llu\n", st->aleSecureDrop);
    EnetAppUtils_printStatsNonZero("  aleAuthDrop             = %llu\n", st->aleAuthDrop);
    EnetAppUtils_printStatsNonZero("  aleUnknownUcast         = %llu\n", st->aleUnknownUcast);
    EnetAppUtils_printStatsNonZero("  aleUnknownUcastBcnt     = %llu\n", st->aleUnknownUcastBcnt);
    EnetAppUtils_printStatsNonZero("  aleUnknownMcast         = %llu\n", st->aleUnknownMcast);
    EnetAppUtils_printStatsNonZero("  aleUnknownMcastBcnt     = %llu\n", st->aleUnknownMcastBcnt);
    EnetAppUtils_printStatsNonZero("  aleUnknownBcast         = %llu\n", st->aleUnknownBcast);
    EnetAppUtils_printStatsNonZero("  aleUnknownBcastBcnt     = %llu\n", st->aleUnknownBcastBcnt);
    EnetAppUtils_printStatsNonZero("  alePolicyMatch          = %llu\n", st->alePolicyMatch);
    EnetAppUtils_printStatsNonZero("  alePolicyMatchRed       = %llu\n", st->alePolicyMatchRed);
    EnetAppUtils_printStatsNonZero("  alePolicyMatchYellow    = %llu\n", st->alePolicyMatchYellow);
    EnetAppUtils_printStatsNonZero("  txMemProtectError       = %llu\n", st->txMemProtectError);

    for (i = 0U; i < ENET_ARRAYSIZE(st->txPri); i++)
    {
        EnetAppUtils_printStatsWithIdxNonZero("  txPri[%u]                = %llu\n", i, st->txPri[i]);
    }

    for (i = 0U; i < ENET_ARRAYSIZE(st->txPriBcnt); i++)
    {
        EnetAppUtils_printStatsWithIdxNonZero("  txPriBcnt[%u]            = %llu\n", i, st->txPriBcnt[i]);
    }

    for (i = 0U; i < ENET_ARRAYSIZE(st->txPriDrop); i++)
    {
        EnetAppUtils_printStatsWithIdxNonZero("  txPriDrop[%u]            = %llu\n", i, st->txPriDrop[i]);
    }

    for (i = 0U; i < ENET_ARRAYSIZE(st->txPriDropBcnt); i++)
    {
        EnetAppUtils_printStatsWithIdxNonZero("  txPriDropBcnt[%u]        = %llu\n", i, st->txPriDropBcnt[i]);
    }
}

void EnetAppUtils_printHostPortStats9G(CpswStats_HostPort_Ng *st)
{
    uint_fast32_t i;

    EnetAppUtils_printStatsNonZero("  rxGoodFrames            = %llu\n", st->rxGoodFrames);
    EnetAppUtils_printStatsNonZero("  rxBcastFrames           = %llu\n", st->rxBcastFrames);
    EnetAppUtils_printStatsNonZero("  rxMcastFrames           = %llu\n", st->rxMcastFrames);
    EnetAppUtils_printStatsNonZero("  rxCrcErrors             = %llu\n", st->rxCrcErrors);
    EnetAppUtils_printStatsNonZero("  rxOversizedFrames       = %llu\n", st->rxOversizedFrames);
    EnetAppUtils_printStatsNonZero("  rxUndersizedFrames      = %llu\n", st->rxUndersizedFrames);
    EnetAppUtils_printStatsNonZero("  rxFragments             = %llu\n", st->rxFragments);
    EnetAppUtils_printStatsNonZero("  aleDrop                 = %llu\n", st->aleDrop);
    EnetAppUtils_printStatsNonZero("  aleOverrunDrop          = %llu\n", st->aleOverrunDrop);
    EnetAppUtils_printStatsNonZero("  rxOctets                = %llu\n", st->rxOctets);
    EnetAppUtils_printStatsNonZero("  txGoodFrames            = %llu\n", st->txGoodFrames);
    EnetAppUtils_printStatsNonZero("  txBcastFrames           = %llu\n", st->txBcastFrames);
    EnetAppUtils_printStatsNonZero("  txMcastFrames           = %llu\n", st->txMcastFrames);
    EnetAppUtils_printStatsNonZero("  txOctets                = %llu\n", st->txOctets);
    EnetAppUtils_printStatsNonZero("  octetsFrames64          = %llu\n", st->octetsFrames64);
    EnetAppUtils_printStatsNonZero("  octetsFrames65to127     = %llu\n", st->octetsFrames65to127);
    EnetAppUtils_printStatsNonZero("  octetsFrames128to255    = %llu\n", st->octetsFrames128to255);
    EnetAppUtils_printStatsNonZero("  octetsFrames256to511    = %llu\n", st->octetsFrames256to511);
    EnetAppUtils_printStatsNonZero("  octetsFrames512to1023   = %llu\n", st->octetsFrames512to1023);
    EnetAppUtils_printStatsNonZero("  octetsFrames1024        = %llu\n", st->octetsFrames1024);
    EnetAppUtils_printStatsNonZero("  netOctets               = %llu\n", st->netOctets);
    EnetAppUtils_printStatsNonZero("  rxBottomOfFifoDrop      = %llu\n", st->rxBottomOfFifoDrop);
    EnetAppUtils_printStatsNonZero("  portMaskDrop            = %llu\n", st->portMaskDrop);
    EnetAppUtils_printStatsNonZero("  rxTopOfFifoDrop         = %llu\n", st->rxTopOfFifoDrop);
    EnetAppUtils_printStatsNonZero("  aleRateLimitDrop        = %llu\n", st->aleRateLimitDrop);
    EnetAppUtils_printStatsNonZero("  aleVidIngressDrop       = %llu\n", st->aleVidIngressDrop);
    EnetAppUtils_printStatsNonZero("  aleDAEqSADrop           = %llu\n", st->aleDAEqSADrop);
    EnetAppUtils_printStatsNonZero("  aleBlockDrop            = %llu\n", st->aleBlockDrop);
    EnetAppUtils_printStatsNonZero("  aleSecureDrop           = %llu\n", st->aleSecureDrop);
    EnetAppUtils_printStatsNonZero("  aleAuthDrop             = %llu\n", st->aleAuthDrop);
    EnetAppUtils_printStatsNonZero("  aleUnknownUcast         = %llu\n", st->aleUnknownUcast);
    EnetAppUtils_printStatsNonZero("  aleUnknownUcastBcnt     = %llu\n", st->aleUnknownUcastBcnt);
    EnetAppUtils_printStatsNonZero("  aleUnknownMcast         = %llu\n", st->aleUnknownMcast);
    EnetAppUtils_printStatsNonZero("  aleUnknownMcastBcnt     = %llu\n", st->aleUnknownMcastBcnt);
    EnetAppUtils_printStatsNonZero("  aleUnknownBcast         = %llu\n", st->aleUnknownBcast);
    EnetAppUtils_printStatsNonZero("  aleUnknownBcastBcnt     = %llu\n", st->aleUnknownBcastBcnt);
    EnetAppUtils_printStatsNonZero("  alePolicyMatch          = %llu\n", st->alePolicyMatch);
    EnetAppUtils_printStatsNonZero("  alePolicyMatchRed       = %llu\n", st->alePolicyMatchRed);
    EnetAppUtils_printStatsNonZero("  alePolicyMatchYellow    = %llu\n", st->alePolicyMatchYellow);
    EnetAppUtils_printStatsNonZero("  aleMultSADrop           = %llu\n", st->aleMultSADrop);
    EnetAppUtils_printStatsNonZero("  aleDualVlanDrop         = %llu\n", st->aleDualVlanDrop);
    EnetAppUtils_printStatsNonZero("  aleLenErrorDrop         = %llu\n", st->aleLenErrorDrop);
    EnetAppUtils_printStatsNonZero("  aleIpNextHdrDrop        = %llu\n", st->aleIpNextHdrDrop);
    EnetAppUtils_printStatsNonZero("  aleIPv4FragDrop         = %llu\n", st->aleIPv4FragDrop);
    EnetAppUtils_printStatsNonZero("  ietRxAssemblyErr        = %llu\n", st->ietRxAssemblyErr);
    EnetAppUtils_printStatsNonZero("  ietRxAssemblyOk         = %llu\n", st->ietRxAssemblyOk);
    EnetAppUtils_printStatsNonZero("  ietRxSmdError           = %llu\n", st->ietRxSmdError);
    EnetAppUtils_printStatsNonZero("  ietRxFrag               = %llu\n", st->ietRxFrag);
    EnetAppUtils_printStatsNonZero("  ietTxHold               = %llu\n", st->ietTxHold);
    EnetAppUtils_printStatsNonZero("  ietTxFrag               = %llu\n", st->ietTxFrag);
    EnetAppUtils_printStatsNonZero("  txMemProtectError       = %llu\n", st->txMemProtectError);

    for (i = 0U; i < ENET_ARRAYSIZE(st->txPri); i++)
    {
        EnetAppUtils_printStatsWithIdxNonZero("  txPri[%u]                = %llu\n", i, st->txPri[i]);
    }

    for (i = 0U; i < ENET_ARRAYSIZE(st->txPriBcnt); i++)
    {
        EnetAppUtils_printStatsWithIdxNonZero("  txPriBcnt[%u]            = %llu\n", i, st->txPriBcnt[i]);
    }

    for (i = 0U; i < ENET_ARRAYSIZE(st->txPriDrop); i++)
    {
        EnetAppUtils_printStatsWithIdxNonZero("  txPriDrop[%u]            = %llu\n", i, st->txPriDrop[i]);
    }

    for (i = 0U; i < ENET_ARRAYSIZE(st->txPriDropBcnt); i++)
    {
        EnetAppUtils_printStatsWithIdxNonZero("  txPriDropBcnt[%u]        = %llu\n", i, st->txPriDropBcnt[i]);
    }
}

void EnetAppUtils_printMacPortStats9G(CpswStats_MacPort_Ng *st)
{
    uint_fast32_t i;

    EnetAppUtils_printStatsNonZero("  rxGoodFrames            = %llu\n", st->rxGoodFrames);
    EnetAppUtils_printStatsNonZero("  rxBcastFrames           = %llu\n", st->rxBcastFrames);
    EnetAppUtils_printStatsNonZero("  rxMcastFrames           = %llu\n", st->rxMcastFrames);
    EnetAppUtils_printStatsNonZero("  rxPauseFrames           = %llu\n", st->rxPauseFrames);
    EnetAppUtils_printStatsNonZero("  rxCrcErrors             = %llu\n", st->rxCrcErrors);
    EnetAppUtils_printStatsNonZero("  rxAlignCodeErrors       = %llu\n", st->rxAlignCodeErrors);
    EnetAppUtils_printStatsNonZero("  rxOversizedFrames       = %llu\n", st->rxOversizedFrames);
    EnetAppUtils_printStatsNonZero("  rxJabberFrames          = %llu\n", st->rxJabberFrames);
    EnetAppUtils_printStatsNonZero("  rxUndersizedFrames      = %llu\n", st->rxUndersizedFrames);
    EnetAppUtils_printStatsNonZero("  rxFragments             = %llu\n", st->rxFragments);
    EnetAppUtils_printStatsNonZero("  aleDrop                 = %llu\n", st->aleDrop);
    EnetAppUtils_printStatsNonZero("  aleOverrunDrop          = %llu\n", st->aleOverrunDrop);
    EnetAppUtils_printStatsNonZero("  rxOctets                = %llu\n", st->rxOctets);
    EnetAppUtils_printStatsNonZero("  txGoodFrames            = %llu\n", st->txGoodFrames);
    EnetAppUtils_printStatsNonZero("  txBcastFrames           = %llu\n", st->txBcastFrames);
    EnetAppUtils_printStatsNonZero("  txMcastFrames           = %llu\n", st->txMcastFrames);
    EnetAppUtils_printStatsNonZero("  txPauseFrames           = %llu\n", st->txPauseFrames);
    EnetAppUtils_printStatsNonZero("  txDeferredFrames        = %llu\n", st->txDeferredFrames);
    EnetAppUtils_printStatsNonZero("  txCollisionFrames       = %llu\n", st->txCollisionFrames);
    EnetAppUtils_printStatsNonZero("  txSingleCollFrames      = %llu\n", st->txSingleCollFrames);
    EnetAppUtils_printStatsNonZero("  txMultipleCollFrames    = %llu\n", st->txMultipleCollFrames);
    EnetAppUtils_printStatsNonZero("  txExcessiveCollFrames   = %llu\n", st->txExcessiveCollFrames);
    EnetAppUtils_printStatsNonZero("  txLateCollFrames        = %llu\n", st->txLateCollFrames);
    EnetAppUtils_printStatsNonZero("  rxIPGError              = %llu\n", st->rxIPGError);
    EnetAppUtils_printStatsNonZero("  txCarrierSenseErrors    = %llu\n", st->txCarrierSenseErrors);
    EnetAppUtils_printStatsNonZero("  txOctets                = %llu\n", st->txOctets);
    EnetAppUtils_printStatsNonZero("  octetsFrames64          = %llu\n", st->octetsFrames64);
    EnetAppUtils_printStatsNonZero("  octetsFrames65to127     = %llu\n", st->octetsFrames65to127);
    EnetAppUtils_printStatsNonZero("  octetsFrames128to255    = %llu\n", st->octetsFrames128to255);
    EnetAppUtils_printStatsNonZero("  octetsFrames256to511    = %llu\n", st->octetsFrames256to511);
    EnetAppUtils_printStatsNonZero("  octetsFrames512to1023   = %llu\n", st->octetsFrames512to1023);
    EnetAppUtils_printStatsNonZero("  octetsFrames1024        = %llu\n", st->octetsFrames1024);
    EnetAppUtils_printStatsNonZero("  netOctets               = %llu\n", st->netOctets);
    EnetAppUtils_printStatsNonZero("  rxBottomOfFifoDrop      = %llu\n", st->rxBottomOfFifoDrop);
    EnetAppUtils_printStatsNonZero("  portMaskDrop            = %llu\n", st->portMaskDrop);
    EnetAppUtils_printStatsNonZero("  rxTopOfFifoDrop         = %llu\n", st->rxTopOfFifoDrop);
    EnetAppUtils_printStatsNonZero("  aleRateLimitDrop        = %llu\n", st->aleRateLimitDrop);
    EnetAppUtils_printStatsNonZero("  aleVidIngressDrop       = %llu\n", st->aleVidIngressDrop);
    EnetAppUtils_printStatsNonZero("  aleDAEqSADrop           = %llu\n", st->aleDAEqSADrop);
    EnetAppUtils_printStatsNonZero("  aleBlockDrop            = %llu\n", st->aleBlockDrop);
    EnetAppUtils_printStatsNonZero("  aleSecureDrop           = %llu\n", st->aleSecureDrop);
    EnetAppUtils_printStatsNonZero("  aleAuthDrop             = %llu\n", st->aleAuthDrop);
    EnetAppUtils_printStatsNonZero("  aleUnknownUcast         = %llu\n", st->aleUnknownUcast);
    EnetAppUtils_printStatsNonZero("  aleUnknownUcastBcnt     = %llu\n", st->aleUnknownUcastBcnt);
    EnetAppUtils_printStatsNonZero("  aleUnknownMcast         = %llu\n", st->aleUnknownMcast);
    EnetAppUtils_printStatsNonZero("  aleUnknownMcastBcnt     = %llu\n", st->aleUnknownMcastBcnt);
    EnetAppUtils_printStatsNonZero("  aleUnknownBcast         = %llu\n", st->aleUnknownBcast);
    EnetAppUtils_printStatsNonZero("  aleUnknownBcastBcnt     = %llu\n", st->aleUnknownBcastBcnt);
    EnetAppUtils_printStatsNonZero("  alePolicyMatch          = %llu\n", st->alePolicyMatch);
    EnetAppUtils_printStatsNonZero("  alePolicyMatchRed       = %llu\n", st->alePolicyMatchRed);
    EnetAppUtils_printStatsNonZero("  alePolicyMatchYellow    = %llu\n", st->alePolicyMatchYellow);
    EnetAppUtils_printStatsNonZero("  aleMultSADrop           = %llu\n", st->aleMultSADrop);
    EnetAppUtils_printStatsNonZero("  aleDualVlanDrop         = %llu\n", st->aleDualVlanDrop);
    EnetAppUtils_printStatsNonZero("  aleLenErrorDrop         = %llu\n", st->aleLenErrorDrop);
    EnetAppUtils_printStatsNonZero("  aleIpNextHdrDrop        = %llu\n", st->aleIpNextHdrDrop);
    EnetAppUtils_printStatsNonZero("  aleIPv4FragDrop         = %llu\n", st->aleIPv4FragDrop);
    EnetAppUtils_printStatsNonZero("  ietRxAssemblyErr        = %llu\n", st->ietRxAssemblyErr);
    EnetAppUtils_printStatsNonZero("  ietRxAssemblyOk         = %llu\n", st->ietRxAssemblyOk);
    EnetAppUtils_printStatsNonZero("  ietRxSmdError           = %llu\n", st->ietRxSmdError);
    EnetAppUtils_printStatsNonZero("  ietRxFrag               = %llu\n", st->ietRxFrag);
    EnetAppUtils_printStatsNonZero("  ietTxHold               = %llu\n", st->ietTxHold);
    EnetAppUtils_printStatsNonZero("  ietTxFrag               = %llu\n", st->ietTxFrag);
    EnetAppUtils_printStatsNonZero("  txMemProtectError       = %llu\n", st->txMemProtectError);

    for (i = 0U; i < ENET_ARRAYSIZE(st->txPri); i++)
    {
        EnetAppUtils_printStatsWithIdxNonZero("  txPri[%u]                = %llu\n", i, st->txPri[i]);
    }

    for (i = 0U; i < ENET_ARRAYSIZE(st->txPriBcnt); i++)
    {
        EnetAppUtils_printStatsWithIdxNonZero("  txPriBcnt[%u]            = %llu\n", i, st->txPriBcnt[i]);
    }

    for (i = 0U; i < ENET_ARRAYSIZE(st->txPriDrop); i++)
    {
        EnetAppUtils_printStatsWithIdxNonZero("  txPriDrop[%u]            = %llu\n", i, st->txPriDrop[i]);
    }

    for (i = 0U; i < ENET_ARRAYSIZE(st->txPriDropBcnt); i++)
    {
        EnetAppUtils_printStatsWithIdxNonZero("  txPriDropBcnt[%u]        = %llu\n", i, st->txPriDropBcnt[i]);
    }
}

void EnetAppUtils_validatePacketState(EnetDma_PktQ *pQueue,
                                      uint32_t expectedState,
                                      uint32_t newState)
{
    uint32_t i;
    EnetDma_Pkt *pktInfo = (EnetDma_Pkt *)pQueue->head;

    for (i = 0; i < EnetQueue_getQCount(pQueue); i++)
    {
        EnetDma_checkPktState(&pktInfo->pktState,
                                ENET_PKTSTATE_MODULE_APP,
                                expectedState,
                                newState);
        pktInfo = (EnetDma_Pkt *)pktInfo->node.next;
    }
}

#if ENET_CFG_IS_ON(RM_PRESENT)
static void EnetAppUtils_reduceCoreMacAllocation(EnetRm_ResPrms *resPrms,
                                                 uint32_t *pReduceCount,
                                                 uint32_t coreMinCount,
                                                 bool skipCore,
                                                 uint32_t skipCoreId)
{
    uint32_t i;

    for (i = 0; (i < resPrms->numCores) && (*pReduceCount > 0); i++)
    {
        if ((resPrms->coreResInfo[i].numMacAddress > coreMinCount)
            &&
            ((skipCore == BFALSE) || (skipCoreId != resPrms->coreResInfo[i].coreId)))
        {
            uint32_t coreMacAddrReducedCount = (resPrms->coreResInfo[i].numMacAddress - coreMinCount);

            if (*pReduceCount >= coreMacAddrReducedCount)
            {
                *pReduceCount -= coreMacAddrReducedCount;
            }
            else
            {
                coreMacAddrReducedCount  = *pReduceCount;
                *pReduceCount            = 0;
            }

            EnetAppUtils_print("EnetAppUtils_reduceCoreMacAllocation: "
                               "Reduced Mac Address Allocation for CoreId:%u From %u To %u \n",
                               resPrms->coreResInfo[i].coreId,
                               resPrms->coreResInfo[i].numMacAddress,
                               (resPrms->coreResInfo[i].numMacAddress - coreMacAddrReducedCount));
            resPrms->coreResInfo[i].numMacAddress -= coreMacAddrReducedCount;
        }
    }
}

static void EnetAppUtils_updatemacResPart(EnetRm_ResPrms *resPrms,
                                                    uint32_t availMacCount,
                                                    uint32_t selfCoreId)
{
    uint32_t totalResPartMacCnt;
    uint32_t i;

    totalResPartMacCnt = 0;
    for (i = 0; i < resPrms->numCores; i++)
    {
        totalResPartMacCnt += resPrms->coreResInfo[i].numMacAddress;
    }

    if (totalResPartMacCnt > availMacCount)
    {
        uint32_t reduceCount = totalResPartMacCnt - availMacCount;

        /* First reduce mac count for cores with more than one mac address allocation.
         * If the available mac addr count is less than expected, Divide the available
         * mac addresses among the cores giving preference to the self core.
        */
        EnetAppUtils_reduceCoreMacAllocation(resPrms, &reduceCount, 1, BFALSE, selfCoreId);
        if (reduceCount)
        {
            /* Next reduce mac address for core other than self core to 0 */
            EnetAppUtils_reduceCoreMacAllocation(resPrms, &reduceCount, 0, BTRUE, selfCoreId);
        }

        /* Finally reduce self core also to 0 */
        if (reduceCount)
        {
            /* Next reduce mac address for core other than self core to 0 */
            EnetAppUtils_reduceCoreMacAllocation(resPrms, &reduceCount, 0, BFALSE, selfCoreId);
        }

        EnetAppUtils_assert(reduceCount == 0);
    }
}

#endif

void EnetAppUtils_initResourceConfig(Enet_Type enetType,
                                     uint32_t instId,
                                     uint32_t selfCoreId,
                                     EnetRm_ResCfg *resCfg)
{
#if ENET_CFG_IS_ON(RM_PRESENT)
    const EnetRm_ResPrms *resPrms = EnetAppRm_getResPartInfo(enetType, instId);
    const EnetRm_IoctlPermissionTable *ioPerms = EnetAppRm_getIoctlPermissionInfo(enetType, instId);
    EnetRm_MacAddressPool *macList = &resCfg->macList;

    EnetAppUtils_assert(resPrms != NULL);
    resCfg->resPartInfo = *resPrms;

    EnetAppUtils_assert(ioPerms != NULL);
    resCfg->ioctlPermissionInfo = *ioPerms;

    macList->numMacAddress = EnetBoard_getMacAddrList(enetType,
                                                      instId,
                                                      macList->macAddress,
                                                      ENET_ARRAYSIZE(macList->macAddress));
    if (macList->numMacAddress > ENET_ARRAYSIZE(macList->macAddress))
    {
        EnetAppUtils_print("EnetAppUtils_initResourceConfig: "
                           "Limiting number of mac address entries to macList->macAddress size"
                           "Available:%u, LimitedTo: %u",
                           macList->numMacAddress,
                           ENET_ARRAYSIZE(macList->macAddress));
        macList->numMacAddress = ENET_ARRAYSIZE(macList->macAddress);
    }

    EnetAppUtils_updatemacResPart(&resCfg->resPartInfo,
                                  macList->numMacAddress,
                                  selfCoreId);
    resCfg->selfCoreId = selfCoreId;
#endif
}

void EnetAppUtils_setNoPhyCfgRgmii(EnetMacPort_Interface *interface,
                                   EnetPhy_Cfg *phyCfg)
{
    phyCfg->phyAddr      = ENETPHY_INVALID_PHYADDR;
    interface->layerType    = ENET_MAC_LAYER_GMII;
    interface->sublayerType = ENET_MAC_SUBLAYER_REDUCED;
    interface->variantType  = ENET_MAC_VARIANT_FORCED;
}

void EnetAppUtils_setNoPhyCfgRmii(EnetMacPort_Interface *interface,
                                   EnetPhy_Cfg *phyCfg)
{
    phyCfg->phyAddr      = ENETPHY_INVALID_PHYADDR;
    interface->layerType    = ENET_MAC_LAYER_MII;
    interface->sublayerType = ENET_MAC_SUBLAYER_REDUCED;
    interface->variantType  = ENET_MAC_VARIANT_NONE;
}

void EnetAppUtils_setNoPhyCfgSgmii(EnetMacPort_Interface *interface,
                                   CpswMacPort_Cfg *macCfg,
                                   EnetPhy_Cfg *phyCfg)
{
    phyCfg->phyAddr      = ENETPHY_INVALID_PHYADDR;
    interface->layerType    = ENET_MAC_LAYER_GMII;
    interface->sublayerType = ENET_MAC_SUBLAYER_SERIAL;
    interface->variantType  = ENET_MAC_VARIANT_NONE;

    macCfg->sgmiiMode = ENET_MAC_SGMIIMODE_SGMII_FORCEDLINK;
}

int8_t EnetAppUtils_hex2Num(char hex)
{
    int8_t num = -1;

    if ((hex >= '0') && (hex <= '9'))
    {
        num =  hex - '0';
    }
    else if (hex >= 'a' && hex <= 'f')
    {
        num = hex - 'a' + 10;
    }
    else if (hex >= 'A' && hex <= 'F')
    {
        num = hex - 'A' + 10;
    }

    return num;
}

int32_t EnetAppUtils_macAddrAtoI(const char *txt, uint8_t *addr)
{
    int32_t status = ENET_SOK;
    int8_t a, b, i;

    for (i = 0; i < 6; i++)
    {
        a = EnetAppUtils_hex2Num(*txt++);
        if (a < 0)
        {
            status = ENET_EFAIL;
        }

        b = EnetAppUtils_hex2Num(*txt++);
        if (b < 0)
        {
            status = ENET_EFAIL;
        }

        *addr++ = (a << 4) | b;

        if ((i < 5) && (*txt++ != ':'))
        {
            status = ENET_EFAIL;
            break;
        }
    }

    return status;
}

int32_t EnetAppUtils_ipAddrAtoI(const char* txt, uint8_t *addr)
{
    int32_t status = ENET_SOK;
    uint8_t i;

    for (i = 0U; i < 4U; i++)
    {
        addr[i] = strtoul(txt, NULL, 10U);
        txt = strchr(txt, '.');
        if (((txt == NULL) || (*txt == '\0')) && (i != 3U))
        {
            status = ENET_EFAIL;
            break;
        }
        txt++;
    }
    return status;

}

static uint32_t EnetAppUtils_convertUsecs2Ticks(uint32_t delayInUsecs)
{
    uint32_t selfFreqHz = osalArch_TimeStampGetFreqKHz()*1000;

    return (uint32_t)(((uint64_t)delayInUsecs * selfFreqHz) / ENET_APPUTILS_SEC2MICROSEC);
}

void EnetAppUtils_delayInUsec(uint32_t delayInUsecs)
{
   uint32_t delayTicks = EnetAppUtils_convertUsecs2Ticks(delayInUsecs);
   uint32_t startTick = CycleprofilerP_getTimeStamp();
   uint32_t currentTick = 0U;
   uint32_t elapsedTicks = 0U;

   while (elapsedTicks < delayTicks)
   {
       NOP50;
       currentTick = CycleprofilerP_getTimeStamp();
       elapsedTicks = currentTick - startTick;
   }
}
