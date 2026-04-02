/*
 * Copyright (C) 2024 Texas Instruments Incorporated
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the
 *   distribution.
 *
 *   Neither the name of Texas Instruments Incorporated nor the names of
 *   its contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*!
 * \file  test_phy.h
 *
 * \brief This is the header file for test_phy.c
 */

#ifndef _TEST_PHY_H_
#define _TEST_PHY_H_

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
extern "C"
{
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structures and Enums                               */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/*!
 * \brief Adds a Unicast MAC address to ALE table.
 *
 * When user passes a MAC Port, this function prints all
 * the register addresses according to Clause 22 format.
 *
 * \param writeBuffer    [OUT] The buffer where messages(strings) may be 
 *                             pushed by APIs. May be used with the snprintf
 *                             inbuilt function in C. FreeRTOS Plus CLI will
 *                             internally handle the printing of buffer
 *                             contents when the control returns.
 * \param writeBufferLen [IN]  Length of the writeBuffer defined above.
 * \param commandString  [IN]  Entire command that user passed as a string.
 *
 * \return pdFALSE - False if no more inputs are expected from user, 
 * \return pdTRUE  - True if more inputs are expected from the user.
 */
BaseType_t Enetcli_printPhyRegs (char *writeBuffer, 
                                 size_t writeBufferLen,
                                 const char *commandString);

/*!
 * \brief Adds a Unicast MAC address to ALE table.
 *
 * The first function which is called whenever the user enters 
 * enet_phymode -p <port number> -d <duplexity> -s <speed> [-r].
 *
 * \param writeBuffer    [OUT] The buffer where messages(strings) may be 
 *                             pushed by APIs. May be used with the snprintf
 *                             inbuilt function in C. FreeRTOS Plus CLI will
 *                             internally handle the printing of buffer
 *                             contents when the control returns.
 * \param writeBufferLen [IN]  Length of the writeBuffer defined above.
 * \param commandString  [IN]  Entire command that user passed as a string.
 *
 * \return pdFALSE - False if no more inputs are expected from user, 
 * \return pdTRUE  - True if more inputs are expected from the user.
 */
BaseType_t Enetcli_phyMode (char *writeBuffer, 
                            size_t writeBufferLen,
                            const char *commandString);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif /* _TEST_PHY_H_ */