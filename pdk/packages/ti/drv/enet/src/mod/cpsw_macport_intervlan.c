/*
 *  Copyright (c) Texas Instruments Incorporated 2020
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
 * \file  cpsw_macport_intervlan.c
 *
 * \brief This file contains the implementation of the InterVLAN feature of
 *        the CPSW MAC port module.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* EnetTrace id for this module, must be unique within Enet LLD */
#define ENETTRACE_MOD_ID 0x207

#include <stdint.h>
#include <stdarg.h>
#include <ti/csl/csl_cpswitch.h>
#include <ti/drv/enet/enet_cfg.h>
#include <ti/drv/enet/include/core/enet_utils.h>
#include <ti/drv/enet/include/core/enet_soc.h>
#include <ti/drv/enet/include/mod/cpsw_macport.h>
#include <ti/drv/enet/priv/core/enet_trace_priv.h>
#include <ti/drv/enet/priv/mod/cpsw_macport_priv.h>

#if ENET_CFG_IS_ON(CPSW_MACPORT_INTERVLAN)

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

static bool CpswMacPort_isRouteIdFree(CSL_Xge_cpswRegs *regs,
                                      uint32_t portNum,
                                      CpswMacPort_InterVlanRouteId routeId);

static bool CpswMacPort_cmpRoutes(const CpswMacPort_InterVlanRoutingCfg *cfg1,
                                  const CpswMacPort_InterVlanRoutingCfg *cfg2);

static void CpswMacPort_clearRouteId(CSL_Xge_cpswRegs *regs,
                                     uint32_t portNum,
                                     CpswMacPort_InterVlanRouteId routeId);

static void CpswMacPort_clearAllRoutes(CSL_Xge_cpswRegs *regs,
                                       Enet_MacPort macPort);

static int32_t CpswMacPort_getCfg(CSL_Xge_cpswRegs *regs,
                                  Enet_MacPort macPort,
                                  CpswMacPort_InterVlanRouteId routeId,
                                  CpswMacPort_InterVlanRoutingCfg *interVlanCfg);

static int32_t CpswMacPort_setCfg(CSL_Xge_cpswRegs *regs,
                                  Enet_MacPort macPort,
                                  CpswMacPort_InterVlanRouteId routeId,
                                  const CpswMacPort_InterVlanRoutingCfg *interVlanCfg);

static int32_t CpswMacPort_findRouteId(CSL_Xge_cpswRegs *regs,
                                       Enet_MacPort macPort,
                                       const CpswMacPort_InterVlanRoutingCfg *routeMatchCfg,
                                       CpswMacPort_InterVlanRouteId *routeId);

static int32_t CpswMacPort_delRoute(CSL_Xge_cpswRegs *regs,
                                    Enet_MacPort macPort,
                                    const CpswMacPort_InterVlanRoutingCfg *interVlanCfg,
                                    CpswMacPort_InterVlanRouteId *routeId);

static int32_t CpswMacPort_setRoute(CSL_Xge_cpswRegs *regs,
                                    Enet_MacPort macPort,
                                    const CpswMacPort_InterVlanRoutingCfg *interVlanCfg,
                                    CpswMacPort_InterVlanRouteId *routeId);

static int32_t CpswMacPort_getFreeRoutes(CSL_Xge_cpswRegs *regs,
                                         Enet_MacPort macPort,
                                         CpswMacPort_InterVlanFreeRouteInfo *freeRoutes);

static int32_t CpswMacPort_checkRouteIsFree(CSL_Xge_cpswRegs *regs,
                                            Enet_MacPort macPort,
                                            CpswMacPort_InterVlanRouteId routeId,
                                            bool *isFree);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void CpswMacPort_openInterVlan(EnetMod_Handle hMod)
{
    CpswMacPort_Handle hPort = (CpswMacPort_Handle)hMod;
    CSL_Xge_cpswRegs *regs = (CSL_Xge_cpswRegs *)hMod->virtAddr;
    Enet_MacPort macPort = hPort->macPort;

    CpswMacPort_clearAllRoutes(regs, macPort);
}

int32_t CpswMacPort_ioctlInterVlan(EnetMod_Handle hMod,
                                   uint32_t cmd,
                                   Enet_IoctlPrms *prms)
{
    CpswMacPort_Handle hPort = (CpswMacPort_Handle)hMod;
    CSL_Xge_cpswRegs *regs = (CSL_Xge_cpswRegs *)hMod->virtAddr;
    Enet_MacPort macPort = hPort->macPort;
    uint32_t portId = ENET_MACPORT_ID(macPort);
    int32_t status = ENET_SOK;

    ENETTRACE_VAR(portId);
    switch (cmd)
    {
        case CPSW_MACPORT_IOCTL_SET_INTERVLAN_ROUTE:
        {
            const CpswMacPort_InterVlanRoutingCfg *interVlanCfg =
                (const CpswMacPort_InterVlanRoutingCfg *)prms->inArgs;
            CpswMacPort_InterVlanRouteId *routeId =
                (CpswMacPort_InterVlanRouteId *)prms->outArgs;

            status = CpswMacPort_setRoute(regs, macPort, interVlanCfg, routeId);
            ENETTRACE_ERR_IF(status != ENET_SOK, status,
                             "MAC %u: Failed to set interVLAN route", portId);
        }
        break;

        case CPSW_MACPORT_IOCTL_SET_SPECIFIC_INTERVLAN_ROUTE:
        {
            const CpswMacPort_SetSpecificInterVlanRouteInArgs *inArgs =
                (const CpswMacPort_SetSpecificInterVlanRouteInArgs *)prms->inArgs;

            status = CpswMacPort_setCfg(regs, macPort, inArgs->routeId, &inArgs->routeCfg);
            ENETTRACE_ERR_IF(status != ENET_SOK, status,
                             "MAC %u: Failed to set specific interVLAN cfg", portId);
        }
        break;

        case CPSW_MACPORT_IOCTL_GET_INTERVLAN_ROUTE:
        {
            CpswMacPort_InterVlanRouteId routeId =
                *(CpswMacPort_InterVlanRouteId *)prms->inArgs;
            CpswMacPort_InterVlanRoutingCfg *interVlanCfg =
                (CpswMacPort_InterVlanRoutingCfg *)prms->outArgs;

            status = CpswMacPort_getCfg(regs, macPort, routeId, interVlanCfg);
            ENETTRACE_ERR_IF(status != ENET_SOK, status,
                             "MAC %u: failed to get interVLAN config", portId);
        }
        break;

        case CPSW_MACPORT_IOCTL_DELETE_INTERVLAN_ROUTE:
        {
            const CpswMacPort_InterVlanRoutingCfg *interVlanCfg =
                (const CpswMacPort_InterVlanRoutingCfg *)prms->inArgs;
            CpswMacPort_InterVlanRouteId *routeId =
                (CpswMacPort_InterVlanRouteId *)prms->outArgs;

            status = CpswMacPort_delRoute(regs, macPort, interVlanCfg, routeId);
            ENETTRACE_ERR_IF(status != ENET_SOK, status,
                             "MAC %u: Failed to delete interVLAN route", portId);
        }
        break;

        case CPSW_MACPORT_IOCTL_GET_INTERVLAN_FREEROUTES:
        {
            CpswMacPort_InterVlanFreeRouteInfo *freeRoutes =
                (CpswMacPort_InterVlanFreeRouteInfo *)prms->outArgs;

            status = CpswMacPort_getFreeRoutes(regs, macPort, freeRoutes);
            ENETTRACE_ERR_IF(status != ENET_SOK, status,
                             "MAC %u: failed to get free interVLAN routes", portId);
        }
        break;

        case CPSW_MACPORT_IOCTL_FIND_INTERVLAN_ROUTE:
        {
            const CpswMacPort_InterVlanRoutingCfg *interVlanCfg =
                (const CpswMacPort_InterVlanRoutingCfg *)prms->inArgs;
            CpswMacPort_InterVlanRouteId *routeId = (CpswMacPort_InterVlanRouteId *)prms->outArgs;

            status = CpswMacPort_findRouteId(regs, macPort, interVlanCfg, routeId);
            ENETTRACE_ERR_IF(status != ENET_SOK, status,
                             "MAC %u: failed to find interVLAN route", portId);
        }
        break;

        case CPSW_MACPORT_IOCTL_IS_INTERVLAN_ROUTE_FREE:
        {
            CpswMacPort_InterVlanRouteId routeId = *(CpswMacPort_InterVlanRouteId *)prms->inArgs;
            bool *isFree = (bool *)prms->outArgs;

            status = CpswMacPort_checkRouteIsFree(regs, macPort, routeId, isFree);
            ENETTRACE_ERR_IF(status != ENET_SOK, status,
                             "MAC %u: failed to check interVLAN route state", portId);
        }
        break;

        default:
        {
            status = ENET_EINVALIDPARAMS;
            ENETTRACE_ERR(status, "Invalid IOCTL cmd 0x%08x", cmd);
        }
        break;
    }

    return status;
}

static bool CpswMacPort_isRouteIdFree(CSL_Xge_cpswRegs *regs,
                                      uint32_t portNum,
                                      CpswMacPort_InterVlanRouteId routeId)
{
    CSL_CPSW_INTERVLANCFG cslCfg;
    uint32_t interVlanPtr = CPSW_MACPORT_INTERVLAN_ROUTE2PTR(routeId);
    bool isFree = BFALSE;

    CSL_CPGMAC_SL_getInterVLANCfg(regs, portNum, interVlanPtr, &cslCfg);

    /* If a route does not have replace VId enabled it is not used. Use that as an
     * indicator to identify free routes.
     * We cannot use any software state to maintain interVLAN route allocation as
     * software state may get lost during warm boot. */
    if ((cslCfg.replaceVid == 0U) &&
        (cslCfg.destForceUntaggedEgress == 0U))
    {
        isFree = BTRUE;
    }

    return isFree;
}

static bool CpswMacPort_cmpRoutes(const CpswMacPort_InterVlanRoutingCfg *cfg1,
                                  const CpswMacPort_InterVlanRoutingCfg *cfg2)
{
    bool isEqual;

    isEqual = (cfg1->decrementTTL == cfg2->decrementTTL) &&
              (cfg1->forceUntaggedEgress == cfg2->forceUntaggedEgress) &&
              (cfg1->replaceDASA == cfg2->replaceDASA);

    if (isEqual && !cfg1->forceUntaggedEgress)
    {
        isEqual = isEqual && (cfg1->vlanId == cfg2->vlanId);
    }

    if (isEqual && cfg1->replaceDASA)
    {
        isEqual = isEqual && EnetUtils_cmpMacAddr(cfg1->dstAddr, cfg2->dstAddr);
        isEqual = isEqual && EnetUtils_cmpMacAddr(cfg1->srcAddr, cfg2->srcAddr);
    }

    return isEqual;
}

static void CpswMacPort_clearRouteId(CSL_Xge_cpswRegs *regs,
                                     uint32_t portNum,
                                     CpswMacPort_InterVlanRouteId routeId)
{
    CSL_CPSW_INTERVLANCFG cslCfg;
    uint32_t interVlanPtr = CPSW_MACPORT_INTERVLAN_ROUTE2PTR(routeId);

    cslCfg.decrementTtl = UFALSE;
    cslCfg.destForceUntaggedEgress = UFALSE;
    cslCfg.replaceDaSa = UFALSE;
    cslCfg.replaceVid = UFALSE;
    cslCfg.vid = UFALSE;
    EnetUtils_clearMacAddr(&cslCfg.dstMacAddress[0U]);
    EnetUtils_clearMacAddr(&cslCfg.srcMacAddress[0U]);

    CSL_CPGMAC_SL_setInterVLANCfg(regs, portNum, interVlanPtr, &cslCfg);
}

static void CpswMacPort_clearAllRoutes(CSL_Xge_cpswRegs *regs,
                                       Enet_MacPort macPort)
{
    CpswMacPort_InterVlanRouteId i;
    uint32_t portNum = ENET_MACPORT_NORM(macPort);

    for (i = CPSW_MACPORT_INTERVLAN_ROUTEID_FIRST;
         i <= CPSW_MACPORT_INTERVLAN_ROUTEID_LAST;
         i = (CpswMacPort_InterVlanRouteId)(i + 1))
    {
        if (!CpswMacPort_isRouteIdFree(regs, portNum, i))
        {
            CpswMacPort_clearRouteId(regs, portNum, i);
        }
    }
}

static int32_t CpswMacPort_getCfg(CSL_Xge_cpswRegs *regs,
                                  Enet_MacPort macPort,
                                  CpswMacPort_InterVlanRouteId routeId,
                                  CpswMacPort_InterVlanRoutingCfg *interVlanCfg)
{
    CSL_CPSW_INTERVLANCFG cslCfg;
    uint32_t portNum = ENET_MACPORT_NORM(macPort);
    uint32_t portId = ENET_MACPORT_ID(macPort);
    uint32_t interVlanPtr;
    int32_t status = ENET_SOK;

    ENETTRACE_VAR(portId);
    if (routeId <= CPSW_MACPORT_INTERVLAN_ROUTEID_LAST)
    {
        interVlanPtr = CPSW_MACPORT_INTERVLAN_ROUTE2PTR(routeId);

        CSL_CPGMAC_SL_getInterVLANCfg(regs, portNum, interVlanPtr, &cslCfg);

        interVlanCfg->decrementTTL        = cslCfg.decrementTtl;
        interVlanCfg->forceUntaggedEgress = cslCfg.destForceUntaggedEgress;
        interVlanCfg->replaceDASA         = cslCfg.replaceDaSa;
        interVlanCfg->vlanId              = cslCfg.vid;

        EnetUtils_copyMacAddr(interVlanCfg->dstAddr, cslCfg.dstMacAddress);
        EnetUtils_copyMacAddr(interVlanCfg->srcAddr, cslCfg.srcMacAddress);
    }
    else
    {
        status = ENET_EINVALIDPARAMS;
        ENETTRACE_ERR(status, "MAC %u: Invalid route id %u", portId, routeId);
    }

    return status;
}

static int32_t CpswMacPort_setCfg(CSL_Xge_cpswRegs *regs,
                                  Enet_MacPort macPort,
                                  CpswMacPort_InterVlanRouteId routeId,
                                  const CpswMacPort_InterVlanRoutingCfg *interVlanCfg)
{
    CSL_CPSW_INTERVLANCFG cslCfg;
    uint32_t portNum = ENET_MACPORT_NORM(macPort);
    uint32_t portId = ENET_MACPORT_ID(macPort);
    uint32_t interVlanPtr;
    int32_t status = ENET_SOK;

    ENETTRACE_VAR(portId);
    if (routeId > CPSW_MACPORT_INTERVLAN_ROUTEID_LAST)
    {
        status = ENET_EINVALIDPARAMS;
        ENETTRACE_ERR(status, "MAC %u: Invalid route id %u", portId, routeId);
    }

    if (EnetUtils_isMcastAddr(interVlanCfg->srcAddr))
    {
        status = ENET_EINVALIDPARAMS;
        ENETTRACE_ERR(status, "MAC %u: source addr is multicast", portId);
    }

    if (interVlanCfg->vlanId > ENET_VLAN_ID_MAX)
    {
        status = ENET_EINVALIDPARAMS;
        ENETTRACE_ERR(status, "MAC %u: interVLAN VLAN ID %u exceeds max", portId, interVlanCfg->vlanId);
    }

    if (status == ENET_SOK)
    {
        interVlanPtr = CPSW_MACPORT_INTERVLAN_ROUTE2PTR(routeId);

        cslCfg.decrementTtl            = interVlanCfg->decrementTTL;
        cslCfg.destForceUntaggedEgress = interVlanCfg->forceUntaggedEgress;
        cslCfg.replaceDaSa             = interVlanCfg->replaceDASA;
        EnetUtils_copyMacAddr(cslCfg.dstMacAddress, interVlanCfg->dstAddr);

        /* CPSW IP required replaceVid irrespective of whether VID needs to be replaced
         * or not. Else DA/SA VLAN strip is not functional. Hence always enable replaceVid. */
        cslCfg.replaceVid = UTRUE;
        cslCfg.vid        = interVlanCfg->vlanId;
        EnetUtils_copyMacAddr(cslCfg.srcMacAddress, interVlanCfg->srcAddr);
        CSL_CPGMAC_SL_setInterVLANCfg(regs, portNum, interVlanPtr, &cslCfg);
    }

    return status;
}

static int32_t CpswMacPort_findRouteId(CSL_Xge_cpswRegs *regs,
                                       Enet_MacPort macPort,
                                       const CpswMacPort_InterVlanRoutingCfg *routeMatchCfg,
                                       CpswMacPort_InterVlanRouteId *routeId)
{
    CpswMacPort_InterVlanRoutingCfg routeCfg;
    CpswMacPort_InterVlanRouteId i;
    bool found = BFALSE;
    uint32_t portId = ENET_MACPORT_ID(macPort);
    int32_t status;

    ENETTRACE_VAR(portId);
    for (i = CPSW_MACPORT_INTERVLAN_ROUTEID_FIRST;
         i <= CPSW_MACPORT_INTERVLAN_ROUTEID_LAST;
         i = (CpswMacPort_InterVlanRouteId)(i + 1))
    {
        if (CpswMacPort_getCfg(regs, macPort, i, &routeCfg) == ENET_SOK)
        {
            if (CpswMacPort_cmpRoutes(routeMatchCfg, &routeCfg) == BTRUE)
            {
                found = BTRUE;
                break;
            }
        }
    }

    if (found)
    {
        Enet_devAssert(i <= CPSW_MACPORT_INTERVLAN_ROUTEID_LAST, "MAC %u: Invalid route id: %d", portId, i);
        *routeId = i;
        status = ENET_SOK;
    }
    else
    {
        status = ENET_ENOTFOUND;
        ENETTRACE_DBG("MAC %u: No interVLAN found with matching params", portId);
    }

    return status;
}

static int32_t CpswMacPort_delRoute(CSL_Xge_cpswRegs *regs,
                                    Enet_MacPort macPort,
                                    const CpswMacPort_InterVlanRoutingCfg *interVlanCfg,
                                    CpswMacPort_InterVlanRouteId *routeId)
{
    uint32_t portNum = ENET_MACPORT_NORM(macPort);
    uint32_t portId = ENET_MACPORT_ID(macPort);
    int32_t status;

    ENETTRACE_VAR(portId);
    status = CpswMacPort_findRouteId(regs, macPort, interVlanCfg, routeId);
    if (status == ENET_SOK)
    {
        CpswMacPort_clearRouteId(regs, portNum, *routeId);
    }
    else
    {
        ENETTRACE_ERR(status, "MAC %u: Failed to find InterVLAN route to be deleted", portId);
    }

    return status;
}

static int32_t CpswMacPort_setRoute(CSL_Xge_cpswRegs *regs,
                                    Enet_MacPort macPort,
                                    const CpswMacPort_InterVlanRoutingCfg *interVlanCfg,
                                    CpswMacPort_InterVlanRouteId *routeId)
{
    CpswMacPort_InterVlanFreeRouteInfo freeRoutes;
    uint32_t portId = ENET_MACPORT_ID(macPort);
    int32_t status;

    ENETTRACE_VAR(portId);
    status = CpswMacPort_findRouteId(regs, macPort, interVlanCfg, routeId);
    if (status == ENET_SOK)
    {
        /* Entry already present. No need to add entry */
    }
    else
    {
        status = CpswMacPort_getFreeRoutes(regs, macPort, &freeRoutes);
        if (status == ENET_SOK)
        {
            if (freeRoutes.numFreeRoutes > 0U)
            {
                status = CpswMacPort_setCfg(regs, macPort, freeRoutes.freeRouteId[0U], interVlanCfg);
                if (status == ENET_SOK)
                {
                    *routeId = freeRoutes.freeRouteId[0];
                }
                else
                {
                    ENETTRACE_ERR(status, "MAC %u: Failed to set interVLAN config", portId);
                }
            }
            else
            {
                status = ENET_EFAIL;
                ENETTRACE_ERR(status, "MAC %u: No free routes available", portId);
            }
        }
        else
        {
            ENETTRACE_ERR(status, "MAC %u: Failed to get free interVLAN routes", portId);
        }
    }

    return status;
}

static int32_t CpswMacPort_getFreeRoutes(CSL_Xge_cpswRegs *regs,
                                         Enet_MacPort macPort,
                                         CpswMacPort_InterVlanFreeRouteInfo *freeRoutes)
{
    CpswMacPort_InterVlanRouteId i;
    uint32_t portNum = ENET_MACPORT_NORM(macPort);
    uint32_t portId = ENET_MACPORT_ID(macPort);
    int32_t status = ENET_SOK;

    ENETTRACE_VAR(portId);
    freeRoutes->numFreeRoutes = 0U;

    for (i = CPSW_MACPORT_INTERVLAN_ROUTEID_FIRST;
         i <= CPSW_MACPORT_INTERVLAN_ROUTEID_LAST;
         i = (CpswMacPort_InterVlanRouteId)(i + 1))
    {
        if (CpswMacPort_isRouteIdFree(regs, portNum, i))
        {
            if (freeRoutes->numFreeRoutes < ENET_ARRAYSIZE(freeRoutes->freeRouteId))
            {
                freeRoutes->freeRouteId[freeRoutes->numFreeRoutes] = i;
                freeRoutes->numFreeRoutes++;
            }
            else
            {
                status = ENET_EINVALIDPARAMS;
                ENETTRACE_ERR(status, "MAC %u: Not enough space for free routes");
                break;
            }
        }
    }

    return status;
}

static int32_t CpswMacPort_checkRouteIsFree(CSL_Xge_cpswRegs *regs,
                                            Enet_MacPort macPort,
                                            CpswMacPort_InterVlanRouteId routeId,
                                            bool *isFree)
{
    uint32_t portNum = ENET_MACPORT_NORM(macPort);
    uint32_t portId = ENET_MACPORT_ID(macPort);
    int32_t status = ENET_SOK;

    ENETTRACE_VAR(portId);
    if (routeId <= CPSW_MACPORT_INTERVLAN_ROUTEID_LAST)
    {
        *isFree = CpswMacPort_isRouteIdFree(regs, portNum, routeId);
    }
    else
    {
        status = ENET_EINVALIDPARAMS;
        ENETTRACE_ERR(status, "MAC %u: Invalid route id %u", portId, routeId);
    }

    return status;
}
#endif
