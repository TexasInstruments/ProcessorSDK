
SRCDIR += src/am64x_evm src/am64x_evm/include
INCDIR += src/am64x_evm src/am64x_evm/include

# Common source files across all platforms and cores
SRCS_COMMON += board_init.c board_lld_init.c board_clock.c board_mmr.c board_pll.c board_utils.c board_i2c_io_exp.c
SRCS_COMMON += board_ddr.c board_info.c board_ethernet_config.c board_pinmux.c board_serdes_cfg.c AM64x_pinmux_data.c
SRCS_COMMON += AM64x_pinmux_data_icssMII.c AM64x_pinmux_data_GPMC.c

PACKAGE_SRCS_COMMON = src/am64x_evm/src_files_am64x_evm.mk
