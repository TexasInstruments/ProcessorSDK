/**
 *   Copyright (c) Texas Instruments Incorporated 2025
 *   All rights reserved.
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

/**
 *  \file ldra_vhwa_coverage_main.c
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */


/* This file is only required for remote cores for LDRA analysis */
#if defined(C7X_FAMILY) || defined(R5F) || defined(C66)

#include <examples/include/ldra_vhwa_coverage_main.h>

int globalLdraStarted = 0;

void ldra_start(void)
{
    globalLdraStarted++;
    static int count;

    Vhwa_print("LDRA_INFO: ldra_start \n");
    #if defined(LDRA_COVERAGE)
    ldra_port_init();
    #endif 

    Vhwa_print("LDRA_INFO: ldra_start called %d time(s)\n", count);
    count++;
}

void ldra_initialize(void)
{
    if (globalLdraStarted == 0){
        ldra_start();
        Vhwa_print("LDRA_INFO: ldra_start called !\n");
    } else {
        Vhwa_print("LDRA_ERROR: ldra_start is already called and couldn't be called again!\n");
    }
}

void ldra_stop(void)
{
    static int count;
    Vhwa_print("LDRA_INFO: ldra_stop called %d time(s)\n", count);

    if (globalLdraStarted > 0){
#if defined LDRA_DYN_COVERAGE_EXIT
#if defined (C7X_FAMILY)
      Vhwa_print("LDRA_INFO: Uploading execution history!!\n");
      upload_execution_history();
      Vhwa_print("LDRA_INFO: Uploading execution history: Done!!!\n");
#endif
#if defined (R5F)
      Vhwa_print("LDRA_INFO: Uploading execution history!!\n");
      for (int i = 0; i <= 5; i++) {
        Vhwa_print("LDRA_INFO: %d: Uploading .. \n", i);
      }
      upload_execution_history();
      Vhwa_print("LDRA_INFO: Uploading execution history: Done!!!\n");
      for (int i = 0; i <= 5; i++) {
        Vhwa_print("LDRA_INFO: %d: Uploaded !! \n", i);
      }
#endif
#else
    Vhwa_print("LDRA_ERROR: Source not instrumented or LDRA_DYN_COVERAGE_EXIT MACRO not defined");
#endif
        globalLdraStarted = 0;
    }
    count++;
}

void ldra_terminate(void)
{
    if (globalLdraStarted != 0){
        ldra_stop();
        Vhwa_print("LDRA_INFO: ldra_stop called !\n");
    } else {
        Vhwa_print("LDRA_ERROR: ldra_stop couldn't be called: %d\n", globalLdraStarted);
    }
}


#endif