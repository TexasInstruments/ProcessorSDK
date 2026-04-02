/*
 *
 * Copyright (c) 2018 Texas Instruments Incorporated
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

/*
 * Note: Marking a region as shareable will cause the region to behave as outer shareable with write through
 *       no write-allocate caching policy irrespective of the actual cache policy set. Therefore, only select
 *       regions that are actually shared outside the R5 CPUSS must be marked as shared.
 */

var MPU = xdc.useModule('ti.sysbios.family.arm.MPU');
MPU.enableMPU = true;
MPU.enableBackgroundRegion = true;

var attrs = new MPU.RegionAttrs();
MPU.initRegionAttrsMeta(attrs);

var index = 0;

/* make all 4G as strongly ordered, non-cacheable */
attrs.enable = true;
attrs.bufferable = false;
attrs.cacheable = false;
attrs.shareable = true;
attrs.noExecute = true;
attrs.accPerm = 1;          /* RW at PL1 */
attrs.tex = 0;
attrs.subregionDisableMask = 0;
MPU.setRegionMeta(index++, 0x00000000, MPU.RegionSize_4G, attrs);

/* make ATCM as cacheable */
attrs.enable = true;
attrs.bufferable = true;
attrs.cacheable = true;
attrs.shareable = false;
attrs.noExecute = false;
attrs.accPerm = 1;          /* RW at PL1 */
attrs.tex = 1;
attrs.subregionDisableMask = 0;
MPU.setRegionMeta(index++, 0x00000000, MPU.RegionSize_32K, attrs);

/* make ATCM as cacheable */
attrs.enable = true;
attrs.bufferable = true;
attrs.cacheable = true;
attrs.shareable = false;
attrs.noExecute = false;
attrs.accPerm = 1;          /* RW at PL1 */
attrs.tex = 1;
attrs.subregionDisableMask = 0;
MPU.setRegionMeta(index++, 0x41000000, MPU.RegionSize_32K, attrs);

/* make BTCM as cacheable */
attrs.enable = true;
attrs.bufferable = true;
attrs.cacheable = true;
attrs.shareable = false;
attrs.noExecute = false;
attrs.accPerm = 1;          /* RW at PL1 */
attrs.tex = 1;
attrs.subregionDisableMask = 0x0;
MPU.setRegionMeta(index++, 0x41010000, MPU.RegionSize_32K, attrs);

/* MCU OCSRAM as cacheable */
attrs.enable = true;
attrs.bufferable = true;
attrs.cacheable = true;
attrs.shareable = false;
attrs.noExecute = false;
attrs.accPerm = 1;          /* RW at PL1 */
attrs.tex = 1;
attrs.subregionDisableMask = 0;
MPU.setRegionMeta(index++, 0x41C00000, MPU.RegionSize_1M, attrs);

/* make all MSMC as cacheable */
attrs.enable = true;
attrs.bufferable = true;
attrs.cacheable = true;
attrs.shareable = false;
attrs.noExecute = false;
attrs.accPerm = 1;          /* RW at PL1 */
attrs.tex = 1;
attrs.subregionDisableMask = 0;
MPU.setRegionMeta(index++, 0x70000000, MPU.RegionSize_8M, attrs);

/* make all 2G DDR as cacheable */
attrs.enable = true;
attrs.bufferable = true;
attrs.cacheable = true;
attrs.shareable = false;
attrs.noExecute = false;
attrs.accPerm = 1;          /* RW at PL1 */
attrs.tex = 1;
attrs.subregionDisableMask = 0;
MPU.setRegionMeta(index++, 0x80000000, MPU.RegionSize_2G, attrs);

/* Note: the next 4 MPU regions start address (second argument of MPU.setRegionMeta)
   must cover the address range of APP_LOG_MEM, TIOVX_OBJ_DESC_MEM, IPC_VRING_MEM,
   TIOVX_LOG_RT_MEM_ADDR in system_memory_map.html and MUST be 16M aligned
 */
var non_cache_base_addr = 0xB0000000;
var MB = 0x100000;

attrs.enable = true;
attrs.bufferable = false;
attrs.cacheable = false;
attrs.shareable = true;
attrs.noExecute = true;
attrs.accPerm = 1;          /* RW at PL1 */
attrs.tex = 0;
attrs.subregionDisableMask = 0;
MPU.setRegionMeta(index++, non_cache_base_addr, MPU.RegionSize_128M, attrs);

/* make DDR_MCU1_0_IPC_ADDR as non-cache */
/* Note: the next MPU regions start address (second argument of MPU.setRegionMeta)
   must cover the address range of DDR_MCU1_0_IPC_ADDR
   in system_memory_map.html and MUST be 1M aligned
 */
attrs.enable = true;
attrs.bufferable = false;
attrs.cacheable = false;
attrs.shareable = true;
attrs.noExecute = false;
attrs.accPerm = 1;          /* RW at PL1 */
attrs.tex = 0;
attrs.subregionDisableMask = 0;
MPU.setRegionMeta(index++, 0xA0000000, MPU.RegionSize_1M, attrs);

eeprom_shadow_base_addr = 0xFE000000;
/* make EEPROM_SHADOW, as non-cache */
/* Note: MUST be 8MB aligned
 */
attrs.enable = true;
attrs.bufferable = false;
attrs.cacheable = false;
attrs.shareable = true;
attrs.noExecute = true;
attrs.accPerm = 1;          /* RW at PL1 */
attrs.tex = 0;
attrs.subregionDisableMask = 0;
MPU.setRegionMeta(index++, eeprom_shadow_base_addr, MPU.RegionSize_16M, attrs);

xdc.print("# MPU setup for " + index + " entries !!!");
