/*
 * DM Stub Firmware
 *
 * VIM Raw driver for direct interrupt manipulation
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

#include <types/short_types.h>
#include <types/sbool.h>

#define INTR_ENABLE           0
#define INTR_DISABLE          1

/**
 * \brief Enable/disable an interrupt
 * \param intr The number of the interrupt to enable/disable
 * \param enable Flag to specify enable or disable
 */
void vim_set_intr_enable(u32 intr, int enable);

/**
 * \brief Clear an interrupt status
 * \param intr The number of the interrupt to clear
 */
void vim_clear_intr(u32 intr);

/**
 * \brief Get the active interrupt number
 *
 * \return s32 Value for the number of the active interrupt,
 *         or -EINVAL when no active interrupt
 */
s32 vim_get_intr_number(void);

/**
 * \brief Complete an interrupt service
 *        shall be called at the end of isr
 */
void vim_irq_complete(void);

/**
 * \brief Get interrupt status
 * \param intr The number of the interrupt
 *
 * \return 0 If the interrupt is not pending, if pending
 *         return a non zero value
 */
u32 vim_get_intr_status(u32 intr);
