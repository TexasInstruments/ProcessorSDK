# User Guide for running JTAG Power Analysis Tool with Trace32 PowerView

This document provides instructions for running JTAG Power Analysis Tool with Trace32 PowerView

## Requirements
-- [Trace32 PowerView for ARM version N.2025.01.xx or above](https://confluence.itg.ti.com/display/J7TDA4xSW/Trace32+Installation) with [openOCD 0.12.0 or above](https://confluence.itg.ti.com/display/PDM/OpenOCD+Guide+for+Installation+and+Building+from+Source)
-- node v22.17.0

## Assumptions
1. If using Trace32 as the debugger, and no instance of trace32 is running (i.e. no Trace32 live debug session is running), then no assumtions are made.

2. If using Trace32 as debugger and a live Trace32 debug session is running (i.e. user is already using Trace32 for other debugging purposes), then the following assumptions apply

- openOCD (if being used) is configured using the same OpenOCD config file as passed in the command line parameters as --arg1 (use [usage options](#usage-options)).
- Trace32 is connected to the target.
- Trace32 Remote API is enabled. Run `SETUP.API.RCL.Enable /Port /RemoteHost ALLOW /Interface UDP` in Trace32 CLI to enable remote API.

**Before Running the Script** : Ensure that the absolute path of the main.js is correctly set in the [`file_path`](../../src/main.js#5) variable in main.js. This is required to enable the script to locate and execute the necessary files.

1. Connect to the SoC using the JTAG interface
2. Make sure the core is not connected through any other debugger, as it will prevent OpenOCD from connecting to the target.
3. Run the `main.js` script to generate tool output.

    Run `node <path to main.js> --soc <soc> [--selector all|clock_tree|pll|psc] [--level <0-2>] --ide t32 --arg1 <path to openOCD board config file> --arg2 <path to Trace32 startup script>`

    Example \
     `node /home/dashboardpc/Desktop/pdm_utils/tools/jtag_power_analysis_tool/src/main.js --soc j784s4 --selector all --level 2 --ide t32 --arg1 /usr/local/share/openocd/scripts/board/ti_j784s4evm.cfg --arg2 /opt/t32/j784s4.cmm`

4. View the output in the console or saved json file in the [output directory](#16-output-formats)
5. [Refer this repository](https://bitbucket.itg.ti.com/projects/KS3PRELINUX/repos/utilities/browse/debugger/t32/openocd) for Trace32 startup scripts.

Please refer to the [usage options](#usage-options) section to view the valid arguments that can be passed.

# Usage Options

| Args        | Description                                                     |
| ----------- | -------------------------------------------------------------   |
| --soc <soc> | The [SoC](../user_guide.md/#18-supported-socs) to be used       |
| --selector  | all : runs all tools (default)                                  |
|             | pll : runs pll tool                                             |
|             | psc : runs psc tool                                             |
|             | pet : print PET tool IP                                         |
|             | clock_tree: runs clk_tree tool                                  |
| --ide       | ccs : runs the tool through CCS                                 |
|             | t32 : runs the tool through Trace32 and OpenOCD                 |
| --level     | [print level](../user_guide.md/#162-console-output) <0-2>       |
| --arg1      | arg1 is the path to openOCD config file                         | 
|             | [Refer for --arg1](../../src/main.js#41)                        | 
| --arg2      | arg2 is the path to Trace32 startup script                      |   
|             | [Refer for --arg2](../../src/main.js#41)                        | 

Run 
   `node <path_to_main.js> --help` 
   
to get help on usage options.