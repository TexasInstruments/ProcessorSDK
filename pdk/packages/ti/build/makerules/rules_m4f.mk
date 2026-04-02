# Filename: rules_m4f.mk
#
# Make rules for M4F - This file has all the common rules and defines required
#                     for M4F ISA
#
# This file needs to change when:
#     1. Code generation tool chain changes (currently it uses TMS470)
#     2. Internal switches (which are normally not touched) has to change
#     3. XDC specific switches change
#     4. a rule common for R5F ISA has to be added or modified

CGT_ISA = M4F
CGT_EXT = m4f
CGT_PATH = $(TOOLCHAIN_PATH_M4)
include $(MAKERULEDIR)/rules_ti_cgt_arm.mk

# Nothing beyond this point
