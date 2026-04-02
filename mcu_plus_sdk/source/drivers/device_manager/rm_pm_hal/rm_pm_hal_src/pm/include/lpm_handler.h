/*
 * Device Manager - LPM interface
 *
 * Handlers for Low Power Mode implementation
 *
 * Copyright (C) 2021-2025, Texas Instruments Incorporated
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

#ifndef LPM_H
#define LPM_H

#include <types/short_types.h>

/**
 *  \brief  LPM enter sleep handler Function.
 *
 *  \param  msg_recv Pointer to the received and returned message. Same buffer
 *                   used.
 *  \return ret      SUCCESS if the API executed successfully.
 *                   EFAIL   if the API failed to execute.
 */
s32 dm_enter_sleep_handler(u32 *msg_recv);

/**
 *  \brief  LPM prepare sleep handler Function.
 *
 *  \param  msg_recv Pointer to the received and returned message. Same buffer
 *                   used.
 *  \return ret      SUCCESS if the API executed successfully.
 *                   EFAIL   if the API failed to execute.
 */
s32 dm_prepare_sleep_handler(u32 *msg_recv);

/**
 *  \brief  LPM wake reason handler Function.
 *
 *  \param  msg_recv Pointer to the received and returned message. Same buffer
 *                   used.
 *  \return ret      SUCCESS if the API executed successfully.
 *                   EFAIL   if the API failed to execute.
 */
s32 dm_lpm_wake_reason_handler(u32 *msg_recv);

/**
 *  \brief  Enable / Disable io isolation handler Function.
 *
 *  \param  msg_recv Pointer to the received and returned message. Same buffer
 *                   used.
 *  \return ret      SUCCESS if the API executed successfully.
 *                   EFAIL   if the API failed to execute.
 */
s32 dm_set_io_isolation_handler(u32 *msg_recv);

/**
 *  \brief  LPM set device constraint handler Function.
 *
 *  \param  msg_recv Pointer to the received and returned message. Same buffer
 *                   used.
 *  \return ret      SUCCESS if the API executed successfully.
 *                   EFAIL   if the API failed to execute.
 */
s32 dm_lpm_set_device_constraint(u32 *msg_recv);

/**
 *  \brief  LPM get device constraint handler Function.
 *
 *  \param  msg_recv Pointer to the received and returned message. Same buffer
 *                   used.
 *  \return ret      SUCCESS if the API executed successfully.
 *                   EFAIL   if the API failed to execute.
 */
s32 dm_lpm_get_device_constraint(u32 *msg_recv);

/**
 *  \brief  LPM set latency constraint handler Function.
 *
 *  \param  msg_recv Pointer to the received and returned message. Same buffer
 *                   used.
 *  \return ret      SUCCESS if the API executed successfully.
 *                   EFAIL   if the API failed to execute.
 */
s32 dm_lpm_set_latency_constraint(u32 *msg_recv);

/**
 *  \brief  LPM get latency constraint handler Function.
 *
 *  \param  msg_recv Pointer to the received and returned message. Same buffer
 *                   used.
 *  \return ret      SUCCESS if the API executed successfully.
 *                   EFAIL   if the API failed to execute.
 */
s32 dm_lpm_get_latency_constraint(u32 *msg_recv);

/**
 *  \brief  LPM get next system mode handler Function.
 *
 *  \param  msg_recv Pointer to the received and returned message. Same buffer
 *                   used.
 *  \return ret      SUCCESS
 */
s32 dm_lpm_get_next_sys_mode(u32 *msg_recv);

/**
 *  \brief  LPM get next host state handler Function.
 *
 *  \param  msg_recv Pointer to the received and returned message. Same buffer
 *                   used.
 *  \return ret      SUCCESS
 */
s32 dm_lpm_get_next_host_state(u32 *msg_recv);

/**
 *  \brief  LPM abort handler Function.
 *
 *  \param  msg_recv Pointer to the received and returned message. Same buffer
 *                   used.
 *  \return ret      SUCCESS
 */
s32 dm_lpm_abort(u32 *msg_recv);

#endif
