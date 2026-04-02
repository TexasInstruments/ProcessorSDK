/**
 *   @file  sa_osal.h
 *
 *   @brief This is the sample OS Adaptation layer which is used by the SA low level
 *          driver. 
 *
 *   @details The OSAL layer can be ported in either of the following 
 *            manners to a native OS:
 *
 *      <b> Approach 1: </b>
 *      @n  Use Prebuilt Libraries
 *           - Ensure that the LLD users provide an implementation of all 
 *             Osal_XXX APIs for their native OS.
 *           - Link the prebuilt libraries with their application.
 *           - Refer to the "test" directory for an example of this
 *       @n <b> Pros: </b>
 *           - Customers can reuse prebuilt TI provided libraries
 *       @n <b> Cons: </b>
 *           - Level of indirection in the API to get to the actual OS call
 *              
 *      <b> Approach 2: </b>
 *      @n  Rebuilt Library 
 *           - Create a copy of this file and modify it to directly 
 *             inline the native OS calls
 *           - Rebuild the SA low level driver library; ensure that the Include 
 *             path points to the directory where the copy of this file 
 *             has been provided.
 *           - Please refer to the "test" directory for an example of this 
 *       @n <b> Pros: </b>
 *           - Optimizations can be done to remove the level of indirection
 *       @n <b> Cons: </b>
 *           - SA LLD Libraries need to be rebuilt by the customer.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2009-2012 Texas Instruments, Inc.
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
#ifndef __SA_OSAL_H__
#define __SA_OSAL_H__

/** @addtogroup SA_LLD_OSAL
 @{ */
 
/**********************************************************************
 ************************* Extern Declarations ************************
 **********************************************************************/

extern void  Osal_saCsEnter (uint32_t *key);
extern void  Osal_saCsExit (uint32_t key);
extern void  Osal_saMtCsEnter (uint32_t *key);
extern void  Osal_saMtCsExit (uint32_t key);
extern void  Osal_saBeginMemAccess(void* addr, uint32_t size);
extern void  Osal_saEndMemAccess  (void* addr, uint32_t size);
extern void  Osal_saBeginScAccess (void* addr, uint32_t size);
extern void  Osal_saEndScAccess   (void* addr, uint32_t size);
extern void  Osal_saLog( char* fmt, ... );
extern uint16_t Osal_saGetProcId (void);
extern void* Osal_saGetSCPhyAddr(void* vaddr);
extern int   Osal_saGetSysEndianMode(void); 

/**
 * @brief   The macro is used by the SA LLD to provide critical section to 
 *          protect its shared resources access from multiple threads 
 *          on a single core. If all the SA LLD APIs are being called from threads
 *          then this API could use semaphores. However, if the SA LLD API's are 
 *          being called from both ISR & Thread context then the critical section here 
 *          would need to disable/enable interrupts.    
 *
 * <b> Prototype: </b>
 *  The following is the C prototype for the expected OSAL API.
 *
 *  @verbatim
       void Osal_saCsEnter (uint32_t *key)
    @endverbatim
 *      
 *  <b> Parameter </b>
 *  @n  Key may be used to lock the critical section.
 *
 *  <b> Return Value </b>
 *  @n  Not applicable.
 */
#define Sa_osalCsEnter            Osal_saCsEnter

/**
 * @brief   The macro is used by the SA LLD to exit a critical section 
 *      protected using the Osal_saCsEnter() API.
 *
 * <b> Prototype: </b>
 *  The following is the C prototype for the expected OSAL API.
 *
 *  @verbatim
       void Osal_saCsExit (uint32_t key)
    @endverbatim
 *      
 *  <b> Parameter </b>
 *  @n  Key may be used to lock the critical section.
 *
 *  <b> Return Value </b>
 *  @n  Not applicable.
 */
#define Sa_osalCsExit             Osal_saCsExit

/**
 * @brief   The macro is used by the SA LLD to provide critical section to 
 *          protect its global and shared resources access from multiple cores. 
 *          If the SALLD operates in single-core environment then these macros can 
 *          be defined to be NOP. 
 *
 * <b> Prototype: </b>
 *  The following is the C prototype for the expected OSAL API.
 *
 *  @verbatim
       void Osal_saMtCsEnter (uint32_t *key)
    @endverbatim
 *      
 *  <b> Parameter </b>
 *  @n  Key may be used to lock the multi-core critical section.
 *
 *  <b> Return Value </b>
 *  @n  Not applicable.
 *
 *  @note The multiple-core protection area is usually pretty small. To avoid the scenario that the
 *        interrupt occurs after the multi-core lock is achieved, the interrupt may be disabled in 
 *        this function and re-enabled at the function Osal_saMtCsExit.
 */
#define Sa_osalMtCsEnter          Osal_saMtCsEnter

/**
 * @brief   The macro is used by the SA LLD to exit a critical section 
 *      protected using the Osal_saMtCsEnter() API.
 *
 * <b> Prototype: </b>
 *  The following is the C prototype for the expected OSAL API.
 *
 *  @verbatim
       void Osal_saMtCsExit (uint32_t key)
    @endverbatim
 *      
 *  <b> Parameter </b>
 *  @n  Key may be used to lock the multi-core critical section.
 *
 *  <b> Return Value </b>
 *  @n  Not applicable.
 */
#define Sa_osalMtCsExit           Osal_saMtCsExit

/**
 * @brief   The macro is used by the SA LLD to log various messages. 
 *
 * <b> Prototype: </b>
 *  The following is the C prototype for the expected OSAL API.
 *
 *  @verbatim
       void Osal_saLog( char* fmt, ... ) 
    @endverbatim
 *
 *  <b> Parameter </b>
 *  @n  printf-style format string 
 *
 *  <b> Return Value </b>
 *  @n  Not applicable.
 */
#define Sa_osalLog                Osal_saLog

/**
 * @brief   The macro is used by the SA LLD to indicate that a block
 * of memory is about to be accessed. If the memory block is cached then
 * this indicates that the application would need to ensure that the cache
 * is updated with the data from the actual memory.
 *
 * <b> Prototype: </b>
 *  The following is the C prototype for the expected OSAL API.
 *
 *  @verbatim
        void Osal_saBeginMemAccess (void* addr, uint32_t sizeWords)
    @endverbatim
 *
 *  <b> Parameters </b>
 *  @n  Address of memory block.
 *  @n  Size of memory block.
 */

#define Sa_osalBeginMemAccess   Osal_saBeginMemAccess


/**
 * @brief   The macro is used by the SA LLD to indicate that the block of 
 * memory has finished being updated. If the memory block is cached then the 
 * application would need to ensure that the contents of the cache are updated
 * immediately to the actual memory.
 *
 * <b> Prototype: </b>
 *  The following is the C prototype for the expected OSAL API.
 *
 *  @verbatim
        void Osal_saEndMemAccess (void* addr, uint32_t sizeWords)
    @endverbatim
 *
 *  <b> Parameters </b>
 *  @n  Address of memory block.
 *  @n  Size of memory block.
 */
 
#define Sa_osalEndMemAccess   Osal_saEndMemAccess

/**
 * @brief   The macro is used by the SA LLD to indicate that the security
 * context byuffer is about to be accessed. If the security context buffer 
 * is cacheable then this indicates that the application would need to ensure 
 * that the cache is updated with the data from the actual memory since the
 * security context will be updated by SASS Cache engine.
 * If the security context buffers are non-cacheable then these macros can 
 * be defined to be NOP. 
 *
 * <b> Prototype: </b>
 *  The following is the C prototype for the expected OSAL API.
 *
 *  @verbatim
        void Osal_saBeginScAccess (void* addr, uint32_t sizeWords)
    @endverbatim
 *
 *  <b> Parameters </b>
 *  @n  Address of memory block.
 *  @n  Size of memory block.
 */

#define Sa_osalBeginScAccess   Osal_saBeginScAccess


/**
 * @brief   The macro is used by the SA LLD to indicate that the security 
 * context buffer has finished being updated. If the memory block is cacheable 
 * then the application would need to ensure that the contents of the cache are 
 * updated immediately to the actual memory.
 * If the security context buffers are non-cacheable then these macros can 
 * be defined to be NOP. 
 *
 * <b> Prototype: </b>
 *  The following is the C prototype for the expected OSAL API.
 *
 *  @verbatim
        void Osal_saEndScAccess (void* addr, uint32_t sizeWords)
    @endverbatim
 *
 *  <b> Parameters </b>
 *  @n  Address of memory block.
 *  @n  Size of memory block.
 */
 
#define Sa_osalEndScAccess   Osal_saEndScAccess

/**
 * @brief   The macro is used by the SA LLD to get the processor ID(zero based)
 * <b> Prototype: </b>
 *  The following is the C prototype for the expected OSAL API.
 *
 *  @verbatim
 *      uint32_t Osal_saGetProcId () 
 *   @endverbatim
 *
 *  <b> Return Value Processor ID zero based </b>
 *  @n  Not applicable.
 */ 
#define Sa_osalGetProcId    Osal_saGetProcId

/**
 * @brief   The macro is used by the SA LLD to get the physical address
 * of the security context buffer or its system instance.  
 * <b> Prototype: </b>
 *  The following is the C prototype for the expected OSAL API.
 *
 *  @verbatim
 *      void* Osal_saGetSCPhyAddr(void* vaddr) 
 *   @endverbatim
 *
 *  <b> Return Value Physical address of the Security Context buffer or system instance </b>
 *  <b> Parameters </b>
 *  @n  Virtual Address of the Security Context buffer.
 */ 
#define Sa_osalGetSCPhyAddr   Osal_saGetSCPhyAddr

/**
 *  @defgroup sysEndianMode SALLD System Endian Mode
 *  @ingroup salld_api_constants
 *  @{
 *
 *  @name SALLD System Endian Mode
 *
 *  Definition of System Endian Modes 
 *  
 */ 
/*@{*/
typedef enum {
  sa_SYS_ENDIAN_MODE_LITTLE = 0,  /**< SoC in Little-Endian mode */
  sa_SYS_ENDIAN_MODE_BIG          /**< SoC in Big-Endian mode */
} Sa_sysEndianMode_e;
/*@}*/
/** @} */


/**
 * @brief   The macro is used by the SA LLD to query the endian mode of the system (SoC).
 *
 * <b> Prototype: </b>
 *  The following is the C prototype for the expected OSAL API.
 *
 *  @verbatim
 *      int Osal_saGetSysEndianMode(void) 
 *   @endverbatim
 *
 *  <b> Return Value Endian mode of the system (SoC) </b>
 *  <b> Parameters </b>
 *  @n  Endian mode of the system (SoC).
 */ 
#define Sa_osalGetSysEndianMode   Osal_saGetSysEndianMode

/**
@}
*/
#endif /* __SA_OSAL_H__ */

