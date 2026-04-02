
SRCDIR += src/awr294x_evm src/awr294x_evm/include
INCDIR += src/awr294x_evm src/awr294x_evm/include

# Common source files across all platforms and cores
SRCS_COMMON += board_init.c board_mmr.c board_pinmux.c board_pll.c board_clock.c board_utils.c board_info.c board_lld_init.c board_ethernet_config.c
PACKAGE_SRCS_COMMON += board_init.c board_mmr.c board_pinmux.c board_pll.c board_clock.c board_utils.c board_info.c board_lld_init.c board_ethernet_config.c
PACKAGE_SRCS_COMMON += AWR294X_pinmux.h src_files_awr294x_evm.mk

#Default Variant is AWR294X_LOP
BOARD_VARIANT ?= AWR294X_ETS

ifeq ($(BOARD_VARIANT), AWR294X_ETS)
SRCS_COMMON += AWR294X_pinmux_data.c
PACKAGE_SRCS_COMMON += AWR294X_pinmux_data.c AWR294X_EVM.syscfg
endif

ifeq ($(BOARD_VARIANT),AWR294X_LOP)
SRCS_COMMON += AWR294X_pinmux_data_lop.c
PACKAGE_SRCS_COMMON += AWR294X_pinmux_data_lop.c AWR294X_EVM_LOP.syscfg
endif

ifeq ($(BOARD_VARIANT),AWR294X_LOP_CZ)
SRCS_COMMON += AWR294X_pinmux_data_lopCZ.c
PACKAGE_SRCS_COMMON += AWR294X_pinmux_data_lopCZ.c AWR294X_EVM_LOP_CZ.syscfg
endif