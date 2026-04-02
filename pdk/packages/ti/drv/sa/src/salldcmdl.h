#ifndef _SALLDCMDL_H
#define _SALLDCMDL_H
/*******************************************************************************
 * FILE PURPOSE: Provide Security Accelerator (SA) Command Label related defintions
 *
 ********************************************************************************
 * FILE NAME:   salldcmdl.h
 *
 * DESCRIPTION: Define the Commnad Label related data structures, MACRO and constands
 *              used by the Security Accelerator (SA)
 *
 *     0                   1                   2                   3
 *     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |     NESC      |   Cmdl Len    |    Length to be processed     |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |  SOP Offset   |  Opt Ctrl 1   |  Opt Ctrl 2   |  Opt Ctrl 3   |                
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                       Option 1                                |
 *   |                 (Variable Size in 8 Bytes)                    |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                                                               |
 *   |                      Option 2                                 |
 *   |                (Variable Size in 8 Bytes)                     |
 *   |                               +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                               |       Padding                 |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                       Option 3                                |
 *   |                 (Variable Size in 8 Bytes)                    |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 *                    Figure: Command Label Format
 *    
 *   Where NESC: Next Engine Select Code
 *         CMDL: Command Label
 *         SOP:  Start Of Packet    
 *
 * REVISION HISTORY:
 *
 * (C) Copyright 2009 Texas Instruments, Inc.
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
 
/******************************************************************************
 * Command Label Definitions
 ******************************************************************************/
#define SALLD_CMDL_BYTE_OFFSET_NESC           0      /* Next Engine Select Code */
#define SALLD_CMDL_BYTE_OFFSET_LABEL_LEN      1      /* Engine Command Label Length */
#define SALLD_CMDL_BYTE_OFFSET_DATA_LEN       2      /* 16-bit Length of Data to be processed */
#define SALLD_CMDL_BYTE_OFFSET_DATA_OFFSET    4      /* Stat Data Offset */
#define SALLD_CMDL_BYTE_OFFSET_OPTION_CTRL1   5      /* Option Control Byte 1 */
#define SALLD_CMDL_BYTE_OFFSET_OPTION_CTRL2   6      /* Option Control Byte 2 */
#define SALLD_CMDL_BYTE_OFFSET_OPTION_CTRL3   7      /* Option Control Byte 3 */
#define SALLD_CMDL_BYTE_OFFSET_OPTION_BYTE    8

#define SALLD_CMDL_HEADER_SIZE_BYTES          8
         
#define SALLD_CMDL_OPTION_BYTES_MAX_SIZE     72
#define SALLD_CMDL_MAX_SIZE_BYTES           (SALLD_CMDL_HEADER_SIZE_BYTES + SALLD_CMDL_OPTION_BYTES_MAX_SIZE)


/******************************************************************************
 * Command Label Engine ID Definitions
 ******************************************************************************/
#define SALLD_CMDL_ENGINE_DEFAULT             0     /* Use the default engine ID specified at the register */
                                                    /* For Host packet only */     
#define SALLD_CMDL_ENGINE_ID_ES1              2     /* ES Pass1: Encryption/Descryption operation AES/DES */
#define SALLD_CMDL_ENGINE_ID_ES2              3     /* ES Pass2: Second level Encryption/Descryption such as CCM */
 
#define SALLD_CMDL_ENGINE_ID_AS1              4     /* AS Pass1: Hashing operation (Authentication) SHA1, SHA2 and MD5 */ 
#define SALLD_CMDL_ENGINE_ID_AS2              5     /* AS Pass2: Second level Hashing operation */

#define SALLD_CMDL_ENGINE_ID_2ES1             6     /* 2ES Pass1: Second instance of ES Pass1 */
#define SALLD_CMDL_ENGINE_ID_2ES2             7     /* 2ES Pass2: Second instance of ES Pass2 */

#define SALLD_CMDL_ENGINE_IPSEC_HPS1          8     /* IPSEC HPS Pass1: packet header parsing and inspection */
#define SALLD_CMDL_ENGINE_IPSEC_HPS2          9     /* IPSEC HPS Pass2: payload processing verification */
#define SALLD_CMDL_ENGINE_IPSEC_HPS3         28     /* IPSEC HPS Pass3: future use */


#define SALLD_CMDL_ENGINE_ID_2AS1            10     /* 2AS Pass1: Second instance of AS Pass1 */
#define SALLD_CMDL_ENGINE_ID_2AS2            11     /* 2AS Pass2: Second instance of AS Pass2 */

#define SALLD_CMDL_ENGINE_ID_OUTPORT1        12     /* Engress Module 1: out of cp_ace */


#define SALLD_CMDL_ENGINE_ID_ACS1            14     /* ACS Pass1: Air Cipher Processing for Kasumi and Snow3G */
#define SALLD_CMDL_ENGINE_ID_ACS2            15     /* ACS Pass2: Second level Air Cipher Processing for GCM/CCM */

#define SALLD_CMDL_ENGINE_SRTP_AC_HPS1       16     /* SRTP/Air Cipher HPS Pass1: packet header parsing and inspection */
#define SALLD_CMDL_ENGINE_SRTP_AC_HPS2       17     /* SRTP/Air Cipher HPS Pass2: payload processing verification */
#define SALLD_CMDL_ENGINE_SRTP_AC_HPS3       30     /* SRTP/Air Cipher HPS Pass3: payload processing verification2 */


#define SALLD_CMDL_ENGINE_IPSEC_2HPS1        18     /* 2IPSEC HPS Pass1: Second instance of IPSEC HPS Pass1 */
#define SALLD_CMDL_ENGINE_IPSEC_2HPS2        19     /* 2IPSEC HPS Pass2: Second instance of IPSEC HPS Pass2 */
#define SALLD_CMDL_ENGINE_IPSEC_2HPS3        31     /* 2IPSEC HPS Pass3: Second instance of IPSEC HPS Pass3 */


#define SALLD_CMDL_ENGINE_ID_OUTPORT2        20     /* Engress Module 2: out of cp_ace */



#define SALLD_CMDL_ENGINE_NONE             0xFF     /* No Engine is specified */

 
/******************************************************************************
 * Command Label Option Control Definitions
 ******************************************************************************/
#define SALLD_CMDL_OPTION_CTRL_MASK_CTX_OFFSET     0xF8   /* Specify the offset from start
                                                           * of the engine specific security 
                                                           * context in units of 8 bytes 
                                                           */ 
#define SALLD_CMDL_OPTION_CTRL_SHIFT_CTX_OFFSET       3
#define SALLD_CMDL_OPTION_CTRL_MASK_LEN            0x07   /* Specify the length of the option in
                                                           * units of 8 bytes including padding
                                                           */ 
#define SALLD_CMDL_OPTION_CTRL_SHIFT_LEN              0
 
#define SALLD_CMDL_GET_OPTION_CTRL_CTX_OFFSET(ctrl) ((ctrl) & SALLD_CMDL_OPTION_CTRL_MASK_CTX_OFFSET))
#define SALLD_CMDL_GET_OPTION_CTRL_LEN(ctrl)        (((ctrl) & SALLD_CMDL_OPTION_CTRL_MASK_LEN) << 3)
#define SALLD_CMDL_MK_OPTION_CTRL(offset, len)      ((offset) | (len >> 3))
 
/******************************************************************************
 * Type:  SALLD_DATA_MODE_CMDL_MODE_T        
 ******************************************************************************
 * Description: Define the Command Label Processing modes for Data Mode
 *
 * Note: It can be used as index to the command label processing dispatch table
 *****************************************************************************/
#define SALLD_DATA_MODE_CMDL_MODE_GEN        0   /* No special processing is required */
#define SALLD_DATA_MODE_CMDL_MODE_GCM        1   /* Combined Mode */
#define SALLD_DATA_MODE_CMDL_MODE_GMAC       2   /* Combined Mode */
#define SALLD_DATA_MODE_CMDL_MODE_GMAC_AH    3   /* Combined Mode */
#define SALLD_DATA_MODE_CMDL_MODE_CCM        4   /* Combined Mode */
 
#endif /* _SALLDCMDL_H */

