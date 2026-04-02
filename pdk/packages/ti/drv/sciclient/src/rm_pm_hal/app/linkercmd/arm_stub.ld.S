/*
 * DMSC firmware
 *
 * Cortex-M3 (CM3) firmware for security
 *
 * Copyright (C) 2017-2019 Texas Instruments Incorporated - http://www.ti.com/
 *
 * This software is licensed under the standard terms and conditions in the
 * Texas Instruments Incorporated Technology and Software Publicly
 * Available Software License Agreement, a copy of which is included in
 * the software download.
 */

#include <config.h>

--args 0x0
-heap  0x0
-stack 0x800

stack_size = 0x400;

MEMORY
{
    NSPS:   org = CONFIG_LNK_NSPS_BASE len = CONFIG_LNK_NSPS_LEN
}

-e _stub_start

SECTIONS
{
        .text: load = CONFIG_LNK_NSPS_BASE
        {
            _privileged_code_begin = .;
            _text_secure_start = .;
            secure.o(.text:entry.o*)
            *(.text:*)
            *(.const:*)
            _text_secure_end = .;
            _privileged_code_end = .;

	}

        .data:
        {
            _privileged_data_begin = .;
            *(.data:*)
            _privileged_data_end = .;
        } > NSPS

        .bss
        {
            _start_bss = .;
            *(.bss:*)
            _end_bss = .;
        } > NSPS

	.stack
	{
		. = ALIGN(0x8);
		_start_stack = .;
		. += stack_size;
		_end_stack = .;
	} > NSPS
}
