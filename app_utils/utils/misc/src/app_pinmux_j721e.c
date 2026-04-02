/*
 *
 * Copyright (c) 2019 Texas Instruments Incorporated
 *
 * All rights reserved not granted herein.
 *
 * Limited License.
 *
 * Texas Instruments Incorporated grants a world-wide, royalty-free, non-exclusive
 * license under copyrights and patents it now or hereafter owns or controls to make,
 * have made, use, import, offer to sell and sell ("Utilize") this software subject to the
 * terms herein.  With respect to the foregoing patent license, such license is granted
 * solely to the extent that any such patent is necessary to Utilize the software alone.
 * The patent license shall not apply to any combinations which include this software,
 * other than combinations with devices manufactured by or for TI ("TI Devices").
 * No hardware patent is licensed hereunder.
 *
 * Redistributions must preserve existing copyright notices and reproduce this license
 * (including the above copyright notice and the disclaimer and (if applicable) source
 * code license limitations below) in the documentation and/or other materials provided
 * with the distribution
 *
 * Redistribution and use in binary form, without modification, are permitted provided
 * that the following conditions are met:
 *
 * *       No reverse engineering, decompilation, or disassembly of this software is
 * permitted with respect to any software provided in binary form.
 *
 * *       any redistribution and use are licensed by TI for use only with TI Devices.
 *
 * *       Nothing shall obligate TI to provide you with source code for the software
 * licensed and provided to you in object code.
 *
 * If software source code is provided to you, modification and redistribution of the
 * source code are permitted provided that the following conditions are met:
 *
 * *       any redistribution and use of the source code, including any resulting derivative
 * works, are licensed by TI for use only with TI Devices.
 *
 * *       any redistribution and use of any object code compiled from the source code
 * and any resulting derivative works, are licensed by TI for use only with TI Devices.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of its suppliers
 *
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * DISCLAIMER.
 *
 * THIS SOFTWARE IS PROVIDED BY TI AND TI'S LICENSORS "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL TI AND TI'S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/board/board.h>
#include <utils/misc/include/app_misc.h>
#include <ti/board/src/j721e_evm/include/board_pinmux.h>
/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

static pinmuxPerCfg_t gVout0PinCfg[] =
{
    /* MyVOUT1 -> VOUT0_DATA0 -> AE22 */
    {
        PIN_PRG1_PRU1_GPO0, PIN_MODE(10) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MyVOUT1 -> VOUT0_DATA1 -> AG23 */
    {
        PIN_PRG1_PRU1_GPO1, PIN_MODE(10) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MyVOUT1 -> VOUT0_DATA2 -> AF23 */
    {
        PIN_PRG1_PRU1_GPO2, PIN_MODE(10) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MyVOUT1 -> VOUT0_DATA3 -> AD23 */
    {
        PIN_PRG1_PRU1_GPO3, PIN_MODE(10) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MyVOUT1 -> VOUT0_DATA4 -> AH24 */
    {
        PIN_PRG1_PRU1_GPO4, PIN_MODE(10) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MyVOUT1 -> VOUT0_DATA5 -> AG21 */
    {
        PIN_PRG1_PRU1_GPO5, PIN_MODE(10) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MyVOUT1 -> VOUT0_DATA6 -> AE23 */
    {
        PIN_PRG1_PRU1_GPO6, PIN_MODE(10) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MyVOUT1 -> VOUT0_DATA7 -> AC21 */
    {
        PIN_PRG1_PRU1_GPO7, PIN_MODE(10) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MyVOUT1 -> VOUT0_DATA8 -> Y23 */
    {
        PIN_PRG1_PRU1_GPO8, PIN_MODE(10) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MyVOUT1 -> VOUT0_DATA9 -> AF21 */
    {
        PIN_PRG1_PRU1_GPO9, PIN_MODE(10) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MyVOUT1 -> VOUT0_DATA10 -> AB23 */
    {
        PIN_PRG1_PRU1_GPO10, PIN_MODE(10) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MyVOUT1 -> VOUT0_DATA11 -> AJ25 */
    {
        PIN_PRG1_PRU1_GPO11, PIN_MODE(10) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MyVOUT1 -> VOUT0_DATA12 -> AH25 */
    {
        PIN_PRG1_PRU1_GPO12, PIN_MODE(10) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MyVOUT1 -> VOUT0_DATA13 -> AG25 */
    {
        PIN_PRG1_PRU1_GPO13, PIN_MODE(10) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MyVOUT1 -> VOUT0_DATA14 -> AH26 */
    {
        PIN_PRG1_PRU1_GPO14, PIN_MODE(10) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MyVOUT1 -> VOUT0_DATA15 -> AJ27 */
    {
        PIN_PRG1_PRU1_GPO15, PIN_MODE(10) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MyVOUT1 -> VOUT0_DATA16 -> AF24 */
    {
        PIN_PRG1_PRU0_GPO11, PIN_MODE(10) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MyVOUT1 -> VOUT0_DATA17 -> AJ24 */
    {
        PIN_PRG1_PRU0_GPO12, PIN_MODE(10) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MyVOUT1 -> VOUT0_DATA18 -> AG24 */
    {
        PIN_PRG1_PRU0_GPO13, PIN_MODE(10) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MyVOUT1 -> VOUT0_DATA19 -> AD24 */
    {
        PIN_PRG1_PRU0_GPO14, PIN_MODE(10) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MyVOUT1 -> VOUT0_DATA20 -> AC24 */
    {
        PIN_PRG1_PRU0_GPO15, PIN_MODE(10) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MyVOUT1 -> VOUT0_DATA21 -> AE24 */
    {
        PIN_PRG1_PRU0_GPO16, PIN_MODE(10) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MyVOUT1 -> VOUT0_DATA22 -> AJ20 */
    {
        PIN_PRG1_PRU0_GPO8, PIN_MODE(10) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MyVOUT1 -> VOUT0_DATA23 -> AG20 */
    {
        PIN_PRG1_PRU0_GPO9, PIN_MODE(10) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MyVOUT1 -> VOUT0_DE -> AC22 */
    {
        PIN_PRG1_PRU1_GPO17, PIN_MODE(10) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MyVOUT1 -> VOUT0_EXTPCLKIN -> AH21 */
    {
        PIN_PRG1_PRU0_GPO19, PIN_MODE(10) | \
        ((PIN_PULL_DISABLE | PIN_INPUT_ENABLE) & (~PIN_PULL_DIRECTION))
    },
    /* MyVOUT1 -> VOUT0_HSYNC -> AJ26 */
    {
        PIN_PRG1_PRU1_GPO16, PIN_MODE(10) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MyVOUT1 -> VOUT0_PCLK -> AH22 */
    {
        PIN_PRG1_PRU1_GPO19, PIN_MODE(10) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },
    /* MyVOUT1 -> VOUT0_VSYNC -> AJ22 */
    {
        PIN_PRG1_PRU1_GPO18, PIN_MODE(10) | \
        ((PIN_PULL_DISABLE) & (~PIN_PULL_DIRECTION & ~PIN_INPUT_ENABLE))
    },

    {PINMUX_END}
};

static pinmuxPerCfg_t gI2c1PinCfg[] =
{
    /* I2C1 is required to configure onboard mux to enable HDMI */
    /* MyI2C1 -> I2C1_SCL -> Y6 */
    {
        PIN_I2C1_SCL, PIN_MODE(0) | \
        ((PIN_PULL_DIRECTION | PIN_INPUT_ENABLE) & (~PIN_PULL_DISABLE))
    },
    /* MyI2C1 -> I2C1_SDA -> AA6 */
    {
        PIN_I2C1_SDA, PIN_MODE(0) | \
        ((PIN_PULL_DIRECTION | PIN_INPUT_ENABLE) & (~PIN_PULL_DISABLE))
    },

    {PINMUX_END}
};

static pinmuxModuleCfg_t gDispPinCfg[] =
{
    {0, 1, gVout0PinCfg},
    {1, 1, gI2c1PinCfg},
    {PINMUX_END}
};

static pinmuxPerCfg_t gI2c6PinCfg[] =
{
    /* MyI2C6 -> I2C6_SCL -> AA3 */
    {
        PIN_SPI0_D1, PIN_MODE(2) | \
        ((PIN_PULL_DIRECTION | PIN_INPUT_ENABLE) & (~PIN_PULL_DISABLE))
    },
    /* MyI2C6 -> I2C6_SDA -> Y2 */
    {
        PIN_SPI1_D1, PIN_MODE(2) | \
        ((PIN_PULL_DIRECTION | PIN_INPUT_ENABLE) & (~PIN_PULL_DISABLE))
    },
    {PINMUX_END}
};

static pinmuxModuleCfg_t gCaptPinCfg[] =
{
    {0, 1, gI2c6PinCfg},
    {PINMUX_END}
};

static pinmuxBoardCfg_t gBasicDemoPinmuxDataInfo[] =
{
    {0, gDispPinCfg},
    {1, gCaptPinCfg},
    {PINMUX_END}
};

/* ========================================================================== */
/*                  Internal/Private Function Declarations                    */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void appPinMuxCfgSetDefault(app_pinmux_cfg_t *cfg)
{
    if (NULL != cfg)
    {
        cfg->enable_hdmi = 0;
        cfg->enable_i2c  = 0;
    }
}

void appSetPinmux(app_pinmux_cfg_t *cfg)
{
    if (NULL != cfg)
    {
        if (1 == cfg->enable_i2c)
        {
            /* Enable Pinmux for I2C0 */
            gDispPinCfg[1].doPinConfig = 1;
        }
        else
        {
            /* Disable Pinmux for I2C0 */
            gDispPinCfg[1].doPinConfig = 0;
        }
        if (1 == cfg->enable_hdmi)
        {
            /* Enable Pinmux for vout0 */
            gDispPinCfg[0].doPinConfig = 1;
        }
        else
        {
            /* Disable Pinmux for vout0 */
            gDispPinCfg[0].doPinConfig = 0;
        }

    }

    Board_pinmuxUpdate(gBasicDemoPinmuxDataInfo,
                       BOARD_SOC_DOMAIN_MAIN);
}
