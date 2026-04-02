/*
 * DMSC RM IRQ
 *
 * Resource Manager IRQ infrastructure APIs
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

#ifndef RM_IRQ_H
#define RM_IRQ_H

#include <types/short_types.h>
#include <types/map.h>
#include <soc_devgrps.h>

typedef s32 (*rm_irq_oes_get_func_t)(u8 host, u16 id, u16 irq, u16 *evt);
typedef s32 (*rm_irq_oes_set_func_t)(u8 host, u16 id, u16 irq, u16 evt);

/**
 * \brief Contains handlers needed to get/set device OES registers.
 *
 * \param oes_get Pointer to the OES register get function.  The function is
 *                passed the host making the request, the SoC Device ID, and the
 *                IRQ output from the device.  It returns the global event
 *                programmed into the device's output IRQ OES register.
 *
 * \param oes_set Pointer to the OES register set function.  The function is
 *                passed the host making the request, the SoC Device ID, the
 *                IRQ output from the device, and global event value to program
 *                into the device's output IRQ OES register.
 *
 * \param node Struct used in map implementation
 */
struct rm_irq_oes_src {
	rm_irq_oes_get_func_t	oes_get;
	rm_irq_oes_set_func_t	oes_set;
	struct map_node		node;
};

/**
 * \brief Confirms managed resource assignment types for IRQ management
 *
 * Returns whether or not the provided board cfg resource assignment unique
 * type is managed by drivers associated with IRQ management (IRQ driver,
 * IA driver, etc.).
 *
 * \param type Resource assignment unique type
 *
 * \return TRUE if resource type is managed by IRQ management, else FALSE
 */
sbool rm_irq_is_managed_resasg_utype(u16 utype);

/**
 * \brief Register a rm_irq_oes_src to handle OES register get/set for the SoC
 *        device mapped to id
 *
 * This function stores the rm_irq_oes_src in a map that is keyed on SoC device
 * id.
 *
 * \param id SoC device ID to be handled by the OES register handler that is
 *           provided.
 *
 * \param src Pointer to a static struct containing configuration for this
 *            SoC device.
 *
 * \return SUCCESS or error code otherwise.
 */
s32 rm_irq_oes_src_register(u16 id, struct rm_irq_oes_src *src);

/**
 * \brief RM IRQ init function
 *
 * Registers IRQ message handlers with the TISCI core infrastructure
 *
 * \return
 * Status of initialization
 *      SUCCESS - Initialization completed successfully
 *      -E... - Initialization completed unsuccessfully.
 */
s32 rm_irq_init(void);

/**
 * \brief RM IRQ deinit function
 *
 * Deinitializes IA and IR drivers in a device group
 *
 * \param devgrp Device Group that has to be uninitialized
 * \param rom_deinit Boolean value to indicate whether to reset
 *                          ROM usage flags or not
 *
 * \return
 * Status of initialization
 *      SUCCESS - Deinitialization completed successfully
 *      -E... - Deinitialization completed unsuccessfully.
 */
s32 rm_irq_deinit(devgrp_t devgrp, sbool rom_deinit);

#endif /* RM_IRQ_H */
