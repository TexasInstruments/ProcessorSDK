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
 * \file types/fterr.h
 *
 * \brief Fault tolerant error codes, types and checks
 *
 */
#ifndef FTERR_H
#define FTERR_H

#include <types/short_types.h>
#include <types/errno.h>
#include <types/sbool.h>

/**
 * Fault tolerant non-boolean return type
 */
typedef s32 fterr;

/*
 * Function prototypes
 */
static inline sbool ft_is_ok(fterr val);
static inline fterr fterr_from_common(s32 e);
static inline s32 fterr_to_common(fterr e);

/**
 * No error (Not using zero for success for fault tolerance). Must be returned
 * as a positive value. (Not as -EFTOK)
 */
#define EFTOK           ((fterr) 0x5A)

/**
 * Translate from common error to fterr, only the success value is different
 * Other error codes are common to allow for easy translation. Does not impact
 * fault tolerance.
 */
static inline fterr fterr_from_common(s32 e)
{
	return (fterr) (((e) == 0) ? EFTOK : (e));
}

/**
 * Translate to common error from fterr, only the success value is different
 * Other error codes are common to allow for easy translation. Does not impact
 * fault tolerance.
 */
static inline s32 fterr_to_common(fterr e)
{
	return (s32) (((e) == EFTOK) ? 0 : (e));
}


/**
 * Check of EFTOK
 */
static inline sbool ft_is_ok(fterr val)
{
	return (val) == EFTOK;
}


#endif /* FTERR_H */
