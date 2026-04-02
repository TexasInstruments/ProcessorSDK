/******************************************************************************
 * Copyright (c) 2020 Texas Instruments Incorporated - http://www.ti.com
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
 *  \file   diag_common_cfg.c
 *
 *  \brief  Common functions used by diagnostic tests modules.
 *
 *  This file contains the utils functions which are common for different
 *  interfaces of given platform and used by diagnostic test modules.
 *
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/csl/tistdtypes.h>
#include <ti/board/board.h>
#include <diag_common_cfg.h>
#include <ti/osal/osal.h>

#include <ti/drv/i2c/i2c.h>
#include <ti/drv/i2c/soc/i2c_soc.h>
#include <ti/drv/uart/UART_stdio.h>

/**
 * \brief  Diagnostic init function
 *
 * This function includes the common initialization sequence for 
 * diagnostic tests.
 *
 * \return
 *          1  - In case of success
 *          0  - In case of failure
 *
 */
Board_STATUS DIAG_init(void)
{
    Board_initCfg boardCfg;
    Board_STATUS status;
    
#ifdef PDK_RAW_BOOT
    boardCfg = BOARD_INIT_DEFAULT;
#else
    boardCfg = BOARD_INIT_UART_STDIO | BOARD_INIT_PINMUX_CONFIG;
#endif

    status = Board_init(boardCfg);
    
    return(status);
}

/**
 * \brief  Compares two data buffers
 *
 * This function verifies the data in two buffers passed as parameters and
 * returns the result. 'failIndex' contains the index of the first mismatch
 * when buffer comparision fails.
 *
 * \param   buf1      [IN]   Data buffer 1
 * \param   buf2      [IN]   Data buffer 2
 * \param   length    [IN]   Length of the buffers
 * \param   failIndex [OUT]  Index of first data mismatch
 *
 * \return
 *          1  - In case of success
 *          0  - In case of failure
 *
 */
bool BoardDiag_memCompare(uint8_t *buf1, uint8_t *buf2, uint32_t length,  
                          uint32_t *failIndex)
{
    uint32_t idx = 0;
    bool match   = 1;
    
    for(idx = 0; ((idx < length) && (match == 1)); idx++)
    {
        if(buf1[idx] != buf2[idx])
        {
            match      = 0;
            *failIndex = idx;
        }            
    }

    return (match);
}

/**
 * \brief  Generate test pattern function
 *
 * This function fills the data buffer passed as input parameter with
 * different test patterns as indicated by 'flag' parameter.
 * Below are the different test patterns supported
 *
 * BOARD_DIAG_TEST_PATTERN_FF     - Fills the buffer with 0xFF
 * BOARD_DIAG_TEST_PATTERN_AA     - Fills the buffer with 0xAA
 * BOARD_DIAG_TEST_PATTERN_55     - Fills the buffer with 0x55
 * BOARD_DIAG_TEST_PATTERN_NULL   - Fills buffer with 0
 * BOARD_DIAG_TEST_PATTERN_RANDOM - Fills the buffer with randon pattern
 * BOARD_DIAG_TEST_PATTERN_INC    - Fills the buffer with increment pattern
 *                                  starting from 0
 * BOARD_DIAG_TEST_PATTERN_AA_55  - Fills the buffer with 0xAA in even locations
 *                                  0x55 in odd locations
 *
 * If 'flag' does match with any of the above values, buffer will be filled
 * with 0 by default.
 *
 * \param   buf    [IN]   Buffer to fill with the test pattern
 * \param   length [IN]   Length of the buffer
 * \param   flag   [IN]   Flag to set the test pattern
 *
 */
void BoardDiag_genPattern(uint8_t *buf, uint32_t length, uint8_t flag)
{
    uint8_t data         = 0;
    uint8_t incFlag      = 0;
    uint8_t randFlag     = 0;
    uint8_t checkerBoard = 0;
    uint32_t idx;

    switch(flag)
    {
        case BOARD_DIAG_TEST_PATTERN_FF:
        case BOARD_DIAG_TEST_PATTERN_AA:
        case BOARD_DIAG_TEST_PATTERN_55:
        case BOARD_DIAG_TEST_PATTERN_NULL:
             data = flag;
             break;
        case BOARD_DIAG_TEST_PATTERN_INC:
             data    = 0;
             incFlag = 1;
             break;
        case BOARD_DIAG_TEST_PATTERN_RANDOM:
             data     = rand();
             randFlag = 1;
        case BOARD_DIAG_TEST_PATTERN_AA_55:
             data         = 0xAA;
             checkerBoard = 1;
             break;
        default:
             data = flag;
             break;
    }

    for(idx = 0; idx < length; idx++)
    {
        buf[idx] = data;
        
        if(incFlag)
            data++;
        
        if(randFlag)
            data = rand();

        if(checkerBoard)
            data = ~data;
    }
}

/**
 * \brief    This API reads a byte from the Receiver Buffer Register
 *           (RBR). It checks once if any character is ready to be read.
 *
 * Note: When UART driver is used through STDIO interface,it is not possible
 * to read the data without waiting forever. This API will be useful in such 
 * cases.
 *
 * \param    instance     Memory address of the UART instance being used.
 *
 * \return   value of RHR register
 */

int8_t BoardDiag_getUserInput(uint8_t instance)
{
    return 0;
}

/**
 * \brief    This API Enables the main I2C instances
 *
 * \param    instance [IN] I2C instance number.
 * \param    baseAddr [IN] Register base address of the I2C instance.
 */
void enableMAINI2C(uint8_t instance, uint32_t baseAddr)
{
    BoardDiag_enableI2C(instance, baseAddr);
}

/**
 * \brief    This API Enables the I2C instances
 *
 * \param    instance [IN] I2C instance.
 * \param    baseAddr [IN] Register base address of the I2C instance.
 */
void BoardDiag_enableI2C(uint8_t instance, uint32_t baseAddr)
{
    I2C_HwAttrs i2cCfg;

    I2C_socGetInitCfg(instance, &i2cCfg);

    i2cCfg.baseAddr = baseAddr;
    i2cCfg.enableIntr = 0;

    I2C_socSetInitCfg(instance, &i2cCfg);
}

/**
 * \brief  Function to disable timer interrupts
 *
 * OSAL uses timer interrupt to track the timestamp overflow.
 * Timer interrupts triggered while diagnostic test is invoked from
 * framework are causing exception. This function disables timer interrupts.
 * All the diagnostic tests should call this function befor main
 * to avoid exceptions while executing on R5 core.
 *
 */
void BoardDiag_timerIntrDisable(void)
{

}

int32_t start_boot_diag(void)
{
    return 0;
}
