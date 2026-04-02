/*
 * DM stub firmware
 *
 * Copyright (C) 2021-2024, Texas Instruments Incorporated
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

#include <string.h>
#include <types/short_types.h>
#include <stdbool.h>

extern unsigned char _start_bss;
extern unsigned char _end_bss;

extern s32 dm_stub_entry(void);
extern s32 dm_stub_resume(void);
extern void dm_stub_irq_handler(void);
extern void data_abt_handler(void);
extern void fiq_handler(void);
extern void irq_handler(void);
extern void prefetch_abt_handler(void);
extern void reserved_handler(void);
extern s32 stub_entry(void);
extern s32 stub_resume(void);
extern void swi_handler(void);
extern void undef_handler(void);

s32 stub_entry(void)
{
	return dm_stub_entry();
}

s32 stub_resume(void)
{
	return dm_stub_resume();
}

void undef_handler(void)
{
	while (true) {
	}
}

void swi_handler(void)
{
	while (true) {
	}
}

void prefetch_abt_handler(void)
{
	while (true) {
	}
}

void data_abt_handler(void)
{
	while (true) {
	}
}

void reserved_handler(void)
{
	while (true) {
	}
}

void irq_handler(void)
{
	dm_stub_irq_handler();
}

void fiq_handler(void)
{
	while (true) {
	}
}
