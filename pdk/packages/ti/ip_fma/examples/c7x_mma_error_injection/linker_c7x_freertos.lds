/*
 *  Copyright (C) 2025 Texas Instruments Incorporated
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
 */

--ram_model
-heap  0x20000
-stack 0x20000
--args 0x1000
--diag_suppress=10068 /* "no matching section" */
--cinit_compression=off
-e _c_int00_secure

#define DDR0_ALLOCATED_START  0xA0000000
#define C7X_ALLOCATED_START DDR0_ALLOCATED_START + 0x06000000

#define C7X_EXT_DATA_BASE   (C7X_ALLOCATED_START + 0x00100000)
#define C7X_MEM_TEXT_BASE   (C7X_ALLOCATED_START + 0x00200000)
#define C7X_MEM_DATA_BASE   (C7X_ALLOCATED_START + 0x00300000)
#define C7X_DDR_SPACE_BASE  (C7X_ALLOCATED_START + 0x00400000)

MEMORY
{
    DDR0_RESERVED: org = 0x80000000,          len = 0x20000000   /* 512MB Reserved for A72 OS */
    C7X_IPC_D:     org = C7X_ALLOCATED_START, len = 0x00100000   /*  1MB DDR */
    C7X_EXT_D:     org = C7X_EXT_DATA_BASE,   len = 0x00100000   /*  1MB DDR */
    C7X_TEXT:      org = C7X_MEM_TEXT_BASE,   len = 0x00100000   /*  1MB DDR */
    C7X_DATA:      org = C7X_MEM_DATA_BASE,   len = 0x00100000   /*  1MB DDR */
    C7X_DDR_SPACE: org = C7X_DDR_SPACE_BASE,  len = 0x01300000   /* 19MB DDR */
}

SECTIONS
{
    boot:
    {
      boot.*<boot.oe71>(.text)
    } load > C7X_TEXT ALIGN(0x200000)
    .vecs       >       C7X_DDR_SPACE ALIGN(0x400000)
    .secure_vecs    >   C7X_DDR_SPACE ALIGN(0x200000)
    .text:_c_int00_secure > C7X_DDR_SPACE ALIGN(0x200000)
    .text       >       C7X_DDR_SPACE ALIGN(0x200000)

    .bss        >       C7X_DDR_SPACE  /* Zero-initialized data */
    .data       >       C7X_DDR_SPACE  /* Initialized data */

    .cinit      >       C7X_DDR_SPACE  /* could be part of const */
    .init_array >       C7X_DDR_SPACE  /* C++ initializations */
    .stack      >       C7X_DDR_SPACE ALIGN(0x2000)
    .args       >       C7X_DDR_SPACE
    .cio        >       C7X_DDR_SPACE
    .const      >       C7X_DDR_SPACE
    .switch     >       C7X_DDR_SPACE /* For exception handling. */
    .sysmem     >       C7X_DDR_SPACE /* heap */
    .ecsp_ss_mem >      C7X_DDR_SPACE
    .ecsp_s_mem >       C7X_DDR_SPACE
    .ecsp_gs_mem >      C7X_DDR_SPACE
    .tcsp_mem   >       C7X_DDR_SPACE
    .estp_ss_mem >      C7X_DDR_SPACE
    .estp_s_mem  >      C7X_DDR_SPACE
    .estp_gs_mem >      C7X_DDR_SPACE
    .rdata       >      C7X_DDR_SPACE
    .boot        >      C7X_DDR_SPACE
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
}
