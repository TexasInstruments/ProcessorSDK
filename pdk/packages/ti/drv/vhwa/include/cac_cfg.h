/* =============================================================================
 *   Copyright (c) Texas Instruments Incorporated 2021
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
/**
 *  \ingroup  DRV_VISS_MODULE
 *  \defgroup DRV_CAC_MODULE_CFG CAC Configuration
 *            This is VISS CAC Configuration file
 *
 *  @{
 */
/**
 *  \file   cac_cfg.h
 *
 *  \brief  Inteface file for Flex Color processing module
 */

#ifndef CAC_CFG_H_
#define CAC_CFG_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <vhwa_vpac_abstraction_layer.h>

#include <include/vhwa_common.h>


#ifdef __cplusplus
extern "C" {
#endif



/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
/**
 *  \anchor Cac_Ioctl
 *  \name   CAC IOCTL macros
 *  \brief  Control Commands for CAC module in VISS
 *
 *  @{
 */
/**
 * \brief Used for setting individual CAC sub-module's configuration.
 *        Single ioctl for configuring all sub-module's configuration,
 *        by selecting module id and setting appropriate pointer in
 *        #Cac_Config.
 *
 * \return  FVID2_SOK on success, else error code.
 */
#define IOCTL_CAC_SET_CONFIG                (VHWA_IOCTL_CAC_IOCTL_BASE + 1U)
/** @} */

#define CAC_LUT_SIZE                        (8192U)

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */



/**
 *  struct Cac_GridSize
 *  \brief Horizontal And Vertical Grid Size
 *
 */
typedef struct
{
    uint32_t                    hCnt;
    /**< Number of horizontal blocks
      *  (HCNT+1)* BLOCKSZ.SIZE >= frame_width */
    uint32_t                    vCnt;
    /**< Number of vertical blocks
      *  (VCNT+1)* BLOCKSZ.SIZE >= frame_height */
} Cac_GridSize;

/**
 *  struct Cac_Config
 *  \brief Configuration Parameters for CAC
 *
 */
typedef struct
{
    uint32_t        colorEnable;
    /**< CAC color processing in 2x2 pixel grid only one pixel in row should be
         enabled valid valies are 0x6, 0x9 */
    uint32_t        blkSize;
    /**< CAC color processing Block Size, Value should be multiple of 4
      *  with minimum value of 8 */

    Cac_GridSize    blkGridSize;
    /**< Structure with Horizontal And Vertical Grid Size */

    uint32_t        *displacementLut;
    /**< Displacement Lut Addr Should Not be Null */
} Cac_Config;

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif  /* #ifndef CAC_CFG_H_ */
 /** @} */
