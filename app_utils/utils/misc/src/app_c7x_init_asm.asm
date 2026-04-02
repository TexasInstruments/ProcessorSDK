;
; ======== Functions to init C7x in non-secure mode ========
;
;
    ;; uint64_t appC7xSecSupv2NonSecGuestSupv ( void );
    .global appC7xSecSupv2NonSecGuestSupv
appC7xSecSupv2NonSecGuestSupv:
        MVK64   .L1    0h,  A2
        MVC     .S1    A2,  ECLMR
        MVK64   .L1    0ffffffffffffffffh, A2
        MVC     .S1    A2,  EASGR
               
        MVC     .S1    TSR, A1
        MVK64   .L1    0fffffffffffffff8h, A2       ;; save all bits except [2:0]
        ANDD    .L1    A1, A2, A3

        ORD     .L1    A3, 01h, A3

        MVC            .S1  RP,                     A2
        MVK64   .L1     1, A4
        RETE    .S1    A2,A3

    ;; uint64_t appC7xSecSupv2NonSecSupv ( void );
    .global appC7xSecSupv2NonSecSupv
appC7xSecSupv2NonSecSupv:

        ;; Allow all events to be handled as interrupts in non-secure mode
        MVK64   .L1    0h,  A2
        MVC     .S1    A2,  ECLMR
        MVK64   .L1    0h, A2
        MVC     .S1    A2,  EASGR
        
        ;; make events 0-15 to be polled and allow them to cleared in non-secure mode
        ;; This is required for TIDL DMAs to work
        MVK64   .L1    0ffh, A2
        MVC     .S1    A2,  UFCMR
               
        ;; now switch to non-secure supervisor mode
        MVC     .S1    TSR, A1
        MVK64   .L1    0fffffffffffffff8h, A2       ;; save all bits except [2:0]
        ANDD    .L1    A1, A2, A3

        ORD     .L1    A3, 03h, A3

        MVC            .S1 RP, A2
        MVK64   .L1     1, A4
        RETE    .S1    A2, A3

