/*
 * Copyright (C) 2023 Texas Instruments Incorporated - http://www.ti.com/
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

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */
/* None */

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* SBL_LOCKSTEP_TEST_NUM_BOOT_CORES indicates number of R5 clusters other than mcu1_0 cluster in SOC */
#if defined (SOC_J7200)
#define SBL_LOCKSTEP_TEST_NUM_BOOT_CORES (1)
#elif defined (SOC_J721S2) || defined (SOC_J721E)
#define SBL_LOCKSTEP_TEST_NUM_BOOT_CORES (2)
#elif defined (SOC_J784S4) || defined (SOC_J742S2)
#define SBL_LOCKSTEP_TEST_NUM_BOOT_CORES (3)
#endif

/* Core boots write UPDATED_VALUE to the following address */
#define POKE_MEM_ADDR_MCU2_0 0x41CE1FFC
#define POKE_MEM_ADDR_MCU3_0 0x41CE2FFC
#define POKE_MEM_ADDR_MCU4_0 0x41CE3FFC

#define POKE_MEM_ADDR_GAP 0x400
#define UPDATE_VALUE 0xC0DEBABE
#define DEFAULT_VALUE 0xFEEDFACE 

/* BOOT_DELAY is used to halt the core for some time using while(boot_delay--) */
#ifdef BUILD_MCU1_0
    #define CORE_NAME "MCU1_0"
    #define BOOT_DELAY (0x1000000)
    __attribute__((section(".sbl_mcu_1_0_resetvector"))) void sbl_puts(char *str);
#endif

#ifdef BUILD_MCU2_0
    #define CORE_NAME "MCU2_0"
    #define BOOT_DELAY (0x300000)
    #define POKE_MEM_ADDR (POKE_MEM_ADDR_MCU2_0)
    __attribute__((section(".sbl_mcu_2_0_resetvector"))) void sbl_puts(char *str);
#endif

#ifdef BUILD_MCU3_0
    #define CORE_NAME "MCU3_0"
    #define BOOT_DELAY (0x400000)
    #define POKE_MEM_ADDR (POKE_MEM_ADDR_MCU3_0)
    __attribute__((section(".sbl_mcu_3_0_resetvector"))) void sbl_puts(char *str);
#endif

#ifdef BUILD_MCU4_0
    #define CORE_NAME "MCU4_0"
    #define BOOT_DELAY (0x500000)
    #define POKE_MEM_ADDR (POKE_MEM_ADDR_MCU4_0)
    __attribute__((section(".sbl_mcu_4_0_resetvector"))) void sbl_puts(char *str);
#endif

