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

#ifndef DMSC_CM
#define DMSC_CM

#define DMSC_CM_CFG_OFFSET                      DMSC_PWR_CTRL_OFFSET
#define DMSC_CM_PID                             0x000
#define DMSC_CM_REV                             0x008
#define DMSC_CM_LOCK0_KICK0                     0x020
#define DMSC_CM_LOCK0_KICK1                     0x024
#define DMSC_CM_FW_CTRL_OUT0                    0x040
#define DMSC_CM_FW_CTRL_OUT0_SET                0x44
#define DMSC_CM_FW_CTRL_OUT0_CLR                0x048
#define DMSC_CM_FW_STS_IN0                      0x04C
#define DMSC_CM_FW_CTRL_OUT1                    0x050
#define DMSC_CM_FW_CTRL_OUT1_SET                0x054
#define DMSC_CM_FW_CTRL_OUT1_CLR                0x058
#define DMSC_CM_FW_STS_IN1                      0x05C
#define DMSC_CM_PMCTRL_SYS                      0x080
#define DMSC_CM_PMCTRL_IO_0                     0x084
#define DMSC_CM_PMCTRL_IO_1                     0x088
#define DMSC_CM_PMCTRL_DDR                      0x08C
#define DMSC_CM_PMCTRL_MOSC                     0x090
#define DMSC_CM_PMCTRL_DMSC                     0x094
#define DMSC_CM_PM_MISC_STATUS                  0x098
#define DMSC_CM_CPU_STCALIB                     0x140
#define DMSC_CM_WKUP0_EN                        0x180
#define DMSC_CM_WKUP1_EN                        0x184
#define DMSC_CM_WKUP2_EN                        0x188
#define DMSC_CM_PM_DMTIMER_CTRL                 0x200
#define DMSC_CM_PM_PERMISSION                   0x300

#define DMSC_SEC_CM_CFG_OFFSET                  DMSC_SEC_CTRL_OFFSET
#define DMSC_SEC_CM_PID                         0x000
#define DMSC_SEC_CM_LOCK0_KICK0                 0x020
#define DMSC_SEC_CM_LOCK0_KICK1                 0x024
#define DMSC_SEC_CM_ROM_START_ADDR              0x100
#define DMSC_SEC_CM_ROM_END_ADDR                0x104
#define DMSC_SEC_CM_ACTIVE                      0x200
#define DMSC_SEC_CM_SOC_WARM_RESET              0x210
#define DMSC_SEC_CM_SOC_COLD_RESET              0x220
#define DMSC_SEC_CM_DMTIMER_CTRL                0x300
#define DMSC_SEC_CM_EMU_WAIT_IN_RESET_REQ       0x400
#define DMSC_SEC_CM_DMSC_JUMP_MAGIC_NUMBER      0x420
#define DMSC_SEC_CM_DMSC_JUMP_ADDRESS           0x430
#define DMSC_SEC_CM_FIREWALL_EXP_LOWER_REG      0x500
#define DMSC_SEC_CM_FIREWALL_EXP_UPPER_REG      0x504
#define DMSC_SEC_CM_CTRL_OUT0                   0x600
#define DMSC_SEC_CM_CTRL_OUT0_SET               0x604
#define DMSC_SEC_CM_CTRL_OUT0_CLR               0x608
#define DMSC_SEC_CM_STS_IN1                     0x60C
#define DMSC_SEC_CM_DMTIMER_INTR_EXT_EN         0x700
#define DMSC_SEC_CM_WWRTI_INTR_EXT_EN           0x704
#define DMSC_SEC_CM_DEBUG_AUTH_INTR_EXT_EN      0x708
#define DMSC_SEC_CM_ECC_AGGR_INTR_EXT_EN        0x70C
#define DMSC_SEC_CM_FIREWALL_EXP_INTR_EXT_EN    0x71C
#define DMSC_SEC_CM_AES_INTR_EVT_EXT_EN         0x720
#define DMSC_SEC_CM_RAT_INTR_EXT_EN             0x724
#define DMSC_SEC_CM_IA_INTR_EXT_EN              0x728
#define DMSC_SEC_CM_SPARE_REG0                  0x900
#define DMSC_SEC_CM_SPARE_REG1                  0x904
#define DMSC_SEC_CM_SPARE_REG2                  0x908
#define DMSC_SEC_CM_SPARE_REG3                  0x90C

#endif
