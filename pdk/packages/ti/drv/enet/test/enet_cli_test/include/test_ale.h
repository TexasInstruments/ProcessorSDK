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
 * \file  test_ale.h
 *
 * \brief This is the header file for test_ale.c
 */

#ifndef _TEST_ALE_H_
#define _TEST_ALE_H_

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
 * User passes a MAC address and this function adds 
 * a unicast entry corresponding to that MAC address 
 * in the ALE table. User may also pass flags -a, -d
 * or -b for ageable, default and blocking ALE
 * entries respectively.
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
BaseType_t EnetCLI_addUcast(char *writeBuffer,
                            size_t writeBufferLen,
                            const char *commandString);

/*!
 * \brief Removes a Unicast MAC address from ALE table.
 *
 * When user passes a MAC address that exists in the
 * ALE, this function removes the entry
 * corresponding to that MAC address from the ALE
 * table.
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
BaseType_t EnetCLI_removeUcast(char *writeBuffer,
                               size_t writeBufferLen,
                               const char *commandString);

/*!
 * \brief Adds a multicast MAC address to ALE table.
 *
 * User passes a MAC address and this function adds 
 * a multicast entry corresponding to that MAC address 
 * in the ALE table.
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
BaseType_t EnetCLI_addMcast(char *writeBuffer,
                            size_t writeBufferLen,
                            const char *commandString);

/*!
 * \brief Removes a Multicast MAC address from ALE table.
 *
 * When user passes a multicast MAC address that 
 * exists in the ALE, this function removes the entry
 * corresponding to that MAC address from the ALE
 * table.
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
BaseType_t EnetCLI_removeMcast(char *writeBuffer,
                               size_t writeBufferLen,
                               const char *commandString);

/*!
 * \brief Adds an OUI corresponding to a MAC address to 
 * ALE table.
 *
 * User passes a MAC address and this function adds 
 * an OUI entry corresponding to that MAC address in 
 * the ALE table. (Organisationally Unique Identifier 
 * is the first 3 bytes of MAC address).
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
BaseType_t EnetCLI_addOui(char *writebuffer,
                          size_t writeBufferLen,
                          const char *commandString);

/*!
 * \brief Removes an OUI corresponding to a 
 * MAC address from ALE table.
 *
 * When user passes a MAC address such that OUI
 * corresponding to that MAC exists in the ALE,
 * this function removes the entry corresponding 
 * to that MAC address from the ALE table.
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
BaseType_t EnetCLI_removeOui(char *writeBuffer,
                             size_t writeBufferLen,
                             const char *commandString);

/*!
 * \brief Adds an IPv4 address to ALE table.
 *
 * User passes an IPv4 address and this function adds 
 * an entry corresponding to this IPv4 address in 
 * the ALE table.
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
BaseType_t EnetCLI_addipv4(char *writeBuffer,
                           size_t writeBufferLen,
                           const char *commandString);

/*!
 * \brief Removes an IPv4 address from ALE table.
 *
 * When user passes an IPv4 address that exists in 
 * the ALE, this function removes the entry from 
 * the ALE table.
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
BaseType_t EnetCLI_removeipv4(char *writeBuffer,
                              size_t writeBufferLen,
                              const char *commandString);

/*!
 * \brief Changes state of Mac Ports.
 *
 * When user passes a port number and one of the states from among
 * Learn, Block, Disable or Forward, this API sets the port state
 * accordingly.
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
BaseType_t EnetCLI_changePortState(char *writeBuffer,
                                   size_t writeBufferLen,
                                   const char *commandString);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

#ifdef __cplusplus
}
#endif

#endif /* _TEST_ALE_H_ */
