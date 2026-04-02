#ifndef _SAFW_V1_H
#define _SAFW_V1_H
/******************************************************************************
 * FILE PURPOSE: SASS Firmware Image Definitions for the SA LLD 
 ******************************************************************************
 * FILE NAME:   safw.h
 *
 * DESCRIPTION: Define firmware image related constants and data structures of 
 *  the second generation SASS.  
 *  The SASS contains three PDSPs wihich perform the protocol header processing. 
 *  There are three PDSP images provided by the module under the ti/drv/sa/fw/v1 directory:
 *  -Packet Header Processing 1 image: saphp1_bin.c for PDSP0
 *  -Packet Header Processing 2 image: saphp2_bin.c for PDSP1
 *  -Packet Header Processing 3 image: saphp3_bin.c for PDSP2
 *
 * REVISION HISTORY:
 *
 *  TEXAS INSTRUMENTS TEXT FILE LICENSE
 * 
 *   Copyright (c) 2016 Texas Instruments Incorporated
 * 
 *  All rights reserved not granted herein.
 *  
 *  Limited License.  
 * 
 *  Texas Instruments Incorporated grants a world-wide, royalty-free, non-exclusive 
 *  license under copyrights and patents it now or hereafter owns or controls to 
 *  make, have made, use, import, offer to sell and sell ("Utilize") this software 
 *  subject to the terms herein.  With respect to the foregoing patent license, 
 *  such license is granted  solely to the extent that any such patent is necessary 
 *  to Utilize the software alone.  The patent license shall not apply to any 
 *  combinations which include this software, other than combinations with devices 
 *  manufactured by or for TI (“TI Devices”).  No hardware patent is licensed hereunder.
 * 
 *  Redistributions must preserve existing copyright notices and reproduce this license 
 *  (including the above copyright notice and the disclaimer and (if applicable) source 
 *  code license limitations below) in the documentation and/or other materials provided 
 *  with the distribution.
 *  
 *  Redistribution and use in binary form, without modification, are permitted provided 
 *  that the following conditions are met:
 * 	No reverse engineering, decompilation, or disassembly of this software is 
 *   permitted with respect to any software provided in binary form.
 * 	Any redistribution and use are licensed by TI for use only with TI Devices.
 * 	Nothing shall obligate TI to provide you with source code for the software 
 *   licensed and provided to you in object code.
 *  
 *  If software source code is provided to you, modification and redistribution of the 
 *  source code are permitted provided that the following conditions are met:
 * 	Any redistribution and use of the source code, including any resulting derivative 
 *   works, are licensed by TI for use only with TI Devices.
 * 	Any redistribution and use of any object code compiled from the source code
 *   and any resulting derivative works, are licensed by TI for use only with TI Devices.
 * 
 *  Neither the name of Texas Instruments Incorporated nor the names of its suppliers 
 *  may be used to endorse or promote products derived from this software without 
 *  specific prior written permission.
 * 
 *  DISCLAIMER.
 * 
 *  THIS SOFTWARE IS PROVIDED BY TI AND TI’S LICENSORS "AS IS" AND ANY EXPRESS OR IMPLIED 
 *  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY 
 *  AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL TI AND TI’S 
 *  LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
 *  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
 *  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * 
 */
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* Packet Header Processing 1 image */
extern const uint32_t Sa2_php1[]; 
extern const int Sa2_php1Size; 
/* Packet Header Processing 2 image */
extern const uint32_t Sa2_php2[]; 
extern const int Sa2_php2Size;
/* Packet Header Processing 3 image */
extern const uint32_t Sa2_php3[]; 
extern const int Sa2_php3Size; 

#ifdef __cplusplus
}
#endif
  
#endif  /* _SAFW_H */
