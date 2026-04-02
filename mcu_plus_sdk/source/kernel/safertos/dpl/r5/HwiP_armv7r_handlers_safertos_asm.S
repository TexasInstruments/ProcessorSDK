/*
 *------------------------------------------------------------------------------
 * vIrqHandler responds to an interrupt by calling an external interrupt handler
 * and then yielding if required.
 *------------------------------------------------------------------------------
 */
    .global vApplicationFiqHandlerHook
    .arm
    .type vApplicationFiqHandlerHook, %function

    .global vApplicationFiqHandlerHook_c

vApplicationFiqHandlerHook:
    PUSH {LR}
    /* Call the interrupt handler. */
    BL      vApplicationFiqHandlerHook_c    /* This handler is provided in the .c file */
    POP {LR}
    

    .global vApplicationDataAbortHook
    .arm
    .type vApplicationDataAbortHook, %function

    .global vApplicationDataAbortHook_c

vApplicationDataAbortHook:
    PUSH {LR}
    /* Call the interrupt handler. */
    BL      vApplicationDataAbortHook_c     /* This handler is provided in the .c file */
    POP {LR}


    .global vApplicationPrefetchAbortHook
    .arm
    .type vApplicationPrefetchAbortHook, %function

    .global vApplicationPrefetchAbortHook_c

vApplicationPrefetchAbortHook:
    PUSH {LR}
    /* Call the interrupt handler. */
    BL      vApplicationPrefetchAbortHook_c  /* This handler is provided in the .c file */
    POP {LR}

    .global vApplicationAbort
    .arm
    .type vApplicationAbort, %function

    .global vApplicationAbort_c

vApplicationAbort:
    PUSH {LR}
    /* Call the interrupt handler. */
    BL      vApplicationAbort_c             /* This handler is provided in the .c file */
    POP {LR}


    .global vApplicationUndefAbortHook
    .arm
    .type vApplicationUndefAbortHook, %function

    .global vApplicationUndefAbortHook_c

vApplicationUndefAbortHook:
    PUSH {LR}
    /* Call the interrupt handler. */
    BL      vApplicationUndefAbortHook_c             /* This handler is provided in the .c file */
    POP {LR}