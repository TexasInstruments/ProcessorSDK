/**
* @file tbr.h
* @brief CT-TBR API definitions.
*
* Copyright (C) 2025 Texas Instruments Incorporated - https://www.ti.com/
* 
* 
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*
*  Redistributions of source code must retain the above copyright 
*  notice, this list of conditions and the following disclaimer.
*
*  Redistributions in binary form must reproduce the above copyright
*  notice, this list of conditions and the following disclaimer in the 
*  documentation and/or other materials provided with the   
*  distribution.
*
*  Neither the name of Texas Instruments Incorporated nor the names of
*  its contributors may be used to endorse or promote products derived
*  from this software without specific prior written permission.
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
#ifndef __TBR_H
#define __TBR_H

#include <stdint.h>
#include <stdbool.h>
#include "td_error.h"
#include "target_access.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
* @brief TBR instance handle.
*
* Client handle is a pointer to a structure internal to the module
* that contains the private instance data.  A client handle is returned by
* the TBR_open() call and is used by the client in subsequent calls
* into the module.
*
*/
typedef struct _tbr_data_t *tbr_handle_t;

/**
* @brief tbr_init_t
*
* Initialization values for TBR_open()
*/
typedef struct tbr_init_t
{
    target_access_handle_t ta;
} tbr_init_t;


/**
* @brief TBR operational mode
*
* Can be configured in either system bridge or or local buffer mode.
* 
*/
typedef enum tbr_mode_t {
    eTBR_MODE_BUFFER,        /**< Local buffer mode */
    eTBR_MODE_BRIDGE,        /**< System Bridge mode */
} tbr_mode_t;

/**
* @brief TBR event types
*
*  Events can be used to control the behavior of the TBR 
*
*  A trigger event is a CTI or ATB input trigger (if TRG_TRGIN bit of the
*  OPCTRL register is enabled) or one generated from a flush completetion
*  event.  
*/
typedef enum tbr_event_t {
    eTBR_TRIGGER,          /**< Trigger event */
    eTBR_FLUSH_COMPLETE    /**< Formatter flush completed */
} tbr_event_t;

/**
* @brief TBR event actions
*
*  Events can be used to control the behavior of the TBR
*  
*  A flush action can only be assigned to the eTBR_TRIGGER event.
*  A trigger action can only be assigned to the eTBR_FLUSH_COMPLETE event
*/
typedef enum tbr_action_t {
    eTBR_ACTION_NONE,   /**< Take no action when event occurs */
    eTBR_ACTION_STOP,   /**< Stop the formatter */
    eTBR_ACTION_FLUSH,  /**< Flush the ATB and formatter (trigger only) */
    eTBR_ACTION_TRIGGER /**< Generate a trigger event (flush only) */
} tbr_action_t;


/**
* @brief TBR buffering mode
*
*  When in local buffer mode the TBR can be configured to stop capturing data
*  when the buffer is full (one-shot) to continue capturing data until the 
*  formatter is stopped (circular)
*/
typedef enum tbr_buffer_mode_t {
    eTBR_MODE_CIRCULAR,
    eTBR_MODE_ONE_SHOT
} tbr_buffer_mode_t;

/**
* @brief TBR trigger delay
*
*  The TBR can be configured to delay the actions associated with trigger 
*  events through the use of the TRGCNT register.  This allows control over 
*  the capture of data relative to the trigger.  
*/
typedef enum tbr_trigger_mode_t
{
    eTBR_TRACE_NOTRG,   /**< not using TRGCNT */
    eTBR_TRACE_BEFORE,  /**< capture data before trigger */
    eTBR_TRACE_ABOUT,   /**< capture data around trigger (before/after) */
    eTBR_TRACE_AFTER    /**< capture data after trigger */
} tbr_trigger_mode_t;


/**
* @brief TBR configuration data
*
* Configuration data controlling the event behavior of the TBR.
* Passed into the TBR_config() call.
*
*/
typedef struct tbr_config_t
{
    tbr_mode_t             mode;               /**< Operation mode */
    tbr_action_t           on_trigger;         /**< Action to take on trigger */
    tbr_action_t           on_flush_complete;  /**< Action on flush */
    tbr_trigger_mode_t     trigger_mode;       /**< Trigger mode */
    bool                   b_trgin_enable;     /**< Enable trgin */
    bool                   b_flshin_enable;    /**< Enable flshin */
    bool                   b_formatter;        /**< Formatter enabled */
    uint32_t               src_period;         /**< Maximum frames without src id change  */
    uint32_t               seq_id;             /**< Source id for sequence */
    uint32_t               seq_period;         /**< Maximum frames between sequence data */
    bool                   b_insert_trig;      /**< Insert triggers into trace stream */
    tbr_buffer_mode_t      buffer_mode;        /**< Mode for buffering in local memory */
    bool                   b_syncreq_wrap;     /**< Generate sync pulse when buffer wraps */
    uint16_t               block_count;        /**< Block count for system bridge mode */
    uint16_t               block_size;         /**< Block size for system bridge mode */
} tbr_config_t;


#define cTBR_DEFAULT_CONFIG                             \
        {                                               \
        eTBR_MODE_BUFFER,       /* mode */              \
        eTBR_ACTION_FLUSH,      /* on_trigger */        \
        eTBR_ACTION_STOP,       /* on_flush */          \
        eTBR_TRACE_NOTRG,       /* trigger_delay */     \
        false,                  /* b_trgin_enable */    \
        false,                  /* b_flshin_enable */   \
        true,                   /* b_formatter */       \
        0x10,                   /* src_period */        \
        0x6f,                   /* seq_id */            \
        0x10,                   /* seq_period */        \
        true,                   /* b_insert_trig */     \
        eTBR_MODE_CIRCULAR,     /* buffer_mode */       \
        false,                  /* b_syncreq_wrap */    \
        0x0,                    /* block_count */       \
        0x3f                    /* block_size */        \
        }

/**
* @brief Status of the TBR
*
*/
typedef struct tbr_status_t
{
    bool b_enabled;               /**< enabled */
    bool b_flush_active;          /**< flush is active */
    bool b_formatter_halted;      /**< formatter is halted */
    bool b_reset_active;          /**< reset is active */
    bool b_partial_out;           /**< partial out */
    bool b_drain_done;            /**< drain is done */
    bool b_format_done;           /**< format is done */
    bool b_capture_done;          /**< capture is done */
    bool b_triggered;             /**< triggered */
    bool b_wrapped;               /**< wrapped/full */
} tbr_status_t;

/**
* @brief Flush request type
*
* Used in the TBR_flush() call
*/
typedef enum tbr_flush_type_t
{
    e_TBR_FLUSH_ACQUISTION,  /**< Aquisition flush */
    e_TBR_FLUSH_OUTPUT       /**< Output flush - system bridge only */
} tbr_flush_type_t;


/**
* @brief Structure containing TBR module property information.
*
* Returned by TBR_get_property().
*/
typedef struct {
    uint32_t depth;  /**< depth of TBR RAM (32-bit words). */
                     /**< a value of 0 implies system bridge only */
} tbr_property_t;

/**
* @brief Open an instance of the TBR
*
* Verify the hardware.\n
* Check that the TBR can be claimed and claim it.\n 
* Create an instance and return the instance pointer.
*
* @param[in]  p_init    The device initialization data
* @param[out] p_handle  On success, the handle to be used in subsequent TBR
*                       calls. The handle is valid until TBR_close() is called.
*
* @return returns td_error_t
*/
td_error_t
TBR_open(
        const tbr_init_t      *p_init,
        tbr_handle_t          *p_handle 
        );


/**
* @brief Configure the TBR
*
* @param[in]  handle    The TBR handle returned as part of the TBR_open() call
* @param[in]  p_config  The configuration data
*
* @return returns td_error_t
*/
td_error_t
TBR_configure(
    const tbr_handle_t      handle,
    const tbr_config_t     *p_config
    );



/**
* @brief Get the properties of the TBR
*
* @param[in]  handle       The TBR handle returned as part of the TBR_open() call
* @param[out] p_properties Pointer to a caller owned property data structure.
*                          On success, contains the structure has been filled
*                          in with the properites of the TBR
*
* @return returns td_error_t
*/
td_error_t
TBR_get_property(
    tbr_handle_t        handle,
    tbr_property_t     *p_properties
    );

/**
* @brief Enable trace capture for TBR.
*
* @param [in] handle The TBR handle returned as part of the TBR_open() call
* @return returns td_error_t
*/
td_error_t  
TBR_enable(
        tbr_handle_t handle
        );

/**
* @brief Disable TBR .
*
* @param [in] handle The TBR handle returned as part of the TBR_open() call
* @return returns td_error_t
*/
td_error_t  
TBR_disable(
    const tbr_handle_t handle
        );


/**
* @brief Flush TBR .
*
* @param [in] handle The TBR handle returned as part of the TBR_open() call
* @param [in] type The type of flush - acquistion or output
* @return returns td_error_t
*/
td_error_t
TBR_flush(
    const tbr_handle_t        handle,
    const tbr_flush_type_t    type
    );

/**
* @brief Restart TBR .
*
* @param [in] handle The TBR handle returned as part of the TBR_open() call
* @return returns td_error_t
*/
td_error_t
TBR_restart(
const tbr_handle_t handle
);



/**
* @brief Reset TBR .
*
* @param [in] handle The TBR handle returned as part of the TBR_open() call
* @return returns td_error_t
*/
td_error_t
TBR_reset(
const tbr_handle_t handle
);

/**
* @brief Get the status of the TBR
*
* @param [in] handle The TBR handle returned as part of the TBR_open() call
* @param [out] p_status On success contains the current status of the TBR
* @param [out] p_available On success contains the amount of data available in the TBR
* @return returns td_error_t
*/
td_error_t  
TBR_status(
        const tbr_handle_t  handle,
        tbr_status_t *      p_status, 
        uint32_t          * p_available
        );

/**
* @brief Read data from the TBR buffer
*
* @param [in] handle The TBR handle returned as part of the TBR_open() call
* @param [in] max Maximum number of 32-bit words to read
* @param [out] p_buffer Pointer to a buffer, which on success will contain the requested data.  It is up to the caller to ensure that the buffer is large enough.
* @param [out] p_actual A pointer to a 32-bit unsigned integer, which on success contains the number of 32-bit words actually returned.  If the actual value returned is less than the maximum requested then the end of the buffered data was reached.
* @return returns td_error_t
*/
td_error_t  
TBR_read(
        const tbr_handle_t handle, 
        const uint32_t  max, 
        uint32_t *p_buffer, 
        uint32_t *p_actual
        );


/**
* @brief Close/Release the TBR
* 
* Unclaim the TBR register.\n 
* Delete the instance data created on TBR_open().\n
* On return the handle is no longer valid and cannot be used by the client.
*
* @param [in] handle The TBR handle returned as part of the TBR_open() call
*
* @return returns void
*/
void  
TBR_close(
    const tbr_handle_t handle
        );


#ifdef __cplusplus
}
#endif

#endif //__TBR_H
