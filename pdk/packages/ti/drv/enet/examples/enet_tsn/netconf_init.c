/*
 *  Copyright (c) Texas Instruments Incorporated 2024
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

/* ========================================================================== */
/*                              Include Files                                 */
/* ========================================================================== */

#include <tsn_unibase/unibase.h>
#include <tsn_combase/combase.h>
#include <tsn_unibase/unibase_binding.h>
#include <tsn_uniconf/yangs/yang_modules.h>
#include <tsn_uniconf/ucman.h>
#include <tsn_uniconf/uc_dbal.h>
#include <nconf_server.h>

#include "debug_log.h"
#include "tsninit.h"
#include "common.h"

#if (NCONF_ENABLE_TCP_PLAINTEXT == 0x00U)
#include "pki/sr/server_cert.h"
#include "pki/sr/server_key.h"
#include "pki/ca/ca_cert.h"
#endif /* (NCONF_ENABLE_TCP_PLAINTEXT == 0x00U) */

/*============================================================================*/
/*                          Macros and Constants                              */
/*============================================================================*/

#define NETCONF_TASK_PRIORITY   (2)
#define NETCONF_TASK_NAME       "netconf_task"

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static void *EnetApp_netconfTask(void *arg);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

static uint8_t gNetconfStackBuf[TSN_TSK_STACK_SIZE] __attribute__ ((aligned(TSN_TSK_STACK_ALIGN)));
volatile int32_t app_stop_flag=0;
extern EnetApp_ModuleCtx_t gModCtxTable[ENETAPP_MAX_TASK_IDX];
extern EnetApp_Ctx_t gAppCtx;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

int EnetApp_addNetconfModCtx(EnetApp_ModuleCtx_t *modCtxTbl)
{
    EnetApp_ModuleCtx_t netconfModCtx = {
        .enable = BTRUE,
        .stopFlag = BTRUE,
        .taskPriority = NETCONF_TASK_PRIORITY,
        .taskName = NETCONF_TASK_NAME,
        .stackBuffer = gNetconfStackBuf,
        .stackSize = sizeof(gNetconfStackBuf),
        .onModuleDBInit = NULL,
        .onModuleRunner = EnetApp_netconfTask,
        .appCtx = &gAppCtx
    };
    memcpy(&modCtxTbl[ENETAPP_NETCONF_TASK_IDX], &netconfModCtx,
           sizeof(EnetApp_ModuleCtx_t));
    return 0;
}

static void *EnetApp_netconfTask(void *arg)
{
    int res = 0;
    EnetApp_ModuleCtx_t *mdctx = (EnetApp_ModuleCtx_t *)arg;
    EnetApp_Ctx_t * ctx = mdctx->appCtx;
    char *dbname=(ctx->dbName) ? ctx->dbName : "NULL";
    nconfopt_t optd={
#if (NCONF_ENABLE_TCP_PLAINTEXT == 0x00U)
        .sopt={
            .tls={
                .ca=ca_crt,
                .ca_len=ca_crt_len,
                .privkey=server_key,
                .privkey_len=server_key_len,
                .privkey_pass=NULL,
                .privkey_pass_len=0,
                .cert=server_crt,
                .cert_len=server_crt_len,
            },
        },
#endif /* (NCONF_ENABLE_TCP_PLAINTEXT == 0x00U) */
        .ucclopt={
            .dbname=ctx->dbName,
            .schemadir=NETCONF_YANG_SCHEMA_DIR,
            .ucthread=BTRUE,
        },
    };
    nconf_uccl_t dbhdl=NULL;

    DPRINT("[NCONF] TASK Started: %s", dbname);
    DPRINT("INF:nconf:%s", NCONF_BUILD_VERSION);
    res=nconf_ucclient_init(&optd.ucclopt, &dbhdl);
    if (0 == res) {
        res=nconf_msghdlr_init(dbhdl);
    }
    if (0 == res) {
        res=nconf_transport_init(&optd.sopt, dbhdl);
    }
    if (0 == res) {
        nconf_msghdlr_main_loop(NULL); // This is blocking,
                                       // call nconf_msghdlr_stop() to stop
        DPRINT("[NCONF] netconf main loop ended!!!");
    }
    DPRINT("[NCONF] cleaning-up resources ...");
    nconf_msghdlr_deinit();
    nconf_ucclient_cleanup(dbhdl);
    DPRINT("[NCONF] TASK Ended: %s", dbname);
    while(BTRUE) { EnetApp_sleep(10000); EnetApp_yield(); }
    return NULL;
}

