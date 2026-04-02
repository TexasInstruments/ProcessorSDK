//=============================================================================
//  mlbss CSL-F API validation test
//
//  Description:
//      This program verifies correct operation of all API calls in the
//      mlbss CSL-F. It is intended to be built and run in the Borg test
//      environment, but can be ported to other environments as needed.
//
//  Notes:
//      An issue currently exists with software resetting the mlbss module.
//      This operation is currently commented out.
//
//  History:
//      07Jul15 BEGR: Initial implementation completed
//=============================================================================
#include <stdio.h>
#include <app_support.h>
#include <csl_osal.h>
#include "csl_mlbss_support.h"
#include <ti/csl/csl_mlb.h>
#include "_assert.h"
#include "../tb_config.h"

#define MLBSS_REGS_BASE_ADDRESS         0x20000000
#define MLBSS_REGS_SIZE                 0x400
#define ECC_AGGR_REGS_BASE_ADDRESS      (MLBSS_REGS_BASE_ADDRESS+0x400)
#define ECC_AGGR_REGS_SIZE              0x100
#define MLB_DIM_REGS_BASE_ADDRESS       (MLBSS_REGS_BASE_ADDRESS+0x800)
#define MLB_DIM_REGS_SIZE               0x400
#define MLBSS_RAT_REGS_BASE_ADDRESS     (MLBSS_REGS_BASE_ADDRESS+0x1000)
#define MLBSS_RAT_REGS_SIZE             0x1000

// Declare Register Overlay Structures and set base addresses
CSL_mlbssRegs *mlbssRegs            = (CSL_mlbssRegs *)MLBSS_REGS_BASE_ADDRESS;
CSL_mlbdimRegs *mlbdimRegs          = (CSL_mlbdimRegs *)MLB_DIM_REGS_BASE_ADDRESS;
CSL_ecc_aggrRegs *ecc_aggrRegs      = (CSL_ecc_aggrRegs *)ECC_AGGR_REGS_BASE_ADDRESS;
CSL_KSBUS_RATRegs *mlsbssRATRegs    = (CSL_KSBUS_RATRegs *)MLBSS_RAT_REGS_BASE_ADDRESS;

void TestReg( volatile uint32_t *pRegAddr, uint32_t ResetVal, uint32_t WriteVal )
{
    _assert( CSL_REG_READ( *pRegAddr ) == ResetVal );
    CSL_REG_WRITE( *pRegAddr, WriteVal );
    _assert( CSL_REG_READ( *pRegAddr ) == WriteVal );
    CSL_REG_WRITE( *pRegAddr, ResetVal );
    _assert( CSL_REG_READ( *pRegAddr ) == ResetVal );
}

void TestMmrRegionAccesses()
{
    CslOsal_Log( "Testing configuration accesses to Subsystem mmr region...\n" );
    TestReg( &mlbssRegs->DMA_CTRL, CSL_MLBSS_DMA_CTRL_RESETVAL, CSL_MLBSS_DMA_CTRL_PRIORITY_MAX );

    CslOsal_Log( "Testing configuration accesses to DIM mmr region...\n" );
    TestReg( &mlbdimRegs->MDAT0, CSL_MLBDIM_MDAT0_RESETVAL, CSL_MLBDIM_MDAT0_DATA_MAX );

    CslOsal_Log( "Testing configuration accesses to RAT mmr region...\n" );
    TestReg( &mlsbssRATRegs->BASE0, CSL_KSBUS_RAT_BASE0_RESETVAL, CSL_KSBUS_RAT_BASE0_BASE_MAX );

    CslOsal_Log( "Testing configuration accesses to ECC mmr region...\n" );
    TestReg( &ecc_aggrRegs->VECTOR, CSL_ECC_AGGR_VECTOR_RESETVAL, CSL_ECC_AGGR_VECTOR_ECC_VECTOR_MAX );
}

void TestDimRamAccesses()
{
    // Initialize WriteData for ASYNCHRONOUS/CONTROL CDT ENTRY FORMAT (all writeable bits are set)
    uint32_t WriteData[4] = {
        (0x1F<<(16+11)) | (0x1F<<11),   // bits 31:0
        (0x7<<(16+12)) | (0x7<<12),     // bits 63:32
        0xFFFFFFFF,                     // bits 95:64
        0x3FFFCFFF                      // bits 127:96
    };
    uint32_t ReadData[4];
    int i;

    CslOsal_Log( "Testing accesses to DIM CTR ram...\n" );
    CSL_MLBSS_setCtrEntry( 0, NULL, WriteData );
    CSL_MLBSS_getCtrEntry( 0, ReadData );
    for( i=0; i<4; i++ )
        _assert( ReadData[i] == WriteData[i] );

    CslOsal_Log( "Testing accesses to DIM DBR ram...\n" );
    CSL_MLBSS_writeDbr8( 0, 0xFF );
    _assert( CSL_MLBSS_readDbr8( 0 ) == 0xFF );
}

//=============================================================================
//  TestMifCsl
//
//  This function tests the following MIF CSL-F functions and macros:
//      void CSL_MLBSS_clearCtr( void );
//      void CSL_MLBSS_setCtrEntry( unsigned CtrAddr, uint32_t *pMask, uint32_t *pData );
//      void CSL_MLBSS_getCtrEntry( unsigned CtrAddr, uint32_t *pData );
//      void CSL_MLBSS_writeDbr8( unsigned ByteAddr, uint8_t data );
//      uint8_t CSL_MLBSS_readDbr8( unsigned ByteAddr );
//      uint32_t mlbss_getCtrField( unsigned CtrAddr, unsigned FieldWidth, unsigned FieldStart );
//      int mlbss_setCtrField( unsigned CtrAddr, unsigned FieldWidth, unsigned FieldStart, uint32_t value );
//      #define CSL_MLBSS_clearCtrEntry(CtrAddr) CSL_MLBSS_setCtrEntry( CtrAddr, NULL, NULL )
//=============================================================================
void TestMifCsl()
{
    uint32_t WrData[4]  = { 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu };
    uint32_t AdtMask[4] = { 0xFFFF7FFFu, 0x87FF87FFu, 0xFFFFFFFFu, 0xFFFFFFFFu };   // Don't write to bits 15 (CE), 43-46 (MEP1, PS1, ERR1, DNE1), 59-62 (MEP2, PS2, ERR2, DNE2)
    uint32_t CatMask[4] = { 0xF7FFF7FFu, 0xF7FFF7FFu, 0xF7FFF7FFu, 0xF7FFF7FFu };   // Don't write to bits 11, 27, 43, 59, 75, 91, 107, 123 (CE)
    uint32_t RdData[4];
    unsigned CtrAddr, ByteAddr;
    int i;

    CslOsal_Log( "Testing MIF CSL functions...\n" );
    //-------------------------------------------------------------------------
    //  CSL_MLBSS_setCtrEntry / CSL_MLBSS_getCtrEntry (CDT)
    //-------------------------------------------------------------------------
    CslOsal_Log( "  Testing CSL_MLBSS_setCtrEntry / CSL_MLBSS_getCtrEntry (CDT)...\n" );
    for( CtrAddr = 0; CtrAddr < 0x40; CtrAddr++ )
        CSL_MLBSS_setCtrEntry( CtrAddr, NULL, WrData );
    for( CtrAddr = 0; CtrAddr < 0x40; CtrAddr++ )
    {
        CSL_MLBSS_getCtrEntry( CtrAddr, RdData );
        for( i=0; i<4; i++ )
            _assert( RdData[i] == WrData[i] );
    }
    //-------------------------------------------------------------------------
    //  CSL_MLBSS_setCtrEntry / CSL_MLBSS_getCtrEntry (ADT)
    //-------------------------------------------------------------------------
    CslOsal_Log( "  Testing CSL_MLBSS_setCtrEntry / CSL_MLBSS_getCtrEntry (ADT)...\n" );
    for( CtrAddr = 0x40; CtrAddr < 0x80; CtrAddr++ )
        CSL_MLBSS_setCtrEntry( CtrAddr, AdtMask, WrData );
    for( CtrAddr = 0x40; CtrAddr < 0x80; CtrAddr++ )
    {
        CSL_MLBSS_getCtrEntry( CtrAddr, RdData );
        for( i=0; i<4; i++ )
            _assert( RdData[i] == AdtMask[i] );
    }
    //-------------------------------------------------------------------------
    //  CSL_MLBSS_setCtrEntry / CSL_MLBSS_getCtrEntry (CAT)
    //-------------------------------------------------------------------------
    CslOsal_Log( "  Testing CSL_MLBSS_setCtrEntry / CSL_MLBSS_getCtrEntry (CAT)...\n" );
    for( CtrAddr = 0x80; CtrAddr < 0x90; CtrAddr++ )
        CSL_MLBSS_setCtrEntry( CtrAddr, CatMask, WrData );
    for( CtrAddr = 0x80; CtrAddr < 0x90; CtrAddr++ )
    {
        CSL_MLBSS_getCtrEntry( CtrAddr, RdData );
        for( i=0; i<4; i++ )
            _assert( RdData[i] == CatMask[i] );
    }
    //-------------------------------------------------------------------------
    //  CSL_MLBSS_clearCtrEntry
    //-------------------------------------------------------------------------
    CslOsal_Log( "  Testing CSL_MLBSS_clearCtrEntry...\n" );
    CSL_MLBSS_clearCtrEntry( 0x24 );
    CSL_MLBSS_getCtrEntry( 0x24, RdData );
    for( i=0; i<4; i++ )
        _assert( RdData[i] == 0 );
    //-------------------------------------------------------------------------
    //  CSL_MLBSS_writeDbr8, CSL_MLBSS_readDbr8
    //-------------------------------------------------------------------------
    CslOsal_Log( "  Testing CSL_MLBSS_writeDbr8, CSL_MLBSS_readDbr8...\n" );
    for( ByteAddr=1; ByteAddr <= (16*1024); ByteAddr*=2 )
    {
        CSL_MLBSS_writeDbr8( ByteAddr-1, (ByteAddr-1)&0xFF );
        if( ByteAddr < (16*1024) )
        {
            CSL_MLBSS_writeDbr8( ByteAddr, (ByteAddr)&0xFF );
            CSL_MLBSS_writeDbr8( ByteAddr+1, (ByteAddr+1)&0xFF );
        }
    }
    for( ByteAddr=1; ByteAddr <= (16*1024); ByteAddr*=2 )
    {
        _assert( CSL_MLBSS_readDbr8( ByteAddr-1 ) == ((ByteAddr-1)&0xFF) );
        if( ByteAddr < (16*1024) )
        {
            _assert( CSL_MLBSS_readDbr8( ByteAddr ) == ((ByteAddr)&0xFF) );
            _assert( CSL_MLBSS_readDbr8( ByteAddr+1 ) ==  ((ByteAddr+1)&0xFF) );
        }
    }
    //-------------------------------------------------------------------------
    //  mlbss_setCtrField, mlbss_getCtrField
    //-------------------------------------------------------------------------
    CslOsal_Log( "  Testing mlbss_setCtrField, mlbss_getCtrField...\n" );
    // Verify error return for invalid arguments
    _assert( mlbss_setCtrField( 0x90, 1, 0, 0 ) == ((int)-1) );
    _assert( mlbss_setCtrField( 0, 0, 0, 0 ) == ((int)-1) );
    _assert( mlbss_setCtrField( 0, 33, 0, 0 ) == ((int)-1) );
    _assert( mlbss_setCtrField( 0, 1, 128, 0 ) == ((int)-1) );
    _assert( mlbss_getCtrField( 0x90, 1, 0 ) == ((uint32_t)-1) );
    _assert( mlbss_getCtrField( 0, 0, 0 ) == ((uint32_t)-1) );
    _assert( mlbss_getCtrField( 0, 33, 0 ) == ((uint32_t)-1) );
    _assert( mlbss_getCtrField( 0, 1, 128 ) == ((uint32_t)-1) );
    // Write/reads within a single 32-bit word
    CSL_MLBSS_clearCtrEntry( 0x3D );
    mlbss_setCtrField( 0x3D, 32, 0, 0xFFFFFFFFu );
    mlbss_setCtrField( 0x3D, 1, 0, 0 );
    mlbss_setCtrField( 0x3D, 1, 31, 0 );
    mlbss_setCtrField( 0x3D, 30, 1, 0xFFFFFFFFu );
    CSL_MLBSS_getCtrEntry( 0x3D, RdData );
    _assert( RdData[0] == 0x7FFFFFFEu );
    _assert( mlbss_getCtrField( 0x3D, 32, 0 ) == 0x7FFFFFFEu );
    _assert( mlbss_getCtrField( 0x3D, 1, 0 ) == 0 );
    _assert( mlbss_getCtrField( 0x3D, 1, 31 ) == 0 );
    _assert( mlbss_getCtrField( 0x3D, 30, 1 ) == 0x3FFFFFFFu );
    // Write/reads across words
    CSL_MLBSS_clearCtrEntry( 0x3D );
    mlbss_setCtrField( 0x3D, 2, 31, 0xFFFFFFFFu );
    _assert( mlbss_getCtrField( 0x3D, 2, 31 ) == 0x3u );
    _assert( mlbss_getCtrField( 0x3D, 31, 0 ) == 0 );
    _assert( mlbss_getCtrField( 0x3D, 31, 33 ) == 0 );
    CSL_MLBSS_clearCtrEntry( 0x3D );
    mlbss_setCtrField( 0x3D, 32, 1, 0xFFFFFFFFu );
    _assert( mlbss_getCtrField( 0x3D, 32, 1 ) == 0xFFFFFFFFu );
    _assert( mlbss_getCtrField( 0x3D, 1, 0 ) == 0 );
    _assert( mlbss_getCtrField( 0x3D, 31, 33 ) == 0 );
    CSL_MLBSS_clearCtrEntry( 0x3D );
    mlbss_setCtrField( 0x3D, 32, 31, 0xFFFFFFFFu );
    _assert( mlbss_getCtrField( 0x3D, 32, 31 ) == 0xFFFFFFFFu );
    _assert( mlbss_getCtrField( 0x3D, 31, 0 ) == 0 );
    _assert( mlbss_getCtrField( 0x3D, 1, 63 ) == 0 );
    //-------------------------------------------------------------------------
    //  CSL_MLBSS_clearCtr
    //-------------------------------------------------------------------------
    CslOsal_Log( "  Testing CSL_MLBSS_clearCtr...\n" );
    CSL_MLBSS_clearCtr();
    for( CtrAddr = 0; CtrAddr < 0x90; CtrAddr++ )
    {
        CSL_MLBSS_getCtrEntry( CtrAddr, RdData );
        for( i=0; i<4; i++ )
            _assert( RdData[i] == 0 );
    }
}

//=============================================================================
//  TestCdtCsl
//
//  This function tests the following CDT CSL-F functions and macros:
//      int mlbss_configCdt( unsigned LogChnNum, unsigned DbrBuffAddr, unsigned DbrBuffLen );
//=============================================================================
void TestCdtCsl()
{
    unsigned LogChnNum;
    uint32_t ReadData[4];

    CslOsal_Log( "Testing CDT CSL functions...\n" );
    _assert( mlbss_configCdt( MLBSS_MAX_CHNS, 0, 0 ) == 1 );               // Invalid channel #
    _assert( mlbss_configCdt( 0, 0, 0 ) == 1 );                            // Invalid DbrBuffLen (must be > 0)
    _assert( mlbss_configCdt( 0, 0x4000, 1 ) == 1 );                       // DbrBuffAddr+DbrBuffLen <= 0x4000
    for( LogChnNum = 0; LogChnNum < MLBSS_MAX_CHNS; LogChnNum++ )
        _assert( mlbss_configCdt( LogChnNum, LogChnNum, LogChnNum+1 ) == 0 );
    for( LogChnNum = 0; LogChnNum < MLBSS_MAX_CHNS; LogChnNum++ )
    {
        CSL_MLBSS_getCtrEntry( LogChnNum, ReadData );
        _assert( ReadData[0] == 0 );
        _assert( ReadData[1] == 0 );
        _assert( ReadData[2] == 0 );
        _assert( ReadData[3] == ((LogChnNum << 16) | LogChnNum) );
    }
}

//=============================================================================
//  TestCatCsl
//
//  This function tests the following CDT CSL-F functions and macros:
//      int mlbss_configCat( unsigned LogChnNum, MLBSS_CHN_TYPE_E ChnType, MLBSS_CHN_DIR_E Dir, bool bEnable, bool bMute, bool bFCE, bool bMFE );
//      void CSL_MLBSS_clearCat( void );
//=============================================================================
void TestCatCsl()
{
    //  mlbss_configCat( unsigned LogChnNum, MLBSS_CHN_TYPE_E ChnType, MLBSS_CHN_DIR_E Dir, bool bEnable, bool bMute, bool bFCE, bool bMFE );
    //      LogChnNum       0-MLBSS_MAX_CHNS
    //      ChnType         0-3
    //      Dir             0,1
    //      bEnable         0,1
    //      bMute           0,1 valid for ChnType: MLBSS_CHN_TYPE_SYNC, MLBSS_CHN_TYPE_CTL, MLBSS_CHN_TYPE_ASYNC,
    //      bFCE            0,1 valid for ChnType: MLBSS_CHN_TYPE_ISO
    //      bMFE            0,1 valid for ChnType: MLBSS_CHN_TYPE_SYNC
    unsigned LogChnNum, CtrEntry;
    MLBSS_CHN_TYPE_E ChnType;
    MLBSS_CHN_DIR_E Dir;
    bool bEnable, bRNW, bMute, bFCE, bMFE;
    uint16_t CatData[8], ExpVal;
    uint32_t CtrData[4];
    int i;

    CslOsal_Log( "Testing CAT CSL functions...\n" );
    _assert( mlbss_configCat( MLBSS_MAX_CHNS, MLBSS_CHN_TYPE_SYNC, MLBSS_CHN_DIR_RX, false, false, false, false ) == 1 );
    _assert( mlbss_configCat( 0, MLBSS_CHN_TYPE_MAX, MLBSS_CHN_DIR_RX, false, false, false, false ) == 1 );
    _assert( mlbss_configCat( 0, MLBSS_CHN_TYPE_SYNC, (MLBSS_CHN_DIR_E)2, false, false, false, false ) == 1 );
    CSL_MLBSS_clearCat();     // Clear all CAT entries
    //-------------------------------------------------------------------------
    //  Configure CAT entries
    //-------------------------------------------------------------------------
    for( LogChnNum=0; LogChnNum<MLBSS_MAX_CHNS; LogChnNum++ )
    {
        ChnType = (MLBSS_CHN_TYPE_E)(LogChnNum & 0x03);
        Dir     = (MLBSS_CHN_DIR_E)(!(LogChnNum & 0x01));
        bEnable = (bool)((LogChnNum & 0x02) != 0);
        bMute   = (bool)((LogChnNum & 0x04) != 0);
        bFCE    = (bool)((LogChnNum & 0x08) != 0);
        bMFE    = (bool)((LogChnNum & 0x10) != 0);
        _assert( mlbss_configCat( LogChnNum, ChnType, Dir, bEnable, bMute, bFCE, bMFE ) == 0 );
    }
    //-------------------------------------------------------------------------
    //  Verify CAT entries
    //-------------------------------------------------------------------------
    for( CtrEntry=0x80; CtrEntry<0x90; CtrEntry++ )
    {
        CSL_MLBSS_getCtrEntry( CtrEntry, (uint32_t *)CatData );    // Read 8 CAT entries from a single CTR entry
        for( i=0; i<8; i++ )
        {
            //-----------------------------------------------------------------
            //  Determine the LogChnNum and from this the value of the
            //  other CAT fields
            //-----------------------------------------------------------------
            LogChnNum = (((CtrEntry-0x80)*8) & 0x3F) + i;              // Determine the channel number
            ChnType = (MLBSS_CHN_TYPE_E)(LogChnNum & 0x03);
            Dir     = (MLBSS_CHN_DIR_E)(!(LogChnNum & 0x01));
            bEnable = (bool)((LogChnNum & 0x02) != 0);
            bMute   = (ChnType == MLBSS_CHN_TYPE_ISO) ? false : (bool)((LogChnNum & 0x04) != 0);
            bFCE    = (ChnType == MLBSS_CHN_TYPE_ISO && Dir == MLBSS_CHN_DIR_RX) ? (bool)((LogChnNum & 0x08) != 0) : false;
            bMFE    = (ChnType == MLBSS_CHN_TYPE_SYNC) ? (bool)((LogChnNum & 0x10) != 0) : false;
            bRNW    = (CtrEntry < 0x88) ? (Dir==MLBSS_CHN_DIR_TX) : (Dir==MLBSS_CHN_DIR_RX);
            ExpVal  = (bFCE<<14) | (bMFE<<14) | (bMute<<13) | (bRNW << 12) | (bEnable<<11) | (ChnType<<8) | LogChnNum;
#ifdef DEBUG
            CslOsal_Log( "CtrEntry=0x%x, i=%d, LogChnNum=%d, ChnType=%d, Dir=%d, bEnable=%d, bRNW=%d, bMute=%d, bFCE=%d, bMFE=%d, ExpVal=0x%04x, ActVal=0x%04x\n",
                CtrEntry, i, LogChnNum, ChnType, Dir, bEnable, bRNW, bMute, bFCE, bMFE, ExpVal, CatData[i] );
#endif
            _assert( CatData[i] == ExpVal );
        }
    }
    //-------------------------------------------------------------------------
    //  Test clearing of all CAT entries
    //-------------------------------------------------------------------------
    CSL_MLBSS_clearCat();     // Clear all CAT entries
    for( CtrEntry=0x80; CtrEntry<0x90; CtrEntry++ )
    {
        CSL_MLBSS_getCtrEntry( CtrEntry, CtrData );
        for( i=0; i<4; i++ )
            _assert( CtrData[i] == 0 );
    }
}

//=============================================================================
//  TestAdtCsl
//
//  This function tests the following ADT CSL-F functions and macros:
//      int mlbss_configAdt( unsigned LogChnNum, MLBSS_CHN_CFG_T *pChn );
//      mlbss_clrAdtBuffErr(lcn,page)
//      mlbss_clrAdtBuffDone(lcn,page)
//      mlbss_clrAdtBuffMEP(lcn,page)
//      mlbss_getAdtBuffPagePtr(lcn)
//      mlbss_getChnEndian(lcn)
//      mlbss_getAdtBuffDepth(lcn,page)
//      mlbss_getAdtBuffAddr(lcn,page)
//      mlbss_isAdtBuffMEP(lcn,page)
//      mlbss_isAdtBuffRdy(lcn,page)
//      mlbss_isAdtBuffDone(lcn,page)
//      mlbss_isAdtBuffErr(lcn,page)
//      mlbss_isAdtBuffPktStart(lcn,page)
//      mlbss_isChnEnabled(lcn)
//      mlbss_setAdtBuffRdy(lcn,page,v)
//      mlbss_setChnEnable(lcn,v)
//      mlbss_setAdtBuffPagePtr(lcn,v)
//      mlbss_setChnEndian(lcn,v)
//      mlbss_setAdtBuffPktStart(lcn,page,v)
//      mlbss_setAdtBuffAddr(lcn,page,v)
//      mlbss_setAdtBuffDepth(lcn,page,v)
//=============================================================================
typedef struct
{
    unsigned ba1;
    unsigned ba2;
    unsigned bd1;
    unsigned bd2;
    unsigned pg;
    unsigned ce;
    unsigned le;
    unsigned rdy1;
    unsigned rdy2;
    unsigned ps1;
    unsigned ps2;
} Adt_TestData;

void _TestAdtWriteReadVerify( unsigned LogChnNum, Adt_TestData *pData )
{
    MLBSS_CHN_SYS_BUFF_T    SysBuff;
    MLBSS_CHN_CFG_T         ChnCfg;
    uint32_t                AdtData[4];

    //-------------------------------------------------------------------------
    //  Write ADT entry with specified data
    //-------------------------------------------------------------------------
    ChnCfg.pSysBuff = &SysBuff;
    SysBuff.bLittleEndian   = pData->le;
    SysBuff.BuffAddr[0]     = pData->ba1;
    SysBuff.BuffAddr[1]     = pData->ba2;
    SysBuff.BuffDepth[0]    = pData->bd1 + 1;
    SysBuff.BuffDepth[1]    = pData->bd2 + 1;
    _assert( mlbss_configAdt( LogChnNum, &ChnCfg ) == 0 );
    mlbss_setAdtBuffPagePtr( LogChnNum, pData->pg );
    mlbss_setChnEnable( LogChnNum, pData->ce );
    mlbss_setChnEndian( LogChnNum, pData->le );
    mlbss_setAdtBuffRdy( LogChnNum, 0, pData->rdy1 );
    mlbss_setAdtBuffPktStart( LogChnNum, 0, pData->ps1 );
    mlbss_setAdtBuffRdy( LogChnNum, 1, pData->rdy2 );
    mlbss_setAdtBuffPktStart( LogChnNum, 1, pData->ps2 );
    //-------------------------------------------------------------------------
    //  Read ADT entry and compare with expected data
    //-------------------------------------------------------------------------
    CSL_MLBSS_getCtrEntry( 0x40 + LogChnNum, AdtData );
#ifdef DEBUG
    CslOsal_Log( "ba1=0x%08x, ba2=0x%08x, bd1=0x%04x, bd2=0x%04x, pg=%d, ce=%d, le=%d, rdy1=%d, rdy2=%d, ps1=%d, ps2=%d\n",
        pData->ba1, pData->ba2, pData->bd1, pData->bd2, pData->pg, pData->ce, pData->le, pData->rdy1, pData->rdy2, pData->ps1, pData->ps2 );
    CslOsal_Log( "AdtData[0]=0x%08x, AdtData[1]=0x%08x, AdtData[2]=0x%08x, AdtData[3]=0x%08x\n",
        AdtData[0], AdtData[1], AdtData[2], AdtData[3] );
#endif
    _assert( AdtData[0] == ((pData->ce<<15) | (pData->le<<14) | (pData->pg<<13)) );
    _assert( AdtData[1] == ((pData->rdy2<<31) | (pData->ps2<<28) | (pData->bd2<<16) | (pData->rdy1<<15) | (pData->ps1<<12) | (pData->bd1<<0)) );
    _assert( AdtData[2] == pData->ba1 );
    _assert( AdtData[3] == pData->ba2 );
    _assert( mlbss_getAdtBuffPagePtr(LogChnNum) == pData->pg );
    _assert( mlbss_getChnEndian(LogChnNum) == pData->le );
    _assert( mlbss_getAdtBuffDepth(LogChnNum,0,11) == pData->bd1 );
    _assert( mlbss_getAdtBuffDepth(LogChnNum,1,11) == pData->bd2 );
    _assert( mlbss_getAdtBuffAddr(LogChnNum,0) == pData->ba1 );
    _assert( mlbss_getAdtBuffAddr(LogChnNum,1) == pData->ba2 );
    _assert( mlbss_isAdtBuffMEP(LogChnNum,0) == 0 );
    _assert( mlbss_isAdtBuffMEP(LogChnNum,1) == 0 );
    _assert( mlbss_isAdtBuffRdy(LogChnNum,0) == pData->rdy1 );
    _assert( mlbss_isAdtBuffRdy(LogChnNum,1) == pData->rdy2 );
    _assert( mlbss_isAdtBuffDone(LogChnNum,0) == 0 );
    _assert( mlbss_isAdtBuffDone(LogChnNum,1) == 0 );
    _assert( mlbss_isAdtBuffErr(LogChnNum,0) == 0 );
    _assert( mlbss_isAdtBuffErr(LogChnNum,1) == 0 );
    _assert( mlbss_isAdtBuffPktStart(LogChnNum,0) == pData->ps1 );
    _assert( mlbss_isAdtBuffPktStart(LogChnNum,1) == pData->ps2 );
    _assert( mlbss_isChnEnabled(LogChnNum) == pData->ce );
}

void TestAdtCsl()
{
    MLBSS_CHN_SYS_BUFF_T    SysBuff;
    MLBSS_CHN_CFG_T         ChnCfg;
    Adt_TestData            Chn0Data  = { 0x55555555, 0xAAAAAAAA, 0x333, 0x4CC, 0, 0, 0, 0, 1, 0, 1 };
    Adt_TestData            Chn63Data = { 0xAAAAAAAA, 0x55555555, 0x4CC, 0x333, 1, 1, 1, 1, 0, 1, 0 };

    CslOsal_Log( "Testing ADT CSL functions...\n" );
    ChnCfg.pSysBuff = &SysBuff;
    //-------------------------------------------------------------------------
    //  Verify error return for invalid arguments
    //-------------------------------------------------------------------------
    _assert( mlbss_configAdt( MLBSS_MAX_CHNS, &ChnCfg ) == 1 );
    _assert( mlbss_configAdt( 0, NULL ) == 1 );
    SysBuff.BuffDepth[0] = 0;
    SysBuff.BuffDepth[1] = 1;
    _assert( mlbss_configAdt( 0, &ChnCfg ) == 1 );
    SysBuff.BuffDepth[0] = 1;
    SysBuff.BuffDepth[1] = 0;
    _assert( mlbss_configAdt( 0, &ChnCfg ) == 1 );
    //-------------------------------------------------------------------------
    //  Write/Read/Verify ADT data
    //-------------------------------------------------------------------------
    CSL_MLBSS_clearCtr();
    _TestAdtWriteReadVerify( 0, &Chn0Data );
    _TestAdtWriteReadVerify( 63, &Chn63Data );
}

//=============================================================================
//  TestRatCsl
//
//  This function tests the following RAT CSL-F functions and macros:
//      int mlbss_addRatRegion( unsigned RegionNum, bool bEnable, uint32_t AhbAddr, unsigned SizeBytes, uint64_t VbusAddr );
//      int mlbss_delRatRegion( unsigned RegionNum );
//      int mlbss_enableRatRegion( unsigned RegionNum, bool bEnable );
//      int mlbss_getRatRegion( unsigned RegionNum, bool *pbEnable, uint32_t *pAhbAddr, unsigned *pSizeBytes, uint64_t *pVbusAddr );
//=============================================================================
typedef struct
{
    unsigned InVal;
    unsigned ExpVal;
} REGION_SIZE_CHECK_T;

static void _TestRegionSizeConversion()
{
    unsigned i, RegionSize;
    REGION_SIZE_CHECK_T RegionSizeCheck[] =
    {
        {     0,   1024 },      // Passed size of     0 should return    1024 (minimum size)
        {  1023,   1024 },      // Passed size of  1023 should return    1024 (minimum size)
        {  1024,   1024 },      // Passed size of  1024 should return    1024 (minimum size)
        {  1025,   2048 },      // Passed size of  1025 should return    2048
        {  2048,   2048 },      // Passed size of  2048 should return    2048
        {  2049,   4096 },      // Passed size of  2049 should return    4096
        { 65535,  65536 },      // Passed size of 65535 should return   65536
        { 65536,  65536 },      // Passed size of 65536 should return   65536
        { 65537, 131072 }       // Passed size of 65537 should return  131072
    };

    for( i = 0; i < sizeof(RegionSizeCheck)/sizeof(REGION_SIZE_CHECK_T); i++ )
    {
        _assert( mlbss_addRatRegion( i, false, 0, RegionSizeCheck[i].InVal, 0 ) == 0 );
        _assert( mlbss_getRatRegion( i, NULL, NULL, &RegionSize, NULL ) == 0 );
        _assert( RegionSize == RegionSizeCheck[i].ExpVal );
    }
    for( i = 0; i < sizeof(RegionSizeCheck)/sizeof(REGION_SIZE_CHECK_T); i++ )
        _assert( mlbss_delRatRegion(i) == 0 );
}

static void _TestRatRegion( unsigned RegionNum )
{
    bool        bEnable     = false;
    uint32_t    AhbAddr     = 0x01234567U + RegionNum;
    unsigned    SizeBytes   = 0x4000 + RegionNum;
    uint64_t    VbusAddr    = 0x89abcdefUL + RegionNum;

    _assert( mlbss_addRatRegion(RegionNum,bEnable,AhbAddr,SizeBytes,VbusAddr) == 0 );       // Add region
    _assert( mlbss_addRatRegion(RegionNum,bEnable,AhbAddr,SizeBytes,VbusAddr) == 2 );       // Region Already Exists
    _assert( mlbss_enableRatRegion(RegionNum,true) == 0 );                                  // Enable the region
    _assert( mlbss_getRatRegion(RegionNum,&bEnable,&AhbAddr,&SizeBytes,&VbusAddr) == 0 );   // Get region information
    _assert( bEnable == true );                                                             //   and verify values
    _assert( AhbAddr == (0x01234567U + RegionNum) );
    _assert( VbusAddr == (0x89abcdefUL + RegionNum) );
    _assert( mlbss_delRatRegion(RegionNum) == 0 );                                          // Delete the region
    _assert( mlbss_delRatRegion(RegionNum) == 2 );                                          // Region Does Not Exist
}

void TestRatCsl()
{
    CslOsal_Log( "Testing RAT CSL functions...\n" );
    //-------------------------------------------------------------------------
    //  Verify error return for invalid arguments
    //-------------------------------------------------------------------------
    _assert( mlbss_addRatRegion(64,false,0,0,0) == 1 );         // RegionNum Invalid
    _assert( mlbss_delRatRegion(64) == 1 );                     // RegionNum Invalid
    _assert( mlbss_enableRatRegion(64,false) == 1 );            // RegionNum Invalid
    _assert( mlbss_getRatRegion(64,NULL,NULL,NULL,NULL) == 1 ); // RegionNum Invalid
    _assert( mlbss_delRatRegion(0) == 2 );                      // Region Does Not Exist
    _assert( mlbss_enableRatRegion(0, false) == 2 );            // Region Does Not Exist
    _assert( mlbss_getRatRegion(0,NULL,NULL,NULL,NULL) == 2 );  // Region Does Not Exist
    //-------------------------------------------------------------------------
    //  Test region size conversion
    //-------------------------------------------------------------------------
    _TestRegionSizeConversion();
    //-------------------------------------------------------------------------
    //  Test a couple of RAT regions
    //-------------------------------------------------------------------------
    _TestRatRegion(0);
    _TestRatRegion(63);
}

//=============================================================================
//  TestSsCsl
//
//  This function tests the following SS CSL-F functions and macros:
//      void        CSL_MLBSS_configSubSystem( bool bEnableFastClkStop, unsigned DmaVbusPriority );
//      uint32_t    mlbss_ssGetRev( void );
//      bool        CSL_MLBSS_isResetInProgress( void );
//      void        CSL_MLBSS_resetSubSystem( bool bWaitForCompletion );
//
//  The following functions have already been used earlier and are not tested here:
//      bool        mlbss_ssIsInitInProgress( void );
//      void        mlbss_ssSetBaseAddr( uint64_t BaseAddr );
//=============================================================================
void TestSsCsl()
{
    CslOsal_Log( "Testing SS CSL functions...\n" );
    _assert( CSL_MLBSS_getPid() == CSL_MLBSS_PID_RESETVAL );
    _assert( CSL_MLBSS_isResetInProgress() == false );
    _assert( CSL_REG_READ(mlbssRegs->CTRL) == CSL_MLBSS_CTRL_RESETVAL );
    _assert( CSL_REG_READ(mlbssRegs->DMA_CTRL) == CSL_MLBSS_DMA_CTRL_RESETVAL );
    CSL_MLBSS_configSubSystem( true, CSL_MLBSS_DMA_CTRL_PRIORITY_MAX );
    _assert( CSL_FEXT(mlbssRegs->CTRL, MLBSS_CTRL_CLKFACK) == 1 );
    _assert( CSL_FEXT(mlbssRegs->DMA_CTRL, MLBSS_DMA_CTRL_PRIORITY) == CSL_MLBSS_DMA_CTRL_PRIORITY_MAX );
#if 1
    CSL_MLBSS_resetSubSystem( true );
    _assert( CSL_MLBSS_isResetInProgress() == false );
    _assert( CSL_FEXT(mlbssRegs->CTRL, MLBSS_CTRL_CLKFACK) == CSL_MLBSS_CTRL_CLKFACK_RESETVAL );
    _assert( CSL_FEXT(mlbssRegs->DMA_CTRL, MLBSS_DMA_CTRL_PRIORITY) == CSL_MLBSS_DMA_CTRL_PRIORITY_RESETVAL );
#endif
}

//=============================================================================
//  TestHbiCsl
//
//  This function tests the following HBI CSL-F functions and macros:
//      void CSL_MLBSS_enableHbi( bool bEnable );
//      void CSL_MLBSS_enableHbiInt( unsigned LogChnNum, bool bEnable );
//      void CSL_MLBSS_enableHbiInts( uint64_t LogChnNumMask, bool bEnable );
//=============================================================================
void TestHbiCsl()
{
    uint64_t TrueMask = 0, FalseMask = 0;
    unsigned LogChnNum;

    CslOsal_Log( "Testing HBI CSL functions...\n" );
    _assert( CSL_FEXT(mlbdimRegs->HCTL, MLBDIM_HCTL_EN) == CSL_MLBDIM_HCTL_EN_RESETVAL );
    CSL_MLBSS_enableHbi( true );
    _assert( CSL_FEXT(mlbdimRegs->HCTL, MLBDIM_HCTL_EN) == 1 );
    CSL_MLBSS_enableHbi( false );
    _assert( CSL_FEXT(mlbdimRegs->HCTL, MLBDIM_HCTL_EN) == CSL_MLBDIM_HCTL_EN_RESETVAL );
    _assert( CSL_REG_READ(mlbdimRegs->HCMR0) == CSL_MLBDIM_HCMR0_RESETVAL );
    _assert( CSL_REG_READ(mlbdimRegs->HCMR1) == CSL_MLBDIM_HCMR1_RESETVAL );
    CSL_REG_WRITE( mlbdimRegs->HCMR0, 0xAAAAAAAA );     // Set odd bits to 1, even bits to 0
    CSL_REG_WRITE( mlbdimRegs->HCMR1, 0xAAAAAAAA );     // Set odd bits to 1, even bits to 0
    _assert( CSL_REG_READ(mlbdimRegs->HCMR0) == 0xAAAAAAAA );
    _assert( CSL_REG_READ(mlbdimRegs->HCMR1) == 0xAAAAAAAA );
    //-------------------------------------------------------------------------
    //  Use CSL-F functions to flip mask bits (odd bits from 1 to 0,
    //  even bits from 0 to 1).
    //      CSL_MLBSS_enableHbiInt/CSL_MLBSS_enableHbiInts will be used for bit-pairs 0+1, 4+5, etc.
    //      CSL_MLBSS_disableHbiInt/CSL_MLBSS_disableHbiInts  will be used for bit pairs 2+3, 6+7, etc.
    //-------------------------------------------------------------------------
    for( LogChnNum=0; LogChnNum<64; LogChnNum+=2 )
    {
        if( (LogChnNum % 4) == 0 )
        {
            CSL_MLBSS_enableHbiInt( LogChnNum );        // Flip even bit from 0 to 1
            CSL_MLBSS_disableHbiInt( LogChnNum + 1 );   // Flip odd bit from 1 to 0
        }
        else
        {
            TrueMask |= ((uint64_t)1) << LogChnNum;         // Flip even bit from 0 to 1
            FalseMask |= ((uint64_t)1) << (LogChnNum+1);    // Flip odd bit from 1 to 0
        }
    }
    CSL_MLBSS_enableHbiInts( TrueMask );
    CSL_MLBSS_disableHbiInts( FalseMask );
    _assert( CSL_REG_READ(mlbdimRegs->HCMR0) == 0x55555555 );
    _assert( CSL_REG_READ(mlbdimRegs->HCMR1) == 0x55555555 );
}

//=============================================================================
//  TestAhbCsl
//
//  This function tests the following AHB CSL-F functions and macros:
//      void        CSL_MLBSS_configAhb( MLBSS_AHB_PKT_MODE_E PacketMode, MLBSS_AHB_DMA_MODE_E DmaMode, bool bIntMux, bool bSwClrInt );
//      bool        CSL_MLBSS_isAhbIntPending( unsigned LogChnNum );
//      uint64_t    CSL_MLBSS_getAhbIntsPending( void );
//      void        CSL_MLBSS_enableAhbInt( unsigned LogChnNum, bool bEnable );
//      void        CSL_MLBSS_enableAhbInts( uint64_t LogChnNumMask, bool bEnable );
//      void        CSL_MLBSS_ackAhbInt( unsigned LogChnNum );
//      void        CSL_MLBSS_ackAhbInts( uint64_t LogChnNumMask );
//=============================================================================
void TestAhbCsl()
{
    uint64_t TrueMask = 0, FalseMask = 0;
    unsigned LogChnNum;

    CslOsal_Log( "Testing AHB CSL functions...\n" );
    //-------------------------------------------------------------------------
    //  Test CSL_MLBSS_isAhbIntPending, CSL_MLBSS_getAhbIntsPending,
    //  CSL_MLBSS_ackAhbInt, CSL_MLBSS_ackAhbInts
    //-------------------------------------------------------------------------
    _assert( CSL_MLBSS_getAhbIntsPending() == 0UL );
    for( LogChnNum=0; LogChnNum<64; LogChnNum++ )
        _assert( CSL_MLBSS_isAhbIntPending( LogChnNum ) == false );
    for( LogChnNum=0; LogChnNum<64; LogChnNum++ )
        CSL_MLBSS_ackAhbInt( LogChnNum );
    _assert( CSL_MLBSS_getAhbIntsPending() == 0UL );
    CSL_MLBSS_ackAhbInts( (uint64_t)-1 );
    _assert( CSL_MLBSS_getAhbIntsPending() == 0UL );
    //-------------------------------------------------------------------------
    //  Test CSL_MLBSS_configAhb
    //-------------------------------------------------------------------------
    _assert( CSL_FEXT( mlbdimRegs->ACTL, MLBDIM_ACTL_SCE ) == 0 );
    _assert( CSL_FEXT( mlbdimRegs->ACTL, MLBDIM_ACTL_SMX ) == 0 );
    _assert( CSL_FEXT( mlbdimRegs->ACTL, MLBDIM_ACTL_DMAMODE ) == 0 );
    _assert( CSL_FEXT( mlbdimRegs->ACTL, MLBDIM_ACTL_MPB ) == 0 );
    CSL_MLBSS_configAhb( MLBSS_AHB_PKT_MODE_MULTIPLE, MLBSS_AHB_DMA_MODE_1, true, true );
    _assert( CSL_FEXT( mlbdimRegs->ACTL, MLBDIM_ACTL_SCE ) == ((unsigned)true) );
    _assert( CSL_FEXT( mlbdimRegs->ACTL, MLBDIM_ACTL_SMX ) == ((unsigned)true) );
    _assert( CSL_FEXT( mlbdimRegs->ACTL, MLBDIM_ACTL_DMAMODE ) == MLBSS_AHB_DMA_MODE_1 );
    _assert( CSL_FEXT( mlbdimRegs->ACTL, MLBDIM_ACTL_MPB ) == MLBSS_AHB_PKT_MODE_MULTIPLE );
    CSL_MLBSS_configAhb( MLBSS_AHB_PKT_MODE_SINGLE, MLBSS_AHB_DMA_MODE_0, false, false );
    _assert( CSL_FEXT( mlbdimRegs->ACTL, MLBDIM_ACTL_SCE ) == 0 );
    _assert( CSL_FEXT( mlbdimRegs->ACTL, MLBDIM_ACTL_SMX ) == 0 );
    _assert( CSL_FEXT( mlbdimRegs->ACTL, MLBDIM_ACTL_DMAMODE ) == 0 );
    _assert( CSL_FEXT( mlbdimRegs->ACTL, MLBDIM_ACTL_MPB ) == 0 );
    //-------------------------------------------------------------------------
    //  Test CSL_MLBSS_enableAhbInt, CSL_MLBSS_enableAhbInts
    //-------------------------------------------------------------------------
    _assert( CSL_REG_READ(mlbdimRegs->ACMR0) == CSL_MLBDIM_ACMR0_RESETVAL );
    _assert( CSL_REG_READ(mlbdimRegs->ACMR1) == CSL_MLBDIM_ACMR1_RESETVAL );
    CSL_REG_WRITE( mlbdimRegs->ACMR0, 0xAAAAAAAA );     // Set odd bits to 1, even bits to 0
    CSL_REG_WRITE( mlbdimRegs->ACMR1, 0xAAAAAAAA );     // Set odd bits to 1, even bits to 0
    _assert( CSL_REG_READ(mlbdimRegs->ACMR0) == 0xAAAAAAAA );
    _assert( CSL_REG_READ(mlbdimRegs->ACMR1) == 0xAAAAAAAA );
    //-------------------------------------------------------------------------
    //  Use CSL-F functions to flip mask bits (odd bits from 1 to 0,
    //  even bits from 0 to 1).
    //      CSL_MLBSS_enableAhbInt/CSL_MLBSS_enableAhbInts will be used for bit-pairs 0+1, 4+5, etc.
    //      CSL_MLBSS_disableAhbInt/CSL_MLBSS_disableAhbInts will be used for bit pairs 2+3, 6+7, etc.
    //-------------------------------------------------------------------------
    for( LogChnNum=0; LogChnNum<64; LogChnNum+=2 )
    {
        if( (LogChnNum % 4) == 0 )
        {
            CSL_MLBSS_enableAhbInt( LogChnNum  );           // Flip even bit from 0 to 1
            CSL_MLBSS_disableAhbInt( LogChnNum  );          // Flip odd bit from 1 to 0
        }
        else
        {
            TrueMask |= ((uint64_t)1) << LogChnNum;         // Flip even bit from 0 to 1
            FalseMask |= ((uint64_t)1) << (LogChnNum+1);    // Flip odd bit from 1 to 0
        }
    }
    CSL_MLBSS_enableAhbInts( TrueMask );
    CSL_MLBSS_disableAhbInts( FalseMask );
    _assert( CSL_REG_READ(mlbdimRegs->ACMR0) == 0x55555555 );
    _assert( CSL_REG_READ(mlbdimRegs->ACMR1) == 0x55555555 );
}

//=============================================================================
//  TestMlbCsl
//
//  This function tests the following MLB CSL-F functions and macros:
//      void        CSL_MLBSS_configMlb( MLBSS_MLB_CFG_T *pMlbCfg );
//      void        CSL_MLBSS_enableMlb( bool bEnable );
//      uint32_t    CSL_MLBSS_getMlbSysStatus( void );
//      uint32_t    CSL_MLBSS_getMlbSysData( void );
//      void        CSL_MLBSS_enableMlbInt( MLBSS_MLB_EVENT_E MlbEvent, bool bEnable );
//      void        CSL_MLBSS_enableMlbInts( uint32_t MlbEventMask, bool bEnable );
//      void        CSL_MLBSS_ackMlbInt( unsigned LogChnNum );
//      void        CSL_MLBSS_ackMlbInts( uint64_t LogChnNumMask );
//      bool        CSL_MLBSS_isMlbIntPending( unsigned LogChnNum );
//      uint64_t    CSL_MLBSS_getMlbIntsPending( void );
//      bool        CSL_MLBSS_isMlbClockMissing( void );
//      bool        CSL_MLBSS_isMlbClockLocked( void );
//=============================================================================
#define VALID_EVENT(n)  (ValidEvents & (1<<(n)))

void TestMlbCsl()
{
    uint32_t ValidEvents, TrueMask = 0, FalseMask = 0;
    unsigned EventNum, LogChnNum;
    MLBSS_MLB_CFG_T MlbCfg;

    CslOsal_Log( "Testing MLB CSL functions...\n" );
    _assert( CSL_MLBSS_getMlbSysStatus() == 0 );
    _assert( CSL_MLBSS_getMlbSysData() == 0 );
    _assert( CSL_MLBSS_isMlbClockLocked() == false );
    _assert( CSL_MLBSS_isMlbClockMissing() == true );
    //-------------------------------------------------------------------------
    //  Test CSL_MLBSS_configMlb
    //-------------------------------------------------------------------------
    _assert( CSL_REG_READ(mlbdimRegs->MLBC0) == 0 );
    _assert( CSL_REG_READ(mlbdimRegs->MLBPC0) == 0 );
    _assert( CSL_REG_READ(mlbdimRegs->MSS) == 0 );
    _assert( CSL_REG_READ(mlbdimRegs->MLBPC1) == 0 );

    MlbCfg.SyncFrameCnt         = MLBSS_SYNC_FCNT_32;
    MlbCfg.PinMode              = MLBSS_MLB_PIN_MODE_6;
    MlbCfg.ClkSpeed             = MLBSS_MLB_CLK_SPEED_6144X;
    MlbCfg.AhbPktMode           = MLBSS_AHB_PKT_MODE_MULTIPLE;
    MlbCfg.AhbDmaMode           = MLBSS_AHB_DMA_MODE_1;
    MlbCfg.bAhbIntMux           = true;
    MlbCfg.bAhbSwClrInt         = true;
    MlbCfg.bCtlRtry             = true;
    MlbCfg.bAsyRetry            = true;
    MlbCfg.bServiceReqEnable    = true;
    MlbCfg.bHysteresisEnable    = true;
    MlbCfg.ClkRxBiasCtl         = CSL_MLBDIM_MLBPC1_CKRCVBIAS_MAX;
    MlbCfg.SigDataTxBiasCtl     = CSL_MLBDIM_MLBPC1_SDXMTBIAS_MAX;
    MlbCfg.SigDataRxBiasCtl     = CSL_MLBDIM_MLBPC1_SDRCVBIAS_MAX;
    CSL_MLBSS_configMlb( &MlbCfg );

    _assert( CSL_FEXT(mlbdimRegs->MLBC0, MLBDIM_MLBC0_FCNT) == MLBSS_SYNC_FCNT_32 );
    _assert( CSL_FEXT(mlbdimRegs->MLBC0, MLBDIM_MLBC0_MLBPEN) == MLBSS_MLB_PIN_MODE_6 );
    _assert( CSL_FEXT(mlbdimRegs->MLBC0, MLBDIM_MLBC0_MLBCLK) == MLBSS_MLB_CLK_SPEED_6144X );
    _assert( CSL_FEXT(mlbdimRegs->MLBC0, MLBDIM_MLBC0_CTLRETRY) == 1 );
    _assert( CSL_FEXT(mlbdimRegs->MLBC0, MLBDIM_MLBC0_ASYRETRY) == 1 );
    _assert( CSL_FEXT(mlbdimRegs->MLBPC0, MLBDIM_MLBPC0_MCLKHYS) == 1 );
    _assert( CSL_FEXT(mlbdimRegs->MSS, MLBDIM_MSS_SERVREQ) == 1 );
    _assert( CSL_FEXT(mlbdimRegs->MLBPC1, MLBDIM_MLBPC1_SDRCVBIAS) == CSL_MLBDIM_MLBPC1_SDRCVBIAS_MAX );
    _assert( CSL_FEXT(mlbdimRegs->MLBPC1, MLBDIM_MLBPC1_SDXMTBIAS) == CSL_MLBDIM_MLBPC1_SDXMTBIAS_MAX );
    _assert( CSL_FEXT(mlbdimRegs->MLBPC1, MLBDIM_MLBPC1_CKRCVBIAS) == CSL_MLBDIM_MLBPC1_CKRCVBIAS_MAX );

    MlbCfg.SyncFrameCnt         = MLBSS_SYNC_FCNT_1;
    MlbCfg.PinMode              = MLBSS_MLB_PIN_MODE_3;
    MlbCfg.ClkSpeed             = MLBSS_MLB_CLK_SPEED_256X;
    MlbCfg.AhbPktMode           = MLBSS_AHB_PKT_MODE_SINGLE;
    MlbCfg.AhbDmaMode           = MLBSS_AHB_DMA_MODE_0;
    MlbCfg.bAhbIntMux           = false;
    MlbCfg.bAhbSwClrInt         = false;
    MlbCfg.bCtlRtry             = false;
    MlbCfg.bAsyRetry            = false;
    MlbCfg.bServiceReqEnable    = false;
    MlbCfg.bHysteresisEnable    = false;
    MlbCfg.ClkRxBiasCtl         = 0;
    MlbCfg.SigDataTxBiasCtl     = 0;
    MlbCfg.SigDataRxBiasCtl     = 0;
    CSL_MLBSS_configMlb( &MlbCfg );

    _assert( CSL_REG_READ(mlbdimRegs->MLBC0) == 0 );
    _assert( CSL_REG_READ(mlbdimRegs->MLBPC0) == 0 );
    _assert( CSL_REG_READ(mlbdimRegs->MSS) == 0 );
    _assert( CSL_REG_READ(mlbdimRegs->MLBPC1) == 0 );
    //-------------------------------------------------------------------------
    //  Test CSL_MLBSS_enableMlb / CSL_MLBSS_disableMlb
    //-------------------------------------------------------------------------
    _assert( CSL_FEXT( mlbdimRegs->MLBC0, MLBDIM_MLBC0_MLBEN ) == 0 );
    CSL_MLBSS_enableMlb();
    _assert( CSL_FEXT( mlbdimRegs->MLBC0, MLBDIM_MLBC0_MLBEN ) == 1 );
    CSL_MLBSS_disableMlb();
    _assert( CSL_FEXT( mlbdimRegs->MLBC0, MLBDIM_MLBC0_MLBEN ) == 0 );
    //-------------------------------------------------------------------------
    //  Test CSL_MLBSS_getMlbIntsPending, mlbss_mlbIntssPending,
    //  CSL_MLBSS_ackMlbInts, mlbss_mlbIntssAck
    //-------------------------------------------------------------------------
    _assert( CSL_MLBSS_getMlbIntsPending() == 0UL );
    for( LogChnNum=0; LogChnNum<64; LogChnNum++ )
        _assert( CSL_MLBSS_isMlbIntPending( LogChnNum ) == false );
    for( LogChnNum=0; LogChnNum<64; LogChnNum++ )
        CSL_MLBSS_ackMlbInt( LogChnNum );
    _assert( CSL_MLBSS_getMlbIntsPending() == 0UL );
    CSL_MLBSS_ackMlbInts( (uint64_t)-1 );
    _assert( CSL_MLBSS_getMlbIntsPending() == 0UL );
    //-------------------------------------------------------------------------
    //  Test CSL_MLBSS_enableMlbInt, CSL_MLBSS_enableMlbInts
    //-------------------------------------------------------------------------
    ValidEvents =
        (1 << MLBSS_MLB_EVENT_ISO_RX_PROTOCOL_ERR)      |
        (1 << MLBSS_MLB_EVENT_ISO_RX_OVERFLOW)          |
        (1 << MLBSS_MLB_EVENT_SYNC_PROTOCOL_ERR)        |
        (1 << MLBSS_MLB_EVENT_ASYNC_RX_PKT_DONE)        |
        (1 << MLBSS_MLB_EVENT_ASYNC_RX_PROTOCOL_ERR)    |
        (1 << MLBSS_MLB_EVENT_ASYNC_RX_BREAK)           |
        (1 << MLBSS_MLB_EVENT_ASYNC_TX_PKT_DONE)        |
        (1 << MLBSS_MLB_EVENT_ASYNC_TX_PROTOCOL_ERR)    |
        (1 << MLBSS_MLB_EVENT_ASYNC_TX_BREAK)           |
        (1 << MLBSS_MLB_EVENT_CTL_RX_PKT_DONE)          |
        (1 << MLBSS_MLB_EVENT_CTL_RX_PROTOCOL_ERR)      |
        (1 << MLBSS_MLB_EVENT_CTL_RX_BREAK)             |
        (1 << MLBSS_MLB_EVENT_CTL_TX_PKT_DONE)          |
        (1 << MLBSS_MLB_EVENT_CTL_TX_PROTOCOL_ERR)      |
        (1 << MLBSS_MLB_EVENT_CTL_TX_BREAK);

    _assert( CSL_REG_READ(mlbdimRegs->MIEN) == CSL_MLBDIM_MIEN_RESETVAL );
    CSL_REG_WRITE( mlbdimRegs->MIEN, ValidEvents & 0xAAAAAAAA );     // Set odd bits to 1, even bits to 0
    _assert( CSL_REG_READ(mlbdimRegs->MIEN) == (ValidEvents & 0xAAAAAAAA) );
    //-------------------------------------------------------------------------
    //  Use CSL-F functions to flip mask bits (odd bits from 1 to 0,
    //  even bits from 0 to 1).
    //      CSL_MLBSS_enableMlbInt / CSL_MLBSS_disableMlbInt will be used for bit-pairs 0+1, 4+5, etc.
    //      CSL_MLBSS_enableMlbInts / CSL_MLBSS_disableMlbInts will be used for bit pairs 2+3, 6+7, etc.
    //-------------------------------------------------------------------------
    for( EventNum=0; EventNum<32; EventNum+=2 )
    {
        if( (EventNum % 4) == 0 )
        {
            if( VALID_EVENT(EventNum) )   CSL_MLBSS_enableMlbInt( (MLBSS_MLB_EVENT_E)EventNum );          // Flip even bit from 0 to 1
            if( VALID_EVENT(EventNum+1) ) CSL_MLBSS_disableMlbInt( (MLBSS_MLB_EVENT_E)(EventNum + 1) );   // Flip odd bit from 1 to 0
        }
        else
        {
            if( VALID_EVENT(EventNum) )   TrueMask |= (1 << EventNum);                  // Flip even bit from 0 to 1
            if( VALID_EVENT(EventNum+1) ) FalseMask |= (1 << (EventNum+1));             // Flip odd bit from 1 to 0
        }
    }
    CSL_MLBSS_enableMlbInts( TrueMask );
    CSL_MLBSS_disableMlbInts( FalseMask );
    _assert( CSL_REG_READ(mlbdimRegs->MIEN) == (ValidEvents & 0x55555555) );
}

//=============================================================================
//  TestChnCsl
//
//  This function tests the following CHN CSL-F functions and macros:
//      int         mlbss_addChannel( unsigned LogChnNum, MLBSS_CHN_CFG_T *pChn );
//      int         mlbss_enableChannel( unsigned LogChnNum, bool bEnable );
//      int         mlbss_getChannelBuffStatus( unsigned LogChnNum, MLBSS_BUFF_PAGE_E BuffPage, MLBSS_CHN_BUFF_STATUS_T *pChnBuffStatus );
//      int         mlbss_muteChannel( unsigned LogChnNum, bool bMute );
//      int         mlbss_setChannelBuffStatus( unsigned LogChnNum, MLBSS_BUFF_PAGE_E BuffPage, MLBSS_CHN_BUFF_STATUS_T *pChnBuffStatus );
//=============================================================================
void TestChnCsl()
{
    MLBSS_CHN_CFG_T         ChnCfg;
    MLBSS_CHN_SYS_BUFF_T    ChnSysBuff;
    MLBSS_CHN_BUFF_STATUS_T ChnBuffStatus;
    MLBSS_CHN_CFG_ISO_T     ChnCfgIso;

    CslOsal_Log( "Testing CHN CSL functions...\n" );
    CSL_MLBSS_clearCtr();
    //-------------------------------------------------------------------------
    //  Verify error return for invalid arguments
    //-------------------------------------------------------------------------
    _assert( mlbss_addChannel( MLBSS_MAX_CHNS, NULL ) == 1 );           // Channel number is invalid
    _assert( mlbss_addChannel( 0, NULL ) == 3 );                        // Channel information not provided
    _assert( mlbss_enableChannel( MLBSS_MAX_CHNS, false ) == 1 );       // Channel number is invalid
    _assert( mlbss_enableChannel( 0, false ) == 1 );                    // Channel number is not defined
    _assert( mlbss_muteChannel( MLBSS_MAX_CHNS, false ) == 1 );         // Channel number is invalid
    _assert( mlbss_muteChannel( 0, false ) == 1 );                      // Channel number is not defined
    _assert( mlbss_getChannelBuffStatus( MLBSS_MAX_CHNS, MLBSS_BUFF_PAGE_PING, NULL ) == 1 );   // Channel number is invalid
    _assert( mlbss_getChannelBuffStatus( 0, MLBSS_BUFF_PAGE_PING, NULL ) == 1 );                // Channel number is not defined
    _assert( mlbss_setChannelBuffStatus( MLBSS_MAX_CHNS, MLBSS_BUFF_PAGE_PING, NULL ) == 1 );   // Channel number is invalid
    _assert( mlbss_setChannelBuffStatus( 0, MLBSS_BUFF_PAGE_PING, NULL ) == 1 );                // Channel number is not defined
    //-------------------------------------------------------------------------
    //  Configure an ISO channel with DbrNumBlocks < 3. This should result in
    //  an error return 4 from mlbss_addChannel.
    //-------------------------------------------------------------------------
    ChnSysBuff.BuffAddr[0]      = 0;
    ChnSysBuff.BuffAddr[1]      = 0x100;
    ChnSysBuff.BuffDepth[0]     = 0x100;
    ChnSysBuff.BuffDepth[1]     = 0x100;
    ChnSysBuff.bLittleEndian    = true;

    ChnCfgIso.BlockSizeBytes    = 64;
    ChnCfgIso.DbrNumBlocks      = 1;                        // 1 < 3 (should generate mlbss_addChannel error return)
    ChnCfgIso.SysNumBlocks      = 8;
    ChnCfgIso.bFlowCtlEnable    = false;

    ChnCfg.Type = MLBSS_CHN_TYPE_ISO;
    ChnCfg.Dir  = MLBSS_CHN_DIR_RX;
    ChnCfg.bEnableInt = false;
    ChnCfg.pChnCfg = (void *)&ChnCfgIso;
    ChnCfg.pSysBuff = &ChnSysBuff;

    _assert( mlbss_addChannel( 0, &ChnCfg ) == 4 );             // Invalid DbrNumBlocks for this channel
    //-------------------------------------------------------------------------
    //  Correct DBR size and try again
    //-------------------------------------------------------------------------
    ChnCfgIso.DbrNumBlocks      = (8*1024)/ChnCfgIso.BlockSizeBytes;    // Allocate 8KB from DBR
    _assert( mlbss_addChannel( 0, &ChnCfg ) == 0 );             // Channel add should pass this time
    //-------------------------------------------------------------------------
    //  Test a couple of other error conditions
    //-------------------------------------------------------------------------
    _assert( mlbss_muteChannel( 0, true ) == 2 );               // Cannot mute an ISO channel
    _assert( mlbss_addChannel(0, &ChnCfg ) == 2 );              // Channel 0 is already defined
    //-------------------------------------------------------------------------
    //  Test channel enabling
    //-------------------------------------------------------------------------
    _assert( mlbss_isChnEnabled(0) == false );              // Channel should currently be disabled
    _assert( mlbss_enableChannel( 0, true ) == 0 );             // Enable channel
    _assert( mlbss_isChnEnabled( 0 ) == true );             // Verify channel is enabled
    _assert( mlbss_enableChannel( 0, false ) == 0 );            // Disable channel
    _assert( mlbss_isChnEnabled(0) == false );              // Channel should be disabled
    //-------------------------------------------------------------------------
    //  Test buffer status
    //-------------------------------------------------------------------------
    _assert( mlbss_getChannelBuffStatus( 0, MLBSS_BUFF_PAGE_PING, &ChnBuffStatus ) == 0 );
    _assert( ChnBuffStatus.bBuffRdy == false );
    _assert( ChnBuffStatus.bBuffDone == false );
    _assert( ChnBuffStatus.bErr == false );
    _assert( ChnBuffStatus.bPktStart == false );
    _assert( ChnBuffStatus.bMEP == false );
    mlbss_setAdtBuffRdy( 0, MLBSS_BUFF_PAGE_PING, true );
    mlbss_setAdtBuffPktStart( 0, MLBSS_BUFF_PAGE_PING, true );
    _assert( mlbss_getChannelBuffStatus( 0, MLBSS_BUFF_PAGE_PING, &ChnBuffStatus ) == 0 );
    _assert( ChnBuffStatus.bBuffRdy == true );
    _assert( ChnBuffStatus.bBuffDone == false );
    _assert( ChnBuffStatus.bErr == false );
    _assert( ChnBuffStatus.bPktStart == true );
    _assert( ChnBuffStatus.bMEP == false );
    _assert( mlbss_setChannelBuffStatus( 0, MLBSS_BUFF_PAGE_PONG, &ChnBuffStatus ) == 0 );
    _assert( mlbss_isAdtBuffRdy( 0, MLBSS_BUFF_PAGE_PONG ) == true );
    _assert( mlbss_isAdtBuffDone( 0, MLBSS_BUFF_PAGE_PONG ) == false );
    _assert( mlbss_isAdtBuffPktStart( 0, MLBSS_BUFF_PAGE_PONG ) == true );
    _assert( mlbss_isAdtBuffErr( 0, MLBSS_BUFF_PAGE_PONG ) == false );
    _assert( mlbss_isAdtBuffMEP( 0, MLBSS_BUFF_PAGE_PONG ) == false );
    //-------------------------------------------------------------------------
    //  Add another ISO channel that exceeds size of DBR to test mlbss_addChannel
    //  error return
    //-------------------------------------------------------------------------
    ChnCfgIso.DbrNumBlocks      = (4*1024)/ChnCfgIso.BlockSizeBytes;    // Allocate 4KB from DBR (for a total of 12KB)
    _assert( mlbss_addChannel(1, &ChnCfg ) == 0 );              // Channel add should succeed
    ChnCfgIso.DbrNumBlocks      = (8*1024)/ChnCfgIso.BlockSizeBytes;    // Allocate 8KB from DBR (for at total of 20KB)
    _assert( mlbss_addChannel(2, &ChnCfg ) == 5 );              // DBR size exceeded
}

//=============================================================================
//  TestCfgCsl
//
//  This function tests the following CFG CSL-F functions and macros:
//      int         mlbss_config( MLBSS_CFG_T *pMlbSsCfg, MLBSS_MLB_CFG_T *pMlbCoreCfg );
//=============================================================================
void TestCfgCsl()
{
    MLBSS_CFG_T MlbssCfg =
    {
        true,                               //  bool        bEnableFastClkStop;
        CSL_MLBSS_DMA_CTRL_PRIORITY_MAX     //  unsigned    DmaVbusPriority;
    };
    MLBSS_MLB_CFG_T MlbDimCfg =
    {
        MLBSS_SYNC_FCNT_64,                 //  MLBSS_SYNC_FCNT_E       SyncFrameCnt;
        MLBSS_MLB_PIN_MODE_6,               //  MLBSS_MLB_PIN_MODE_E    PinMode;
        MLBSS_MLB_CLK_SPEED_8192X,          //  MLBSS_MLB_CLK_SPEED_E   ClkSpeed;
        MLBSS_AHB_PKT_MODE_MULTIPLE,        //  MLBSS_AHB_PKT_MODE_E    AhbPktMode;
        MLBSS_AHB_DMA_MODE_1,               //  MLBSS_AHB_DMA_MODE_E    AhbDmaMode;
        true,                               //  bool                    bAhbIntMux;
        true,                               //  bool                    bAhbSwClrInt;
        true,                               //  bool                    bCtlRtry;
        true,                               //  bool                    bAsyRetry;
        true,                               //  bool                    bServiceReqEnable;
        true,                               //  bool                    bHysteresisEnable;
        CSL_MLBDIM_MLBPC1_CKRCVBIAS_MAX,    //  unsigned                ClkRxBiasCtl;
        CSL_MLBDIM_MLBPC1_SDXMTBIAS_MAX,    //  unsigned                SigDataTxBiasCtl;
        CSL_MLBDIM_MLBPC1_SDRCVBIAS_MAX     //  unsigned                SigDataRxBiasCtl;
    };

    CslOsal_Log( "Testing CFG CSL functions...\n" );
    //-------------------------------------------------------------------------
    //  Verify error return for invalid arguments
    //-------------------------------------------------------------------------
    _assert( mlbss_config( NULL, &MlbDimCfg ) == 1 );
    _assert( mlbss_config( &MlbssCfg, NULL ) == 1 );
    //-------------------------------------------------------------------------
    //  Reset the mlbss, then do a valid configuration call and verify
    //  expected register settings
    //-------------------------------------------------------------------------
#if 0
    CSL_MLBSS_resetSubSystem( true );
#endif
    _assert( mlbss_config( &MlbssCfg, &MlbDimCfg ) == 0 );
    _assert( CSL_FEXT(mlbssRegs->CTRL, MLBSS_CTRL_CLKFACK) == ((unsigned)true) );
    _assert( CSL_FEXT(mlbssRegs->DMA_CTRL, MLBSS_DMA_CTRL_PRIORITY) == CSL_MLBSS_DMA_CTRL_PRIORITY_MAX );
    _assert( CSL_FEXT(mlbdimRegs->MLBC0, MLBDIM_MLBC0_FCNT) == MLBSS_SYNC_FCNT_64 );
    _assert( CSL_FEXT(mlbdimRegs->MLBC0, MLBDIM_MLBC0_MLBPEN) == MLBSS_MLB_PIN_MODE_6 );
    _assert( CSL_FEXT(mlbdimRegs->MLBC0, MLBDIM_MLBC0_MLBCLK) == MLBSS_MLB_CLK_SPEED_8192X );
    _assert( CSL_FEXT(mlbdimRegs->MLBC0, MLBDIM_MLBC0_CTLRETRY) == ((unsigned)true) );
    _assert( CSL_FEXT(mlbdimRegs->MLBC0, MLBDIM_MLBC0_ASYRETRY) == ((unsigned)true) );
    _assert( CSL_FEXT(mlbdimRegs->MLBPC0, MLBDIM_MLBPC0_MCLKHYS) == ((unsigned)true) );
    _assert( CSL_FEXT(mlbdimRegs->MSS, MLBDIM_MSS_SERVREQ) == ((unsigned)true) );
    _assert( CSL_FEXT(mlbdimRegs->MLBPC1, MLBDIM_MLBPC1_SDRCVBIAS) == CSL_MLBDIM_MLBPC1_SDRCVBIAS_MAX );
    _assert( CSL_FEXT(mlbdimRegs->MLBPC1, MLBDIM_MLBPC1_SDXMTBIAS) == CSL_MLBDIM_MLBPC1_SDXMTBIAS_MAX );
    _assert( CSL_FEXT(mlbdimRegs->MLBPC1, MLBDIM_MLBPC1_CKRCVBIAS) == CSL_MLBDIM_MLBPC1_CKRCVBIAS_MAX );
    _assert( CSL_FEXT(mlbdimRegs->ACTL, MLBDIM_ACTL_SCE) == ((unsigned)true) );
    _assert( CSL_FEXT(mlbdimRegs->ACTL, MLBDIM_ACTL_SMX) == ((unsigned)true) );
    _assert( CSL_FEXT(mlbdimRegs->ACTL, MLBDIM_ACTL_DMAMODE) == MLBSS_AHB_DMA_MODE_1 );
    _assert( CSL_FEXT(mlbdimRegs->ACTL, MLBDIM_ACTL_MPB) == MLBSS_AHB_PKT_MODE_MULTIPLE );
}

int test()
{
    CslOsal_Log( "MLBSS CSL-F validation tests:\n", MLBSS_REGS_BASE_ADDRESS );

    CslOsal_Log( "Setting base address to 0x%08x...\n", MLBSS_REGS_BASE_ADDRESS );
    mlbss_initCsl( MLBSS_REGS_BASE_ADDRESS );

    CslOsal_Log( "Waiting for ECC initialization of DIM rams to complete...\n" );
    while( mlbss_ssIsInitInProgress() );    // Wait for ECC initialization of DIM rams to complete

    //-------------------------------------------------------------------------
    //  Perform tests in bottoms-up fashion (lowest-level functions first,
    //  working up to higher level functions)
    //-------------------------------------------------------------------------
    TestMmrRegionAccesses();
    TestDimRamAccesses();
    TestMifCsl();
    TestCdtCsl();
    TestAdtCsl();
    TestCatCsl();
    TestRatCsl();
    TestSsCsl();
    TestHbiCsl();
    TestAhbCsl();
    TestMlbCsl();
    TestChnCsl();
    TestCfgCsl();

    CslOsal_Log( "End of MLBSS CSL-F validation tests\n", MLBSS_REGS_BASE_ADDRESS );
    return 0;
}

int mymain( int argc, char *argv[], char *envp[] )
{
    cPortConfig myConfig(pTB);

    //Set Borg Region mappings
    myConfig.SetMasterPortMap("mlbss_cfg_vbusp"       ,0u,(physptr_t)MLBSS_REGS_BASE_ADDRESS, MLBSS_REGS_SIZE, true);
    myConfig.SetMasterPortMap("mlbss_cfg_vbusp"       ,1u,(physptr_t)ECC_AGGR_REGS_BASE_ADDRESS, ECC_AGGR_REGS_SIZE, true);
    myConfig.SetMasterPortMap("mlbss_cfg_vbusp"       ,2u,(physptr_t)MLB_DIM_REGS_BASE_ADDRESS,MLB_DIM_REGS_SIZE, true);
    myConfig.SetMasterPortMap("mlbss_cfg_vbusp"       ,3u,(physptr_t)MLBSS_RAT_REGS_BASE_ADDRESS, MLBSS_RAT_REGS_SIZE, true);

    test();

    Osal_Shutdown(0);
    return 0;
}
