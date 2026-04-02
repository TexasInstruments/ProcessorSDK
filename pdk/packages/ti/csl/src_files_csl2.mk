#
# This file is the source file lists for building CSL2 library.
#
#   Copyright (c) Texas Instruments Incorporated 2019
#
#SOC modules
-include $(PDK_CSL_COMP_PATH)/soc/src_files_soc.mk

# Arch modules
-include $(PDK_CSL2_COMP_PATH)/arch/r5/src_files_r5_vim.mk
-include $(PDK_CSL2_COMP_PATH)/arch/r5/src_files_r5_ccmr5.mk
-include $(PDK_CSL2_COMP_PATH)/arch/r5/src_files_r5_mpu.mk
-include $(PDK_CSL2_COMP_PATH)/arch/r5/src_files_r5_pmu.mk
-include $(PDK_CSL2_COMP_PATH)/arch/r5/src_files_r5_hard_err_cache.mk

#IP modules
-include $(PDK_CSL2_COMP_PATH)/src/ip/rat/src_files_rat.mk
-include src/ip/ecc_aggr/src_files_ecc_aggr.mk
-include src/ip/esm/src_files_esm.mk
-include src/ip/timer/src_files_timer.mk
-include src/ip/rti/src_files_rti.mk
-include src/ip/crc/src_files_crc.mk
-include src/ip/cbass/src_files_cbass.mk
-include src/ip/vtm/src_files_vtm.mk
-include src/ip/tog/src_files_tog.mk
-include src/ip/lbist/src_files_lbist.mk
-include src/ip/pbist/src_files_pbist.mk
-include src/ip/pok/src_files_pok.mk

