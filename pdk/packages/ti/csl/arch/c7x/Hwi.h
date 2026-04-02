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
 *     CREATE ARGS
 *     INTERNAL DEFINITIONS
 *     MODULE-WIDE CONFIGS
 *     PER-INSTANCE TYPES
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

#ifndef Hwi__include
#define Hwi__include

/*
 * ======== INCLUDES ========
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stddef.h>
#include <c7x.h>
#include <ti/csl/tistdtypes.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * ======== INCLUDES ========
 */

typedef struct Hwi_Params Hwi_Params;
typedef struct Hwi_Object Hwi_Object;
typedef struct Hwi_Object HwiC7x_Struct;
typedef Hwi_Object* Hwi_Handle;
typedef Hwi_Object* Hwi_Instance;



/*
 * ======== AUXILIARY DEFINITIONS ========
 */


/* FuncPtr */
typedef void (*Hwi_FuncPtr)(uint32_t arg1);

/* Irp */
typedef uint32_t Hwi_Irp;

/* MaskingOption */
enum Hwi_MaskingOption {
    Hwi_MaskingOption_NONE,
    Hwi_MaskingOption_ALL,
    Hwi_MaskingOption_SELF,
    Hwi_MaskingOption_BITMASK,
    Hwi_MaskingOption_LOWER
};
typedef enum Hwi_MaskingOption Hwi_MaskingOption;

/* StackInfo */
struct Hwi_StackInfo {
    size_t hwiStackPeak;
    size_t hwiStackSize;
    uintptr_t hwiStackBase;
};
typedef struct Hwi_StackInfo Hwi_StackInfo;


/* NUM_INTERRUPTS */
#define Hwi_NUM_INTERRUPTS (64U)

/* PlugFuncPtr */
typedef void (*Hwi_PlugFuncPtr)(void );

/* TSR_CXM */
enum Hwi_TSR_CXM {
    Hwi_TSR_CXM_GuestUser,
    Hwi_TSR_CXM_GuestSupervisor,
    Hwi_TSR_CXM_RootUser,
    Hwi_TSR_CXM_RootSupervisor,
    Hwi_TSR_CXM_SecureUser,
    Hwi_TSR_CXM_SecureSupervisor
};
typedef enum Hwi_TSR_CXM Hwi_TSR_CXM;


/*
 * ======== CREATE ARGS ========
 */

/* Args__create */
typedef struct Hwi_Args_create {
    int32_t intNum;
    Hwi_FuncPtr hwiFxn;
} Hwi_Args_create;


/*
 * ======== INTERNAL DEFINITIONS ========
 */


/* Module_State */
typedef int32_t Hwi_Module_State_intEvents[Hwi_NUM_INTERRUPTS];
typedef Hwi_Handle Hwi_Module_State_dispatchTable[Hwi_NUM_INTERRUPTS];


/*
 * ======== MODULE-WIDE CONFIGS ========
 */

/* DEFAULT_INT_PRIORITY */
extern const uint32_t Hwi_DEFAULT_INT_PRIORITY;

/* enableException */
extern const bool Hwi_enableException;

/* vectorTableBase */
extern const void * Hwi_vectorTableBase;

/* vectorTableBase_SS */
extern const void * Hwi_vectorTableBase_SS;


/*
 * ======== PER-INSTANCE TYPES ========
 */

/* Params */
struct Hwi_Params {
    Hwi_MaskingOption maskSetting;
    uintptr_t arg;
    bool enableInt;
    uint32_t eventId;
    int32_t priority;
    uint64_t disableMask;
    uint64_t restoreMask;
};

/*
 * ======== FUNCTION DECLARATIONS ========
 */

/* Module_startup */
void Hwi_Module_startup( void );


/* Instance_init */
int32_t Hwi_Instance_init(Hwi_Object *obj, uint32_t intNum, Hwi_FuncPtr hwiFxn, const Hwi_Params *prms);

/* Instance_finalize */
void Hwi_Instance_finalize(Hwi_Object *obj, int32_t status);

/* create */
Hwi_Handle Hwi_create( int32_t intNum, Hwi_FuncPtr hwiFxn, const Hwi_Params *prms);

/* construct */
int32_t Hwi_construct(HwiC7x_Struct *obj, uint32_t intNum, Hwi_FuncPtr hwiFxn, const Hwi_Params *prms);

/* delete */
void Hwi_delete(Hwi_Handle *instp);

/* destruct */
void Hwi_destruct(HwiC7x_Struct *obj);

/* getStackInfo */
bool Hwi_getStackInfo(Hwi_StackInfo *stkInfo, bool computeStackDepth);

/* startup */
void Hwi_startup(void);

/* switchFromBootStack */
void Hwi_switchFromBootStack(void);

/* post */
void Hwi_post(uint32_t intNum);

/* disableInterrupt */
uint32_t Hwi_disableInterrupt(uint32_t intNum);

/* enableInterrupt */
uint32_t Hwi_enableInterrupt(uint32_t intNum);

/* clearInterrupt */
void Hwi_clearInterrupt(uint32_t intNum);

/* GetHandle */
Hwi_Handle Hwi_getHandle(uint32_t intNum);

/* getIrp */
Hwi_Irp Hwi_getIrp(Hwi_Handle instp);

/* eventMap */
void Hwi_eventMap(uint32_t intNum, uint32_t eventId);

/* plug */
void Hwi_plug(uint32_t intNum, Hwi_PlugFuncPtr fxn);

/* getEventId */
int32_t Hwi_getEventId(uint32_t intNum);

/* disableIER */
uintptr_t Hwi_disableIER(uintptr_t mask);

/* enableIER */
uintptr_t Hwi_enableIER(uintptr_t mask);

/* restoreIER */
uint64_t Hwi_restoreIER(uint64_t mask);

/* getCXM */
Hwi_TSR_CXM Hwi_getCXM( void);

/* setCOP */
void Hwi_setCOP(int32_t cop);

/* secureStart */
void Hwi_secureStart(void);

/* reconfig */
void Hwi_reconfig(Hwi_Handle instp, Hwi_FuncPtr fxn, const Hwi_Params *params);

/* getIsrStackAddress */
char *Hwi_getIsrStackAddress(void);

void Hwi_dispatchC(uint32_t intNum);

/* dispatchCore */
void Hwi_dispatchCore(uint32_t intNum);

/* switchAndDispatch */
void Hwi_switchAndDispatch(uint32_t intNum);

/* Params_init */
void Hwi_Params_init(Hwi_Params *prms);

void Mmu_disable(void);
void Mmu_disableI(void);
void Mmu_enable(void);
void Mmu_enableI_secure(void);
void Mmu_enableI(void);
void Mmu_startup(void);
void Mmu_initFuncDefault(void);
uint32_t Hwi_disable(void);
uint32_t Hwi_enable(void);
void Hwi_Params__init__S(Hwi_Params *prms);
void Hwi_restore(uint32_t key);
Hwi_FuncPtr Hwi_getFunc(Hwi_Object *hwi, uint32_t *arg);
void Hwi_setFunc(Hwi_Object *hwi, void (*fxn)(void), uint32_t arg);
void Hwi_Params__init__S( Hwi_Params *prms);
void c7x_startup_init(void);

/*
 * ======== STATE STRUCTURES ========
 */


/* Module_State */
struct Hwi_Module_State {
    Hwi_Module_State_intEvents intEvents;
    uint64_t ierMask;
    volatile uint32_t intNum;
    char *taskSP;
    char *isrStack;
    int32_t scw;
    Hwi_Module_State_dispatchTable dispatchTable;
};

/* Module__state__V */
extern struct Hwi_Module_State Hwi_Module_state;

/* Object */
struct Hwi_Object {
    uint64_t disableMask;
    uint64_t restoreMask;
    uintptr_t arg;
    Hwi_FuncPtr fxn;
    uint32_t intNum;
    int32_t priority;
    Hwi_Irp irp;
};

#ifdef __cplusplus
    } /* extern C */
#endif

#endif

