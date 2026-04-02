/*
 * LPM Interrupt Router Tracker
 *
 * LPM management interrupt router tracking infrastructure
 *
 * Copyright (C) 2024, Texas Instruments Incorporated
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

#ifndef IR_TRK_CFG_H
#define IR_TRK_CFG_H

#include <config.h>
#include <types/short_types.h>

#ifdef CONFIG_LPM_LIMIT_IR_TRACKING
#define NUM_IR_CONFIGS                  512U
#else
#define NUM_IR_CONFIGS                  65535U
#endif

/**
 * \brief Struct for containing all of the IR configuration parameters
 *
 * \param id
 * IR SoC device ID.
 *
 * \param inp
 * Input line to specified IR
 *
 * \param outp
 * Output line from specified IR
 *
 * \param validate
 * Validate the inp and outp prior to programming mux if STRUE, do not validate
 * if SFALSE.
 */
struct ir_config_entry {
	u16	id;
	u16	inp;
	u16	outp;
	sbool	validate;
} __attribute__((__packed__));

#endif /* IR_TRK_CFG_H */
