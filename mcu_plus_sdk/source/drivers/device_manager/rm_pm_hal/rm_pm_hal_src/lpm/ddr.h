/*
 * DM Stub Firmware
 *
 * LPM DDR driver
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

#ifndef __LPM_DDR_H__
#define __LPM_DDR_H__

/**
 * \brief Enter DDR low power mode
 */
s32 ddr_enter_low_power_mode(void);

/**
 * \brief Exit DDR low power mode
 */
s32 ddr_exit_low_power_mode(void);

/**
 * \brief Enter DDR io only plus ddr mode
 */
s32 ddr_enter_io_ddr_mode(void);

/**
 * \brief Run software triggered trainings after self refresh exit
 *        DDR4 trainings: 1. Read gate level training
 *                        2. Read level training
 *                        3. Write level training
 *
 * \note Current implementation handles single rank training.
 */
s32 ddr_deepsleep_exit_training(void);

/**
 * \brief Enable DDR data retention
 */
void ddr_enable_retention(void);

/**
 * \brief Disable DDR data retention
 */
void ddr_disable_retention(void);
#endif
