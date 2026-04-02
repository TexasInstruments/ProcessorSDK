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

/**
  ******************************************************************************
  *  @file     TIDL_bufParams.h
  *
  *  @brief    File to hold buffer parameter related info for TIDL
  *
  *  @version  0.1 - Jan 2018 : Initial Version - wrapper on VXLIB [PKS]
  *
 *******************************************************************************
*/

#ifndef TIDL_BUFPARAMS_H_
#define TIDL_BUFPARAMS_H_

#ifndef __OPENCL_VERSION__
#include <stdint.h>
#endif

#include "tidl_dataflow.h"
#include  "VXLIB_bufParams.h"

/* ----------------------------------------------------------------
 *  MISRAC Rule 8.5 Deviation: This header file contains definitions
 *  for the following inline funtions.  These functions are shared
 *  among more than one TIDL kernel, so it is better to have them
 *  in a common header file than to replicate the code.
 * ---------------------------------------------------------------- */
static inline uint32_t TIDL_sizeof(uint32_t usrtype);

/* @} */

/*! \brief The enumeration of multidimensional buffer types. */
typedef enum _TIDL_dimension_e {
    TIDL_2D= VXLIB_2D,
    TIDL_3D= VXLIB_3D,
    TIDL_4D= VXLIB_4D,
    TIDL_5D= VXLIB_5D,
    TIDL_6D= VXLIB_6D
} TIDL_dimension_e;

/*! \brief A structure for a 1 dimensional buffer descriptor. */
typedef VXLIB_bufParams1D_t  TIDL_bufParams1D_t ;

/*! \brief A structure for a 2 dimensional buffer descriptor. */
typedef VXLIB_bufParams2D_t TIDL_bufParams2D_t;



/*! \brief A structure for a N dimensional buffer descriptor. */
typedef VXLIB_bufParamsND_t TIDL_bufParamsND_t ;

/*! \brief Inline function returns number of bytes per element given a type of \ref TIDL_data_type_e. */
static inline uint32_t TIDL_sizeof(uint32_t usrtype)
{
  return VXLIB_sizeof(usrtype);
}

#endif /* TIDL_BUFPARAMS_H_ */
