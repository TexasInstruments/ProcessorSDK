
SECTIONS
{
    .sbl_mcu_2_1_resetvector 	: {} palign(8) 		> MSMC3_MCU2_CPU1
    .text 	                    : {} palign(8) 		> MSMC3_MCU2_CPU1
    .rodata 	                : {} palign(8) 		> MSMC3_MCU2_CPU1
}

