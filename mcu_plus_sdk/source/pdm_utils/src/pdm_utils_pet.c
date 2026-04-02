/*
 * Copyright (c) 2025, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
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
 *  \file pdm_utils_pet.c
 *
 *  \brief This file implements APIs to provide PET clock info
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdio.h>
#include "pdm_utils.h"
#include "pdm_utils_soc.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                       Internal Function Declarations                       */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* Array that holds the PET clock names and their respective clock IDs
 * This is defined in pdm_utils_soc.c file, used here to produce PET clock table
 */
extern PdmUtils_PetClock PdmUtils_PetToolClocks[PDM_UTILS_PET_NUM_CLOCKS];

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */


int32_t PdmUtils_getPetClockBook(void *buf, uint32_t size)
{

   int32_t retVal = PDM_UTILS_PASS;
   uint32_t count = 0U;
   uint64_t frequency;
   PdmUtils_PetClockDataBook *petClockBook;

   if((NULL == buf) || (size < (sizeof(PdmUtils_PetClock))))
   {
       retVal = PDM_UTILS_EFAIL;
   }
   else
   {
       petClockBook = (PdmUtils_PetClockDataBook*)buf;
       petClockBook->numClocks = PDM_UTILS_PET_NUM_CLOCKS;
       for(count=0U; count<PDM_UTILS_PET_NUM_CLOCKS; count++)
       {
           petClockBook->petClocks[count].deviceId = PdmUtils_PetToolClocks[count].deviceId;
           /* Get the device data for the given deviceId */
           petClockBook->petClocks[count].clockId = PdmUtils_PetToolClocks[count].clockId;
           snprintf(petClockBook->petClocks[count].clockName,
                    sizeof(petClockBook->petClocks[count].clockName),
                    "%s", PdmUtils_PetToolClocks[count].clockName);
           /* Get the clk structure object from clock Id */
           Sciclient_pmGetModuleClkFreq(petClockBook->petClocks[count].deviceId,
                                        petClockBook->petClocks[count].clockId,
                                        &frequency,
                                        PDM_UTILS_WAIT_FOREVER);
           petClockBook->petClocks[count].freq = (uint32_t)(frequency/1000000);
       }
    }

    return retVal;
}
