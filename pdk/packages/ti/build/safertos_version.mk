################################################################################
# Configure SafeRTOS Package versions for various SOC/ISA
################################################################################

# Update the following to set custom path to SafeRTOS Package.
# Else the default path will be set to $(SDK_INSTALL_PATH)/safertos_$(SOC)_$(ISA)_$(SAFERTOS_VERSION_$(ISA))
# IMPORTANT:
#   Make sure the paths specified below DO NOT have any spaces in them.

SAFERTOS_VERSION_c7x=009-004-230-005-219-004
SAFERTOS_VERSION_r5f=009-005-199-024-219-005
SAFERTOS_VERSION_c66=009-002-201-005-219-002

ifeq ($(BOARD),$(filter $(BOARD), j721e_evm ))
  export SAFERTOS_j721e_r5f_INSTALL_PATH = 
  export SAFERTOS_j721e_c66_INSTALL_PATH = 
  export SAFERTOS_j721e_c7x_INSTALL_PATH = 
endif

ifeq ($(BOARD),$(filter $(BOARD), j7200_evm ))
  export SAFERTOS_j7200_r5f_INSTALL_PATH = 
endif

ifeq ($(BOARD),$(filter $(BOARD), j721s2_evm ))
  export SAFERTOS_j721s2_r5f_INSTALL_PATH = 
  export SAFERTOS_j721s2_c7x_INSTALL_PATH = 
endif

ifeq ($(BOARD),$(filter $(BOARD), j784s4_evm ))
  export SAFERTOS_j784s4_r5f_INSTALL_PATH = 
  export SAFERTOS_j784s4_c7x_INSTALL_PATH =
endif

# ISA based directory extensions used in SafeRTOS Package 
SAFERTOS_ISA_EXT_r5f = 199_TI_CR5
SAFERTOS_ISA_EXT_c66 = 201_C66x
SAFERTOS_ISA_EXT_c7x = 230_C7x
# Compiler based directory extensions used in SafeRTOS Package 
SAFERTOS_COMPILER_EXT_r5f = 024_Clang
SAFERTOS_COMPILER_EXT_c66 = 005_TI_CGT
SAFERTOS_COMPILER_EXT_c7x = 005_TI_CGT

TDA_EXT_j721s2 = TDA4VL
TDA_EXT_j721e = TDA4VM
TDA_EXT_j784s4 = TDA4VH
