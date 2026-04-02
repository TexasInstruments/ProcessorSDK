/*
 * System Firmware
 *
 * Deinfitions for various sysfw domgrps
 *
 * Copyright (C) 2020, Texas Instruments Incorporated
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

#ifndef SYSFW_DOMGRPS_H
#define SYSFW_DOMGRPS_H

#include <types/errno.h>
#include <types/short_types.h>

/**
 * \brief Defines the sysfw DOMGRP type. This is meant to be used in code
 * or data structures that require distinction of domgrps.
 */
typedef u8 domgrp_t;

/* External definitions */

/** SoC SYSFW domgrp for backward compatibility */
#define DOMGRP_COMPATIBILITY    (0x00U)

/** SoC defined SYSFW domgrp 00 */
#define DOMGRP_00               ((0x01U) << 0U)
/** SoC defined SYSFW domgrp 01 */
#define DOMGRP_01               ((0x01U) << 1U)
/** SoC defined SYSFW domgrp 02 */
#define DOMGRP_02               ((0x01U) << 2U)
/** SoC defined SYSFW domgrp 03 */
#define DOMGRP_03               ((0x01U) << 3U)
/** SoC defined SYSFW domgrp 04 */
#define DOMGRP_04               ((0x01U) << 4U)
/** SoC defined SYSFW domgrp 05 */
#define DOMGRP_05               ((0x01U) << 5U)
/** SoC defined SYSFW domgrp 06 */
#define DOMGRP_06               ((0x01U) << 6U)

/** SYSFW internal usage ONLY */
#define DOMGRP_07               ((0x01U) << 7U)
/** Match everything - STRICTLY INTERNAL USAGE ONLY */
#define DOMGRP_ALL              (0xFFU)

/**
 * Maximum number of domgrps that are supported by SYSFW.
 * Derived from the above definitions
 */
#define MAX_NUM_DOMGRPS (8U)

#endif /* SYSFW_DOMGRPS_H */
