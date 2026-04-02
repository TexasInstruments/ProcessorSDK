#!/bin/bash
cd ../../../build/
gmake -j diag_ex_wwdt_perm_example_app_clean
gmake -j diag_ex_wwdt_perm_example_app_multicore_clean
gmake -j BOARD=j721e_evm CORE=mcu1_0 BUILD_PROFILE=debug diag_ex_wwdt_perm_example_app
gmake -j BOARD=j721e_evm CORE=mcu2_0 BUILD_PROFILE=debug diag_ex_wwdt_perm_example_app
gmake -j BOARD=j721e_evm CORE=mcu1_0 BUILD_PROFILE=debug diag_ex_wwdt_perm_example_app_multicore
gmake -j diag_ex_wwdt_perm_fifty_example_app_clean
gmake -j diag_ex_wwdt_perm_fifty_example_app_multicore_clean
gmake -j BOARD=j721e_evm CORE=mcu1_0 BUILD_PROFILE=debug diag_ex_wwdt_perm_fifty_example_app
gmake -j BOARD=j721e_evm CORE=mcu2_0 BUILD_PROFILE=debug diag_ex_wwdt_perm_fifty_example_app
gmake -j BOARD=j721e_evm CORE=mcu1_0 BUILD_PROFILE=debug diag_ex_wwdt_perm_fifty_example_app_multicore
gmake -j diag_ex_wwdt_early_example_app_clean
gmake -j diag_ex_wwdt_early_example_app_multicore_clean
gmake -j BOARD=j721e_evm CORE=mcu1_0 BUILD_PROFILE=debug diag_ex_wwdt_early_example_app
gmake -j BOARD=j721e_evm CORE=mcu2_0 BUILD_PROFILE=debug diag_ex_wwdt_early_example_app
gmake -j BOARD=j721e_evm CORE=mcu1_0 BUILD_PROFILE=debug diag_ex_wwdt_early_example_app_multicore
gmake -j diag_ex_wwdt_late_example_app_clean
gmake -j diag_ex_wwdt_late_example_app_multicore_clean
gmake -j BOARD=j721e_evm CORE=mcu1_0 BUILD_PROFILE=debug diag_ex_wwdt_late_example_app
gmake -j BOARD=j721e_evm CORE=mcu2_0 BUILD_PROFILE=debug diag_ex_wwdt_late_example_app
gmake -j BOARD=j721e_evm CORE=mcu1_0 BUILD_PROFILE=debug diag_ex_wwdt_late_example_app_multicore
cd ../diag/examples/wwdt_example_app
