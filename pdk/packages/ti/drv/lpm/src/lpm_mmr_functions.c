/*
*
* Copyright (c) 2021 Texas Instruments Incorporated
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

/*!
 *  \file
 *  \brief MMR related functions
 */

#include <ti/drv/lpm/include/lpm_mmr_functions.h>
#include <ti/drv/lpm/include/io_retention/dev_info.h>

#include <stdint.h>


#define MMR_PLL0_KICK0_UNLOCK_VAL                (0x68EF3490)
#define MMR_PLL0_KICK1_UNLOCK_VAL                (0xD172BC5A)
#define MMR_LOCK0_KICK0_UNLOCK_VAL                (0x68EF3490)
#define MMR_LOCK0_KICK1_UNLOCK_VAL                (0xD172BC5A)
#define MMR_LOCK0_KICK0                           (0x01008)
#define SOC_LOCK_MMR_UNLOCK     (0u)

#define SOC_NUM_SPINLOCKS       ( 256u )

 int32_t SOC_lock( uint32_t lockNum )
 {
     volatile uint32_t *spinLockReg;
 
     if ( lockNum >= SOC_NUM_SPINLOCKS )
         return -1;
 
     spinLockReg = (volatile uint32_t *)(base2core(CSL_NAVSS0_SPINLOCK_BASE + 0x800));
 
     while ( spinLockReg[ lockNum ] != 0x0 );
 
     return 0;
 }


int32_t SOC_unlock( uint32_t lockNum )
{
    volatile uint32_t *spinLockReg;

    if ( lockNum >= SOC_NUM_SPINLOCKS )
        return -1;

    spinLockReg = (volatile uint32_t *)(base2core(CSL_NAVSS0_SPINLOCK_BASE + 0x800));
    spinLockReg[ lockNum ] = 0x0;

    return 0;
}

bool Lpm_mmr_isLocked(
    uintptr_t base,
    uint32_t  partition
    )
{
    volatile uint32_t * lock = mkptr(base, partition * 0x4000 + MMR_LOCK0_KICK0);

    return (*lock & 0x1u) ? false : true;
}

/*!
 *  \brief Function to unlock a partition within an MMR block.
 *
 *  \b Example
 *  \code
 *  void test_main (void)
 *  {
 *    uint32_t partition = 7;
 *    Lpm_mmr_unlock(CSL_WKUP_CTRL_MMR0_CFG0_BASE, partition);
 *  }
 *  \endcode
 */
void Lpm_mmr_unlock (
    uintptr_t base,     /*!< physical base address of the mmr to unlock */
    uint32_t partition  /*!< which partition to unlock */
    )
{
#ifndef TBARM9CORE
  /* translate the base address */
  volatile uint32_t * lock = mkptr(base, partition * 0x4000 + MMR_LOCK0_KICK0);

  if (!Lpm_mmr_isLocked( base, partition ) )
  {
    /* return if already unlocked */
    return;
  }
  else
  {
    /* unlock the partition */
    SOC_lock( SOC_LOCK_MMR_UNLOCK );
    lock[0] = MMR_LOCK0_KICK0_UNLOCK_VAL;
    lock[1] = MMR_LOCK0_KICK1_UNLOCK_VAL;
    SOC_unlock( SOC_LOCK_MMR_UNLOCK );
  }
#endif
}

/*!
 *  \brief Function to lock a partition within an MMR block.
 *
 *  \b Example
 *  \code
 *  void test_main (void)
 *  {
 *    uint32_t partition = 5;
 *    Lpm_mmr_lock(CSL_MCU_CTRL_MMR0_CFG0_BASE, partition);
 *  }
 *  \endcode
 */
void Lpm_mmr_lock (
    uintptr_t base,     /*!< physical base address of the mmr to target */
    uint32_t partition  /*!< which partition to lock */
    )
{
  volatile uint32_t * lock = mkptr(base, (partition * 0x4000 + MMR_LOCK0_KICK0));
  *lock = 0;
}

/* Below is the code for MMR modules */
void Lpm_unlock(unsigned int unlock_num, unsigned int base ) {
  unsigned int const_lock_start_addr, partitionOffset, kick1_val, kick0_val;
  if(base == CSL_WKUP_CTRL_MMR0_CFG0_BASE || base == CSL_MCU_CTRL_MMR0_CFG0_BASE || base == MAIN_CTRL_BASE)
  {
    const_lock_start_addr = 0x01008;
    kick0_val = MMR_LOCK0_KICK0_UNLOCK_VAL;
    kick1_val = MMR_LOCK0_KICK1_UNLOCK_VAL; 
    partitionOffset = 0x04000; 
  }
  else
  {
    const_lock_start_addr = 0x00010;
    kick0_val = MMR_PLL0_KICK0_UNLOCK_VAL;
    kick1_val = MMR_PLL0_KICK1_UNLOCK_VAL; 	
    partitionOffset = 0x01000;
  }

  *mkptr(base, (const_lock_start_addr+partitionOffset*unlock_num)) = kick0_val;
  *mkptr(base, (const_lock_start_addr+partitionOffset*unlock_num+0x00004)) = kick1_val;
}
