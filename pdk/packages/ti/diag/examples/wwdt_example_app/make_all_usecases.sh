#!/bin/bash
cd ../../../build/
make -j diag_ex_wwdt_perm_example_app_clean
make -j diag_ex_wwdt_perm_example_app_multicore_clean
make -j BOARD=j721e_evm CORE=mcu1_0 BUILD_PROFILE=debug diag_ex_wwdt_perm_example_app
make -j BOARD=j721e_evm CORE=mcu2_0 BUILD_PROFILE=debug diag_ex_wwdt_perm_example_app
make -j BOARD=j721e_evm CORE=mcu1_0 BUILD_PROFILE=debug diag_ex_wwdt_perm_example_app_multicore
make -j diag_ex_wwdt_perm_fifty_example_app_clean
make -j diag_ex_wwdt_perm_fifty_example_app_multicore_clean
make -j BOARD=j721e_evm CORE=mcu1_0 BUILD_PROFILE=debug diag_ex_wwdt_perm_fifty_example_app
make -j BOARD=j721e_evm CORE=mcu2_0 BUILD_PROFILE=debug diag_ex_wwdt_perm_fifty_example_app
make -j BOARD=j721e_evm CORE=mcu1_0 BUILD_PROFILE=debug diag_ex_wwdt_perm_fifty_example_app_multicore
make -j diag_ex_wwdt_early_example_app_clean
make -j diag_ex_wwdt_early_example_app_multicore_clean
make -j BOARD=j721e_evm CORE=mcu1_0 BUILD_PROFILE=debug diag_ex_wwdt_early_example_app
make -j BOARD=j721e_evm CORE=mcu2_0 BUILD_PROFILE=debug diag_ex_wwdt_early_example_app
make -j BOARD=j721e_evm CORE=mcu1_0 BUILD_PROFILE=debug diag_ex_wwdt_early_example_app_multicore
make -j diag_ex_wwdt_late_example_app_clean
make -j diag_ex_wwdt_late_example_app_multicore_clean
make -j BOARD=j721e_evm CORE=mcu1_0 BUILD_PROFILE=debug diag_ex_wwdt_late_example_app
make -j BOARD=j721e_evm CORE=mcu2_0 BUILD_PROFILE=debug diag_ex_wwdt_late_example_app
make -j BOARD=j721e_evm CORE=mcu1_0 BUILD_PROFILE=debug diag_ex_wwdt_late_example_app_multicore
cd ../diag/examples/wwdt_example_app
