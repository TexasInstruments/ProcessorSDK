/*
 * Copyright (c) 2015-2020, Texas Instruments Incorporated
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
 *  \ingroup DRV_OSAL_MODULE
 *  \defgroup DRV_OSAL_LoadP LoadP
 *            LoadP interface
 *
 *  @{
 */

/** ============================================================================
 *  @file       LoadP.h
 *
 *  @brief      Load module to measure CPU Load and Task Load for the RTOS Porting Interface
 *
 *
 *  ============================================================================
 */

#ifndef ti_osal_LoadP__include
#define ti_osal_LoadP__include

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <ti/osal/TaskP.h>

/*!
 *  @brief    Status codes for LoadP APIs
 */
typedef enum LoadP_Status_e
{
    /*! API completed successfully */
    LoadP_OK         = 0,
    /*! API failed */
    LoadP_FAILURE    = (-(int32_t)1)
} LoadP_Status;

/*!
 * @brief Load statistics info
 */
typedef struct LoadP_Stats_s
{
    const char  *name;          /**< Name of the task */
    uint64_t     threadTime;    /*!< Runtime for the thread */
    uint64_t     totalTime;     /*!< Total Runtime */
    uint32_t     percentLoad;   /*!< Percentage load of the thread */
} LoadP_Stats;

/*!
 *  @brief  Function to Reset load statistics
 *
 *          Until load statistics is reset the load statistics keep getting accumulated.
 */
extern void LoadP_reset(void);

/*!
 *  @brief  Function to get task load statistics
 *        
 *  @param  taskHandle [in]  A TaskP_Handle returned from \ref TaskP_create
 *  @param  stats      [out] Returned Task Load stats  
 * 
 *  @return Status of the function
 *    - LoadP_OK: LoadP_Stats populated with the task load statistics
 *    - LoadP_FAILURE: Failed to populate LoadP_Stats with the task load statistics
 */
extern LoadP_Status LoadP_getTaskLoad(TaskP_Handle taskHandle, LoadP_Stats *stats);

/*!
 *  @brief  Function to get CPU load in percentage
 *          
 *  @return CPU load in percentage. NOTE: In case of SafeRTOS, the return value is fixed-point values with 2 decimal digits, i.e. 1 is equal to 0.01%. 
 */
extern uint32_t LoadP_getCPULoad(void);

/*!
 *  @brief  Update load statistics
 *          
 *          This updates task load statistics for all tasks created with TaskP_create().
 *          Also updates idle task load/CPU Load.
 *
 *          This function is called, every "configLOAD_WINDOW_IN_MS" msecs within the IDLE task.
 *          It is important that idle task get to run atleast once every "configLOAD_WINDOW_IN_MS" msecs
 *          for the load statistics to be correct
 *          
 *          In case of FreeRTOS, FreeRTOSConfig.h enables update in idle
 *          and idle task calls this API.
 *          FreeRTOSConfig.h:-
 * 
 *              #define configLOAD_UPDATE_IN_IDLE   (1)
 *              #define configLOAD_WINDOW_IN_MS     (500)
 */
extern void LoadP_update(void);

/*!
 *  @brief  Function to get timer counter Difference
 *          
 *  @return Differnce value
 */
uint32_t LoadP_calcCounterDiff(uint32_t cur, uint32_t last);


#ifdef __cplusplus
}
#endif

#endif /* ti_osal_LoadP__include */
/* @} */
