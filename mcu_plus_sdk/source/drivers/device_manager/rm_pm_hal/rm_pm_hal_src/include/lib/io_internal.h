/*
 * System Firmware
 *
 * Cortex-M3 (CM3) firmware for power management
 *
 * Copyright (C) 2014-2020, Texas Instruments Incorporated
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

#ifndef IO_INTERNAL_H
#define IO_INTERNAL_H

/*
 * This file should only be included from either include/lib/io.h or
 * include/lib/ioremap.h
 */

#include <types/errno.h>
#include <types/short_types.h>
#include <types/address_types.h>

static inline local_phys_addr_t ioremap_direct(local_phys_addr_t a)
{
	return a;
}

u32 ioremap_internal(u32 a);

#if !defined(IOREMAP_H) && !defined(IO_H)
#error lib/io_internal.h should not be included directly.
#endif

#ifdef IOREMAP_H
#define ioremap ioremap_internal
#else
#define ioremap ioremap_direct
#endif

/**
 * \brief provide a simplified read u32 operation
 * \param a address to write to
 *
 * \return u32 value at the address pointed by a
 */
static inline u32 readl(local_phys_addr_t a)
{
	return *(volatile u32 *) (ioremap(a));
}

/**
 * \brief provide a simplified write u32 operation
 * \param v value
 * \param a address to write to
 */
static inline void writel(u32 v, local_phys_addr_t a)
{
	*(volatile u32 *) (ioremap(a)) = v;
}

/**
 * \brief provide a verified write u32 operation
 * \param v value
 * \param a address to write to
 *
 * \return SUCCESS if we readback the same value we wrote, else return
 * EFAILVERIFY
 */
static inline s32 writel_verified(u32 v, local_phys_addr_t a)
{
	u32 readback_val;
	s32 ret = SUCCESS;

	/* First write the value */
	writel(v, a);

	/* Read it back */
	readback_val = readl(a);

	if (readback_val != v) {
		ret = -EFAILVERIFY;
	}
	return ret;
}

/**
 * \brief provide a simplified read u16 operation
 * \param a address to write to
 *
 * \return u16 value at the address pointed by a
 */
static inline u16 readw(local_phys_addr_t a)
{
	return *(volatile u16 *) (ioremap(a));
}

/**
 * \brief provide a simplified write u16 operation
 * \param v value
 * \param a address to write to
 */
static inline void writew(u16 v, local_phys_addr_t a)
{
	*(volatile u16 *) (ioremap(a)) = v;
}

/**
 * \brief provide a verified write u16 operation
 * \param v value
 * \param a address to write to
 *
 * \return SUCCESS if we readback the same value we wrote, else return
 * EFAILVERIFY
 */
static inline s32 writew_verified(u16 v, local_phys_addr_t a)
{
	u16 readback_val;
	s32 ret = SUCCESS;

	/* First write the value */
	writew(v, a);

	/* Read it back */
	readback_val = readw(a);

	if (readback_val != v) {
		ret = -EFAILVERIFY;
	}
	return ret;
}

/**
 * \brief provide a simplified read u8 operation
 * \param a address to write to
 *
 * \return u8 value at the address pointed by a
 */
static inline u8 readb(local_phys_addr_t a)
{
	return *(volatile u8 *) (ioremap(a));
}

/**
 * \brief provide a simplified write u8 operation
 * \param v value
 * \param a address to write to
 */
static inline void writeb(u8 v, local_phys_addr_t a)
{
	*(volatile u8 *) (ioremap(a)) = v;
}

/**
 * \brief provide a verified write u8 operation
 * \param v value
 * \param a address to write to
 *
 * \return SUCCESS if we readback the same value we wrote, else return
 * EFAILVERIFY
 */
static inline s32 writeb_verified(u8 v, local_phys_addr_t a)
{
	u8 readback_val;
	s32 ret = SUCCESS;

	/* First write the value */
	writeb(v, a);

	/* Read it back */
	readback_val = readb(a);

	if (readback_val != v) {
		ret = -EFAILVERIFY;
	}
	return ret;
}

#endif /* IO_INTERNAL_H */
