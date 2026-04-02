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

/**
 *  \file crc_test.h
 *
 *  \brief This file contains common CRC Test Functions.
 */

#ifndef CRC_TEST_H_
#define CRC_TEST_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#ifdef __cplusplus
extern "C" {
#endif

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

/**
 * \brief The function is used to test the CRC Driver Initialization and Cleanup
 *        API
 *
 *  \retval status of the test
 *              Success  -   0
 *              Error    -   <0
 */
int32_t Test_crcInitAPI(void);

/**
 * \brief The function is used to test the Compute Signature API
 *
 *  \retval status of the test
 *              Success  -   0
 *              Error    -   <0
 */
int32_t Test_crcComputeSignatureAPI(void);

/**
 * \brief The function is used to test the Get Signature API
 *
 *  \retval status of the test
 *              Success  -   0
 *              Error    -   <0
 */
int32_t Test_crcGetSignatureAPI(void);

/**
 * \brief The function is used to test the Cancel Signature API
 *
 *  \retval status of the test
 *              Success  -   0
 *              Error    -   <0
 */
int32_t Test_crcCancelSignatureAPI(void);

/**
 * \brief The function is used to test the basic CRC Driver API
 *
 *  \retval status of the test
 *              Success  -   0
 *              Error    -   <0
 */
int32_t Test_crcAPI(CRC_Type type, CRC_DataLen dataLen);

/**
 * \brief The function is used to test the basic CRC Driver API
 */
void Test_crcAppPrint0(const char   *infoString);

/**
 * \brief The function is used to test the basic CRC Driver API
 *
 * \param param0 The first parameter which needs to be printed.
 */
void Test_crcAppPrint1(const char   *infoString,
                  uintptr_t     param0);

/**
 * \brief The function is used to test the basic CRC Driver API
 *
 * \param param0 The first parameter which needs to be printed.
 * \param param1 The second parameter which needs to be printed.
 */
void Test_crcAppPrint2(const char   *infoString,
                  uintptr_t     param0,
                  uintptr_t     param1);

/**
 * \brief The function is used to test the basic CRC Driver API
 *
 * \param param0 The first parameter which needs to be printed.
 * \param param1 The second parameter which needs to be printed.
 * \param param2 The third parameter which needs to be printed.
 */
void Test_crcAppPrint3(const char   *infoString,
                       uintptr_t     param0,
                       uintptr_t     param1,
                       uintptr_t     param2);

/* ========================================================================== */
/*                       Static Function Definitions                          */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef CRC_TEST_H_ */
