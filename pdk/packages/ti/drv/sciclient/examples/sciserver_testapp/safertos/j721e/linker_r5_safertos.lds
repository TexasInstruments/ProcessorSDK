/*=========================*/
/*     Linker Settings     */
/*=========================*/

--retain="*(.bootCode)"
--retain="*(.startupCode)"
--retain="*(.startupData)"
--retain="*(.irqStack)"
--retain="*(.fiqStack)"
--retain="*(.abortStack)"
--retain="*(.undStack)"
--retain="*(.svcStack)"
--retain="*(.intvecs)"
--retain="*(.intc_text)"
--retain="*(.rstvectors)"
--retain="*(.safeRTOSrstvectors)"

--fill_value=0
--diag_suppress=10063                   /* entry point not _c_int00 */
--stack_size=0x4000
--heap_size=0x8000
--entry_point=_axSafeRTOSresetVectors     /* C RTS boot.asm with added SVC handler	*/

-stack  0x4000  /* SOFTWARE STACK SIZE */
-heap   0x8000  /* HEAP AREA SIZE      */

/*-------------------------------------------*/
/*       Stack Sizes for various modes       */
/*-------------------------------------------*/
__IRQ_STACK_SIZE   = 0x1000;
__FIQ_STACK_SIZE   = 0x0100;
__ABORT_STACK_SIZE = 0x0100;
__UND_STACK_SIZE   = 0x0100;
__SVC_STACK_SIZE   = 0x1000;

/*--------------------------------------------------------------------------*/
/*                               Memory Map                                 */
/*--------------------------------------------------------------------------*/
MEMORY
{
    VECTORS (X)                 : ORIGIN = 0x00000000 , LENGTH = 0x00000100
    R5F_TCMA0(RWIX)             : ORIGIN = 0x00000100 , LENGTH = 0x00007F00
    R5F_TCMB0(RWIX)             : ORIGIN = 0x41010100 , LENGTH = 0x00007F00
    SBL_USED_OCMC_RAM           : ORIGIN = 0x41C00000 , LENGTH = 0x80000
    OCMC_RAM_BOARD_CFG (RWIX)   : ORIGIN = 0x41c80000 , LENGTH = 0x2000
    /* Sciserver App Space */
    OCMC_RAM_SCISERVER (RWIX)   : ORIGIN = 0x41C82000 , LENGTH = 0x61100
    OCMC_RAM_X509_HEADER1 (RWIX) : ORIGIN = 0x41cffb00 , LENGTH = 0x500 /* covers header for J7200/J721E */
    OCMC_RAM_X509_HEADER2 (RWIX) : ORIGIN = 0x41cfdb00 , LENGTH = 0x500 /* covers header for J721S2 */
}  /* end of MEMORY */

/*----------------------------------------------------------------------------*/
/* Section Configuration                                                      */

SECTIONS
{
    GROUP
    {
        .safeRTOSrstvectors                                 : {} palign(8)
        .rstvectors                                         : {} palign(8)
    } > VECTORS
    
/* Startup code sections. */
    GROUP
    {
        .pinit                                                  : {} align( 32 )
        .cinit                                                  : {} align( 32 )
        .MPU_INIT_FUNCTION                                      : {} palign( 8 )
    } > R5F_TCMA0


    .bootCode                                               : {} palign( 8 ) > R5F_TCMB0
    .startupCode                                            : {} palign( 8 ) > R5F_TCMB0
    .startupData                                            : {} palign( 8 ), type = NOINIT > R5F_TCMB0

/* Code sections. */
    GROUP LOAD_START( lnkFlashStartAddr ), LOAD_END( lnkFlashEndAddr )
    {
        .KERNEL_FUNCTION LOAD_START( lnkKernelFuncStartAddr ),
                         LOAD_END( lnkKernelFuncEndAddr ) : {} align( 0x8000 )
    } > OCMC_RAM_SCISERVER

    .unpriv_flash palign( 32 ) :
    {
        *(.text)
    } > OCMC_RAM_SCISERVER

    .rodatasec align( 32 ) :
    {
        *(.rodata)
    } >>  R5F_TCMB0 | OCMC_RAM_SCISERVER

/* Data sections. */

    .KERNEL_DATA LOAD_START( lnkKernelDataStartAddr ),
                    LOAD_END( lnkKernelDataEndAddr )        : {} palign( 0x800 )  > R5F_TCMB0
    .boardcfg_data                                          : {} palign( 128 ) > OCMC_RAM_SCISERVER

    GROUP  palign( 32 ), LOAD_START( lnkRamStartAddr ), LOAD_END( lnkRamEndAddr )
    {
        .far                                                    : {} align( 4 )
        .sysmem                                                 : {}
        .data                                                   : {} palign( 128 ) 
        .const.devgroup                                         : {*(.const.devgroup*)} align( 4 )
        .bss.devgroup                                           : {*(.bss.devgroup*)} align( 4 )

    } > OCMC_RAM_SCISERVER

    /* Due to creation of new task, size of OCMC_RAM_SCISERVER is not sufficient for Data sections
     * Since .bss is a no init section, move .bss section from OCMC_RAM_SCISERVER to SBL_USED_OCMC_RAM  */
    .bss                                                    : {} align( 4 )  > SBL_USED_OCMC_RAM

    /* Stack sections. */
    .stack  RUN_START( lnkStacksStartAddr ) : {} align(4) > R5F_TCMA0

    .irqStack                               : {. = . + __IRQ_STACK_SIZE;}   align(4) > R5F_TCMA0
    RUN_START(__IRQ_STACK_START)
    RUN_END(__IRQ_STACK_END)
    .fiqStack                               : {. = . + __FIQ_STACK_SIZE;}   align(4) > R5F_TCMB0
    RUN_START(__FIQ_STACK_START)
    RUN_END(__FIQ_STACK_END)
    .abortStack                             : {. = . + __ABORT_STACK_SIZE;} align(4) > R5F_TCMB0
    RUN_START(__ABORT_STACK_START)
    RUN_END(__ABORT_STACK_END)
    .undStack                               : {. = . + __UND_STACK_SIZE;}   align(4) > R5F_TCMB0
    RUN_START(__UND_STACK_START)
    RUN_END(__UND_STACK_END)
    .svcStack    END( lnkStacksEndAddr )    : {. = . + __SVC_STACK_SIZE;}   align(4) > OCMC_RAM_SCISERVER
    RUN_START(__SVC_STACK_START)
    RUN_END(__SVC_STACK_END)

}
