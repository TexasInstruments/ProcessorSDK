/*
 * Copyright (C) 2014 - 2024 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
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
 *
 */

/**
 *  \file   i2c_board.h
 *
 *  \brief  Board header file.
 *
 */

#ifndef _I2C_BOARD_H
#define _I2C_BOARD_H

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* Example/Board Header files */
#include <ti/board/board.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define I2C_EEPROM_ADDR_SIZE     2   /* in bytes */
#define I2C_EEPROM_TEST_LENGTH   10  /* Read (and write) test in bytes */

#ifdef BOARD_I2C_EEPROM_INSTANCE
#define I2C_EEPROM_INSTANCE      BOARD_I2C_EEPROM_INSTANCE
#else
#define I2C_EEPROM_INSTANCE      0
#endif

#ifdef BOARD_I2C_EEPROM_ADDR
#define I2C_EEPROM_ADDR          BOARD_I2C_EEPROM_ADDR
#else
#define I2C_EEPROM_ADDR          0x50
#endif

/* AM/K1 platform read only test */
#undef I2C_EEPROM_WRITE_ENABLE

#define I2C_EEPROM_TEST_ADDR     0x0001           /* Read and write test start address */

#ifdef __cplusplus
}
#endif

#endif /* _I2C_BOARD_H */
