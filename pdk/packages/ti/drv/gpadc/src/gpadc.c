/*
 *  Copyright (c) Texas Instruments Incorporated 2021
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
 *   \file gpadc.c
 *
 *   \brief The file implements the Platform specific GPADC Driver Interface
 */

 /******************************************************************************
 * INCLUDE FILES
 ******************************************************************************
 */
#include <stdint.h>
#include <string.h>
#include <ti/osal/DebugP.h>
#include <ti/csl/arch/r5/csl_arm_r5_pmu.h>
#include "gpadc.h"
#include "gpadc_soc.h"

/******************************************************************************
 * GLOBAL VARIABLES/DATA-TYPES DEFINITIONS
 ******************************************************************************
 */
extern GPADC_DriverObjectType GPADCDrvObj;

bool isGPADCInitialized = FALSE;

/******************************************************************************
 * FUNCTION DEFINITIONS
 ******************************************************************************
 */

GPADC_StdReturnType GPADC_Deinit(void)
{
    GPADC_StdReturnType retVal;

    if(FALSE == isGPADCInitialized)
    {
    	retVal = E_NOT_OK;
    }
    else if(GPADC_BUSY == GPADCDrvObj.driverStatus)
    {
    	retVal = E_NOT_OK;
    }
    else
    {
    	GPADC_DriverDeInit();

        isGPADCInitialized = FALSE;

        retVal = E_OK;
    }

    return retVal;
}

GPADC_StdReturnType GPADC_Init(const GPADC_ConfigType *CfgPtr)
{
    GPADC_StdReturnType retVal;

    if(TRUE == isGPADCInitialized)
    {
    	retVal = E_NOT_OK;
    }
    else if(NULL == CfgPtr)
    {
		M_ASSERT(CfgPtr);
    	retVal = E_NOT_OK;
    }
    else
    {
        GPADC_DriverInit(CfgPtr);
        /* Release Reset for GPADC Analog and Digital Module */
        GPADC_ResetRelease();

        isGPADCInitialized = TRUE;

        retVal = E_OK;
    }

    return retVal;
}

GPADC_ConvResultType GPADC_StartGroupConversion(GPADC_channelsGroupSelectType channels)
{
    GPADC_ConvResultType retVal;

    if(FALSE == isGPADCInitialized)
    {
    	retVal = GPADC_CONV_ERROR;
    }
    else if(0U == channels.b16_Val)
    {
    	retVal = GPADC_CONV_ERROR;
    }
    else if (NULL == GPADCDrvObj.ResultBufferPtr)
    {
    	retVal = GPADC_CONV_ERROR;
    }
    else
    {
    	retVal = GPADC_StartGroup(channels);
    }

	return retVal;
}

GPADC_ConvResultType GPADC_StartSingleChannelConversion(GPADC_MeasExtSrcType channelID, uint16_t *gpadcValue)
{
    GPADC_ConvResultType retVal;

    if(FALSE == isGPADCInitialized)
    {
    	retVal = GPADC_CONV_ERROR;
    }
    else if(MAX_GPADC_MEAS_SOURCES <= channelID)
    {
    	retVal = GPADC_CONV_ERROR;
    }
    else if(NULL == gpadcValue)
    {
    	retVal = GPADC_CONV_ERROR;
    }
    else
    {
    	retVal = GPADC_StartSingleChannel(channelID, gpadcValue);
    }

	return retVal;
}

GPADC_StdReturnType GPADC_StopConversion(void)
{
	GPADC_StdReturnType retVal;

    if(FALSE == isGPADCInitialized)
    {
		retVal = E_NOT_OK;
    }
	else if(GPADC_IDLE == GPADCDrvObj.driverStatus)
	{
		retVal = E_NOT_OK;
	}
	else
	{
		GPADC_StopGroup();
		retVal = E_OK;
	}

	return retVal;
}

GPADC_StdReturnType GPADC_SetupResultBuffer(uint16_t * ResBufferPtr)
{
	GPADC_StdReturnType retVal;

	if(FALSE == isGPADCInitialized)
	{
		retVal = E_NOT_OK;
	}
	else if(NULL == ResBufferPtr)
	{
		M_ASSERT(ResBufferPtr);
		retVal = E_NOT_OK;
	}
	else if(GPADC_IDLE != GPADCDrvObj.driverStatus)
	{
		retVal = E_NOT_OK;
	}
	else
	{
		GPADCDrvObj.ResultBufferPtr = ResBufferPtr;
		retVal = E_OK;
	}

	return retVal;
}

GPADC_StdReturnType GPADC_ReadRResultBuffer(uint16_t *ResBufferPtr)
{
	uint8_t index;
	GPADC_StdReturnType retVal;

	if(FALSE == isGPADCInitialized)
	{
		retVal = E_NOT_OK;
	}
	else if(NULL == ResBufferPtr)
	{
		retVal = E_NOT_OK;
	}
	else if(GPADC_IDLE == GPADCDrvObj.driverStatus)
	{
		retVal = E_NOT_OK;
	}
	else
	{
	    for (index = 0U; index < MAX_GPADC_MEAS_SOURCES; index++)
	    {
	    	ResBufferPtr[index] = GPADCDrvObj.ResultBufferPtr[index];
	    }

	    GPADCDrvObj.driverStatus = GPADC_IDLE;
	    retVal = E_OK;
	}

    return retVal;
}

extern GPADC_StatusType GPADC_GetStatus(void)
{
	GPADC_StatusType groupStatus = GPADC_IDLE;

	if(FALSE == isGPADCInitialized)
	{
		M_ASSERT(NULL);
	}
	else
	{
		groupStatus = GPADCDrvObj.driverStatus;
	}

	return groupStatus;
}


/*
 * END OF gpadc.c FILE
 */
