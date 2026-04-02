/******************************************************************************
 * Copyright (c) 2020-2023 Texas Instruments Incorporated - http://www.ti.com
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
 *  \file   board_utils.c
 *
 *  \brief  Implements multiple board utility functions
 *
 */

#include <stdio.h>
#include <string.h>
#include <ti/csl/arch/csl_arch.h>
#include <ti/drv/mmcsd/MMCSD.h>
#include <ti/drv/mmcsd/soc/MMCSD_soc.h>
#include "board_internal.h"
#include "board_utils.h"
#include "board_info_ddr.h"
#include "board_cfg.h"

Board_DetectCfg_t  gBoardDetCfg[BOARD_ID_MAX_BOARDS] =
 {{BOARD_COMMON_EEPROM_I2C_INST, BOARD_GESI_EEPROM_SLAVE_ADDR, BOARD_SOC_DOMAIN_WKUP, "J7X-GESI-EXP"},
  {BOARD_COMMON_EEPROM_I2C_INST, BOARD_ENET_EEPROM_SLAVE_ADDR, BOARD_SOC_DOMAIN_WKUP, "J7X-VSC8514-ETH"},
  {BOARD_COMMON_EEPROM_I2C_INST, BOARD_SOM_EEPROM_SLAVE_ADDR, BOARD_SOC_DOMAIN_WKUP, "J7200X-PM2-SOM"},
  {BOARD_COMMON_EEPROM_I2C_INST, BOARD_CP_EEPROM_SLAVE_ADDR, BOARD_SOC_DOMAIN_WKUP, "J7X-BASE-CPB"}};

Board_I2cInitCfg_t gBoardI2cInitCfg = {0U, BOARD_SOC_DOMAIN_MAIN, BFALSE};
Board_initParams_t gBoardInitParams = {BOARD_UART_INSTANCE, BOARD_UART_SOC_DOMAIN, BOARD_PSC_DEVICE_MODE_NONEXCLUSIVE,
                                       BOARD_MAIN_CLOCK_GROUP_ALL, BOARD_MCU_CLOCK_GROUP_ALL};

/**
 * \brief Board ID read function
 *
 * \param   info     [IN]  Board info structure
 * \param   boardID  [IN]  ID of the board to be detected
 * \n                      BOARD_ID_GESI(0x0) - GESI Board
 * \n                      BOARD_ID_ENET(0x1) - Quad ENET expansion
 * \n                      BOARD_ID_SOM(0x2) - Dual PMIC SoM Board
 * \n                      BOARD_ID_CP(0x3) - CP Board
 *
 * \return   BOARD_SOK in case of success or appropriate error code.
 *
 */
Board_STATUS Board_getBoardData(Board_IDInfo_v2 *info, uint32_t boardID)
{
    Board_I2cInitCfg_t i2cCfg;
    Board_STATUS status;

    i2cCfg.i2cInst    = gBoardDetCfg[boardID].i2cInst;
    i2cCfg.socDomain  = gBoardDetCfg[boardID].socDomain;
    i2cCfg.enableIntr = BFALSE;
    Board_setI2cInitConfig(&i2cCfg);

    if(Board_isBoardDDRIdDataValid())
    {
        if((BOARD_ID_GESI == boardID)  ||
           (BOARD_ID_CP   == boardID)  ||
           (BOARD_ID_ENET == boardID))
        {
            status = Board_getBoardIdData(info, boardID);
            if(BOARD_SOK != status)
            {
#if defined(BOARD_ID_MEMORY_DUMP_ERR_FALLBACK)
                status = Board_getIDInfo_v2(info, gBoardDetCfg[boardID].slaveAddr);
#endif
            }
        }
        else
        {
            status = Board_getIDInfo_v2(info, gBoardDetCfg[boardID].slaveAddr);
        }
    }
    else
    {
        status = Board_getIDInfo_v2(info, gBoardDetCfg[boardID].slaveAddr);
    }

    return status;
}

/**
 * \brief Board detect function
 *
 *  Checks if the board with given 'boardID' is connected to the
 *  base board.
 *
 *  \n Note: Board ID EEPROM should be programmed for this function
 *           to work properly.
 *
 * \param   boardID  [IN]  ID of the board to be detected
 * \n                      BOARD_ID_GESI(0x0) - GESI Board
 * \n                      BOARD_ID_ENET(0x1) - Quad ENET expansion
 * \n                      BOARD_ID_SOM(0x2) - Dual PMIC SoM Board
 * \n                      BOARD_ID_CP(0x3) - CP Board
 *
 * \return   BTRUE if the given board is detected else BFALSE.
 *           SoM board will be always connected to the base board.
 *           For SoM boardID return value BTRUE indicates dual PMIC
 *           SoM and BFALSE indicates alternate PMIC SoM
 *
 */
bool Board_detectBoard(uint32_t boardID)
{
    Board_IDInfo_v2 info;
    Board_STATUS status;
    bool bDet = BFALSE;

    memset(&info, 0, sizeof(info));

    if(BOARD_ID_SOM >= boardID)
    {
        status = Board_getBoardData(&info, boardID);
        if(BOARD_SOK == status)
        {
            if(!(strncmp(info.boardInfo.boardName,
                         gBoardDetCfg[boardID].bName,
                         BOARD_BOARD_NAME_LEN)))
            {
                bDet = BTRUE;
            }
        }
    }

    return bDet;
}

/**
 * \brief  Checks for Alpha board revision
 *
 * \param   boardID  [IN]  ID of the board to be detected
 * \n                      ID of the board to be detected
 * \n					   BOARD_ID_SOM(0x0) - Dual PMIC SoM Board
 * \n                      BOARD_ID_CP(0x1) - CP Board
 * \n                      BOARD_ID_GESI(0x2) - GESI Board
 * \n                      BOARD_ID_ENET(0x3) - Quad ENET expansion
 *
 * \return BTRUE if board revision is E2, BFALSE for all other cases
 */
bool Board_isAlpha(uint32_t boardID)
{
    Board_IDInfo_v2 info;
    Board_STATUS status;
    bool alphaBoard = BFALSE;

    memset(&info, 0, sizeof(info));

    status = Board_getBoardData(&info, boardID);
    if(BOARD_SOK == status)
    {
        if(BOARD_ID_SOM == boardID)
        {
            /* Check for PM1 and PM2 SoM versions */
            if(!(strncmp(info.boardInfo.designRev, "E2", BOARD_DESIGN_REV_LEN)) ||
               !(strncmp(info.boardInfo.designRev, "E5", BOARD_DESIGN_REV_LEN)))
            {
                alphaBoard = BTRUE;
            }
        }
        else
        {
            if(!(strncmp(info.boardInfo.designRev,
                         "E2",
                         BOARD_DESIGN_REV_LEN)))
            {
                alphaBoard = BTRUE;
            }
        }
    }

    return alphaBoard;
}

/**
 *  \brief    Function to detect ENET expansion application card type
 *
 *  ENET expansion connector supports QSGMII and SGMII application cards.
 *  This function detects type of the application card connected on
 *  ENET expansion connector.
 *
 *  \return
 *            0 (BOARD_ENET_NONE)   - No board connected or invalid board ID data
 *            1 (BOARD_ENET_QSGMII) - QSGMII board connected
 *            2 (BOARD_ENET_SGMII)  - SGMII board connected
 *           -1 (BOARD_ENET_UNKOWN) - Unknown board
*/
int32_t Board_detectEnetCard(void)
{
    Board_IDInfo_v2 info;
    Board_STATUS status;
    int32_t ret = BOARD_ENET_NONE;

    memset(&info, 0, sizeof(info));

    status = Board_getBoardData(&info, BOARD_ID_ENET);
    if(BOARD_SOK == status)
    {
        if(0 == (strcmp(info.boardInfo.boardName, "J7X-VSC8514-ETH")))
        {
            ret = BOARD_ENET_QSGMII;
        }
        else if(0 == (strcmp(info.boardInfo.boardName, "J7X-DP83869-ETH")))
        {
            ret = BOARD_ENET_SGMII;
        }
        else
        {
            ret = BOARD_ENET_UNKOWN;
        }
    }
    else
    {
        ret = BOARD_ENET_NONE;
    }

    return ret;
}

/**
 * \brief Read MAC ID function
 *
 *  This function reads the MAC addresses programmed to the board ID EEPROM
 *  on the boards with Ethernet ports. Exception is for MCU Ethernet port
 *  which is supposed to use MAC ID from SoC MMR registers.
 *
 *  There can be multiple MAC IDs stored in the EEPROM based on the
 *  number of Ethernet ports on the board. Number of MAC IDs copied
 *  to 'macAddrBuf' can be read using 'macAddrCnt' parameters.
 *
 *  Each MAC address will be 6 bytes long. MAC IDs will be copied to buffer
 *  based on 'macBufSize'. If the buffer size is long enough, all the MAC
 *  addresses from EEPROM will be copied to 'macAddrBuf' else fewer MAC
 *  IDs to fit within 'macBufSize'. MAC count for a given board can be
 *  read using Board_readMacAddrCount() function.
 *
 *  \n Note: Board ID EEPROM should be programmed for this function
 *           to work properly.
 *
 * \param  boardID  [IN]  ID of the board to be detected
 * \n                      BOARD_ID_GESI(0x0) - GESI Board
 * \n                      BOARD_ID_ENET(0x1) - Quad ENET expansion
 * \n                      BOARD_ID_SOM(0x2) - Dual PMIC SoM Board
 * \n                      BOARD_ID_CP(0x3) - CP Board
 * \param  macAddrBuf[OUT] Buffer to write MAC IDs read from EEPROM
 * \param  macBufSize[IN]  Size of the macAddrBuf
 * \param  macAddrCnt[OUT] Number of valid MAC addresses programmed to the EEPROM
 *                         This an optional variable to read the MAC ID count
 *                         filled to the 'macAddrBuf'.
 *                         Pass a valid address to get MAC ID count.
 *
 * \return   BOARD_SOK in case of success or appropriate error code.
 *
 */
Board_STATUS Board_readMacAddr(uint32_t boardID,
                               uint8_t  *macAddrBuf,
                               uint32_t macBufSize,
                               uint32_t *macAddrCnt)
{
    Board_IDInfo_v2 info;
    Board_STATUS status;
    uint8_t macCount = 0;

    memset(&info, 0, sizeof(info));

    if((BOARD_ID_SOM >= boardID) && (NULL != macAddrBuf))
    {
        status = Board_getBoardData(&info, boardID);
        if(BOARD_SOK == status)
        {
            macCount = ((info.macInfo.macControl & BOARD_MAC_COUNT_MASK)
                          >> BOARD_MAC_COUNT_SHIFT) + 1U;
            if(macBufSize < (macCount * BOARD_MAC_ADDR_BYTES))
            {
                macCount = (macBufSize / BOARD_MAC_ADDR_BYTES);
            }

            memcpy(macAddrBuf, &(info.macInfo.macAddress[0]),
                   (macCount * BOARD_MAC_ADDR_BYTES));
        }
    }
    else
    {
        status = BOARD_INVALID_PARAM;
    }

    if(NULL != macAddrCnt)
    {
        *macAddrCnt = macCount;
    }

    return status;
}

/**
 * \brief Read MAC ID count
 *
 *  This function reads the number of MAC addresses programmed to
 *  board ID EEPROM on the boards with Ethernet ports. Exception is
 *  for MCU Ethernet port which is supposed to use MAC ID from SoC
 *  MMR registers. Each MAC address programmed to EEPROM is 6 bytes long.
 *
 *  \n Note: Board ID EEPROM should be programmed for this function
 *           to work properly.
 *
 * \param  boardID  [IN]  ID of the board to be detected
 * \n                      BOARD_ID_GESI(0x0) - GESI Board
 * \n                      BOARD_ID_ENET(0x1) - Quad ENET expansion
 * \param  macAddrCnt[OUT] Number of valid MAC addresses programmed to the EEPROM
 *
 * \return   BOARD_SOK in case of success or appropriate error code.
 *
 */
Board_STATUS Board_readMacAddrCount(uint32_t boardID,
                                    uint32_t *macAddrCnt)
{
    Board_IDInfo_v2 info;
    Board_STATUS status;

    memset(&info, 0, sizeof(info));

    if((BOARD_ID_SOM >= boardID) && (NULL != macAddrCnt))
    {
        status = Board_getBoardData(&info, boardID);
        if(BOARD_SOK == status)
        {
            *macAddrCnt = ((info.macInfo.macControl & BOARD_MAC_COUNT_MASK)
                           >> BOARD_MAC_COUNT_SHIFT) + 1U;
        }
    }
    else
    {
        status = BOARD_INVALID_PARAM;
    }

    return status;
}

/**
 * \brief Function to configure I2C configurations used by board
 *
 * This function is used to set the I2C controller instance and
 * SoC domain used by the board module for IO expander and board
 * ID info read.
 *
 * Usage:
 * Call Board_setI2cInitConfig to set the I2C configurations
 * Call IO expander Init or Board ID info read/write
 *
 *  \return   BOARD_SOK in case of success or appropriate error code.
 *
 */
Board_STATUS Board_setI2cInitConfig(Board_I2cInitCfg_t *i2cCfg)
{
    if(NULL == i2cCfg)
    {
        return BOARD_INVALID_PARAM;
    }

    gBoardI2cInitCfg = *i2cCfg;

    return BOARD_SOK;
}

/**
 * \brief Voltage Switching function for MMCSD
 *
 * Functionality: Change the voltage of the MMC CMD & DAT lines.
 *  This function is called by the MMCSD card driver (if the driver is
 *  configured to use this function at init time by the application) to change
 *  the CMD & DAT voltage from 3.0V to 1.8V if a UHS-I card is found.
 *  This function configures the PMIC controller of the board to switch the voltage
 *
 *  Note: This function uses non-standard board API naming and return type
 *        to align with existing platforms.
 *
 * \param   instance       [IN]  Device instance
 * \param   switchVoltage  [IN]  MMCSD IO voltage value
 *
 */
MMCSD_Error Board_mmc_voltageSwitchFxn(uint32_t instance,
                                       MMCSD_BusVoltage_e switchVoltage)
{
    int32_t ret = BOARD_FAIL;
	MMCSD_Error mmcRetVal = MMCSD_ERR;

    if(MMCSD_BUS_VOLTAGE_1_8V == switchVoltage)
    {
       ret = Board_pmSdVoltageCtrl(0);
    }
    else if(MMCSD_BUS_VOLTAGE_3_3V == switchVoltage)
    {
       ret = Board_pmSdVoltageCtrl(1);
	  }
    else
    {
       /* Do Nothing */
    }

    if(BOARD_SOK == ret)
    {
	   mmcRetVal = MMCSD_OK;
	  }

	return(mmcRetVal);
}

/**
 * \brief Function to get board init params
 *
 *  This function shall be used to know the current board init
 *  parameters and update them if needed using the function Board_setInitParams.
 *
 * \param   initParams  [IN]  Board init params structure
 *
 * \return   BOARD_SOK in case of success or appropriate error code.
 *
 */
Board_STATUS Board_getInitParams(Board_initParams_t *initParams)
{
    if(NULL == initParams)
    {
        return BOARD_INVALID_PARAM;
    }

    *initParams = gBoardInitParams;

    return BOARD_SOK;
}

/**
 * \brief Function to configure board init parameters
 *
 *  Board init params includes the parameters used by Board_init
 *  function for different operations. Default init parameters
 *  used by Board_init can be updated using this function.
 *  All the default params can be overwritten by calling this function
 *  or some of can be changed by reading the existing init parameters
 *  using Board_getInitParams function.
 *
 * Usage:
 * Call Board_getInitParams to get the default board init parameters
 * Update the parameters as needed
 * Call Board_setInitParams to update the default board init parameters
 *
 * \param   initCfg  [IN]  Board Init config structure
 *
 * \return   BOARD_SOK in case of success or appropriate error code.
 *
 */
Board_STATUS Board_setInitParams(Board_initParams_t *initParams)
{
    if(NULL == initParams)
    {
        return BOARD_INVALID_PARAM;
    }

    gBoardInitParams = *initParams;

    return BOARD_SOK;
}

/**
 * \brief Function to get the SoC domain
 *
 *  This function returns the domain of the SoC core on which
 *  it is executing.
 *
 * \return   SoC domain of the core.
 *
 */
uint32_t Board_getSocDomain(void)
{
    uint32_t socDomain = BOARD_SOC_DOMAIN_MAIN;

#ifdef BUILD_MCU
    CSL_ArmR5CPUInfo info = {0};

    CSL_armR5GetCpuID(&info);
    if (CSL_ARM_R5_CLUSTER_GROUP_ID_0 == info.grpId)
    {
        socDomain = BOARD_SOC_DOMAIN_MCU;
    }
#endif

  return socDomain;
}

/**
 *  \brief    Function to generate delay in micro seconds
 *
 *  This function takes the delay parameters in usecs but the generated
 *  delay will be in multiples of msecs due to the osal function which
 *  generates delay in msecs. Delay parameter passed will be converted to
 *  msecs and fractional value will be adjusted to nearest msecs value.
 *  Minimum delay generated by this function is 1 msec.
 *  Function parameter is kept in usecs to match with existing
 *  platforms which has delay function for usecs.
 *
 *  \param    usecs [IN]  Specifies the time to delay in micro seconds.
 *
 */
void BOARD_delay(uint32_t usecs)
{
    uint32_t msecs;

    msecs = usecs/1000U;
    if(usecs%1000U)
    {
        msecs += 1U;
    }

    Osal_delay(msecs);
}
