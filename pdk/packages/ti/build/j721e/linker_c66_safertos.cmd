/*
 *
 * Copyright (c) 2023 Texas Instruments Incorporated
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
 
/*=========================*/
/*     Linker Settings     */
/*=========================*/

-u InterruptVectorTable

-stack  0x2000                              /* SOFTWARE STACK SIZE           */
-heap   0x1000                              /* HEAP AREA SIZE                */

#define DDR0_CODE_START             ( 0x80000000 )
#define DEFAULT_SECTION_ALIGNMENT   ( 32 )

/*--------------------------------------------------------------------------*/
/*                               Memory Map                                 */
/*--------------------------------------------------------------------------*/
MEMORY
{
    L2SRAM                :   o = 0x00800000 l = 0x00048000   /* 288KB LOCAL L2/SRAM */
    L1PSRAM               :   o = 0x00E00000 l = 0x00008000   /* 32KB LOCAL L1P/SRAM */
    L1DSRAM               :   o = 0x00F00000 l = 0x00008000   /* 32KB LOCAL L1D/SRAM */

    /*========================J721E MCMS3 LOCATIONS ===================*/
    /*---------- J721E Reserved Memory for ARM Trusted Firmware -------*/
    MSMC3_ARM_FW   (RWIX)   : o = 0x70000000, l = 0x20000       /* 128KB */
    MSMC3 (RWIX)            : o = 0x70020000  l = 0x7D0000      /* 8MB - 192KB */
    /*------------- J721E Reserved Memory for DMSC Firmware -----------*/
    MSMC3_DMSC_FW  (RWIX)   : o = 0x707F0000, l = 0x10000       /* 64KB */

    /*======================= J721E DDR LOCATION =======================*/
    DDR0        :   o = 0x80000000 l = 0x20000000   /* 512MB DDR0 */
}

/*----------------------------------------------------------------------------*/
/* Section Configuration                                                      */
/*----------------------------------------------------------------------------*/
SECTIONS
{    
   	/* For SafeRTOS based applications the following symbols need to be exported:
	 *
	 * lnkFlashStartAddr - the start of the Flash memory section used during runtime.
	 *
	 * IMPORTANT NOTE: lnkFlashStartAddr can be relocated (e.g. when a
	 * bootloader is used), but it must be aligned according to its size
	 * (rounded up to the next power of 2), otherwise the MPU will be
	 * configured incorrectly.
	 *
	 * lnkFlashEndAddr - the end of the Flash memory section used during runtime.
	 *
	 * lnkKernelFuncStartAddr - the start address of the Flash block containing
	 * the interrupt vector table and the kernel functions.
	 * NOTE: This should be aligned according to the section size, i.e. 0x8000.
	 *
	 * lnkKernelFuncEndAddr - the end address of the Flash block containing
	 * the interrupt vector table and the kernel functions.
	 *
	 * lnkKernelDataStartAddr - the start address of the RAM block containing
	 * the system stacks and the kernel data.
	 *
	 * lnkKernelDataEndAddr - the end address of the RAM block containing
	 * the system stacks and the kernel data.
	 */

    /* hard addresses forces vecs to be allocated there */
   
    .interrupt_vectors : {. = align(32); }  > DDR0_CODE_START

    .priv_code          palign( DEFAULT_SECTION_ALIGNMENT ),
                        LOAD_START( lnkKernelFuncStartAddr ),
                        LOAD_END( lnkKernelFuncEndAddr ) :
    {
        *(.KERNEL_FUNCTION)
    } > DDR0

    .text:_c_int00      load >  DDR0 ALIGN(0x10000)
    .text:              load >  DDR0 palign( DEFAULT_SECTION_ALIGNMENT ), 
                                     LOAD_START( lnkStartFlashAddress )

    .unpriv_rodata      palign( DEFAULT_SECTION_ALIGNMENT ) :
    {
        *(.const)
        *(.switch)
    } > DDR0

    .cinit              LOAD_END( lnkEndFlashAddress )          : {} > DDR0

    GROUP               palign( DEFAULT_SECTION_ALIGNMENT ),
                        LOAD_START( lnkUnprivilegedDataStartAddr )
    {
        .rodata:  
        .bss:
        .neardata:
        .KERNEL_DATA        palign( DEFAULT_SECTION_ALIGNMENT ),
                            LOAD_START( lnkKernelDataStartAddr ),
                            LOAD_END( lnkKernelDataEndAddr )
    } > DDR0

    GROUP               palign( DEFAULT_SECTION_ALIGNMENT ),
                        LOAD_END( lnkUnprivilegedDataEndAddr )
    {
        .cio:
        .data:
        .sysmem:
        .fardata:
        .far:
    } > DDR0

    .stack              palign( DEFAULT_SECTION_ALIGNMENT ),
                        LOAD_START( lnkStacksStartAddr )        : {} > DDR0 
}

/*-------------------------------- END ---------------------------------------*/
