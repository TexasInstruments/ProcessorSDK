/*
 * Copyright (c) 2018-2022, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* SBL_AMP_TEST_NUM_BOOT_CORES indicates number of cores in SOC */
/* BOOTAPP_NUM_BOOT_CORES indicates number of cores booted from lateapps */
#if defined (SOC_J7200)
#define SBL_AMP_TEST_NUM_BOOT_CORES (6)    /* Total number of R5 cores + A72 cores */
#define BOOTAPP_NUM_BOOT_CORES (3)         /* Total number of R5 cores (except mcu1_0 cluster) + mpu1_0 */
#endif
#if defined (SOC_J721S2)
#define SBL_AMP_TEST_NUM_BOOT_CORES (10)   /* Total number of R5 cores + DSP cores + A72 cores */
#define BOOTAPP_NUM_BOOT_CORES (7)         /* Total number of R5 cores (except mcu1_0 cluster) + DSP cores + mpu1_0 */
#endif
#if defined (SOC_J784S4)
#define SBL_AMP_TEST_NUM_BOOT_CORES (20)   /* Total number of R5 cores + DSP cores + A72 cores */
#define BOOTAPP_NUM_BOOT_CORES (11)        /* Total number of R5 cores (except mcu1_0 cluster) + DSP cores + mpu1_0 */
#endif
#if defined (SOC_J742S2)
#define SBL_AMP_TEST_NUM_BOOT_CORES (15)   /* Total number of R5 cores + DSP cores + A72 cores */
#define BOOTAPP_NUM_BOOT_CORES (10)         /* Total number of R5 cores (except mcu1_0 cluster) + DSP cores + mpu1_0 */
#endif
#if defined (SOC_J721E)
#define SBL_AMP_TEST_NUM_BOOT_CORES (11)   /* Total number of R5 cores + DSP cores + A72 cores */
#define BOOTAPP_NUM_BOOT_CORES (8)         /* Total number of R5 cores (except mcu1_0 cluster) + DSP cores + mpu1_0 */
#endif

#define POKE_MEM_ADDR_MCU1_0 0x41CE1FFC
#define POKE_MEM_ADDR_MCU1_1 0x41CE2FFC
#define POKE_MEM_ADDR_MCU2_0 0x41CE3FFC
#define POKE_MEM_ADDR_MCU2_1 0x41CE4FFC
#define POKE_MEM_ADDR_MCU3_0 0x41CE5FFC
#define POKE_MEM_ADDR_MCU3_1 0x41CE6FFC
#define POKE_MEM_ADDR_MCU4_0 0x41CE7FFC
#define POKE_MEM_ADDR_MCU4_1 0x41CE8FFC
#define POKE_MEM_ADDR_C66X_0 0x41CE9FFC
#define POKE_MEM_ADDR_C66X_1 0x41CEAFFC
#define POKE_MEM_ADDR_C7X_0  0x41CEBFFC
#define POKE_MEM_ADDR_C7X_1  0x41CECFFC
#define POKE_MEM_ADDR_C7X_2  0x41CEDFFC
#define POKE_MEM_ADDR_C7X_3  0x41CEEFFC
#define POKE_MEM_ADDR_MPU1_0 0x41CEFFFC
#define POKE_MEM_ADDR_MPU1_1 0x41CF0FFC
#define POKE_MEM_ADDR_MPU1_2 0x41CF1FFC
#define POKE_MEM_ADDR_MPU1_3 0x41CF2FFC
#define POKE_MEM_ADDR_MPU2_0 0x41CF3FFC
#define POKE_MEM_ADDR_MPU2_1 0x41CF4FFC
#define POKE_MEM_ADDR_MPU2_2 0x41CF5FFC
#define POKE_MEM_ADDR_MPU2_3 0x41CF6FFC

#define START_CHECK_ADDR 0x7002DFFC
#define CORE_COUNTER_ADDR 0x7002FFFC
#define SEMAPHORE_ADDR 0x70031FFC
#define PUTS_BUFFER 0x70033FFC //should be 2 bytes
#define STARTED 0xDEADBEEF
#define SEMAPHORE 0xBEEEEF00
#define POKE_MEM_ADDR_GAP 0x400
#define UPDATE_VALUE 0xC0DEBABE
#define DEFAULT_VALUE 0xFEEDFACE 

/* BOOT_DELAY is used to halt the core for some time using while(boot_delay--) */
#ifdef BUILD_MCU1_0
    #define CORE_NAME "MCU1_0"
    #define BOOT_DELAY (0x600000)
    #define POKE_MEM_ADDR (POKE_MEM_ADDR_MCU1_0)
    __attribute__((section(".sbl_mcu_1_0_resetvector"))) void sbl_puts(char *str);
#endif

#ifdef BUILD_MCU1_1
    #define CORE_NAME "MCU1_1"
    #define BOOT_DELAY (0x700000)
    #define POKE_MEM_ADDR (POKE_MEM_ADDR_MCU1_1)
    __attribute__((section(".sbl_mcu_1_1_resetvector"))) void sbl_puts(char *str);
#endif

#ifdef BUILD_MCU2_0
    #define CORE_NAME "MCU2_0"
    #define BOOT_DELAY (0x340000)
    #define POKE_MEM_ADDR (POKE_MEM_ADDR_MCU2_0)
    __attribute__((section(".sbl_mcu_2_0_resetvector"))) void sbl_puts(char *str);
#endif

#ifdef BUILD_MCU2_1
    #define CORE_NAME "MCU2_1"
    #define BOOT_DELAY (0x380000)
    #define POKE_MEM_ADDR (POKE_MEM_ADDR_MCU2_1)
    __attribute__((section(".sbl_mcu_2_1_resetvector"))) void sbl_puts(char *str);
#endif

#ifdef BUILD_MCU3_0
    #define CORE_NAME "MCU3_0"
    #define BOOT_DELAY (0x4C0000)
    #define POKE_MEM_ADDR (POKE_MEM_ADDR_MCU3_0)
    __attribute__((section(".sbl_mcu_3_0_resetvector"))) void sbl_puts(char *str);
#endif

#ifdef BUILD_MCU3_1
    #define CORE_NAME "MCU3_1"
    #define BOOT_DELAY (0x500000)
    #define POKE_MEM_ADDR (POKE_MEM_ADDR_MCU3_1)
    __attribute__((section(".sbl_mcu_3_1_resetvector"))) void sbl_puts(char *str);
#endif

#ifdef BUILD_MCU4_0
    #define CORE_NAME "MCU4_0"
    #define BOOT_DELAY (0x540000)
    #define POKE_MEM_ADDR (POKE_MEM_ADDR_MCU4_0)
    __attribute__((section(".sbl_mcu_4_0_resetvector"))) void sbl_puts(char *str);
#endif

#ifdef BUILD_MCU4_1
    #define CORE_NAME "MCU4_1"
    #define BOOT_DELAY (0x580000)
    #define POKE_MEM_ADDR (POKE_MEM_ADDR_MCU4_1)
    __attribute__((section(".sbl_mcu_4_1_resetvector"))) void sbl_puts(char *str);
#endif

#ifdef BUILD_C66X_1
    #define CORE_NAME "C66X_0"
    #define BOOT_DELAY (0x80000)
    #define POKE_MEM_ADDR (POKE_MEM_ADDR_C66X_0)
    #pragma SET_CODE_SECTION(".sbl_c66x_0_resetvector")
#endif

#ifdef BUILD_C66X_2
    #define CORE_NAME "C66X_1"
    #define BOOT_DELAY (0xA0000)
    #define POKE_MEM_ADDR (POKE_MEM_ADDR_C66X_1)
    #pragma SET_CODE_SECTION(".sbl_c66x_1_resetvector")
#endif

#ifdef BUILD_C7X_1
    #define CORE_NAME "C7X_0"
    #define BOOT_DELAY (0x40000)
    #define POKE_MEM_ADDR (POKE_MEM_ADDR_C7X_0)
    #pragma SET_CODE_SECTION(".sbl_c7x_0_resetvector")
#endif

#ifdef BUILD_C7X_2
    #define CORE_NAME "C7X_1"
    #define BOOT_DELAY (0x80000)
    #define POKE_MEM_ADDR (POKE_MEM_ADDR_C7X_1)
    #pragma SET_CODE_SECTION(".sbl_c7x_1_resetvector")
#endif

#ifdef BUILD_C7X_3
    #define CORE_NAME "C7X_2"
    #define BOOT_DELAY (0xC0000)
    #define POKE_MEM_ADDR (POKE_MEM_ADDR_C7X_2)
    #pragma SET_CODE_SECTION(".sbl_c7x_2_resetvector")
#endif

#ifdef BUILD_C7X_4
    #define CORE_NAME "C7X_3"
    #define BOOT_DELAY (0x100000)
    #define POKE_MEM_ADDR (POKE_MEM_ADDR_C7X_3)
    #pragma SET_CODE_SECTION(".sbl_c7x_3_resetvector")
#endif

#ifdef BUILD_MPU1_0
    #define CORE_NAME "MPU1_0"
    #define BOOT_DELAY (0x140000)
    #define POKE_MEM_ADDR (POKE_MEM_ADDR_MPU1_0)
    int SBLApp_main(void)  __attribute__((section(".sbl_mpu_1_0_resetvector")));
#endif

#ifdef BUILD_MPU1_1
    #define CORE_NAME "MPU1_1"
    #define BOOT_DELAY (0x180000)
    #define POKE_MEM_ADDR (POKE_MEM_ADDR_MPU1_1)
    int SBLApp_main(void)  __attribute__((section(".sbl_mpu_1_1_resetvector")));
#endif

#ifdef BUILD_MPU1_2
    #define CORE_NAME "MPU1_2"
    #define BOOT_DELAY (0x1C0000)
    #define POKE_MEM_ADDR (POKE_MEM_ADDR_MPU1_2)
    int SBLApp_main(void)  __attribute__((section(".sbl_mpu_1_2_resetvector")));
#endif

#ifdef BUILD_MPU1_3
    #define CORE_NAME "MPU1_3"
    #define BOOT_DELAY (0x200000)
    #define POKE_MEM_ADDR (POKE_MEM_ADDR_MPU1_3)
    int SBLApp_main(void)  __attribute__((section(".sbl_mpu_1_3_resetvector")));
#endif

#ifdef BUILD_MPU2_0
    #define CORE_NAME "MPU2_0"
    #define BOOT_DELAY (0x240000)
    #define POKE_MEM_ADDR (POKE_MEM_ADDR_MPU2_0)
    int SBLApp_main(void)  __attribute__((section(".sbl_mpu_2_0_resetvector")));
#endif

#ifdef BUILD_MPU2_1
    #define CORE_NAME "MPU2_1"
    #define BOOT_DELAY (0x280000)
    #define POKE_MEM_ADDR (POKE_MEM_ADDR_MPU2_1)
    int SBLApp_main(void)  __attribute__((section(".sbl_mpu_2_1_resetvector")));
#endif

#ifdef BUILD_MPU2_2
    #define CORE_NAME "MPU2_2"
    #define BOOT_DELAY (0x2C0000)
    #define POKE_MEM_ADDR (POKE_MEM_ADDR_MPU2_2)
    int SBLApp_main(void)  __attribute__((section(".sbl_mpu_2_2_resetvector")));
#endif

#ifdef BUILD_MPU2_3
    #define CORE_NAME "MPU2_3"
    #define BOOT_DELAY (0x300000)
    #define POKE_MEM_ADDR (POKE_MEM_ADDR_MPU2_3)
    int SBLApp_main(void)  __attribute__((section(".sbl_mpu_2_3_resetvector")));
#endif



