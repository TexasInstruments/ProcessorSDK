# User Guide for running JTAG Power Analysis Tool with Code Composer Studio (CCS)

This document provides instructions for running JTAG Power Analysis Tool with Code Composer Studio

## Requirements:
-- [CCS20xx platform](https://software-dl.ti.com/ccs/esd/documents/users_guide_ccs_20.1.1)

**Before Running the Script** : Ensure that the absolute path of the main.js is correctly set in the [`file_path`](../../src/main.js#5) variable in main.js. This is required to enable the script to locate and execute the necessary files.

## Running from CCS Scripting Console

This feature allows this script to run on a live CCS debugging session.

1. Open CCS and launch target config
2. Open the Scripting console (view->console->scripting console)
3. Load JS file `main.js` (Load JS File using UI or type `.load <path_to_main.js>`)
4. Connect to WKUP/MCU core (if not connected) and step to the point where you want to run the tool
5. Run the tool\
   `await main(debugServer=ds, soc=<soc>, selector=<selector>, level=<level>)`

   Example \
     `.load /home/dashboardpc/Desktop/pdm_utils/tools/jtag_power_analysis_tool/src/main.js`

     `await main(debugServer=ds, soc='j784s4', selector='pll', level=0)`
6. View the output in the console or saved json file in the [output directory](../user_guide.md/#16-output-formats)

Please refer to the [usage options](#usage-options) section to view the valid arguments that can be passed in the above API calls.

Sample run:

```
js:> await main(debugServer=ds,soc="j784s4", selector="all", level=2)
Debug server initialized
Target Connected: True
┌────────────────────────────────────────────┐
│          PLL Configuration Table           │
├────────────┬────────────┬──────────────────┤
│ Name       │ PLL Status │ VCO Freq(MHz)    │
├────────────┼────────────┼──────────────────┤
│ main_0     │ ENABLED    │ 2000.00          │
├────────────┼────────────┼──────────────────┤
│ main_1     │ ENABLED    │ 1920.00          │
└────────────┴────────────┴──────────────────┘

Session cleared
```

## 1.5.2. Running from terminal with CCS

### Assumption
- If JTAG tool is invoked from terminal, it is expected that no instance of CCS GUI is running.

1. Connect to the SoC using the JTAG interface
2. Run the `main.js` script to generate tool output.
   `main.js` is run via CCS scripting which is located at <ccs_install_dir>/ccs/scripting

    `cd <ccs_install_dir>/ccs/scripting`

    Run `./run.sh <path to main.js> --soc <soc> [--selector all|clock_tree|pll|psc] [--level <0-2>] --ide ccs --arg1 <path to ccxml file of soc>`

    Example \
     `./run.sh /home/dashboardpc/Desktop/pdm_utils/tools/jtag_power_analysis_tool/src/main.js --soc j784s4 --selector all --level 2 --ide ccs --arg1 /home/dashboardpc/ti/CCSTargetConfigurations/j784s4.ccxml`

3. View the output in the console or saved json file in the [output directory](../user_guide.md/#16-output-formats)

Please refer to the [usage options](#usage-options) section to view the valid arguments that can be passed.

## Usage Options

| Args        | Description                                                         |
| ----------- | ----------------------------------------------------------------    |
| --soc <soc> | The [SoC](../user_guide.md/#18-supported-socs) to be used           |
| --selector  | all : runs all tools (default)                                      |
|             | pll : runs pll tool                                                 |
|             | psc : runs psc tool                                                 |
|             | pet : print PET tool IP                                             |
|             | clock_tree: runs clk_tree tool                                      |
|             | junction_temp: print junction temperature(for CCS Debugger only)    |
| --ide       | ccs : runs the tool through CCS                                     |
|             | t32 : runs the tool through Trace32 and OpenOCD                     |
| --level     | [print level](../user_guide.md/#162-console-output) <0-2>           |
| --arg1      | arg1 is the path to ccxml file for soc                              |
|             | [Refer for --arg1](../../src/main.js#41)                            |

1. Get help in CCS Scripting Console:
- Load JS file `main.js` (Load JS File button in console or type `.load <path_to_main.js>`)
- Call help function\
   `help()`

2. Get help in terminal:
   `cd <ccs_install_dir>/ccs/scripting`

   `./run.sh <path_to_main.js> --help`
