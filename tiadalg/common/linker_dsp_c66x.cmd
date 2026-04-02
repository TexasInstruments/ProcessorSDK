/*
*
* Copyright (c) 2009-2019 Texas Instruments Incorporated
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

-c
-heap  0x20000
-stack 0x8000
--args 0xF000

MEMORY
{
        L1P_CACHE                 : origin = 0x00E00000,  len = 0x8000     /* 32 KB program cache */
        L1D_CACHE                 : origin = 0x00F00000,  len = 0x8000     /* 32 KB data cache */
        L2_SRAM                   : origin = 0x00800000,  len = 0x28000    /* SARAM in L2, = 256 + 32 - 128 = 160 KB*/
        L2_CACHE                  : origin = 0x00828000,  len = 0x20000    /* Cache for L2, which is configured as 128 KB. Nothing should be placed here*/
        ALGO_FILE_IO              : origin = 0x80000000,  len = 0x07000000 /* Algo input and output, non cached region for A15 and cached region for DSP*/
        FILE_IO_SHARED_MEM_BASE   : origin = 0x87000000,  len = 0x01000000 /* Reserved for A15 state variables in fast file io, non cached for A15 and DSP*/
        EXT_MEM_HOLE              : origin = 0x88000000,  len = 0x03000000 /* Hole, non mandatory configuration as non cached for DSP*/
        EXT_MEM_FAST_FILE_IO_CODE : origin = 0x8b000000,  len = 0x05000000 /* Fast file io code of A15, non mandatory configuration as non cached for DSP*/
        EXT0                      : origin = 0x90000000,  len = 0x02000000 /* Cached external memory (DDR), use for algorithm*/

}

SECTIONS
{
    .text       >       EXT0

    .stack      >       L2_SRAM
    .args       >       EXT0

    GROUP
    {
            .neardata   /* Move .bss after .neardata and .rodata.  ELF allows */
            .rodata     /* uninitialized data to follow initialized data in a */
            .bss        /* single segment. This order facilitates a single    */
                        /* segment for the near DP sections.                  */
    }>EXT0

    .cinit      >       EXT0
    .cio        >       EXT0
    .const      >       EXT0
    .data       >       EXT0
    .switch     >       EXT0
    .sysmem     >       EXT0
    .far        >       EXT0
    .fardata    >       EXT0
    .ppinfo     >       EXT0
    .ppdata     >       EXT0, palign(32) /* Work-around kelvin bug */
    .dmem1      >       L2_SRAM
    .ddrcacheSect >     ALGO_FILE_IO
    .fileio       >     FILE_IO_SHARED_MEM_BASE
    .ddrfileio    >     ALGO_FILE_IO
}
