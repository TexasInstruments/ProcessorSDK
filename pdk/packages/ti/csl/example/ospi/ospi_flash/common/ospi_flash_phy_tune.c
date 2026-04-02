/*
 * Copyright (c) 2020, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
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
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <ti/drv/uart/UART_stdio.h>
#include "ospi_flash_patterns.h"
#include "ospi_flash_phy_tune.h"

#undef OSPI_FLASH_SPI_TUNE_DEBUG
#undef OSPI_FLASH_DISABLE_TXDLL_WINDOW

#ifdef OSPI_FLASH_SPI_TUNE_DEBUG
#define OspiFlash_log UART_printf
uint32_t ospiFlashSpiTuneCnt = 0;
#endif

static OspiFlash_PhyConfig ddrTuningPoint = {0, };
static OspiFlash_PhyConfig sdrTuningPoint = {0, };

static void OspiFlash_spiRdDelayConfig(uint32_t rdDelay)
{
    CSL_ospiSetDataReadCapDelay((const CSL_ospi_flash_cfgRegs *)(OSPI_FLASH_CONFIG_REG_BASE_ADDR),rdDelay);
}

static void OspiFlash_spiTxRxDllConfig(uint32_t txDLL, uint32_t rxDLL, uint32_t funcClk)
{
    uint32_t    phyOpMode = CSL_OSPI_CFG_PHY_OP_MODE_MASTER;
    CSL_ospiSetPreScaler((const CSL_ospi_flash_cfgRegs *)(OSPI_FLASH_CONFIG_REG_BASE_ADDR), CSL_OSPI_BAUD_RATE_DIVISOR(2U));
    CSL_ospiPhyEnable((const CSL_ospi_flash_cfgRegs *)(OSPI_FLASH_CONFIG_REG_BASE_ADDR), TRUE);
    CSL_ospiConfigPhyDLL((const CSL_ospi_flash_cfgRegs *)(OSPI_FLASH_CONFIG_REG_BASE_ADDR), txDLL, rxDLL, phyOpMode, 
                          CSL_OSPI_CFG_PHY_DLL_MODE_DEFAULT, funcClk);
}

static void OspiFlash_spiPhyConfig(OspiFlash_PhyConfig phyConfig, uint32_t funcClk)
{
    OspiFlash_spiRdDelayConfig(phyConfig.rdDelay);
    OspiFlash_spiTxRxDllConfig(phyConfig.txDLL, phyConfig.rxDLL, funcClk);
}

static uint32_t rdBuf[OSPI_FLASH_ATTACK_VECTOR_SIZE/sizeof(uint32_t)];
static int32_t OspiFlash_spiPhyRdAttack(uintptr_t flashVectorAddr)
{
    int32_t        status = OSPI_FLASH_APP_STATUS_SUCCESS;
    uint32_t          i;
    volatile uint8_t *pByte = (volatile uint8_t *)rdBuf;

    for (i = 0; i < OSPI_FLASH_ATTACK_VECTOR_SIZE/sizeof(uint32_t); i++)
    {
        rdBuf[i] = CSL_REG32_RD(flashVectorAddr + i * 4);
    }

    for (i = 0; i < OSPI_FLASH_ATTACK_VECTOR_SIZE; i++)
    {
        if (pByte[i] != ospi_flash_attack_vector[i])
        {
            status = OSPI_FLASH_APP_STATUS_ERROR;
            break;
        }
    }
    return (status);
}

/*
 * Searches txDLL down from start until the tuning basis passes.
 * Does not look at the next rdDelay setting.  Returns txDLL=128 if fail.
 */
OspiFlash_PhyConfig OspiFlash_spiPhyFindTxHigh(OspiFlash_PhyConfig start, uint32_t offset, uint32_t funcClk)
{
    int32_t             status;

    OspiFlash_spiPhyConfig(start, funcClk);
    status = OspiFlash_spiPhyRdAttack(OSPI_FLASH_DATA_BASE_ADDR + offset);
#ifdef OSPI_FLASH_SPI_TUNE_DEBUG
    ospiFlashSpiTuneCnt++;
#endif
    while(status == OSPI_FLASH_APP_STATUS_ERROR)
    {
        start.txDLL--;
        if(start.txDLL < 48U)
        {
            start.txDLL = 128U;
            break;
        }
        OspiFlash_spiPhyConfig(start, funcClk);
        status = OspiFlash_spiPhyRdAttack(OSPI_FLASH_DATA_BASE_ADDR + offset);
#ifdef OSPI_FLASH_SPI_TUNE_DEBUG
        ospiFlashSpiTuneCnt++;
#endif
    }
    return start;
}

/*
 * Searches txDLL up from start until the tuning basis passes.
 * Does not look at the next rdDelay setting.  Returns txDLL=128 if fail.
 */
OspiFlash_PhyConfig OspiFlash_spiPhyFindTxLow(OspiFlash_PhyConfig start, uint32_t offset, uint32_t funcClk)
{
    int32_t  status;

    OspiFlash_spiPhyConfig(start, funcClk);
    status = OspiFlash_spiPhyRdAttack(OSPI_FLASH_DATA_BASE_ADDR + (uintptr_t)offset);
#ifdef OSPI_FLASH_SPI_TUNE_DEBUG
    ospiFlashSpiTuneCnt++;
#endif
    while(status == OSPI_FLASH_APP_STATUS_ERROR)
    {
        start.txDLL++;
        if (start.txDLL > 32U)
        {
            start.txDLL = 128U;
            break;
        }
        OspiFlash_spiPhyConfig(start, funcClk);
        status = OspiFlash_spiPhyRdAttack(OSPI_FLASH_DATA_BASE_ADDR + offset);
#ifdef OSPI_FLASH_SPI_TUNE_DEBUG
        ospiFlashSpiTuneCnt++;
#endif
    }
    return start;
}

/*
 * Searches rxDLL down from start until the tuning basis passes.
 * Does not look at the next rdDelay setting.  Returns rxDLL=128 if fail.
 */
OspiFlash_PhyConfig OspiFlash_spiPhyFindRxHigh(OspiFlash_PhyConfig start, uint32_t offset, uint32_t funcClk)
{
    int32_t  status;

    OspiFlash_spiPhyConfig(start, funcClk);
    status = OspiFlash_spiPhyRdAttack(OSPI_FLASH_DATA_BASE_ADDR + offset);
#ifdef OSPI_FLASH_SPI_TUNE_DEBUG
    ospiFlashSpiTuneCnt++;
#endif
    while(status == OSPI_FLASH_APP_STATUS_ERROR)
    {
        start.rxDLL--;
        if(start.rxDLL < 25U)
        {
            start.rxDLL = 128U;
            break;
        }
        OspiFlash_spiPhyConfig(start, funcClk);
        status = OspiFlash_spiPhyRdAttack(OSPI_FLASH_DATA_BASE_ADDR + offset);
#ifdef OSPI_FLASH_SPI_TUNE_DEBUG
        ospiFlashSpiTuneCnt++;
#endif
    }
    return start;
}

/*
 * Searches rxDLL up from start until the tuning basis passes.
 * Does not look at the next rdDelay setting. Returns rxDLL=128 if fail.
 */
OspiFlash_PhyConfig OspiFlash_spiPhyFindRxLow(OspiFlash_PhyConfig start, uint32_t offset, uint32_t funcClk)
{
    int32_t  status;

    OspiFlash_spiPhyConfig(start, funcClk);
    status = OspiFlash_spiPhyRdAttack(OSPI_FLASH_DATA_BASE_ADDR + offset);
#ifdef OSPI_FLASH_SPI_TUNE_DEBUG
    ospiFlashSpiTuneCnt++;
#endif
    while(status == OSPI_FLASH_APP_STATUS_ERROR)
    {
        start.rxDLL++;
        if(start.rxDLL > 10U)
        {
            start.rxDLL = 128U;
            break;
        }
        OspiFlash_spiPhyConfig(start, funcClk);
        status = OspiFlash_spiPhyRdAttack(OSPI_FLASH_DATA_BASE_ADDR + offset);
#ifdef OSPI_FLASH_SPI_TUNE_DEBUG
        ospiFlashSpiTuneCnt++;
#endif
    }
    return start;
}

double OspiFlash_spiPhyAvgVtmTemp(double vtm125){
    double   avg = 0;
    double   m = 0;
    double   b = 0;
    uint32_t temp;
    uint32_t j;
    uint32_t statReg; /* VTM temperature sensor status register addr */
    uint32_t ctrlReg; /* VTM temperature sensor control register addr */

    statReg = OSPI_FLASH_STATUS_REG;
    ctrlReg = OSPI_FLASH_CONTROL_REG;

    /* Take the average VTM value */
    for (j = 0; j < 5U; j++)
    {
        uintptr_t pCtrl = (uintptr_t)(ctrlReg + (j * 0x20U));
        uintptr_t pStat = (uintptr_t)(statReg + (j * 0x20U));

        /* Setting sensor to continous readout mode. */
        CSL_REG32_WR(pCtrl, (CSL_REG32_RD(pCtrl) & ~0x10U) | (1U << 4U));
        CSL_REG32_WR(pCtrl, (CSL_REG32_RD(pCtrl) & ~0x10U) | (1U << 4U));

        /* Read from pStat register to get temp */
        temp = CSL_REG32_RD(pStat) & 0x3FFU;

        /* Accumlate a number to average */
        avg += temp;
    }
    avg=avg/5U;
    /* Convert to a temperature */
    m = 160U/(vtm125-43U);
    b = (125U/m)-vtm125;
    avg = m*(avg+b);

    return avg;
}

/* Fast tuning in DDR mode (DQS enabled) */
int32_t OspiFlash_spiPhyDdrTune(uint32_t offset, uint32_t funcClk)
{
    int32_t             status;
    OspiFlash_PhyConfig          searchPoint;
    OspiFlash_PhyConfig          bottomLeft;
    OspiFlash_PhyConfig          topRight;
    OspiFlash_PhyConfig          gapLow;
    OspiFlash_PhyConfig          gapHigh;
    uint32_t                     maxSearchRange = 63U;
    OspiFlash_PhyConfig          rxLow, rxHigh, txLow, txHigh, temp;
    int32_t                rdDelay;
    float                  temperature = 0;
    float                  m,b,length1,length2;

    /*
     * Bottom left corner is present in initial rdDelay value and search from there.
     * Can be adjusted to save time.
     */
    rdDelay = 1U;
 
    const CSL_ospi_flash_cfgRegs *pRegs = (const CSL_ospi_flash_cfgRegs *)(OSPI_FLASH_CONFIG_REG_BASE_ADDR);

    /*
     * Acquire half clock lock, and update search range to 127
    */
    CSL_ospiPhyResyncDll(pRegs, CSL_OSPI_CFG_PHY_DLL_MODE_DEFAULT);
    uint32_t dll_lock_mode = CSL_REG32_FEXT(&pRegs->DLL_OBSERVABLE_LOWER_REG,
                                   OSPI_FLASH_CFG_DLL_OBSERVABLE_LOWER_REG_DLL_OBSERVABLE_LOWER_LOCK_MODE_FLD);
    if(dll_lock_mode != 0U)
    {
        maxSearchRange = 127U;
    }

    /*
     * Finding RxDLL fails at some of the TxDLL values based on the HW platform.
     * A window of TxDLL values is used to find the RxDLL without errors.
     * This can increase the number of CPU cycles taken for the PHY tuning
     * in the cases where more TxDLL values need to be parsed to find a stable RxDLL.
     *
     * Update OSPI_FLASH_SPI_PHY_TXDLL_LOW_WINDOW_START based on the TxDLL value where
     * stable RxDLL can be found consistently for a given platform and
     * define the macro OSPI_FLASH_DISABLE_TXDLL_WINDOW after fixing TxDLL value
     * to reduce the time taken for PHY tuning.
     */
#if !defined(OSPI_FLASH_DISABLE_TXDLL_WINDOW)
    /* Look for rxDLL boundaries at a txDLL Window to find rxDLL Min */
    searchPoint.txDLL = OSPI_FLASH_SPI_PHY_TXDLL_LOW_WINDOW_START;
    while(searchPoint.txDLL <= OSPI_FLASH_SPI_PHY_TXDLL_LOW_WINDOW_END)
    {
        searchPoint.rdDelay = rdDelay;
        searchPoint.rxDLL   = 0;
        rxLow = OspiFlash_spiPhyFindRxLow(searchPoint, offset, funcClk);
        while(rxLow.rxDLL == 128U)
        {
            searchPoint.rdDelay++;
            if(searchPoint.rdDelay > 4U)
            {
                if(searchPoint.txDLL >= OSPI_FLASH_SPI_PHY_TXDLL_LOW_WINDOW_END)
                {
#ifdef OSPI_FLASH_SPI_TUNE_DEBUG
                    OspiFlash_log("Unable to find RX Min\n");
#endif
                    return OSPI_FLASH_APP_STATUS_ERROR;
                }
                else
                {
                    break;
                }
            }
            rxLow = OspiFlash_spiPhyFindRxLow(searchPoint, offset, funcClk);
        }

        if(rxLow.rxDLL != 128U)
        {
            break;
        }

        searchPoint.txDLL++;
    }
#else
    /* Look for rxDLL boundaries at txDLL = 16 to find rxDLL Min */
    searchPoint.rdDelay = rdDelay;
    searchPoint.txDLL = OSPI_FLASH_SPI_PHY_TXDLL_LOW_WINDOW_START;
    searchPoint.rxDLL = 0U;
    rxLow = OspiFlash_spiPhyFindRxLow(searchPoint, offset);
    while(rxLow.rxDLL == 128U)
    {
        searchPoint.rdDelay++;
        if(searchPoint.rdDelay > 4U)
        {
#ifdef OSPI_FLASH_SPI_TUNE_DEBUG
            OspiFlash_log("Unable to find RX Min\n");
#endif
            return OSPI_FLASH_APP_STATUS_ERROR;
        }
        rxLow = OspiFlash_spiPhyFindRxLow(searchPoint, offset);
    }
#endif  /* #if !defined(OSPI_FLASH_DISABLE_TXDLL_WINDOW) */

    /* Find rxDLL Max at a txDLL */
    searchPoint.rxDLL = maxSearchRange;
    rxHigh = OspiFlash_spiPhyFindRxHigh(searchPoint, offset, funcClk);
    while(rxHigh.rxDLL == 128U)
    {
        searchPoint.rdDelay++;
        if(searchPoint.rdDelay > 4U)
        {
#ifdef OSPI_FLASH_SPI_TUNE_DEBUG
            OspiFlash_log("Unable to find RX Min\n");
#endif
            return OSPI_FLASH_APP_STATUS_ERROR;
        }
        rxHigh = OspiFlash_spiPhyFindRxHigh(searchPoint, offset, funcClk);
    }

    /*
     * Check a different point if the rxLow and rxHigh are on the same rdDelay.
     * This avoids mistaking the metastability gap for an rxDLL boundary
     */
    if (rxLow.rdDelay == rxHigh.rdDelay)
    {
#ifdef OSPI_FLASH_SPI_TUNE_DEBUG
        OspiFlash_log("rxLow and rxHigh are on the same rdDelay\n");
#endif

    /*
     * Finding RxDLL fails at some of the TxDLL values based on the HW platform.
     * A window of TxDLL values is used to find the RxDLL without errors.
     * This can increase the number of CPU cycles taken for the PHY tuning
     * in the cases where more TxDLL values need to be parsed to find a stable RxDLL.
     *
     * Update OSPI_FLASH_SPI_PHY_TXDLL_HIGH_WINDOW_START based on the TxDLL value where
     * stable RxDLL can be found consistently for a given platform and
     * define the macro OSPI_FLASH_DISABLE_TXDLL_WINDOW after fixing TxDLL value
     * to reduce the time taken for PHY tuning.
     */
#if !defined(OSPI_FLASH_DISABLE_TXDLL_WINDOW)
        /* Look for rxDLL boundaries at a txDLL Window */
        searchPoint.txDLL = OSPI_FLASH_SPI_PHY_TXDLL_HIGH_WINDOW_START;

        /* Find rxDLL Min */
        while(searchPoint.txDLL >= OSPI_FLASH_SPI_PHY_TXDLL_HIGH_WINDOW_END)
        {
            searchPoint.rdDelay = rdDelay;
            searchPoint.rxDLL   = 0;
            temp = OspiFlash_spiPhyFindRxLow(searchPoint, offset, funcClk);
            while(temp.rxDLL == 128U)
            {
                searchPoint.rdDelay++;
                if(searchPoint.rdDelay > 4U)
                {
                    if(searchPoint.txDLL <= OSPI_FLASH_SPI_PHY_TXDLL_HIGH_WINDOW_END)
                    {
#ifdef OSPI_FLASH_SPI_TUNE_DEBUG
                        OspiFlash_log("Unable to find RX Min\n");
#endif
                        return OSPI_FLASH_APP_STATUS_ERROR;
                    }
                    else
                    {
                        break;
                    }
                }
                temp = OspiFlash_spiPhyFindRxLow(searchPoint, offset, funcClk);
            }

            if(temp.rxDLL != 128U)
            {
                break;
            }

            searchPoint.txDLL--;
        }
#else
        /* Look for rxDLL boundaries at txDLL=48 */
        searchPoint.rdDelay = rdDelay;
        searchPoint.txDLL = OSPI_FLASH_SPI_PHY_TXDLL_HIGH_WINDOW_START;
        searchPoint.rxDLL = 0;

        /* Find rxDLL Min */
        temp = OspiFlash_spiPhyFindRxLow(searchPoint, offset, funcClk);
        while(temp.rxDLL == 128U)
        {
            searchPoint.rdDelay++;
            if(searchPoint.rdDelay > 4U)
            {
#ifdef OSPI_FLASH_SPI_TUNE_DEBUG
                OspiFlash_log("Unable to find RX Min\n");
#endif
                return OSPI_FLASH_APP_STATUS_ERROR;
            }
            temp = OspiFlash_spiPhyFindRxLow(searchPoint, offset, funcClk);
        }
#endif

        if(temp.rxDLL<rxLow.rxDLL){
            rxLow = temp;
        }

        /* Find rxDLL Max */
        searchPoint.rxDLL = maxSearchRange;
        temp = OspiFlash_spiPhyFindRxHigh(searchPoint, offset, funcClk);
        while(temp.rxDLL == 128U)
        {
            searchPoint.rdDelay++;
            if(searchPoint.rdDelay > 4U)
            {
#ifdef OSPI_FLASH_SPI_TUNE_DEBUG
                OspiFlash_log("Unable to find RX Max\n");
#endif
                return OSPI_FLASH_APP_STATUS_ERROR;
            }
            temp = OspiFlash_spiPhyFindRxHigh(searchPoint, offset, funcClk);
        }
        if(temp.rxDLL > rxHigh.rxDLL)
        {
            rxHigh = temp;
        }
    }

    /*
     * Look for txDLL boundaries at 1/4 of rxDLL window
     * Find txDLL Min
     */
    searchPoint.rdDelay = rdDelay;
    searchPoint.rxDLL = (rxHigh.rxDLL+rxLow.rxDLL)/4U;
    searchPoint.txDLL = 0U;
    txLow = OspiFlash_spiPhyFindTxLow(searchPoint,offset,funcClk);
    while(txLow.txDLL==128U){
        searchPoint.rdDelay++;
        txLow = OspiFlash_spiPhyFindTxLow(searchPoint,offset,funcClk);
        if(searchPoint.rdDelay>4U){
#ifdef OSPI_FLASH_SPI_TUNE_DEBUG
            OspiFlash_log("Unable to find TX Min\n");
#endif
            return OSPI_FLASH_APP_STATUS_ERROR;
        }
    }

    /* Find txDLL Max */
    searchPoint.txDLL = maxSearchRange;
    txHigh = OspiFlash_spiPhyFindTxHigh(searchPoint,offset,funcClk);
    while(txHigh.txDLL==128U){
        searchPoint.rdDelay++;
        txHigh = OspiFlash_spiPhyFindTxHigh(searchPoint,offset,funcClk);
        if(searchPoint.rdDelay>4U){
#ifdef OSPI_FLASH_SPI_TUNE_DEBUG
            OspiFlash_log("Unable to find TX Max\n");
#endif
            return OSPI_FLASH_APP_STATUS_ERROR;
        }
    }

    /*
     * Check a different point if the txLow and txHigh are on the same rdDelay.
     * This avoids mistaking the metastability gap for an rxDLL boundary
     */
    if(txLow.rdDelay==txHigh.rdDelay){
        /* Look for txDLL boundaries at 3/4 of rxDLL window
           Find txDLL Min */
        searchPoint.rdDelay = rdDelay;
        searchPoint.rxDLL = 3U*(rxHigh.rxDLL+rxLow.rxDLL)/4U;
        searchPoint.txDLL = 0U;
        temp = OspiFlash_spiPhyFindTxLow(searchPoint,offset,funcClk);
        while(temp.txDLL==128U){
            searchPoint.rdDelay++;
            temp = OspiFlash_spiPhyFindTxLow(searchPoint,offset,funcClk);
            if(searchPoint.rdDelay>4U){
#ifdef OSPI_FLASH_SPI_TUNE_DEBUG
                OspiFlash_log("Unable to find TX Min\n");
#endif
                return OSPI_FLASH_APP_STATUS_ERROR;
            }
        }
        if(temp.txDLL<txLow.txDLL){
            txLow = temp;
        }

        /* Find txDLL Max */
        searchPoint.txDLL = maxSearchRange;
        temp = OspiFlash_spiPhyFindTxHigh(searchPoint,offset,funcClk);
        while(temp.txDLL==128U){
            searchPoint.rdDelay++;
            temp = OspiFlash_spiPhyFindTxHigh(searchPoint,offset,funcClk);
            if(searchPoint.rdDelay>4U){
#ifdef OSPI_FLASH_SPI_TUNE_DEBUG
                OspiFlash_log("Unable to find TX Max\n");
#endif
                return OSPI_FLASH_APP_STATUS_ERROR;
            }
        }
        if(temp.txDLL>txHigh.txDLL){
            txHigh = temp;
        }
    }

    /*
     * Set bottom left and top right right corners.  These are theoretical corners. They may not actually be "good" points.
     * But the longest diagonal of the shmoo will be between these corners.
     */
    /* Bottom Left */
    bottomLeft.txDLL = txLow.txDLL;
    bottomLeft.rxDLL = rxLow.rxDLL;
    if(txLow.rdDelay<=rxLow.rdDelay){
        bottomLeft.rdDelay = txLow.rdDelay;
    }else bottomLeft.rdDelay = rxLow.rdDelay;
    temp = bottomLeft;
    temp.txDLL += 4U;
    temp.rxDLL += 4U;
    OspiFlash_spiPhyConfig(temp, funcClk);
    status = OspiFlash_spiPhyRdAttack(OSPI_FLASH_DATA_BASE_ADDR + offset);
#ifdef OSPI_FLASH_SPI_TUNE_DEBUG
    ospiFlashSpiTuneCnt++;
#endif
    if(status == OSPI_FLASH_APP_STATUS_ERROR){
        temp.rdDelay--;
        OspiFlash_spiPhyConfig( temp, funcClk);
        status = OspiFlash_spiPhyRdAttack(OSPI_FLASH_DATA_BASE_ADDR + offset);
#ifdef OSPI_FLASH_SPI_TUNE_DEBUG
        ospiFlashSpiTuneCnt++;
#endif
    }
    if (status == OSPI_FLASH_APP_STATUS_SUCCESS){
        bottomLeft.rdDelay = temp.rdDelay;
    }

    /* Top Right */
    topRight.txDLL = txHigh.txDLL;
    topRight.rxDLL = rxHigh.rxDLL;
    if(txHigh.rdDelay>=rxHigh.rdDelay){
        topRight.rdDelay = txHigh.rdDelay;
    }else topRight.rdDelay = rxHigh.rdDelay;
    temp = topRight;
    temp.txDLL -= 4U;
    temp.rxDLL -= 4U;
    OspiFlash_spiPhyConfig(temp, funcClk);
    status = OspiFlash_spiPhyRdAttack(OSPI_FLASH_DATA_BASE_ADDR + offset);
#ifdef OSPI_FLASH_SPI_TUNE_DEBUG
    ospiFlashSpiTuneCnt++;
#endif
    if(status == OSPI_FLASH_APP_STATUS_ERROR){
        temp.rdDelay++;
        OspiFlash_spiPhyConfig(temp, funcClk);
        status = OspiFlash_spiPhyRdAttack(OSPI_FLASH_DATA_BASE_ADDR + offset);
#ifdef OSPI_FLASH_SPI_TUNE_DEBUG
        ospiFlashSpiTuneCnt++;
#endif
    }
    if(status == OSPI_FLASH_APP_STATUS_SUCCESS){
        topRight.rdDelay = temp.rdDelay;
    }

    /* Draw a line between the two */
    m = ((float)topRight.rxDLL-(float)bottomLeft.rxDLL)/((float)topRight.txDLL-(float)bottomLeft.txDLL);
    b = (float)topRight.rxDLL-m*(float)topRight.txDLL;

    /* Search along line between the corners */
    searchPoint = bottomLeft;
    do{
        OspiFlash_spiPhyConfig(searchPoint, funcClk);
        status = OspiFlash_spiPhyRdAttack(OSPI_FLASH_DATA_BASE_ADDR + offset);
#ifdef OSPI_FLASH_SPI_TUNE_DEBUG
        ospiFlashSpiTuneCnt++;
#endif
        searchPoint.txDLL+=1U;
        searchPoint.rxDLL = (int32_t)(m*searchPoint.txDLL+b);
    }while(status == OSPI_FLASH_APP_STATUS_ERROR);

    do{
        OspiFlash_spiPhyConfig(searchPoint, funcClk);
        status = OspiFlash_spiPhyRdAttack(OSPI_FLASH_DATA_BASE_ADDR + offset);
#ifdef OSPI_FLASH_SPI_TUNE_DEBUG
        ospiFlashSpiTuneCnt++;
#endif
        searchPoint.txDLL+=1U;
        searchPoint.rxDLL = (int)(m*searchPoint.txDLL+b);
    }while(status == OSPI_FLASH_APP_STATUS_SUCCESS);

    searchPoint.txDLL-=1U;
    searchPoint.rxDLL = (int)(m*searchPoint.txDLL+b);
    gapLow = searchPoint;

    /* If there's only one segment, put tuning point in the middle and adjust for temperature */
    if(bottomLeft.rdDelay==topRight.rdDelay){
        //The "true" top right corner was too small to find, so the start of the metastability gap is a good approximation
        topRight = gapLow;
        searchPoint.rdDelay = bottomLeft.rdDelay;
        searchPoint.txDLL = (bottomLeft.txDLL+topRight.txDLL)/2U;
        searchPoint.rxDLL = (bottomLeft.rxDLL+topRight.rxDLL)/2U;
#ifdef OSPI_FLASH_SPI_TUNE_DEBUG
        OspiFlash_log("Only one passing window found, from txDLL,rxDLL of %d,%d to %d,%d, and a rdDelay of %d\n",bottomLeft.txDLL,bottomLeft.rxDLL,topRight.txDLL,topRight.rxDLL,topRight.rdDelay);
#endif
        temperature = OspiFlash_spiPhyAvgVtmTemp(OSPI_FLASH_SPI_PHY_VTM_TARGET);
        searchPoint.txDLL+= (topRight.txDLL-bottomLeft.txDLL)*(0.5*(temperature-42.5)/165U);
        searchPoint.rxDLL+= (topRight.rxDLL-bottomLeft.rxDLL)*(0.5*(temperature-42.5)/165U);
    }else{
        /* If there are two segments, find the start and end of the second one */
        searchPoint = topRight;
        do{
            OspiFlash_spiPhyConfig(searchPoint, funcClk);
            status = OspiFlash_spiPhyRdAttack(OSPI_FLASH_DATA_BASE_ADDR + offset);
#ifdef OSPI_FLASH_SPI_TUNE_DEBUG
            ospiFlashSpiTuneCnt++;
#endif
            searchPoint.txDLL-=1;
            searchPoint.rxDLL = (int)(m*searchPoint.txDLL+b);
        }while(status == OSPI_FLASH_APP_STATUS_ERROR);

        do{
            OspiFlash_spiPhyConfig(searchPoint, funcClk);
            status = OspiFlash_spiPhyRdAttack(OSPI_FLASH_DATA_BASE_ADDR + offset);
#ifdef OSPI_FLASH_SPI_TUNE_DEBUG
            ospiFlashSpiTuneCnt++;
#endif
            searchPoint.txDLL-=1U;
            searchPoint.rxDLL = (int)(m*searchPoint.txDLL+b);
        }while(status == OSPI_FLASH_APP_STATUS_SUCCESS);

        searchPoint.txDLL+=1U;
        searchPoint.rxDLL = (int)(m*searchPoint.txDLL+b);
        gapHigh = searchPoint;
        /* Place the final tuning point of the PHY in the corner furthest from the gap */
        length1 = abs(gapLow.txDLL-bottomLeft.txDLL) + abs(gapLow.rxDLL-bottomLeft.rxDLL);
        length2 = abs(gapHigh.txDLL-topRight.txDLL) + abs(gapHigh.rxDLL-topRight.rxDLL);
        if(length2>length1){
            searchPoint = topRight;
            searchPoint.txDLL-=16U;
            searchPoint.rxDLL-= 16U*m;
        }else{
            searchPoint = bottomLeft;
            searchPoint.txDLL+=16U;
            searchPoint.rxDLL+=16U*m;
        }
#ifdef OSPI_FLASH_SPI_TUNE_DEBUG
        OspiFlash_log("Bottom left found at txDLL,rxDLL of %d,%d to %d,%d, and a rdDelay of %d\n",bottomLeft.txDLL,bottomLeft.rxDLL,gapLow.txDLL,gapLow.rxDLL,gapLow.rdDelay);
        OspiFlash_log("Top Right found at txDLL,rxDLL of %d,%d to %d,%d, and a rdDelay of %d\n",gapHigh.txDLL,gapHigh.rxDLL,topRight.txDLL,topRight.rxDLL,gapHigh.rdDelay);
#endif
    }
#ifdef OSPI_FLASH_SPI_TUNE_DEBUG
    OspiFlash_log("Tuning was complete in %d steps\n", ospiFlashSpiTuneCnt);
    OspiFlash_log("Tuning PHY to txDLL,rxDLL of %d,%d and rdDelay of %d\n",searchPoint.txDLL,searchPoint.rxDLL,searchPoint.rdDelay);
#endif
    OspiFlash_spiPhyConfig(searchPoint, funcClk);

    /* Save SDR tuning config point data */
    ddrTuningPoint = searchPoint;

    return OSPI_FLASH_APP_STATUS_SUCCESS;
}

/* Returns the first rxDLL value which passes the tuning basis test, searching up from given txDLL,rxDLL */
static int32_t OspiFlash_spiPhyFindRxStart(int32_t txDLL, int32_t rxDLL, uint32_t offset, uint32_t funcClk)
{
    const CSL_ospi_flash_cfgRegs *pRegs = (const CSL_ospi_flash_cfgRegs *)(OSPI_FLASH_CONFIG_REG_BASE_ADDR);
    int32_t                       status;
    int32_t                       txSaved;
    int32_t                       rxSaved;

    txSaved = CSL_REG32_FEXT(&pRegs->PHY_CONFIGURATION_REG,
                             OSPI_FLASH_CFG_PHY_CONFIGURATION_REG_PHY_CONFIG_TX_DLL_DELAY_FLD);
    rxSaved = CSL_REG32_FEXT(&pRegs->PHY_CONFIGURATION_REG,
                             OSPI_FLASH_CFG_PHY_CONFIGURATION_REG_PHY_CONFIG_RX_DLL_DELAY_FLD);

    OspiFlash_spiTxRxDllConfig( txDLL, rxDLL, funcClk);
    status = OspiFlash_spiPhyRdAttack(OSPI_FLASH_CONFIG_REG_BASE_ADDR + offset);
#ifdef OSPI_FLASH_SPI_TUNE_DEBUG
    ospiFlashSpiTuneCnt++;
#endif
    if (status == OSPI_FLASH_APP_STATUS_SUCCESS)
        return rxDLL;
    while(status == OSPI_FLASH_APP_STATUS_ERROR){
        rxDLL++;
        if(rxDLL==128U) break;
        OspiFlash_spiTxRxDllConfig(txDLL,rxDLL,funcClk);
        status = OspiFlash_spiPhyRdAttack(OSPI_FLASH_DATA_BASE_ADDR + offset);
#ifdef OSPI_FLASH_SPI_TUNE_DEBUG
        ospiFlashSpiTuneCnt++;
#endif
    }
    OspiFlash_spiTxRxDllConfig(txSaved,rxSaved,funcClk);
    return rxDLL;
}

/*
 * Returns the last rxDLL value which passes the tuning basis test, searching up from given txDLL,rxDLL
 * Returns rxDLL passed into the function if failure
 */
static int32_t OspiFlash_spiPhyFindRxEnd(int32_t txDLL, int32_t rxDLL, uint32_t offset, uint32_t funcClk)
{
    const CSL_ospi_flash_cfgRegs *pRegs = (const CSL_ospi_flash_cfgRegs *)(OSPI_FLASH_CONFIG_REG_BASE_ADDR);
    int32_t                       status;
    int32_t                       startRx = rxDLL;
    int32_t                       txSaved;
    int32_t                       rxSaved;

    txSaved = CSL_REG32_FEXT(&pRegs->PHY_CONFIGURATION_REG,
                             OSPI_FLASH_CFG_PHY_CONFIGURATION_REG_PHY_CONFIG_TX_DLL_DELAY_FLD);
    rxSaved = CSL_REG32_FEXT(&pRegs->PHY_CONFIGURATION_REG,
                             OSPI_FLASH_CFG_PHY_CONFIGURATION_REG_PHY_CONFIG_RX_DLL_DELAY_FLD);

    OspiFlash_spiTxRxDllConfig(txDLL,rxDLL,funcClk);
    status = OspiFlash_spiPhyRdAttack(OSPI_FLASH_CONFIG_REG_BASE_ADDR + offset);
#ifdef OSPI_FLASH_SPI_TUNE_DEBUG
    ospiFlashSpiTuneCnt++;
#endif
    if(status == OSPI_FLASH_APP_STATUS_ERROR){
        OspiFlash_spiTxRxDllConfig(txSaved,rxSaved,funcClk);
        return startRx;
    }
    while (status == OSPI_FLASH_APP_STATUS_SUCCESS){
        rxDLL++;
        OspiFlash_spiTxRxDllConfig(txDLL,rxDLL,funcClk);
        status = OspiFlash_spiPhyRdAttack(OSPI_FLASH_DATA_BASE_ADDR + offset);
#ifdef OSPI_FLASH_SPI_TUNE_DEBUG
        ospiFlashSpiTuneCnt++;
#endif
    }
    OspiFlash_spiTxRxDllConfig(txSaved,rxSaved,funcClk);

    return rxDLL;
}


/* Tries to find an rxDLL window at a given txDLL point. Puts start and end of the rxDLL window into the pointers */
static int32_t OspiFlash_spiPhyFindRxWindow(int32_t txDLL, int32_t *rxStart, int32_t *rxEnd, uint32_t offset, uint32_t funcClk)
{
    /* Search up from 0 to find the start of the Rx window */
    *rxStart = OspiFlash_spiPhyFindRxStart(txDLL,0,offset,funcClk);

    /* If rxStart gets all the way to rxDLL=128 and doesn't pass, there is no rxDLL window */
    if(*rxStart == 128U)
        return OSPI_FLASH_APP_STATUS_ERROR;

    /* Find the end of the rxDLL window, searching up from rxStart */
    *rxEnd = OspiFlash_spiPhyFindRxEnd(txDLL,*rxStart+4U,offset, funcClk);

    /* If rxEnd is greater than rxStart, we found a window. */
    if(*rxEnd>*rxStart+4U)
        return OSPI_FLASH_APP_STATUS_SUCCESS;
    else
        return OSPI_FLASH_APP_STATUS_ERROR;
}

/*
 * Prints out DLL settings, and check the lock status bits
 *     0 = Full cycle
 *     2 = Half cycle
 *     3 = Failed to lock
 * if not full cycl locked, turn on the master PHY bypass mode
 */
static void OspiFlash_spiPhyDllObserve()
{
    const CSL_ospi_flash_cfgRegs *pRegs = (const CSL_ospi_flash_cfgRegs *)(OSPI_FLASH_CONFIG_REG_BASE_ADDR);
    uint32_t                      dll_lock_mode;

#ifdef OSPI_FLASH_SPI_TUNE_DEBUG
    uint32_t rx_decode, tx_decode, dll_lock_value, dll_lock_status;

    /* Parse the observable upper and lower registers for the bit fields */
    rx_decode = CSL_REG32_FEXT(&pRegs->DLL_OBSERVABLE_UPPER_REG,
                               OSPI_FLASH_CFG_DLL_OBSERVABLE_UPPER_REG_DLL_OBSERVABLE__UPPER_RX_DECODER_OUTPUT_FLD);
    tx_decode = CSL_REG32_FEXT(&pRegs->DLL_OBSERVABLE_UPPER_REG,
                               OSPI_FLASH_CFG_DLL_OBSERVABLE_UPPER_REG_DLL_OBSERVABLE_UPPER_TX_DECODER_OUTPUT_FLD);
    dll_lock_value = CSL_REG32_FEXT(&pRegs->DLL_OBSERVABLE_LOWER_REG,
                                    OSPI_FLASH_CFG_DLL_OBSERVABLE_LOWER_REG_DLL_OBSERVABLE_LOWER_LOCK_VALUE_FLD);
    dll_lock_mode = CSL_REG32_FEXT(&pRegs->DLL_OBSERVABLE_LOWER_REG,
                                   OSPI_FLASH_CFG_DLL_OBSERVABLE_LOWER_REG_DLL_OBSERVABLE_LOWER_LOCK_MODE_FLD);
    dll_lock_status = CSL_REG32_FEXT(&pRegs->DLL_OBSERVABLE_LOWER_REG,
                                     OSPI_FLASH_CFG_DLL_OBSERVABLE_LOWER_REG_DLL_OBSERVABLE_LOWER_LOOPBACK_LOCK_FLD);

    /* Print out lock status, the lock value, and the tx/rx decoded value */
    switch(dll_lock_mode)
    {
        case 0b00:
            OspiFlash_log("Decoded TX,RX is %d,%d\nDLL locked on full cycle with %d Delay elements\n",tx_decode,rx_decode,dll_lock_value);
            break;
        case 0b10:
            OspiFlash_log("Decoded TX,RX is %d,%d\nDLL locked on half cycle with %d Delay elements\n",tx_decode,rx_decode,dll_lock_value);
            break;
        case 0b11:
            OspiFlash_log("Decoded TX,RX is %d,%d\nDLL did not lock\n",tx_decode,rx_decode);
            break;
        default:
            break;
    }
    OspiFlash_log("lock mode is %d, lock status is %d, \n",dll_lock_mode,dll_lock_status);
#else
    dll_lock_mode = CSL_REG32_FEXT(&pRegs->DLL_OBSERVABLE_LOWER_REG,
                                   OSPI_FLASH_CFG_DLL_OBSERVABLE_LOWER_REG_DLL_OBSERVABLE_LOWER_LOCK_MODE_FLD);
#endif
    if(dll_lock_mode != 0U){
        /* Put DLL into bypass mode */
       CSL_REG32_FINS(&pRegs->PHY_MASTER_CONTROL_REG,
                      OSPI_FLASH_CFG_PHY_MASTER_CONTROL_REG_PHY_MASTER_BYPASS_MODE_FLD, 1U);
    }
}

/*
 * This is a fast tuning algorithm.
 * It takes in the handle for the ospi instance it must tune.
 * Assumes Protocol is SDR, clock mode is internal loopback and PHY is in DLL Master mode.
 */
int32_t OspiFlash_spiPhySdrTune(uint32_t offset, uint32_t funcClk)
{
    int32_t       rdDelay1 = 0;
    int32_t       rdDelay2;
    int32_t       rxStart1,rxEnd1;
    int32_t       rxStart2,rxEnd2;
    OspiFlash_PhyConfig startPoint, tuningPoint;
    float         rxWindow1 = 0;
    float         rxWindow2 = 0;
    float         temperature = 0;

    /* SDR tuning requires as much delay as possible. If locked on a half cycle, go into bypass mode */
    startPoint.txDLL = 16;
    startPoint.rxDLL = 16;
    startPoint.rdDelay = rdDelay1;

    OspiFlash_spiPhyConfig( startPoint, funcClk);
    /* Check if PHY DLL is locked */
    OspiFlash_spiPhyDllObserve();
    while(OspiFlash_spiPhyFindRxWindow( 64U, &rxStart1, &rxEnd1, offset, funcClk) == OSPI_FLASH_APP_STATUS_ERROR)
    {
        rdDelay1++;
        OspiFlash_spiRdDelayConfig( rdDelay1);
        if(rdDelay1>4){
#ifdef OSPI_FLASH_SPI_TUNE_DEBUG
            OspiFlash_log("Unable to find any rxDLL window.  Panic!\n");
#endif
            break;
        }
    }
    rxWindow1 = rxEnd1-rxStart1;
    rdDelay2 = rdDelay1+1;
    OspiFlash_spiRdDelayConfig( rdDelay2);
    if(OspiFlash_spiPhyFindRxWindow( 64U, &rxStart2, &rxEnd2, offset, funcClk) == OSPI_FLASH_APP_STATUS_SUCCESS)
    {
        rxWindow2 = rxEnd2-rxStart2;
    }else{
        rxWindow2 = 0;
    }

    temperature = OspiFlash_spiPhyAvgVtmTemp(OSPI_FLASH_SPI_PHY_VTM_TARGET);
    if(rxWindow2>rxWindow1){
        rdDelay1 = rdDelay2;
        rxWindow1 = rxWindow2;
        rxStart1 = rxStart2;
        rxEnd1 = rxEnd2;
    }
#ifdef OSPI_FLASH_SPI_TUNE_DEBUG
    OspiFlash_log("rxDLL Window of width %d found from %d to %d at txDLL of 64 and Read Delay of %d\n",(int)rxWindow1,rxStart1,rxEnd1,rdDelay1);
#endif
    tuningPoint.rdDelay = rdDelay1;
    tuningPoint.txDLL = 64U;
    tuningPoint.rxDLL = rxStart1;
    tuningPoint.rxDLL = (int)(((double)tuningPoint.rxDLL+rxWindow1/2U) - ((temperature-42.5)/165)*rxWindow1*0.75);
#ifdef OSPI_FLASH_SPI_TUNE_DEBUG
    OspiFlash_log("Tuning was complete in %d steps\n", ospiFlashSpiTuneCnt);
    OspiFlash_log("Tuning PHY to txDLL,rxDLL of %d,%d and rdDelay of %d\n",tuningPoint.txDLL,tuningPoint.rxDLL,tuningPoint.rdDelay);
#endif
    OspiFlash_spiPhyConfig( tuningPoint, funcClk);

    /* Save ddr tuning config point data */
    sdrTuningPoint = tuningPoint;

    return OSPI_FLASH_APP_STATUS_SUCCESS;
}

int32_t OspiFlash_spiPhyTune(bool ddrMode, uint32_t offset, uint32_t funcClk)
{
    const CSL_ospi_flash_cfgRegs *pRegs = (const CSL_ospi_flash_cfgRegs *)(OSPI_FLASH_CONFIG_REG_BASE_ADDR);
    int32_t  status = 0;

#ifdef OSPI_FLASH_SPI_TUNE_DEBUG
    OspiFlash_log("\n");
    OspiFlash_log("\n Fast Tuning at temperature %dC\n",(int)OspiFlash_spiPhyAvgVtmTemp(OSPI_FLASH_SPI_PHY_VTM_TARGET));
#endif
    CSL_REG32_FINS(&pRegs->DEV_INSTR_RD_CONFIG_REG,
                   OSPI_FLASH_CFG_DEV_INSTR_RD_CONFIG_REG_DUMMY_RD_CLK_CYCLES_FLD,
                   OSPI_FLASH_NOR_OCTAL_READ_DUMMY_CYCLE_DAC - 1U);
    if (ddrMode)
    {
        if ((ddrTuningPoint.txDLL == 0U) && (ddrTuningPoint.rxDLL == 0U))
        {
            /* process the PHY tuning only once */
            status = OspiFlash_spiPhyDdrTune( offset, funcClk);
        }
        else
        {
            OspiFlash_spiPhyConfig( ddrTuningPoint, funcClk);
        }
    }
    else
    {
        if ((sdrTuningPoint.txDLL == 0U) && (sdrTuningPoint.rxDLL == 0U))
        {
            /* process the PHY tuning only once */
            status = OspiFlash_spiPhySdrTune( offset, funcClk);
        }
        else
        {
            OspiFlash_spiPhyConfig( sdrTuningPoint, funcClk);
        }
    }

    CSL_ospiPipelinePhyEnable(pRegs, TRUE);

    return status;
}

void OspiFlash_spiPhyTuneReset(bool ddrMode)
{
    if (ddrMode == (bool)true)
    {
        ddrTuningPoint.txDLL   = 0;
        ddrTuningPoint.rxDLL   = 0;
        ddrTuningPoint.rdDelay = 0;
    }
    else
    {
        sdrTuningPoint.txDLL   = 0;
        sdrTuningPoint.rxDLL   = 0;
        sdrTuningPoint.rdDelay = 0;
    }
}
