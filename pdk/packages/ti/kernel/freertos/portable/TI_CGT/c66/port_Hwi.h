/*
 *  Do not modify this file; it is automatically 
 *  generated and any modifications will be overwritten.
 *
 * @(#) xdc-J06
 */

#ifndef ti_sysbios_family_c64p_Hwi__INTERNAL__
#define ti_sysbios_family_c64p_Hwi__INTERNAL__

#ifndef ti_sysbios_family_c64p_Hwi__internalaccess
#define ti_sysbios_family_c64p_Hwi__internalaccess
#endif

#include <stdint.h>

typedef struct ti_sysbios_family_c64p_Hwi_Module_State__ {
    uint16_t ierMask;
    volatile int32_t intNum;
    char *taskSP;
    char *isrStack;
    void * vectorTableBase;
    void *  bss;
    int32_t scw;
} ti_sysbios_family_c64p_Hwi_Module_State__;


extern struct ti_sysbios_family_c64p_Hwi_Module_State__ ti_sysbios_family_c64p_Hwi_Module__state__V;

#endif /* ti_sysbios_family_c64p_Hwi__INTERNAL____ */
