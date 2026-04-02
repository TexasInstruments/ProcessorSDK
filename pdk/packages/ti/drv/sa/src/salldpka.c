/******************************************************************************
 * FILE PURPOSE:  SA LLD PKA APIs
 ******************************************************************************
 * FILE NAME:   salldpka.c  
 *
 * DESCRIPTION: SA LLD PKA related API implementations 
 *
 * FUNCTION               DESCRIPTION
 * ========               ===========
 *
 * REVISION HISTORY:
 *
 * (C) Copyright 2014-2020, Texas Instruments Inc.
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
/* System level header files */
#include <stdlib.h>
#include <string.h>

/* SALLD header files */
#include <ti/drv/sa/salld.h>
#include <ti/drv/sa/sa_osal.h>
#include <ti/csl/cslr_cp_ace.h>
#ifdef NSS_LITE2
#include <ti/csl/src/ip/sa/V2/cslr_eip_29t2_ram.h>
#endif
#include <ti/drv/sa/src/salldloc.h>
#include <ti/drv/sa/src/pkafw/pkafw.h>

/* Local functions prototypes */
#ifdef NSS_PKA_GEN2
static int16_t salld_pka_op_pkcp_setup(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo);
static int16_t salld_pka_op_modexp_setup(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo);
static int16_t salld_pka_op_modexp_crt_setup(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo);
static int16_t salld_pka_op_modinv_setup(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo);
static int16_t salld_pka_op_ecp_add_setup(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo);
static int16_t salld_pka_op_ecp_mul_setup(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo);
static int16_t salld_pka_op_ecp_scale_setup(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo);
static int16_t salld_pka_op_ecp_dsa_sign_setup(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo);
static int16_t salld_pka_op_ecp_dsa_verify_setup(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo);
static int16_t salld_pka_op_ecp_mul_scale_setup(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo);
static int16_t salld_pka_op_ec2m_add_setup(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo);
static int16_t salld_pka_op_ec2m_mul_setup(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo);
static int16_t salld_pka_op_ec2m_scale_setup(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo);
static int16_t salld_pka_op_ec2m_mul_scale_setup(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo);
static int16_t salld_pka_op_pkcp_proc(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo, void* resultPtr);
static int16_t salld_pka_op_mod_common_proc(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo, void* resultPtr);
static int16_t salld_pka_op_modexp_crt_proc(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo, void* resultPtr);
static int16_t salld_pka_op_ecp_common_proc(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo, void* resultPtr);
static int16_t salld_pka_op_ecp_mul_scale_proc(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo, void* resultPtr);
static int16_t salld_pka_op_ecp_dsa_sign_proc(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo, void* resultPtr);
static int16_t salld_pka_op_ecp_dsa_verify_proc(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo, void* resultPtr);
static int16_t salld_pka_op_ec2m_common_proc(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo, void* resultPtr);
static int16_t salld_pka_op_ec2m_mul_scale_proc(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo, void* resultPtr);
static int16_t salld_pkt_poll(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo2);
#endif

/******************************************************************************
 * FUNCTION PURPOSE: Create and initialize the PKA module 
 ******************************************************************************
 * DESCRIPTION: The function is called to initialize the PKA 
 *              (Public Key Accelerator) module inside SA
 *
 * int16_t Sa_pkaInit (
 *          Sa_Handle            handle) - salld instance handle
 *
 * Return values:  sa_ERR_OK
 *
 *****************************************************************************/
int16_t Sa_pkaInit (Sa_Handle handle)
{
  salldObj_t *inst = (salldObj_t *)sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, handle);
  CSL_Cp_aceRegs*  pSaRegs = (CSL_Cp_aceRegs*)salldLObj.baseAddr;
#ifndef NSS_PKA_GEN2
  CSL_Cp_acePkaRegs* pPkaRegs = &pSaRegs->PKA; 
#else   
  Firmware_EIP29T2_t fwInfo;
  uint32_t fwVer;
#endif
  int16_t ret_code = sa_ERR_OK;
  uint32_t mtKey;
  
  Sa_osalMtCsEnter(&mtKey); 
  
  Sa_osalBeginMemAccess(inst, sizeof(salldObj_t));  
  
  if (inst->magic != SALLD_INST_MAGIC_WORD)
  {
  
    Sa_osalMtCsExit(mtKey);
  
    if(inst->magic == salld_swiz32(SALLD_INST_MAGIC_WORD))
        return(sa_ERR_INV_ENDIAN_MODE);
    else
        return(sa_ERR_INV_HANDLE);    
  }  
  
  
  if(!SALLD_TEST_STATE_PKA(inst))
  {
    /* Enable the PKA module */
    #ifdef NSS_LITE2
    if((pSaRegs->MMR.CMD_STATUS & CSL_CP_ACE_CMD_STATUS_PKA_EN_MASK) !=
        CSL_CP_ACE_CMD_STATUS_PKA_EN_MASK)
    {
        if (SALLD_TEST_LIMIT_ACCESS(inst))
        {
            /* This is a critical error */
            return(sa_ERR_ENGINE_STATE);
        }
        else
        {
            pSaRegs->UPDATES.ENGINE_ENABLE |= CSL_CP_ACE_CMD_STATUS_PKA_EN_MASK;
        }
    }
    #else
    pSaRegs->MMR.CMD_STATUS |= CSL_CP_ACE_CMD_STATUS_PKA_EN_MASK;
    #endif

    SALLD_SET_STATE_PKA(inst, 1); 
    
    /* Writeback the instance updates */
    Sa_osalEndMemAccess(inst, sizeof(salldObj_t));  
    
#ifndef NSS_PKA_GEN2    
    /* Pre-set ABCD register */
    pPkaRegs->PKA_ABCDPTR[SA_PKA_AINDEX] = SA_PKA_A_OFFSET;
    pPkaRegs->PKA_ABCDPTR[SA_PKA_BINDEX] = SA_PKA_B_OFFSET;
    pPkaRegs->PKA_ABCDPTR[SA_PKA_CINDEX] = SA_PKA_C_OFFSET;
    pPkaRegs->PKA_ABCDPTR[SA_PKA_DINDEX] = SA_PKA_D_OFFSET;
    pPkaRegs->PKA_ABCDPTR[SA_PKA_DINDEX] = SA_PKA_D_OFFSET;
#else
    /* Download PKA firmware */
    Firmware_EIP29T2_GetReferences(&fwInfo);
    
    ret_code = Sa_pkaDownloadImage (handle, (void *)fwInfo.Image_p, (int) fwInfo.WordCount, &fwVer);

    /* TBD: check the version number */

#endif
     
  } 

  Sa_osalMtCsExit(mtKey);
  
  return (ret_code);

}

/******************************************************************************
 * FUNCTION PURPOSE:  Deactivate the PKA module (optional) 
 ******************************************************************************
 * DESCRIPTION: This function deactivates the PKA module and clears its
 *              corresponding LLD internal state 
 *
 *  int16_t  Sa_pkaClose  (
 *              Sa_Handle handle)      - SALLD instance
 *
 * Return values:  sa_ERR_OK
 *
 ******************************************************************************/
int16_t  Sa_pkaClose  (Sa_Handle handle)
{
  salldObj_t *inst = (salldObj_t *)sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, handle);
  CSL_Cp_aceRegs*  pSaRegs = (CSL_Cp_aceRegs*)salldLObj.baseAddr;
  uint32_t mtKey;
  
  Sa_osalMtCsEnter(&mtKey); 
  
  Sa_osalBeginMemAccess(inst, sizeof(salldObj_t)); 
  
  if (inst->magic != SALLD_INST_MAGIC_WORD)
  {
  
    Sa_osalMtCsExit(mtKey);
  
    if(inst->magic == salld_swiz32(SALLD_INST_MAGIC_WORD))
        return(sa_ERR_INV_ENDIAN_MODE);
    else
        return(sa_ERR_INV_HANDLE);    
  }  
  
  if (SALLD_TEST_STATE_PKA(inst))
  {
    
    SALLD_SET_STATE_PKA(inst, 0);  
    
    /* Disable the RNG module */
    #ifdef NSS_LITE2
    if (!SALLD_TEST_LIMIT_ACCESS(inst))
    {
        pSaRegs->UPDATES.ENGINE_ENABLE &= ~CSL_CP_ACE_CMD_STATUS_PKA_EN_MASK;
    }
    #else
    pSaRegs->MMR.CMD_STATUS &= ~CSL_CP_ACE_CMD_STATUS_PKA_EN_MASK;
    #endif
    /* Writeback the instance updates */
    Sa_osalEndMemAccess(inst, sizeof(salldObj_t));  
  }
  
  Sa_osalMtCsExit(mtKey);
  
  return(sa_ERR_OK);  

} /* Sa_pkaClose */

/***********************************************************************************************
 * FUNCTION PURPOSE: Zero check
 ***********************************************************************************************
 * DESCRIPTION: Verify whether a PKA input vector is zero
 *
 * Return: TRUE: zero
 *         FALSE: non-zero
 *          
 ***********************************************************************************************/
static int sap_pka_is_zero(uint32_t* data, uint16_t size) 
{
    int i;
    
    for ( i = 0; i < size; i++)
    {
        if(data[i])
            return(FALSE);
    }
    return(TRUE);
}

/***********************************************************************************************
 * FUNCTION PURPOSE: PKA Compare
 ***********************************************************************************************
 * DESCRIPTION: Compare two PKA input parameters
 *
 * Return: sa_PKA_COMP_AEQB
 *         sa_PKA_COMP_AINFB
 *         sa_PKA_COMP_ASUPB
 *          
 ***********************************************************************************************/
static Sa_PkaCompResults_t sap_pka_compare(uint32_t* a, uint32_t* b, uint16_t size) 
{
    int i;
    
    for ( i = size - 1; i >= 0; i--)
    {
        if (a[i] > b[i])
        {
            return(sa_PKA_COMP_ASUPB);
        }    
        else if (a[i] < b[i])
        {
            return(sa_PKA_COMP_AINFB);
        }    
    }
    
    return(sa_PKA_COMP_AEQB);
}
/***********************************************************************************************
 * FUNCTION PURPOSE: PKA scratch read to memory
 ***********************************************************************************************
 * DESCRIPTION: Copy PKA scratch to external memory
 *
 * Return: none
 ***********************************************************************************************/
static void sap_pka_read(uint32_t* mem, volatile uint32_t* pka_scr, uint16_t size)
{
    int i;
    
    for  (i = 0 ; i < size; i++ ) {
        *mem++ = *pka_scr++;
    }
}

/***********************************************************************************************
 * FUNCTION PURPOSE: PKA scratch write to memory
 ***********************************************************************************************
 * DESCRIPTION: write to PKA scratch from external memory
 *
 * Return: none
 ***********************************************************************************************/
static void sap_pka_write(volatile uint32_t* pka_scr, uint32_t* mem, uint16_t size)
{
    int i;
    
    for  (i = 0 ; i < size; i++ ) {
        *pka_scr++ = *mem++ ;
    }
}

/******************************************************************************
 * FUNCTION PURPOSE:  Perform large vector arithmetic operation via PKA 
 ******************************************************************************
 * DESCRIPTION: This function is called to triggers a large vector arithmetic 
 *              operation through the PKA module.
 *              It is considered as a blocking function call since it will wait 
 *              for the PKA module to complete the arithmetic operation. 
 *              However, it only takes a few cycles for PKA to complete any operation. 
 *              This function also returns with error code immediately if 
 *              the PKA is still in the process to perform the previous operation
 *              or when timeout occurs.
 *
 *  int16_t Sa_pkaOperation (
 *   Sa_Handle        handle      - SALLD instance handle
 *   Sa_PkaReqInfo_t  pkaReqInfo) - Pointer to the PKA operation request information structure
 *
 * Return values:  sa_ERR_OK
 *                 sa_ERR_PARAMS 
 *                 sa_ERR_MOUDLE_BUSY
 *                 sa_ERR_MODULE_UNAVAIL 
 *                 sa_ERR_PKA_TIMEOUT
 *
 ******************************************************************************/
int16_t Sa_pkaOperation (Sa_Handle handle, Sa_PkaReqInfo_t* pPkaReqInfo)
{
  int16_t ret_code;
  Sa_PkaReqInfo2_t pkaReqInfo2;
  
  if (!pPkaReqInfo)
  {
    return(sa_ERR_PARAMS);
  }
  
  /* Construct PKA request info2 */
  pkaReqInfo2.operation = pPkaReqInfo->operation;
  pkaReqInfo2.mode = sa_PKA_MODE_WAIT;
  pkaReqInfo2.aLen = pPkaReqInfo->oprandSize[SA_PKA_AINDEX];
  pkaReqInfo2.bLen = pPkaReqInfo->oprandSize[SA_PKA_BINDEX];
  pkaReqInfo2.params.basicOp.pA = pPkaReqInfo->oprandPtr[SA_PKA_AINDEX];
  pkaReqInfo2.params.basicOp.pB = pPkaReqInfo->oprandPtr[SA_PKA_BINDEX];
  pkaReqInfo2.params.basicOp.pC = pPkaReqInfo->oprandPtr[SA_PKA_CINDEX];
  pkaReqInfo2.params.basicOp.numShiftBits = pPkaReqInfo->numShiftBits;
  pkaReqInfo2.params.basicOp.pResult = pPkaReqInfo->resultPtr;
  pkaReqInfo2.params.basicOp.pRem = pPkaReqInfo->remPtr;

  ret_code = Sa_pkaOperation2(handle, &pkaReqInfo2);
  
  if (ret_code == sa_ERR_OK)
  {
    pPkaReqInfo->resultSize = pkaReqInfo2.params.basicOp.resultSize;
    pPkaReqInfo->remSize = pkaReqInfo2.params.basicOp.remSize;
    pPkaReqInfo->cmpResult = pkaReqInfo2.params.basicOp.cmpResult;
  }
  
  return(ret_code);  
}

#ifndef NSS_PKA_GEN2

/***********************************************************************************************
 * FUNCTION PURPOSE: Download PKA firmware image
 ***********************************************************************************************
 * DESCRIPTION: This function is used to download an executable firmware image to the second
 *              generation PKA module, triggers the firmware to run and verify its operation. 
 *              The PKA firmware is required for PKA to perform complex operations such as 
 *              modular exponentiation and etc.
 *
 *  int16_t Sa_pkaDownloadImage (
 *   Sa_Handle        handle,     - SALLD instance handle
 *   void*            image,      - Pointer to firmware image 
 *   int              sizeBytes,  - The image size 
 *   uint32_t*        pVersion)   - The firmware revision number 
 *
 * Return values:  sa_ERR_OK
 *                 sa_ERR_PARAMS 
 *                 sa_ERR_INV_ENDIAN_MODE
 *                 sa_ERR_INV_HANDLE
 *                 sa_ERR_MODULE_UNAVAIL
 *                 sa_ERR_PKA_DOWNLOAD_FAIL
 ***********************************************************************************************/
int16_t Sa_pkaDownloadImage (Sa_Handle handle, void* image, int sizeBytes, uint32_t* pVersion)
{
  return (sa_ERR_API_UNSUPPORTED);

} /* Sa_pkaDownloadImage */

/******************************************************************************
 * FUNCTION PURPOSE:  Perform large vector arithmetic operation via PKA 
 ******************************************************************************
 * DESCRIPTION: This function is called to triggers a large vector arithmetic 
 *              operation through the PKA module.
 *              It is considered as a blocking function call since it will wait 
 *              for the PKA module to complete the arithmetic operation. 
 *              However, it only takes a few cycles for PKA to complete any operation. 
 *              This function also returns with error code immediately if 
 *              the PKA is still in the process to perform the previous operation
 *              or when timeout occurs.
 *
 *  int16_t Sa_pkaOperation2 (
 *   Sa_Handle        handle      - SALLD instance handle
 *   Sa_PkaReqInfo2_t pkaReqInfo) - Pointer to the PKA operation request information structure
 *
 * Return values:  sa_ERR_OK
 *                 sa_ERR_PARAMS 
 *                 sa_ERR_MOUDLE_BUSY
 *                 sa_ERR_MODULE_UNAVAIL 
 *                 sa_ERR_PKA_TIMEOUT
 *
 ******************************************************************************/
int16_t Sa_pkaOperation2 (Sa_Handle handle, Sa_PkaReqInfo2_t* pPkaReqInfo)
{
  salldObj_t *inst = (salldObj_t *)sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, handle);
  uint32_t  baseAddr = (uint32_t) salldLObj.baseAddr;
  CSL_Cp_aceRegs*  pSaRegs = (CSL_Cp_aceRegs*)baseAddr;
  CSL_Cp_acePkaRegs* pPkaRegs = &pSaRegs->PKA;  
  Sa_PkaBasicOpParams_t* pPkaOp;
  uint32_t op_code, *pB, *pC;
  int16_t ret_code = sa_ERR_PARAMS;
  uint32_t mtKey;
  int i;
  
  if (inst->magic != SALLD_INST_MAGIC_WORD)
  {
  
    if(inst->magic == salld_swiz32(SALLD_INST_MAGIC_WORD))
        return(sa_ERR_INV_ENDIAN_MODE);
    else
        return(sa_ERR_INV_HANDLE);    
  } 
  
  if (pPkaReqInfo == NULL) 
  {
    return(sa_ERR_PARAMS);
  }
  
  pPkaOp = &pPkaReqInfo->params.basicOp;
  
  if ((pPkaReqInfo->operation != sa_PKA_OP_COMP) && (pPkaOp->pResult == NULL))
  {
    return(sa_ERR_PARAMS);
  }
  
  Sa_osalMtCsEnter(&mtKey); 
  
  if(!SALLD_TEST_STATE_PKA(inst))
  {
    ret_code = sa_ERR_MODULE_UNAVAIL;
    
  } 
  else if (pPkaRegs->PKA_FUNCTION & CSL_CP_ACE_PKA_FUNCTION_RUN_MASK)
  {
    ret_code = sa_ERR_MODULE_BUSY;
  }
  else
  {
    
    /* 
    * Perform the following common operations
    * - Copy data for oprand A and B
    * - Configure registers for oprand A & B
    */
    if ((!pPkaReqInfo->aLen) || (!pPkaOp->pA))
    {
        goto Sa_pkaOperation_end;
    }  
    
    if (pPkaReqInfo->aLen > sa_MAX_PKA_OPERAND_SIZE)
    {
       goto Sa_pkaOperation_end;
    } 
        
    sap_pka_write((volatile uint32_t *) (baseAddr + SA_PKA_A_BASE), 
           pPkaOp->pA, pPkaReqInfo->aLen<<2); 

    pPkaRegs->PKA_ABLENGTH[SA_PKA_AINDEX] = pPkaReqInfo->aLen;
    
    if (pPkaReqInfo->bLen)
    {
        if (!pPkaOp->pB)
        {
            goto Sa_pkaOperation_end;
        }    
        
        if (pPkaReqInfo->bLen > sa_MAX_PKA_OPERAND_SIZE)
        {
            goto Sa_pkaOperation_end;
        } 
    
        sap_pka_write((volatile uint32_t *)(baseAddr + SA_PKA_B_BASE), 
               pPkaOp->pB, pPkaReqInfo->bLen<<2);
        pPkaRegs->PKA_ABLENGTH[SA_PKA_BINDEX] = pPkaReqInfo->bLen;
    }
    
    switch (pPkaReqInfo->operation)
    {
        case sa_PKA_OP_ADD:
            /* Range check: Alength > 0; Blength > 0 */
            if (!pPkaReqInfo->bLen)
            {
                goto Sa_pkaOperation_end;
            }    
        
            op_code = CSL_CP_ACE_PKA_FUNCTION_ADD_MASK |
                      CSL_CP_ACE_PKA_FUNCTION_RUN_MASK;
            break;
        
        case sa_PKA_OP_SUB:
            /* Range check: Alength > 0; Blength > 0; Alength >= Blength */
            if ((!pPkaReqInfo->bLen) || 
                (pPkaReqInfo->aLen < pPkaReqInfo->bLen))
            {
                goto Sa_pkaOperation_end;
            }   
            
            /* A < B ?*/
            if (sap_pka_is_zero(&pPkaOp->pA[pPkaReqInfo->bLen], pPkaReqInfo->aLen - pPkaReqInfo->bLen) &&
                (sap_pka_compare(pPkaOp->pA, pPkaOp->pB, pPkaReqInfo->bLen) == sa_PKA_COMP_AINFB))
            {
                goto Sa_pkaOperation_end;
            }
        
            op_code = CSL_CP_ACE_PKA_FUNCTION_SUBSTRACT_MASK |
                      CSL_CP_ACE_PKA_FUNCTION_RUN_MASK;
            break;

        case sa_PKA_OP_MUL:
            /* Range check: Alength > 0; Blength > 0 */
            if (!pPkaReqInfo->bLen)
            {
                goto Sa_pkaOperation_end;
            } 
               
            op_code = CSL_CP_ACE_PKA_FUNCTION_MULTIPLY_MASK |
                      CSL_CP_ACE_PKA_FUNCTION_RUN_MASK;
            break;
        
        case sa_PKA_OP_DIV:
            /* 
             * Alength > 1; Blength > 1
             * B != 0  
             * Alength >= Blength
             * B[Blength - 1] != 0 ==> B != 0
             */
            if ((pPkaReqInfo->aLen <= 1) ||
                (pPkaReqInfo->bLen <= 1) || 
                (pPkaReqInfo->aLen < pPkaReqInfo->bLen) ||
                (pPkaOp->pB[pPkaReqInfo->bLen - 1] == 0) ||
                (!pPkaOp->pRem))
            {
                goto Sa_pkaOperation_end;
            }    
             
            op_code = CSL_CP_ACE_PKA_FUNCTION_DIVIDE_MASK |
                      CSL_CP_ACE_PKA_FUNCTION_RUN_MASK; 
            break;
        
        case sa_PKA_OP_RSHIFT:
            pPkaRegs->PKA_SHIFT = pPkaOp->numShiftBits;
            op_code = CSL_CP_ACE_PKA_FUNCTION_RSHIFT_MASK |
                      CSL_CP_ACE_PKA_FUNCTION_RUN_MASK;
            break;
        
        case sa_PKA_OP_LSHIFT:
            pPkaRegs->PKA_SHIFT = pPkaOp->numShiftBits;
            op_code = CSL_CP_ACE_PKA_FUNCTION_LSHIFT_MASK |
                    CSL_CP_ACE_PKA_FUNCTION_RUN_MASK;
            break;
        
        case sa_PKA_OP_COMP:
            /* Alength == Blength */
            if (pPkaReqInfo->aLen != pPkaReqInfo->bLen)
            {
                goto Sa_pkaOperation_end;
            }
            op_code = CSL_CP_ACE_PKA_FUNCTION_COMPARE_MASK |
                      CSL_CP_ACE_PKA_FUNCTION_RUN_MASK;
            break;
        
        case sa_PKA_OP_COPY:
            op_code = CSL_CP_ACE_PKA_FUNCTION_COPY_MASK |
                      CSL_CP_ACE_PKA_FUNCTION_RUN_MASK;
            break;
        
        case sa_PKA_OP_EXP2:
        case sa_PKA_OP_EXP4:
            /* 
             * Alength > 0; Blength > 1
             * A != 0; C != 0  (TBD: non-zero check)
             * Alength < Blength
             * B:odd, B[Length - 1] != 0
             * B > C 
             */
            if ((pPkaReqInfo->bLen <= 1) || 
                (pPkaReqInfo->aLen >= pPkaReqInfo->bLen) ||
                (!(pPkaOp->pB[0] & 0x1))                 ||
                (pPkaOp->pB[pPkaReqInfo->bLen - 1] == 0) ||
                (!pPkaOp->pC))
            {
                goto Sa_pkaOperation_end;
            }   
            
            if (sap_pka_is_zero(pPkaOp->pA, pPkaReqInfo->aLen))
            {
                goto Sa_pkaOperation_end;
            } 
            
            if (sap_pka_is_zero(pPkaOp->pC, pPkaReqInfo->bLen))
            {
                goto Sa_pkaOperation_end;
            }
            
            if ((pPkaReqInfo->operation == sa_PKA_OP_EXP4) && (pPkaReqInfo->bLen > sa_MAX_PKA_OPERAND_SIZE_EXP4))
            {
                goto Sa_pkaOperation_end;
            } 
            
            pB = pPkaOp->pB;
            pC = pPkaOp->pC;
            for ( i = pPkaReqInfo->bLen - 1; i > 0; i--)
            {
                if (pC[i] > pB[i])
                {
                    goto Sa_pkaOperation_end;
                }
                else if (pC[i] < pB[i])
                {
                    break;
                }
            } 
            sap_pka_write((volatile uint32_t *)(baseAddr + SA_PKA_C_BASE), 
                   pPkaOp->pC, pPkaReqInfo->bLen<<2);
             
        
            op_code = ((pPkaReqInfo->operation == sa_PKA_OP_EXP2)?CSL_CP_ACE_PKA_FUNCTION_EXP2_MASK:CSL_CP_ACE_PKA_FUNCTION_EXP4_MASK) |
                      CSL_CP_ACE_PKA_FUNCTION_RUN_MASK;
            break;
        
        default:
            ret_code = sa_ERR_PKA_OP_UNSUPPORTED;
            goto Sa_pkaOperation_end;
    }
  
    /* Initialize the Operation */
    pPkaRegs->PKA_FUNCTION  = op_code;
  
    /* Wait for the operation to be complete or timeout occurs */
    for (i = 0; i < SA_PKA_MAX_PKCP_OP_LOOP_COUNT; i++)
    {
        if (!(pPkaRegs->PKA_FUNCTION & CSL_CP_ACE_PKA_FUNCTION_RUN_MASK))
            break;
    }
  
    /* figure out how to calculate the size of operation result */
    if (i >= SA_PKA_MAX_PKCP_OP_LOOP_COUNT)
    {
        ret_code = sa_ERR_PKA_TIMEOUT;
    }
    else
    {
        /* Process the operation result */
        ret_code = sa_ERR_OK;
        switch (pPkaReqInfo->operation)
        {
            case sa_PKA_OP_ADD:
            case sa_PKA_OP_SUB:
            case sa_PKA_OP_MUL:
            case sa_PKA_OP_RSHIFT:
            case sa_PKA_OP_LSHIFT:
            case sa_PKA_OP_COPY:
                pPkaOp->resultSize = (pPkaRegs->PKA_MSW & CSL_CP_ACE_PKA_MSW_RES_ZERO_MASK)?0:
                                      (uint16_t)(pPkaRegs->PKA_MSW - SA_PKA_C_OFFSET + 1);  
                sap_pka_read(pPkaOp->pResult, 
                       (volatile uint32_t *)(baseAddr + SA_PKA_C_BASE), 
                       pPkaOp->resultSize << 2);
                break;
    
            case sa_PKA_OP_DIV:
                pPkaOp->resultSize = (pPkaRegs->PKA_MSW & CSL_CP_ACE_PKA_MSW_RES_ZERO_MASK)?0:
                                      (uint16_t)(pPkaRegs->PKA_MSW - SA_PKA_D_OFFSET + 1);  
                pPkaOp->remSize = (pPkaRegs->PKA_DIVMSW & CSL_CP_ACE_PKA_DIVMSW_RES_ZERO_MASK)?0:
                                   (uint16_t)(pPkaRegs->PKA_DIVMSW - SA_PKA_C_OFFSET + 1);  
                sap_pka_read(pPkaOp->pRem, 
                       (volatile uint32_t *)(baseAddr + SA_PKA_C_BASE), 
                       pPkaOp->remSize << 2);
                sap_pka_read(pPkaOp->pResult, 
                       (volatile uint32_t *)(baseAddr + SA_PKA_D_BASE), 
                       pPkaOp->resultSize << 2);
                break;
    
            case sa_PKA_OP_COMP:
                pPkaOp->cmpResult =  (Sa_PkaCompResults_t)pPkaRegs->PKA_COMPARE;
                break;
    
            case sa_PKA_OP_EXP2:
            case sa_PKA_OP_EXP4:
                pPkaOp->resultSize = (pPkaRegs->PKA_MSW & CSL_CP_ACE_PKA_MSW_RES_ZERO_MASK)?0:
                                      (uint16_t)(pPkaRegs->PKA_MSW - SA_PKA_D_OFFSET + 1);  
                sap_pka_read(pPkaOp->pResult, 
                       (volatile uint32_t *)(baseAddr + SA_PKA_D_BASE), 
                       pPkaOp->resultSize << 2);
                break;
        
            default:
                ret_code = sa_ERR_PKA_OP_UNSUPPORTED;
                break;    
        }
    }
  }
  
Sa_pkaOperation_end:
  
  Sa_osalMtCsExit(mtKey);
  
  return(ret_code);  
}

#else

/***********************************************************************************************
 * FUNCTION PURPOSE: Download PKA firmware image
 ***********************************************************************************************
 * DESCRIPTION: This function is used to download an executable firmware image to the second
 *              generation PKA module, triggers the firmware to run and verify its operation. 
 *              The PKA firmware is required for PKA to perform complex operations such as 
 *              modular exponentiation and etc.
 *
 *  int16_t Sa_pkaDownloadImage (
 *   Sa_Handle        handle,     - SALLD instance handle
 *   void*            image,      - Pointer to firmware image 
 *   int              sizeWords,  - The image size in 32-bit words
 *   uint32_t*        pVersion)   - The firmware revision number 
 *
 * Return values:  sa_ERR_OK
 *                 sa_ERR_PARAMS 
 *                 sa_ERR_INV_ENDIAN_MODE
 *                 sa_ERR_INV_HANDLE
 *                 sa_ERR_MODULE_UNAVAIL
 *                 sa_ERR_PKA_DOWNLOAD_FAIL
 ***********************************************************************************************/
int16_t Sa_pkaDownloadImage (Sa_Handle handle, void* image, int sizeWords, uint32_t* pVersion)
{

  salldObj_t *inst = (salldObj_t *)sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, handle);
  CSL_Cp_aceRegs*  pSaRegs = (CSL_Cp_aceRegs*)salldLObj.baseAddr;
  CSL_Eip_29t2_ramEip_28px12_gf2_2pram_eip28_registersRegs* pPkaCtrlRegs = &pSaRegs->PKA.EIP_28PX12_GF2_2PRAM_EIP28_REGISTERS;
  CSL_Eip_29t2_ramEip_29t2_ram_host_registersRegs* pHostRegs = &pSaRegs->PKA.EIP_29T2_RAM_HOST_REGISTERS;
  
  volatile uintptr_t iramptr = ((uintptr_t)salldLObj.baseAddr + (uintptr_t)SA_PKA_RAM_BASE);
  uint32_t *iram   = (uint32_t *) iramptr;
  uint32_t *pImage = (uint32_t *) image;
  int i;
  
  if (inst->magic != SALLD_INST_MAGIC_WORD)
  {
    if(inst->magic == salld_swiz32(SALLD_INST_MAGIC_WORD))
        return(sa_ERR_INV_ENDIAN_MODE);
    else
        return(sa_ERR_INV_HANDLE);    
  }
  
  if(!SALLD_TEST_STATE_PKA(inst))
  {
    return(sa_ERR_MODULE_UNAVAIL);
  } 
  
  /* Verify the image size is in range */
  if ((sizeWords < 0)  || (sizeWords >= 0x1000))
    return (sa_ERR_PARAMS);
    
  if((image == NULL) || (pVersion == NULL))
    return (sa_ERR_PARAMS);
    
  //Sa_osalMtCsEnter(&mtKey); 
  
  /* Enable PKA Program RAM access */
  pHostRegs->PKA_CLK_CTRL = 0x00000002UL;
  pPkaCtrlRegs->PKA_SEQ_CTRL |= CSL_EIP_29T2_RAM_PKA_SEQ_CTRL_RESET_MASK;
  
  /* Copy the firmware image into the PKA Program RAM */
  for (i = 0; i < sizeWords; i++)
  {
    iram[i] = pImage[i];  
  } 
  
  /* Trigger firmware image by taking it out of reset, check the return code */
  pPkaCtrlRegs->PKA_SEQ_CTRL &= ~CSL_EIP_29T2_RAM_PKA_SEQ_CTRL_RESET_MASK;
  pHostRegs->PKA_CLK_CTRL = 0x00000000UL;
  
  /* Wait for PKA to return init OK */
  for (i = 0; i < SA_PKA_MAX_FIRMWARE_LOOP_COUNT; i++)  {
    if (pPkaCtrlRegs->PKA_SEQ_CTRL == SA_PKA_FIRMWARE_INIT_CODE)  {
      break;
    }
  }
  
  if (i >= SA_PKA_MAX_FIRMWARE_LOOP_COUNT)
  {
    //Sa_osalMtCsExit(mtKey);
    return(sa_ERR_PKA_DOWNLOAD_FAIL);
  }
  
  /* Enable PKA Program RAM access */
  pHostRegs->PKA_CLK_CTRL = 0x00000002UL;
  pPkaCtrlRegs->PKA_SEQ_CTRL |= CSL_EIP_29T2_RAM_PKA_SEQ_CTRL_RESET_MASK;
  
  /* Verify the firmware image at the PKA Program RAM */
  for (i = 0; i < sizeWords; i++)
  {
    if (iram[i] != pImage[i])
    {
        //Sa_osalMtCsExit(mtKey);
        return(sa_ERR_PKA_DOWNLOAD_FAIL);
    }  
  } 
  
  /* Trigger firmware image by taking it out of reset, check the return code */
  pPkaCtrlRegs->PKA_SEQ_CTRL &= ~CSL_EIP_29T2_RAM_PKA_SEQ_CTRL_RESET_MASK;
  pHostRegs->PKA_CLK_CTRL = 0x00000000UL;
  
  /* Wait for PKA to return init OK again */
  for (i = 0; i < SA_PKA_MAX_FIRMWARE_LOOP_COUNT; i++)  {
    if (pPkaCtrlRegs->PKA_SEQ_CTRL == SA_PKA_FIRMWARE_INIT_CODE)  {
      break;
    }
  }
  
  if (i >= SA_PKA_MAX_FIRMWARE_LOOP_COUNT)
  {
    //Sa_osalMtCsExit(mtKey);
    return(sa_ERR_PKA_DOWNLOAD_FAIL);
  }
  
  /* Read PKA version */
  *pVersion = pPkaCtrlRegs->PKA_SW_REV;

  //Sa_osalMtCsExit(mtKey);
  return (sa_ERR_OK);

} /* Sa_pkaDownloadImage */

/****************************************************************************
 * FUNCTION PURPOSE: PKA Basic Operation setup
 ****************************************************************************
 * DESCRIPTION: Setup and initialize PKA basic operations perfroming by the 
 *              PKCP module.
 *              - Perform operation-specific input parameter check
 *              - Setup PKCP registers and PKA RAM per operation
 *              - Trigger the operation 
 *
 *  int16_t salld_pka_op_pkcp_setup (
 *            salldObj_t*       inst,         -> Pointer to SALLD instance
 *            Sa_PkaReqInfo2_t* pPkaReqInfo   -> Pointer to the PKA operation 
 *                                               request information structure
 *                                 )
 * Return values:   sa_ERR_OK
 *                  sa_ERR_PARAMS
 *                  sa_ERR_PKA_OP_UNSUPPORTED
 *
 ***************************************************************************/
static int16_t salld_pka_op_pkcp_setup(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo)
{
    uintptr_t  baseAddr = (uintptr_t) salldLObj.baseAddr;
    CSL_Cp_aceRegs*  pSaRegs = (CSL_Cp_aceRegs*)baseAddr;
    CSL_Eip_29t2_ramEip_27b_eip27_registersRegs* pPkcpRegs = &pSaRegs->PKA.EIP_27B_EIP27_REGISTERS;  
    Sa_PkaBasicOpParams_t* pPkaOp = &pPkaReqInfo->params.basicOp;
    uint32_t op_code;
    
    if ((pPkaReqInfo->operation != sa_PKA_OP_COMP) && (pPkaOp->pResult == NULL))
    {
        return(sa_ERR_PARAMS);
    }
    else if (pPkcpRegs->PKA_FUNCTION & CSL_EIP_29T2_RAM_PKA_FUNCTION_RUN_MASK)
    {
        return(sa_ERR_PKA_NOT_READY);
    }
      
    /* 
     * Perform the following common operations
     * - Copy data for oprand A and B
     * - Configure registers for oprand A & B
     */
     
    if ((!pPkaReqInfo->aLen) || (!pPkaOp->pA))
    {
        return (sa_ERR_PARAMS);
    }  
    
    if (pPkaReqInfo->aLen > sa_MAX_PKA_OPERAND_SIZE)
    {
        return (sa_ERR_PARAMS);
    } 
        
    sap_pka_write((volatile uint32_t *)(baseAddr + SA_PKA_A_BASE),  
           pPkaOp->pA, pPkaReqInfo->aLen);
    pPkcpRegs->PKA_ALENGTH = pPkaReqInfo->aLen;
    
    if (pPkaReqInfo->bLen && (pPkaReqInfo->operation != sa_PKA_OP_ADD_SUB))
    {
        if (!pPkaOp->pB)
        {
            return (sa_ERR_PARAMS);
        }    
        
        if (pPkaReqInfo->bLen > sa_MAX_PKA_OPERAND_SIZE)
        {
            return (sa_ERR_PARAMS);
        } 
    
        sap_pka_write((volatile uint32_t *)(baseAddr + SA_PKA_B_BASE), 
               pPkaOp->pB, pPkaReqInfo->bLen);
        pPkcpRegs->PKA_BLENGTH = pPkaReqInfo->bLen;
        pPkcpRegs->PKA_BPTR = SA_PKA_B_OFFSET;
        
    }
    
    pPkcpRegs->PKA_APTR = SA_PKA_A_OFFSET;
    pPkcpRegs->PKA_BPTR = SA_PKA_B_OFFSET;
    pPkcpRegs->PKA_CPTR = SA_PKA_C_OFFSET;
    pPkcpRegs->PKA_DPTR = SA_PKA_D_OFFSET;
    
    switch (pPkaReqInfo->operation)
    {
        case sa_PKA_OP_ADD:
            /* Range check: Alength > 0; Blength > 0 */
            if (!pPkaReqInfo->bLen)
            {
                return (sa_ERR_PARAMS);
            }    
        
            op_code = CSL_EIP_29T2_RAM_PKA_FUNCTION_ADD_MASK |
                      CSL_EIP_29T2_RAM_PKA_FUNCTION_RUN_MASK;
            break;
        
        case sa_PKA_OP_SUB:
            /* Range check: Alength > 0; Blength > 0; Alength >= Blength */
            if ((!pPkaReqInfo->bLen) || 
                (pPkaReqInfo->aLen < pPkaReqInfo->bLen))
            {
                return (sa_ERR_PARAMS);
            }    
            
            /* A < B ?*/
            if (sap_pka_is_zero(&pPkaOp->pA[pPkaReqInfo->bLen], pPkaReqInfo->aLen - pPkaReqInfo->bLen) &&
                (sap_pka_compare(pPkaOp->pA, pPkaOp->pB, pPkaReqInfo->bLen) == sa_PKA_COMP_AINFB))
            {
                return (sa_ERR_PARAMS);
            }
        
            op_code = CSL_EIP_29T2_RAM_PKA_FUNCTION_SUBTRACT_MASK |
                      CSL_EIP_29T2_RAM_PKA_FUNCTION_RUN_MASK;
            break;
            
        case sa_PKA_OP_ADD_SUB:
            /* Range check: Alength > 0; Operand B and Operand C present and Blength is ingored */
            if ((!pPkaOp->pB) || (!pPkaOp->pC)) 
            {
                return (sa_ERR_PARAMS);
            }  
            else
            {
                sap_pka_write((volatile uint32_t *)(baseAddr + SA_PKA_B_BASE), pPkaOp->pB, pPkaReqInfo->aLen);
                sap_pka_write((volatile uint32_t *)(baseAddr + SA_PKA_C_BASE), pPkaOp->pC, pPkaReqInfo->aLen);
            }  
        
            op_code = CSL_EIP_29T2_RAM_PKA_FUNCTION_ADDSUB_MASK |
                      CSL_EIP_29T2_RAM_PKA_FUNCTION_RUN_MASK;
            break;

        case sa_PKA_OP_MUL:
            /* Range check: Alength > 0; Blength > 0 */
            if (!pPkaReqInfo->bLen)
            {
                return (sa_ERR_PARAMS);
            } 
               
            op_code = CSL_EIP_29T2_RAM_PKA_FUNCTION_MULTIPLY_MASK |
                      CSL_EIP_29T2_RAM_PKA_FUNCTION_RUN_MASK;
            break;
        
        case sa_PKA_OP_DIV:
        case sa_PKA_OP_MOD:
            /* 
             * Alength > 1; Blength > 1
             * B != 0  
             * Alength >= Blength
             * B[Blength - 1] != 0 ==> B != 0
             */
            if ((pPkaReqInfo->aLen <= 1) ||
                (pPkaReqInfo->bLen <= 1) || 
                (pPkaReqInfo->aLen < pPkaReqInfo->bLen) ||
                (pPkaOp->pB[pPkaReqInfo->bLen - 1] == 0) ||
                (!pPkaOp->pRem))
            {
                return (sa_ERR_PARAMS);
            }    
             
            if (pPkaReqInfo->operation == sa_PKA_OP_DIV)
            { 
                op_code = CSL_EIP_29T2_RAM_PKA_FUNCTION_DIVIDE_MASK |
                          CSL_EIP_29T2_RAM_PKA_FUNCTION_RUN_MASK; 
            }          
            else
            {
                op_code = CSL_EIP_29T2_RAM_PKA_FUNCTION_MODULO_MASK |
                          CSL_EIP_29T2_RAM_PKA_FUNCTION_RUN_MASK; 
            }
            break;
        
        case sa_PKA_OP_RSHIFT:
            pPkcpRegs->PKA_SHIFT = pPkaOp->numShiftBits;
            op_code = CSL_EIP_29T2_RAM_PKA_FUNCTION_RSHIFT_MASK |
                      CSL_EIP_29T2_RAM_PKA_FUNCTION_RUN_MASK;
            break;
        
        case sa_PKA_OP_LSHIFT:
            pPkcpRegs->PKA_SHIFT = pPkaOp->numShiftBits;
            op_code = CSL_EIP_29T2_RAM_PKA_FUNCTION_LSHIFT_MASK |
                      CSL_EIP_29T2_RAM_PKA_FUNCTION_RUN_MASK;
            break;
        
        case sa_PKA_OP_COMP:
            /* Alength == Blength */
            if (pPkaReqInfo->aLen != pPkaReqInfo->bLen)
            {
                return (sa_ERR_PARAMS);
            }
            op_code = CSL_EIP_29T2_RAM_PKA_FUNCTION_COMPARE_MASK |
                      CSL_EIP_29T2_RAM_PKA_FUNCTION_RUN_MASK;
            break;
        
        case sa_PKA_OP_COPY:
            op_code = CSL_EIP_29T2_RAM_PKA_FUNCTION_COPY_MASK |
                      CSL_EIP_29T2_RAM_PKA_FUNCTION_RUN_MASK;
            break;
        
        default:
            return (sa_ERR_PKA_OP_UNSUPPORTED);
    }
  
    /* Initialize the Operation */
    pPkcpRegs->PKA_FUNCTION  = op_code;
    
    return (sa_ERR_OK);
} 

/****************************************************************************
 * FUNCTION PURPOSE: PKA ModExp Operation setup
 ****************************************************************************
 * DESCRIPTION: Setup and initialize PKA ModExp operations perfroming by the 
 *              PKA module.
 *              - Perform operation-specific input parameter check
 *              - Setup PKCP registers and PKA RAM per operation
 *              - Trigger the operation 
 *
 *  int16_t salld_pka_op_modexp_setup (
 *            salldObj_t*       inst,         -> Pointer to SALLD instance
 *            Sa_PkaReqInfo2_t* pPkaReqInfo   -> Pointer to the PKA operation 
 *                                               request information structure
 *                                 )
 * Return values:   sa_ERR_OK
 *                  sa_ERR_PARAMS
 *                  sa_ERR_PKA_WORKSPACE_ERROR
 *
 ***************************************************************************/
static int16_t salld_pka_op_modexp_setup(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo)
{
    uintptr_t baseAddr = (uintptr_t)salldLObj.baseAddr;
    CSL_Cp_aceRegs*  pSaRegs = (CSL_Cp_aceRegs*)baseAddr;
    CSL_Eip_29t2_ramEip_27b_eip27_registersRegs* pPkcpRegs = &pSaRegs->PKA.EIP_27B_EIP27_REGISTERS;  
    Sa_PkaModExpParams_t* pPkaOp = &pPkaReqInfo->params.modExp;
    uint32_t offset = 0, wsSize, wsAvail;
    
    if ((pPkaOp->pE == NULL) || (pPkaOp->pN == NULL) || (pPkaOp->pM == NULL) || (pPkaOp->pResult == NULL))
    {
        return(sa_ERR_PARAMS);
    }
      
    /* 
     * Perform the following parameter checks
     * - 0 < ALen < Max_Len
     * - 1 < BLen < Max_Len
     * - Modulus N must be odd (i.e. the least significant bit must be ONE)
     * - Modulus N > 2**32
     * - Base M < Modulus N 
     * - #of odd powers to use >= 1
     */
    
    if ((pPkaReqInfo->aLen > sa_MAX_PKA_OPERAND_SIZE) || (pPkaReqInfo->aLen == 0))
    {
        return (sa_ERR_PARAMS);
    } 
    
    if ((pPkaReqInfo->bLen > sa_MAX_PKA_OPERAND_SIZE) || (pPkaReqInfo->bLen <= 1))
    {
        return (sa_ERR_PARAMS);
    } 
    
    if ((!(pPkaOp->pN[0] & 0x1)) || (sap_pka_is_zero(&pPkaOp->pN[1], pPkaReqInfo->bLen - 1)))
    {
        return (sa_ERR_PARAMS);
    }
    
    if (pPkaOp->numOddPowers == 0)
    {
        return (sa_ERR_PARAMS);
    }
    
    if (sap_pka_compare(pPkaOp->pM, pPkaOp->pN, pPkaReqInfo->bLen) != sa_PKA_COMP_AINFB)
    {
        return (sa_ERR_PARAMS);
    }
    
    /* Calculate offsets and copy N, E, M to the PKA RAM */
    baseAddr += SA_PKA_A_BASE;
    pPkcpRegs->PKA_BPTR = offset;    
    sap_pka_write((volatile uint32_t *)(baseAddr + (offset << 2)),  
           pPkaOp->pN, pPkaReqInfo->bLen);
           
    offset += (pPkaReqInfo->bLen + 1);
    offset += (offset & 0x1);
    
    pPkcpRegs->PKA_APTR = offset; 
    sap_pka_write((volatile uint32_t *)(baseAddr + (offset << 2)),  
           pPkaOp->pE, pPkaReqInfo->aLen);
    
    offset += (pPkaReqInfo->aLen);
    offset += (offset & 0x1);
    
    pPkcpRegs->PKA_CPTR = offset;    
    sap_pka_write((volatile uint32_t *)(baseAddr + (offset << 2)),  
           pPkaOp->pM, pPkaReqInfo->bLen);
           
    offset += (pPkaReqInfo->bLen + 1);
    offset += (offset & 0x1);
    
    pPkcpRegs->PKA_DPTR = offset;
    
    /* Calculate and check whether there is enough workspace */
    offset += (pPkaReqInfo->bLen + 1);
    offset += (offset & 0x01);
    
    wsAvail = SA_PKA_RAM_SIZE_IN_WORDS - SA_PKA_RAM_SCRATCHPAD_SIZE_IN_WORDS - offset;
    wsSize = max(2*(pPkaReqInfo->bLen + 2U - (pPkaReqInfo->bLen & 0x01U)) + 10U,
                 pPkaOp->numOddPowers*(pPkaReqInfo->bLen + 2U - (pPkaReqInfo->bLen & 0x01U)));
    
    if (wsAvail < wsSize)
    {
        return(sa_ERR_PKA_WORKSPACE_ERROR);
    }
          
    pPkcpRegs->PKA_ALENGTH = pPkaReqInfo->aLen;
    pPkcpRegs->PKA_BLENGTH = pPkaReqInfo->bLen;
    pPkcpRegs->PKA_SHIFT   = pPkaOp->numOddPowers;
  
    /* Initialize the Operation */
    pPkcpRegs->PKA_FUNCTION  = SA_PKA_COMPLEX_OP_MODEXP | CSL_EIP_29T2_RAM_PKA_FUNCTION_RUN_MASK;
    
    return (sa_ERR_OK);
} 

/****************************************************************************
 * FUNCTION PURPOSE: PKA ModExp CRT Operation setup
 ****************************************************************************
 * DESCRIPTION: Setup and initialize PKA ModExp CRT operations perfroming by the 
 *              PKA module.
 *              - Perform operation-specific input parameter check
 *              - Setup PKCP registers and PKA RAM per operation
 *              - Trigger the operation 
 *
 *  int16_t salld_pka_op_modexp_crt_setup (
 *            salldObj_t*       inst,         -> Pointer to SALLD instance
 *            Sa_PkaReqInfo2_t* pPkaReqInfo   -> Pointer to the PKA operation 
 *                                               request information structure
 *                                 )
 * Return values:   sa_ERR_OK
 *                  sa_ERR_PARAMS
 *                  sa_ERR_PKA_WORKSPACE_ERROR
 *
 ***************************************************************************/
static int16_t salld_pka_op_modexp_crt_setup(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo)
{
    uintptr_t baseAddr = (uintptr_t)salldLObj.baseAddr;
    CSL_Cp_aceRegs*  pSaRegs = (CSL_Cp_aceRegs*)baseAddr;
    CSL_Eip_29t2_ramEip_27b_eip27_registersRegs* pPkcpRegs = &pSaRegs->PKA.EIP_27B_EIP27_REGISTERS;  
    Sa_PkaModExpCRTParams_t* pPkaOp = &pPkaReqInfo->params.modExpCRT;
    uint32_t offset = 0, wsSize, wsAvail;
    
    if ((pPkaOp->pDp == NULL) || (pPkaOp->pDq == NULL) || (pPkaOp->pModP == NULL) || (pPkaOp->pModQ == NULL) || 
        (pPkaOp->pQInv == NULL) || (pPkaOp->pM == NULL) || (pPkaOp->pResult == NULL))
    {
        return(sa_ERR_PARAMS);
    }
      
    /* 
     * Perform the following parameter checks
     * - 0 < ALen < Max_Len
     * - 1 < BLen < Max_Len
     * - ModP and ModQ must be odd (i.e. the least significant bit must be ONE)
     * - ModP > ModQ > 2**32
     * - 0 < Exp P < (Mod P - 1)?
     * - 0 < Exp Q < (Mod Q - 1)?
     * - (Q inverse * Mod Q) = 1 (modulo Mod P) (not chcek)
     * - Base M < (ModP * ModQ) (not check) 
     * - #of odd powers to use >= 1
     */
    
    if ((pPkaReqInfo->aLen > sa_MAX_PKA_OPERAND_SIZE) || (pPkaReqInfo->aLen == 0))
    {
        return (sa_ERR_PARAMS);
    } 
    
    if ((pPkaReqInfo->bLen > sa_MAX_PKA_OPERAND_SIZE) || (pPkaReqInfo->bLen <= 1))
    {
        return (sa_ERR_PARAMS);
    } 
    
    if ((!(pPkaOp->pModP[0] & 0x1)) || (sap_pka_is_zero(&pPkaOp->pModP[1], pPkaReqInfo->bLen - 1)))
    {
        return (sa_ERR_PARAMS);
    }
    
    if ((!(pPkaOp->pModQ[0] & 0x1)) || (sap_pka_is_zero(&pPkaOp->pModQ[1], pPkaReqInfo->bLen - 1)))
    {
        return (sa_ERR_PARAMS);
    }
    
    
    if (pPkaOp->numOddPowers == 0)
    {
        return (sa_ERR_PARAMS);
    }
    
    if (sap_pka_compare(pPkaOp->pModP, pPkaOp->pModQ, pPkaReqInfo->bLen) != sa_PKA_COMP_ASUPB)
    {
        return (sa_ERR_PARAMS);
    }
    
    /* Calculate offsets and copy (modP, modQ), (Dp, Dq), qInv and M to PKA RAM */
    baseAddr += SA_PKA_A_BASE;
    pPkcpRegs->PKA_BPTR = offset;    
    sap_pka_write((volatile uint32_t *)(baseAddr + (offset << 2)),  
           pPkaOp->pModP, pPkaReqInfo->bLen);
           
    offset += (pPkaReqInfo->bLen+1);
    offset += (offset & 0x1);
    
    sap_pka_write((volatile uint32_t *)(baseAddr + (offset << 2)),  
           pPkaOp->pModQ, pPkaReqInfo->bLen);
           
    offset += (pPkaReqInfo->bLen+1);
    offset += (offset & 0x1);
    
    pPkcpRegs->PKA_APTR = offset; 
    sap_pka_write((volatile uint32_t *)(baseAddr + (offset << 2)),  
           pPkaOp->pDp, pPkaReqInfo->aLen);
    
    offset += (pPkaReqInfo->aLen);
    offset += (offset & 0x1);
    
    sap_pka_write((volatile uint32_t *)(baseAddr + (offset << 2)),  
           pPkaOp->pDq, pPkaReqInfo->aLen);
    
    offset += (pPkaReqInfo->aLen);
    offset += (offset & 0x1);
    
    pPkcpRegs->PKA_CPTR = offset;    
    sap_pka_write((volatile uint32_t *)(baseAddr + (offset << 2)),  
           pPkaOp->pQInv, pPkaReqInfo->bLen);
           
    offset += (pPkaReqInfo->bLen);
    offset += (offset & 0x1);
    
    pPkcpRegs->PKA_DPTR = offset;
    sap_pka_write((volatile uint32_t *)(baseAddr + (offset << 2)),  
           pPkaOp->pM, pPkaReqInfo->bLen<<1);
           
    /* Calculate and check whether there is enough workspace */
    offset += (pPkaReqInfo->bLen*2 + 1);
    offset += (offset & 0x1);
    
    wsAvail = SA_PKA_RAM_SIZE_IN_WORDS - SA_PKA_RAM_SCRATCHPAD_SIZE_IN_WORDS - offset;
    wsSize = max(3*(pPkaReqInfo->bLen + 2U - (pPkaReqInfo->bLen & 0x01U)) + 10U,
                 (pPkaOp->numOddPowers + 1U)*(pPkaReqInfo->bLen + 2U - (pPkaReqInfo->bLen & 0x01U)));
    
    if (wsAvail < wsSize)
    {
        return(sa_ERR_PKA_WORKSPACE_ERROR);
    }
          
    pPkcpRegs->PKA_ALENGTH = pPkaReqInfo->aLen;
    pPkcpRegs->PKA_BLENGTH = pPkaReqInfo->bLen;
    pPkcpRegs->PKA_SHIFT   = pPkaOp->numOddPowers;
  
    /* Initialize the Operation */
    pPkcpRegs->PKA_FUNCTION  = SA_PKA_COMPLEX_OP_MODEXP_CRT | CSL_EIP_29T2_RAM_PKA_FUNCTION_RUN_MASK;
    
    return (sa_ERR_OK);
} 

/****************************************************************************
 * FUNCTION PURPOSE: PKA ModInv Operation setup
 ****************************************************************************
 * DESCRIPTION: Setup and initialize PKA ModInv operations perfroming by the 
 *              PKA module.
 *              - Perform operation-specific input parameter check
 *              - Setup PKCP registers and PKA RAM per operation
 *              - Trigger the operation 
 *
 *  int16_t salld_pka_op_modinvp_setup (
 *            salldObj_t*       inst,         -> Pointer to SALLD instance
 *            Sa_PkaReqInfo2_t* pPkaReqInfo   -> Pointer to the PKA operation 
 *                                               request information structure
 *                                 )
 * Return values:   sa_ERR_OK
 *                  sa_ERR_PARAMS
 *                  sa_ERR_PKA_WORKSPACE_ERROR
 *
 ***************************************************************************/
static int16_t salld_pka_op_modinv_setup(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo)
{
    uintptr_t baseAddr = (uintptr_t)salldLObj.baseAddr;
    CSL_Cp_aceRegs*  pSaRegs = (CSL_Cp_aceRegs*)baseAddr;
    CSL_Eip_29t2_ramEip_27b_eip27_registersRegs* pPkcpRegs = &pSaRegs->PKA.EIP_27B_EIP27_REGISTERS;  
    Sa_PkaModInvParams_t* pPkaOp = &pPkaReqInfo->params.modInv;
    uint32_t offset = 0;
    
    if ((pPkaOp->pN == NULL) || (pPkaOp->pZ == NULL) || (pPkaOp->pResult == NULL))
    {
        return(sa_ERR_PARAMS);
    }
      
    /* 
     * Perform the following parameter checks
     *  - ModINVp: 0 < ALen <= Max Len (130)  
     *  - ModINVp: 0 < BLen <= Max Len (130)
     *  - ModINV2m: 0 < ALen <= 18
     *  - ModINV2m: 0 < BLen <= 18
     *  - ModINV2m: Z < Modulus N
     *  - Modulus N must be odd (i.e. the least significant bit must be ONE)
     *  - Modulus N should not be 1
     *  - The highest word of the modulus vector, as indicated by BLen, may not be zero.
     */
    
    if (pPkaReqInfo->operation == sa_PKA_OP_MODINVp)
    {
        if ((pPkaReqInfo->aLen > sa_MAX_PKA_OPERAND_SIZE) || (pPkaReqInfo->aLen == 0))
        {
            return (sa_ERR_PARAMS);
        } 
    
        if ((pPkaReqInfo->bLen > sa_MAX_PKA_OPERAND_SIZE) || (pPkaReqInfo->bLen == 0))
        {
            return (sa_ERR_PARAMS);
        } 
    }
    else
    {
        if ((pPkaReqInfo->aLen > sa_MAX_PKA_OPERAND_SIZE_EC2m) || (pPkaReqInfo->aLen == 0))
        {
            return (sa_ERR_PARAMS);
        } 
    
        if ((pPkaReqInfo->bLen > sa_MAX_PKA_OPERAND_SIZE_EC2m) || (pPkaReqInfo->bLen == 0))
        {
            return (sa_ERR_PARAMS);
        } 
        
        /* Z < N ? */
        if ((pPkaReqInfo->aLen > pPkaReqInfo->bLen) && 
           (!sap_pka_is_zero(&pPkaOp->pZ[pPkaReqInfo->bLen], pPkaReqInfo->aLen - pPkaReqInfo->bLen)))
        {
            return(sa_ERR_PARAMS);
        }
        else if ((pPkaReqInfo->aLen >= pPkaReqInfo->bLen) &&
                 (sap_pka_compare(pPkaOp->pZ, pPkaOp->pN, pPkaReqInfo->bLen) != sa_PKA_COMP_AINFB))
        {
            return(sa_ERR_PARAMS);
        }
    }
    
    if ((!(pPkaOp->pN[0] & 0x1)) || (!pPkaOp->pN[pPkaReqInfo->bLen - 1]))
    {
        return (sa_ERR_PARAMS);
    }
    
    if ((pPkaReqInfo->bLen == 1) && (pPkaOp->pN[0] == 1))
    {
        return (sa_ERR_PARAMS);
    }
    
    /* Calculate offsets and copy N, Z to PKA RAM */
    baseAddr += SA_PKA_A_BASE;
    pPkcpRegs->PKA_BPTR = offset;    
    sap_pka_write((volatile uint32_t *)(baseAddr + (offset << 2)),  
           pPkaOp->pN, pPkaReqInfo->bLen);
           
    offset += (pPkaReqInfo->bLen);
    offset += (offset & 0x1);
    
    pPkcpRegs->PKA_APTR = offset; 
    sap_pka_write((volatile uint32_t *)(baseAddr + (offset << 2)),  
           pPkaOp->pZ, pPkaReqInfo->aLen);
    
    offset += (pPkaReqInfo->aLen);
    offset += (offset & 0x1);
    
    pPkcpRegs->PKA_DPTR = offset;
    
    /* Calculate and check whether there is enough workspace 
     * ModINVp: 5 * (M + 2 + (M MOD 2)), with M = Max(ALen, BLen)
     * Skip workspac check since there is always enough workspace with 2K-words PKA RAM 
     *
    offset += (pPkaReqInfo->bLen);
    offset += (offset & 0x01);
    
    wsAvail = SA_PKA_RAM_SIZE_IN_WORDS - SA_PKA_RAM_SCRATCHPAD_SIZE_IN_WORDS - offset;
    wsSize = 5*(max(pPkaReqInfo->aLen, pPkaReqInfo->bLen) + 2 + (max(pPkaReqInfo->aLen, pPkaReqInfo->bLen) & 0x01))
    
    if (wsAvail < wsSize)
    {
        return(sa_ERR_PKA_WORKSPACE_ERROR);
    }
     */
          
    pPkcpRegs->PKA_ALENGTH = pPkaReqInfo->aLen;
    pPkcpRegs->PKA_BLENGTH = pPkaReqInfo->bLen;
  
    /* Initialize the Operation */
    if (pPkaReqInfo->operation == sa_PKA_OP_MODINVp)
    {
        pPkcpRegs->PKA_FUNCTION  = SA_PKA_COMPLEX_OP_MODINVp | CSL_EIP_29T2_RAM_PKA_FUNCTION_RUN_MASK;
    }
    else
    {
        pPkcpRegs->PKA_FUNCTION  = SA_PKA_COMPLEX_OP_MODINV2m | CSL_EIP_29T2_RAM_PKA_FUNCTION_RUN_MASK;
    }
    
    return (sa_ERR_OK);
} 

/****************************************************************************
 * FUNCTION PURPOSE: PKA ECp Add Operation setup
 ****************************************************************************
 * DESCRIPTION: Setup and initialize PKA ECpAdd operations perfroming by the 
 *              PKA module.
 *              - Perform operation-specific input parameter check
 *              - Setup PKCP registers and PKA RAM per operation
 *              - Trigger the operation 
 *
 *  int16_t salld_pka_op_ecp_add_setup (
 *            salldObj_t*       inst,         -> Pointer to SALLD instance
 *            Sa_PkaReqInfo2_t* pPkaReqInfo   -> Pointer to the PKA operation 
 *                                               request information structure
 *                                 )
 * Return values:   sa_ERR_OK
 *                  sa_ERR_PARAMS
 *                  sa_ERR_PKA_WORKSPACE_ERROR
 *
 ***************************************************************************/
static int16_t salld_pka_op_ecp_add_setup(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo)
{
    uintptr_t baseAddr = (uintptr_t)salldLObj.baseAddr;
    CSL_Cp_aceRegs*  pSaRegs = (CSL_Cp_aceRegs*)baseAddr;
    CSL_Eip_29t2_ramEip_27b_eip27_registersRegs* pPkcpRegs = &pSaRegs->PKA.EIP_27B_EIP27_REGISTERS;  
    Sa_PkaECAddParams_t* pPkaOp = &pPkaReqInfo->params.ecAdd;
    uint32_t offset = 0;
    
    if ((pPkaOp->pModP == NULL) || (pPkaOp->pEcA == NULL) || (pPkaOp->pEcB == NULL) || 
        (pPkaOp->point1.pX == NULL) || (pPkaOp->point1.pY == NULL) || (pPkaOp->point1.pZ == NULL) || 
        (pPkaOp->point2.pX == NULL) || (pPkaOp->point2.pY == NULL) || (pPkaOp->point2.pZ == NULL) || 
        (pPkaOp->point0.pX == NULL) || (pPkaOp->point0.pY == NULL) || (pPkaOp->point0.pZ == NULL))  
    {
        return(sa_ERR_PARAMS);
    }
      
    /* 
     * Perform the following parameter checks
     * - 1 < BLen <= 24 (maximum vector length is 768 bits)
     * - Modulus P must be a prime > 2**63
     * - The highest word of the modulus vector, as indicated by BLen, may not be zero.
     * - a < p and b < p
     * - P1 and P2 must be on the curve (not check) 
     */
    
    if ((pPkaReqInfo->bLen > sa_MAX_PKA_OPERAND_SIZE_ECp) || (pPkaReqInfo->bLen <= 1))
    {
        return (sa_ERR_PARAMS);
    } 
    
    if ((!(pPkaOp->pModP[0] & 0x1)) || (!pPkaOp->pModP[pPkaReqInfo->bLen - 1]))
    {
        return (sa_ERR_PARAMS);
    }
    
    if ((pPkaReqInfo->bLen == 2) && ((pPkaOp->pModP[1] & 0x80000000UL) == 0))
    {
        return (sa_ERR_PARAMS);
    }
    
    if (sap_pka_compare(pPkaOp->pEcA, pPkaOp->pModP, pPkaReqInfo->bLen) != sa_PKA_COMP_AINFB)
    {
        return (sa_ERR_PARAMS);
    }
    
    if (sap_pka_compare(pPkaOp->pEcB, pPkaOp->pModP, pPkaReqInfo->bLen) != sa_PKA_COMP_AINFB)
    {
        return (sa_ERR_PARAMS);
    }
    
    /* Calculate offsets and copy (p, a, b), P1xyz, P2xyz to PKA RAM */
    baseAddr += SA_PKA_A_BASE;
    pPkcpRegs->PKA_BPTR = offset; /* p, a, b */  
    sap_pka_write((volatile uint32_t *)(baseAddr + (offset << 2)),  
           pPkaOp->pModP, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen + 2);
    offset += (offset & 0x1);
    
    sap_pka_write((volatile uint32_t *)(baseAddr + (offset << 2)),  
           pPkaOp->pEcA, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen + 2);
    offset += (offset & 0x1);
    
    sap_pka_write((volatile uint32_t *)(baseAddr + (offset << 2)),  
           pPkaOp->pEcB, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen + 2);
    offset += (offset & 0x1);
    
    pPkcpRegs->PKA_APTR = offset; /* P1xyz */
    sap_pka_write((volatile uint32_t *)(baseAddr + (offset << 2)),  
           pPkaOp->point1.pX, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen + 2);
    offset += (offset & 0x1);
    
    sap_pka_write((volatile uint32_t *)(baseAddr + (offset << 2)),  
           pPkaOp->point1.pY, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen + 2);
    offset += (offset & 0x1);
    
    sap_pka_write((volatile uint32_t *)(baseAddr + (offset << 2)),  
           pPkaOp->point1.pZ, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen + 2);
    offset += (offset & 0x1);
    
    pPkcpRegs->PKA_CPTR = offset; /* P2xyz */ 
    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),  
           pPkaOp->point2.pX, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen + 2);
    offset += (offset & 0x1);
    
    sap_pka_write((volatile uint32_t *)(baseAddr + (offset << 2)),  
           pPkaOp->point2.pY, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen + 2);
    offset += (offset & 0x1);
    
    sap_pka_write((volatile uint32_t *)(baseAddr + (offset << 2)),  
           pPkaOp->point2.pZ, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen + 2);
    offset += (offset & 0x1);
    
    pPkcpRegs->PKA_DPTR = offset;  /* P0xyx */
    
    /* Calculate and check whether there is enough workspace 
     * Wokspace: 15*(BLen + 2 + BLen MOD 2)
     * Maximum worksapce: 15*(24 + 2 + 0) = 380 words 
     *
    offset += (pPkaReqInfo->bLen + 2 + (pPkaReqInfo->bLen & 0x01))*3;
    
    wsAvail = SA_PKA_RAM_SIZE_IN_WORDS - SA_PKA_RAM_SCRATCHPAD_SIZE_IN_WORDS - offset;
    wsSize = 15*(pPkaReqInfo->bLen + 2 - (pPkaReqInfo->bLen & 0x01))
    
    if (wsAvail < wsSize)
    {
        return(sa_ERR_PKA_WORKSPACE_ERROR);
    }
    */
          
    pPkcpRegs->PKA_ALENGTH = pPkcpRegs->PKA_BLENGTH = pPkaReqInfo->bLen;
  
    /* Initialize the Operation */
    pPkcpRegs->PKA_FUNCTION  = SA_PKA_COMPLEX_OP_ECp_ADD | CSL_EIP_29T2_RAM_PKA_FUNCTION_RUN_MASK;
    
    return (sa_ERR_OK);
} 

/****************************************************************************
 * FUNCTION PURPOSE: PKA ECp Multiply Operation setup
 ****************************************************************************
 * DESCRIPTION: Setup and initialize PKA ECpMul operations perfroming by the 
 *              PKA module.
 *              - Perform operation-specific input parameter check
 *              - Setup PKCP registers and PKA RAM per operation
 *              - Trigger the operation 
 *
 *  int16_t salld_pka_op_ecp_mul_setup (
 *            salldObj_t*       inst,         -> Pointer to SALLD instance
 *            Sa_PkaReqInfo2_t* pPkaReqInfo   -> Pointer to the PKA operation 
 *                                               request information structure
 *                                 )
 * Return values:   sa_ERR_OK
 *                  sa_ERR_PARAMS
 *                  sa_ERR_PKA_WORKSPACE_ERROR
 *
 ***************************************************************************/
static int16_t salld_pka_op_ecp_mul_setup(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo)
{
    uintptr_t baseAddr = (uintptr_t)salldLObj.baseAddr;
    CSL_Cp_aceRegs*  pSaRegs = (CSL_Cp_aceRegs*)baseAddr;
    CSL_Eip_29t2_ramEip_27b_eip27_registersRegs* pPkcpRegs = &pSaRegs->PKA.EIP_27B_EIP27_REGISTERS;  
    Sa_PkaECMulParams_t* pPkaOp = &pPkaReqInfo->params.ecMul;
    uint32_t offset = 0;
    
    if ((pPkaOp->pK == NULL) || (pPkaOp->pModP == NULL) || (pPkaOp->pEcA == NULL) || (pPkaOp->pEcBC == NULL) || 
        (pPkaOp->point1.pX == NULL) || (pPkaOp->point1.pY == NULL) || (pPkaOp->point1.pZ == NULL) || 
        (pPkaOp->point0.pX == NULL) || (pPkaOp->point0.pY == NULL) || (pPkaOp->point0.pZ == NULL))  
    {
        return (sa_ERR_PARAMS);
    }
      
    /* 
     * Perform the following parameter checks
     * - 0 < ALen <= 24 (maximum vector length is 768 bits)
     * - 1 < BLen <= 24 (maximum vector length is 768 bits)
     * - Modulus P must be a prime > 2**63
     * - The highest word of the modulus vector, as indicated by BLen, may not be zero.
     * - a < p and b < p
     * - P1z = 1
     * - 1 < k <= n, where n is the curve's order. (not check)
     * - P1 must be on the curve (not check) 
     */
    
    if ((pPkaReqInfo->aLen > sa_MAX_PKA_OPERAND_SIZE_ECp) || (pPkaReqInfo->aLen < 1))
    {
        return (sa_ERR_PARAMS);
    } 
    
    if ((pPkaReqInfo->bLen > sa_MAX_PKA_OPERAND_SIZE_ECp) || (pPkaReqInfo->bLen <= 1))
    {
        return (sa_ERR_PARAMS);
    } 
    
    if ((!(pPkaOp->pModP[0] & 0x1)) || (!pPkaOp->pModP[pPkaReqInfo->bLen - 1]))
    {
        return (sa_ERR_PARAMS);
    }
    
    if ((pPkaReqInfo->bLen == 2) && ((pPkaOp->pModP[1] & 0x80000000UL) == 0))
    {
        return (sa_ERR_PARAMS);
    }
    
    if (sap_pka_compare(pPkaOp->pEcA, pPkaOp->pModP, pPkaReqInfo->bLen) != sa_PKA_COMP_AINFB)
    {
        return (sa_ERR_PARAMS);
    }
    
    if (sap_pka_compare(pPkaOp->pEcBC, pPkaOp->pModP, pPkaReqInfo->bLen) != sa_PKA_COMP_AINFB)
    {
        return (sa_ERR_PARAMS);
    }
    
    if ((pPkaOp->point1.pZ[0] != 1) || (!sap_pka_is_zero(&pPkaOp->point1.pZ[1], pPkaReqInfo->bLen-1)))
    {
        return (sa_ERR_PARAMS);
    }
    
    /* Calculate offsets and copy (p, a, b), K, P1xyz to PKA RAM */
    baseAddr += SA_PKA_A_BASE;
    pPkcpRegs->PKA_BPTR = offset; /* p, a, b */  
    sap_pka_write((volatile uint32_t *)(baseAddr + (offset << 2)),  
           pPkaOp->pModP, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen + 2);
    offset += (offset & 0x1);
    
    sap_pka_write((volatile uint32_t *)(baseAddr + (offset << 2)),  
           pPkaOp->pEcA, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen + 2);
    offset += (offset & 0x1);
    
    sap_pka_write((volatile uint32_t *)(baseAddr + (offset << 2)),  
           pPkaOp->pEcBC, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen + 2);
    offset += (offset & 0x1);
    
    pPkcpRegs->PKA_APTR = offset; /* K */
    sap_pka_write((volatile uint32_t *)(baseAddr + (offset << 2)),  
           pPkaOp->pK, pPkaReqInfo->aLen);
    offset += (pPkaReqInfo->aLen);
    offset += (offset & 0x1);
    
    pPkcpRegs->PKA_CPTR = offset; /* P1xyz */ 
    sap_pka_write((volatile uint32_t *)(baseAddr + (offset << 2)),  
           pPkaOp->point1.pX, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen + 2);
    offset += (offset & 0x1);
    
    sap_pka_write((volatile uint32_t *)(baseAddr + (offset << 2)),  
           pPkaOp->point1.pY, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen + 2);
    offset += (offset & 0x1);
    
    sap_pka_write((volatile uint32_t *)(baseAddr + (offset << 2)),  
           pPkaOp->point1.pZ, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen + 2);
    offset += (offset & 0x1);
    
    pPkcpRegs->PKA_DPTR = offset;  /* P0xyx */
    
    /* Calculate and check whether there is enough workspace 
     * Wokspace: 15*(BLen + 2 + BLen MOD 2)
     * Maximum worksapce: 15*(24 + 2 + 0) = 380 words 
     *
    offset += (pPkaReqInfo->bLen + 2 + (pPkaReqInfo->bLen & 0x01))*3;
    
    wsAvail = SA_PKA_RAM_SIZE_IN_WORDS - SA_PKA_RAM_SCRATCHPAD_SIZE_IN_WORDS - offset;
    wsSize = 15*(pPkaReqInfo->bLen + 2 - (pPkaReqInfo->bLen & 0x01))
    
    if (wsAvail < wsSize)
    {
        return(sa_ERR_PKA_WORKSPACE_ERROR);
    }
    */
          
    pPkcpRegs->PKA_ALENGTH = pPkaReqInfo->aLen;
    pPkcpRegs->PKA_BLENGTH = pPkaReqInfo->bLen;
  
    /* Initialize the Operation */
    pPkcpRegs->PKA_FUNCTION  = SA_PKA_COMPLEX_OP_ECp_MUL | CSL_EIP_29T2_RAM_PKA_FUNCTION_RUN_MASK;
    
    return (sa_ERR_OK);
} 

/****************************************************************************
 * FUNCTION PURPOSE: PKA ECp Scale Operation setup
 ****************************************************************************
 * DESCRIPTION: Setup and initialize PKA ECpScale operations perfroming by the 
 *              PKA module.
 *              - Perform operation-specific input parameter check
 *              - Setup PKCP registers and PKA RAM per operation
 *              - Trigger the operation 
 *
 *  int16_t salld_pka_op_ecp_mul_setup (
 *            salldObj_t*       inst,         -> Pointer to SALLD instance
 *            Sa_PkaReqInfo2_t* pPkaReqInfo   -> Pointer to the PKA operation 
 *                                               request information structure
 *                                 )
 * Return values:   sa_ERR_OK
 *                  sa_ERR_PARAMS
 *                  sa_ERR_PKA_WORKSPACE_ERROR
 *
 ***************************************************************************/
static int16_t salld_pka_op_ecp_scale_setup(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo)
{
    uintptr_t baseAddr = (uintptr_t)salldLObj.baseAddr;
    CSL_Cp_aceRegs*  pSaRegs = (CSL_Cp_aceRegs*)baseAddr;
    CSL_Eip_29t2_ramEip_27b_eip27_registersRegs* pPkcpRegs = &pSaRegs->PKA.EIP_27B_EIP27_REGISTERS;  
    Sa_PkaECScaleParams_t* pPkaOp = &pPkaReqInfo->params.ecScale;
    uint32_t offset = 0;
    
    if ((pPkaOp->pModP == NULL) || (pPkaOp->pEcA == NULL) || (pPkaOp->pEcB == NULL) || 
        (pPkaOp->point1.pX == NULL) || (pPkaOp->point1.pY == NULL) || (pPkaOp->point1.pZ == NULL) || 
        (pPkaOp->point0.pX == NULL) || (pPkaOp->point0.pY == NULL) || (pPkaOp->point0.pZ == NULL))  
    {
        return (sa_ERR_PARAMS);
    }
      
    /* 
     * Perform the following parameter checks
     * - 1 < BLen <= 24 (maximum vector length is 768 bits)
     * - Modulus P must be a prime > 2**63
     * - The highest word of the modulus vector, as indicated by BLen, may not be zero.
     * - a < p and b < p (?)
     */
    if ((pPkaReqInfo->bLen > sa_MAX_PKA_OPERAND_SIZE_ECp) || (pPkaReqInfo->bLen <= 1))
    {
        return (sa_ERR_PARAMS);
    } 
    
    if ((!(pPkaOp->pModP[0] & 0x1)) || (!pPkaOp->pModP[pPkaReqInfo->bLen - 1]))
    {
        return (sa_ERR_PARAMS);
    }
    
    if ((pPkaReqInfo->bLen == 2) && ((pPkaOp->pModP[1] & 0x80000000UL) == 0))
    {
        return (sa_ERR_PARAMS);
    }
    
    if (sap_pka_compare(pPkaOp->pEcA, pPkaOp->pModP, pPkaReqInfo->bLen) != sa_PKA_COMP_AINFB)
    {
        return (sa_ERR_PARAMS);
    }
    
    if (sap_pka_compare(pPkaOp->pEcB, pPkaOp->pModP, pPkaReqInfo->bLen) != sa_PKA_COMP_AINFB)
    {
        return (sa_ERR_PARAMS);
    }
    
    /* Calculate offsets and copy (p, a, b), P1xyz to PKA RAM */
    baseAddr += SA_PKA_A_BASE;
    pPkcpRegs->PKA_BPTR = offset; /* p, a, b */  
    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),  
           pPkaOp->pModP, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen + 2);
    offset += (offset & 0x1);
    
    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),  
           pPkaOp->pEcA, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen + 2);
    offset += (offset & 0x1);
    
    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),  
           pPkaOp->pEcB, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen + 2);
    offset += (offset & 0x1);
    
    pPkcpRegs->PKA_DPTR = offset; /* P1xyz and P0xyx*/ 
    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),  
           pPkaOp->point1.pX, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen + 2);
    offset += (offset & 0x1);
    
    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),  
           pPkaOp->point1.pY, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen + 2);
    offset += (offset & 0x1);
    
    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),  
           pPkaOp->point1.pZ, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen + 2);
    offset += (offset & 0x1);
    
    /* Calculate and check whether there is enough workspace 
     * Wokspace: 15*(BLen + 2 + BLen MOD 2)
     * Maximum worksapce: 15*(24 + 2 + 0) = 380 words 
     *
    offset += (pPkaReqInfo->bLen + 2 + (pPkaReqInfo->bLen & 0x01))*3;
    
    wsAvail = SA_PKA_RAM_SIZE_IN_WORDS - SA_PKA_RAM_SCRATCHPAD_SIZE_IN_WORDS - offset;
    wsSize = 15*(pPkaReqInfo->bLen + 2 - (pPkaReqInfo->bLen & 0x01))
    
    if (wsAvail < wsSize)
    {
        return(sa_ERR_PKA_WORKSPACE_ERROR);
    }
    */
          
    pPkcpRegs->PKA_BLENGTH = pPkaReqInfo->bLen;
  
    /* Initialize the Operation */
    pPkcpRegs->PKA_FUNCTION  = SA_PKA_COMPLEX_OP_ECp_SCALE | CSL_EIP_29T2_RAM_PKA_FUNCTION_RUN_MASK;
    
    return (sa_ERR_OK);
} 


/****************************************************************************
 * FUNCTION PURPOSE: PKA ECp Multiply plus Scale Operation setup
 ****************************************************************************
 * DESCRIPTION: Setup and initialize PKA ECpMul plus ECpScale operations 
 *              perfroming by the PKA module.
 *              - Perform operation-specific input parameter check
 *              - Setup PKCP registers and PKA RAM per operation
 *              - Trigger the operation 
 *
 *  int16_t salld_pka_op_ecp_mul_scale_setup (
 *            salldObj_t*       inst,         -> Pointer to SALLD instance
 *            Sa_PkaReqInfo2_t* pPkaReqInfo   -> Pointer to the PKA operation 
 *                                               request information structure
 *                                 )
 * Return values:   sa_ERR_OK
 *                  sa_ERR_PARAMS
 *                  sa_ERR_PKA_WORKSPACE_ERROR
 *
 ***************************************************************************/
static int16_t salld_pka_op_ecp_mul_scale_setup(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo)
{
    salldPkaInst_t*  pPkaInst = &inst->pkaInst;   
    int16_t ret_code;
    
    ret_code = salld_pka_op_ecp_mul_setup(inst, pPkaReqInfo);
    if (ret_code != sa_ERR_OK)
    {
        return(ret_code);
    }
    
    pPkaInst->nextState = SA_PKA_OP_STATE_STEP1;
    
    return(sa_ERR_OK);
}

/****************************************************************************
 * FUNCTION PURPOSE: PKA ECp DSA Verify Operation setup
 ****************************************************************************
 * DESCRIPTION: Setup and initialize PKA ECpDSA Verify operations perfroming by
 *              the PKA module.
 *              - Perform operation-specific input parameter check
 *              - Setup PKCP registers and PKA RAM per operation
 *              - Trigger the operation 
 *
 *  int16_t salld_pka_op_ecp_dsa_verify_setup (
 *            salldObj_t*       inst,         -> Pointer to SALLD instance
 *            Sa_PkaReqInfo2_t* pPkaReqInfo   -> Pointer to the PKA operation 
 *                                               request information structure
 *                                 )
 * Return values:   sa_ERR_OK
 *                  sa_ERR_PARAMS
 *                  sa_ERR_PKA_WORKSPACE_ERROR
 *
 ***************************************************************************/
static int16_t salld_pka_op_ecp_dsa_verify_setup(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo)
{
    uintptr_t baseAddr = (uintptr_t)salldLObj.baseAddr;
    CSL_Cp_aceRegs*  pSaRegs = (CSL_Cp_aceRegs*)baseAddr;
    CSL_Eip_29t2_ramEip_27b_eip27_registersRegs* pPkcpRegs = &pSaRegs->PKA.EIP_27B_EIP27_REGISTERS;  
    Sa_PkaECDSAVerifyParams_t* pPkaOp = &pPkaReqInfo->params.ecDSAVerify;
    uint32_t offset = 0;

    if ((pPkaOp->pRes == NULL) || (pPkaOp->pModP == NULL) || (pPkaOp->pEcA == NULL) || (pPkaOp->pEcBC == NULL) ||
        (pPkaOp->pN == NULL) || (pPkaOp->pH == NULL) || (pPkaOp->pS == NULL) ||
        (pPkaOp->pR == NULL) || (pPkaOp->point1.pX == NULL) || (pPkaOp->point1.pY == NULL) || (pPkaOp->point1.pZ == NULL) ||
        (pPkaOp->pY.pX == NULL) || (pPkaOp->pY.pY == NULL) || (pPkaOp->pY.pZ == NULL))
    {
        return (sa_ERR_PARAMS);
    }

    /*
     * Perform the following parameter checks
     * - 0 < ALen <= 24 (maximum vector length is 768 bits)
     * - 1 < BLen <= 24 (maximum vector length is 768 bits)
     * - The highest word of the modulus vector, as indicated by BLen, may not be zero.
     * - a < p and b < p
     * - P1z = 1
     * - 1 < k <= n, where n is the curve's order. (not check)
     * - P1 must be on the curve (not check) 
     */

    if ((pPkaReqInfo->aLen > sa_MAX_PKA_OPERAND_SIZE_ECp) || (pPkaReqInfo->aLen < 1))
    {
        return (sa_ERR_PARAMS);
    }

    if ((pPkaReqInfo->bLen > sa_MAX_PKA_OPERAND_SIZE_ECp) || (pPkaReqInfo->bLen <= 1))
    {
        return (sa_ERR_PARAMS);
    }

    if ((pPkaReqInfo->bLen == 2) && ((pPkaOp->pModP[1] & 0x80000000UL) == 0))
    {
        return (sa_ERR_PARAMS);
    }

    if (sap_pka_compare(pPkaOp->pEcA, pPkaOp->pModP, pPkaReqInfo->bLen) != sa_PKA_COMP_AINFB)
    {
        return (sa_ERR_PARAMS);
    }

    if (sap_pka_compare(pPkaOp->pEcBC, pPkaOp->pModP, pPkaReqInfo->bLen) != sa_PKA_COMP_AINFB)
    {
        return (sa_ERR_PARAMS);
    }

    /* Calculate offsets and copy (p, a, b), K, N, h, d, P1xyz to PKA RAM */
    baseAddr += SA_PKA_A_BASE;
    pPkcpRegs->PKA_BPTR = offset; /* p, a, b */  
    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),
           pPkaOp->pModP, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen + 2);
    offset += (offset & 0x1);

    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),
           pPkaOp->pEcA, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen + 2);
    offset += (offset & 0x1);

    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),
           pPkaOp->pEcBC, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen + 2);
    offset += (offset & 0x1);

    /* N, H, D which are used at the subsequent operations */
    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),
           pPkaOp->pN, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen + 2);
    offset += (offset & 0x1);

    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),
           pPkaOp->point1.pX, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen + 2);
    offset += (offset & 0x1);

    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),
           pPkaOp->point1.pY, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen + 2);
    offset += (offset & 0x1);

    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),
           pPkaOp->point1.pZ, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen + 2);
    offset += (offset & 0x1);

    pPkcpRegs->PKA_CPTR = offset; /* H */

    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),
           pPkaOp->pH, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen + 2);
    offset += (offset & 0x1);

    pPkcpRegs->PKA_APTR = offset; /* Yx Yy Yz (public key) */

    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),
           pPkaOp->pY.pX, pPkaReqInfo->aLen);
    offset += (pPkaReqInfo->aLen + 2);
    offset += (offset & 0x1);

    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),
           pPkaOp->pY.pY, pPkaReqInfo->aLen);
    offset += (pPkaReqInfo->aLen + 2);
    offset += (offset & 0x1);

    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),
           pPkaOp->pY.pZ, pPkaReqInfo->aLen);
    offset += (pPkaReqInfo->aLen + 2);
    offset += (offset & 0x1);

    pPkcpRegs->PKA_DPTR = offset;  /* R and S */

    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),
           pPkaOp->pR, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen + 2);
    offset += (offset & 0x1);

    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),
           pPkaOp->pS, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen + 2);
    offset += (offset & 0x1);

    pPkcpRegs->PKA_ALENGTH = pPkaReqInfo->aLen;
    pPkcpRegs->PKA_BLENGTH = pPkaReqInfo->bLen;

    /* Initialize the Operation */
    pPkcpRegs->PKA_FUNCTION  = SA_PKA_COMPLEX_OP_ECpDSA_VERIFY | CSL_EIP_29T2_RAM_PKA_FUNCTION_RUN_MASK;

    return (sa_ERR_OK);
}

/****************************************************************************
 * FUNCTION PURPOSE: PKA ECp DSA Sign Operation setup
 ****************************************************************************
 * DESCRIPTION: Setup and initialize PKA ECpDSA Sign operations perfroming by  
 *              the PKA module.
 *              - Perform operation-specific input parameter check
 *              - Setup PKCP registers and PKA RAM per operation
 *              - Trigger the operation 
 *
 *  int16_t salld_pka_op_ecp_dsa_sign_setup (
 *            salldObj_t*       inst,         -> Pointer to SALLD instance
 *            Sa_PkaReqInfo2_t* pPkaReqInfo   -> Pointer to the PKA operation 
 *                                               request information structure
 *                                 )
 * Return values:   sa_ERR_OK
 *                  sa_ERR_PARAMS
 *                  sa_ERR_PKA_WORKSPACE_ERROR
 *
 ***************************************************************************/
static int16_t salld_pka_op_ecp_dsa_sign_setup(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo)
{
    uintptr_t baseAddr = (uintptr_t)salldLObj.baseAddr;
    CSL_Cp_aceRegs*  pSaRegs = (CSL_Cp_aceRegs*)baseAddr;
    CSL_Eip_29t2_ramEip_27b_eip27_registersRegs* pPkcpRegs = &pSaRegs->PKA.EIP_27B_EIP27_REGISTERS;  
    Sa_PkaECDSASignParams_t* pPkaOp = &pPkaReqInfo->params.ecDSASign;
    salldPkaInst_t*  pPkaInst = &inst->pkaInst;  
    pkaEcdsaSignInst_t* pEcdsaInst = &pPkaInst->inst.ecdsaSign; 
    uint32_t offset = 0;
    
    if ((pPkaOp->pK == NULL) || (pPkaOp->pModP == NULL) || (pPkaOp->pEcA == NULL) || (pPkaOp->pEcBC == NULL) || 
        (pPkaOp->pN == NULL) || (pPkaOp->pH == NULL) || (pPkaOp->pD == NULL) || (pPkaOp->pS == NULL) || 
        (pPkaOp->pR == NULL) || (pPkaOp->point1.pX == NULL) || (pPkaOp->point1.pY == NULL) || (pPkaOp->point1.pZ == NULL)) 
    {
        return (sa_ERR_PARAMS);
    }
      
    /* 
     * Perform the following parameter checks
     * - 0 < ALen <= 24 (maximum vector length is 768 bits)
     * - 1 < BLen <= 24 (maximum vector length is 768 bits)
     * - Modulus P must be a prime > 2**63
     * - The highest word of the modulus vector, as indicated by BLen, may not be zero.
     * - a < p and b < p
     * - P1z = 1
     * - 1 < k <= n, where n is the curve's order. (not check)
     * - P1 must be on the curve (not check) 
     */
    
    if ((pPkaReqInfo->aLen > sa_MAX_PKA_OPERAND_SIZE_ECp) || (pPkaReqInfo->aLen < 1))
    {
        return (sa_ERR_PARAMS);
    } 
    
    if ((pPkaReqInfo->bLen > sa_MAX_PKA_OPERAND_SIZE_ECp) || (pPkaReqInfo->bLen <= 1))
    {
        return (sa_ERR_PARAMS);
    } 
    
    if ((!(pPkaOp->pModP[0] & 0x1)) || (!pPkaOp->pModP[pPkaReqInfo->bLen - 1]))
    {
        return (sa_ERR_PARAMS);
    }
    
    if ((pPkaReqInfo->bLen == 2) && ((pPkaOp->pModP[1] & 0x80000000UL) == 0))
    {
        return (sa_ERR_PARAMS);
    }
    
    if (sap_pka_compare(pPkaOp->pEcA, pPkaOp->pModP, pPkaReqInfo->bLen) != sa_PKA_COMP_AINFB)
    {
        return (sa_ERR_PARAMS);
    }
    
    if (sap_pka_compare(pPkaOp->pEcBC, pPkaOp->pModP, pPkaReqInfo->bLen) != sa_PKA_COMP_AINFB)
    {
        return (sa_ERR_PARAMS);
    }
    
    if ((pPkaOp->point1.pZ[0] != 1) || (!sap_pka_is_zero(&pPkaOp->point1.pZ[1], pPkaReqInfo->bLen-1)))
    {
        return (sa_ERR_PARAMS);
    }

    /* Calculate offsets and copy (p, a, b), K, N, h, d, P1xyz to PKA RAM */
    baseAddr += SA_PKA_A_BASE;
    pPkcpRegs->PKA_BPTR = offset; /* p, a, b */  
    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),
           pPkaOp->pModP, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen + 2);
    offset += (offset & 0x1);

    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),
           pPkaOp->pEcA, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen + 2);
    offset += (offset & 0x1);

    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),
           pPkaOp->pEcBC, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen + 2);
    offset += (offset & 0x1);

    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),
           pPkaOp->pN, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen + 2);
    offset += (offset & 0x1);

    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),
           pPkaOp->point1.pX, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen + 2);
    offset += (offset & 0x1);

    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),
           pPkaOp->point1.pY, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen + 2);
    offset += (offset & 0x1);

    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),
           pPkaOp->point1.pZ, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen + 2);
    offset += (offset & 0x1);

    pPkcpRegs->PKA_CPTR = offset; /* H */
    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),
           pPkaOp->pH, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen + 2);
    offset += (offset & 0x1);

    pPkcpRegs->PKA_APTR = offset; /* D */
    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),
           pPkaOp->pD, pPkaReqInfo->aLen);
    offset += (pPkaReqInfo->aLen + 2);
    offset += (offset & 0x1);

    pPkcpRegs->PKA_DPTR = offset; /* K */
    pEcdsaInst->offsetR = offset;
    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),
           pPkaOp->pK, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen + 2);
    offset += (offset & 0x1);

    pEcdsaInst->offsetS    = offset;
    pPkcpRegs->PKA_ALENGTH = pPkaReqInfo->aLen;
    pPkcpRegs->PKA_BLENGTH = pPkaReqInfo->bLen;

    /* Initialize the Operation */
    pPkcpRegs->PKA_FUNCTION  = SA_PKA_COMPLEX_OP_ECpDSA_SIGN | CSL_EIP_29T2_RAM_PKA_FUNCTION_RUN_MASK;

    return (sa_ERR_OK);

}

/****************************************************************************
 * FUNCTION PURPOSE: PKA EC2m Add Operation setup
 ****************************************************************************
 * DESCRIPTION: Setup and initialize PKA EC2mAdd operations perfroming by the 
 *              PKA module.
 *              - Perform operation-specific input parameter check
 *              - Setup PKCP registers and PKA RAM per operation
 *              - Trigger the operation 
 *
 *  int16_t salld_pka_op_ecp_add_setup (
 *            salldObj_t*       inst,         -> Pointer to SALLD instance
 *            Sa_PkaReqInfo2_t* pPkaReqInfo   -> Pointer to the PKA operation 
 *                                               request information structure
 *                                 )
 * Return values:   sa_ERR_OK
 *                  sa_ERR_PARAMS
 *                  sa_ERR_PKA_WORKSPACE_ERROR
 *
 ***************************************************************************/
static int16_t salld_pka_op_ec2m_add_setup(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo)
{
    uintptr_t baseAddr = (uintptr_t)salldLObj.baseAddr;
    CSL_Cp_aceRegs*  pSaRegs = (CSL_Cp_aceRegs*)baseAddr;
    CSL_Eip_29t2_ramEip_27b_eip27_registersRegs* pPkcpRegs = &pSaRegs->PKA.EIP_27B_EIP27_REGISTERS;  
    Sa_PkaECAddParams_t* pPkaOp = &pPkaReqInfo->params.ecAdd;
    uint32_t offset = 0;
    
    if ((pPkaOp->pModP == NULL) || (pPkaOp->pEcA == NULL) || (pPkaOp->pEcB == NULL) || 
        (pPkaOp->point1.pX == NULL) || (pPkaOp->point1.pY == NULL) || (pPkaOp->point1.pZ == NULL) || 
        (pPkaOp->point2.pX == NULL) || (pPkaOp->point2.pY == NULL) || (pPkaOp->point2.pZ == NULL) || 
        (pPkaOp->point0.pX == NULL) || (pPkaOp->point0.pY == NULL) || (pPkaOp->point0.pZ == NULL))  
    {
        return(sa_ERR_PARAMS);
    }
      
    /* 
     * Perform the following parameter checks
     * -  1 < BLen <= 18 (maximum vector length is 571 bits)
     * - Modulus P must be a prime
     * - The highest word of the modulus vector, as indicated by BLen, may not be zero.
     * - a < p and b < p
     * - P1 and P2 must be on the curve (not check) 
     */
    
    if ((pPkaReqInfo->bLen > sa_MAX_PKA_OPERAND_SIZE_EC2m) || (pPkaReqInfo->bLen <= 1))
    {
        return (sa_ERR_PARAMS);
    } 
    
    if ((!(pPkaOp->pModP[0] & 0x1)) || (!pPkaOp->pModP[pPkaReqInfo->bLen - 1]))
    {
        return (sa_ERR_PARAMS);
    }
    
    if (sap_pka_compare(pPkaOp->pEcA, pPkaOp->pModP, pPkaReqInfo->bLen) != sa_PKA_COMP_AINFB)
    {
        return (sa_ERR_PARAMS);
    }
    
    if (sap_pka_compare(pPkaOp->pEcB, pPkaOp->pModP, pPkaReqInfo->bLen) != sa_PKA_COMP_AINFB)
    {
        return (sa_ERR_PARAMS);
    }
    
    /* Calculate offsets and copy (p, a, b), P1xyz, P2xyz to PKA RAM */
    baseAddr += SA_PKA_A_BASE;
    pPkcpRegs->PKA_BPTR = offset; /* p, a, b */  
    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),  
           pPkaOp->pModP, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen);
    offset += (offset & 0x1);
    
    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),  
           pPkaOp->pEcA, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen);
    offset += (offset & 0x1);
    
    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),  
           pPkaOp->pEcB, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen);
    offset += (offset & 0x1);
    
    pPkcpRegs->PKA_APTR = offset; /* P1xyz */
    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),  
           pPkaOp->point1.pX, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen);
    offset += (offset & 0x1);
    
    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),  
           pPkaOp->point1.pY, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen);
    offset += (offset & 0x1);
    
    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),  
           pPkaOp->point1.pZ, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen);
    offset += (offset & 0x1);
    
    pPkcpRegs->PKA_CPTR = offset; /* P2xyz */ 
    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),  
           pPkaOp->point2.pX, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen);
    offset += (offset & 0x1);
    
    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),  
           pPkaOp->point2.pY, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen);
    offset += (offset & 0x1);
    
    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),  
           pPkaOp->point2.pZ, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen);
    offset += (offset & 0x1);
    
    pPkcpRegs->PKA_DPTR = offset;  /* P0xyx */
    
    /* Calculate and check whether there is enough workspace 
     * Wokspace: 15*(BLen + 2 + BLen MOD 2)
     * Maximum worksapce: 15*(18 + 2 + 0) = 300 words 
     *
    offset += (pPkaReqInfo->bLen + (pPkaReqInfo->bLen & 0x01))*3;
    
    wsAvail = SA_PKA_RAM_SIZE_IN_WORDS - SA_PKA_RAM_SCRATCHPAD_SIZE_IN_WORDS - offset;
    wsSize = 15*(pPkaReqInfo->bLen + 2 - (pPkaReqInfo->bLen & 0x01))
    
    if (wsAvail < wsSize)
    {
        return(sa_ERR_PKA_WORKSPACE_ERROR);
    }
    */
          
    pPkcpRegs->PKA_ALENGTH = pPkcpRegs->PKA_BLENGTH = pPkaReqInfo->bLen;
  
    /* Initialize the Operation */
    pPkcpRegs->PKA_FUNCTION  = SA_PKA_COMPLEX_OP_EC2m_ADD | CSL_EIP_29T2_RAM_PKA_FUNCTION_RUN_MASK;
    
    return (sa_ERR_OK);
} 

/****************************************************************************
 * FUNCTION PURPOSE: PKA EC2m Multiply Operation setup
 ****************************************************************************
 * DESCRIPTION: Setup and initialize PKA EC2mMul operations perfroming by the 
 *              PKA module.
 *              - Perform operation-specific input parameter check
 *              - Setup PKCP registers and PKA RAM per operation
 *              - Trigger the operation 
 *
 *  int16_t salld_pka_op_ecp_mul_setup (
 *            salldObj_t*       inst,         -> Pointer to SALLD instance
 *            Sa_PkaReqInfo2_t* pPkaReqInfo   -> Pointer to the PKA operation 
 *                                               request information structure
 *                                 )
 * Return values:   sa_ERR_OK
 *                  sa_ERR_PARAMS
 *                  sa_ERR_PKA_WORKSPACE_ERROR
 *
 ***************************************************************************/
static int16_t salld_pka_op_ec2m_mul_setup(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo)
{
    uintptr_t baseAddr = (uintptr_t)salldLObj.baseAddr;
    CSL_Cp_aceRegs*  pSaRegs = (CSL_Cp_aceRegs*)baseAddr;
    CSL_Eip_29t2_ramEip_27b_eip27_registersRegs* pPkcpRegs = &pSaRegs->PKA.EIP_27B_EIP27_REGISTERS;  
    Sa_PkaECMulParams_t* pPkaOp = &pPkaReqInfo->params.ecMul;
    uint32_t offset = 0;
    
    if ((pPkaOp->pK == NULL) || (pPkaOp->pModP == NULL) || (pPkaOp->pEcA == NULL) || (pPkaOp->pEcBC == NULL) || 
        (pPkaOp->point1.pX == NULL) || (pPkaOp->point1.pY == NULL) || (pPkaOp->point1.pZ == NULL) || 
        (pPkaOp->point0.pX == NULL) || (pPkaOp->point0.pY == NULL) || (pPkaOp->point0.pZ == NULL))  
    {
        return (sa_ERR_PARAMS);
    }
      
    /* 
     * Perform the following parameter checks
     * - 0 < ALen <= 18 (maximum vector length is 571 bits)
     * - 1 < BLen <= 18 (maximum vector length is 571 bits)
     * - Modulus P must be a prime
     * - The highest word of the modulus vector, as indicated by BLen, may not be zero.
     * - a < p and b < p
     * - P1z = 1
     * - 1 < k <= n, where n is the curve's order. (not check)
     * - P1 must be on the curve (not check) 
     */
    
    if ((pPkaReqInfo->aLen > sa_MAX_PKA_OPERAND_SIZE_EC2m) || (pPkaReqInfo->aLen < 1))
    {
        return (sa_ERR_PARAMS);
    } 
    
    if ((pPkaReqInfo->bLen > sa_MAX_PKA_OPERAND_SIZE_EC2m) || (pPkaReqInfo->bLen <= 1))
    {
        return (sa_ERR_PARAMS);
    } 
    
    if ((!(pPkaOp->pModP[0] & 0x1)) || (!pPkaOp->pModP[pPkaReqInfo->bLen - 1]))
    {
        return (sa_ERR_PARAMS);
    }
    
    if (sap_pka_compare(pPkaOp->pEcA, pPkaOp->pModP, pPkaReqInfo->bLen) != sa_PKA_COMP_AINFB)
    {
        return (sa_ERR_PARAMS);
    }
    
    if (sap_pka_compare(pPkaOp->pEcBC, pPkaOp->pModP, pPkaReqInfo->bLen) != sa_PKA_COMP_AINFB)
    {
        return (sa_ERR_PARAMS);
    }
    
    if ((pPkaOp->point1.pZ[0] != 1) || (!sap_pka_is_zero(&pPkaOp->point1.pZ[1], pPkaReqInfo->bLen-1)))
    {
        return (sa_ERR_PARAMS);
    }
    
    /* Calculate offsets and copy (p, a, b), K, P1xyz to PKA RAM */
    baseAddr += SA_PKA_A_BASE;
    pPkcpRegs->PKA_BPTR = offset; /* p, a, b */  
    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),  
           pPkaOp->pModP, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen);
    offset += (offset & 0x1);
    
    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),  
           pPkaOp->pEcA, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen);
    offset += (offset & 0x1);
    
    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),  
           pPkaOp->pEcBC, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen);
    offset += (offset & 0x1);
    
    pPkcpRegs->PKA_APTR = offset; /* K */
    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),  
           pPkaOp->pK, pPkaReqInfo->aLen);
    offset += (pPkaReqInfo->aLen);
    offset += (offset & 0x1);
    
    pPkcpRegs->PKA_CPTR = offset; /* P1xyz */ 
    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),  
           pPkaOp->point1.pX, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen);
    offset += (offset & 0x1);
    
    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),  
           pPkaOp->point1.pY, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen);
    offset += (offset & 0x1);
    
    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),  
           pPkaOp->point1.pZ, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen);
    offset += (offset & 0x1);
    
    pPkcpRegs->PKA_DPTR = offset;  /* P0xyx */
    
    /* Calculate and check whether there is enough workspace 
     * Wokspace: 15*(BLen + 2 + BLen MOD 2)
     * Maximum worksapce: 15*(18 + 2 + 0) = 300 words 
     *
    offset += (pPkaReqInfo->bLen + (pPkaReqInfo->bLen & 0x01))*3;
    
    wsAvail = SA_PKA_RAM_SIZE_IN_WORDS - SA_PKA_RAM_SCRATCHPAD_SIZE_IN_WORDS - offset;
    wsSize = 15*(pPkaReqInfo->bLen + 2 - (pPkaReqInfo->bLen & 0x01))
    
    if (wsAvail < wsSize)
    {
        return(sa_ERR_PKA_WORKSPACE_ERROR);
    }
    */
          
    pPkcpRegs->PKA_ALENGTH = pPkaReqInfo->aLen;
    pPkcpRegs->PKA_BLENGTH = pPkaReqInfo->bLen;
  
    /* Initialize the Operation */
    pPkcpRegs->PKA_FUNCTION  = SA_PKA_COMPLEX_OP_EC2m_MUL | CSL_EIP_29T2_RAM_PKA_FUNCTION_RUN_MASK;
    
    return (sa_ERR_OK);
} 

/****************************************************************************
 * FUNCTION PURPOSE: PKA EC2m Scale Operation setup
 ****************************************************************************
 * DESCRIPTION: Setup and initialize PKA EC2mScale operations perfroming by the 
 *              PKA module.
 *              - Perform operation-specific input parameter check
 *              - Setup PKCP registers and PKA RAM per operation
 *              - Trigger the operation 
 *
 *  int16_t salld_pka_op_ecp_mul_setup (
 *            salldObj_t*       inst,         -> Pointer to SALLD instance
 *            Sa_PkaReqInfo2_t* pPkaReqInfo   -> Pointer to the PKA operation 
 *                                               request information structure
 *                                 )
 * Return values:   sa_ERR_OK
 *                  sa_ERR_PARAMS
 *                  sa_ERR_PKA_WORKSPACE_ERROR
 *
 ***************************************************************************/
static int16_t salld_pka_op_ec2m_scale_setup(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo)
{
    uintptr_t baseAddr = (uintptr_t)salldLObj.baseAddr;
    CSL_Cp_aceRegs*  pSaRegs = (CSL_Cp_aceRegs*)baseAddr;
    CSL_Eip_29t2_ramEip_27b_eip27_registersRegs* pPkcpRegs = &pSaRegs->PKA.EIP_27B_EIP27_REGISTERS;  
    Sa_PkaECScaleParams_t* pPkaOp = &pPkaReqInfo->params.ecScale;
    uint32_t offset = 0;
    
    if ((pPkaOp->pModP == NULL) || (pPkaOp->pEcA == NULL) || (pPkaOp->pEcB == NULL) || 
        (pPkaOp->point1.pX == NULL) || (pPkaOp->point1.pY == NULL) || (pPkaOp->point1.pZ == NULL) || 
        (pPkaOp->point0.pX == NULL) || (pPkaOp->point0.pY == NULL) || (pPkaOp->point0.pZ == NULL))  
    {
        return (sa_ERR_PARAMS);
    }
      
    /* 
     * Perform the following parameter checks
     * - 1 < BLen <= 18 (maximum vector length is 571 bits
     * - Modulus P must be a prime 
     * - The highest word of the modulus vector, as indicated by BLen, may not be zero.
     * - a < p and b < p (?)
     */
    if ((pPkaReqInfo->bLen > sa_MAX_PKA_OPERAND_SIZE_EC2m) || (pPkaReqInfo->bLen <= 1))
    {
        return (sa_ERR_PARAMS);
    } 
    
    if ((!(pPkaOp->pModP[0] & 0x1)) || (!pPkaOp->pModP[pPkaReqInfo->bLen - 1]))
    {
        return (sa_ERR_PARAMS);
    }
    
    if (sap_pka_compare(pPkaOp->pEcA, pPkaOp->pModP, pPkaReqInfo->bLen) != sa_PKA_COMP_AINFB)
    {
        return (sa_ERR_PARAMS);
    }
    
    if (sap_pka_compare(pPkaOp->pEcB, pPkaOp->pModP, pPkaReqInfo->bLen) != sa_PKA_COMP_AINFB)
    {
        return (sa_ERR_PARAMS);
    }
    
    /* Calculate offsets and copy (p, a, b), P1xyz to PKA RAM */
    baseAddr += SA_PKA_A_BASE;
    pPkcpRegs->PKA_BPTR = offset; /* p, a, b */  
    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),  
           pPkaOp->pModP, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen);
    offset += (offset & 0x1);
    
    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),  
           pPkaOp->pEcA, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen);
    offset += (offset & 0x1);
    
    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),  
           pPkaOp->pEcB, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen);
    offset += (offset & 0x1);
    
    pPkcpRegs->PKA_DPTR = offset; /* P1xyz and P0xyx*/ 
    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),  
           pPkaOp->point1.pX, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen);
    offset += (offset & 0x1);
    
    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),  
           pPkaOp->point1.pY, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen);
    offset += (offset & 0x1);
    
    sap_pka_write((volatile uint32_t*)(baseAddr + (offset << 2)),  
           pPkaOp->point1.pZ, pPkaReqInfo->bLen);
    offset += (pPkaReqInfo->bLen);
    offset += (offset & 0x1);
    
    /* Calculate and check whether there is enough workspace 
     * Wokspace: 15*(BLen + 2 + BLen MOD 2)
     * Maximum worksapce: 15*(18 + 2 + 0) = 300 words 
     *
    offset += (pPkaReqInfo->bLen + (pPkaReqInfo->bLen & 0x01))*3;
    
    wsAvail = SA_PKA_RAM_SIZE_IN_WORDS - SA_PKA_RAM_SCRATCHPAD_SIZE_IN_WORDS - offset;
    wsSize = 15*(pPkaReqInfo->bLen + 2 - (pPkaReqInfo->bLen & 0x01))
    
    if (wsAvail < wsSize)
    {
        return(sa_ERR_PKA_WORKSPACE_ERROR);
    }
    */
          
    pPkcpRegs->PKA_BLENGTH = pPkaReqInfo->bLen;
  
    /* Initialize the Operation */
    pPkcpRegs->PKA_FUNCTION  = SA_PKA_COMPLEX_OP_EC2m_SCALE | CSL_EIP_29T2_RAM_PKA_FUNCTION_RUN_MASK;
    
    return (sa_ERR_OK);
} 


/****************************************************************************
 * FUNCTION PURPOSE: PKA EC2m Multiply plus Scale Operation setup
 ****************************************************************************
 * DESCRIPTION: Setup and initialize PKA EC2mMul plus EC2mScale operations 
 *              perfroming by the PKA module.
 *              - Perform operation-specific input parameter check
 *              - Setup PKCP registers and PKA RAM per operation
 *              - Trigger the operation 
 *
 *  int16_t salld_pka_op_ecp_mul_scale_setup (
 *            salldObj_t*       inst,         -> Pointer to SALLD instance
 *            Sa_PkaReqInfo2_t* pPkaReqInfo   -> Pointer to the PKA operation 
 *                                               request information structure
 *                                 )
 * Return values:   sa_ERR_OK
 *                  sa_ERR_PARAMS
 *                  sa_ERR_PKA_WORKSPACE_ERROR
 *
 ***************************************************************************/
static int16_t salld_pka_op_ec2m_mul_scale_setup(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo)
{
    salldPkaInst_t*  pPkaInst = &inst->pkaInst;   
    int16_t ret_code;
    
    ret_code = salld_pka_op_ec2m_mul_setup(inst, pPkaReqInfo);
    if (ret_code != sa_ERR_OK)
    {
        return(ret_code);
    }
    
    pPkaInst->nextState = SA_PKA_OP_STATE_STEP1;
    
    return(sa_ERR_OK);
}


/****************************************************************************
 * FUNCTION PURPOSE: PKA Basic Operation Process
 ****************************************************************************
 * DESCRIPTION: Process PKA basic operation results. 
 *              - Copy the PKA operation results from PKCP registers
 *                and PKA RAM to output buffers 
 *
 *  int16_t salld_pka_op_pkcp_proc (
 *            salldObj_t*       inst,         -> Pointer to SALLD instance
 *            Sa_PkaReqInfo2_t* pPkaReqInfo   -> Pointer to the PKA operation 
 *                                               request information structure
 *            void*             resultPtr     -> Point to the operation results
 *                                 )
 * Return values:   sa_ERR_OK
 *                  sa_PKA_OP_COMPLETE
 *
 ***************************************************************************/
static int16_t salld_pka_op_pkcp_proc(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo, void* resultPtr)
{
    uintptr_t  baseAddr = (uintptr_t) salldLObj.baseAddr;
    CSL_Cp_aceRegs*  pSaRegs = (CSL_Cp_aceRegs*)baseAddr;
    CSL_Eip_29t2_ramEip_27b_eip27_registersRegs* pPkcpRegs = &pSaRegs->PKA.EIP_27B_EIP27_REGISTERS;  
    Sa_PkaBasicOpParams_t* pPkaOp = &pPkaReqInfo->params.basicOp;
    int i;
    
    /* Basic Operation: Just wait for results since it will take only a few cycles */
    /* Wait for the operation to be complete or timeout occurs */
    for (i = 0; i < SA_PKA_MAX_PKCP_OP_LOOP_COUNT; i++)
    {
        if (!(pPkcpRegs->PKA_FUNCTION & CSL_EIP_29T2_RAM_PKA_FUNCTION_RUN_MASK))
            break;
    }
  
    /* figure out how to calculate the size of operation result */
    if (i >= SA_PKA_MAX_PKCP_OP_LOOP_COUNT)
    {
        return(sa_ERR_PKA_TIMEOUT);
    }
    
    /* Process the operation result */
    switch (pPkaReqInfo->operation)
    {
        case sa_PKA_OP_ADD:
        case sa_PKA_OP_SUB:
        case sa_PKA_OP_MUL:
        case sa_PKA_OP_RSHIFT:
        case sa_PKA_OP_LSHIFT:
        case sa_PKA_OP_COPY:
        case sa_PKA_OP_MOD:
            pPkaOp->resultSize = (pPkcpRegs->PKA_MSW & CSL_EIP_29T2_RAM_PKA_MSW_RESULT_IS_ZERO_MASK)?0:
                                  (uint16_t)(pPkcpRegs->PKA_MSW - SA_PKA_C_OFFSET + 1);  
            sap_pka_read(pPkaOp->pResult, 
                  (volatile uint32_t*)(baseAddr  + SA_PKA_C_BASE), 
                   pPkaOp->resultSize);
            break;
            
        case sa_PKA_OP_ADD_SUB:
            pPkaOp->resultSize = (pPkcpRegs->PKA_MSW & CSL_EIP_29T2_RAM_PKA_MSW_RESULT_IS_ZERO_MASK)?0:
                                  (uint16_t)(pPkcpRegs->PKA_MSW - SA_PKA_D_OFFSET + 1);  
            sap_pka_read(pPkaOp->pResult, 
                  (volatile uint32_t*)(baseAddr  + SA_PKA_D_BASE), 
                   pPkaOp->resultSize);
            break;
    
        case sa_PKA_OP_DIV:
            pPkaOp->resultSize = (pPkcpRegs->PKA_MSW & CSL_EIP_29T2_RAM_PKA_MSW_RESULT_IS_ZERO_MASK)?0:
                                  (uint16_t)(pPkcpRegs->PKA_MSW - SA_PKA_D_OFFSET + 1);  
            pPkaOp->remSize = (pPkcpRegs->PKA_DIVMSW & CSL_EIP_29T2_RAM_PKA_DIVMSW_RESULT_IS_ZERO_MASK)?0:
                               (uint16_t)(pPkcpRegs->PKA_DIVMSW - SA_PKA_C_OFFSET + 1);  
            sap_pka_read(pPkaOp->pRem, 
                  (volatile uint32_t*)(baseAddr  + SA_PKA_C_BASE), 
                   pPkaOp->remSize);
            sap_pka_read(pPkaOp->pResult, 
                  (volatile uint32_t*)(baseAddr  + SA_PKA_D_BASE), 
                   pPkaOp->resultSize);
            break;
    
        case sa_PKA_OP_COMP:
            pPkaOp->cmpResult =  (Sa_PkaCompResults_t)pPkcpRegs->PKA_COMPARE;
            break;
    
        default:
            break;    
    }
    
    return(sa_PKA_OP_COMPLETE);
}

/****************************************************************************
 * FUNCTION PURPOSE: PKA Mod Common Operation Process
 ****************************************************************************
 * DESCRIPTION: Process PKA ModExp and ModINv operation results. 
 *              - Copy the PKA operation results from PKCP/PKA registers
 *                and PKA RAM to output buffers 
 *
 *  int16_t salld_pka_op_modexp_proc (
 *            salldObj_t*       inst,         -> Pointer to SALLD instance
 *            Sa_PkaReqInfo2_t* pPkaReqInfo   -> Pointer to the PKA operation 
 *                                               request information structure
 *            void*             resultPtr     -> Point to the operation results
 *                                 )
 * Return values:   sa_ERR_OK
 *                  sa_PKA_OP_COMPLETE
 *                  sa_ERR_PKA_OP_ERROR
 *
 ***************************************************************************/
static int16_t salld_pka_op_mod_common_proc(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo, void* resultPtr)
{
    uintptr_t  baseAddr = (uintptr_t) salldLObj.baseAddr;
    CSL_Cp_aceRegs*  pSaRegs = (CSL_Cp_aceRegs*)baseAddr;
    CSL_Eip_29t2_ramEip_27b_eip27_registersRegs* pPkcpRegs = &pSaRegs->PKA.EIP_27B_EIP27_REGISTERS;  
    CSL_Eip_29t2_ramEip_28px12_gf2_2pram_eip28_registersRegs* pPkaCtrlRegs = &pSaRegs->PKA.EIP_28PX12_GF2_2PRAM_EIP28_REGISTERS;
    uint32_t *pResult = (uint32_t *)resultPtr;
    
    /* Extract the status code */
    pPkaReqInfo->statusCode = (pPkaCtrlRegs->PKA_SEQ_CTRL & CSL_EIP_29T2_RAM_PKA_SEQ_CTRL_SEQ_STATUS_MASK)
                              >> CSL_EIP_29T2_RAM_PKA_SEQ_CTRL_SEQ_STATUS_SHIFT;
                              
    if (pPkaReqInfo->statusCode == sa_PKA_OP_STATUS_SUCCESS)
    {
        /* Copy the operation result */
         baseAddr += SA_PKA_A_BASE;
         sap_pka_read(pResult, 
               (volatile uint32_t*)(baseAddr  + (pPkcpRegs->PKA_DPTR << 2)), 
                pPkaReqInfo->bLen);
                
         return (sa_PKA_OP_COMPLETE);       
    }                  
    else
    {
         return (sa_ERR_PKA_OP_ERROR);       
    }        
}

/****************************************************************************
 * FUNCTION PURPOSE: PKA ModExp CRT Operation Process
 ****************************************************************************
 * DESCRIPTION: Process PKA ModExp CRT operation results. 
 *              - Copy the PKA operation results from PKCP/PKA registers
 *                and PKA RAM to output buffers 
 *
 *  int16_t salld_pka_op_modexp_crt_proc_setup (
 *            salldObj_t*       inst,         -> Pointer to SALLD instance
 *            Sa_PkaReqInfo2_t* pPkaReqInfo   -> Pointer to the PKA operation 
 *                                               request information structure
 *            void*             resultPtr     -> Point to the operation results
 *                                 )
 * Return values:   sa_ERR_OK
 *                  sa_PKA_OP_COMPLETE
 *                  sa_ERR_PKA_OP_ERROR
 *
 ***************************************************************************/
static int16_t salld_pka_op_modexp_crt_proc(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo, void* resultPtr)
{
    uintptr_t  baseAddr = (uintptr_t) salldLObj.baseAddr;
    CSL_Cp_aceRegs*  pSaRegs = (CSL_Cp_aceRegs*)baseAddr;
    CSL_Eip_29t2_ramEip_27b_eip27_registersRegs* pPkcpRegs = &pSaRegs->PKA.EIP_27B_EIP27_REGISTERS;  
    CSL_Eip_29t2_ramEip_28px12_gf2_2pram_eip28_registersRegs* pPkaCtrlRegs = &pSaRegs->PKA.EIP_28PX12_GF2_2PRAM_EIP28_REGISTERS;
    uint32_t *pResult = (uint32_t *)resultPtr;
    
    /* Extract the status code */
    pPkaReqInfo->statusCode = (pPkaCtrlRegs->PKA_SEQ_CTRL & CSL_EIP_29T2_RAM_PKA_SEQ_CTRL_SEQ_STATUS_MASK)
                              >> CSL_EIP_29T2_RAM_PKA_SEQ_CTRL_SEQ_STATUS_SHIFT;
                              
    if (pPkaReqInfo->statusCode == sa_PKA_OP_STATUS_SUCCESS)
    {
        /* Copy the operation result */
         baseAddr += SA_PKA_A_BASE;
         sap_pka_read(pResult, 
               (volatile uint32_t*)(baseAddr  + (pPkcpRegs->PKA_DPTR << 2)), 
                pPkaReqInfo->bLen << 1);
                
         return (sa_PKA_OP_COMPLETE);       
    }                  
    else
    {
         return (sa_ERR_PKA_OP_ERROR);       
    }        
}

/****************************************************************************
 * FUNCTION PURPOSE: PKA ECp Common Operation Process
 ****************************************************************************
 * DESCRIPTION: Process PKA ECpAdd/ECpMul/ECpScale operation results. 
 *              - Copy the PKA operation results from PKCP/PKA registers
 *                and PKA RAM to output buffers 
 *
 *  int16_t salld_pka_op_ecp_common_proc (
 *            salldObj_t*       inst,         -> Pointer to SALLD instance
 *            Sa_PkaReqInfo2_t* pPkaReqInfo   -> Pointer to the PKA operation 
 *                                               request information structure
 *            void*             resultPtr     -> Point to the operation results
 *                                 )
 * Return values:   sa_ERR_OK
 *                  sa_PKA_OP_COMPLETE
 *                  sa_ERR_PKA_OP_ERROR
 *
 ***************************************************************************/
static int16_t salld_pka_op_ecp_common_proc(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo, void* resultPtr)
{
    uintptr_t  baseAddr = (uintptr_t) salldLObj.baseAddr;
    CSL_Cp_aceRegs*  pSaRegs = (CSL_Cp_aceRegs*)baseAddr;
    CSL_Eip_29t2_ramEip_27b_eip27_registersRegs* pPkcpRegs = &pSaRegs->PKA.EIP_27B_EIP27_REGISTERS;  
    CSL_Eip_29t2_ramEip_28px12_gf2_2pram_eip28_registersRegs* pPkaCtrlRegs = &pSaRegs->PKA.EIP_28PX12_GF2_2PRAM_EIP28_REGISTERS;
    Sa_PkaECPoint_t*  pPoint0 = (Sa_PkaECPoint_t*)resultPtr;
    
    uint32_t offset = pPkcpRegs->PKA_DPTR;
    
    /* Extract the status code */
    pPkaReqInfo->statusCode = (pPkaCtrlRegs->PKA_SEQ_CTRL & CSL_EIP_29T2_RAM_PKA_SEQ_CTRL_SEQ_STATUS_MASK)
                              >> CSL_EIP_29T2_RAM_PKA_SEQ_CTRL_SEQ_STATUS_SHIFT;
                              
    baseAddr += SA_PKA_A_BASE;
    if ((pPkaReqInfo->statusCode == sa_PKA_OP_STATUS_SUCCESS) ||
        (pPkaReqInfo->statusCode == sa_PKA_OP_STATUS_INFINITY))
    {
        /* Copy the operation results */
        sap_pka_read(pPoint0->pX, 
              (volatile uint32_t*)(baseAddr  + (offset << 2)), 
               pPkaReqInfo->bLen);
        offset += (pPkaReqInfo->bLen + 2);
        offset += (offset & 0x1);
        
        sap_pka_read(pPoint0->pY, 
              (volatile uint32_t*)(baseAddr  + (offset << 2)), 
               pPkaReqInfo->bLen);
        offset += (pPkaReqInfo->bLen + 2);
        offset += (offset & 0x1);
        
        sap_pka_read(pPoint0->pZ, 
              (volatile uint32_t*)(baseAddr  + (offset << 2)), 
               pPkaReqInfo->bLen);
                
         return (sa_PKA_OP_COMPLETE);       
    }                  
    else
    {
         return (sa_ERR_PKA_OP_ERROR);       
    }        
}

/****************************************************************************
 * FUNCTION PURPOSE: PKA ECp Multiple plus Scale Operation Process
 ****************************************************************************
 * DESCRIPTION: Process PKA ECpMul plus ECpScale operation results. 
 *              - Copy the PKA operation results from PKCP/PKA registers
 *                and PKA RAM to output buffers 
 *
 *  int16_t salld_pka_op_ecp_mul_scale_proc (
 *            salldObj_t*       inst,         -> Pointer to SALLD instance
 *            Sa_PkaReqInfo2_t* pPkaReqInfo   -> Pointer to the PKA operation 
 *                                               request information structure
 *            void*             resultPtr     -> Point to the operation results
 *                                 )
 * Return values:   sa_ERR_OK
 *                  sa_PKA_OP_CONTINUE
 *                  sa_PKA_OP_COMPLETE
 *                  sa_ERR_PKA_OP_ERROR
 *
 ***************************************************************************/
static int16_t salld_pka_op_ecp_mul_scale_proc(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo, void* resultPtr)
{
    uintptr_t  baseAddr = (uintptr_t) salldLObj.baseAddr;
    CSL_Cp_aceRegs*  pSaRegs = (CSL_Cp_aceRegs*)baseAddr;
    CSL_Eip_29t2_ramEip_27b_eip27_registersRegs* pPkcpRegs = &pSaRegs->PKA.EIP_27B_EIP27_REGISTERS;  
    CSL_Eip_29t2_ramEip_28px12_gf2_2pram_eip28_registersRegs* pPkaCtrlRegs = &pSaRegs->PKA.EIP_28PX12_GF2_2PRAM_EIP28_REGISTERS;
    Sa_PkaECPoint_t*  pPoint0 = (Sa_PkaECPoint_t*)resultPtr;
    salldPkaInst_t*  pPkaInst = &inst->pkaInst;   
    
    /* Extract the status code */
    pPkaReqInfo->statusCode = (pPkaCtrlRegs->PKA_SEQ_CTRL & CSL_EIP_29T2_RAM_PKA_SEQ_CTRL_SEQ_STATUS_MASK)
                              >> CSL_EIP_29T2_RAM_PKA_SEQ_CTRL_SEQ_STATUS_SHIFT;
                              
    baseAddr += SA_PKA_A_BASE;
    switch (pPkaInst->nextState)
    {
        case SA_PKA_OP_STATE_STEP1:
            if (pPkaReqInfo->statusCode == sa_PKA_OP_STATUS_SUCCESS)
            {
                /* Record the output offset */
                pPkaInst->offset = pPkcpRegs->PKA_DPTR;
                
                /* Trigger the next operation */
                pPkcpRegs->PKA_FUNCTION  = SA_PKA_COMPLEX_OP_ECp_SCALE | CSL_EIP_29T2_RAM_PKA_FUNCTION_RUN_MASK;
                
                pPkaInst->nextState = SA_PKA_OP_STATE_DONE;
                
                return(sa_PKA_OP_CONTINUE);
            }
            
            return (sa_ERR_PKA_OP_ERROR); 
            
        case SA_PKA_OP_STATE_DONE:
            if ((pPkaReqInfo->statusCode == sa_PKA_OP_STATUS_SUCCESS) ||
                (pPkaReqInfo->statusCode == sa_PKA_OP_STATUS_INFINITY))
            {
               uint32_t offset = pPkaInst->offset;
            
                /* Copy the operation results */
                sap_pka_read(pPoint0->pX, 
                   (volatile uint32_t*)(baseAddr  + (offset << 2)), 
                    pPkaReqInfo->bLen);
                offset += (pPkaReqInfo->bLen + 2);
                offset += (offset & 0x1);
        
                sap_pka_read(pPoint0->pY, 
                   (volatile uint32_t*)(baseAddr  + (offset << 2)), 
                    pPkaReqInfo->bLen);
                offset += (pPkaReqInfo->bLen + 2);
                offset += (offset & 0x1);
        
                sap_pka_read(pPoint0->pZ, 
                   (volatile uint32_t*)(baseAddr  + (offset << 2)), 
                    pPkaReqInfo->bLen);
                
                return (sa_PKA_OP_COMPLETE);       
            }     
            
            /*  pass through */                 
                  
        default:
            return (sa_ERR_PKA_OP_ERROR);        
    }                          
}

/****************************************************************************
 * FUNCTION PURPOSE: PKA ECp DSA Verify Operation Process
 ****************************************************************************
 * DESCRIPTION: Process PKA ECp DSA Sign operation results. 
 *              - Copy the PKA operation results from PKCP/PKA registers
 *                and PKA RAM to output buffers 
 *
 *  int16_t salld_pka_op_ecp_dsa_verify_proc (
 *            salldObj_t*       inst,         -> Pointer to SALLD instance
 *            Sa_PkaReqInfo2_t* pPkaReqInfo   -> Pointer to the PKA operation 
 *                                               request information structure
 *            void*             resultPtr     -> Point to the operation results
 *                                 )
 * Return values:   sa_ERR_OK
 *                  sa_PKA_OP_COMPLETE
 *                  sa_ERR_PKA_OP_ERROR
 *
 ***************************************************************************/
static int16_t salld_pka_op_ecp_dsa_verify_proc(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo, void* resultPtr)
{
  uintptr_t  baseAddr = (uintptr_t) salldLObj.baseAddr;
  CSL_Cp_aceRegs*  pSaRegs = (CSL_Cp_aceRegs*)baseAddr;
  Sa_PkaECDSAVerifyParams_t* pPkaOp = (Sa_PkaECDSAVerifyParams_t*)resultPtr;
  CSL_Eip_29t2_ramEip_28px12_gf2_2pram_eip28_registersRegs* pPkaCtrlRegs = &pSaRegs->PKA.EIP_28PX12_GF2_2PRAM_EIP28_REGISTERS;
  CSL_Eip_29t2_ramEip_27b_eip27_registersRegs* pPkcpRegs = &pSaRegs->PKA.EIP_27B_EIP27_REGISTERS;
  uint32_t   i, compResult;

  /* Basic Operation: Just wait for results since it will take only a few cycles */
  /* Wait for the operation to be complete or timeout occurs */
  for (i = 0; i < SA_PKA_MAX_PKCP_OP_LOOP_COUNT; i++)
  {
      if (!(pPkcpRegs->PKA_FUNCTION & CSL_EIP_29T2_RAM_PKA_FUNCTION_RUN_MASK))
          break;
  }

  /* figure out how to calculate the size of operation result */
  if (i >= SA_PKA_MAX_PKCP_OP_LOOP_COUNT)
  {
      return(sa_ERR_PKA_TIMEOUT);
  }

  /* Extract the status code */
  pPkaReqInfo->statusCode = (pPkaCtrlRegs->PKA_SEQ_CTRL & CSL_EIP_29T2_RAM_PKA_SEQ_CTRL_SEQ_STATUS_MASK)
                            >> CSL_EIP_29T2_RAM_PKA_SEQ_CTRL_SEQ_STATUS_SHIFT;

  /* Compare result value */
  compResult  = (pPkcpRegs->PKA_COMPARE & CSL_EIP_29T2_RAM_PKA_COMPARE_A_EQUAL_B_MASK)
                            >> CSL_EIP_29T2_RAM_PKA_COMPARE_A_EQUAL_B_SHIFT;

  /* Copy the operation results */
  *pPkaOp->pRes                = (uint32_t) compResult;
  if ((pPkaReqInfo->statusCode == sa_PKA_OP_STATUS_SUCCESS) &&
      (compResult              == sa_PKA_OP_STATUS_SUCCESS))
  {
    return(sa_PKA_OP_COMPLETE);
  }
  else {
    return (sa_ERR_PKA_OP_ERROR);
  }
}

/****************************************************************************
 * FUNCTION PURPOSE: PKA ECp DSA Sign Operation Process
 ****************************************************************************
 * DESCRIPTION: Process PKA ECp DSA Sign operation results. 
 *              - Copy the PKA operation results from PKCP/PKA registers
 *                and PKA RAM to output buffers 
 *
 *  int16_t salld_pka_op_ecp_dsa_sign_proc (
 *            salldObj_t*       inst,         -> Pointer to SALLD instance
 *            Sa_PkaReqInfo2_t* pPkaReqInfo   -> Pointer to the PKA operation 
 *                                               request information structure
 *            void*             resultPtr     -> Point to the operation results
 *                                 )
 * Return values:   sa_ERR_OK
 *                  sa_PKA_OP_CONTINUE
 *                  sa_PKA_OP_COMPLETE
 *                  sa_ERR_PKA_OP_ERROR
 *
 ***************************************************************************/
static int16_t salld_pka_op_ecp_dsa_sign_proc(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo, void* resultPtr)
{
    uintptr_t  baseAddr = (uintptr_t) salldLObj.baseAddr;
    CSL_Cp_aceRegs*  pSaRegs = (CSL_Cp_aceRegs*)baseAddr;
    Sa_PkaECDSASignParams_t* pPkaOp = (Sa_PkaECDSASignParams_t*)resultPtr;
    CSL_Eip_29t2_ramEip_28px12_gf2_2pram_eip28_registersRegs* pPkaCtrlRegs = &pSaRegs->PKA.EIP_28PX12_GF2_2PRAM_EIP28_REGISTERS;
    CSL_Eip_29t2_ramEip_27b_eip27_registersRegs* pPkcpRegs = &pSaRegs->PKA.EIP_27B_EIP27_REGISTERS;
    salldPkaInst_t*  pPkaInst = &inst->pkaInst;
    pkaEcdsaSignInst_t* pEcdsaInst = &pPkaInst->inst.ecdsaSign;
    uint32_t   i;

    /* Basic Operation: Just wait for results since it will take only a few cycles */
    /* Wait for the operation to be complete or timeout occurs */
    for (i = 0; i < SA_PKA_MAX_PKCP_OP_LOOP_COUNT; i++)
    {
        if (!(pPkcpRegs->PKA_FUNCTION & CSL_EIP_29T2_RAM_PKA_FUNCTION_RUN_MASK))
            break;
    }
  
    /* figure out how to calculate the size of operation result */
    if (i >= SA_PKA_MAX_PKCP_OP_LOOP_COUNT)
    {
        return(sa_ERR_PKA_TIMEOUT);
    }
  
    /* Extract the status code */
    pPkaReqInfo->statusCode = (pPkaCtrlRegs->PKA_SEQ_CTRL & CSL_EIP_29T2_RAM_PKA_SEQ_CTRL_SEQ_STATUS_MASK)
                              >> CSL_EIP_29T2_RAM_PKA_SEQ_CTRL_SEQ_STATUS_SHIFT;
  
    if (pPkaReqInfo->statusCode == sa_PKA_OP_STATUS_SUCCESS)
    {
      /* Copy the operation result */
       baseAddr += SA_PKA_A_BASE;
       sap_pka_read(pPkaOp->pR,
             (volatile uint32_t*)(baseAddr  + (pEcdsaInst->offsetR << 2)),
              pPkaReqInfo->bLen);

       sap_pka_read(pPkaOp->pS,
             (volatile uint32_t*)(baseAddr  + (pEcdsaInst->offsetS << 2)),
              pPkaReqInfo->bLen);

       return(sa_PKA_OP_COMPLETE);
    }
    else {
      return (sa_ERR_PKA_OP_ERROR);
    }
}

/****************************************************************************
 * FUNCTION PURPOSE: PKA EC2m Common Operation Process
 ****************************************************************************
 * DESCRIPTION: Process PKA EC2mAdd/EC2mMul/EC2mScale operation results. 
 *              - Copy the PKA operation results from PKCP/PKA registers
 *                and PKA RAM to output buffers 
 *
 *  int16_t salld_pka_op_ecp_common_proc (
 *            salldObj_t*       inst,         -> Pointer to SALLD instance
 *            Sa_PkaReqInfo2_t* pPkaReqInfo   -> Pointer to the PKA operation 
 *                                               request information structure
 *            void*             resultPtr     -> Point to the operation results
 *                                 )
 * Return values:   sa_ERR_OK
 *                  sa_PKA_OP_COMPLETE
 *                  sa_ERR_PKA_OP_ERROR
 *
 ***************************************************************************/
static int16_t salld_pka_op_ec2m_common_proc(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo, void* resultPtr)
{
    uintptr_t  baseAddr = (uintptr_t) salldLObj.baseAddr;
    CSL_Cp_aceRegs*  pSaRegs = (CSL_Cp_aceRegs*)baseAddr;
    CSL_Eip_29t2_ramEip_27b_eip27_registersRegs* pPkcpRegs = &pSaRegs->PKA.EIP_27B_EIP27_REGISTERS;  
    CSL_Eip_29t2_ramEip_28px12_gf2_2pram_eip28_registersRegs* pPkaCtrlRegs = &pSaRegs->PKA.EIP_28PX12_GF2_2PRAM_EIP28_REGISTERS;
    Sa_PkaECPoint_t*  pPoint0 = (Sa_PkaECPoint_t*)resultPtr;
    
    uint32_t offset = pPkcpRegs->PKA_DPTR;
    
    /* Extract the status code */
    pPkaReqInfo->statusCode = (pPkaCtrlRegs->PKA_SEQ_CTRL & CSL_EIP_29T2_RAM_PKA_SEQ_CTRL_SEQ_STATUS_MASK)
                              >> CSL_EIP_29T2_RAM_PKA_SEQ_CTRL_SEQ_STATUS_SHIFT;
                              
    baseAddr += SA_PKA_A_BASE;
    if ((pPkaReqInfo->statusCode == sa_PKA_OP_STATUS_SUCCESS) ||
        (pPkaReqInfo->statusCode == sa_PKA_OP_STATUS_INFINITY))
    {
        /* Copy the operation results */
        sap_pka_read(pPoint0->pX, 
              (volatile uint32_t*)(baseAddr  + (offset << 2)), 
               pPkaReqInfo->bLen);
        offset += (pPkaReqInfo->bLen);
        offset += (offset & 0x1);
        
        sap_pka_read(pPoint0->pY, 
              (volatile uint32_t*)(baseAddr  + (offset << 2)), 
               pPkaReqInfo->bLen);
        offset += (pPkaReqInfo->bLen);
        offset += (offset & 0x1);
        
        sap_pka_read(pPoint0->pZ, 
              (volatile uint32_t*)(baseAddr  + (offset << 2)), 
               pPkaReqInfo->bLen);
                
         return (sa_PKA_OP_COMPLETE);       
    }                  
    else
    {
         return (sa_ERR_PKA_OP_ERROR);       
    }        
}


/****************************************************************************
 * FUNCTION PURPOSE: PKA EC2m Multiple plus Scale Operation Process
 ****************************************************************************
 * DESCRIPTION: Process PKA EC2mMul plus EC2mScale operation results. 
 *              - Copy the PKA operation results from PKCP/PKA registers
 *                and PKA RAM to output buffers 
 *
 *  int16_t salld_pka_op_ecp_mul_scale_proc (
 *            salldObj_t*       inst,         -> Pointer to SALLD instance
 *            Sa_PkaReqInfo2_t* pPkaReqInfo   -> Pointer to the PKA operation 
 *                                               request information structure
 *            void*             resultPtr     -> Point to the operation results
 *                                 )
 * Return values:   sa_ERR_OK
 *                  sa_PKA_OP_CONTINUE
 *                  sa_PKA_OP_COMPLETE
 *                  sa_ERR_PKA_OP_ERROR
 *
 ***************************************************************************/
static int16_t salld_pka_op_ec2m_mul_scale_proc(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo, void* resultPtr)
{
    uintptr_t  baseAddr = (uintptr_t) salldLObj.baseAddr;
    CSL_Cp_aceRegs*  pSaRegs = (CSL_Cp_aceRegs*)baseAddr;
    CSL_Eip_29t2_ramEip_27b_eip27_registersRegs* pPkcpRegs = &pSaRegs->PKA.EIP_27B_EIP27_REGISTERS;  
    CSL_Eip_29t2_ramEip_28px12_gf2_2pram_eip28_registersRegs* pPkaCtrlRegs = &pSaRegs->PKA.EIP_28PX12_GF2_2PRAM_EIP28_REGISTERS;
    Sa_PkaECPoint_t*  pPoint0 = (Sa_PkaECPoint_t*)resultPtr;
    salldPkaInst_t*  pPkaInst = &inst->pkaInst;   
    
    /* Extract the status code */
    pPkaReqInfo->statusCode = (pPkaCtrlRegs->PKA_SEQ_CTRL & CSL_EIP_29T2_RAM_PKA_SEQ_CTRL_SEQ_STATUS_MASK)
                              >> CSL_EIP_29T2_RAM_PKA_SEQ_CTRL_SEQ_STATUS_SHIFT;
                              
    baseAddr += SA_PKA_A_BASE;
    switch (pPkaInst->nextState)
    {
        case SA_PKA_OP_STATE_STEP1:
            if (pPkaReqInfo->statusCode == sa_PKA_OP_STATUS_SUCCESS)
            {
                /* Record the output offset */
                pPkaInst->offset = pPkcpRegs->PKA_DPTR;
                
                /* Trigger the next operation */
                pPkcpRegs->PKA_FUNCTION  = SA_PKA_COMPLEX_OP_EC2m_SCALE | CSL_EIP_29T2_RAM_PKA_FUNCTION_RUN_MASK;
                
                pPkaInst->nextState = SA_PKA_OP_STATE_DONE;
                
                return(sa_PKA_OP_CONTINUE);
            }
            
            return (sa_ERR_PKA_OP_ERROR); 
            
        case SA_PKA_OP_STATE_DONE:
            if ((pPkaReqInfo->statusCode == sa_PKA_OP_STATUS_SUCCESS) ||
                (pPkaReqInfo->statusCode == sa_PKA_OP_STATUS_INFINITY))
            {
               uint32_t offset = pPkaInst->offset;
            
                /* Copy the operation results */
                sap_pka_read(pPoint0->pX, 
                   (volatile uint32_t*)(baseAddr  + (offset << 2)), 
                    pPkaReqInfo->bLen);
                offset += (pPkaReqInfo->bLen);
                offset += (offset & 0x1);
        
                sap_pka_read(pPoint0->pY, 
                   (volatile uint32_t*)(baseAddr  + (offset << 2)), 
                    pPkaReqInfo->bLen);
                offset += (pPkaReqInfo->bLen);
                offset += (offset & 0x1);
        
                sap_pka_read(pPoint0->pZ, 
                   (volatile uint32_t*)(baseAddr  + (offset << 2)), 
                    pPkaReqInfo->bLen);
                
                return (sa_PKA_OP_COMPLETE);       
            }     
            
            /*  pass through */                 
                  
        default:
            return (sa_ERR_PKA_OP_ERROR);        
    }                          
}



/******************************************************************************
 * FUNCTION PURPOSE:  PKA Poll Operation 
 ******************************************************************************
 * DESCRIPTION: This function is called to check the status of the current large 
 *              vector arithmetic operation through the PKA module and then act
 *              accordingly.
 *
 *              If the operation is still in progress, it just returns 
 *              sa_PKA_OP_IN_PROGRESS. If the operation is complete and more operation 
 *              is required, it will prepare and trigger the next (complex) operation 
 *              and then return sa_PKA_OP_CONTINUE, otherwise, it copies operation 
 *              results from the PKA registers and vector RAM to the result locations 
 *              specified by the PKA request information data structure and 
 *              return sa_PKA_OP_COMPLETE. 
 *
 *  int16_t salld_pkt_poll (
 *   salldObj_t*       inst,        - Pointer to SALLD instance
 *   Sa_PkaReqInfo2_t* pPkaReqInfo) - Pointer to the PKA operation request information structure
 *
 * Return values:  sa_PKA_OP_IN_PROGRESS
 *                 sa_PKA_OP_CONTINUE 
 *                 sa_PKA_OP_COMPLETE
 *                 sa_ERR_PKA_OP_ERROR 
 *                 sa_ERR_PKA_INVALID_STATE
 *                 sa_ERR_INV_ENDIAN_MODE 
 *                 sa_ERR_INV_HANDLE
 *
 ******************************************************************************/
static int16_t salld_pkt_poll(salldObj_t *inst, Sa_PkaReqInfo2_t* pPkaReqInfo2)
{
    CSL_Cp_aceRegs*  pSaRegs = (CSL_Cp_aceRegs*)salldLObj.baseAddr;
    CSL_Eip_29t2_ramEip_29t2_ram_host_registersRegs* pHostRegs = &pSaRegs->PKA.EIP_29T2_RAM_HOST_REGISTERS;
    Sa_PkaReqInfo2_t* pPkaReqInfo = &inst->pkaReqInfo; 
    //salldPkaInst_t*   pPkaInst = &inst->pkaInst;
    int16_t ret_code = sa_ERR_OK;
    
    if (pPkaReqInfo->operation <= sa_PKA_OP_MOD)
    {
        return(sa_ERR_PKA_INVALID_STATE);
    }  
    
    /* Verify whether the previous command is completed */
    if (pHostRegs->PKA_IRQSTATUS & CSL_EIP_29T2_RAM_PKA_IRQENABLE_PKAIRQEN_MASK)
    {
        /* clear  interrupt */
        pHostRegs->PKA_IRQSTATUS = CSL_EIP_29T2_RAM_PKA_IRQENABLE_PKAIRQEN_MASK;
    }
    else
    {
        return(sa_PKA_OP_IN_PROGRESS);
    }
    
    /* Process operation result and continue */
    switch (pPkaReqInfo->operation)
    {
        case sa_PKA_OP_MODEXP:
            ret_code = salld_pka_op_mod_common_proc(inst, pPkaReqInfo, (void *)pPkaReqInfo->params.modExp.pResult);
            break;
        
        case sa_PKA_OP_MODEXP_CRT:
            ret_code = salld_pka_op_modexp_crt_proc(inst, pPkaReqInfo, (void *)pPkaReqInfo->params.modExpCRT.pResult);
            break;
        
        case sa_PKA_OP_MODINVp:
        case sa_PKA_OP_MODINV2m:
            ret_code = salld_pka_op_mod_common_proc(inst, pPkaReqInfo, (void *)pPkaReqInfo->params.modInv.pResult);
            break;
        
        case sa_PKA_OP_ECp_ADD:
            ret_code = salld_pka_op_ecp_common_proc(inst, pPkaReqInfo, (void *)&pPkaReqInfo->params.ecAdd.point0);
            break;
        
        case sa_PKA_OP_ECp_MUL:
            ret_code = salld_pka_op_ecp_common_proc(inst, pPkaReqInfo, (void *)&pPkaReqInfo->params.ecMul.point0);
            break;
        
        case sa_PKA_OP_ECp_SCALE:
            ret_code = salld_pka_op_ecp_common_proc(inst, pPkaReqInfo, (void *)&pPkaReqInfo->params.ecScale.point0);
            break;
    
        case sa_PKA_OP_ECp_MUL_SACLE:
            ret_code = salld_pka_op_ecp_mul_scale_proc(inst, pPkaReqInfo, (void *)&pPkaReqInfo->params.ecMul.point0);
            break;
            
        case sa_PKA_OP_ECp_DSA_SIGN:
            ret_code = salld_pka_op_ecp_dsa_sign_proc(inst, pPkaReqInfo, (void *)&pPkaReqInfo->params.ecDSASign);
            break;

        case sa_PKA_OP_ECp_DSA_VERIFY:
            ret_code = salld_pka_op_ecp_dsa_verify_proc(inst, pPkaReqInfo, (void *)&pPkaReqInfo->params.ecDSAVerify);
            break;

        case sa_PKA_OP_EC2m_ADD:
            ret_code = salld_pka_op_ec2m_common_proc(inst, pPkaReqInfo, (void *)&pPkaReqInfo->params.ecAdd.point0);
            break;
        
        case sa_PKA_OP_EC2m_MUL:
            ret_code = salld_pka_op_ec2m_common_proc(inst, pPkaReqInfo, (void *)&pPkaReqInfo->params.ecMul.point0);
            break;
        
        case sa_PKA_OP_EC2m_SCALE:
            ret_code = salld_pka_op_ec2m_common_proc(inst, pPkaReqInfo, (void *)&pPkaReqInfo->params.ecScale.point0);
            break;
    
        case sa_PKA_OP_EC2m_MUL_SACLE:
            ret_code = salld_pka_op_ec2m_mul_scale_proc(inst, pPkaReqInfo, (void *)&pPkaReqInfo->params.ecMul.point0);
            break;
        
        default:
            return(sa_ERR_PKA_OP_UNSUPPORTED);
    }
    
    if ((ret_code == sa_PKA_OP_COMPLETE) || (ret_code < 0))
    {
        pHostRegs->PKA_IRQENABLE &= ~CSL_EIP_29T2_RAM_PKA_IRQENABLE_PKAIRQEN_MASK;   
        
        /* PKA operation is completed */
        SALLD_SET_PKA_ACTIVE(inst, 0);
        
        /* Update the local copy of the PKA request information */
        if(pPkaReqInfo2 && (pPkaReqInfo2 != pPkaReqInfo))
            *pPkaReqInfo2 = *pPkaReqInfo;
        
    }
    
    return (ret_code);
}  

/******************************************************************************
 * FUNCTION PURPOSE:  Perform large vector arithmetic operation via PKA 
 ******************************************************************************
 * DESCRIPTION: This function is called to triggers a large vector arithmetic 
 *              operation through the PKA module.
 *              It can operate in either blocking or non-blocking mode. 
 *              In blocking mode, this function will not return until the PKA 
 *              module completes all requested large vector arithmetic operations.
 *              In non-blocking mode, this function will returns immediately after 
 *              it sets up and starts the first requested PKA operation.  
 *              This function also returns with error code immediately if the PKA 
 *              in not initialized or it is still in the process to perform the 
 *               previous operation or when timeout occurs.
 *
 *  int16_t Sa_pkaOperation2 (
 *   Sa_Handle        handle       - SALLD instance handle
 *   Sa_PkaReqInfo2_t pPkaReqInfo) - Pointer to the PKA operation request information structure
 *
 * Return values:  sa_ERR_OK
 *                 sa_ERR_PARAMS 
 *                 sa_ERR_MOUDLE_BUSY
 *                 sa_ERR_MODULE_UNAVAIL 
 *                 sa_ERR_PKA_NOT_READY
 *                 sa_ERR_PKA_TIMEOUT
 *
 ******************************************************************************/
int16_t Sa_pkaOperation2 (Sa_Handle handle, Sa_PkaReqInfo2_t* pPkaReqInfo)
{
    salldObj_t *inst = (salldObj_t *)sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, handle);
    CSL_Cp_aceRegs*  pSaRegs = (CSL_Cp_aceRegs*)salldLObj.baseAddr;
    CSL_Eip_29t2_ramEip_29t2_ram_host_registersRegs* pHostRegs = &pSaRegs->PKA.EIP_29T2_RAM_HOST_REGISTERS;
    int16_t ret_code;
    uint32_t mtKey;
  
    if (inst->magic != SALLD_INST_MAGIC_WORD)
    {
        if(inst->magic == salld_swiz32(SALLD_INST_MAGIC_WORD))
            return(sa_ERR_INV_ENDIAN_MODE);
        else
            return(sa_ERR_INV_HANDLE);    
    } 
  
    if (pPkaReqInfo == NULL) 
    {
        return(sa_ERR_PARAMS);
    }
  
    Sa_osalMtCsEnter(&mtKey); 
  
    /* Invalidate the Cache Contents */
    Sa_osalBeginMemAccess(inst, sizeof(salldObj_t));  
  
    if(!SALLD_TEST_STATE_PKA(inst))
    {
        ret_code = sa_ERR_MODULE_UNAVAIL;
    } 
    else if (SALLD_TEST_PKA_ACTIVE(inst))
    {
        ret_code = sa_ERR_PKA_NOT_READY;
    }
    else
    {
        /* PKA Engine is available to be used */
        if (pPkaReqInfo->operation <= sa_PKA_OP_MOD)
        {
            ret_code = salld_pka_op_pkcp_setup(inst, pPkaReqInfo);
            
            if (ret_code == sa_ERR_OK)
            {
                ret_code = salld_pka_op_pkcp_proc(inst, pPkaReqInfo, NULL);
            }
        }
        else
        {
            /* Setup PKA complex operations per operation */
            /* Enable PKA interrupt */
            pHostRegs->PKA_IRQENABLE |= CSL_EIP_29T2_RAM_PKA_IRQENABLE_PKAIRQEN_MASK;   
            
            /* TBD: Verify whether the previous command is completed */
            if (pHostRegs->PKA_IRQSTATUS & CSL_EIP_29T2_RAM_PKA_IRQENABLE_PKAIRQEN_MASK)
            {
                /* clear  interrupt */
                pHostRegs->PKA_IRQSTATUS = CSL_EIP_29T2_RAM_PKA_IRQENABLE_PKAIRQEN_MASK;
            }
            
            switch (pPkaReqInfo->operation)
            {
                case sa_PKA_OP_MODEXP:
                    ret_code = salld_pka_op_modexp_setup(inst, pPkaReqInfo);
                    break;
                
                case sa_PKA_OP_MODEXP_CRT:
                    ret_code = salld_pka_op_modexp_crt_setup(inst, pPkaReqInfo);
                    break;
                
                case sa_PKA_OP_MODINVp:
                case sa_PKA_OP_MODINV2m:
                    ret_code = salld_pka_op_modinv_setup(inst, pPkaReqInfo);
                    break;
                
                case sa_PKA_OP_ECp_ADD:
                    ret_code = salld_pka_op_ecp_add_setup(inst, pPkaReqInfo);
                    break;
                
                case sa_PKA_OP_ECp_MUL:
                    ret_code = salld_pka_op_ecp_mul_setup(inst, pPkaReqInfo);
                    break;
                
                case sa_PKA_OP_ECp_SCALE:
                    ret_code = salld_pka_op_ecp_scale_setup(inst, pPkaReqInfo);
                    break;
        
                case sa_PKA_OP_ECp_MUL_SACLE:
                    ret_code = salld_pka_op_ecp_mul_scale_setup(inst, pPkaReqInfo);
                    break;
        
                case sa_PKA_OP_ECp_DSA_SIGN:
                    ret_code = salld_pka_op_ecp_dsa_sign_setup(inst, pPkaReqInfo);
                    break;

                case sa_PKA_OP_ECp_DSA_VERIFY:
                    ret_code = salld_pka_op_ecp_dsa_verify_setup(inst, pPkaReqInfo);
                    break;

                case sa_PKA_OP_EC2m_ADD:
                    ret_code = salld_pka_op_ec2m_add_setup(inst, pPkaReqInfo);
                    break;
                
                case sa_PKA_OP_EC2m_MUL:
                    ret_code = salld_pka_op_ec2m_mul_setup(inst, pPkaReqInfo);
                    break;
                
                case sa_PKA_OP_EC2m_SCALE:
                    ret_code = salld_pka_op_ec2m_scale_setup(inst, pPkaReqInfo);
                    break;
        
                case sa_PKA_OP_EC2m_MUL_SACLE:
                    ret_code = salld_pka_op_ec2m_mul_scale_setup(inst, pPkaReqInfo);
                    break;
                    
                default:
                    ret_code = sa_ERR_PKA_OP_UNSUPPORTED;
                    break;
            }
            
            if (ret_code == sa_ERR_OK)
            {
                /* Indicate that PKA operation is in progress */
                SALLD_SET_PKA_ACTIVE(inst, 1);   
                
                /* Record the PKA request information to used at PKA Pooling routine */ 
                inst->pkaReqInfo = *pPkaReqInfo;
            
                if(pPkaReqInfo->mode == sa_PKA_MODE_WAIT)
                {
                    /* Invoke salld_pkt_poll() until the operation is completed or error occurs */
                    do
                    {
                        ret_code = salld_pkt_poll(inst, pPkaReqInfo);
                
                    } while ((ret_code == sa_PKA_OP_IN_PROGRESS) || (ret_code == sa_PKA_OP_CONTINUE)); 
            
                }
                else
                {
                    ret_code = sa_PKA_OP_IN_PROGRESS;
                }
            }
            else
            {
                /* Disable PKA interrupt */
                pHostRegs->PKA_IRQENABLE &= ~CSL_EIP_29T2_RAM_PKA_IRQENABLE_PKAIRQEN_MASK;   
            }
        }
    }
  
    /* Writeback the instance updates */
    Sa_osalEndMemAccess(inst, sizeof(salldObj_t));  
  
    Sa_osalMtCsExit(mtKey);
  
    return(ret_code);  
}

/******************************************************************************
 * FUNCTION PURPOSE:  Continue large vector arithmetic operation via PKA 
 ******************************************************************************
 * DESCRIPTION: This function is called to check the status of the current large 
 *              vector arithmetic operation through the PKA module and then act
 *              accordingly.
 *
 *              If the operation is still in progress, it just returns 
 *              sa_PKA_OP_IN_PROGRESS. If the operation is complete and more operation 
 *              is required, it will prepare and trigger the next (complex) operation 
 *              and then return sa_PKA_OP_CONTINUE, otherwise, it copies operation 
 *              results from the PKA registers and vector RAM to the result locations 
 *              specified by the PKA request information data structure and 
 *              return sa_PKA_OP_COMPLETE. 
 *
 *  int16_t Sa_pkaPoll (
 *   Sa_Handle         handle       - SALLD instance handle
 *   Sa_PkaReqInfo2_t* pPkaReqInfo) - Pointer to the PKA operation request information structure
 *
 * Return values:  sa_PKA_OP_IN_PROGRESS
 *                 sa_PKA_OP_CONTINUE 
 *                 sa_PKA_OP_COMPLETE
 *                 sa_ERR_PKA_OP_ERROR 
 *                 sa_ERR_PKA_INVALID_STATE
 *                 sa_ERR_INV_ENDIAN_MODE 
 *                 sa_ERR_INV_HANDLE
 *
 ******************************************************************************/
int16_t Sa_pkaPoll (Sa_Handle handle, Sa_PkaReqInfo2_t* pPkaReqInfo)
{
    salldObj_t *inst = (salldObj_t *)sa_CONV_OFFSET_TO_ADDR(salldLObj.instPoolBaseAddr, handle);
    int16_t  ret_code;
    uint32_t mtKey;

    if (inst->magic != SALLD_INST_MAGIC_WORD)
    {
        if(inst->magic == salld_swiz32(SALLD_INST_MAGIC_WORD))
            return(sa_ERR_INV_ENDIAN_MODE);
        else
            return(sa_ERR_INV_HANDLE);    
    } 
    
    Sa_osalMtCsEnter(&mtKey); 
  
    /* Invalidate the Cache Contents */
    Sa_osalBeginMemAccess(inst, sizeof(salldObj_t));
    
    if(!SALLD_TEST_STATE_PKA(inst))
    {
        ret_code = sa_ERR_MODULE_UNAVAIL;
    }
    else if (!SALLD_TEST_PKA_ACTIVE(inst))
    {
        ret_code = sa_ERR_PKA_INVALID_STATE;
    }
    else
    {
        ret_code = salld_pkt_poll(inst, pPkaReqInfo);
    }
    
    /* Writeback the instance updates */
    Sa_osalEndMemAccess(inst, sizeof(salldObj_t));  
  
    Sa_osalMtCsExit(mtKey);
  
    return(ret_code);  
}

#endif






  




