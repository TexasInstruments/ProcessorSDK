/*!
* \file phy_reset.c
*
* \brief
* AM64xx EVM specific PHY reset component implementation
*
* \author
* KUNBUS GmbH
*
* \date
* 2022-02-18
*
* \copyright
* Copyright (c) 2022, KUNBUS GmbH<br /><br />
* All rights reserved.<br />
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:<br />
* <ol>
* <li>Redistributions of source code must retain the above copyright notice, this
* list of conditions and the following disclaimer.</li>
* <li>Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation
* and/or other materials provided with the distribution.</li>
* <li>Neither the name of the copyright holder nor the names of its
* contributors may be used to endorse or promote products derived from
* this software without specific prior written permission.</li>
* </ol>
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/
#include <kernel/dpl/ClockP.h>

#include "board/ioexp/ioexp_tca6424.h"

#include "ti_drivers_config.h"
#include "ti_drivers_open_close.h"
#include "ti_board_open_close.h"

#include "../appPhyReset.h"

#define INDEX_NOT_FOUND    -1   /* \brief Index for specified PHY address was not found */

#define APP_PHY_RESET_IO_EXP_I2C_INST     CONFIG_I2C1_STATUS_LEDS    /* \brief I2C instance which controls the IO expander */
#define APP_PHY_RESET_IO_EXP_ADDR         0x22                       /* \brief Address of the IO expander */

/*!
 *  \brief IO expander-specific PHY reset objects
 */
typedef struct APP_PHY_RESET_SIndexList
{
    uint32_t phyAddr;
    uint32_t ioExpIndex;
} APP_PHY_RESET_SIndexList_t;

/*!
 *  \brief IO expander handle
 */
static TCA6424_Config APP_PHY_RESET_ioExpConfig_g = { 0 };

/*!
 *  \brief List of PHY reset objects
 */
static APP_PHY_RESET_SIndexList_t APP_PHY_RESET_indexList_g[] =
{
  { 15 /* PHY addr */, 3 /* IO Exp Index */},
  { 3  /* PHY addr */, 4 /* IO Exp Index */}
};

/*!
*  Function: APP_PHY_RESET_getIndex
*
*  \brief
*  Get PHY index for a PHY with specified address.
*
*  \details
*  Searches through the PHY list for a PHY reset object with specified address.
*  Returns index of the corresponding PHY.
*
*  \param[in]     addr_p    PHY Address
*
*  \return        index     Index of the PHY object with the specified address. #INDEX_NOT_FOUND in case search was unsuccessful.
*/
static int32_t APP_PHY_RESET_getIndex(uint32_t addr_p)
{
    uint32_t i = 0;
    uint32_t elems = sizeof(APP_PHY_RESET_indexList_g) / sizeof(APP_PHY_RESET_indexList_g[0]);
    int32_t index = INDEX_NOT_FOUND;

    for (i = 0; i < elems; i++)
    {
        if (APP_PHY_RESET_indexList_g[i].phyAddr == addr_p)
        {
            index = (int32_t)APP_PHY_RESET_indexList_g[i].ioExpIndex;
            break;
        }
    }

    return index;
}

/*!
*  Function: APP_PHY_RESET_open
*
*  \brief
*  Configure PHY reset pins.
*
*  \details
*  This functions implementation is adapted to the AM64x / AM243x EVM boards.
*  Here the PHY reset pins are connected to a TCA6424 IO expander
*
*  Opens a TCA6424 driver for controlling the PHY reset pins.
*  Configures the pins as outputs and sets them to the HIGH state.
*
*  \param[in]     None.
*
*  \return SystemP_SUCCESS on success, else failure
*/
int32_t APP_PHY_RESET_open(void)
{
    int32_t status = SystemP_FAILURE;
    TCA6424_Params ioExpParams = { 0 };

    TCA6424_Params_init(&ioExpParams);
    ioExpParams.i2cInstance = APP_PHY_RESET_IO_EXP_I2C_INST;
    ioExpParams.i2cAddress = APP_PHY_RESET_IO_EXP_ADDR;
    status = TCA6424_open(&APP_PHY_RESET_ioExpConfig_g, &ioExpParams);
    if (status == SystemP_SUCCESS)
    {
        uint32_t i = 0;
        uint32_t elems = sizeof(APP_PHY_RESET_indexList_g) / sizeof(APP_PHY_RESET_indexList_g[0]);
        for (i = 0; i < elems; i++)
        {
            status = TCA6424_config(&APP_PHY_RESET_ioExpConfig_g, APP_PHY_RESET_indexList_g[i].ioExpIndex,
                TCA6424_MODE_OUTPUT);
            if (status != SystemP_SUCCESS)
            {
                break;
            }

            status = TCA6424_setOutput(&APP_PHY_RESET_ioExpConfig_g, APP_PHY_RESET_indexList_g[i].ioExpIndex,
                TCA6424_OUT_STATE_HIGH);
            if (status != SystemP_SUCCESS)
            {
                break;
            }
        }
    }

    return status;
}

/*!
*  Function: APP_PHY_RESET_close
*
*  \brief
*  Close PHY reset pins.
*
*  \details
*  This functions implementation is adapted to the AM64x / AM243x EVM boards.
*  Here the PHY reset pins are connected to a TCA6424 IO expander
*
*  Closes the TCA6424 driver.
*
*  \param[in]     None.
*
*  \return        None.
*/
void APP_PHY_RESET_close(void)
{
    TCA6424_close(&APP_PHY_RESET_ioExpConfig_g);
}

/*!
*  Function: APP_PHY_RESET_withAddr
*
*  \brief
*  Reset PHY with the particular address.
*
*  \details
*  This functions implementation is adapted to the AM64x / AM243x EVM boards.
*  Here the PHY reset pins are connected to a TCA6424 IO expander
*
*  Executes hardware reset of a PHY with particular address.
*  Sends a LOW pulse to the PHY reset pin for a duration of #APP_PHY_RESET_PULSE_DUR_US.
* 
*  \remarks
*  APP_PHY_RESET_open must be executed initially to be able to reset PHYs.
* 
*  \param[in]     addr_p    PHY Address
*
*  \return SystemP_SUCCESS on success, else failure
*/
int32_t APP_PHY_RESET_withAddr(uint32_t addr_p)
{
    int32_t status = SystemP_FAILURE;
    int32_t index = APP_PHY_RESET_getIndex(addr_p);

    if (index != INDEX_NOT_FOUND)
    {
        status = TCA6424_setOutput(&APP_PHY_RESET_ioExpConfig_g, index, TCA6424_OUT_STATE_LOW);
        if (status == SystemP_SUCCESS)
        {
            ClockP_usleep(APP_PHY_RESET_PULSE_DUR_US);

            status = TCA6424_setOutput(&APP_PHY_RESET_ioExpConfig_g, index, TCA6424_OUT_STATE_HIGH);
        }
    }

    return status;
}

/*!
*  Function: APP_PHY_RESET_all
*
*  \brief
*  Reset all PHYs.
*
*  \details
*  This functions implementation is adapted to the AM64x / AM243x EVM boards.
*  Here the PHY reset pins are connected to a TCA6424 IO expander
*
*  Executes hardware reset of all PHYs.
*  Sends a LOW pulse to each PHY reset pin for a duration of #APP_PHY_RESET_PULSE_DUR_US.
* 
*  \remarks
*  APP_PHY_RESET_open must be executed initially to be able to reset PHYs.
*
*  \param[in]     addr_p    PHY Address
*
*  \return SystemP_SUCCESS on success, else failure
*/
int32_t APP_PHY_RESET_all(void)
{
    int32_t status = SystemP_FAILURE;
    uint32_t i = 0;
    uint32_t elems = sizeof(APP_PHY_RESET_indexList_g) / sizeof(APP_PHY_RESET_indexList_g[0]);

    for (i = 0; i < elems; i++)
    {
        status = TCA6424_setOutput(&APP_PHY_RESET_ioExpConfig_g, APP_PHY_RESET_indexList_g[i].ioExpIndex,
            TCA6424_OUT_STATE_LOW);
        if (status != SystemP_SUCCESS)
        {
            break;
        }

        ClockP_usleep(APP_PHY_RESET_PULSE_DUR_US);

        status = TCA6424_setOutput(&APP_PHY_RESET_ioExpConfig_g, APP_PHY_RESET_indexList_g[i].ioExpIndex,
            TCA6424_OUT_STATE_HIGH);
        if (status != SystemP_SUCCESS)
        {
            break;
        }
    }

    return status;
}