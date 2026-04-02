/*
 * @file  csl_ecc_aggr.c
 *
 * @brief
 *  C implementation file for the ECC Aggregator module CSL.
 *
 *  \par
 *  ============================================================================
 *  @n   (C) Copyright 2015-2020, Texas Instruments, Inc.
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
#include <ti/csl/csl_ecc_aggr.h>

#define     CSL_TWO_POWER_TWO_VALUE          (4U)

/*===========================================================================*/
/*  Internal Functions                                                       */
/*===========================================================================*/
static bool CSL_ecc_aggrIsSVBUSRegReadDone(const CSL_ecc_aggrRegs *pEccAggrRegs);
static bool CSL_ecc_aggrIsValidRamId(const CSL_ecc_aggrRegs *pEccAggrRegs,
                                     uint32_t ramId);
static bool CSL_ecc_aggrIsValidEccRamRegOffset(uint32_t regOffset);
static bool CSL_ecc_aggrIsValidEDCInterconnectRegOffset(uint32_t regOffset);
static bool CSL_ecc_aggrIsValidIntrSrc(const CSL_Ecc_AggrIntrSrc intrSrc);
static bool CSL_ecc_aggrIsValidInstSel(uint32_t instSelect, uint32_t maxInst);
static bool CSL_ecc_aggrToggleEccRamIntrPending(CSL_ecc_aggrRegs *pEccAggrRegs,
                                                uint32_t ramId,
                                                CSL_Ecc_AggrIntrSrc intrSrc,
                                                uint32_t numEvents, bool bSet);
static bool CSL_ecc_aggrToggleEDCInterconnectIntrPending(
    CSL_ecc_aggrRegs *pEccAggrRegs, uint32_t ramId, CSL_Ecc_AggrIntrSrc intrSrc,
    CSL_Ecc_AggrEDCErrorSubType subType, uint32_t numEvents, bool bSet);
static bool CSL_ecc_aggrToggleIntrEnable(const CSL_ecc_aggrRegs *pEccAggrRegs,
    uint32_t ramId, CSL_Ecc_AggrIntrSrc intrSrc, bool bEnable);
static bool CSL_ecc_aggrToggleIntrsEnable(const CSL_ecc_aggrRegs *pEccAggrRegs,
    CSL_Ecc_AggrIntrSrc intrSrc, bool bEnable);
static uintptr_t CSL_ecc_aggrStatusRegAddr(uintptr_t base, uint32_t n, uint32_t et);
static uintptr_t CSL_ecc_aggrSecStatusRegAddr(uintptr_t base, uint32_t n);
static uintptr_t CSL_ecc_aggrDedStatusRegAddr(uintptr_t base, uint32_t n);
static uintptr_t CSL_ecc_aggrDedEnableSetRegAddr(uintptr_t base, uint32_t n);
static uintptr_t CSL_ecc_aggrSecEnableSetRegAddr(uintptr_t base, uint32_t n);
static uintptr_t CSL_ecc_aggrDedEnableClrRegAddr(uintptr_t base, uint32_t n);
static uintptr_t CSL_ecc_aggrSecEnableClrRegAddr(uintptr_t base, uint32_t n);
static uintptr_t CSL_ecc_aggrEnableSetRegAddr(uintptr_t base, uint32_t n, uint32_t et);
static uintptr_t CSL_ecc_aggrEnableClrRegAddr(uintptr_t base, uint32_t n, uint32_t et);
static void CSL_ecc_aggrReadSVBUSReg(CSL_ecc_aggrRegs *pEccAggrRegs,
    uint32_t ramId, uint32_t regOffset, uint32_t *pRegVal );
static void CSL_ecc_aggrWriteSVBUSReg(CSL_ecc_aggrRegs *pEccAggrRegs,
    uint32_t ramId, uint32_t regOffset, uint32_t val );


static uintptr_t CSL_ecc_aggrDedEnableClrRegAddr(uintptr_t base, uint32_t n) 
{
    return ((base)+CSL_ECC_AGGR_DED_ENABLE_CLR_REG0+ (uint32_t)((n)*4U));
}

static uintptr_t CSL_ecc_aggrSecEnableClrRegAddr(uintptr_t base, uint32_t n) 
{ 
    return ((base)+CSL_ECC_AGGR_SEC_ENABLE_CLR_REG0+ (uint32_t)((n)*4U));
}

static uintptr_t CSL_ecc_aggrDedEnableSetRegAddr(uintptr_t base, uint32_t n) 
{
    return ((base)+CSL_ECC_AGGR_DED_ENABLE_SET_REG0+ (uint32_t)((n)*4U));
}

static uintptr_t CSL_ecc_aggrSecEnableSetRegAddr(uintptr_t base, uint32_t n) 
{ 
    return ((base)+CSL_ECC_AGGR_SEC_ENABLE_SET_REG0+ (uint32_t)((n)*4U));
}

static uintptr_t CSL_ecc_aggrEnableSetRegAddr(uintptr_t base, uint32_t n, uint32_t et) 
{
    uintptr_t addr;

    if (et == CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT)
    {
        addr = CSL_ecc_aggrSecEnableSetRegAddr(base,n);
    }
    else
    {
        addr = CSL_ecc_aggrDedEnableSetRegAddr(base,n);
    }
    return (addr);
}

static uintptr_t CSL_ecc_aggrEnableClrRegAddr(uintptr_t base, uint32_t n, uint32_t et)
{
    uintptr_t addr;

    if (et == CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT)
    {
        addr = CSL_ecc_aggrSecEnableClrRegAddr(base,n);
    }
    else
    {
        addr = CSL_ecc_aggrDedEnableClrRegAddr(base,n);
    }
    return (addr);
}

static uintptr_t  CSL_ecc_aggrSecStatusRegAddr(uintptr_t base, uint32_t n)
{
    return ((base)+CSL_ECC_AGGR_SEC_STATUS_REG0+(uint32_t)((n)*4U));
}

static uintptr_t  CSL_ecc_aggrDedStatusRegAddr(uintptr_t base, uint32_t n)
{
    return ((base)+CSL_ECC_AGGR_DED_STATUS_REG0+(uint32_t)((n)*4U));
}

static uintptr_t CSL_ecc_aggrStatusRegAddr(uintptr_t base, uint32_t n, uint32_t et) 
{
     return (((et)==CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT)? \
                    CSL_ecc_aggrSecStatusRegAddr(base,n): \
                    CSL_ecc_aggrDedStatusRegAddr(base,n));
}

static bool CSL_ecc_aggrIsSVBUSRegReadDone(const CSL_ecc_aggrRegs *pEccAggrRegs)
{
    bool retVal = (bool)false;

    if( CSL_REG32_FEXT(&pEccAggrRegs->VECTOR, ECC_AGGR_VECTOR_RD_SVBUS_DONE) == 1U )
    {
        retVal = (bool)true;
    }
    return retVal;
}

static bool CSL_ecc_aggrIsValidRamId(const CSL_ecc_aggrRegs *pEccAggrRegs, uint32_t ramId)
{
    bool       retVal = (bool)false;
    uint32_t   numRams;
    int32_t    cslRet;

    /* read number of Rams*/
    cslRet = CSL_ecc_aggrGetNumRams(pEccAggrRegs, &numRams);

    if (cslRet == CSL_PASS)
    {
        if( ramId < numRams )
        {
            retVal = (bool)true;
        }
    }
    return retVal;
}

static bool CSL_ecc_aggrIsValidEccRamRegOffset(uint32_t regOffset)
{
    bool retVal = (bool)true;

    /* Check bounds of register offset for ECC wrapper */
    if( (regOffset < CSL_ECC_RAM_WRAP_REV) ||
        (regOffset > CSL_ECC_RAM_ERR_STAT3) )
    {
        retVal = (bool)false;
    }
    return retVal;
}

static bool CSL_ecc_aggrIsValidEDCInterconnectRegOffset(uint32_t regOffset)
{
    bool retVal = (bool)true;

    /* Check bounds of register offset for EDC Interconnect*/
    if( (regOffset < CSL_EDC_CTL_REVISION) ||
        (regOffset > CSL_EDC_CTL_ERR_STATUS2) )
    {
        retVal = (bool)false;
    }
    return retVal;
}

static bool CSL_ecc_aggrIsValidIntrSrc(const CSL_Ecc_AggrIntrSrc intrSrc)
{
    bool retVal = (bool)true;

    if( (intrSrc < CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT)      ||
        (intrSrc > CSL_ECC_AGGR_INTR_SRC_DOUBLE_BIT) )
    {
        retVal = (bool)false;
    }
    return retVal;
}

static bool CSL_ecc_aggrIsValidInstSel(uint32_t instSelect, uint32_t maxInst)
{
    bool retVal = (bool)true;

    if( instSelect >= maxInst )
    {
        retVal = (bool)false;
    }
    return retVal;
}

static bool CSL_ecc_aggrToggleEccRamIntrPending(CSL_ecc_aggrRegs *pEccAggrRegs,
    uint32_t ramId, CSL_Ecc_AggrIntrSrc intrSrc, uint32_t numEvents, bool bSet)
{
    bool retVal = (bool)false;

    if( CSL_ecc_aggrIsValidRamId(pEccAggrRegs, ramId) == (bool)true )
    {
        uint32_t regVal;

        retVal = (bool)true;

        /* Set the appropriate bits to set or unset selected event bits */
        if( intrSrc == CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT )
        {
            if( bSet == (bool)true )
            {
                regVal = CSL_FMK(ECC_RAM_ERR_STAT1_ECC_SEC, (uint32_t)numEvents);
            }
            else
            {
                regVal = CSL_FMK(ECC_RAM_ERR_STAT1_CLR_ECC_SEC, (uint32_t)numEvents);
            }
        }
        else if( intrSrc == CSL_ECC_AGGR_INTR_SRC_DOUBLE_BIT )
        {
            if( bSet == (bool)true )
            {
                regVal = CSL_FMK(ECC_RAM_ERR_STAT1_ECC_DED, (uint32_t)numEvents);
            }
            else
            {
                regVal = CSL_FMK(ECC_RAM_ERR_STAT1_CLR_ECC_DED, (uint32_t)numEvents);
            }
        }
        else if( intrSrc == CSL_ECC_ADDR_ERROR_TYPE_SUCCESSIVE_SINGLE_BITS )
        {
            if( bSet == (bool)true )
            {
                regVal = CSL_FMK(ECC_RAM_ERR_STAT1_ECC_OTHER, (uint32_t)numEvents);
            }
            else
            {
                regVal = CSL_FMK(ECC_RAM_ERR_STAT1_CLR_ECC_OTHER, (uint32_t)numEvents);
            }
        }
        else
        {
            retVal = (bool)false;
        }
        if( retVal == (bool)true )
        {
            /* Write the bitmap to the Status register */
            CSL_ecc_aggrWriteSVBUSReg(pEccAggrRegs, ramId, CSL_ECC_RAM_ERR_STAT1, regVal);

            /* Readback to make sure write is complete */
            CSL_ecc_aggrReadSVBUSReg(pEccAggrRegs, ramId, CSL_ECC_RAM_ERR_STAT1, &regVal);
        }
    }

    return retVal;
}

static bool CSL_ecc_aggrToggleEDCInterconnectIntrPending(CSL_ecc_aggrRegs *pEccAggrRegs,
    uint32_t ramId, CSL_Ecc_AggrIntrSrc intrSrc,
    CSL_Ecc_AggrEDCErrorSubType subType, uint32_t numEvents, bool bSet)
{
    bool retVal = (bool)false;

    /* Check for valid RAM Id */
    if( CSL_ecc_aggrIsValidRamId(pEccAggrRegs, ramId) == (bool)true )
    {
        uint32_t regVal;

        retVal = (bool)true;

        /* Set the appropriate bits to set or unset selected event bits */
        if( intrSrc == CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT )
        {
            if( bSet == (bool)true )
            {
                if (subType == CSL_ECC_AGGR_ERROR_SUBTYPE_INJECT)
                {
                    regVal = CSL_FMK(EDC_CTL_ERR_STATUS1_INJ_COR_PEND, (uint32_t)numEvents);
                }
                else 
                {
                    regVal = CSL_FMK(EDC_CTL_ERR_STATUS1_COR_PEND, (uint32_t)numEvents);
                }
            }
            else
            {
                if (subType == CSL_ECC_AGGR_ERROR_SUBTYPE_INJECT)
                {
                    regVal = CSL_FMK(EDC_CTL_ERR_STATUS1_INJ_COR_PEND_CLR, (uint32_t)numEvents);
                }
                else 
                {
                    regVal = CSL_FMK(EDC_CTL_ERR_STATUS1_COR_PEND_CLR, (uint32_t)numEvents);
                }
            }
        }
        else if( intrSrc == CSL_ECC_AGGR_INTR_SRC_DOUBLE_BIT )
        {
            if( bSet == (bool)true )
            {
                if (subType == CSL_ECC_AGGR_ERROR_SUBTYPE_INJECT)
                {
                    regVal = CSL_FMK(EDC_CTL_ERR_STATUS1_INJ_UNC_PEND, (uint32_t)numEvents);
                }
                else 
                {
                    regVal = CSL_FMK(EDC_CTL_ERR_STATUS1_UNC_PEND, (uint32_t)numEvents);
                }
            }
            else
            {
                if (subType == CSL_ECC_AGGR_ERROR_SUBTYPE_INJECT)
                {
                    regVal = CSL_FMK(EDC_CTL_ERR_STATUS1_INJ_UNC_PEND_CLR, (uint32_t)numEvents);
                }
                else 
                {
                    regVal = CSL_FMK(EDC_CTL_ERR_STATUS1_UNC_PEND_CLR, (uint32_t)numEvents);
                }
            }
        }
        else
        {
            retVal = (bool)false;
        }

        if( retVal == (bool)true )
        {
            /* Write the bitmap to the Status register */
            CSL_ecc_aggrWriteSVBUSReg(pEccAggrRegs, ramId, CSL_EDC_CTL_ERR_STATUS1, regVal);
            /* Readback to make sure write is complete */
            CSL_ecc_aggrReadSVBUSReg(pEccAggrRegs, ramId, CSL_EDC_CTL_ERR_STATUS1, &regVal);
        }
    }

    return retVal;
}

static bool CSL_ecc_aggrToggleIntrEnable(const CSL_ecc_aggrRegs *pEccAggrRegs,
    uint32_t ramId, CSL_Ecc_AggrIntrSrc intrSrc, bool bEnable)
{
    bool retVal = (bool)true;

    if( (CSL_ecc_aggrIsValidRamId(pEccAggrRegs, ramId) == (bool) true ) && 
        (CSL_ecc_aggrIsValidIntrSrc(intrSrc)           == (bool) true ) )
    {
        if(bEnable == (bool)true )
        {
            CSL_REG32_WR( CSL_ecc_aggrEnableSetRegAddr( (uintptr_t)pEccAggrRegs,(ramId >> 5U), intrSrc ),
                          ((uint32_t)1U << (ramId & 0x1FU)) );
        }
        else
        {
            CSL_REG32_WR( CSL_ecc_aggrEnableClrRegAddr( (uintptr_t)pEccAggrRegs,(ramId >> 5U), intrSrc ),
                          ((uint32_t)1U << (ramId & 0x1FU)) );
        }
    }
    else
    {
        retVal = (bool)false;
    }
    return retVal;
}

static bool CSL_ecc_aggrToggleIntrsEnable(const CSL_ecc_aggrRegs *pEccAggrRegs,
    CSL_Ecc_AggrIntrSrc intrSrc, bool bEnable)
{
    bool retVal = (bool)true;
    int32_t      cslRet = CSL_EBADARGS;

    if( CSL_ecc_aggrIsValidIntrSrc(intrSrc) == (bool)true )
    {
        uint32_t ramId;
        uint32_t numRams;

        cslRet = CSL_ecc_aggrGetNumRams(pEccAggrRegs, &numRams);
        if (cslRet == CSL_PASS)
        {
            for( ramId=((uint32_t)(0u)); ramId<numRams; ramId++ )
            {
                if( bEnable == (bool)true)
                {
                    cslRet = CSL_ecc_aggrEnableIntr( pEccAggrRegs, ramId, intrSrc );
                }
                else
                {
                    cslRet = CSL_ecc_aggrDisableIntr( pEccAggrRegs, ramId, intrSrc );
                }
            }
        }
    }

    if (cslRet == CSL_PASS)
    {
        retVal = (bool) true;
    }
    else
    {
        retVal = (bool) false;
    }
    return retVal;
}

/*===========================================================================*/
/*  External CSL-FL Functions                                                */
/*===========================================================================*/

/**
 * Requirement: REQ_TAG(PDK-6030)
 * Design: did_csl_ecc_aggr_read
 */
int32_t CSL_ecc_aggrGetRevision(const CSL_ecc_aggrRegs *pEccAggrRegs, uint32_t *pRev)
{
    int32_t    retVal = CSL_EBADARGS;

    if ( pEccAggrRegs != NULL_PTR )
    {
        if (pRev  != NULL_PTR)
        {
            *pRev = CSL_REG32_RD(&pEccAggrRegs->REV);
            retVal = CSL_PASS;
        }
    }

    /* Return the API success/fail with value in the address provided by caller */
    return (retVal);
}

/**
 * Requirement: REQ_TAG(PDK-6030)
 * Design: did_csl_ecc_aggr_read
 */
int32_t CSL_ecc_aggrGetNumRams(const CSL_ecc_aggrRegs *pEccAggrRegs, uint32_t *pNumRams)
{
    int32_t    retVal = CSL_EBADARGS;

    if ( pEccAggrRegs != NULL_PTR )
    {
        if (pNumRams  != NULL_PTR)
        {
            *pNumRams = (uint32_t)CSL_REG32_FEXT(&pEccAggrRegs->STAT, ECC_AGGR_STAT_NUM_RAMS);
             retVal   = CSL_PASS;
        }
    }
    /* Return the API success/fail with value in the address provided by caller */
    return (retVal);
}

/**
 * Requirement: REQ_TAG(PDK-6030)
 * Design: did_csl_ecc_aggr_read
 */
int32_t CSL_ecc_aggrReadEccRamReg(CSL_ecc_aggrRegs *pEccAggrRegs,
    uint32_t ramId, uint32_t regOffset, uint32_t *pRegVal )
{
    int32_t    retVal = CSL_EBADARGS;

    if( (pEccAggrRegs                                   != NULL_PTR) &&
        (CSL_ecc_aggrIsValidEccRamRegOffset(regOffset)  == (bool) true )              &&
        (CSL_ecc_aggrIsValidRamId(pEccAggrRegs, ramId)  == (bool) true) )
    {
        if (pRegVal  != NULL_PTR)
        {
           CSL_ecc_aggrReadSVBUSReg(pEccAggrRegs, ramId, regOffset, pRegVal);
           retVal = CSL_PASS;
        }
    }
    return retVal;
}

/**
 * Requirement: REQ_TAG(PDK-6030)
 * Design: did_csl_ecc_aggr_read
 */
int32_t CSL_ecc_aggrReadEccRamWrapRevReg(CSL_ecc_aggrRegs *pEccAggrRegs,
    uint32_t ramId, uint32_t *pRegVal)
{
    return CSL_ecc_aggrReadEccRamReg(pEccAggrRegs, ramId, CSL_ECC_RAM_WRAP_REV, pRegVal);
}

/**
 * Requirement: REQ_TAG(PDK-6030) REQ_TAG(PDK-5893)
 * Design: did_csl_ecc_aggr_read
 */
int32_t CSL_ecc_aggrReadEccRamCtrlReg(CSL_ecc_aggrRegs *pEccAggrRegs,
    uint32_t ramId, uint32_t *pRegVal)
{
    return CSL_ecc_aggrReadEccRamReg(pEccAggrRegs, ramId, CSL_ECC_RAM_CTRL, pRegVal);
}

/**
 * Requirement: REQ_TAG(PDK-6030)
 * Design: (did_csl_ecc_aggr_read)
 */
int32_t CSL_ecc_aggrReadEccRamErrCtrlReg(CSL_ecc_aggrRegs *pEccAggrRegs,
    uint32_t ramId, uint32_t instSelect, uint32_t *pRegVal)
{
    int32_t    retVal = CSL_EBADARGS;

    if( CSL_ecc_aggrIsValidInstSel(instSelect, CSL_ECC_AGGR_MAX_NUM_RAM_ERR_CTRL)== (bool)true )
    {
        retVal = CSL_ecc_aggrReadEccRamReg(pEccAggrRegs, ramId,
                     CSL_ECC_RAM_ERR_CTRL1+((instSelect)*4U), pRegVal);
    }
    return retVal;
}

/**
 * Requirement: REQ_TAG(PDK-6030)
 * Design: did_csl_ecc_aggr_read
 */
int32_t CSL_ecc_aggrReadEccRamErrStatReg(CSL_ecc_aggrRegs *pEccAggrRegs,
    uint32_t ramId, uint32_t instSelect, uint32_t *pRegVal)
{
    int32_t    retVal = CSL_EBADARGS;

    if( CSL_ecc_aggrIsValidInstSel(instSelect, CSL_ECC_AGGR_MAX_NUM_RAM_ERR_STAT)==(bool)true )
    {
        retVal = CSL_ecc_aggrReadEccRamReg(pEccAggrRegs, ramId,
                     CSL_ECC_RAM_ERR_STAT1+((instSelect)*4U), pRegVal);
    }
    return retVal;
}

/**
 * Requirement: REQ_TAG(PDK-6040)
 * Design: did_csl_ecc_aggr_write
 */
int32_t CSL_ecc_aggrWriteEccRamReg(CSL_ecc_aggrRegs *pEccAggrRegs,
    uint32_t ramId, uint32_t regOffset, uint32_t val )
{
    int32_t    retVal = CSL_EBADARGS;

    if( (pEccAggrRegs                                   != NULL_PTR) &&
        (CSL_ecc_aggrIsValidEccRamRegOffset(regOffset)  == (bool) true ) &&
        (CSL_ecc_aggrIsValidRamId(pEccAggrRegs, ramId)  == (bool) true) )
    {
        CSL_ecc_aggrWriteSVBUSReg(pEccAggrRegs, ramId, regOffset, val);
        retVal = CSL_PASS;
    }
    return retVal;
}

/**
 * Requirement: REQ_TAG(PDK-6040)
 * Design: did_csl_ecc_aggr_write
 */
int32_t CSL_ecc_aggrWriteEccRamCtrlReg(CSL_ecc_aggrRegs *pEccAggrRegs,
    uint32_t ramId, uint32_t val)
{
    return CSL_ecc_aggrWriteEccRamReg(pEccAggrRegs, ramId, CSL_ECC_RAM_CTRL, val);
}

/**
 * Requirement: REQ_TAG(PDK-6040)
 * Design: did_csl_ecc_aggr_write
 */
int32_t CSL_ecc_aggrWriteEccRamErrCtrlReg(CSL_ecc_aggrRegs *pEccAggrRegs,
    uint32_t ramId, uint32_t instSelect, uint32_t val)
{
    int32_t    retVal = CSL_EBADARGS;

    if( CSL_ecc_aggrIsValidInstSel(instSelect, CSL_ECC_AGGR_MAX_NUM_RAM_ERR_CTRL) ==(bool)true )
    {
        retVal = CSL_ecc_aggrWriteEccRamReg(pEccAggrRegs, ramId,
                     CSL_ECC_RAM_ERR_CTRL1+((instSelect)*4U), val);
    }
    return retVal;
}

/**
 * Requirement: REQ_TAG(PDK-6040)
 * Design: did_csl_ecc_aggr_write
 */
int32_t CSL_ecc_aggrWriteEccRamErrStatReg(CSL_ecc_aggrRegs *pEccAggrRegs,
    uint32_t ramId, uint32_t instSelect, uint32_t val)
{
    int32_t    retVal = CSL_EBADARGS;

    if( CSL_ecc_aggrIsValidInstSel(instSelect, CSL_ECC_AGGR_MAX_NUM_RAM_ERR_STAT) == (bool)true )
    {
        retVal = CSL_ecc_aggrWriteEccRamReg(pEccAggrRegs, ramId,
                     CSL_ECC_RAM_ERR_STAT1+((instSelect)*4U), val);
    }
    return retVal;
}

/**
 * Requirement: REQ_TAG(PDK-6040) REQ_TAG(PDK-6058)
 * Design: did_csl_ecc_aggr_config
 */
int32_t CSL_ecc_aggrConfigEccRam(CSL_ecc_aggrRegs *pEccAggrRegs,
    uint32_t ramId, bool bEnable, bool bEccCheck, bool bEnableRMW)
{
    int32_t    retVal = CSL_EBADARGS;

    if ( (pEccAggrRegs                                  != NULL_PTR)  &&
         (CSL_ecc_aggrIsValidRamId(pEccAggrRegs, ramId) == (bool)true) )
    {
        uint32_t val;

        retVal = CSL_ecc_aggrReadEccRamCtrlReg(pEccAggrRegs, ramId, &val);
        if (retVal == CSL_PASS)
        {
            CSL_FINS(val, ECC_RAM_CTRL_ENABLE_RMW, (bEnableRMW ? (uint32_t)1U : (uint32_t)0) );
            CSL_FINS(val, ECC_RAM_CTRL_ECC_CHECK, (bEccCheck ? (uint32_t)1U : (uint32_t)0) );
            CSL_FINS(val, ECC_RAM_CTRL_ECC_ENABLE,(bEnable ? (uint32_t)1U : (uint32_t)0) );
            retVal = CSL_ecc_aggrWriteEccRamCtrlReg(pEccAggrRegs, ramId, val);
        }
    }
    return retVal;
}

/**
 * Requirement: REQ_TAG(PDK-6040) REQ_TAG(PDK-6058)
 * Design: did_csl_ecc_aggr_config
 */
int32_t CSL_ecc_aggrVerifyConfigEccRam(CSL_ecc_aggrRegs *pEccAggrRegs,
    uint32_t ramId, bool bEnable, bool bEccCheck, bool bEnableRMW)
{
    int32_t    retVal = CSL_EBADARGS;

    if ( (pEccAggrRegs != NULL_PTR)       &&
         (CSL_ecc_aggrIsValidRamId(pEccAggrRegs, ramId) == (bool)true) )
    {
        uint32_t val, valExp = 0U;
        uint32_t mask = (CSL_ECC_RAM_CTRL_ECC_ENABLE_MASK |
                         CSL_ECC_RAM_CTRL_ECC_CHECK_MASK  |
                         CSL_ECC_RAM_CTRL_ENABLE_RMW_MASK);

        retVal = CSL_ecc_aggrReadEccRamCtrlReg(pEccAggrRegs, ramId, &val);
        if (retVal == CSL_PASS)
        {
            CSL_FINS(valExp, ECC_RAM_CTRL_ENABLE_RMW, (bEnableRMW ? (uint32_t)1U : (uint32_t)0) );
            CSL_FINS(valExp, ECC_RAM_CTRL_ECC_CHECK, (bEccCheck ? (uint32_t)1U : (uint32_t)0) );
            CSL_FINS(valExp, ECC_RAM_CTRL_ECC_ENABLE,(bEnable ? (uint32_t)1U : (uint32_t)0) );
            /* Get the bit fields for the expected values */
            valExp &= mask;
            val    &= mask;
            if (val == valExp)
            {
                retVal = CSL_PASS;
            }
            else
            {
                retVal = CSL_EFAIL;
            }
        }
    }
    return retVal;
}

/**
 * Requirement: REQ_TAG(PDK-6030)
 * Design: did_csl_ecc_aggr_read
 */
int32_t CSL_ecc_aggrGetEccRamErrorStatus(CSL_ecc_aggrRegs *pEccAggrRegs,
    uint32_t ramId, CSL_Ecc_AggrEccRamErrorStatusInfo *pEccErrorStatus)
{
    int32_t    retVal = CSL_EBADARGS;

    if ( (pEccAggrRegs != NULL_PTR)       &&
         (pEccErrorStatus != NULL_PTR)       &&
         ( CSL_ecc_aggrIsValidRamId(pEccAggrRegs, ramId) == (bool)true ) )
    {
        uint32_t  errorStatus1, errorStatus2, errorStatus3;

        /* Read error status register 1 */
        CSL_ecc_aggrReadSVBUSReg(pEccAggrRegs, ramId, CSL_ECC_RAM_ERR_STAT1, &errorStatus1);

        /* Read error status register 2 */
        CSL_ecc_aggrReadSVBUSReg(pEccAggrRegs, ramId, CSL_ECC_RAM_ERR_STAT2, &errorStatus2);

        /* Read error status register 3 */
        CSL_ecc_aggrReadSVBUSReg(pEccAggrRegs, ramId, CSL_ECC_RAM_ERR_STAT3, &errorStatus3);

        /* Populate the status structure with the appropriate bits */
        pEccErrorStatus->controlRegErr    =  ((CSL_FEXT(errorStatus1, ECC_RAM_ERR_STAT1_CTR_REG_ERR) != 0U) ? TRUE : FALSE);
        pEccErrorStatus->successiveSingleBitErr =  ((CSL_FEXT(errorStatus1, ECC_RAM_ERR_STAT1_CLR_ECC_OTHER) != 0U) ? TRUE : FALSE);
        pEccErrorStatus->parityErrorCount = ((uint8_t)CSL_FEXT(errorStatus1, ECC_RAM_ERR_STAT1_PARITY_ERR));
        pEccErrorStatus->singleBitErrorCount = ((uint8_t)CSL_FEXT(errorStatus1, ECC_RAM_ERR_STAT1_ECC_SEC));
        pEccErrorStatus->doubleBitErrorCount = ((uint8_t)CSL_FEXT(errorStatus1, ECC_RAM_ERR_STAT1_ECC_DED));
        pEccErrorStatus->eccRow    =  CSL_FEXT(errorStatus2, ECC_RAM_ERR_STAT2_ECC_ROW);
        pEccErrorStatus->eccBit1   = CSL_FEXT(errorStatus1, ECC_RAM_ERR_STAT1_ECC_BIT1);
        pEccErrorStatus->sVBUSTimeoutErr = ((CSL_FEXT(errorStatus3, ECC_RAM_ERR_STAT3_SVBUS_TIMEOUT_ERR) != 0U) ? TRUE : FALSE);
        pEccErrorStatus->writebackPend   = ((CSL_FEXT(errorStatus3, ECC_RAM_ERR_STAT3_WB_PEND) != 0U) ? TRUE : FALSE);
        retVal = CSL_PASS;
    }
    return retVal;
}

/**
 * Requirement: REQ_TAG(PDK-6040)
 * Design: did_csl_ecc_aggr_write
 */
int32_t CSL_ecc_aggrForceEccRamError(CSL_ecc_aggrRegs *pEccAggrRegs,
    uint32_t ramId, const CSL_Ecc_AggrErrorInfo *pEccForceError)
{
    int32_t    retVal = CSL_EBADARGS;

    if ( (pEccAggrRegs != NULL_PTR)       &&
         (pEccForceError != NULL_PTR)       &&
         ( CSL_ecc_aggrIsValidRamId(pEccAggrRegs, ramId) == (bool)true ) )
    {
        uint32_t regVal;

        /* Set first the bits to insert error */
        CSL_ecc_aggrWriteSVBUSReg(pEccAggrRegs, ramId, CSL_ECC_RAM_ERR_CTRL2,
            CSL_FMK(ECC_RAM_ERR_CTRL2_ECC_BIT1, pEccForceError->eccBit1)     |
            CSL_FMK(ECC_RAM_ERR_CTRL2_ECC_BIT2, pEccForceError->eccBit2) );

        /* Configure the row to insert error */
        CSL_ecc_aggrWriteSVBUSReg(pEccAggrRegs, ramId, CSL_ECC_RAM_ERR_CTRL1,
            CSL_FMK(ECC_RAM_ERR_CTRL1_ECC_ROW, pEccForceError->eccRow) );

        /* Read content of Control register */
        CSL_ecc_aggrReadSVBUSReg(pEccAggrRegs, ramId, CSL_ECC_RAM_CTRL, &regVal);

        /* Update the register value with the required error force settings */
        if(pEccForceError->intrSrc == CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT)
        {
            CSL_FINS(regVal, ECC_RAM_CTRL_FORCE_SEC, (uint32_t)1U);
            CSL_FINS(regVal, ECC_RAM_CTRL_FORCE_DED, (uint32_t)0);
        }
        else if(pEccForceError->intrSrc == CSL_ECC_AGGR_INTR_SRC_DOUBLE_BIT)
        {
            CSL_FINS(regVal, ECC_RAM_CTRL_FORCE_SEC, (uint32_t)0);
            CSL_FINS(regVal, ECC_RAM_CTRL_FORCE_DED, (uint32_t)1U);
        }
        else
        {
            CSL_FINS(regVal, ECC_RAM_CTRL_FORCE_SEC, (uint32_t)0);
            CSL_FINS(regVal, ECC_RAM_CTRL_FORCE_DED, (uint32_t)0);
        }
        CSL_FINS(regVal, ECC_RAM_CTRL_FORCE_N_ROW,
            ((pEccForceError->bNextRow == TRUE) ? (uint32_t)1U : (uint32_t)0) );
        CSL_FINS(regVal, ECC_RAM_CTRL_ERROR_ONCE, 
            ((pEccForceError->bOneShotMode == TRUE) ? (uint32_t)1U : (uint32_t)0) );

        /* Write back the control register to inject the error */
        CSL_ecc_aggrWriteSVBUSReg(pEccAggrRegs, ramId, CSL_ECC_RAM_CTRL, regVal);
        retVal = CSL_PASS;
    }
    return retVal;
}

static void CSL_ecc_aggrReadSVBUSReg(CSL_ecc_aggrRegs *pEccAggrRegs,
    uint32_t ramId, uint32_t regOffset, uint32_t *pRegVal )
{

    /* Write to vector register the RAM ID and offset to read */
    CSL_REG32_WR( &pEccAggrRegs->VECTOR,
        CSL_FMK(ECC_AGGR_VECTOR_ECC_VECTOR, ramId)              |
        CSL_FMK(ECC_AGGR_VECTOR_RD_SVBUS_ADDRESS, regOffset)    |
        CSL_FMK(ECC_AGGR_VECTOR_RD_SVBUS, (uint32_t)1U) );

    /* Wait till read operation is complete */
    while( !CSL_ecc_aggrIsSVBUSRegReadDone(pEccAggrRegs) ) { }

    /* Now read the read value */
    *pRegVal = CSL_REG32_RD(((uintptr_t)pEccAggrRegs)+regOffset);
}

static void CSL_ecc_aggrWriteSVBUSReg(CSL_ecc_aggrRegs *pEccAggrRegs,
    uint32_t ramId, uint32_t regOffset, uint32_t val )
{
    /* Write to vector register the RAM Id register to map */
    CSL_REG32_WR(&pEccAggrRegs->VECTOR, CSL_FMK(ECC_AGGR_VECTOR_ECC_VECTOR, ramId));

    /* Write the value to the register */
    CSL_REG32_WR(((uintptr_t)pEccAggrRegs)+regOffset, val);
}

/**
 * Requirement: REQ_TAG(PDK-6030) REQ_TAG(PDK-5886)
 * Design: did_csl_ecc_aggr_read
 */
int32_t CSL_ecc_aggrReadEDCInterconnectReg(CSL_ecc_aggrRegs *pEccAggrRegs,
    uint32_t ramId, uint32_t regOffset, uint32_t *pRegVal )
{
    int32_t    retVal = CSL_EBADARGS;

    /* Do parameter checks */
    if( (pEccAggrRegs                                   != NULL_PTR) &&
        (CSL_ecc_aggrIsValidEDCInterconnectRegOffset(regOffset)  == (bool) true ) &&
        (CSL_ecc_aggrIsValidRamId(pEccAggrRegs, ramId)  == (bool) true) )
    {
        if (pRegVal  != NULL_PTR)
        {
            /* Call routine to read wrapper register */
            CSL_ecc_aggrReadSVBUSReg(pEccAggrRegs, ramId, regOffset, pRegVal);
            retVal = CSL_PASS;
        }
    }
    return retVal;
}

/**
 * Requirement: REQ_TAG(PDK-6040) REQ_TAG(PDK-5893)
 * Design: did_csl_ecc_aggr_write
 */
int32_t CSL_ecc_aggrWriteEDCInterconnectReg(CSL_ecc_aggrRegs *pEccAggrRegs,
    uint32_t ramId, uint32_t regOffset, uint32_t val )
{
    int32_t    retVal = CSL_EBADARGS;

    /* Do parameter checks */
    if( (pEccAggrRegs                                   != NULL_PTR) &&
        (CSL_ecc_aggrIsValidEDCInterconnectRegOffset(regOffset)  == (bool) true ) &&
        (CSL_ecc_aggrIsValidRamId(pEccAggrRegs, ramId)  == (bool) true) )
    {
        /* Call routine to write wrapper register */
        CSL_ecc_aggrWriteSVBUSReg(pEccAggrRegs, ramId, regOffset, val);
        retVal = CSL_PASS;
    }
    return retVal;
}
/**
 * Requirement: REQ_TAG(PDK-6040) REQ_TAG(PDK-6058) REQ_TAG(PDK-5893)
 * Design: did_csl_ecc_aggr_config
 */
int32_t CSL_ecc_aggrConfigEDCInterconnect(CSL_ecc_aggrRegs *pEccAggrRegs,
    uint32_t ramId, bool bEccCheck)
{
    int32_t    retVal = CSL_EBADARGS;

    /* Do parameter checks */
    if ( (pEccAggrRegs                                  != NULL_PTR)  &&
         (CSL_ecc_aggrIsValidRamId(pEccAggrRegs, ramId) == (bool)true) )
    {
        uint32_t val;

        /* Read the EDC control register */
        CSL_ecc_aggrReadSVBUSReg(pEccAggrRegs, ramId, CSL_EDC_CTL_CONTROL, &val);

        /* Update fields to configure */
        CSL_FINS(val, EDC_CTL_CONTROL_ECC_CHECK, (bEccCheck ? (uint32_t)1U : (uint32_t)0) );

        /* Write back to EDC control register */
        CSL_ecc_aggrWriteSVBUSReg(pEccAggrRegs, ramId, CSL_EDC_CTL_CONTROL, val);
        retVal = CSL_PASS;
    }
    return retVal;
}

/**
 * Requirement: REQ_TAG(PDK-6040) REQ_TAG(PDK-6058) REQ_TAG(PDK-5893)
 * Design: did_csl_ecc_aggr_config
 */
int32_t CSL_ecc_aggrVerifyConfigEDCInterconnect(CSL_ecc_aggrRegs *pEccAggrRegs,
    uint32_t ramId, bool bEccCheck)
{
    int32_t    retVal = CSL_EBADARGS;

    /* Do parameter checks */
    if ( (pEccAggrRegs != NULL_PTR)       &&
         (CSL_ecc_aggrIsValidRamId(pEccAggrRegs, ramId) == (bool)true) )
    {
        uint32_t val, valExp = 0U;
        uint32_t mask = (CSL_ECC_RAM_CTRL_ECC_CHECK_MASK);

        /* Read the EDC control register */
        CSL_ecc_aggrReadSVBUSReg(pEccAggrRegs, ramId, CSL_EDC_CTL_CONTROL, &val);

        /* set expected configuration fields */
        CSL_FINS(valExp, ECC_RAM_CTRL_ECC_CHECK, (bEccCheck ? (uint32_t)1U : (uint32_t)0) );

        /* Compare the masked values */
        if ((val & mask) == (valExp & mask))
        {
            retVal = CSL_PASS;
        }
        else
        {
            retVal = CSL_EFAIL;
        }
    }
    return retVal;
}

/**
 * Requirement: REQ_TAG(PDK-6030) REQ_TAG(PDK-5886)
 * Design: did_csl_ecc_aggr_read
 */
int32_t CSL_ecc_aggrGetEDCInterconnectErrorStatus(CSL_ecc_aggrRegs *pEccAggrRegs,
    uint32_t ramId, CSL_Ecc_AggrEDCInterconnectErrorStatusInfo *pEccErrorStatus)
{
    int32_t    retVal = CSL_EBADARGS;

    /* Do parameter checks */
    if ( (pEccAggrRegs != NULL_PTR)       &&
         (pEccErrorStatus != NULL_PTR)       &&
         ( CSL_ecc_aggrIsValidRamId(pEccAggrRegs, ramId) == (bool)true ) )
    {
        uint32_t  errorStatus1, errorStatus2;

        /* Read error status register 1 */
        CSL_ecc_aggrReadSVBUSReg(pEccAggrRegs, ramId, CSL_EDC_CTL_ERR_STATUS1, &errorStatus1);

        /* Read error status register 2 */
        CSL_ecc_aggrReadSVBUSReg(pEccAggrRegs, ramId, CSL_EDC_CTL_ERR_STATUS2, &errorStatus2);

        /* Populate the status structure with the appropriate bits */
        pEccErrorStatus->singleBitErrorCount = ((uint8_t)CSL_FEXT(errorStatus1, EDC_CTL_ERR_STATUS1_COR_PEND));
        pEccErrorStatus->doubleBitErrorCount = ((uint8_t)CSL_FEXT(errorStatus1, EDC_CTL_ERR_STATUS1_UNC_PEND));
        pEccErrorStatus->injectSingleBitErrorCount = ((uint8_t)CSL_FEXT(errorStatus1, EDC_CTL_ERR_STATUS1_INJ_COR_PEND));
        pEccErrorStatus->injectDoubleBitErrorCount = ((uint8_t)CSL_FEXT(errorStatus1, EDC_CTL_ERR_STATUS1_INJ_UNC_PEND));
        pEccErrorStatus->eccGroup    =  CSL_FEXT(errorStatus1, EDC_CTL_ERR_STATUS1_ERR_GRP);
        pEccErrorStatus->eccBit1   = CSL_FEXT(errorStatus2, EDC_CTL_ERR_STATUS2_ERR_BIT);
        retVal = CSL_PASS;
    }
    return retVal;
}

/**
 * Requirement: REQ_TAG(PDK-6040) REQ_TAG(PDK-5893)
 * Design: did_csl_ecc_aggr_write
 */
int32_t CSL_ecc_aggrForceEDCInterconnectError(CSL_ecc_aggrRegs *pEccAggrRegs,
    uint32_t ramId, const CSL_Ecc_AggrEDCInterconnectErrorInfo *pEccForceError)
{
    int32_t    retVal = CSL_PASS;

    /* Do parameter checks */
    if ( (pEccAggrRegs == NULL_PTR) ||
         (pEccForceError == NULL_PTR) ||
         ( CSL_ecc_aggrIsValidRamId(pEccAggrRegs, ramId) == (bool)false) )
    {
        retVal = CSL_EBADARGS;
    }

    if ( retVal == CSL_PASS)
    {
        if ( pEccForceError->eccPattern > CSL_ECC_EGGR_INJECT_PATTERN_MAX )
        {
            retVal = CSL_EBADARGS;
        }
    }

    if ( retVal == CSL_PASS )
    {
        uint32_t regVal;

        /* Configure Group, Bits for error injection */
        CSL_ecc_aggrWriteSVBUSReg(pEccAggrRegs, ramId, CSL_EDC_CTL_ERR_INJECT1,
            CSL_FMK(EDC_CTL_ERR_INJECT1_ECC_BIT1, pEccForceError->eccBit1)     |
            CSL_FMK(EDC_CTL_ERR_INJECT1_ECC_GRP, pEccForceError->eccGroup) );

        CSL_ecc_aggrWriteSVBUSReg(pEccAggrRegs, ramId, CSL_EDC_CTL_ERR_INJECT2,
            CSL_FMK(EDC_CTL_ERR_INJECT2_ECC_BIT2, pEccForceError->eccBit2) );

        CSL_ecc_aggrReadSVBUSReg(pEccAggrRegs, ramId, CSL_EDC_CTL_CONTROL, &regVal);

        /* Configure single bit or double bit */
        if(pEccForceError->intrSrc == CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT)
        {
            CSL_FINS(regVal, EDC_CTL_CONTROL_FORCE_SE, (uint32_t)1U);
            CSL_FINS(regVal, EDC_CTL_CONTROL_FORCE_DE, (uint32_t)0);
        }
        else if(pEccForceError->intrSrc == CSL_ECC_AGGR_INTR_SRC_DOUBLE_BIT)
        {
            CSL_FINS(regVal, EDC_CTL_CONTROL_FORCE_SE, (uint32_t)0);
            CSL_FINS(regVal, EDC_CTL_CONTROL_FORCE_DE, (uint32_t)1U);
        }
        else
        {
            CSL_FINS(regVal, EDC_CTL_CONTROL_FORCE_SE, (uint32_t)0);
            CSL_FINS(regVal, EDC_CTL_CONTROL_FORCE_DE, (uint32_t)0);
        }

        /* Configure force n bit */
        CSL_FINS(regVal, EDC_CTL_CONTROL_FORCE_N_BIT,
            ((pEccForceError->bNextBit == TRUE) ? (uint32_t)1U : (uint32_t)0) );

        /* Configure ECC pattern */
        CSL_FINS(regVal, EDC_CTL_CONTROL_ECC_PATTERN, (((uint32_t)pEccForceError->eccPattern) & 0x3U));

        /* Write the wrapper register to inject the error */
        CSL_ecc_aggrWriteSVBUSReg(pEccAggrRegs, ramId, CSL_EDC_CTL_CONTROL, regVal);
        retVal = CSL_PASS;
    }
    return retVal;
}

/**
 * Requirement: REQ_TAG(PDK-6038)
 * Design: did_csl_ecc_aggr_intr
 */
int32_t CSL_ecc_aggrAckIntr(CSL_ecc_aggrRegs *pEccAggrRegs, CSL_Ecc_AggrIntrSrc intrSrc)
{
    int32_t    retVal = CSL_PASS;

    if ( pEccAggrRegs == NULL_PTR)
    {
         retVal = CSL_EBADARGS;
    }
    else
    {
        if( intrSrc == CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT )
        {
            CSL_REG32_WR( &pEccAggrRegs->SEC_EOI_REG, CSL_FMK(ECC_AGGR_SEC_EOI_REG_EOI_WR,(uint32_t)1U) );
        }
        else if( intrSrc == CSL_ECC_AGGR_INTR_SRC_DOUBLE_BIT )
        {
            CSL_REG32_WR( &pEccAggrRegs->DED_EOI_REG, CSL_FMK(ECC_AGGR_DED_EOI_REG_EOI_WR,(uint32_t)1U) );
        }
        else
        {
            retVal = CSL_EBADARGS;
        }
    }
    return retVal;
}

/**
 * Requirement: REQ_TAG(PDK-6038)
 * Design: did_csl_ecc_aggr_intr
 */
int32_t CSL_ecc_aggrIsEccRamIntrPending(CSL_ecc_aggrRegs *pEccAggrRegs,
    uint32_t ramId, CSL_Ecc_AggrIntrSrc intrSrc, bool *pIsPend)
{
    bool        pend  = (bool)false;
    int32_t     retVal = CSL_EBADARGS;

    if ( (pEccAggrRegs != NULL_PTR)       &&
         ( CSL_ecc_aggrIsValidRamId(pEccAggrRegs, ramId) == (bool)true ) )
    {
        uint32_t regVal;
        /* No need to check the return value from CSL as arguments are already checked above */
        retVal = CSL_ecc_aggrReadEccRamErrStatReg(pEccAggrRegs, ramId, CSL_ECC_AGGR_SELECT_ERR_STAT1, &regVal);
        if (retVal == CSL_PASS)
        {
            if( ((intrSrc == CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT) && 
                 (CSL_FEXT(regVal,ECC_RAM_ERR_STAT1_ECC_SEC) != 0U) ) ||
                ((intrSrc == CSL_ECC_AGGR_INTR_SRC_DOUBLE_BIT) &&
                 (CSL_FEXT(regVal,ECC_RAM_ERR_STAT1_ECC_DED) != 0U) ) ||
                ((intrSrc == CSL_ECC_ADDR_ERROR_TYPE_SUCCESSIVE_SINGLE_BITS) &&
                 (CSL_FEXT(regVal,ECC_RAM_ERR_STAT1_ECC_OTHER) != 0U) ) )
            {
                pend = (bool)true;
            }
        }
    }
    if (pIsPend != NULL_PTR)
    {
        *pIsPend = pend;
    }
    else
    {
        retVal = CSL_EBADARGS;
    }
    return retVal;
}

/**
 * Requirement: REQ_TAG(PDK-6038)
 * Design: did_csl_ecc_aggr_intr
 */
int32_t CSL_ecc_aggrSetEccRamIntrPending(CSL_ecc_aggrRegs *pEccAggrRegs,
    uint32_t ramId, CSL_Ecc_AggrIntrSrc intrSrc)
{
    bool       ret = (bool) false;
    int32_t    retVal = CSL_EBADARGS;

    /* RAM ID and Interrupt Source is validated internally in below function */
    if ( (pEccAggrRegs != NULL_PTR) &&
         ((CSL_ecc_aggrIsValidIntrSrc(intrSrc)  == (bool) true ) ||
          (intrSrc == CSL_ECC_ADDR_ERROR_TYPE_SUCCESSIVE_SINGLE_BITS)) )
    {
        ret = CSL_ecc_aggrToggleEccRamIntrPending(pEccAggrRegs, ramId, intrSrc, 1U, (bool)true);
    }
                     
    if (ret == (bool) true)
    {
        retVal = CSL_PASS;
    }
    return (retVal);
}

/**
 * Requirement: REQ_TAG(PDK-6038)
 * Design: did_csl_ecc_aggr_intr
 */
int32_t CSL_ecc_aggrSetEccRamNIntrPending(CSL_ecc_aggrRegs *pEccAggrRegs,
    uint32_t ramId, CSL_Ecc_AggrIntrSrc intrSrc, uint32_t numEvents)
{
    bool       ret = (bool) false;
    int32_t    retVal = CSL_EBADARGS;
    void       *pChkPtr = (void *) pEccAggrRegs;

    /* RAM ID and Interrupt Source is validated internally in below function */
    if ( (pChkPtr != NULL_PTR) && (numEvents != 0U) &&
         (numEvents <= 3U) &&
         ((CSL_ecc_aggrIsValidIntrSrc(intrSrc)  == (bool) true ) ||
          (intrSrc == CSL_ECC_ADDR_ERROR_TYPE_SUCCESSIVE_SINGLE_BITS)) )
    {
        ret = CSL_ecc_aggrToggleEccRamIntrPending(pEccAggrRegs, ramId,
                  intrSrc, numEvents, (bool)true);
    }

    if (ret == (bool) true)
    {
        retVal = CSL_PASS;
    }
    return (retVal);
}

/**
 * Requirement: REQ_TAG(PDK-6038)
 * Design: did_csl_ecc_aggr_intr
 */
int32_t CSL_ecc_aggrClrEccRamIntrPending(CSL_ecc_aggrRegs *pEccAggrRegs,
    uint32_t ramId, CSL_Ecc_AggrIntrSrc intrSrc)
{
    bool       ret = (bool) false;
    int32_t    retVal = CSL_EBADARGS;

    /* RAM ID and Interrupt Source is validated internally in below function */
    if ( (pEccAggrRegs != NULL_PTR) &&
         ((CSL_ecc_aggrIsValidIntrSrc(intrSrc)  == (bool) true ) ||
          (intrSrc == CSL_ECC_ADDR_ERROR_TYPE_SUCCESSIVE_SINGLE_BITS)) )
    {
        ret = CSL_ecc_aggrToggleEccRamIntrPending(pEccAggrRegs, ramId,
                  intrSrc, 1U, (bool)false);
    }

    if (ret == (bool) true)
    {
        retVal = CSL_PASS;
    }
    return (retVal);
}

/**
 * Requirement: REQ_TAG(PDK-6038)
 * Design: did_csl_ecc_aggr_intr
 */
int32_t CSL_ecc_aggrClrEccRamNIntrPending(CSL_ecc_aggrRegs *pEccAggrRegs,
    uint32_t ramId, CSL_Ecc_AggrIntrSrc intrSrc, uint32_t numEvents)
{
    bool       ret = (bool) false;
    int32_t    retVal = CSL_EBADARGS;

    /* RAM ID and Interrupt Source is validated internally in below function */
    if ( (pEccAggrRegs != NULL_PTR) &&
         (numEvents != 0U) &&
         (numEvents <= 3U) &&
         ((CSL_ecc_aggrIsValidIntrSrc(intrSrc)  == (bool) true ) ||
          (intrSrc == CSL_ECC_ADDR_ERROR_TYPE_SUCCESSIVE_SINGLE_BITS)) )
    {
        ret = CSL_ecc_aggrToggleEccRamIntrPending(pEccAggrRegs, ramId,
                  intrSrc, numEvents, (bool)false);
    }

    if (ret == (bool) true)
    {
        retVal = CSL_PASS;
    }
    return (retVal);
}

/**
 * Requirement: REQ_TAG(PDK-6038) REQ_TAG(PDK-5886)
 * Design: did_csl_ecc_aggr_intr
 */
int32_t CSL_ecc_aggrIsEDCInterconnectIntrPending(CSL_ecc_aggrRegs *pEccAggrRegs,
    uint32_t ramId, CSL_Ecc_AggrIntrSrc intrSrc, bool *pIsPend)
{
    bool        pend  = (bool)false;
    int32_t     retVal = CSL_EBADARGS;

    /* Do paramter checks */
    if ( (pEccAggrRegs != NULL_PTR) &&
         ( CSL_ecc_aggrIsValidRamId(pEccAggrRegs, ramId) == (bool)true ) &&
         (CSL_ecc_aggrIsValidIntrSrc(intrSrc)  == (bool) true ) &&
         (pIsPend != NULL_PTR) )
    {
        uint32_t regVal;

        /* Read Error Status register 1 */
        CSL_ecc_aggrReadSVBUSReg(pEccAggrRegs, ramId, CSL_EDC_CTL_ERR_STATUS1, &regVal);

        /* Check event pending */
        if( ((intrSrc == CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT) &&
             (CSL_FEXT(regVal, EDC_CTL_ERR_STATUS1_COR_PEND) != 0U) ) ||
            ((intrSrc == CSL_ECC_AGGR_INTR_SRC_DOUBLE_BIT) &&
             (CSL_FEXT(regVal, EDC_CTL_ERR_STATUS1_UNC_PEND) != 0U) ) ||
            ((intrSrc == CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT) &&
             (CSL_FEXT(regVal, EDC_CTL_ERR_STATUS1_INJ_COR_PEND) != 0U) ) ||
            ((intrSrc == CSL_ECC_AGGR_INTR_SRC_DOUBLE_BIT) &&
             (CSL_FEXT(regVal, EDC_CTL_ERR_STATUS1_INJ_UNC_PEND) != 0U) ) )
        {
            pend = (bool)true;
        }

        /* Return is pending flag */
        *pIsPend = pend;
        retVal = CSL_PASS;
    }

    return retVal;
}

/**
 * Requirement: REQ_TAG(PDK-6038) REQ_TAG(PDK-5886)
 * Design: did_csl_ecc_aggr_intr
 */
int32_t CSL_ecc_aggrSetEDCInterconnectNIntrPending(CSL_ecc_aggrRegs *pEccAggrRegs,
                                                   uint32_t ramId,
                                                   CSL_Ecc_AggrIntrSrc intrSrc,
                                                   CSL_Ecc_AggrEDCErrorSubType subType,
                                                   uint32_t numEvents)
{
    bool       ret = (bool) false;
    int32_t    retVal = CSL_EBADARGS;
    void       *pChkPtr = (void *) pEccAggrRegs;

    /* RAM ID and Interrupt Source is validated internally in below function */
    if ( (pChkPtr != NULL_PTR) &&
         (CSL_ecc_aggrIsValidIntrSrc(intrSrc)  == (bool) true ) &&
         (numEvents != 0U) &&
         (numEvents <= 3U) )
    {
        ret = CSL_ecc_aggrToggleEDCInterconnectIntrPending(pEccAggrRegs, ramId,
                  intrSrc, subType, numEvents, (bool)true);
    }

    if (ret == (bool) true)
    {
        retVal = CSL_PASS;
    }
    return (retVal);
}

/**
 * Requirement: REQ_TAG(PDK-6038) REQ_TAG(PDK-5886)
 * Design: did_csl_ecc_aggr_intr
 */
int32_t CSL_ecc_aggrClrEDCInterconnectNIntrPending(CSL_ecc_aggrRegs *pEccAggrRegs,
                                                   uint32_t ramId,
                                                   CSL_Ecc_AggrIntrSrc intrSrc,
                                                   CSL_Ecc_AggrEDCErrorSubType subType,
                                                   uint32_t numEvents)
{
    bool       ret = (bool) false;
    int32_t    retVal = CSL_EBADARGS;

    /* RAM ID and Interrupt Source is validated internally in below function */
    if ( (pEccAggrRegs != NULL_PTR) &&
         (CSL_ecc_aggrIsValidIntrSrc(intrSrc)  == (bool) true ) &&
         (numEvents <= 3U) && (numEvents != 0U) )
    {
        ret = CSL_ecc_aggrToggleEDCInterconnectIntrPending(pEccAggrRegs,
                  ramId, intrSrc, subType, numEvents, (bool)false);
    }

    if (ret == (bool) true)
    {
        retVal = CSL_PASS;
    }
    return (retVal);
}

/**
 * Requirement: REQ_TAG(PDK-6038) REQ_TAG(PDK-5886)
 * Design: did_csl_ecc_aggr_intr
 */
int32_t CSL_ecc_aggrIsIntrPending(const CSL_ecc_aggrRegs *pEccAggrRegs,
    uint32_t ramId, CSL_Ecc_AggrIntrSrc intrSrc, bool *pIsPend)
{
    bool       pend = (bool) false;
    int32_t    retVal = CSL_EBADARGS;

    if ( (pEccAggrRegs != NULL_PTR) )
    {
        if( CSL_ecc_aggrIsValidRamId(pEccAggrRegs, ramId) &&
            CSL_ecc_aggrIsValidIntrSrc(intrSrc) )
        {
            uint32_t    regVal;
            uintptr_t   regAddr;

            retVal  = CSL_PASS;
            regAddr = CSL_ecc_aggrStatusRegAddr( (uintptr_t)pEccAggrRegs,
                          (ramId >> 5U), intrSrc );
            regVal = CSL_REG32_RD( regAddr );
            regVal >>= (ramId & 0x1FU);                    // Shift bit corresponding to the ramId into bit 0
            if( (regVal & 1U) != 0U )
            {
                pend = (bool)true;
            }
        }
    }

    if (pIsPend != NULL_PTR)
    {
        *pIsPend = pend;
    }
    else
    {
        retVal = CSL_EBADARGS;
    }

    return retVal;
}

/**
 * Requirement: REQ_TAG(PDK-6038)
 * Design: did_csl_ecc_aggr_intr
 */
int32_t CSL_ecc_aggrIsAnyIntrPending(const CSL_ecc_aggrRegs *pEccAggrRegs,
    uint32_t ramId, bool *pIsPend)
{
    bool       pend = (bool) false;
    bool       sBitPend, dBitPend;
    int32_t    retVal;

    /* Argument verification is done at below APIs, hence do not need to check here */
    retVal = CSL_ecc_aggrIsIntrPending(pEccAggrRegs, ramId,
                 CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT, &sBitPend);
    if (retVal == CSL_PASS)
    {
        retVal = CSL_ecc_aggrIsIntrPending(pEccAggrRegs, ramId,
                     CSL_ECC_AGGR_INTR_SRC_DOUBLE_BIT, &dBitPend);
    }
    if (retVal == CSL_PASS)
    {
        if ( sBitPend || dBitPend ) 
        {
           pend = (bool)true;
        }
    }

    if (pIsPend != NULL_PTR)
    {
        *pIsPend = pend;
    }
    else
    {
        retVal = CSL_EBADARGS;
    }
    return (retVal);
}

/**
 * Requirement: REQ_TAG(PDK-6038)
 * Design: did_csl_ecc_aggr_intr
 */
int32_t CSL_ecc_aggrEnableIntr(const CSL_ecc_aggrRegs *pEccAggrRegs,
    uint32_t ramId, CSL_Ecc_AggrIntrSrc intrSrc)
{
    int32_t retVal = CSL_EBADARGS;
    bool    operation = (bool) false;

    if ( (pEccAggrRegs != NULL_PTR) )
    {
        /* other arguments are verified in below function */
        operation = CSL_ecc_aggrToggleIntrEnable(pEccAggrRegs, ramId,
                        intrSrc, (bool)true);
    }

    if (operation == (bool) true)
    {
        retVal = CSL_PASS;
    }
    else
    {
        retVal = CSL_EBADARGS;
    }

    return (retVal);
}

/**
 * Requirement: REQ_TAG(PDK-6038)
 * Design: did_csl_ecc_aggr_intr
 */
int32_t CSL_ecc_aggrDisableIntr(const CSL_ecc_aggrRegs *pEccAggrRegs,
    uint32_t ramId, CSL_Ecc_AggrIntrSrc intrSrc)
{
    int32_t retVal = CSL_EBADARGS;
    bool    operation = (bool) false;

    if ( (pEccAggrRegs != NULL_PTR) )
    {
        operation = CSL_ecc_aggrToggleIntrEnable(pEccAggrRegs, ramId,
                        intrSrc, (bool)false);
    }

    if (operation == (bool) true)
    {
        retVal = CSL_PASS;
    }
    else
    {
        retVal = CSL_EBADARGS;
    }

    return (retVal);
}

/**
 * Requirement: REQ_TAG(PDK-6038)
 * Design: did_csl_ecc_aggr_intr
 */
int32_t CSL_ecc_aggrEnableAllIntr(const CSL_ecc_aggrRegs *pEccAggrRegs,
    uint32_t ramId)
{
    int32_t retVal = CSL_EBADARGS;

    if ( (pEccAggrRegs != NULL_PTR) )
    {
        if( CSL_ecc_aggrIsValidRamId(pEccAggrRegs, ramId) == (bool)true )
        {
            /* Other Argument verification is done in below APIs */
            retVal = CSL_ecc_aggrEnableIntr(pEccAggrRegs, ramId,
                         CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT);
            if (retVal == CSL_PASS)
            {
                retVal = CSL_ecc_aggrEnableIntr(pEccAggrRegs, ramId,
                             CSL_ECC_AGGR_INTR_SRC_DOUBLE_BIT);
            }
        }
    }
    return retVal;
}

/**
 * Requirement: REQ_TAG(PDK-6038)
 * Design: did_csl_ecc_aggr_intr
 */
int32_t CSL_ecc_aggrDisableAllIntr(const CSL_ecc_aggrRegs *pEccAggrRegs,
    uint32_t ramId)
{
    int32_t retVal = CSL_EBADARGS;

    if ( (pEccAggrRegs != NULL_PTR) )
    {
        if( CSL_ecc_aggrIsValidRamId(pEccAggrRegs, ramId) == (bool)true )
        {
            /* Other Argument verification is done in below APIs */
            retVal = CSL_ecc_aggrDisableIntr(pEccAggrRegs, ramId,
                         CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT);
            if (retVal == CSL_PASS)
            {
                retVal = CSL_ecc_aggrDisableIntr(pEccAggrRegs, ramId,
                             CSL_ECC_AGGR_INTR_SRC_DOUBLE_BIT);
            }
        }
    }
    return retVal;
}

/**
 * Requirement: REQ_TAG(PDK-6038)
 * Design: did_csl_ecc_aggr_intr
 */
int32_t CSL_ecc_aggrEnableIntrs(const CSL_ecc_aggrRegs *pEccAggrRegs,
    CSL_Ecc_AggrIntrSrc intrSrc)
{
    int32_t retVal = CSL_EBADARGS;
    bool    operation = (bool) false;

    /* Other argument checks are done internal to below API */
    if ( (pEccAggrRegs != NULL_PTR) )
    {
        operation = CSL_ecc_aggrToggleIntrsEnable(pEccAggrRegs, intrSrc,
                       (bool)true);
    }

    if (operation == (bool) true)
    {
        retVal = CSL_PASS;
    }
    else
    {
        retVal = CSL_EBADARGS;
    }

    return (retVal);
}

/**
 * Requirement: REQ_TAG(PDK-6038)
 * Design: did_csl_ecc_aggr_intr
 */
int32_t CSL_ecc_aggrDisableIntrs(const CSL_ecc_aggrRegs *pEccAggrRegs,
    CSL_Ecc_AggrIntrSrc intrSrc)
{
    int32_t retVal = CSL_EBADARGS;
    bool    operation = (bool) false;

    /* Other argument checks are done internal to below API */
    if ( (pEccAggrRegs != NULL_PTR) )
    {
        operation = CSL_ecc_aggrToggleIntrsEnable(pEccAggrRegs, intrSrc, (bool)false);
    }

    if (operation == (bool) true)
    {
        retVal = CSL_PASS;
    }
    else
    {
        retVal = CSL_EBADARGS;
    }

    return (retVal);
}

/**
 * Requirement: REQ_TAG(PDK-6038)
 * Design: did_csl_ecc_aggr_intr
 */
int32_t CSL_ecc_aggrEnableAllIntrs(const CSL_ecc_aggrRegs *pEccAggrRegs)
{
    int32_t retVal = CSL_EBADARGS;

    /* Other argument checks are done internal to below API */
    if ( (pEccAggrRegs != NULL_PTR) )
    {
        retVal = CSL_ecc_aggrEnableIntrs(pEccAggrRegs, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT);
        if (retVal == CSL_PASS)
        {
            retVal = CSL_ecc_aggrEnableIntrs(pEccAggrRegs, CSL_ECC_AGGR_INTR_SRC_DOUBLE_BIT);
        }
    }
    return (retVal);
}

/**
 * Requirement: REQ_TAG(PDK-6038)
 * Design: did_csl_ecc_aggr_intr
 */
int32_t CSL_ecc_aggrDisableAllIntrs(const CSL_ecc_aggrRegs *pEccAggrRegs)
{
    int32_t retVal = CSL_EBADARGS;

    /* Other argument checks are done internal to below API */
    if ( (pEccAggrRegs != NULL_PTR) )
    {
        retVal = CSL_ecc_aggrDisableIntrs(pEccAggrRegs, CSL_ECC_AGGR_INTR_SRC_SINGLE_BIT);
        if (retVal == CSL_PASS)
        {
            retVal = CSL_ecc_aggrDisableIntrs(pEccAggrRegs, CSL_ECC_AGGR_INTR_SRC_DOUBLE_BIT);
        }
    }
    return (retVal);
}

/**
 * Requirement: REQ_TAG(PDK-6038)
 * Design: did_csl_ecc_aggr_intr
 */
int32_t CSL_ecc_aggrReadStaticRegs(CSL_ecc_aggrRegs *pEccAggrRegs,
    CSL_ecc_aggrStaticRegs *pEccAggrStaticRegs)
{
    int32_t  retVal = CSL_EBADARGS;
    uint32_t regOffset, *pRegPtr;
    uint32_t i;

    /* Read below static registers */
    retVal = CSL_ecc_aggrGetRevision((const CSL_ecc_aggrRegs *) pEccAggrRegs,
                 &pEccAggrStaticRegs->REV);

    /* ECC Control register */
    if (retVal == CSL_PASS)
    {
        regOffset = CSL_ECC_RAM_CTRL;
        retVal = CSL_ecc_aggrReadEccRamReg(pEccAggrRegs, 0, regOffset,
                     &pEccAggrStaticRegs->ECC_CTRL);
    }

    /* ECC Err Control1 register */
    if (retVal == CSL_PASS)
    {
        regOffset = CSL_ECC_RAM_ERR_CTRL1;
        retVal = CSL_ecc_aggrReadEccRamReg(pEccAggrRegs, 0, regOffset,
                     &pEccAggrStaticRegs->ECC_ERR_CTRL1);
    }

    /* ECC Err Control2 register */
    if (retVal == CSL_PASS)
    {
        regOffset = CSL_ECC_RAM_ERR_CTRL2;
        retVal = CSL_ecc_aggrReadEccRamReg(pEccAggrRegs, 0, regOffset,
                     &pEccAggrStaticRegs->ECC_ERR_CTRL2);
    }

    /* Other argument checks are done internal to below API */
    if ( (pEccAggrRegs != NULL_PTR) &&
         (pEccAggrStaticRegs != NULL_PTR) )
    {
        if (retVal == CSL_PASS)
        {
            for ( i = ((uint32_t) (0u)); i < (CSL_ECC_AGGR_NUM_ENABLE_REGISTERS); i++)
            {
                /* ECC_SEC_ENABLE_SET_REG registers */
                pRegPtr = (uint32_t *) CSL_ecc_aggrSecEnableSetRegAddr ((uintptr_t) pEccAggrRegs, i);
                pEccAggrStaticRegs->ECC_SEC_ENABLE_SET_REG[i] = CSL_REG_RD(pRegPtr);

                /* ECC_SEC_ENABLE_CLR_REG registers */
                pRegPtr = (uint32_t *) CSL_ecc_aggrSecEnableClrRegAddr ((uintptr_t) pEccAggrRegs, i);
                pEccAggrStaticRegs->ECC_SEC_ENABLE_CLR_REG[i] = CSL_REG_RD(pRegPtr);

                /* ECC_DED_ENABLE_SET_REG registers */
                pRegPtr = (uint32_t *) CSL_ecc_aggrDedEnableSetRegAddr ((uintptr_t) pEccAggrRegs, i);
                pEccAggrStaticRegs->ECC_DED_ENABLE_SET_REG[i] = CSL_REG_RD(pRegPtr);

                /* ECC_DED_ENABLE_CLR_REG registers */
                pRegPtr = (uint32_t *) CSL_ecc_aggrDedEnableClrRegAddr ((uintptr_t) pEccAggrRegs, i);
                pEccAggrStaticRegs->ECC_DED_ENABLE_CLR_REG[i] = CSL_REG_RD(pRegPtr);
            }
        }
    }

    return (retVal);
}

/**
 * Requirement: REQ_TAG(PDK-6038) REQ_TAG(PDK-5861)
 * Design: did_csl_ecc_aggr_intr
 */

int32_t CSL_ecc_aggrIntrEnableCtrl(CSL_ecc_aggrRegs *pEccAggrRegs,
    const CSL_ecc_aggrEnableCtrl *pEnableCtrl)
{
    int32_t retVal = CSL_EBADARGS;

    if ((pEccAggrRegs   != NULL_PTR) &&
        (pEnableCtrl    != NULL_PTR))
    {
        if ((pEnableCtrl->validCfg & CSL_ECC_AGGR_VALID_TIMEOUT_ERR) == \
             CSL_ECC_AGGR_VALID_TIMEOUT_ERR)
        {
            if((pEnableCtrl->intrEnableTimeoutErr) == TRUE)
            {
                CSL_REG32_FINS(&pEccAggrRegs->AGGR_ENABLE_SET,      \
                               ECC_AGGR_AGGR_ENABLE_SET_TIMEOUT,    \
                               1u);
            }
            else
            {
                CSL_REG32_FINS(&pEccAggrRegs->AGGR_ENABLE_CLR,      \
                               ECC_AGGR_AGGR_ENABLE_CLR_TIMEOUT,    \
                               1u);
            }
        }
        if ((pEnableCtrl->validCfg & CSL_ECC_AGGR_VALID_PARITY_ERR) == \
             CSL_ECC_AGGR_VALID_PARITY_ERR)
        {
            if((pEnableCtrl->intrEnableParityErr) == TRUE)
            {
                CSL_REG32_FINS(&pEccAggrRegs->AGGR_ENABLE_SET,      \
                               ECC_AGGR_AGGR_ENABLE_SET_PARITY,     \
                               1u);          
            }
            else
            {
                CSL_REG32_FINS(&pEccAggrRegs->AGGR_ENABLE_CLR,      \
                               ECC_AGGR_AGGR_ENABLE_CLR_PARITY,     \
                               1u);            
            }
        }

        retVal = CSL_PASS;
    }
    return (retVal);    
}

/**
 * Requirement: REQ_TAG(PDK-6038) REQ_TAG(PDK-5861)
 * Design: did_csl_ecc_aggr_intr
 */
int32_t CSL_ecc_aggrIntrStatusCtrl(CSL_ecc_aggrRegs *pEccAggrRegs,
    const CSL_ecc_aggrStatusCtrl *pStatusCtrl)
{
    int32_t retVal = CSL_EBADARGS;
    uint32_t tCnt, pCnt;

    if ((pEccAggrRegs   != NULL_PTR) &&
        (pStatusCtrl    != NULL_PTR) &&
        (pStatusCtrl->timeOutCnt < CSL_TWO_POWER_TWO_VALUE) &&
        (pStatusCtrl->parityCnt  < CSL_TWO_POWER_TWO_VALUE))
    {
        if ((pStatusCtrl->validCfg & CSL_ECC_AGGR_VALID_TIMEOUT_ERR) == \
             CSL_ECC_AGGR_VALID_TIMEOUT_ERR)
        {
            tCnt    = (uint32_t)pStatusCtrl->timeOutCnt;
            if((pStatusCtrl->intrStatusSetTimeoutErr) == TRUE)
            {
                CSL_REG32_FINS(&pEccAggrRegs->AGGR_STATUS_SET,      \
                               ECC_AGGR_AGGR_STATUS_SET_TIMEOUT,     \
                               tCnt);            
            }
            else
            {
                CSL_REG32_FINS(&pEccAggrRegs->AGGR_STATUS_CLR,      \
                               ECC_AGGR_AGGR_STATUS_CLR_TIMEOUT,     \
                               tCnt);            
            }
        }
        if ((pStatusCtrl->validCfg & CSL_ECC_AGGR_VALID_PARITY_ERR) == \
             CSL_ECC_AGGR_VALID_PARITY_ERR)
        {
            pCnt    = (uint32_t)pStatusCtrl->parityCnt;
            if((pStatusCtrl->intrStatusSetParityErr) == TRUE)
            {
                CSL_REG32_FINS(&pEccAggrRegs->AGGR_STATUS_SET,      \
                               ECC_AGGR_AGGR_STATUS_SET_PARITY,     \
                               pCnt);           
            }
            else
            {
                CSL_REG32_FINS(&pEccAggrRegs->AGGR_STATUS_CLR,      \
                               ECC_AGGR_AGGR_STATUS_CLR_PARITY,     \
                               pCnt);           
            }
            retVal = CSL_PASS;
        }        
    }
    return (retVal);
}

/**
 * Requirement: REQ_TAG(PDK-6038) REQ_TAG(PDK-5861)
 * Design: did_csl_ecc_aggr_intr
 */
int32_t CSL_ecc_aggrIntrGetStatus(const CSL_ecc_aggrRegs       *pEccAggrRegs,
                                      CSL_ecc_aggrStatusCtrl *pStatusCtrl)
{
    int32_t retVal = CSL_EBADARGS;
    

    if ((pEccAggrRegs   != NULL_PTR) &&
        (pStatusCtrl    != NULL_PTR))
    {
        if ((pStatusCtrl->validCfg & CSL_ECC_AGGR_VALID_TIMEOUT_ERR) == \
             CSL_ECC_AGGR_VALID_TIMEOUT_ERR)
        {
            if((pStatusCtrl->intrStatusSetTimeoutErr)== TRUE)
            {
                pStatusCtrl->timeOutCnt = \
                CSL_REG32_FEXT(&pEccAggrRegs->AGGR_STATUS_SET,      \
                               ECC_AGGR_AGGR_STATUS_SET_TIMEOUT);
            }
            else
            {
                pStatusCtrl->timeOutCnt = \
                CSL_REG32_FEXT(&pEccAggrRegs->AGGR_STATUS_CLR,      \
                               ECC_AGGR_AGGR_STATUS_CLR_TIMEOUT);
            }
        }
        if ((pStatusCtrl->validCfg & CSL_ECC_AGGR_VALID_PARITY_ERR) == \
             CSL_ECC_AGGR_VALID_PARITY_ERR)
        {
            if((pStatusCtrl->intrStatusSetParityErr) == TRUE)
            {
                pStatusCtrl->parityCnt = \
                CSL_REG32_FEXT(&pEccAggrRegs->AGGR_STATUS_SET,      \
                               ECC_AGGR_AGGR_STATUS_SET_PARITY);
            }
            else
            {
                pStatusCtrl->parityCnt = \
                CSL_REG32_FEXT(&pEccAggrRegs->AGGR_STATUS_CLR,      \
                               ECC_AGGR_AGGR_STATUS_CLR_PARITY);
            }
            retVal = CSL_PASS;
        }        
    }
    return (retVal);
}
/* Nothing past this point */

