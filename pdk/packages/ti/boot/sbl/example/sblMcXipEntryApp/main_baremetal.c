/*
 *  Copyright (c) Texas Instruments Incorporated 2021
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
 */

/**
 *  \file main_baremetal.c
 *
 *  \brief Main file for baremetal build that jumps into sbl xip application
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* CSL Header files */
#ifdef BUILD_MCU2_0
#include <stdint.h>
#include <ti/board/board.h>
#endif

int main(void)
{

/*  To simplify running multicore apps, mcu1_0 by default
 *  will run from 0x501c0000 and mcu2_0 by default will
 *  run from 0x508c0000. Note that these constraints are
 *  arbitrary. Each core can start its application from
 *  anywhere in the FLASH memory as long as you do 2 things.
 *
 *  1. Be sure to place the vecs section from the linker 
 *     file at that location within the flash.
 *  2. When flashing using the uniflash tool, be sure to 
 *     use the correct offset as the starting point. In this
 *     case, mcu1_0 xer5f.bin files are flashed to 0x1c0000
 *     and mcu2_0 xer5f.bin files are flashed to 0x8c0000.
 *  
 *  The appimage associated with building this file should 
 *  always be flashed to 0x100000.
 */
#ifdef BUILD_MCU1_0
    ((void(*)(void)) 0x501c0000)();
#else
    ((void(*)(void)) 0x508c0000)();
#endif
    return(0);
}
