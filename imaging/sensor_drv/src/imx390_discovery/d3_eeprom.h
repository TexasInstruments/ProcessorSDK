#ifndef D3_EEPROM_H
#define D3_EEPROM_H

/*
* The MIT License (MIT)
*
* Copyright (C) 2024 Define Design Deploy Corp.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of
* this software and associated documentation files (the "Software"), to deal in
* the Software without restriction, including without limitation the rights to use,
* copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
* Software, and to permit persons to whom the Software is furnished to do so,
* subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*/

/*!
 * @file        d3_eeprom.h
 * @brief       d3_eeprom.c Header File
 * @details     Data structures and defines
 * @copyright   Copyright (C) 2024 Define Design Deploy Corp. All Rights Reserved.
 */

#include <stdint.h>

#define SKIP_EEPROM

// Cameras that are known
typedef enum {
    NOT_RUN,
    UNRECOGNIZED,
    IMX390_WIDE,
    IMX390_MID,
    IMX390_NARROW,
} camera_type_t;

// for ease of reference
#define IMX390_MID_PRO              "D3CMC52N-106-085-S"
#define IMX390_NARROW_PRO           "D3CMC67N-106-085-S"
#define IMX390_WIDE_PRO             "D3CMC70N-106-085-S"
#define IMX390_MID_UNSEALED         "D3CMC68N-106-085"
#define IMX390_NARROW_UNSEALED      "D3CMC69N-106-085"
#define IMX390_WIDE_UNSEALED        "D3CMC70N-106-085"

// 8 bit address
#define M24C64_MEM_I2C    (0x54U << 1U)
#define M24C64_ID_I2C     (0x5CU << 1U)

camera_type_t getCameraTypeFromEEPROM(void* sensorI2cHandle, uint32_t chId);

#endif /* D3_EEPROM_H */