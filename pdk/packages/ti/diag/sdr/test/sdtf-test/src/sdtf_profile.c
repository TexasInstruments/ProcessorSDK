/*
 * SDR TEST
 *
 * Software Diagnostics Reference Test
 *
 *  Copyright (c) Texas Instruments Incorporated 2018-2020
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
 *
 */

#include <string.h>
#include "sdtf_profile.h"

#define SDTF_MAX_PROFIlE_ENTRIES 32

typedef struct SDTF_profileEntry_s {
    uint32_t profileStart;
    uint32_t profileEnd;
} SDTF_profileEntry_t;

SDTF_profileEntry_t SDTF_profileEntry[SDTF_MAX_PROFIlE_ENTRIES];

/*********************************************************************
* @fn      SDTF_profileInit
*
* @brief   Profile point init routine
*
* @param   maxNumProfiles  Max Number of profile points
*
* @return    0 : Success; < 0 for failures
*/
int32_t SDTF_profileInit(uint32_t maxNumProfiles)
{

    memset(SDTF_profileEntry, 0, sizeof(SDTF_profileEntry));

    SDTF_profileTimerInit();

    return 0;
}


/*********************************************************************
* @fn      SDTF_profileBegin
*
* @brief   Profile point begin routine
*
* @param   profileNumber  Profile number
*
* @return  None
*/
void SDTF_profileBegin(uint32_t profileNumber)
{
    SDTF_profileEntry[profileNumber].profileStart = SDTF_profileTimerRead();
}

/*********************************************************************
* @fn      SDTF_profileEnd
*
* @brief   Profile point end routine
*
* @param   profileNumber  Profile number
*
* @return  None
*/
void SDTF_profileEnd(uint32_t profileNumber)
{
    SDTF_profileEntry[profileNumber].profileEnd = SDTF_profileTimerRead();
}

/*********************************************************************
* @fn      SDTF_profileDelta
*
* @brief   calculate delta time
*
* @param   profileNumber  Profile number
*
* @return  Time delta value for profile number
*/
uint32_t SDTF_profileDelta(uint32_t profileNumber)
{
    return(SDTF_profileEntry[profileNumber].profileEnd-
           SDTF_profileEntry[profileNumber].profileStart);
}
