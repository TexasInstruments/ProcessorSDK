/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
/**
 *  Copyright (c) Texas Instruments Incorporated 2023
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef LWIP_LWIPOPTS_H
#define LWIP_LWIPOPTS_H

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef LWIP_OPTTEST_FILE
#include "lwipopts_test.h"
#else /* LWIP_OPTTEST_FILE */

#include "lwipopts_common.h"
#include "lwipopts_os.h"

/* Overwriting the existing macros from lwipopts_common.h */

/* ---------- Memory options ---------- */
/* MEMP_NUM_TCP_SEG: the number of simultaneously queued TCP segments
 * Num of TCP segments you can allocate if using internal memory pools
 * MEMP_NUM_TCP_SEG should at least be >= TCP_SND_QUEUELEN */
#if defined(MEMP_NUM_TCP_SEG)
#undef MEMP_NUM_TCP_SEG
#define MEMP_NUM_TCP_SEG                (128)
#endif

/* ---------- pbuf options ---------- */
#if defined(PBUF_POOL_SIZE)
#undef PBUF_POOL_SIZE
#define PBUF_POOL_SIZE                  (320U)
#endif

/* ---------- TCP options ---------- */
/* TCP sender buffer space (bytes) */
#if defined(TCP_SND_BUF)
#undef TCP_SND_BUF
#define TCP_SND_BUF                     (16 * TCP_MSS)
#endif

/* TCP sender buffer space (pbufs). This must be at least = 2 * TCP_SND_BUF/TCP_MSS
 * for things to work. It limits the number of pbufs in send buffer */
#if defined(TCP_SND_QUEUELEN)
#undef TCP_SND_QUEUELEN
#define TCP_SND_QUEUELEN                (8 * TCP_SND_BUF/TCP_MSS)
#endif

#endif /* LWIP_OPTTEST_FILE */

#ifdef __cplusplus
}
#endif

#endif /* LWIP_LWIPOPTS_H */
