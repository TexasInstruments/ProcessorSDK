/**
 * \copyright  Copyright (C) 2018 Texas Instruments Incorporated -
 *             http://www.ti.com/
 */

/**
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

/* 
 * This file contains mpu settings
 */
#include <stdio.h>
#include <xdc/std.h>
#include <xdc/runtime/System.h>

#if (defined (SOC_AM65XX) && defined (BUILD_MPU))

#include <ti/sysbios/family/arm/v8a/Mmu.h>
#include <ti/csl/soc.h>

volatile int emuwait_mmu=1;
void InitMmu()
{
    Bool ret= FALSE;
    Mmu_MapAttrs attrs;

    Mmu_initMapAttrs(&attrs);

   /* TODO: Check if the return value is non zero, then raise error and exit TODO!!!!!*/
    attrs.attrIndx = Mmu_AttrIndx_MAIR0;
    ret=Mmu_map(0x01800000, 0x01800000, 0x00100000, &attrs); /* gicv3       */

    if(ret== FALSE)
    {
        goto mmu_exit;
    }

    ret=Mmu_map(0x02400000, 0x02400000, 0x000c0000, &attrs); /* dmtimer     */

    if(ret==FALSE)
    {
        goto mmu_exit;
    }


    ret=Mmu_map(0x02800000, 0x02800000, 0x00001000, &attrs); /* uart        */

    if(ret== FALSE)
    {
        goto mmu_exit;
    }

    ret=Mmu_map(0x2A430000, 0x2A430000, 0x00001000, &attrs); /* ctrcontrol0 */

     if(ret== FALSE)
     {
         goto mmu_exit;
     }


    ret=Mmu_map(0x02B00000, 0x02B00000, 0x00030000, &attrs); /* uart        */

    if(ret==FALSE)
    {
        goto mmu_exit;
    }


    ret=Mmu_map(0x030800000, 0x030800000, 0xC000000, &attrs); /* navss        */

    if(ret==FALSE)
    {
        goto mmu_exit;
    }

    ret=Mmu_map(0x4E00000, 0x4E00000, 0x030000, &attrs); /* sa2ul        */

    if(ret==FALSE)
    {
        goto mmu_exit;
    }


    ret=Mmu_map(0xa80000, 0xa80000, 0x0C0000, &attrs); /* GTC counters   */

    if(ret==FALSE)
    {
        goto mmu_exit;
    }

    attrs.attrIndx = Mmu_AttrIndx_MAIR7;

    ret=Mmu_map(0x70000000, 0x70000000, 0x00200000, &attrs); /* msmc        */
    if(ret==FALSE)
    {
        goto mmu_exit;
    }

mmu_exit:
    if(ret==FALSE) {
        System_printf("Mmu_map returned error %d",ret);
                       while(emuwait_mmu);
      }
    return;
}


#endif

/* end of file */


