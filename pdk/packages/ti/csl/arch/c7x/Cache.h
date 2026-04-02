/*
 *  Copyright (C) 2018-2023 Texas Instruments Incorporated
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

/*
 * ======== GENERATED SECTIONS ========
 *
 *     PROLOGUE
 *     INCLUDES
 *
 *     INTERNAL DEFINITIONS
 *     MODULE-WIDE CONFIGS
 *     VIRTUAL FUNCTIONS
 *     FUNCTION DECLARATIONS
 *     CONVERTORS
 *     SYSTEM FUNCTIONS
 *
 *     EPILOGUE
 *     STATE STRUCTURES
 *     PREFIX ALIASES
 */


/*
 * ======== PROLOGUE ========
 */

#ifndef Cache__include
#define Cache__include

/*
 * ======== INCLUDES ========
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stddef.h>
#include <c7x.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Cache_Size Cache_Size;
typedef struct Cache_Module_State Cache_Module_State;

/**
 *  \anchor Cache_Type
 *  \name Cache type definitions
 *  @{
 */
/*!
 *  @brief  This enumerator defines the cache types
 */
typedef uint16_t Cache_Type;
/** Cache L1P type */
#define Cache_Type_L1P                        ((uint16_t) 0x1U)
/**Cache L1D type */
#define Cache_Type_L1D                        ((uint16_t) 0x2U)
/**Cache L1 type */
#define Cache_Type_L1                         ((uint16_t) 0x3U)
/**Cache L2P type */
#define Cache_Type_L2P                        ((uint16_t) 0x4U)
/**Cache L2D type */
#define Cache_Type_L2D                        ((uint16_t) 0x8U)
/**Cache L2 type */
#define Cache_Type_L2                         ((uint16_t) 0xCU)
/**Cache ALLP type */
#define Cache_Type_ALLP                       ((uint16_t) 0x5U)
/**Cache ALLD type */
#define Cache_Type_ALLD                       ((uint16_t) 0xAU)
/**Cache ALL type */
#define Cache_Type_ALL                        ((uint16_t) 0x7FFFU)
/* @} */

/**
 *  \anchor Cache_L1Size
 *  \name Cache L1 Size definitions
 *  @{
 */
/*!
 *  @brief  This enumerator defines the cache L1 Sizes
 */
typedef uint16_t Cache_L1Size;
/**Cache L1 size of 0K */
#define Cache_L1Size_0K                       ((uint16_t) 0U)
/**Cache L1 size of 4K */
#define Cache_L1Size_4K                       ((uint16_t) 1U)
/**Cache L1 size of 8K */
#define Cache_L1Size_8K                       ((uint16_t) 2U)
/**Cache L1 size of 16K */
#define Cache_L1Size_16K                      ((uint16_t) 3U)
/**Cache L1 size of 32K */
#define Cache_L1Size_32K                      ((uint16_t) 4U)
/* @} */

/**
 *  \anchor Cache_L2Size
 *  \name Cache L2 Size definitions
 *  @{
 */
/*!
 *  @brief  This enumerator defines the cache L2 Sizes
 */
typedef uint16_t Cache_L2Size;
/**Cache L2 size of 0K */
#define Cache_L2Size_0K                      ((uint16_t) 0U)
/**Cache L2 size of 32K */
#define Cache_L2Size_32K                     ((uint16_t) 1U)
/**Cache L2 size of 64K */
#define Cache_L2Size_64K                     ((uint16_t) 2U)
/**Cache L2 size of 128K */
#define Cache_L2Size_128K                    ((uint16_t) 3U)
/**Cache L2 size of 256K */
#define Cache_L2Size_256K                    ((uint16_t) 4U)
/**Cache L2 size of 512K */
#define Cache_L2Size_512K                    ((uint16_t) 5U)
/**Cache L2 size of 1024K */
#define Cache_L2Size_1024K                   ((uint16_t) 6U)
/* @} */

/*
 * ======== AUXILIARY DEFINITIONS ========
 */

/* Mode */
enum Cache_Mode {
    Cache_Mode_FREEZE,
    Cache_Mode_BYPASS,
    Cache_Mode_NORMAL
};
typedef enum Cache_Mode Cache_Mode;

/* Size */
struct Cache_Size {
    Cache_L1Size l1pSize;
    Cache_L1Size l1dSize;
    Cache_L2Size l2Size;
};

/* initSize */
extern const Cache_Size Cache_initSize;
/*
 * ======== FUNCTION DECLARATIONS ========
 */

/* enable */
void Cache_enable( uint16_t type);

/* inv */
void Cache_inv(void * blockPtr, size_t byteCnt, uint16_t type, bool wait);

/* wb */
void Cache_wb(void * blockPtr, size_t byteCnt, uint16_t type, bool wait);

/* wbInv */
void Cache_wbInv(void * blockPtr, size_t byteCnt, uint16_t type, bool wait);

/* wait */
void Cache_wait( void);

/* disable */
void Cache_disable( uint16_t type);

/* getSize */
void Cache_getSize( Cache_Size *size);

/* setSize */
void Cache_setSize( Cache_Size *size);

/* wbAll */
void Cache_wbAll( void);

/* wbL1dAll */
void Cache_wbL1dAll( void);

/* wbInvAll */
void Cache_wbInvAll( void);

/* wbInvL1dAll */
void Cache_wbInvL1dAll( void);

/* setL2CFG */
void Cache_setL2CFG( uint64_t size);

/* getL2CFG */
uint64_t Cache_getL2CFG( void);

/* setL1DCFG */
void Cache_setL1DCFG( uint64_t size);

/* getL1DCFG */
uint64_t Cache_getL1DCFG( void);

/* setL2WB */
void Cache_setL2WB( uint32_t flag);

/* setL2WBINV */
void Cache_setL2WBINV( uint32_t flag);

/* setL1DWB */
void Cache_setL1DWB( uint32_t flag);

/* setL1DWBINV */
void Cache_setL1DWBINV( uint32_t flag);

/* startup */
void Cache_startup( void);

void Cache_Module_startup( void );


/*
 * ======== STATE STRUCTURES ========
 */
/* Module_State */
struct Cache_Module_State {
    uint64_t L1DCFG;
    uint64_t L2CFG;
};

/* Module__state__V */
extern struct Cache_Module_State Cache_Module_state;

#ifdef __cplusplus
    } /* extern C */
#endif

#endif
