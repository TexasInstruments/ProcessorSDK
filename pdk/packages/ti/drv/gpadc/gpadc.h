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
 *  \defgroup DRV_GPADC_MODULE GPADC Driver
 *
 *  The GPADC Driver allows the monitoring of the GPADC channels<1-9> external to the device.
 *
 *  The GPADC header file should be included in an application as follows:
 *  \code
    #include <ti/drv/gpadc/gpadc.h>
    \endcode
 *
 *
 * ## Initializing the driver #
 * Before using the GPADC Driver, it has to be initialized; The GPADC Driver instance should be
 * initialized by calling the #GPADC_Init. Configuration parameters should be passed to the
 * function for all the channels which ADC conversion to be done.
 *
 * ## Configuration Parameters #
 * Each GPADC channel needs to be configured with certain parameters to start conversion.
 * #GPADC_ConfigType is the structure which needs to be passed to #GPADC_Init .This structure contains
 * the configuration of conversion mode, trigger source, configuration parameters for each channel.
 *
 * #GPADC_ConfigType structure supports channel configurations for maximum of 9 channels together.
 * Each channel configuration #GPADC_ChannelConfigType structure contains fields for configuring
 * channel id, buffered/ unbuffered mode, number of samples to be skipped, number of samples to be
 * collected, flag to choose skip samples and collect samples from the lookup table.
 *
 * NOTE: As of today, only IFM mode is supported in TPR12 GPADC Driver and hence GPADC_ONESHOT_CONV_MODE
 * has to be used for configuration of mode and since IFM mode doesn't support HW triggers for conversion,
 * GPADC_TRIGG_SRC_SW has to be used.
 *
 *
 * ## Using the driver #
 *
 * For single channel conversion and multi channel conversion each example has been given in the test
 * application.
 *
 * ## Multichannel or Group Conversion #
 *
 * * Driver initialization for all the channels to be monitored.
 * * Setup result buffer which can accomodate all channels conversion values.
 * * Set the channel bitmap and start group conversion
 *
 * The result buffer contains zero as the value for the channels which aren't set in the bitmap or which
 * aren't configured in the initialization
 *
 * \code

    memset(&CfgPtr,0,sizeof(CfgPtr));
    memset(&gpadc_result[0],0,sizeof(gpadc_result));


	CfgPtr.triggSrc = GPADC_TRIGG_SRC_SW;
	CfgPtr.convMode = GPADC_ONESHOT_CONV_MODE;

	UART_printf("Select Channel Bitmap for Group Read <0x1FF to 0x001>\t");
	UART_scanFmt ("%x",&channelIndex);

	for (index = 0; index < MAX_GPADC_MEAS_SOURCES; index++)
	{
		if(channelIndex | (0x01 << index))
		{
			CfgPtr.channelConfig[index].channelID = (GPADC_MeasExtSrcType)index;
			CfgPtr.channelConfig[index].isConfigured = TRUE;
			CfgPtr.channelConfig[index].isBufferedMode = FALSE;
			CfgPtr.channelConfig[index].useLuTable = TRUE;
		}
	}

	channels.bits.b9_ChannelSelectionBitMap = (channelIndex & 0x1FF);

	GPADC_Init(&CfgPtr);
	GPADC_SetupResultBuffer(&gpadc_result[0]);
	convRes = GPADC_StartGroupConversion(channels);

   \endcode
 *
 * ## Single channel Conversion #
 *
 * * Driver initialization for the specific channel
 * * Configure channel ID and start specific channel conversion
 *
 * \code

    memset(&CfgPtr,0,sizeof(CfgPtr));

	CfgPtr.triggSrc = GPADC_TRIGG_SRC_SW;
	CfgPtr.convMode = GPADC_ONESHOT_CONV_MODE;

	CfgPtr.channelConfig[0].channelID = (GPADC_MeasExtSrcType)channelIndex;
	CfgPtr.channelConfig[0].isConfigured = TRUE;
	CfgPtr.channelConfig[0].isBufferedMode = FALSE;
	CfgPtr.channelConfig[0].useLuTable = TRUE;

	GPADC_Init(&CfgPtr);
	convRes = GPADC_StartSingleChannelConversion((GPADC_MeasExtSrcType)channelIndex, &gpadc_result);

   \endcode
 *
 */

/**
 *   \file  drv/gpadc/gpadc.h
 *
 *   \brief
 *      This is the header file for the GPADC driver which exposes the
 *      data structures and exported API which can be used by the
 *      applications to use the GPADC driver.
 */

/****************************************************************************************
 * FILE INCLUSION PROTECTION
 ****************************************************************************************
 */
 #ifndef DRV_GPADC_H
 #define DRV_GPADC_H

/****************************************************************************************
 * INCLUDE FILES
 ****************************************************************************************
 */
#ifdef __cplusplus
extern "C" {
#endif

 #include <ti/drv/gpadc/gpadc_cfg.h>
/****************************************************************************************
 * MACRO DEFINITIONS
 ****************************************************************************************
 */

/**
\defgroup GPADC_DRIVER_EXTERNAL_FUNCTION            GPADC Driver External Functions
\ingroup DRV_GPADC_MODULE
\brief
*   The section has a list of all the exported API which the applications need to
*   invoke in order to use the driver
*/

/******************************************************************************
 * TYPE-DEFINE STRUCT/ENUM/UNION DEFINITIONS
 ******************************************************************************
 */

/******************************************************************************
 * EXTERN GLOBAL VARIABLES/DATA-TYPES DECLARATIONS
 ******************************************************************************
 */

/******************************************************************************
 * FUNCTION PROTOTYPES
 ******************************************************************************
 */

/**
 *  @b Description
 *  @n
 *      Initializes the GPADC Driver with the channels configuration.
 *
 *  \param[in]  CfgPtr
 *      Pointer to the GPADC configuration
 *
 *  \ingroup GPADC_DRIVER_EXTERNAL_FUNCTION
 *
 *  \retval
 *              E_OK:     initialization successful
 *  \retval
 *              E_NOT_OK: initialization failed
 */
extern GPADC_StdReturnType GPADC_Init(const GPADC_ConfigType *CfgPtr);

/**
 *  @b Description
 *  @n
 *      De-initializes the GPADC Driver.
 *
 *  <b>Note</b>\n      GPADC_Init() should be done before de-initialization\n
 *                     GPADC driver status shouldn't be in GPADC_BUSY state.
 *
 *  \ingroup GPADC_DRIVER_EXTERNAL_FUNCTION
 *
 *  \retval
 *              E_OK:     De-init successful
 *  \retval
 *              E_NOT_OK: De-init failed
 */
extern GPADC_StdReturnType GPADC_Deinit(void);

/**
 *  @b Description
 *  @n
 *      Starts and triggers the multi channel ADC conversion.
 *
 *      Pass the channel selection bitmap for group conversion to the GPADC Driver and
 *      check the result buffer passed to the driver using GPADC_SetupResultBuffer()
 *      for conversion results of all configured channels.
 *
 *      Conversion happens based on the parameters provided through GPADC_Init()
 *      for GPADC HW unit and each channel config like sampling parameters, buffered/ unbuffered.
 *
 *  <b>Note</b>\n      GPADC_Init() should be done before starting conversion\n
 *                     Channel bitmap selected shouldn't be zero\n
 *                     Result buffer should be setup before starting conversion using GPADC_SetupResultBuffer().
 *
 *  \param[in]  channels
 *  	Channel selection bitmap to start conversion together
 *
 *  \ingroup GPADC_DRIVER_EXTERNAL_FUNCTION
 *
 *  \retval
 *              GPADC_CONV_ERROR: GPADC Conversion Error
 *  \retval
 *              GPADC_CONV_DONE: GPADC Conversion Successfully Done
 *  \retval
 *              GPADC_CONV_CHANNEL_CONFIG_MISSING: One or more of the bitmap selected
 *               channel group is missing configuration during initialization
 */
extern GPADC_ConvResultType GPADC_StartGroupConversion(GPADC_channelsGroupSelectType channels);

/**
 *  @b Description
 *  @n
 *      Starts and triggers the single channel conversion.
 *
 *      Pass the channelID and result address to the driver and
 *      the result will be stored in the address passed.
 *
 *  \param[in]  channelID
 *		Channel index from <0-8> available external sources
 *
 *  \param[out]  gpadcValue
 *      Pointer to the address where the result shall be stored.
 *
 *  \ingroup GPADC_DRIVER_EXTERNAL_FUNCTION
 *
 *  \retval
 *              GPADC_CONV_ERROR: GPADC Conversion Error
 *  \retval
 *              GPADC_CONV_DONE: GPADC Conversion Successfully Done
 *  \retval
 *              GPADC_CONV_CHANNEL_CONFIG_MISSING: The specified channel
 *               channel is not configured using GPADC_Init()
 */
extern GPADC_ConvResultType GPADC_StartSingleChannelConversion(GPADC_MeasExtSrcType channelID, uint16_t *gpadcValue);

/**
 *  @b Description
 *  @n
 *      Stops the GPADC conversion.
 *
 *
 *  <b>Note</b>\n  This API should not be called before initialization.\n
 *                 This API should not be called before starting group/single channel start conversion\n
 *                 To be used when the mode of conversion is chosen as GPADC_CONTINUOUS_CONV_MODE.
 *
 *  \ingroup GPADC_DRIVER_EXTERNAL_FUNCTION
 *
 *  \retval
 *              E_OK:     Stop conversion successful.
 *  \retval
 *              E_NOT_OK: Stop conversion failed.
 */
extern GPADC_StdReturnType GPADC_StopConversion(void);

/**
 *  @b Description
 *  @n
 *      Initializes GPADC driver with the group specific result
 *      buffer start address where the conversion results will be stored.
 *
 *      The application has to ensure that the application buffer,
 *      where ResBufferPtr points to, can hold all the conversion results
 *      of the specified group. The initialization with GPADC_SetupResultBuffer
 *      is required after GPADC_Init(), before a group conversion can be started.
 *
 *  \param[in]  ResBufferPtr
 *
 *  \ingroup GPADC_DRIVER_EXTERNAL_FUNCTION
 *
 *  \retval
 *              E_OK:     Result Buffer Setup successful
 *  \retval
 *              E_NOT_OK: Result Buffer Setup failed
 */
extern GPADC_StdReturnType GPADC_SetupResultBuffer(uint16_t * ResBufferPtr);

/**
 *  @b Description
 *  @n
 *      The function is used to read the result buffer from the GPADC Driver.
 *
 *      Reads the group conversion result of the last completed conversion round
 *      of the requested group and stores the channel values starting at the
 *      ResBufferPtr address.
 *
 *  \param[out]  ResBufferPtr
 *
 *  \ingroup GPADC_DRIVER_EXTERNAL_FUNCTION
 *
 *  \retval
 *              E_OK:     Read Result Buffer successful
 *  \retval
 *              E_NOT_OK: Read Result Buffer failed
 */
extern GPADC_StdReturnType GPADC_ReadRResultBuffer(uint16_t *ResBufferPtr);

/**
 *  @b Description
 *  @n
 *      Gets the status of GPADC Driver HW unit.
 *
 *  \ingroup GPADC_DRIVER_EXTERNAL_FUNCTION
 *
 *  \retval
 *              GPADC_IDLE: The conversion of the specified group has not been started.
 *                          No result is available.
 *  \retval
 *              GPADC_BUSY: The conversion of the specified group has been started and is still
 *                          going on. So far no result is available.
 *  \retval
 *              GPADC_COMPLETED: A conversion round of the specified group has been finished.
 *                               A result is available for all specified channels of the group.
 *
 */
extern GPADC_StatusType GPADC_GetStatus(void);


#ifdef __cplusplus
}
#endif

#endif
/*
 * END OF DRV_GPADC_H
 */
