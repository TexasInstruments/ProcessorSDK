/**
 * @file  csl_slv_tog.c
 *
 * @brief
 *  Implementation file for the VBUSM Slave Timeout Gasket module CSL.
 *
 *  \par
 *  ============================================================================
 *  @n   (C) Copyright 2019-2020, Texas Instruments, Inc.
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
 *    Neither the name of Texas Instruments Incorposlv_toged nor the names of
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

#include <stdint.h>
#include <stdbool.h>
#include <ti/csl/csl_tog.h>
#include <ti/csl/csl_types.h>

#define CSL_SLV_TOG_ENABLE_KEY      ((uint32_t) 0x0000000FU)
#define CSL_SLV_TOG_FLUSH_MODE_KEY  ((uint32_t) 0x0000000FU)

static void CSL_slvTogStopLocal( CSL_ksbus_vbusm_to_gasketRegs *pRegs );
static void CSL_slvTogGetIntrCountInternal( const CSL_ksbus_vbusm_to_gasketRegs *pRegs, CSL_SlvTogIntrSrc intrSrc, uint32_t *pIntrCnt );

/**
 * Requirement: REQ_TAG(PDK-5885)
 * Design: did_csl_tog_interfaces did_csl_stog_get_error_information
 */
int32_t CSL_slvTogGetRevision( const CSL_ksbus_vbusm_to_gasketRegs *pRegs, uint32_t *pRev )
{
    int32_t retVal = CSL_EBADARGS;

    if( (pRegs != NULL_PTR) && (pRev != NULL_PTR) )
    {
        *pRev = CSL_REG32_RD( &pRegs->PID );
        retVal = CSL_PASS;
    }
    return retVal;
}

/**
 * Requirement: REQ_TAG(PDK-5885)
 * Design: did_csl_tog_interfaces did_csl_stog_get_error_information
 */
int32_t CSL_slvTogGetCfg( const CSL_ksbus_vbusm_to_gasketRegs *pRegs, uint32_t *pTotalReads, uint32_t *pTotalWrites )
{
    int32_t retVal = CSL_EBADARGS;

    if( (pRegs != NULL_PTR) && (pTotalReads != NULL_PTR) && (pTotalWrites != NULL_PTR) )
    {
        uint32_t regVal;

        regVal = CSL_REG32_RD( &pRegs->CFG );
        *pTotalReads = CSL_FEXT( regVal, KSBUS_VBUSM_TO_GASKET_CFG_NUM_READS );
        *pTotalWrites = CSL_FEXT( regVal, KSBUS_VBUSM_TO_GASKET_CFG_NUM_WRITES );
        retVal = CSL_PASS;
    }
    return retVal;
}

/**
 * Requirement: REQ_TAG(PDK-5885)
 * Design: did_csl_tog_interfaces did_csl_stog_get_error_information
 */
int32_t CSL_slvTogGetStatus( const CSL_ksbus_vbusm_to_gasketRegs *pRegs, uint32_t *pCurrReads, uint32_t *pCurrWrites )
{
    int32_t retVal = CSL_EBADARGS;

    if( (pRegs != NULL_PTR) && (pCurrReads != NULL_PTR) && (pCurrWrites != NULL_PTR) )
    {
        uint32_t regVal;

        regVal = CSL_REG32_RD( &pRegs->INFO );
        *pCurrReads = CSL_FEXT( regVal, KSBUS_VBUSM_TO_GASKET_INFO_CUR_READS );
        *pCurrWrites = CSL_FEXT( regVal, KSBUS_VBUSM_TO_GASKET_INFO_CUR_WRITES );
        retVal = CSL_PASS;
    }
    return retVal;
}

/**
 * Requirement: REQ_TAG(PDK-5885)
 * Design: did_csl_tog_interfaces did_csl_stog_get_error_information
 */
int32_t CSL_slvTogGetErrInfo( const CSL_ksbus_vbusm_to_gasketRegs *pRegs, CSL_SlvTogErrInfo *pErrInfo )
{
    int32_t  retVal = CSL_EFAIL;

    if( (pRegs == NULL_PTR) || (pErrInfo == NULL_PTR) )
    {
        retVal = CSL_EBADARGS;
    }
    else
    {
        uint32_t regVal = CSL_REG32_RD( &pRegs->ERR_VAL );

        if( CSL_FEXT( regVal, KSBUS_VBUSM_TO_GASKET_ERR_VAL_VAL ) == 1U )
        {
            uint32_t regVal2;

            pErrInfo->routeId   = CSL_FEXT( regVal, KSBUS_VBUSM_TO_GASKET_ERR_VAL_RID );
            pErrInfo->orderId   = CSL_FEXT( regVal, KSBUS_VBUSM_TO_GASKET_ERR_VAL_OID );
            pErrInfo->dir   = CSL_FEXT( regVal, KSBUS_VBUSM_TO_GASKET_ERR_VAL_DIR );
            pErrInfo->type  = CSL_FEXT( regVal, KSBUS_VBUSM_TO_GASKET_ERR_VAL_TYP );

            regVal = CSL_REG32_RD( &pRegs->ERR_TAG );
            pErrInfo->tag   = CSL_FEXT( regVal, KSBUS_VBUSM_TO_GASKET_ERR_TAG_TAG );
            pErrInfo->commandId   = CSL_FEXT( regVal, KSBUS_VBUSM_TO_GASKET_ERR_TAG_CID );

            regVal = CSL_REG32_RD( &pRegs->ERR_BYT );
            pErrInfo->orgByteCnt    = CSL_FEXT( regVal, KSBUS_VBUSM_TO_GASKET_ERR_BYT_OBYTECNT );
            pErrInfo->currByteCnt   = CSL_FEXT( regVal, KSBUS_VBUSM_TO_GASKET_ERR_BYT_CBYTECNT );

            regVal  = CSL_REG32_RD( &pRegs->ERR_ADDR_L );
            regVal2 = CSL_REG32_RD( &pRegs->ERR_ADDR_U );
            pErrInfo->address = (((uint64_t)regVal2) << 32U) | ((uint64_t)regVal);
            retVal = CSL_PASS;
        }
    }
    return retVal;
}

/**
 * Requirement: REQ_TAG(PDK-5885) REQ_TAG(PDK-5889)
 * Design: did_csl_tog_interfaces did_csl_stog_get_error_information
 */
int32_t CSL_slvTogGetIntrPending( const CSL_ksbus_vbusm_to_gasketRegs *pRegs, uint32_t *pPendInts )
{
    int32_t retVal = CSL_EBADARGS;

    if( (pRegs != NULL_PTR) && (pPendInts != NULL_PTR) )
    {
        *pPendInts = CSL_REG32_RD( &pRegs->ERR );
        retVal = CSL_PASS;
    }
    return retVal;
}

/**
 * Requirement: REQ_TAG(PDK-5885) REQ_TAG(PDK-5889)
 * Design: did_csl_tog_interfaces did_csl_stog_get_error_information
 */
int32_t CSL_slvTogGetRawIntrPending( const CSL_ksbus_vbusm_to_gasketRegs *pRegs, uint32_t *pRawPendInts )
{
    int32_t retVal = CSL_EBADARGS;

    if( (pRegs != NULL_PTR) && (pRawPendInts != NULL_PTR) )
    {
        *pRawPendInts = CSL_REG32_RD( &pRegs->ERR_RAW );
        retVal = CSL_PASS;
    }
    return retVal;
}

/**
 * Requirement: REQ_TAG(PDK-5887) REQ_TAG(PDK-5889)
 * Design: did_csl_tog_interfaces did_csl_stog_error_injection
 */
int32_t CSL_slvTogSetIntrPending( CSL_ksbus_vbusm_to_gasketRegs *pRegs, CSL_SlvTogIntrSrc intrSrcs )
{
    int32_t retVal = CSL_EBADARGS;

    if( (pRegs != NULL_PTR) && (intrSrcs > 0U) && (intrSrcs <= CSL_SLV_TOG_INTRSRC_ALL) )
    {
        CSL_REG32_WR( &pRegs->ERR_RAW, intrSrcs );
        retVal = CSL_PASS;
    }
    return retVal;
}

/**
 * Requirement: REQ_TAG(PDK-5885) REQ_TAG(PDK-5889)
 * Design: did_csl_tog_interfaces did_csl_stog_get_error_information
 */
int32_t CSL_slvTogClrIntrPending( CSL_ksbus_vbusm_to_gasketRegs *pRegs, CSL_SlvTogIntrSrc intrSrcs )
{
    int32_t retVal = CSL_EBADARGS;

    if( (pRegs != NULL_PTR) && (intrSrcs > 0U) && (intrSrcs <= CSL_SLV_TOG_INTRSRC_ALL) )
    {
        CSL_REG32_WR( &pRegs->ERR, intrSrcs );
        retVal = CSL_PASS;
    }
    return retVal;
}

/**
 * Requirement: REQ_TAG(PDK-5894) REQ_TAG(PDK-5889)
 * Design: did_csl_tog_interfaces did_csl_stog_cfg
 */

int32_t CSL_slvTogSetIntrEnable( CSL_ksbus_vbusm_to_gasketRegs *pRegs, CSL_SlvTogIntrSrc intrSrcs, bool bEnable )
{
    int32_t retVal = CSL_EBADARGS;

    if( (pRegs != NULL_PTR) && (intrSrcs > 0U) && (intrSrcs <= CSL_SLV_TOG_INTRSRC_ALL) )
    {
        if( bEnable == (bool)true )
        {
            CSL_REG32_WR( &pRegs->ERR_MSK_SET, intrSrcs );
        }
        else
        {
            CSL_REG32_WR( &pRegs->ERR_MSK_CLR, intrSrcs );
        }
        retVal = CSL_PASS;
    }
    return retVal;
}

/**
 * Requirement: REQ_TAG(PDK-5891)
 * Design: did_csl_tog_interfaces did_csl_stog_wr_read_back
 */
int32_t CSL_slvTogVerifyIntrEnable( CSL_ksbus_vbusm_to_gasketRegs *pRegs, CSL_SlvTogIntrSrc intrSrcs, bool bEnable )
{
    int32_t retVal = CSL_EBADARGS;
    uint32_t readVal;

    if( (pRegs != NULL_PTR) && (intrSrcs > 0U) && (intrSrcs <= CSL_SLV_TOG_INTRSRC_ALL) )
    {
        readVal = pRegs->ERR_MSK_SET & (CSL_KSBUS_VBUSM_TO_GASKET_ERR_MSK_SET_CMD_MASK
                                        | CSL_KSBUS_VBUSM_TO_GASKET_ERR_MSK_SET_UNEXP_MASK
                                        | CSL_KSBUS_VBUSM_TO_GASKET_ERR_MSK_SET_TIMEOUT_MASK);
        if( ((bEnable == (bool)true ) && ((readVal & intrSrcs) == intrSrcs))
            || ((bEnable == (bool)false) && ((readVal & intrSrcs) == 0U)))
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

static void CSL_slvTogGetIntrCountInternal( const CSL_ksbus_vbusm_to_gasketRegs *pRegs, CSL_SlvTogIntrSrc intrSrc, uint32_t *pIntrCnt )
{
    if( intrSrc == CSL_SLV_TOG_INTRSRC_TRANSACTION_TIMEOUT )
    {
        *pIntrCnt = CSL_REG32_RD( &pRegs->ERR_TM_INFO );
    }
    else if( intrSrc == CSL_SLV_TOG_INTRSRC_UNEXPECTED_RESPONSE )
    {
        *pIntrCnt = CSL_REG32_RD( &pRegs->ERR_UN_INFO );
    }
    else
    {
        ; /* No action required */
    }

    return;
}

/**
 * Requirement: REQ_TAG(PDK-5885) REQ_TAG(PDK-5889)
 * Design: did_csl_tog_interfaces did_csl_stog_get_error_information
 */
int32_t CSL_slvTogGetIntrCount( const CSL_ksbus_vbusm_to_gasketRegs *pRegs, CSL_SlvTogIntrSrc intrSrc, uint32_t *pIntrCnt )
{
    int32_t  retVal = CSL_EFAIL;

    if( (pRegs == NULL_PTR) || (pIntrCnt == NULL_PTR)
         || ((intrSrc != CSL_SLV_TOG_INTRSRC_UNEXPECTED_RESPONSE)
             && (intrSrc != CSL_SLV_TOG_INTRSRC_TRANSACTION_TIMEOUT)))

    {
        retVal = CSL_EBADARGS;
    }
    else
    {
        CSL_slvTogGetIntrCountInternal(pRegs, intrSrc, pIntrCnt);
        retVal = CSL_PASS;
    }
    return retVal;
}

/**
 * Requirement: REQ_TAG(PDK-5885) REQ_TAG(PDK-5889)
 * Design: did_csl_tog_interfaces did_csl_stog_get_error_information
 */
int32_t CSL_slvTogAckIntr( CSL_ksbus_vbusm_to_gasketRegs *pRegs, CSL_SlvTogIntrSrc intrSrc, uint32_t ackCnt )
{
    int32_t  retVal = CSL_EFAIL;

    if( (pRegs == NULL_PTR) || (ackCnt <= 0U)
        || ((intrSrc != CSL_SLV_TOG_INTRSRC_UNEXPECTED_RESPONSE)
            && (intrSrc != CSL_SLV_TOG_INTRSRC_TRANSACTION_TIMEOUT)))
    {
        retVal = CSL_EBADARGS;
    }
    else
    {
        uint32_t pendingIntrCnt = 0U;

        if( intrSrc == CSL_SLV_TOG_INTRSRC_TRANSACTION_TIMEOUT )
        {
            CSL_slvTogGetIntrCountInternal( pRegs, intrSrc, &pendingIntrCnt );
            if( ackCnt <= pendingIntrCnt )
            {
                CSL_REG32_WR( &pRegs->ERR_TM_INFO, CSL_FMK( KSBUS_VBUSM_TO_GASKET_ERR_TM_INFO_CNT, ackCnt ) );
                retVal = CSL_PASS;
            }
        }
        else if( intrSrc == CSL_SLV_TOG_INTRSRC_UNEXPECTED_RESPONSE )
        {
            CSL_slvTogGetIntrCountInternal( pRegs, intrSrc, &pendingIntrCnt );
            if( ackCnt <= pendingIntrCnt )
            {
                CSL_REG32_WR( &pRegs->ERR_UN_INFO, CSL_FMK( KSBUS_VBUSM_TO_GASKET_ERR_UN_INFO_CNT, ackCnt ) );
                retVal = CSL_PASS;
            }
        }
        else
        {
             ; /* No Action required */
        }
    }
    return retVal;
}

/**
 * Requirement: REQ_TAG(PDK-5894)
 * Design: did_csl_tog_interfaces did_csl_stog_cfg
 */
int32_t CSL_slvTogSetTimeoutVal( CSL_ksbus_vbusm_to_gasketRegs *pRegs, uint32_t timeoutVal )
{
    int32_t retVal = CSL_EBADARGS;

    if( (pRegs != NULL_PTR ) && (timeoutVal <= CSL_KSBUS_VBUSM_TO_GASKET_TIMEOUT_TO_MAX) )
    {
        CSL_REG32_WR( &pRegs->TIMEOUT, CSL_FMK( KSBUS_VBUSM_TO_GASKET_TIMEOUT_TO, timeoutVal ) );
        retVal = CSL_PASS;
    }
    return retVal;
}

/**
 * Requirement: REQ_TAG(PDK-5891)
 * Design: did_csl_tog_interfaces did_csl_stog_wr_read_back
 */
int32_t CSL_slvTogVerifyTimeoutVal( const CSL_ksbus_vbusm_to_gasketRegs *pRegs,
                                    uint32_t timeoutVal )
{
    int32_t retVal = CSL_EBADARGS;
    uint32_t readTimeoutVal;

    if( pRegs != NULL_PTR )
    {
        readTimeoutVal = pRegs->TIMEOUT;
        readTimeoutVal = CSL_FEXT(readTimeoutVal, KSBUS_VBUSM_TO_GASKET_TIMEOUT_TO);
        if (timeoutVal == readTimeoutVal)
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
 * Requirement: REQ_TAG(PDK-5892)
 * Design: did_csl_tog_interfaces did_csl_stog_control
 */
int32_t CSL_slvTogStart( CSL_ksbus_vbusm_to_gasketRegs *pRegs )
{
    int32_t retVal = CSL_EBADARGS;

    if( pRegs != NULL_PTR )
    {
        CSL_REG32_WR( &pRegs->ENABLE, CSL_FMK( KSBUS_VBUSM_TO_GASKET_ENABLE_EN, CSL_SLV_TOG_ENABLE_KEY ) );
        retVal = CSL_PASS;
    }
    return retVal;
}

static void CSL_slvTogStopLocal( CSL_ksbus_vbusm_to_gasketRegs *pRegs )
{

        CSL_REG32_WR( &pRegs->ENABLE, CSL_FMK( KSBUS_VBUSM_TO_GASKET_ENABLE_EN, 0U ) );
}

/**
 * Requirement: REQ_TAG(PDK-5892)
 * Design: did_csl_tog_interfaces did_csl_stog_control
 */
int32_t CSL_slvTogStop( CSL_ksbus_vbusm_to_gasketRegs *pRegs )
{
    int32_t retVal = CSL_EBADARGS;

    if( pRegs != NULL_PTR )
    {
        CSL_slvTogStopLocal(pRegs);
        retVal = CSL_PASS;
    }
    return retVal;
}

/**
 * Requirement: REQ_TAG(PDK-5892)
 * Design: did_csl_tog_interfaces did_csl_stog_control
 */
int32_t CSL_slvTogReset( CSL_ksbus_vbusm_to_gasketRegs *pRegs )
{
    int32_t retVal = CSL_EBADARGS;

    if( pRegs != NULL_PTR )
    {
        /* Stop timer */
        CSL_slvTogStopLocal( pRegs );
        /* Reset timer counter and eon to 0 */
        CSL_REG32_FINS( &pRegs->TIMER, KSBUS_VBUSM_TO_GASKET_TIMER_CNTR, 0U );
        retVal = CSL_PASS;
    }
    return retVal;
}

/**
 * Requirement: REQ_TAG(PDK-5885)
 * Design: did_csl_tog_interfaces did_csl_stog_get_error_information
 */
int32_t CSL_slvTogGetCurrTimerCnt( const CSL_ksbus_vbusm_to_gasketRegs *pRegs, uint32_t *pTimerCnt )
{
    int32_t retVal = CSL_EBADARGS;

    if( (pRegs != NULL_PTR) && (pTimerCnt != NULL_PTR) )
    {
        *pTimerCnt = CSL_REG32_RD( &pRegs->TIMER );
        retVal = CSL_PASS;
    }
    return retVal;
}

/**
 * Requirement: REQ_TAG(PDK-5894)
 * Design: did_csl_tog_interfaces did_csl_stog_cfg
 */
int32_t CSL_slvTogSetFlushModeEnable( CSL_ksbus_vbusm_to_gasketRegs *pRegs, bool bEnable )
{
    int32_t retVal = CSL_EBADARGS;

    if( pRegs != NULL_PTR )
    {
        if( bEnable == (bool)true )
        {
            CSL_REG32_WR( &pRegs->FLUSH, CSL_FMK( KSBUS_VBUSM_TO_GASKET_FLUSH_FL, CSL_SLV_TOG_FLUSH_MODE_KEY ) );
        }
        else
        {
            CSL_REG32_WR( &pRegs->FLUSH, CSL_FMK( KSBUS_VBUSM_TO_GASKET_FLUSH_FL, 0U ) );
        }
        retVal = CSL_PASS;
    }
    return retVal;
}

/**
 * Requirement: REQ_TAG(PDK-5891)
 * Design: did_csl_tog_interfaces did_csl_stog_wr_read_back
 */
int32_t CSL_slvTogVerifyFlushModeEnable( const CSL_ksbus_vbusm_to_gasketRegs *pRegs, bool bEnable )
{
    int32_t retVal = CSL_EBADARGS;
    uint32_t readVal;

    if( pRegs != NULL_PTR )
    {
        retVal = CSL_EFAIL;
        readVal = pRegs->FLUSH;
        readVal = CSL_FEXT(readVal, KSBUS_VBUSM_TO_GASKET_FLUSH_FL);
        if( ((bEnable == (bool)true) && (readVal == CSL_SLV_TOG_FLUSH_MODE_KEY ))
            || ((bEnable == (bool)false) && (readVal == 0U )) )
        {
            retVal = CSL_PASS;
        }
    }
    return retVal;
}

/**
 * Requirement: REQ_TAG(PDK-5891)
 * Design: did_csl_tog_interfaces did_csl_tog_static_read_back
 */

int32_t CSL_slvTogReadBackStaticRegisters(const  CSL_ksbus_vbusm_to_gasketRegs *pRegs,
                                          CSL_SlvTog_staticRegs *pStaticRegs)
{
    int32_t retVal = CSL_EBADARGS;

    if( (pRegs != NULL_PTR) && (pStaticRegs != NULL_PTR) )
    {
        pStaticRegs->PID = pRegs->PID;
        pStaticRegs->CFG = pRegs->CFG;
        pStaticRegs->ENABLE = pRegs->ENABLE;
        pStaticRegs->FLUSH = pRegs->FLUSH;
        pStaticRegs->TIMEOUT = pRegs->TIMEOUT;

        retVal = CSL_PASS;
    }

    return retVal;

}
