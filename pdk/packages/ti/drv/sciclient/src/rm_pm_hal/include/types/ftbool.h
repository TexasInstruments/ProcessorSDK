/*
 * Copyright (C) 2017-2020, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 * its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * \file ftbool.h
 *
 * \brief Fault tolerant boolean types and checks
 *
 */
#ifndef FTBOOL_H
#define FTBOOL_H

#include <types/short_types.h>
#include <types/sbool.h>

/**
 * Fault tolerant boolean type
 */
typedef u8 ftbool;

/*
 * function prototypes
 */
static inline sbool ft_is_true(ftbool val);
static inline sbool ft_is_false(ftbool val);
static inline sbool ft_s_is_true(u32 val);

/**
 * Fault tolerant "STRUE" value
 */
#define FT_TRUE         0x5AU
/**
 * Fault tolerant "SFALSE" value
 */
#define FT_FALSE        0xA5U

/**
 * Fault tolerant check for "STRUE"
 */
static inline sbool ft_is_true(ftbool val)
{
	return (val) == FT_TRUE;
}

/**
 * Fault tolerant check for "SFALSE"
 */
static inline sbool ft_is_false(ftbool val)
{
	return (val) == FT_FALSE;
}

/**
 * Short fault tolerant "STRUE" value (4-bits) used in H/W registers
 */
#define FT_S_TRUE       0xAU

/**
 * Short fault tolerant "SFALSE" value
 */
#define FT_S_FALSE        0x5U

/**
 * Fault tolerant check for "STRUE" (short value)
 */
static inline sbool ft_s_is_true(u32 val)
{
	return (val) == FT_S_TRUE;
}

#endif /* FTBOOL */
