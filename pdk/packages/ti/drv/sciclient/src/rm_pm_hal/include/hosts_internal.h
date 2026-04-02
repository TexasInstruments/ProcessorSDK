/*
 * System Firmware Source File
 *
 * Host IDs for internal use
 *
 * Copyright (C) 2017-2021, Texas Instruments Incorporated
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

/**
 * \file hosts_internal.h
 *
 * \brief Host IDs for internal use by system firmware
 */

#ifndef HOSTS_INTERNAL_H
#define HOSTS_INTERNAL_H

/* Host IDs for internal DMSC use */

/** DMSC RM: Device Management and Security Control - resource management
 *  subsystem */
#define HOST_ID_RM (240U)

/** None or a wildcard host ID, used for bookkeeping an item or resource on
 *  behalf of any processing entity - and not exclusive to one.
 */
#define HOST_ID_NONE (255U)

#ifdef HOST_ID_DMSC
/* Create name alias for HOST_ID_DMSC to enable legacy platform compatibility */
#define HOST_ID_TIFS HOST_ID_DMSC
#endif
#ifdef HOST_ID_DM2DMSC
/*
 * Create name alias for HOST_ID_DM2DMSC to enable legacy platform compatibility
 */
#define HOST_ID_DM2TIFS HOST_ID_DM2DMSC
#endif

#endif /* HOSTS_INTERNAL_H */
