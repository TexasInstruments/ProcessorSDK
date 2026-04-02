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

#include <boardcfg/boardcfg_rm_data.h>
#include <ir_track_config.h>
#include <rm_ir.h>
#include <rm_lpm.h>
#include <types/errno.h>

static struct ir_config_entry ir_config_data[NUM_IR_CONFIGS] = { 0U };

s32 lpm_sleep_save_ir_config(u16 id, u16 inp, u16 outp, sbool validate)
{
	u16 i;

	/* Insert config into the next empty spot */
	for (i = 0U; i < NUM_IR_CONFIGS; i++) {
		if ((ir_config_data[i].id == 0U) &&
		    (ir_config_data[i].inp == 0U) &&
		    (ir_config_data[i].outp == 0U) &&
		    (ir_config_data[i].validate == SFALSE)) {
			ir_config_data[i].id = id;
			ir_config_data[i].inp = inp;
			ir_config_data[i].outp = outp;
			ir_config_data[i].validate = validate;

			break;
		}
	}

	return SUCCESS;
}

s32 lpm_sleep_save_ir_unconfig(u16 id, u16 inp, u16 outp)
{
	u16 i;

	/* Remove the entry from the list */
	for (i = 0U; i < NUM_IR_CONFIGS; i++) {
		if ((ir_config_data[i].id == id) &&
		    (ir_config_data[i].inp == inp) &&
		    (ir_config_data[i].outp == outp)) {
			ir_config_data[i].id = 0U;
			ir_config_data[i].inp = 0U;
			ir_config_data[i].outp = 0U;
			ir_config_data[i].validate = SFALSE;

			break;
		}
	}

	/*
	 * Remove the empty slot from the middle of the list
	 * by swapping the empty slot with all of the filled slots
	 */
	for (i = 0U; i < (NUM_IR_CONFIGS - 1U); i++) {
		if ((ir_config_data[i].id == 0U) &&
		    (ir_config_data[i].inp == 0U) &&
		    (ir_config_data[i].outp == 0U) &&
		    (ir_config_data[i].validate == SFALSE)) {
			if ((ir_config_data[i + 1U].id == 0U) &&
			    (ir_config_data[i + 1U].inp == 0U) &&
			    (ir_config_data[i + 1U].outp == 0U) &&
			    (ir_config_data[i + 1U].validate == SFALSE)) {
				/* No more filled entries remaining to be swapped */
				break;
			} else {
				/* Next entry is filled, so swap it */

				ir_config_data[i].id = ir_config_data[i + 1U].id;
				ir_config_data[i].inp = ir_config_data[i + 1U].inp;
				ir_config_data[i].outp = ir_config_data[i + 1U].outp;
				ir_config_data[i].validate = ir_config_data[i + 1U].validate;

				ir_config_data[i + 1U].id = 0U;
				ir_config_data[i + 1U].inp = 0U;
				ir_config_data[i + 1U].outp = 0U;
				ir_config_data[i + 1U].validate = SFALSE;
			}
		}
	}

	return SUCCESS;
}

s32 lpm_resume_restore_ir_config(void)
{
	u16 i;

	for (i = 0U; i < NUM_IR_CONFIGS; i++) {
		if ((ir_config_data[i].id == 0U) &&
		    (ir_config_data[i].inp == 0U) &&
		    (ir_config_data[i].outp == 0U) &&
		    (ir_config_data[i].validate == SFALSE)) {
			/* No more entries remaining to be reconfigured */
			break;
		} else {
			u16 temp_id;
			u16 temp_inp;
			u16 temp_outp;
			sbool temp_validate;

			/* Store the parameters before they are erased */
			temp_id = ir_config_data[i].id;
			temp_inp = ir_config_data[i].inp;
			temp_outp = ir_config_data[i].outp;
			temp_validate = ir_config_data[i].validate;

			/*
			 * Remove the entry from the list so that way when it's reconfigured
			 * in the next step, it will be re-added to the list in its original
			 * spot, rather than the end of the list
			 */
			ir_config_data[i].id = 0U;
			ir_config_data[i].inp = 0U;
			ir_config_data[i].outp = 0U;
			ir_config_data[i].validate = SFALSE;

			/* Reconfigure the IR entry */
			rm_ir_configure(temp_id,
					temp_inp,
					temp_outp,
					temp_validate);
		}
	}

	return SUCCESS;
}
