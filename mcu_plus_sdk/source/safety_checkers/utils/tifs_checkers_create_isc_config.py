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

# Maximum number of regions for CBASS ISC
ISC_CBASS_MAX_REGIONS: int = 32
# Maximum number of regions for CC ISC
ISC_CC_MAX_REGIONS: int = 255
# Maximum number of regions for RA ISC
ISC_RA_MAX_REGIONS: int = 1024


####################### Function Definiton #######################

def extract_isc_data(input_filename, isc_dict_cbass, isc_dict_cc, isc_dict_ra):
    # Extract desired macros and associated values from the input CSL file
    macro_names = []
    macro_values = []
    flag = 1

    with open(input_filename, "r") as input_file:
        # Define regEx based on macros to be selected
        pattern = r"^#define (\S+)\s+\((\S+U)\)$"

        '''
        Parse the file line-by-line. If the current line matches the specified regEx,
        extract information related to each firewall id.
        Ignore the firewall id if it is already present in the macro lists.
        Update the attributes of the isc_dict_cbass using the appropriate macro values extracted.
        '''
        for line in input_file:
            if "_CORE_TYPE" in line:
                flag = 2
            match_line = re.match(pattern, line)
            if match_line:
                csl_macro, macro_value = match_line.groups()
                macro_name = re.split("_TYPE|_ID|_DEFAULT_PRIV_ID|_REGION_COUNT", csl_macro)
                if len(macro_name) == 2:
                    macro_name = macro_name[0]
                    if macro_name not in macro_names and macro_value not in macro_values:
                        if csl_macro.endswith("T_ID") or csl_macro.endswith("DMA_ID") or csl_macro.endswith("0_ID") \
                        or csl_macro.endswith("1_ID") or csl_macro.endswith("E_ID") or csl_macro.endswith("RING_ID") \
                        or csl_macro.endswith("RMST_ID") or csl_macro.endswith("_RMST_ID") :
                            valid_id = int(macro_value[:-1])
                            # Filter out invalid firewall ids
                            if valid_id < 180143985 or valid_id > 1:
                                macro_names.append(macro_name)
                                macro_values.append(macro_value)
                                if((macro_value == '128U') or (macro_value == '448U')):
                                    isc_dict_ra[macro_name] = {}
                                    isc_dict_ra[macro_name]["isc_id"] = macro_value
                                    flag = 3
                                if (flag == 1):
                                    isc_dict_cbass[macro_name] = {}
                                    isc_dict_cbass[macro_name]["isc_id"] = macro_value
                                elif (flag == 2):
                                    isc_dict_cc[macro_name] = {}
                                    isc_dict_cc[macro_name]["isc_id"] = macro_value
                    elif macro_name in macro_names:
                        if csl_macro.endswith("_REGION_COUNT"):
                            if (flag == 1):
                                isc_dict_cbass[macro_name]["num_regions"] =  macro_value
                                isc_dict_cbass[macro_name]["max_num_regions"] =  macro_value
                                isc_dict_cbass[macro_name]["registers"] = ['0x0U'] * 6
                                isc_dict_cbass[macro_name]["rcr"] = '0x0U'
                            elif (flag == 2):
                                isc_dict_cc[macro_name]["num_regions"] =  macro_value
                                isc_dict_cc[macro_name]["max_num_regions"] =  macro_value
                                isc_dict_cc[macro_name]["registers"] = ['0x0U'] * 2
                            elif (flag == 3):
                                isc_dict_ra[macro_name]["num_regions"] =  macro_value
                                isc_dict_ra[macro_name]["max_num_regions"] =  macro_value
                                isc_dict_ra[macro_name]["registers"] = ['0x0U'] * 2
                                flag = 1

isc_ccid = ['192U','193U','33792U','33800U','33824U','34816U','34848U','35072U','35104U','33808U', \
            '33816U','34048U','34056U','34064U','34072U','34080U','35328U','35360U','35584U', \
            '35616U','37888U','37920U','38912U','38944U']
isc_cc_offset_id = ['0x8000U','0x8400U','0x8000U','0x8008U','0x8020U','0x8400U','0x8420U','0x8500U', \
            '0x8520U','0x8010U','0x8018U','0x8100U','0x8108U','0x8110U','0x8118U','0x8120U','0x8600U',\
            '0x8620U','0x8700U','0x8720U','0x9000U','0x9020U','0x9400U','0x9420U']

def print_isc_data(output_filename, isc_dict_cbass, isc_dict_cc, isc_dict_ra):
    with open(output_filename, "w") as output_file:
        # Write copyright to file
        writeBanner(output_file)

        # Write auto-generated comment along with timestamp
        output_file.write("\n/* \n * Auto-generated cfg using the command 'python tifs_checkers_create_isc_config.py " + args[0] + "' \n * on " + datetime.now().strftime("%d/%m/%Y %H:%M:%S") + " \n */\n\n")

        # Write section headers
        output_file.write("#ifndef TIFS_CHECKERS_ISC_CONFIG_H_\n")
        output_file.write("#define TIFS_CHECKERS_ISC_CONFIG_H_\n\n")
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

        output_file.write("/* Static ISC configuration to be populated with register values */ \n")
        output_file.write("SafetyCheckers_TifsIscCbassConfig gSafetyCheckers_TifsIscCbassConfig[] = {\n")

        # Iterating over the keys in the dictionary and writing to file along with necessary comments
        for keys in isc_dict_cbass:
            output_file.write("{\n")
            for key,value in isc_dict_cbass[keys].items():
                if key == "isc_id":
                    output_file.write("\t" + value + "," + "\t/* iscId */" + "\n")
                elif key == "num_regions":
                    output_file.write("\t" + value + "," + "\t/* numRegions */" + "\n")
                elif key == "max_num_regions":
                    output_file.write("\t" + value + "," + "\t/* maxNumRegions */" + "\n")
                elif type(value) is list:
                    output_file.write("\t{ \t/* ISC registers for a given region : {controlReg0, controlReg1, startAddrLow, startAddrHigh, endAddrLow, endAddrHigh} */\n")
                    n = int(isc_dict_cbass[keys]["num_regions"][:-1])
                    for i in range(ISC_CBASS_MAX_REGIONS):
                        output_file.write("\t\t{")
                        for ele in value:
                            output_file.write("0x0U, ")
                        output_file.seek(output_file.tell()-2)
                        output_file.truncate()
                        output_file.write("},\n")
                    output_file.write("\t},\n")
                elif key == "rcr":
                    output_file.write("\t" + value + "," + "\t/* RCR */" + "\n")
            output_file.write("},\n")
        output_file.seek(output_file.tell()-2)
        output_file.truncate()
        output_file.write("\n};")

        if (len(isc_dict_cc) != 0):
            output_file.write("\n\nSafetyCheckers_TifsIscCcConfig gSafetyCheckers_TifsIscCcConfig[] = { \n")

            # Iterating over the keys in the dictionary and writing to file along with necessary comments
            for keys in isc_dict_cc:
                output_file.write("{\n")
                for key,value in isc_dict_cc[keys].items():
                    if key == "isc_id":
                        if value in isc_ccid:
                            output_file.write("\t" + isc_cc_offset_id[isc_ccid.index(value)] + "," + "\t/* iscIdOffset */" + "\n")
                        else:
                            output_file.write("\t" + value + "," + "\t/* iscIdOffset */" + "\n")
                    elif key == "num_regions":
                        output_file.write("\t" + value + "," + "\t/* numRegions */" + "\n")
                    elif key == "max_num_regions":
                        output_file.write("\t" + value + "," + "\t/* maxNumRegions */" + "\n")
                    elif type(value) is list:
                        output_file.write("\t{ \t/* ISC registers for a given region : {privId, lock} */\n")
                        n = int(isc_dict_cc[keys]["num_regions"][:-1])
                        for i in range(ISC_CC_MAX_REGIONS):
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

        if (len(isc_dict_ra) != 0):
            output_file.write("\n\nSafetyCheckers_TifsIscRaConfig gSafetyCheckers_TifsIscRaConfig[] = { \n")

            # Iterating over the keys in the dictionary and writing to file along with necessary comments
            for keys in isc_dict_ra:
                output_file.write("{\n")
                for key,value in isc_dict_ra[keys].items():
                    if key == "isc_id":
                        output_file.write("\t" + value + "," + "\t/* iscId */" + "\n")
                    elif key == "num_regions":
                        output_file.write("\t" + value + "," + "\t/* numRegions */" + "\n")
                    elif key == "max_num_regions":
                        output_file.write("\t" + value + "," + "\t/* maxNumRegions */" + "\n")
                    elif type(value) is list:
                        output_file.write("\t{ \t/* ISC registers for a given region : {controlReg1, controlReg2} */\n")
                        n = int(isc_dict_ra[keys]["num_regions"][:-1])
                        for i in range(ISC_RA_MAX_REGIONS):
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
        output_file.write("#endif  /* #ifndef TIFS_CHECKERS_ISC_CONFIG_H_ */")

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
            input_filename = "source/drivers/hw_include/am62x/csl_soc_isc.h"
            output_filename = "source/safety_checkers/examples/tifs_checkers_app/soc/am62x/tifs_checkers_isc_config.h"
        elif args[0] == 'am62ax':
            input_filename = "source/drivers/hw_include/am62ax/csl_soc_isc.h"
            output_filename = "source/safety_checkers/examples/tifs_checkers_app/soc/am62ax/tifs_checkers_isc_config.h"
        elif args[0] == 'am62px':
            input_filename = "source/drivers/hw_include/am62px/csl_soc_isc.h"
            output_filename = "source/safety_checkers/examples/tifs_checkers_app/soc/am62px/tifs_checkers_isc_config.h"
        elif args[0] == 'j722s':
            input_filename = "source/drivers/hw_include/j722s/csl_soc_isc.h"
            output_filename = "source/safety_checkers/examples/tifs_checkers_app/soc/j722s/tifs_checkers_isc_config.h"
        elif args[0] == 'j784s4':
            input_filename = "packages/ti/csl/soc/j784s4/src/csl_soc_isc.h"
            output_filename = "packages/ti/safety_checkers/examples/tifs_checkers_app/soc/j784s4/tifs_checkers_isc_config.h"
        elif args[0] == 'j721e':
            input_filename = "packages/ti/csl/soc/j721e/src/csl_soc_isc.h"
            output_filename = "packages/ti/safety_checkers/examples/tifs_checkers_app/soc/j721e/tifs_checkers_isc_config.h"
        elif args[0] == 'j7200':
            input_filename = "packages/ti/csl/soc/j7200/src/csl_soc_isc.h"
            output_filename = "packages/ti/safety_checkers/examples/tifs_checkers_app/soc/j7200/tifs_checkers_isc_config.h"
        elif args[0] == 'j721s2':
            input_filename = "packages/ti/csl/soc/j721s2/src/csl_soc_isc.h"
            output_filename = "packages/ti/safety_checkers/examples/tifs_checkers_app/soc/j721s2/tifs_checkers_isc_config.h"
        else:
            # Print error message in case the input arg doesn't match the list of supported SoCs
            print("Unsupported SoC!")
            print("List of supported SoCs : am62x, am62ax, am62px, j722s, j784s4, j721e, j7200, j721s2")
            exit()

        # Create the structure of the nested dictionary along with its attributes
        isc_dict_cbass = {}
        isc_dict_cbass["dummy"] = '0'
        isc_attrib_dict = {"isc_id": 0, "num_regions": 0, "max_num_regions": 0, "registers":['0x0']}
        isc_dict_cbass["dummy"] = isc_attrib_dict
        del isc_dict_cbass["dummy"]

        isc_dict_cc = {}
        isc_dict_cc["dummy"] = '0'
        isc_dict_cc["dummy"] = isc_attrib_dict
        del isc_dict_cc["dummy"]

        isc_dict_ra = {}
        isc_dict_ra["dummy"] = '0'
        isc_dict_ra["dummy"] = isc_attrib_dict
        del isc_dict_ra["dummy"]

        # Extract data from input CSL file
        extract_isc_data(input_filename, isc_dict_cbass, isc_dict_cc, isc_dict_ra)

        # Write the firewall data into an output file
        print_isc_data(output_filename, isc_dict_cbass, isc_dict_cc, isc_dict_ra)
    else:
        print("Usage: python tifs_checkers_create_isc_config.py <soc>")
        print("<soc> : am62x, am62ax, am62px, j722s, j784s4, j721e, j7200, j721s2")

if __name__ == "__main__":
    args = sys.argv[1:]
    main(args)
