
SRCDIR += src/tpr12_evm src/tpr12_evm/include
INCDIR += src/tpr12_evm src/tpr12_evm/include

# Common source files across all platforms and cores
SRCS_COMMON += board_init.c board_mmr.c board_pinmux.c board_pll.c board_clock.c board_utils.c board_info.c board_lld_init.c board_ethernet_config.c TPR12_pinmux_data.c 
