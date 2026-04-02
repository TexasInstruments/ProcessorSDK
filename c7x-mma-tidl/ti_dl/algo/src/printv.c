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
#if defined TIDL_COVERAGE_DEAD_CODE
#include "tidl_types.h"
#include "tidl_alg_int.h"
#include "tidl_commonUtils.h"
#include "tidl_scalability_support.h"
#include "printv.h"
/*----------------------------------------------------------------------------*/
/* Func: PRINTV                                                               */
/*                                                                            */
/* Args: const char *msg                                                      */
/*            - Input message string to be prefixed.                          */
/*       long8 inVec                                                            */
/*            - Input 512 bit data vector to be displayed                     */
/*       uint32_t data_type                                                    */
/*            - Predefined macro values indicating bitdepth and sign.         */
/*       uint32_t HEX                                                         */
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
void PRINTV(const char *msg, c7x::long_vec inVec, uint32_t data_type, uint32_t mode)
{
    c7x::long_vec Vec = inVec;
#if(PRINTV_QUIET)
#else
    /*------------------------------------------------------------------------*/
    /* Begin by displaying the input message string.                          */
    /*------------------------------------------------------------------------*/
    tidl_printf(0,"%s",msg);

    /*------------------------------------------------------------------------*/
    /* Format the data in 8-bit unsigned representation before displaying.    */
    /*------------------------------------------------------------------------*/
    if(data_type == PRINTV_UNSIGNED_8BIT)
    {
        uint32_t ctr;
        for(ctr = 0; ctr < TIDL_SIMD_WIDTH; ctr++)
        {
           c7x::uchar_vec newVec = c7x::as_uchar_vec(Vec);
           Vec = __vshr_vkv(Vec, 8);

            if(mode == PRINTV_HEX_OFF)
            {
              tidl_printf(0," %4d,", newVec.s[0]);
            }
            else
            {
              tidl_printf(0," 0x%02x,", newVec.s[0]);
            }
        }
        tidl_printf(0,"\n");
    }

    /*------------------------------------------------------------------------*/
    /* Format the data in 16-bit unsigned representation before displaying.   */
    /*------------------------------------------------------------------------*/
    if(data_type == PRINTV_UNSIGNED_16BIT)
    {
        uint32_t ctr;
        for(ctr = 0; ctr < (TIDL_SIMD_WIDTH/2); ctr++)
        {
            c7x::ushort_vec newVec = c7x::as_ushort_vec(Vec);
            Vec = __vshr_vkv(Vec, 16);
            if(mode == PRINTV_HEX_OFF)
            {
              tidl_printf(0," %4d,", newVec.s[0]);
            }
            else
            {
              tidl_printf(0," 0x%04x,", newVec.s[0]);
            }
        }
        tidl_printf(0,"\n");
    }

    /*------------------------------------------------------------------------*/
    /* Format the data in 32-bit unsigned representation before displaying.   */
    /*------------------------------------------------------------------------*/
    if(data_type == PRINTV_UNSIGNED_32BIT)
    {
        uint32_t ctr;
        for(ctr = 0; ctr < (TIDL_SIMD_WIDTH/4); ctr++)
        {
            c7x::uint_vec newVec = c7x::as_uint_vec(Vec);
            Vec = __vshr_vkv(Vec, 16);
            Vec = __vshr_vkv(Vec, 16);
            if(mode == PRINTV_HEX_OFF)
            {
              tidl_printf(0," %8d,", newVec.s[0]);
            }
            else
            {
              tidl_printf(0," 0x%08x,", newVec.s[0]);
            }

        }
        tidl_printf(0,"\n");
    }

    /*------------------------------------------------------------------------*/
    /* Format the data in 64-bit unsigned representation before displaying.   */
    /*------------------------------------------------------------------------*/
    if(data_type == PRINTV_UNSIGNED_64BIT)
    {
        uint32_t ctr;
        for(ctr = 0; ctr < (TIDL_SIMD_WIDTH/8); ctr++)
        {
            c7x::ulong_vec newVec = c7x::as_ulong_vec(Vec);
            Vec = __vshr_vkv(Vec, 16);
            Vec = __vshr_vkv(Vec, 16);
            Vec = __vshr_vkv(Vec, 16);
            Vec = __vshr_vkv(Vec, 16);
            if(mode == PRINTV_HEX_OFF)
            {
              tidl_printf(0," %16lld,", newVec.s[0]);
            }
            else
            {
              tidl_printf(0," 0x%016llx,", newVec.s[0]);
            }

        }
        tidl_printf(0,"\n");
    }

    /*------------------------------------------------------------------------*/
    /* Format the data in 8-bit signed representation before displaying.      */
    /*------------------------------------------------------------------------*/
    if(data_type == PRINTV_SIGNED_8BIT)
    {
        uint32_t ctr;
        for(ctr = 0; ctr < TIDL_SIMD_WIDTH; ctr++)
        {
            c7x::char_vec newVec = c7x::as_char_vec(Vec);
            Vec = __vshr_vkv(Vec, 8);
            if(mode == PRINTV_HEX_OFF)
            {
              tidl_printf(0," %4d,", newVec.s[0]);
            }
            else
            {
              tidl_printf(0," 0x%02x,", (newVec.s[0] & 0xFF));
            }
        }
        tidl_printf(0,"\n");
    }

    /*------------------------------------------------------------------------*/
    /* Format the data in 16-bit signed representation before displaying.     */
    /*------------------------------------------------------------------------*/
    if(data_type == PRINTV_SIGNED_16BIT)
    {
        uint32_t ctr;
        for(ctr = 0; ctr < (TIDL_SIMD_WIDTH/2); ctr++)
        {
            c7x::short_vec newVec = c7x::as_short_vec(Vec);
            Vec = __vshr_vkv(Vec, 16);

            if(mode == PRINTV_HEX_OFF)
            {
              tidl_printf(0," %4d,", newVec.s[0]);
            }
            else
            {
              tidl_printf(0," 0x%04x,", (newVec.s[0] & 0xFFFF));
            }

        }
        tidl_printf(0,"\n");
    }

    /*------------------------------------------------------------------------*/
    /* Format the data in 32-bit signed representation before displaying.     */
    /*------------------------------------------------------------------------*/
    if(data_type == PRINTV_SIGNED_32BIT)
    {
        uint32_t ctr;
        for(ctr = 0; ctr < (TIDL_SIMD_WIDTH/4); ctr++)
        {
            c7x::int_vec newVec = c7x::as_int_vec(Vec);
            Vec = __vshr_vkv(Vec, 16);
            Vec = __vshr_vkv(Vec, 16);
            if(mode == PRINTV_HEX_OFF)
            {
              tidl_printf(0," %8d,", newVec.s[0]);
            }
            else
            {
              tidl_printf(0," 0x%08x,", newVec.s[0]);
            }
        }
        tidl_printf(0,"\n");
    }

    /*------------------------------------------------------------------------*/
    /* Format the data in 64-bit signed representation before displaying.     */
    /*------------------------------------------------------------------------*/
    if(data_type == PRINTV_SIGNED_64BIT)
    {
        uint32_t ctr;
        for(ctr = 0; ctr < (TIDL_SIMD_WIDTH/8); ctr++)
        {
            c7x::long_vec newVec = c7x::as_long_vec(Vec);
            Vec = __vshr_vkv(Vec, 16);
            Vec = __vshr_vkv(Vec, 16);
            Vec = __vshr_vkv(Vec, 16);
            Vec = __vshr_vkv(Vec, 16);
            if(mode == PRINTV_HEX_OFF)
            {
              tidl_printf(0," %16lld,", newVec.s[0]);
            }
            else
            {
              tidl_printf(0," 0x%016llx,", newVec.s[0]);
            }

        }
        tidl_printf(0,"\n");
    }

    /*------------------------------------------------------------------------*/
    /* Format the data in SP floating point esentation before displaying.     */
    /*------------------------------------------------------------------------*/
    if(data_type == PRINTV_FLOAT)
    {
        uint32_t ctr;
        for(ctr = 0; ctr < (TIDL_SIMD_WIDTH/4); ctr++)
        {
            c7x::float_vec newVec = c7x::as_float_vec(Vec);
            float32_tidl value    = newVec.s[0];
            Vec = __vshr_vkv(Vec, 16);
            Vec = __vshr_vkv(Vec, 16);
            tidl_printf(0," %8.2F,", value);
        }
        tidl_printf(0,"\n");
    }

#endif //PRINTV_QUIET
  return;
}
#endif
