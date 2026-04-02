
SRCDIR += src/j784s4_evm src/j784s4_evm/include
INCDIR += src/j784s4_evm src/j784s4_evm/include

# Common source files across all platforms and cores
SRCS_COMMON += board_init.c board_lld_init.c board_clock.c board_mmr.c board_pll.c board_serdes_cfg.c
SRCS_COMMON += board_ddr.c board_info.c board_info_ddr.c board_ethernet_config.c board_utils.c board_control.c board_i2c_io_exp.c board_power.c
SRCS_COMMON += board_pinmux.c J784S4_pinmux_data.c
SRCS_COMMON += board_ddr_prime_dma.c
SRCS_COMMON += board_ecu.c
PACKAGE_SRCS_COMMON = src/j784s4_evm/src_files_j784s4_evm.mk
