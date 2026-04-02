/*
 *  Copyright (C) 2024 Texas Instruments Incorporated
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
 *  \file     safety_checkers_tifs.c
 *
 *  \brief    This file contains TIFS safety checker library functions
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <cslr.h>
#include <sciclient.h>
#include <safety_checkers_common.h>
#include <safety_checkers_tifs.h>

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

/* None */

/* ========================================================================== */
/*                  Internal/Private Function Declarations                    */
/* ========================================================================== */

static uint32_t SafetyCheckers_tifsGetFwlRegValue(uint32_t fwlId, uint32_t fwlRegion, uint32_t fwlReg);
static uint32_t SafetyCheckers_tifsGetIscCbassRegValue(uint32_t iscId, uint32_t iscRegion, uint32_t iscReg);
#if defined(SOC_J784S4) || defined(SOC_J721S2 )|| defined(SOC_J742S2)
static uint32_t SafetyCheckers_tifsGetIscCcRegValue(uint32_t iscIdOffset, uint32_t iscRegion, uint32_t iscReg);
#endif
#if defined(SOC_J7200) || defined(SOC_J721E ) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)
static uint32_t SafetyCheckers_tifsGetIscRaRegValue(uint32_t iscId, uint32_t iscRegion, uint32_t iscReg);
#endif

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/**
 * Design: SAFETY_CHECKERS-40
*/
int32_t SafetyCheckers_tifsReqFwlOpen(void)
{
    int32_t status = SAFETY_CHECKERS_SOK;

    struct tisci_fwl_req request = {0};
    struct tisci_fwl_resp response = {0};

    Sciclient_ReqPrm_t  reqParam = {0};
    Sciclient_RespPrm_t respParam = {0};

    reqParam.messageType    = (uint16_t) SAFETY_CHECKERS_TIFS_FWL_OPEN;
    reqParam.flags          = (uint32_t) TISCI_MSG_FLAG_AOP;
    reqParam.pReqPayload    = (const uint8_t *) &request;
    reqParam.reqPayloadSize = (uint32_t) sizeof (request);
    reqParam.timeout        = (uint32_t) SAFETY_CHECKERS_DEFAULT_TIMEOUT;

    respParam.flags           = (uint32_t) 0;   /* Populated by the API */
    respParam.pRespPayload    = (uint8_t *) &response;
    respParam.respPayloadSize = (uint32_t)  sizeof (response);

    status = Sciclient_service(&reqParam, &respParam);
    if ((status == SAFETY_CHECKERS_SOK)  && (respParam.flags == TISCI_MSG_FLAG_ACK))
    {
        status = SAFETY_CHECKERS_SOK;
    }
    else
    {
        status = SAFETY_CHECKERS_FAIL;
    }
    return status;
}

/**
 * Design: SAFETY_CHECKERS-41
*/
int32_t SafetyCheckers_tifsGetFwlCfg(SafetyCheckers_TifsFwlConfig *fwlConfig, uint32_t size)
{
    uint32_t i = 0U, j = 0U, id = 0U;
    int32_t status = SAFETY_CHECKERS_SOK;

    for (i = 0U; i<size; i++)
    {
        id = fwlConfig[i].fwlId;
        if (fwlConfig[i].numRegions <= fwlConfig[i].maxNumRegions)
        {
            for (j = 0U; j<fwlConfig[i].numRegions; j++)
            {
                /* Read control register from firewall registers */
                fwlConfig[i].fwlCfgPerRegion[j].controlReg = SafetyCheckers_tifsGetFwlRegValue(id, j, SAFETY_CHECKERS_TIFS_CONTROL_REG);

                /* Read permission registers from firewall registers */
                fwlConfig[i].fwlCfgPerRegion[j].privId0 = SafetyCheckers_tifsGetFwlRegValue(id, j, SAFETY_CHECKERS_TIFS_PRIV_ID0);
                fwlConfig[i].fwlCfgPerRegion[j].privId1 = SafetyCheckers_tifsGetFwlRegValue(id, j, SAFETY_CHECKERS_TIFS_PRIV_ID1);
                fwlConfig[i].fwlCfgPerRegion[j].privId2 = SafetyCheckers_tifsGetFwlRegValue(id, j, SAFETY_CHECKERS_TIFS_PRIV_ID2);

                /* Read start address from firewall registers */
                fwlConfig[i].fwlCfgPerRegion[j].startAddrLow =  SafetyCheckers_tifsGetFwlRegValue(id, j, SAFETY_CHECKERS_TIFS_START_ADDRL);
                fwlConfig[i].fwlCfgPerRegion[j].startAddrHigh = SafetyCheckers_tifsGetFwlRegValue(id, j, SAFETY_CHECKERS_TIFS_START_ADDRH);

                /* Read end address from firewall register */
                fwlConfig[i].fwlCfgPerRegion[j].endAddrLow =  SafetyCheckers_tifsGetFwlRegValue(id, j, SAFETY_CHECKERS_TIFS_END_ADDRL);
                fwlConfig[i].fwlCfgPerRegion[j].endAddrHigh = SafetyCheckers_tifsGetFwlRegValue(id, j, SAFETY_CHECKERS_TIFS_END_ADDRH);
            }
        }
        else
        {
            status = SAFETY_CHECKERS_FAIL;
        }
    }
    return status;
}

/**
 * Design: SAFETY_CHECKERS-368
*/
int32_t SafetyCheckers_tifsGetIscCbassCfg(SafetyCheckers_TifsIscCbassConfig *iscConfig, uint32_t size)
{
    uint32_t i = 0U, j = 0U, id = 0U;
    int32_t status = SAFETY_CHECKERS_SOK;

    for (i = 0U; i<size; i++)
    {
        id = iscConfig[i].iscId;
        if (iscConfig[i].numRegions <= iscConfig[i].maxNumRegions)
        {
            for (j = 0U; j<iscConfig[i].numRegions; j++)
            {
                /* Read control register from isc registers */
                iscConfig[i].iscCfgPerRegionCbass[j].controlReg0 = SafetyCheckers_tifsGetIscCbassRegValue(id, j, SAFETY_CHECKERS_TIFS_ISC_CONTROL0_REG);
                iscConfig[i].iscCfgPerRegionCbass[j].controlReg1 = SafetyCheckers_tifsGetIscCbassRegValue(id, j, SAFETY_CHECKERS_TIFS_ISC_CONTROL1_REG);

                /* Read start address from isc registers */
                iscConfig[i].iscCfgPerRegionCbass[j].startAddrLow =  SafetyCheckers_tifsGetIscCbassRegValue(id, j, SAFETY_CHECKERS_TIFS_START_ADDRL);
                iscConfig[i].iscCfgPerRegionCbass[j].startAddrHigh = SafetyCheckers_tifsGetIscCbassRegValue(id, j, SAFETY_CHECKERS_TIFS_START_ADDRH);

                /* Read end address from isc register */
                iscConfig[i].iscCfgPerRegionCbass[j].endAddrLow =  SafetyCheckers_tifsGetIscCbassRegValue(id, j, SAFETY_CHECKERS_TIFS_END_ADDRL);
                iscConfig[i].iscCfgPerRegionCbass[j].endAddrHigh = SafetyCheckers_tifsGetIscCbassRegValue(id, j, SAFETY_CHECKERS_TIFS_END_ADDRH);
            }
            iscConfig[i].rcr = SafetyCheckers_tifsGetIscCbassRegValue(id, j, 0U);
        }
        else
        {
            status = SAFETY_CHECKERS_FAIL;
        }
    }
    return status;
}

#if defined(SOC_J784S4) || defined(SOC_J721S2 )|| defined(SOC_J742S2)
/**
 * Design: SAFETY_CHECKERS-368
*/
int32_t SafetyCheckers_tifsGetIscCcCfg(SafetyCheckers_TifsIscCcConfig *iscConfig, uint32_t size)
{
    uint32_t i = 0U, j = 0U, id = 0U;
    int32_t status = SAFETY_CHECKERS_SOK;

    for (i = 0U; i<size; i++)
    {
        id = iscConfig[i].iscIdOffset;
        if (iscConfig[i].numRegions <= iscConfig[i].maxNumRegions)
        {
            for (j = 0U; j<iscConfig[i].numRegions; j++)
                {
                    /* Read privId and lock register from isc registers */
                    iscConfig[i].iscCfgPerRegionCc[j].privId = SafetyCheckers_tifsGetIscCcRegValue(id, j, SAFETY_CHECKERS_TIFS_ISC_PRIVID_REG);
                    iscConfig[i].iscCfgPerRegionCc[j].lock = SafetyCheckers_tifsGetIscCcRegValue(id, j, SAFETY_CHECKERS_TIFS_ISC_PRIVID_LOCK_REG);
                }
        }
        else
        {
            status = SAFETY_CHECKERS_FAIL;
        }
    }
    return status;
}
#endif

#if defined(SOC_J7200) || defined(SOC_J721E ) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)
/**
 * Design: SAFETY_CHECKERS-368
*/
int32_t SafetyCheckers_tifsGetIscRaCfg(SafetyCheckers_TifsIscRaConfig *iscConfig, uint32_t size)
{
    uint32_t i = 0U, j = 0U, id = 0U;
    int32_t status = SAFETY_CHECKERS_SOK;

    for (i = 0U; i<size; i++)
    {
        id = iscConfig[i].iscId;
        if (iscConfig[i].numRegions <= iscConfig[i].maxNumRegions)
        {
            for (j = 0U; j<iscConfig[i].maxNumRegions; j++)
            {
                /* Read control register from ra isc registers */
                iscConfig[i].iscCfgPerRegionRa[j].controlReg1 = SafetyCheckers_tifsGetIscRaRegValue(id, j, SAFETY_CHECKERS_TIFS_ISC_CONTROL0_REG);
                iscConfig[i].iscCfgPerRegionRa[j].controlReg2 = SafetyCheckers_tifsGetIscRaRegValue(id, j, SAFETY_CHECKERS_TIFS_ISC_CONTROL1_REG);
            }
        }
        else
        {
            status = SAFETY_CHECKERS_FAIL;
        }
    }
    return status;
}
#endif

/**
 * Design: SAFETY_CHECKERS-42
*/
int32_t SafetyCheckers_tifsVerifyFwlCfg(const SafetyCheckers_TifsFwlConfig *fwlConfig, uint32_t size)
{
    uint32_t i = 0U, j = 0U, regData = 0U, id = 0U, mismatch = 0U;
    int32_t status = SAFETY_CHECKERS_SOK;

    for (i = 0U; i<size; i++)
    {
        id = fwlConfig[i].fwlId;
        if (fwlConfig[i].numRegions > fwlConfig[i].maxNumRegions)
        {
            status = SAFETY_CHECKERS_FAIL;
            break;
        }
        for (j = 0U; j<fwlConfig[i].numRegions; j++)
        {
            mismatch = 0U;

            /* Read control register and check for mismatch */
            regData = SafetyCheckers_tifsGetFwlRegValue(id, j, SAFETY_CHECKERS_TIFS_CONTROL_REG);
            mismatch |= fwlConfig[i].fwlCfgPerRegion[j].controlReg ^ regData;

            /* Read permission registers and check for mismatch */
            regData = SafetyCheckers_tifsGetFwlRegValue(id, j, SAFETY_CHECKERS_TIFS_PRIV_ID0);
            mismatch |= fwlConfig[i].fwlCfgPerRegion[j].privId0 ^ regData;

            regData = SafetyCheckers_tifsGetFwlRegValue(id, j, SAFETY_CHECKERS_TIFS_PRIV_ID1);
            mismatch |= fwlConfig[i].fwlCfgPerRegion[j].privId1 ^ regData;

            regData = SafetyCheckers_tifsGetFwlRegValue(id, j, SAFETY_CHECKERS_TIFS_PRIV_ID2);
            mismatch |= fwlConfig[i].fwlCfgPerRegion[j].privId2 ^ regData;

            /* Read start address and check for mismatch */
            regData = SafetyCheckers_tifsGetFwlRegValue(id, j, SAFETY_CHECKERS_TIFS_START_ADDRL);
            mismatch |= fwlConfig[i].fwlCfgPerRegion[j].startAddrLow ^ regData;

            regData = SafetyCheckers_tifsGetFwlRegValue(id, j, SAFETY_CHECKERS_TIFS_START_ADDRH);
            mismatch |= fwlConfig[i].fwlCfgPerRegion[j].startAddrHigh ^ regData;

            /* Read end address and check for mismatch */
            regData = SafetyCheckers_tifsGetFwlRegValue(id, j, SAFETY_CHECKERS_TIFS_END_ADDRL);
            mismatch |= fwlConfig[i].fwlCfgPerRegion[j].endAddrLow ^ regData;

            regData = SafetyCheckers_tifsGetFwlRegValue(id, j, SAFETY_CHECKERS_TIFS_END_ADDRH);
            mismatch |= fwlConfig[i].fwlCfgPerRegion[j].endAddrHigh ^ regData;

            if(mismatch != 0U)
            {
                status = SAFETY_CHECKERS_REG_DATA_MISMATCH;
            }
        }
    }
    return status;
}

/**
 * Design: SAFETY_CHECKERS-310
*/
int32_t SafetyCheckers_tifsVerifyIscCbassCfg(const SafetyCheckers_TifsIscCbassConfig *iscConfig, uint32_t size)
{
    uint32_t i = 0U, j = 0U, regData = 0U, id = 0U, mismatch = 0U;
    int32_t status = SAFETY_CHECKERS_SOK;

    for (i = 0U; i<size; i++)
    {
        id = iscConfig[i].iscId;
        if (iscConfig[i].numRegions <= iscConfig[i].maxNumRegions)
        {
            for (j = 0U; j<iscConfig[i].numRegions; j++)
            {
                mismatch = 0U;

                /* Read control register and check for mismatch */
                regData = SafetyCheckers_tifsGetIscCbassRegValue(id, j, SAFETY_CHECKERS_TIFS_ISC_CONTROL0_REG);
                mismatch |= iscConfig[i].iscCfgPerRegionCbass[j].controlReg0 ^ regData;

                regData = SafetyCheckers_tifsGetIscCbassRegValue(id, j, SAFETY_CHECKERS_TIFS_ISC_CONTROL1_REG);
                mismatch |= iscConfig[i].iscCfgPerRegionCbass[j].controlReg1 ^ regData;

                /* Read start address and check for mismatch */
                regData = SafetyCheckers_tifsGetIscCbassRegValue(id, j, SAFETY_CHECKERS_TIFS_START_ADDRL);
                mismatch |= iscConfig[i].iscCfgPerRegionCbass[j].startAddrLow ^ regData;

                regData = SafetyCheckers_tifsGetIscCbassRegValue(id, j, SAFETY_CHECKERS_TIFS_START_ADDRH);
                mismatch |= iscConfig[i].iscCfgPerRegionCbass[j].startAddrHigh ^ regData;

                /* Read end address and check for mismatch */
                regData = SafetyCheckers_tifsGetIscCbassRegValue(id, j, SAFETY_CHECKERS_TIFS_END_ADDRL);
                mismatch |= iscConfig[i].iscCfgPerRegionCbass[j].endAddrLow ^ regData;

                regData = SafetyCheckers_tifsGetIscCbassRegValue(id, j, SAFETY_CHECKERS_TIFS_END_ADDRH);
                mismatch |= iscConfig[i].iscCfgPerRegionCbass[j].endAddrHigh ^ regData;

            }
            regData = SafetyCheckers_tifsGetIscCbassRegValue(id, j, 0U);
            mismatch |= iscConfig[i].rcr ^ regData;

            if (mismatch != 0U)
            {
                status = SAFETY_CHECKERS_REG_DATA_MISMATCH;
            }
        }
        else
        {
            status = SAFETY_CHECKERS_FAIL;
        }
    }
    return status;
}

#if defined(SOC_J784S4) || defined(SOC_J721S2 )|| defined(SOC_J742S2)
/**
 * Design: SAFETY_CHECKERS-310
*/
int32_t SafetyCheckers_tifsVerifyIscCcCfg(const SafetyCheckers_TifsIscCcConfig *iscConfig, uint32_t size)
{
    uint32_t i = 0U, j = 0U, regData = 0U, id = 0U, mismatch = 0U;
    int32_t status = SAFETY_CHECKERS_SOK;

    for (i = 0U; i<size; i++)
    {
        id = iscConfig[i].iscIdOffset;
        if (iscConfig[i].numRegions > iscConfig[i].maxNumRegions)
        {
            status = SAFETY_CHECKERS_FAIL;
            break;
        }
        for (j = 0U; j<iscConfig[i].numRegions; j++)
        {
            mismatch = 0U;

            /* Read privId and lock register from isc registers */
            regData = SafetyCheckers_tifsGetIscCcRegValue(id, j, SAFETY_CHECKERS_TIFS_ISC_PRIVID_REG);
            mismatch |= iscConfig[i].iscCfgPerRegionCc[j].privId  ^ regData;

            regData = SafetyCheckers_tifsGetIscCcRegValue(id, j, SAFETY_CHECKERS_TIFS_ISC_PRIVID_LOCK_REG);
            mismatch |= iscConfig[i].iscCfgPerRegionCc[j].lock  ^ regData;

            if(mismatch != 0U)
            {
                status = SAFETY_CHECKERS_REG_DATA_MISMATCH;
            }
        }
    }
    return status;
}
#endif

#if defined(SOC_J7200) || defined(SOC_J721E ) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)
/**
 * Design: SAFETY_CHECKERS-310
*/
int32_t SafetyCheckers_tifsVerifyIscRaCfg(const SafetyCheckers_TifsIscRaConfig *iscConfig, uint32_t size)
{
    uint32_t i = 0U, j = 0U, regData = 0U, id = 0U, mismatch = 0U;
    int32_t status = SAFETY_CHECKERS_SOK;

    for (i = 0U; i<size; i++)
    {
        id = iscConfig[i].iscId;
        if (iscConfig[i].numRegions > iscConfig[i].maxNumRegions)
        {
            status = SAFETY_CHECKERS_FAIL;
            break;
        }
        for (j = 0U; j<iscConfig[i].maxNumRegions; j++)
        {
            mismatch = 0U;

            /* Read privId and lock register from isc registers */
            regData = SafetyCheckers_tifsGetIscRaRegValue(id, j, SAFETY_CHECKERS_TIFS_ISC_CONTROL0_REG);
            mismatch |= iscConfig[i].iscCfgPerRegionRa[j].controlReg1  ^ regData;

            regData = SafetyCheckers_tifsGetIscRaRegValue(id, j, SAFETY_CHECKERS_TIFS_ISC_CONTROL1_REG);
            mismatch |= iscConfig[i].iscCfgPerRegionRa[j].controlReg2  ^ regData;

            if(mismatch != 0U)
            {
                status = SAFETY_CHECKERS_REG_DATA_MISMATCH;
            }
        }
    }
    return status;
}
#endif

/**
 * Design: SAFETY_CHECKERS-43
*/
int32_t SafetyCheckers_tifsReqFwlClose(void)
{
    int32_t status = SAFETY_CHECKERS_SOK;

    struct tisci_fwl_req request;
    struct tisci_fwl_resp response;

    Sciclient_ReqPrm_t  reqParam = {0};
    Sciclient_RespPrm_t respParam = {0};

    reqParam.messageType    = (uint16_t) SAFETY_CHECKERS_TIFS_FWL_CLOSE;
    reqParam.flags          = (uint32_t) TISCI_MSG_FLAG_AOP;
    reqParam.pReqPayload    = (const uint8_t *) &request;
    reqParam.reqPayloadSize = (uint32_t) sizeof (request);
    reqParam.timeout        = (uint32_t) SAFETY_CHECKERS_DEFAULT_TIMEOUT;

    respParam.flags           = (uint32_t) 0;   /* Populated by the API */
    respParam.pRespPayload    = (uint8_t *) &response;
    respParam.respPayloadSize = (uint32_t)  sizeof (response);

   status = Sciclient_service(&reqParam, &respParam);
   if((status == SAFETY_CHECKERS_SOK) && (respParam.flags == TISCI_MSG_FLAG_ACK))
   {
        status = SAFETY_CHECKERS_SOK;
   }
   else
   {
        status = SAFETY_CHECKERS_FAIL;
   }
   return status;
}

/* ========================================================================== */
/*                   Internal/Private Function Definitions                    */
/* ========================================================================== */

static uint32_t SafetyCheckers_tifsGetFwlRegValue(uint32_t fwlId, uint32_t fwlRegion, uint32_t fwlReg)
{
    uint32_t fwlBlkBaseAddr = 0U, fwlBlkOffset = 0U, fwlRegionOffset = 0U, fwlRegValue = 0U;
    fwlBlkBaseAddr = SAFETY_CHECKERS_TIFS_FWL_BASE;
    fwlBlkOffset = fwlBlkBaseAddr + (0x400U * fwlId);
    fwlRegionOffset = fwlBlkOffset + (0x20U * fwlRegion);
    fwlRegValue = CSL_REG32_RD(fwlRegionOffset + fwlReg);
    return fwlRegValue;
}

static uint32_t SafetyCheckers_tifsGetIscCbassRegValue(uint32_t iscId, uint32_t iscRegion, uint32_t iscReg)
{
    uint32_t iscBlkBaseAddr = 0U, iscBlkOffset = 0U, iscRegionOffset = 0U, iscRegValue = 0U;
    iscBlkBaseAddr = SAFETY_CHECKERS_TIFS_ISC_BASE;
    iscBlkOffset = iscBlkBaseAddr + (0x400U * iscId);
    iscRegionOffset = iscBlkOffset + (0x20U * iscRegion);
    iscRegValue = CSL_REG32_RD(iscRegionOffset + iscReg);
    return iscRegValue;
}

#if defined(SOC_J784S4) || defined(SOC_J721S2 )|| defined(SOC_J742S2)
static uint32_t SafetyCheckers_tifsGetIscCcRegValue(uint32_t iscIdOffset, uint32_t iscRegion, uint32_t iscReg)
{
    uint32_t iscBlkBaseAddr = 0U, iscBlkOffset = 0U, iscRegionOffset = 0U, iscRegValue = 0U;
    iscBlkBaseAddr = SAFETY_CHECKERS_TIFS_CC_ISC_BASE;
    iscBlkOffset = iscBlkBaseAddr + iscIdOffset;
    iscRegionOffset = iscBlkOffset + (0x4U * iscRegion);
    iscRegValue = CSL_REG32_RD(iscRegionOffset + iscReg);
    return iscRegValue;
}
#endif

#if defined(SOC_J7200) || defined(SOC_J721E ) || defined(SOC_J721S2) || defined(SOC_J784S4) || defined(SOC_J742S2)
static uint32_t SafetyCheckers_tifsGetIscRaRegValue(uint32_t iscId, uint32_t iscRegion, uint32_t iscReg)
{
    uint32_t iscBlkBaseAddr = 0U, iscBlkOffset = 0U, iscRegionOffset = 0U, iscRegValue = 0U;
    iscBlkBaseAddr = SAFETY_CHECKERS_TIFS_ISC_BASE;
    iscBlkOffset = iscBlkBaseAddr + (0x400U * iscId);
    iscRegionOffset = iscBlkOffset + (0x4U * iscRegion);
    iscRegValue = CSL_REG32_RD(iscRegionOffset + iscReg);
    return iscRegValue;
}
#endif
