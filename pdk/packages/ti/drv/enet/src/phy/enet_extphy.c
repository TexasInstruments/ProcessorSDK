/*
 *  Copyright (c) Texas Instruments Incorporated 2025
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
 */

/*!
 * \file  enet_extphy.c
 *
 * \brief This file contains details for all PHY drivers.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <ti/drv/enet/include/phy/enetphy.h>
#include "enetphy_priv.h"
#include "generic_phy.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* PHY drivers */
extern EnetPhy_Drv gEnetPhyDrvGeneric;
extern EnetPhy_Drv gEnetPhyDrvDp83822;
extern EnetPhy_Drv gEnetPhyDrvDp83867;
extern EnetPhy_Drv gEnetPhyDrvVsc8514;

/*! \brief All the registered PHY specific drivers.
 *
 * When adding new PHY drivers, users must add the PHY specific
 * driver handles in this array, ensuring that the generic PHY
 * driver is the last entry since it is the default fallback 
 * driver selected in case no PHY ID match is found.
 */
EnetPhyDrv_Handle gEnetPhyDrvs[] =
{
    &gEnetPhyDrvVsc8514,   /* VSC8514 */
    &gEnetPhyDrvDp83822,   /* DP83822 */
    &gEnetPhyDrvDp83867,   /* DP83867 */
    &gEnetPhyDrvGeneric,   /* Generic PHY - must be last */
};

/*! \brief Number of PHY drivers supported.
 *
 * Stores the size of the gEnetPhyDrvs Structure. This will
 * tell us the total number of PHY drivers supported by us.
 */
const uint32_t gEnetPhyDrvsSize = ENETPHY_ARRAYSIZE(gEnetPhyDrvs);
/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/* None */
