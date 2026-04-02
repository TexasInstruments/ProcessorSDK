/*
*
* Copyright (c) {2015 - 2020} Texas Instruments Incorporated
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

#ifndef PRINTV_H_
#define PRINTV_H_

#if HOST_EMULATION
#include "c7x.h"
#else
#include "c7x.h"
#endif

#include <stdint.h>
#include <stdio.h>
#include "tidl_scalability_support.h"
/*----------------------------------------------------------------------------*/
/* Console print options.                                                     */
/* Use the below macros as needed to indicate the bit depth and sign of the   */
/* elements constituting a vector.                                            */
/*----------------------------------------------------------------------------*/
#define PRINTV_UNSIGNED_8BIT  (0U)
#define PRINTV_UNSIGNED_16BIT (1U)
#define PRINTV_UNSIGNED_32BIT (2U)
#define PRINTV_UNSIGNED_64BIT (3U)
#define PRINTV_SIGNED_8BIT    (4U)
#define PRINTV_SIGNED_16BIT   (5U)
#define PRINTV_SIGNED_32BIT   (6U)
#define PRINTV_SIGNED_64BIT   (7U)
#define PRINTV_FLOAT          (8U)

/*----------------------------------------------------------------------------*/
/* Flags to indicate if the output must be printed in hex or decimal.         */
/*----------------------------------------------------------------------------*/
#define PRINTV_HEX_ON  (1U)
#define PRINTV_HEX_OFF (0U)

/*----------------------------------------------------------------------------*/
/* Flag to indicate quite mode. No prints!                                    */
/*----------------------------------------------------------------------------*/
#define PRINTV_QUIET   (0U)

/*----------------------------------------------------------------------------*/
/* Func: PRINTV                                                               */
/*                                                                            */
/* Args: const char *msg                                                      */
/*            - Input message string to be prefixed.                          */
/*       long8 Vec                                                            */
/*            - Input 512 bit data vector to be displayed                     */
/*       U32 data_type                                                         */
/*            - Predefined macro values indicating bitdepth and sign.         */
/*       U32 HEX                                                              */
/*            - Predefined macro value to indicate hex or decimal display.    */
/* Return: None                                                               */
/*                                                                            */
/* Desc: This function is used to display the contents of a vector            */
/*       horizontally with spacing as indicated by data_type and HEX flags.    */
/*       Ex. Using PRINTV_USIGN_16BIT with PRINTV_HEX_ON will print as,       */
/*                                                                            */
/*       0x0000 0x0001 0x0002 0x0003 0x0004 0x0005 0x0006 0x0007 0x0008 ....  */
/*                                                                            */
/*----------------------------------------------------------------------------*/
void PRINTV(const char *msg, c7x::long_vec inVec, uint32_t data_type, uint32_t mode);

#endif /* PRINTV_H_ */

/* ======================================================================== */
/*  End of file:  printv.h                                                  */
/* ======================================================================== */

