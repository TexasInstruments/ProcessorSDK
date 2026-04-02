/*
 * Copyright (C) 2024 Texas Instruments Incorporated
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the
 *   distribution.
 *
 *   Neither the name of Texas Instruments Incorporated nor the names of
 *   its contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*!
 * \file  cli_common.h
 *
 * \brief This is the common header file of the CLI application.
 */

#ifndef _CLI_COMMON_H_
#define _CLI_COMMON_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include <ti/drv/enet/include/core/enet_osal.h>
#include <ti/osal/TaskP.h>
#include <ti/osal/ClockP.h>
#include <ti/osal/SemaphoreP.h>
#include "test_config.h"

#include <ti/drv/enet/include/core/enet_dma.h>
#include <ti/drv/enet/include/per/cpsw.h>

#include <enet_apputils.h>
#include <enet_appmemutils.h>
#include <enet_appmemutils_cfg.h>
#include <ti/drv/uart/UART.h>

/*! FreeRTOS library */
#include "FreeRTOS.h"

/*! FreeRTOS CLI Library */
#include "FreeRTOS_CLI.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/*! Maximum write buffer length for CLI */
#define MAX_WRITE_BUFFER_LEN            500

/*! Maximum read buffer length for CLI */
#define MAX_READ_BUFFER_LEN             200

/*! Status flag for Dma channel when it is closed. */
#define CH_CLOSE                        0

/*! Status flag for Dma channel when it is open and idle. */
#define CH_IDLE                         1

/*! Status flag for Dma channel when it is open and running. */
#define CH_RUNNING                      2

/*! Status flag for Dma channel when it is stopped. */
#define CH_STOP                         3

/*! The number of Tx channels in total. */
#define ENET_TX_CHANNELS_NUM            (3U)

/*! The number of Rx flows in total. */
#define ENET_RX_FLOWS_NUM               (4U)

/*! TX DMA max num packets allocated to the DMA channel
  * Application should use the define below when opening a Tx DMA channel */

/*! Tx DMA max num packets allocated to channel 0 */
#define ENET_DMA_TX_CH0_NUM_PKTS        (4U)

/*! Tx DMA max num packets allocated to channel 1 */
#define ENET_DMA_TX_CH1_NUM_PKTS        (4U)

/*! Tx DMA max num packets allocated to channel 2 */
#define ENET_DMA_TX_CH2_NUM_PKTS        (4U)

/*! Tx DMA max num packets allocated to channel 3 */
#define ENET_DMA_TX_CH3_NUM_PKTS        (4U)

/*! RX DMA DMA max num packets allocated to the DMA channel  */
/*! Application should use the define below when opening a Rx DMA channel */

/*! Rx DMA max num packets allocated to channel 0 */
#define ENET_DMA_RX_CH0_NUM_PKTS        (8U)

/*! Tx DMA max num packets allocated to channel 1 */
#define ENET_DMA_RX_CH1_NUM_PKTS        (8U)

/*! Tx DMA max num packets allocated to channel 2 */
#define ENET_DMA_RX_CH2_NUM_PKTS        (8U)

/*! Tx DMA max num packets allocated to channel 3 */
#define ENET_DMA_RX_CH3_NUM_PKTS        (8U)

/*! Task stack size */
#define ENETCli_TASK_STACK_SZ           (10U * 1024U)

/*! Each variable on the stack is going to start from a byte that is 
*   a multiple of ENETCli_TASK_STACK_ALIGN/8 bytes from the 
*   current stack pointer */
#define ENETCli_TASK_STACK_ALIGN        (32U)

/*! 100-ms periodic tick */
#define ENETAPP_PERIODIC_TICK_MS        (100U)

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/*! Structure with parameters of an Ethernet object */
typedef struct EnetApp_Obj_s
{
    /*! Ethernet Peripheral Type */
    Enet_Type enetType;

    /*! Instance Id which is core specific */
    uint32_t instId;

    /*! Core ID for which code is built */
    uint32_t coreId;

    /*! Core Key */
    uint32_t coreKey;

    /*! Different Enet Board IDs (GESI_ID, QPNET_ID etc) */
    uint32_t boardId[ENET_MAC_PORT_NUM];

    /*! The expansion port for the baseboard */
    uint32_t expPort[ENET_MAC_PORT_NUM];

    /*! Number of mac ports depending on the peripheral type */
    uint8_t numMacPorts;

    /*! Array holds the type of mac ports
    *  ie ENET_MAC_PORT_1, ENET_MAC_PORT_2 and so on */
    Enet_MacPort macPort[ENET_MAC_PORT_NUM];

    /*! The MII Mode for each of the macports defined above */
    emac_mode macMode[ENET_MAC_PORT_NUM];
    
    /*! The handle to the Enet Driver */
    Enet_Handle hEnet;

    /*! MAC address for the Host Port */
    uint8_t hostMacAddr[ENET_MAC_ADDR_LEN];

    /*! Array which holds the state of tx dma channel
    *   ie. whether it's idle or open. */
    uint8_t txDmaCh[ENET_TX_CHANNELS_NUM];

    /*! Array which holds the state of rx dma channel
    *   ie. whether it's idle or open. */
    uint8_t rxDmaCh[ENET_RX_FLOWS_NUM];

    /*! Flowid useful for opening/closing
    *   rxFlows for a particular channel */
    uint32_t rxFlowIdx;

    /*! Startid useful for opening/closing
    *   rxFlows for a particular channel */
    uint32_t rxStartFlowIdx;
    
    /*! Handle for UDMA driver */
    Udma_DrvHandle hUdmaDrv;
    
    /*! Packet transmission */
    /*! Array of Tx channel handles */
    EnetDma_TxChHandle hTxCh[ENET_TX_CHANNELS_NUM];
    
    /*! Array of Tx free queues */
    EnetDma_PktQ txFreePktInfoQ[ENET_TX_CHANNELS_NUM];

    /*! Array of semaphore object handles */
    SemaphoreP_Handle txSemObj[ENET_TX_CHANNELS_NUM];

    /*! Packet reception */
    /*! Array of Rx flow handles */
    EnetDma_RxChHandle hRxCh[ENET_RX_FLOWS_NUM];

    /*! Array of Rx free queues */
    EnetDma_PktQ rxFreeQ[ENET_RX_FLOWS_NUM];

    /*! Array of Rx ready queues */
    EnetDma_PktQ rxReadyQ[ENET_RX_FLOWS_NUM];

    /*! Array of semaphore object handles */
    SemaphoreP_Handle rxSemObj[ENET_RX_FLOWS_NUM];

    /*! The Tx channel number */
    uint32_t txChNum;
    
    /*! Status flag for initialization */
    int8_t initFlag;

    /*! Status flag for tsn related business */
    int8_t tsnFlag;

    /*! Status flag for lwip shell */
    int8_t shellFlag;

    /*! Test runtime params */
    volatile bool exitFlag; /* Exit test? */

    /*! Periodic tick for timer */
    ClockP_Handle hTickTimer;

    /*! Periodic tick for task */
    TaskP_Handle hTickTask;

    /*! Periodic tick for semaphore */
    SemaphoreP_Handle hTimerSem;

    /*! Packet transmission */
    /*! Handle for a TxTask */
    TaskP_Handle hTxTask;

    /*! Handle for a semaphore */
    SemaphoreP_Handle hTxDoneSem;

    /*! Count of total tx channels */
    uint32_t totalTxCnt;

    /*! Packet reception */
    /*! Handle to RxTask */
    TaskP_Handle hRxTask;

    /*! Handle to semaphore */
    SemaphoreP_Handle hRxDoneSem;

    /*! Count of toal rx flows */
    uint32_t totalRxCnt;

} EnetApp_Obj;

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/*! Initializes semaphores and creates timer params */
void EnetApp_createClock(void);

/*! Deletes semaphores and creates timer params */
void EnetApp_deleteClock(void);

/*! Function to read user input from the terminal */
void Enetcli_read(char *rxBuffer, uint32_t rxBufferLen);

/*! Function to display string on the terminal */
void Enetcli_write(char *txBuffer);

/*! Starts periodic tick task */
void EnetApp_tickTask(void *a0, void *a1);

/*! Posts the semaphore */
void EnetApp_timerCallback(void *arg);

/*! Opens Tx channel and Rx flow */
int32_t EnetApp_openDma();

/*! Closes Tx channel and Rx flow */
void EnetApp_closeDma();

/*! Posts the semaphore */
void EnetApp_rxIsrFxn(void *appData);

/*! Posts the semaphore */
void EnetApp_txIsrFxn(void *appData);

/*! Set the Rx and Tx task handles to NULL */
void EnetApp_deleteRxTxTasks(void);

/*! Initialize ready packet queue and descriptors*/
void EnetApp_initRxReadyPktQ(uint8_t dmaChNum);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/*! Enet object instance declaration */
extern EnetApp_Obj EnetApp_inst;

/*! Number of packets per tx channel */
int32_t EnetApp_pktPerTxCh[ENET_TX_CHANNELS_NUM];

/*! Number of packets per rx channel */
int32_t EnetApp_pktPerRxCh[ENET_RX_FLOWS_NUM];

#ifdef __cplusplus
}
#endif

#endif /* _CLI_COMMON_H_ */
