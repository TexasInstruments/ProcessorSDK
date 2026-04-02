/******************************************************************************
 * Copyright (c) 2019 Texas Instruments Incorporated - http://www.ti.com
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
 *****************************************************************************/

/**
 *   \file    ds90ub9xx_tlc59108.c
 *
 *   \brief   This file contains the default API's for initializing the
 *            FPD module for the communicating with the remote slave device.
 *
 */

#include "ds90ub9xx_tlc59108.h"

/**
 * \brief  Board_ub941SerInit
 *
 * This function is used to initialize the serializer
 * connected to host controller.
 *
 * \param   handle        - pointer to the i2c handle
 * \param   fpdParams     – structure holding the fpd params.
 *
 * \return  BOARD_SOK in case of success or appropriate error code.
 */
Board_STATUS Board_fpdUb941SerInit(void *handle,
                                   Board_FpdModuleObj *fpdModParams,
                                   Board_FpdRmtDevObj *fpdRmtParams)
{
    Board_STATUS ret = BOARD_SOK;

    if(NULL == handle)
    {
        return BOARD_INVALID_PARAM;
    }

    BOARD_DEVICES_STS_LOG("Resetting the serializer...\n\r");
    ret = Board_fpdUb941SetDigitalRst1ModeCtrl(handle,
                                               fpdModParams,
                                               BOARD_FPD_MODE_RESET);
    if(BOARD_SOK != ret)
    {
        BOARD_DEVICES_ERR_LOG("Resetting the serializer failed\n\r");
        return ret;
    }

    BOARD_DEVICES_STS_LOG("Setting the serializer bus frequency to 400kbits/sec...\n\r");
    ret = Board_fpdUb941SetI2CBusFreq(handle,
                                      fpdModParams,
                                      BOARD_FPD_KBPS_400);
    if(BOARD_SOK != ret)
    {
        BOARD_DEVICES_ERR_LOG("Setting the serializer bus frequency to 400kbits/sec failed\n\r");
        return ret;
    }

    BOARD_DEVICES_STS_LOG("Enabling the I2C Pass-Through Mode...\n");
    ret = Board_fpdUb941SetI2CPassThrModeCfg(handle,
                                             fpdModParams,
                                             BOARD_FPD_MODE_ENABLE);
    if(BOARD_SOK != ret)
    {
        BOARD_DEVICES_ERR_LOG("Enabling the I2C Pass-Through Mode failed\n");
        return BOARD_INVALID_PARAM;
    }

    Board_delay(BOARD_FPD_I2C_CFG_DELAY);

    BOARD_DEVICES_STS_LOG("Enabling the I2C Pass-Through All Transactions...\n");
    ret = Board_fpdUb941SetI2CPassAllModeCtrl(handle,
                                              fpdModParams,
                                              BOARD_FPD_MODE_ENABLE);
    if(BOARD_SOK != ret)
    {
        BOARD_DEVICES_ERR_LOG("Enabling the I2C Pass-Through All Transactions failed\n");
        return BOARD_INVALID_PARAM;
    }

    Board_delay(BOARD_FPD_I2C_CFG_DELAY);

    BOARD_DEVICES_STS_LOG("Setting the remote device cfg...\n");
    ret = Board_fpdUb941RmtDesAliasCfg(handle,
                                       fpdModParams,
                                       fpdRmtParams);
    if(BOARD_SOK != ret)
    {
        BOARD_DEVICES_ERR_LOG("Setting the remote device cfg failed\n");
        return BOARD_INVALID_PARAM;
    }

    Board_delay(BOARD_FPD_I2C_CFG_DELAY);

    return BOARD_SOK;
}

/**
 * \brief  DSI module init
 *
 * This function is used to initialize the ub941 DSI module.
 *
 * \param   handle         [IN]  Low level driver handle
 * \param   fpdModParams   [IN]  FPD module params
 *
 * \return  Board_STATUS in case of success or appropriate error code.
 *
 */
Board_STATUS Board_fpdUb941DsiModuleInit(void *handle,
                                         Board_FpdModuleObj *fpdModParams)
{
    uint8_t regAddr;
    uint8_t regData;
    Board_STATUS ret = BOARD_SOK;

    if(NULL == handle)
    {
        return BOARD_INVALID_PARAM;
    }

    /* Disable DSI */
    ret = Board_fpdUb941SetResetModeCtrl(handle,
                                         fpdModParams,
                                         BOARD_FPD_MODE_RESET);
    if(BOARD_SOK != ret)
    {
        BOARD_DEVICES_ERR_LOG("Disabling the DSI module failed\n");
        return BOARD_INVALID_PARAM;
    }

    Board_delay(BOARD_FPD_I2C_CFG_DELAY);

    /* Select FPD-Link III Port 0 */
    ret = Board_fpdUb941SelI2CAccessPort(handle,
                                         fpdModParams,
                                         BOARD_FPD_UB941_PORT0_SEL);
    if(BOARD_SOK != ret)
    {
        BOARD_DEVICES_ERR_LOG("Select the FPD-Link III Port0 failed\n");
        return BOARD_INVALID_PARAM;
    }

    Board_delay(BOARD_FPD_I2C_CFG_DELAY);

    ret = Board_fpdUb941SetRxCRCCheckerEn(handle,
                                          fpdModParams,
                                          BOARD_FPD_MODE_ENABLE);
    if(BOARD_SOK != ret)
    {
        BOARD_DEVICES_ERR_LOG("Enabling the Rx CRC failed\n");
        return BOARD_INVALID_PARAM;
    }

    Board_delay(BOARD_FPD_I2C_CFG_DELAY);

    ret = Board_fpdUb941SetFilterEn(handle,
                                    fpdModParams,
                                    BOARD_FPD_MODE_ENABLE);
    if(BOARD_SOK != ret)
    {
        BOARD_DEVICES_ERR_LOG("Enabling serializer filter failed\n");
        return BOARD_INVALID_PARAM;
    }

    Board_delay(BOARD_FPD_I2C_CFG_DELAY);

    /* Enable I2C_PASSTHROUGH, FPD-Link III Port 0 */
    ret = Board_fpdUb941SetI2CPassThrModeCfg(handle,
                                             fpdModParams,
                                             BOARD_FPD_MODE_ENABLE);
    if(BOARD_SOK != ret)
    {
        BOARD_DEVICES_ERR_LOG("Enabling the I2C Pass-Through Mode for PORT0 failed\n");
        return BOARD_INVALID_PARAM;
    }

    Board_delay(BOARD_FPD_I2C_CFG_DELAY);

    /* Select DSI Port 0 digital registers */
    ret = Board_fpdUb941IndirRegAccBlkSel(handle,
                                          fpdModParams,
                                          BOARD_FPD_UB941_DSI_PORT0_REG);
    if(BOARD_SOK != ret)
    {
        BOARD_DEVICES_ERR_LOG("Accessing the DSI PORT0 registers failed\n");
        return BOARD_INVALID_PARAM;
    }

    Board_delay(BOARD_FPD_I2C_CFG_DELAY);

    /* Read/Write Mode sel */
    ret = Board_fpdUb941SetIndirRegRdEn(handle,
                                        fpdModParams,
                                        BOARD_FPD_UB941_Indirect_Register_Read_ENABLE);
    if(BOARD_SOK != ret)
    {
        BOARD_DEVICES_ERR_LOG("Indirect register rd/wr mode selected failed\n");
        return BOARD_INVALID_PARAM;
    }

    Board_delay(BOARD_FPD_I2C_CFG_DELAY);

    regAddr = BOARD_FPD_UB941_DSI_CONFIG_1_REG_ADDR;
    regData = 0x60;
    /* Select DSI_CONFIG_1 register & Set DSI_VS_POLARITY=DSI_HS_POLARITY=1 */
    ret = Board_fpdUb941DsiIndirRegWr(handle,
                                      fpdModParams,
                                      BOARD_FPD_UB941_DSI_PORT_INDIRECT_ACCESS,
                                      regAddr,
                                      regData);
    if(BOARD_SOK != ret)
    {
        BOARD_DEVICES_ERR_LOG("Writing to the indirect register, DSI_CONFIG_1 failed\n");
        return BOARD_INVALID_PARAM;
    }

    Board_delay(BOARD_FPD_I2C_CFG_DELAY);
    
    /* Select DSI Port 0 digital registers */
    ret = Board_fpdUb941IndirRegAccBlkSel(handle,
                                          fpdModParams,
                                          BOARD_FPD_UB941_DSI_PORT1_REG);
    if(BOARD_SOK != ret)
    {
        BOARD_DEVICES_ERR_LOG("Accessing the DSI PORT0 registers failed\n");
        return BOARD_INVALID_PARAM;
    }

    Board_delay(BOARD_FPD_I2C_CFG_DELAY);

    /* Read/Write Mode sel */
    ret = Board_fpdUb941SetIndirRegRdEn(handle,
                                        fpdModParams,
                                        BOARD_FPD_UB941_Indirect_Register_Read_ENABLE);
    if(BOARD_SOK != ret)
    {
        BOARD_DEVICES_ERR_LOG("Indirect register rd/wr mode selected failed\n");
        return BOARD_INVALID_PARAM;
    }

    Board_delay(BOARD_FPD_I2C_CFG_DELAY);

    regAddr = BOARD_FPD_UB941_DSI_CONFIG_1_REG_ADDR;
    regData = 0x60;
    /* Select DSI_CONFIG_1 register & Set DSI_VS_POLARITY=DSI_HS_POLARITY=1 */
    ret = Board_fpdUb941DsiIndirRegWr(handle,
                                      fpdModParams,
                                      BOARD_FPD_UB941_DSI_PORT_INDIRECT_ACCESS,
                                      regAddr,
                                      regData);
    if(BOARD_SOK != ret)
    {
        BOARD_DEVICES_ERR_LOG("Writing to the indirect register, DSI_CONFIG_1 failed\n");
        return BOARD_INVALID_PARAM;
    }

    Board_delay(BOARD_FPD_I2C_CFG_DELAY);

    /* FORCING 2:2 MODE */
    ret = Board_fpdUb941Tx_Mode(handle,
                                fpdModParams,
                                BOARD_FPD_UB941_FORCED_INDEPENDENT_2_2_MODE);
    if(BOARD_SOK != ret)
    {
        BOARD_DEVICES_ERR_LOG("forcing 2:2 mode failed\n");
        return BOARD_INVALID_PARAM;
    }

    Board_delay(BOARD_FPD_I2C_CFG_DELAY);

    /* Enable DSI_CONTINUOUS_CLOCK for Port1 */
    ret = Board_fpdUb941SetSerClkModeCtrl(handle,
                                          fpdModParams,
                                          BOARD_FPD_UB941_DSI_CONTINUOUS_CLK_ENABLE);
    if(BOARD_SOK != ret)
    {
        BOARD_DEVICES_ERR_LOG("Enabling DSI_CONTINUOUS_CLOCK for Port1 failed\n");
        return BOARD_INVALID_PARAM;
    }

    Board_delay(BOARD_FPD_I2C_CFG_DELAY);

    /* Enable 4 lanes of DSI Port 1 */
    ret = Board_fpdUb941SetDsiLaneModeCtrl(handle,
                                           fpdModParams,
                                           BOARD_FPD_UB941_LANES_2);
    if(BOARD_SOK != ret)
    {
        BOARD_DEVICES_ERR_LOG("Enabling all 4 lanes of DSI Port 1 failed\n");
        return BOARD_INVALID_PARAM;
    }

    Board_delay(BOARD_FPD_I2C_CFG_DELAY);

    /* Read/Write Mode sel */
    ret = Board_fpdUb941SetIndirRegRdEn(handle,
                                        fpdModParams,
                                        BOARD_FPD_UB941_INDIRECT_REG_RD_DISABLE);
    if(BOARD_SOK != ret)
    {
        BOARD_DEVICES_ERR_LOG("Indirect register rd/wr mode selected failed\n");
        return BOARD_INVALID_PARAM;
    }

    Board_delay(BOARD_FPD_I2C_CFG_DELAY);

    regAddr = BOARD_FPD_UB941_DPHY_SKIP_TIMING_REG_ADDR;
    regData = 0x16;
    /* Select DPHY_SKIP_TIMING register & write TSKIP_CNT value for 315 MHz DSI clock frequency (1080p, PCLK = 105 MHz*/
    ret = Board_fpdUb941DsiIndirRegWr(handle,
                                      fpdModParams,
                                      BOARD_FPD_UB941_DSI_PORT_INDIRECT_ACCESS,
                                      regAddr,
                                      regData);
    if(BOARD_SOK != ret)
    {
        BOARD_DEVICES_ERR_LOG("Writing to the indirect register, DPHY_SKIP_TIMING failed\n");
        return BOARD_INVALID_PARAM;
    }

    Board_delay(BOARD_FPD_I2C_CFG_DELAY);


    /* Select DSI Port 0 digital registers */
    ret = Board_fpdUb941IndirRegAccBlkSel(handle,
                                          fpdModParams,
                                          BOARD_FPD_UB941_DSI_PORT1_REG);
    if(BOARD_SOK != ret)
    {
        BOARD_DEVICES_ERR_LOG("Accessing the DSI PORT0 registers failed\n");
        return BOARD_INVALID_PARAM;
    }

    Board_delay(BOARD_FPD_I2C_CFG_DELAY);

    /* Read/Write Mode sel */
    ret = Board_fpdUb941SetIndirRegRdEn(handle,
                                        fpdModParams,
                                        BOARD_FPD_UB941_INDIRECT_REG_RD_DISABLE);
    if(BOARD_SOK != ret)
    {
        BOARD_DEVICES_ERR_LOG("Indirect register rd/wr mode selected failed\n");
        return BOARD_INVALID_PARAM;
    }

    Board_delay(BOARD_FPD_I2C_CFG_DELAY);

    regAddr = BOARD_FPD_UB941_DPHY_SKIP_TIMING_REG_ADDR;
    regData = 0x0C;
    /* Select DPHY_SKIP_TIMING register & write TSKIP_CNT value for 315 MHz DSI clock frequency (1080p, PCLK = 105 MHz*/
    ret = Board_fpdUb941DsiIndirRegWr(handle,
                                      fpdModParams,
                                      BOARD_FPD_UB941_DSI_PORT_INDIRECT_ACCESS,
                                      regAddr,
                                      regData);
    if(BOARD_SOK != ret)
    {
        BOARD_DEVICES_ERR_LOG("Writing to the indirect register, DPHY_SKIP_TIMING failed\n");
        return BOARD_INVALID_PARAM;
    }

    Board_delay(BOARD_FPD_I2C_CFG_DELAY);

    /* Enable DSI */
    ret = Board_fpdUb941SetResetModeCtrl(handle,
                                         fpdModParams,
                                         BOARD_FPD_MODE_NORMAL);
    if(BOARD_SOK != ret)
    {
        BOARD_DEVICES_ERR_LOG("Enabling the DSI module failed\n");
        return BOARD_INVALID_PARAM;
    }

    Board_delay(BOARD_FPD_I2C_CFG_DELAY);

    return BOARD_SOK;
}

/**
 * \brief  Pattern generation config
 *
 * This function is used to initialize the ub941 DSI module.
 *
 * \param   handle         [IN]  Low level driver handle
 * \param   fpdModParams   [IN]  FPD module params
 *
 * \return  Board_STATUS in case of success or appropriate error code.
 *
 */
Board_STATUS Board_fpdUb941PatternGenConfig(void *handle,
                                            Board_FpdModuleObj *fpdModParams)
{
    uint8_t regAddr;
    uint8_t regData;
    Board_STATUS ret = BOARD_SOK;

    regAddr = BOARD_FPD_UB941_PGCDC1_REG_ADDR;
    regData = 0x03;
    /* Pattern generation clock divider configuration */
    ret = Board_fpdUb941DsiIndirRegWr(handle,
                                      fpdModParams,
                                      BOARD_FPD_UB941_PATTERN_GEN_INDIRECT_ACESS,
                                      regAddr,
                                      regData);
    if(BOARD_SOK != ret)
    {
        BOARD_DEVICES_ERR_LOG("Writing to the indirect register, PGCDC1 failed\n");
        return BOARD_INVALID_PARAM;
    }

    Board_delay(BOARD_FPD_I2C_CFG_DELAY);

    /* Enabling auto scroll of pattern */
    ret = Board_fpdUb941SetAutoScrollPattEn(handle,
                                            fpdModParams,
                                            BOARD_FPD_MODE_ENABLE);
    if(BOARD_SOK != ret)
    {
        BOARD_DEVICES_ERR_LOG("Inverted color pattern select failed\n");
        return BOARD_INVALID_PARAM;
    }

    Board_delay(BOARD_FPD_I2C_CFG_DELAY);

    /* Selecting pattern */
    ret = Board_fpdUb941SelPatt(handle,
                                fpdModParams,
                                BOARD_FPD_UB941_PATTERN_CHECKERBOARD);
    if(BOARD_SOK != ret)
    {
        BOARD_DEVICES_ERR_LOG("selecting pattern failed\n");
        return BOARD_INVALID_PARAM;
    }

    Board_delay(BOARD_FPD_I2C_CFG_DELAY);

    return BOARD_SOK;
}

/**
 * \brief  DSI module init
 *
 * This function is used to initialize the ub941 DSI module.
 *
 * \param   handle         [IN]  Low level driver handle
 * \param   fpdModParams   [IN]  FPD module params
 *
 * \return  Board_STATUS in case of success or appropriate error code.
 *
 */
Board_STATUS Board_fpdUb924ModuleInit(void *handle,
                                      Board_FpdModuleObj *fpdModParams)
{
    Board_STATUS ret = BOARD_SOK;

    if(NULL == handle)
    {
        return BOARD_INVALID_PARAM;
    }

    /* Resetting the deserializer... */
    ret = Board_fpdUb924SelDesDigialRst1Mode(handle,
                                             fpdModParams,
                                             BOARD_FPD_MODE_RESET);
    if(BOARD_SOK != ret)
    {
        BOARD_DEVICES_ERR_LOG("Resetting the deserializer... failed\n");
        return BOARD_INVALID_PARAM;
    }

	Board_delay(BOARD_FPD_I2C_CFG_DELAY);

	/* Normal mode... */
    ret = Board_fpdUb924SelDesDigialRst1Mode(handle,
                                             fpdModParams,
                                             BOARD_FPD_MODE_NORMAL);
    if(BOARD_SOK != ret)
    {
        BOARD_DEVICES_ERR_LOG("Setting the deserializer in normal mode... failed\n");
        return BOARD_INVALID_PARAM;
    }

    Board_delay(BOARD_FPD_I2C_CFG_DELAY);

    /* Enabling the Rx CRC  */
    ret = Board_fpdUb924SetRxCRCCheckerEn(handle,
                                          fpdModParams,
                                          BOARD_FPD_MODE_ENABLE);
    if(BOARD_SOK != ret)
    {
        BOARD_DEVICES_ERR_LOG("Enabling the Rx CRC failed\n");
        return BOARD_INVALID_PARAM;
    }

    Board_delay(BOARD_FPD_I2C_CFG_DELAY);

    /* Enabling deserializer filter  */
    ret = Board_fpdUb924SelDesFilterEnMode(handle,
                                           fpdModParams,
                                           BOARD_FPD_MODE_ENABLE);
    if(BOARD_SOK != ret)
    {
        BOARD_DEVICES_ERR_LOG("Enabling deserializer filter failed\n");
        return BOARD_INVALID_PARAM;
    }

    Board_delay(BOARD_FPD_I2C_CFG_DELAY);

    /* Enable I2C_PASSTHROUGH */
    ret = Board_fpdUb924SetI2CPassThrModeCfg(handle,
                                             fpdModParams,
                                             BOARD_FPD_MODE_ENABLE);
    if(BOARD_SOK != ret)
    {
        BOARD_DEVICES_ERR_LOG("Enabling the I2C Pass-Through Mode failed\n");
        return BOARD_INVALID_PARAM;
    }

    Board_delay(BOARD_FPD_I2C_CFG_DELAY);

    /*horizontal sync polarity*/
    ret = Board_fpdUb924SelDesHSSyncPolarity(handle,
                                             fpdModParams,
                                             BOARD_FPD_UB924_ACTIVE_HIGH);
    if(BOARD_SOK != ret)
    {
        BOARD_DEVICES_ERR_LOG("Enabling horizontal sync polarity failed\n");
        return BOARD_INVALID_PARAM;
    }

    Board_delay(BOARD_FPD_I2C_CFG_DELAY);

    /*vertical sync polarity*/
    ret = Board_fpdUb924SelDesVSSyncPolarity(handle,
                                             fpdModParams,
                                             BOARD_FPD_UB924_ACTIVE_HIGH);
    if(BOARD_SOK != ret)
    {
        BOARD_DEVICES_ERR_LOG("Enabling vertical sync polarity failed\n");
        return BOARD_INVALID_PARAM;
    }

    Board_delay(BOARD_FPD_I2C_CFG_DELAY);

    /*selecting pattern generator bit mode*/
    ret = Board_fpdUb924SelDesPGBitMode(handle,
                                        fpdModParams,
                                        BOARD_FPD_UB924_MODE_SEL_24);
    if(BOARD_SOK != ret)
    {
        BOARD_DEVICES_ERR_LOG("Selecting pattern generator bit mode failed\n");
        return BOARD_INVALID_PARAM;
    }

    Board_delay(BOARD_FPD_I2C_CFG_DELAY);

    /*video timing select*/
    ret = Board_fpdUb924SelDesPGTmngSel(handle,
                                        fpdModParams,
                                        BOARD_FPD_UB924_EXT_VIDEO_TIMING);
    if(BOARD_SOK != ret)
    {
        BOARD_DEVICES_ERR_LOG("video timing select failed\n");
        return BOARD_INVALID_PARAM;
    }

    Board_delay(BOARD_FPD_I2C_CFG_DELAY);

    return BOARD_SOK;
}

/**
 * \brief  TLC59108 Led driver config
 *
 * This function is used for configuring the TLC59108 LED driver
 * sesnor device to control backlight.
 *
 * \param   handle        [IN]   Low level driver handle
 * \param   fpdModParams  [IN]   FPD module params
 * \param   fpdRmtParams  [IN]   FPD remote params
 *
 * \return  Board_STATUS in case of success or appropriate error code.
 *
 */
Board_STATUS Board_tlc59108LedDrvCfg(void *handle,
                                     Board_FpdModuleObj *fpdModParams,
                                     Board_FpdRmtDevObj *fpdRmtParams)
{
    Board_STATUS ret = BOARD_SOK;

    if(NULL == handle)
    {
        return BOARD_INVALID_PARAM;
    }

    BOARD_DEVICES_STS_LOG("Making the UB924 GPIO0 level high...\n\r");
    ret = Board_fpdUb924GpioOutputMode(handle,
                                       fpdModParams,
                                       BOARD_FPD_UB924_GPIO0,
                                       BOARD_FPD_UB924_OUTPUT_HIGH);
    if(BOARD_SOK != ret)
    {
        return ret;
    }

    BOARD_DEVICES_STS_LOG("Enabling the UB924 GPIO0...\n\r");
    ret = Board_fpdUb924GpioEnMode(handle,
                                   fpdModParams,
                                   BOARD_FPD_UB924_GPIO0,
                                   BOARD_FPD_UB924_GPIO);
    if(BOARD_SOK != ret)
    {
        return ret;
    }

    BOARD_DEVICES_STS_LOG("Configuring the LED output...\n\r");
    ret = Board_tlc59108LedOut0Cfg(handle,
                                   fpdRmtParams,
                                   BOARD_FPD_TLC59108_LED2,
                                   BOARD_FPD_TLC59108_LED_DRV_ON_PMWX);
    if(BOARD_SOK != ret)
    {
        return ret;
    }
    Board_delay(BOARD_FPD_I2C_CFG_DELAY);

    BOARD_DEVICES_STS_LOG("Configuring response mode...\n\r");
    ret = Board_tlc59108DeviceRespModeCfg(handle,
                                          fpdRmtParams,
                                          BOARD_FPD_MODE_ENABLE,
                                          BOARD_FPD_TLC59108_OSC_NORAMAL_OPERATION);
    if(BOARD_SOK != ret)
    {
        return ret;
    }
    Board_delay(BOARD_FPD_I2C_CFG_DELAY);

    return BOARD_SOK;
}

