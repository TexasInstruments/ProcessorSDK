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

--ram_model
--args 0x1000
--diag_suppress=10068 /* "no matching section" */
--cinit_compression=off
-e _c_int00_secure

-heap  0x20000      /* HEAP AREA SIZE                */
-stack 0x20000      /* SOFTWARE STACK SIZE           */

#define DDR0_ALLOCATED_START  0xA0000000
#define C7X_ALLOCATED_START DDR0_ALLOCATED_START + 0x06000000

#define C7X_EXT_DATA_BASE   (C7X_ALLOCATED_START + 0x00100000)
#define C7X_MEM_TEXT_BASE   (C7X_ALLOCATED_START + 0x00200000)
#define C7X_MEM_DATA_BASE   (C7X_ALLOCATED_START + 0x00300000)
#define C7X_DDR_SPACE_BASE  (C7X_ALLOCATED_START + 0x00400000)

/*--------------------------------------------------------------------------*/
/*                               Memory Map                                 */
/*--------------------------------------------------------------------------*/
MEMORY
{
    DDR0_RESERVED: org = 0x80000000,          len = 0x20000000   /* 512MB Reserved for A72 OS */
    C7X_IPC_D:     org = C7X_ALLOCATED_START, len = 0x00100000   /*  1MB DDR */
    C7X_EXT_D:     org = C7X_EXT_DATA_BASE,   len = 0x00100000   /*  1MB DDR */
    C7X_TEXT:      org = C7X_MEM_TEXT_BASE,   len = 0x00100000   /*  1MB DDR */
    C7X_DATA:      org = C7X_MEM_DATA_BASE,   len = 0x00100000   /*  1MB DDR */
    C7X_DDR_SPACE: org = C7X_DDR_SPACE_BASE,  len = 0x00C00000   /* 12MB DDR */
}

/*----------------------------------------------------------------------------*/
/* Section Configuration                                                      */
/*----------------------------------------------------------------------------*/
SECTIONS
{
    boot:
    {
      boot.*<boot.oe71>(.text)
    } load > C7X_TEXT ALIGN(0x200000)
    
/* Code sections. */
    GROUP LOAD_START( lnkStartFlashAddress ),
          LOAD_END( lnkEndFlashAddress )
    {
        .vecs                   : {} ALIGN(0x400000)
        .interrupt_vectors      : {} ALIGN(0x400000)
        .secure_vecs            : {} ALIGN(0x200000)
        .text:_c_int00_secure   : {} ALIGN(0x200000)
        .text                   : {} ALIGN(0x200000)

        .cinit                  : {}  /* could be part of const */
        .const                  : {}
        .KERNEL_FUNCTION LOAD_START( lnkKernelFuncStartAddr ),
                         LOAD_END( lnkKernelFuncEndAddr )
                                : {} palign( 0x10000 )
    } > C7X_DDR_SPACE

/* Data sections. */
    GROUP  palign( 0x10000 ), LOAD_START( lnkRamStartAddr ), LOAD_END( lnkRamEndAddr )
    {
        .bss                    : {}  /* Zero-initialized data */
        .data                   : {}  /* Initialized data */

        .init_array             : {}  /* C++ initializations */
        .stack                  : {} ALIGN(0x10000)
        .args                   : {}
        .cio                    : {}
        .switch                 : {} /* For exception handling. */
        .sysmem      /* heap */
        .KERNEL_DATA LOAD_START( lnkKernelDataStartAddr ),
                     LOAD_END( lnkKernelDataEndAddr )
                                : {} palign( 0x800 )
    } > C7X_DDR_SPACE_BASE

    GROUP:              >  C7X_DDR_SPACE
    {
        .data.Mmu_tableArray          : type=NOINIT
        .data.Mmu_tableArraySlot      : type=NOINIT
        .data.Mmu_level1Table         : type=NOINIT
        .data.Mmu_tableArray_NS       : type=NOINIT
        .data.Mmu_tableArraySlot_NS   : type=NOINIT
        .data.Mmu_level1Table_NS      : type=NOINIT
    }

    ipc_data_buffer:       > C7X_DDR_SPACE
    .benchmark_buffer:     > C7X_DDR_SPACE ALIGN (32)

    .resource_table: { __RESOURCE_TABLE = .;} > C7X_EXT_D

/* Additional sections settings. */
}

/*-------------------------------- END ---------------------------------------*/
