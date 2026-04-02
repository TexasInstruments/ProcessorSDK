/*
 * System Firmware Utility function
 *
 * Utility function for MMR Lock and Unlock
 *
 * Copyright (C) 2020-2024, Texas Instruments Incorporated
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
#ifndef MMR_LOCK_H
#define MMR_LOCK_H

/** \brief Function to unlock a Control module MMR partition.
 *
 *  \param base base address of the region to be unlocked
 *  \param partition which partition to unlock
 */
void mmr_unlock(u32 base, u32 partition);

/** \brief Function to lock a Control module MMR partition.
 *
 *  \param base base address of the region to be locked
 *  \param partition which partition to lock
 */
void mmr_lock(u32 base, u32 partition);

/** \brief Function to read a Control module MMR partition.
 *
 *  \param base base address of the region to be read
 *  \param partition which partition to read
 *  \param value MMR partition macro value
 */
void mmr_read(u32 base, u32 partition, u32 value);

/** \brief Function to unlock the Control module MMRs needed for PM to
 *         function.
 */
void mmr_unlock_all(void);

/** \brief Function to lock the Control module MMRs needed for PM to
 *         ensure no other software entity can play around with these
 *         registers.
 */
void mmr_lock_all(void);

#endif /* MMR_LOCK_H */
