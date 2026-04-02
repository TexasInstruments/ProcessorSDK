/*!
* \file phy_reset.c
*
* \brief
* AM243x LP specific PHY reset component implementation
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
#include <kernel/dpl/AddrTranslateP.h>
#include <drivers/gpio.h>

#include "ti_drivers_config.h"
#include "ti_drivers_open_close.h"
#include "ti_board_open_close.h"

#include "../appPhyReset.h"

#define PIN_NOT_FOUND          -1   /* \brief Specified pin was not found */
#define BASE_ADDR_NOT_FOUND    -1   /* \brief Specified base address of the pin was not found */

/*!
 *  \brief GPIO-specific PHY reset objects
 */
typedef struct APP_PHY_RESET_SIndexList
{
    uint32_t phyAddr;
    uint32_t ioPin;
    uint32_t baseAddr;
} APP_PHY_RESET_SIndexList_t;

/*!
 *  \brief List of PHY reset objects
 */
static APP_PHY_RESET_SIndexList_t PHY_RESET_indexList_g[] =
{
  {
    3,                                      /* PHY addr */
    CONFIG_GPIO0_PHY0_RESET_PIN,            /* GPIO Pin */
    CONFIG_GPIO0_PHY0_RESET_BASE_ADDR       /* GPIO Base Addr */
  },
  {
    15,                                     /* PHY addr */
    CONFIG_GPIO0_PHY1_RESET_PIN,            /* GPIO Pin */
    CONFIG_GPIO0_PHY1_RESET_BASE_ADDR       /* GPIO Base Addr */
  },
};

/*!
*  Function: APP_PHY_RESET_getPin
*
*  \brief
*  Get PHY reset pin for a PHY with specified address.
*
*  \details
*  Searches through the PHY list for a PHY reset object with specified PHY address.
*  Returns corresponding pin, if available.
*
*  \param[in]     addr_p     PHY Address.
*
*  \return        pin        Pin of the specified PHY, if found. Otherwise #PIN_NOT_FOUND.
*/
static int32_t APP_PHY_RESET_getPin(uint32_t addr_p)
{
    uint32_t i = 0;
    uint32_t elems = sizeof(PHY_RESET_indexList_g) / sizeof(PHY_RESET_indexList_g[0]);
    int32_t pin = PIN_NOT_FOUND;

    for (i = 0; i < elems; i++)
    {
        if (PHY_RESET_indexList_g[i].phyAddr == addr_p)
        {
            pin = (int32_t)PHY_RESET_indexList_g[i].ioPin;
            break;
        }
    }

    return pin;
}

/*!
*  Function: APP_PHY_RESET_getBaseAddr
*
*  \brief
*  Get PHY reset pin base address for a PHY with specified address.
*
*  \details
*  Searches through the PHY list for a PHY reset object with specified PHY address.
*  Returns corresponding base address, if available.
*
*  \param[in]     addr_p     PHY Address.
*
*  \return        baseAddr   Base address of the specified PHY, if available. Otherwise #BASE_ADDR_NOT_FOUND.
*/
static int32_t APP_PHY_RESET_getBaseAddr(uint32_t addr_p)
{
    uint32_t i = 0;
    uint32_t elems = sizeof(PHY_RESET_indexList_g) / sizeof(PHY_RESET_indexList_g[0]);
    int32_t baseAddr = BASE_ADDR_NOT_FOUND;

    for (i = 0; i < elems; i++)
    {
        if (PHY_RESET_indexList_g[i].phyAddr == addr_p)
        {
            baseAddr = (int32_t)PHY_RESET_indexList_g[i].baseAddr;
            break;
        }
    }

    return baseAddr;
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
*  \warnings
*  It is imporant to set reset pin to the HIGH state and the configure it as output. 
*  Otherwise accidental PHY reset can occur, as PHY reset is active LOW.
*
*  \param[in]     None.
*
*  \return SystemP_SUCCESS on success, else failure
*/
int32_t APP_PHY_RESET_open(void)
{
    int32_t status = SystemP_SUCCESS;
    uint32_t i = 0;
    uint32_t elems = sizeof(PHY_RESET_indexList_g) / sizeof(PHY_RESET_indexList_g[0]);
    for (i = 0; i < elems; i++)
    {
        uint32_t translatedAddr =
            (uint32_t)AddrTranslateP_getLocalAddr(PHY_RESET_indexList_g[i].baseAddr);
        GPIO_pinWriteHigh(translatedAddr, PHY_RESET_indexList_g[i].ioPin);
        GPIO_setDirMode(translatedAddr, PHY_RESET_indexList_g[i].ioPin, GPIO_DIRECTION_OUTPUT);
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
    return;
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
    int32_t pin = APP_PHY_RESET_getPin(addr_p);
    int32_t baseAddr = APP_PHY_RESET_getBaseAddr(addr_p);

    if ((pin != PIN_NOT_FOUND) && (baseAddr != BASE_ADDR_NOT_FOUND))
    {
        uint32_t translatedAddr = (uint32_t)AddrTranslateP_getLocalAddr(baseAddr);
        GPIO_pinWriteLow(translatedAddr, pin);

        ClockP_usleep(APP_PHY_RESET_PULSE_DUR_US);

        GPIO_pinWriteHigh(translatedAddr, pin);
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
    int32_t status = SystemP_SUCCESS;
    uint32_t i = 0;
    uint32_t elems = sizeof(PHY_RESET_indexList_g) / sizeof(PHY_RESET_indexList_g[0]);

    for (i = 0; i < elems; i++)
    {
        uint32_t translatedAddr =
            (uint32_t)AddrTranslateP_getLocalAddr(PHY_RESET_indexList_g[i].baseAddr);
        GPIO_pinWriteLow(translatedAddr, PHY_RESET_indexList_g[i].ioPin);

        ClockP_usleep(APP_PHY_RESET_PULSE_DUR_US);

        GPIO_pinWriteHigh(translatedAddr, PHY_RESET_indexList_g[i].ioPin);
    }

    return status;
}