/*
 *
 * Copyright (c) 2018 Texas Instruments Incorporated
 *
 * All rights reserved not granted herein.
 *
 * Limited License.
 *
 * Texas Instruments Incorporated grants a world-wide, royalty-free, non-exclusive
 * license under copyrights and patents it now or hereafter owns or controls to make,
 * have made, use, import, offer to sell and sell ("Utilize") this software subject to the
 * terms herein.  With respect to the foregoing patent license, such license is granted
 * solely to the extent that any such patent is necessary to Utilize the software alone.
 * The patent license shall not apply to any combinations which include this software,
 * other than combinations with devices manufactured by or for TI ("TI Devices").
 * No hardware patent is licensed hereunder.
 *
 * Redistributions must preserve existing copyright notices and reproduce this license
 * (including the above copyright notice and the disclaimer and (if applicable) source
 * code license limitations below) in the documentation and/or other materials provided
 * with the distribution
 *
 * Redistribution and use in binary form, without modification, are permitted provided
 * that the following conditions are met:
 *
 * *       No reverse engineering, decompilation, or disassembly of this software is
 * permitted with respect to any software provided in binary form.
 *
 * *       any redistribution and use are licensed by TI for use only with TI Devices.
 *
 * *       Nothing shall obligate TI to provide you with source code for the software
 * licensed and provided to you in object code.
 *
 * If software source code is provided to you, modification and redistribution of the
 * source code are permitted provided that the following conditions are met:
 *
 * *       any redistribution and use of the source code, including any resulting derivative
 * works, are licensed by TI for use only with TI Devices.
 *
 * *       any redistribution and use of any object code compiled from the source code
 * and any resulting derivative works, are licensed by TI for use only with TI Devices.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of its suppliers
 *
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * DISCLAIMER.
 *
 * THIS SOFTWARE IS PROVIDED BY TI AND TI'S LICENSORS "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL TI AND TI'S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#pragma once
#ifndef PTK_CORE_H
#define PTK_CORE_H

/**
 * \defgroup group_ptk_core PTK Core APIs
 * \ingroup group_ptk_base
 *
 * The core component provides the basic facilities that are required for all
 * other components to work. It primarily addresses three things:
 * 1. C runtime/host system interface (printf, exit)
 * 2. Visualization framework, where appropriate
 * 3. Testing API for creating unit tests
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct
{
    /** Exit function used if the program must be terminated
     *  (i.e. in an assertion failure).
     */
	void (*exit)(int);

    /** Write the given formatted string to a useful output location
     *  (on screen, log le, over network, etc.).
     */
	int (*printf)(const char *fmt, ...);

    /** Get the current time in microseconds as a monotonically increasing
     *  realtime clock.
     */
	uint64_t (*time)(void);

} PTK_CRT;

/**
 * \brief This function initializes the library with the given list of function
 *        pointers to functions that provide the C runtime library functions.
 *        The most basic requirement is that exit be available, but the
 *        remaining members are optional.
 *
 * \param [in] crt Contains a list of function pointers for available C runtime
 *                 behavior
 *
 * \ingroup group_ptk_core
 */
void PTK_init(PTK_CRT *crt);

/**
 * \brief This performs an immediate exit of the entire task, which typically
 *        will also kill all other threads running. It should only be used when
 *        an unrecoverable error has been detected. The status code may or may
 *        not be useful on all systems, but it is required per the C standard
 *        library specification.
 *
 * \param [in] status Status code indicating reason for exit
 *
 * \ingroup group_ptk_core
 */
void PTK_exit(int status);

/**
 * \brief If printf was configured, this formats the given string and prints it
 *        appropriately. Otherwise, it does nothing.
 *
 * \param [in] fmt Format string for printing
 *
 * \param [in] ... Variable arguments for values to be printed in the format
 *                 string
 *
 * \ingroup group_ptk_core
 */
void PTK_printf(const char *fmt, ...);

/**
 * \brief If available, this obtains the real time in microseconds since a some
 *        reference point in the past. This reference point may not change
 *        during a single program's execution, but it may be reset, e.g. to 0
 *        or another constant, whenever the program is restarted.
 *
 * \return Time in microseconds
 *
 * \ingroup group_ptk_core
 */
uint64_t PTK_getTime();

#define PTK_assert(x) do { \
	if (!(x)) { \
		PTK_printf("Assertion `" #x "` failed on line %d in %s\n", __LINE__, __FILE__); \
		PTK_exit(1); \
	} } while(0)

#ifdef __cplusplus
}
#endif

#endif
