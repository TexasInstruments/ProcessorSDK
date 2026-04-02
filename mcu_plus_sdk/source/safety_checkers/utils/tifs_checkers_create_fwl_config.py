'''
  Copyright (C) 2024 Texas Instruments Incorporated

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

    Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

    Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the
    distribution.

    Neither the name of Texas Instruments Incorporated nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
'''

############################ Imports ############################

import re
import sys
from datetime import datetime

# Maximum number of regions for FWL
FWL_MAX_REGIONS: int = 36

####################### Function Definiton #######################

def extract_fwl_data(input_filename, fwl_dict):
    # Extract desired macros and associated values from the input CSL file
    macro_names = []
    macro_values = []

    with open(input_filename, "r") as input_file:
        # Define regEx based on macros to be selected
        pattern = r"^#define (\S+)\s+\((\S+U)\)$"

        '''
        Parse the file line-by-line. If the current line matches the specified regEx,
        extract information related to each firewall id.
        Ignore the firewall id if it is already present in the macro lists.
        Update the attributes of the fwl_dict using the appropriate macro values extracted.
        '''
        for line in input_file:
            match_line = re.match(pattern, line)
            if match_line:
                csl_macro, macro_value = match_line.groups()
                macro_name = re.split("_ID|_NUM_REGIONS|_NUM_PRIV_IDS_PER_REGION", csl_macro)
                if len(macro_name) == 2:
                    macro_name = macro_name[0]
                    if macro_name not in macro_names and macro_value not in macro_values:
                        if csl_macro.endswith("_ID"):
                            valid_id = int(macro_value[:-1])
                            # Filter out invalid firewall ids
                            if valid_id < 512 or valid_id > 1024:
                                macro_names.append(macro_name)
                                macro_values.append(macro_value)
                                fwl_dict[macro_name] = {}
                                fwl_dict[macro_name]["fwl_id"] = macro_value
                    elif macro_name in macro_names:
                        if csl_macro.endswith("_NUM_REGIONS"):
                            fwl_dict[macro_name]["num_regions"] =  macro_value
                            fwl_dict[macro_name]["max_num_regions"] =  macro_value
                            fwl_dict[macro_name]["registers"] = ['0x0U'] * 8

def print_fwl_data(output_filename, fwl_dict):
    with open(output_filename, "w") as output_file:
        # Write copyright to file
        writeBanner(output_file)

        # Write auto-generated comment along with timestamp
        output_file.write("\n/* \n * Auto-generated cfg using the command 'python tifs_checkers_create_fwl_config.py " + args[0] + "' \n * on " + datetime.now().strftime("%d/%m/%Y %H:%M:%S") + " \n */\n\n")

        # Write section headers
        output_file.write("#ifndef TIFS_CHECKERS_FWL_CONFIG_H_\n")
        output_file.write("#define TIFS_CHECKERS_FWL_CONFIG_H_\n\n")
        output_file.write("/* ========================================================================== */\n")
        output_file.write("/*                             Include Files                                  */\n")
        output_file.write("/* ========================================================================== */\n\n")
        output_file.write("#include <safety_checkers_soc.h>\n\n")
        output_file.write("#ifdef __cplusplus\n")
        output_file.write("extern \"C\" {\n")
        output_file.write("#endif\n\n")
        output_file.write("/* ========================================================================== */\n")
        output_file.write("/*                           Macros & Typedefs                                */\n")
        output_file.write("/* ========================================================================== */\n\n")
        output_file.write("/* None */\n\n")
        output_file.write("/* ========================================================================== */\n")
        output_file.write("/*                         Structure Declarations                             */\n")
        output_file.write("/* ========================================================================== */\n\n")
        output_file.write("/* None */\n\n")
        output_file.write("/* ========================================================================== */\n")
        output_file.write("/*                            Global Variables                                */\n")
        output_file.write("/* ========================================================================== */\n\n")

        output_file.write("/* Static firewall configuration to be populated with register values */ \n")
        output_file.write("SafetyCheckers_TifsFwlConfig gSafetyCheckers_TifsFwlConfig[TIFS_CHECKER_FWL_MAX_NUM] = {\n")

        # Iterating over the keys in the dictionary and writing to file along with necessary comments
        for keys in fwl_dict:
            output_file.write("{\n")
            for key,value in fwl_dict[keys].items():
                if key == "fwl_id":
                    output_file.write("\t" + value + "," + "\t/* fwlId */" + "\n")
                elif key == "num_regions":
                    output_file.write("\t" + value + "," + "\t/* numRegions */" + "\n")
                elif key == "max_num_regions":
                    output_file.write("\t" + value + "," + "\t/* maxNumRegions */" + "\n")
                elif type(value) is list:
                    output_file.write("\t{ \t/* Firewall registers for a given region : {controlReg, privId0, privId1, privId2, startAddrLow, startAddrHigh, endAddrLow, endAddrHigh} */\n")
                    n = int(fwl_dict[keys]["num_regions"][:-1])
                    for i in range(FWL_MAX_REGIONS):
                        output_file.write("\t\t{")
                        for ele in value:
                            output_file.write("0x0U, ")
                        output_file.seek(output_file.tell()-2)
                        output_file.truncate()
                        output_file.write("},\n")
                    output_file.write("\t},\n")
            output_file.write("},\n")
        output_file.seek(output_file.tell()-2)
        output_file.truncate()
        output_file.write("\n};")

        output_file.write("\n\n#ifdef __cplusplus\n")
        output_file.write("}\n")
        output_file.write("#endif\n\n")
        output_file.write("#endif  /* #ifndef TIFS_CHECKERS_FWL_CONFIG_H_ */")

def writeBanner(file):
    # TI copyright
    file.write('/*\n')
    file.write(' * Copyright (C) 2024 Texas Instruments Incorporated\n')
    file.write(' *  \n')
    file.write(' *  Redistribution and use in source and binary forms, with or without \n')
    file.write(' *  modification, are permitted provided that the following conditions\n')
    file.write(' *  are met:\n')
    file.write(' *\n')
    file.write(' *    Redistributions of source code must retain the above copyright\n')
    file.write(' *    notice, this list of conditions and the following disclaimer.\n')
    file.write(' *\n')
    file.write(' *    Redistributions in binary form must reproduce the above copyright\n')
    file.write(' *    notice, this list of conditions and the following disclaimer in the\n')
    file.write(' *    documentation and/or other materials provided with the\n')
    file.write(' *    distribution.\n')
    file.write(' *\n')
    file.write(' *    Neither the name of Texas Instruments Incorporated nor the names of\n')
    file.write(' *    its contributors may be used to endorse or promote products derived\n')
    file.write(' *    from this software without specific prior written permission.\n')
    file.write(' *\n')
    file.write(' *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \n')
    file.write(' *  \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT \n')
    file.write(' *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR \n')
    file.write(' *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT \n')
    file.write(' *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, \n')
    file.write(' *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT \n')
    file.write(' *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,\n')
    file.write(' *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY\n')
    file.write(' *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT\n')
    file.write(' *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE\n')
    file.write(' *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n')
    file.write(' */\n')


############################ Main ############################

def main(args):
    # Check if sufficient input args are present
    if len(args) == 1:
        '''
        input_filename : Path to input CSL file
        output_filename : Path to store the output .h file
        '''
        if args[0] == 'am62x':
            input_filename = "source/drivers/hw_include/am62x/csl_soc_firewalls.h"
            output_filename = "source/safety_checkers/examples/tifs_checkers_app/soc/am62x/tifs_checkers_fwl_config.h"
        elif args[0] == 'am62ax':
            input_filename = "source/drivers/hw_include/am62ax/csl_soc_firewalls.h"
            output_filename = "source/safety_checkers/examples/tifs_checkers_app/soc/am62ax/tifs_checkers_fwl_config.h"
        elif args[0] == 'am62px':
            input_filename = "source/drivers/hw_include/am62px/csl_soc_firewalls.h"
            output_filename = "source/safety_checkers/examples/tifs_checkers_app/soc/am62px/tifs_checkers_fwl_config.h"
        elif args[0] == 'j722s':
            input_filename = "source/drivers/hw_include/j722s/csl_soc_firewalls.h"
            output_filename = "source/safety_checkers/examples/tifs_checkers_app/soc/j722s/tifs_checkers_fwl_config.h"
        elif args[0] == 'j784s4':
            input_filename = "packages/ti/csl/soc/j784s4/src/csl_soc_firewalls.h"
            output_filename = "packages/ti/safety_checkers/examples/tifs_checkers_app/soc/j784s4/tifs_checkers_fwl_config.h"
        elif args[0] == 'j721e':
            input_filename = "packages/ti/csl/soc/j721e/src/csl_soc_firewalls.h"
            output_filename = "packages/ti/safety_checkers/examples/tifs_checkers_app/soc/j721e/tifs_checkers_fwl_config.h"
        elif args[0] == 'j7200':
            input_filename = "packages/ti/csl/soc/j7200/src/csl_soc_firewalls.h"
            output_filename = "packages/ti/safety_checkers/examples/tifs_checkers_app/soc/j7200/tifs_checkers_fwl_config.h"
        elif args[0] == 'j721s2':
            input_filename = "packages/ti/csl/soc/j721s2/src/csl_soc_firewalls.h"
            output_filename = "packages/ti/safety_checkers/examples/tifs_checkers_app/soc/j721s2/tifs_checkers_fwl_config.h"
        else:
            # Print error message in case the input arg doesn't match the list of supported SoCs
            print("Unsupported SoC!")
            print("List of supported SoCs : am62x, am62ax, am62px, j722s, j784s4, j721e, j7200, j721s2")
            exit()

        # Create the structure of the nested dictionary along with its attributes
        fwl_dict = {}
        fwl_dict["dummy"] = '0'
        fwl_attrib_dict = {"fwl_id": 0, "num_regions": 0, "max_num_regions": 0, "registers":['0x0']}
        fwl_dict["dummy"] = fwl_attrib_dict
        del fwl_dict["dummy"]

        # Extract data from input CSL file
        extract_fwl_data(input_filename, fwl_dict)

        # Number of valid firewall entries
        num_entries = len(fwl_dict)

        # Write the firewall data into an output file
        print_fwl_data(output_filename, fwl_dict)
    else:
        print("Usage: python tifs_checkers_create_fwl_config.py <soc>")
        print("<soc> : am62x, am62ax, am62px, j722s, j784s4, j721e, j7200, j721s2")

if __name__ == "__main__":
    args = sys.argv[1:]
    main(args)
