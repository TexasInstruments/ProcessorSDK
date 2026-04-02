/*
 * IRQ Interrupt Router
 *
 * IRQ management interrupt router infrastructure
 *
 * Copyright (C) 2017-2025, Texas Instruments Incorporated
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

#ifndef RM_IR_H
#define RM_IR_H

#include <types/sbool.h>
#include <types/short_types.h>

/**
 * \brief Confirms managed resource assignment types for IR driver
 *
 * Returns whether or not the provided board cfg resource assignment unique
 * type is managed by IR driver.
 *
 * \param type Resource assignment unique type
 *
 * \return TRUE if resource type is managed by IR driver, else FALSE
 */
sbool rm_ir_is_managed_resasg_utype(u16 utype);

/**
 * \brief Returns output line to which input line is mapped
 *
 * Returns the output line to which the specified input line has been mapped.
 * An error is returned if the inp line does not map to an output line.
 *
 * \param id
 * IR SoC device ID.
 *
 * \param inp
 * Input line to specified IR
 *
 * \param outp
 * Pointer to returned output line from specified IR
 *
 * \return
 *      SUCCESS - Retrieval of output successful
 *      < 0 - Error occurred while attempting to retrieve output line
 */
s32 rm_ir_get_outp(u16 id, u16 inp, u16 *outp);

/**
 * \brief Validates an IR output against the RM board configuration
 *        resource assignments
 *
 * \param host Host requesting configuration of IR output
 *
 * \param id IR SoC device ID
 *
 * \param outp Output line from specified IR
 *
 * \return SUCCESS if output is valid else -EINVAL
 */
s32 rm_ir_validate_output(u8 host, u16 id, u16 outp);

/**
 * \brief Configures a route through an IR
 *
 * Configures the input line to output line route for the specified IR
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
 *
 * \return
 *      SUCCESS - Input to output route successfully configured
 *      -E... - Error occurred while attempting to configure route
 */
s32 rm_ir_configure(u16 id, u16 inp, u16 outp, sbool validate);

/**
 * \brief Clears a route through an IR
 *
 * Clears the input line to output line route for the specified IR
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
 * \return
 *      SUCCESS - Input to output route cleared successfully
 *      -E... - Error occurred while attempting to clear route
 */
s32 rm_ir_clear(u16 id, u16 inp, u16 outp);

/**
 * \brief Initializes the IR driver
 *
 * Initializes the IR driver data structures
 *
 * \return
 *      SUCCESS - Initialization completed successfully
 *      -E... - Error occurred during initialization
 */
s32 rm_ir_init(void);

/**
 * \brief Deinitializes the IR driver for a device group
 *
 * Set the IR driver's initialized flag to false
 *
 * \param devgrp Device Group that has to be uninitialized
 * \param rom_deinit Boolean value to indicate whether to reset
 *                          ROM usage flags or not
 *
 * \return
 *      SUCCESS - Deinitialized the chosen device group successfully
 *      -E... - Failed to deinitialize the chosen device group
 */
s32 rm_ir_deinit(devgrp_t devgrp, sbool rom_deinit);

#endif /* RM_IR_H */
