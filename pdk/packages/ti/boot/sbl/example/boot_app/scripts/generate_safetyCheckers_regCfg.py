import serial
import os
import sys

# Open the serial port with a timeout of 1 second
ser = serial.Serial('/dev/ttyUSB1', baudrate=115200, timeout=1)

def get_files_path(board_name):
    # Dictionary to store board names and corresponding file paths
    board_details = {
        "j721e_evm": {
            "header_file": "../soc/j721e/safety_checkers_regcfg.h",
            "log_file": "../soc/j721e/BootApp_log.txt",
        },
        "j7200_evm": {
            "header_file": "../soc/j7200/safety_checkers_regcfg.h",
            "log_file": "../soc/j7200/BootApp_log.txt",
        },
        "j721s2_evm": {
            "header_file": "../soc/j721s2/safety_checkers_regcfg.h",
            "log_file": "../soc/j721s2/BootApp_log.txt",
        },
        "j784s4_evm": {
            "header_file": "../soc/j784s4/safety_checkers_regcfg.h",
            "log_file": "../soc/j784s4/BootApp_log.txt",
        }
    }

    if board_name not in board_details:
        print("Invalid board name!!!")
        print("Board supported : j721e_evm, j7200_evm, j721s2_evm, j784s4_evm")
        exit()

    return board_details[board_name]["header_file"], board_details[board_name]["log_file"]

def start_header_banners(header_file):
    header_file.write("/*\n")
    header_file.write("*\n")
    header_file.write("* Copyright (c) 2024 Texas Instruments Incorporated\n")
    header_file.write("*\n")
    header_file.write("* All rights reserved not granted herein.\n")
    header_file.write("*\n")
    header_file.write("* Limited License.\n")
    header_file.write("*\n")
    header_file.write("* Texas Instruments Incorporated grants a world-wide, royalty-free, non-exclusive\n")
    header_file.write("* license under copyrights and patents it now or hereafter owns or controls to make,\n")
    header_file.write("* have made, use, import, offer to sell and sell (\"Utilize\") this software subject to the\n")
    header_file.write("* terms herein.  With respect to the foregoing patent license, such license is granted\n")
    header_file.write("* solely to the extent that any such patent is necessary to Utilize the software alone.\n")
    header_file.write("* The patent license shall not apply to any combinations which include this software,\n")
    header_file.write("* other than combinations with devices manufactured by or for TI (\"TI Devices\").\n")
    header_file.write("* No hardware patent is licensed hereunder.\n")
    header_file.write("*\n")
    header_file.write("* Redistributions must preserve existing copyright notices and reproduce this license\n")
    header_file.write("* (including the above copyright notice and the disclaimer and (if applicable) source\n")
    header_file.write("* code license limitations below) in the documentation and/or other materials provided\n")
    header_file.write("* with the distribution\n")
    header_file.write("*\n")
    header_file.write("* Redistribution and use in binary form, without modification, are permitted provided\n")
    header_file.write("* that the following conditions are met:\n")
    header_file.write("*\n")
    header_file.write("* *       No reverse engineering, decompilation, or disassembly of this software is\n")
    header_file.write("* permitted with respect to any software provided in binary form.\n")
    header_file.write("*\n")
    header_file.write("* *       any redistribution and use are licensed by TI for use only with TI Devices.\n")
    header_file.write("*\n")
    header_file.write("* *       Nothing shall obligate TI to provide you with source code for the software\n")
    header_file.write("* licensed and provided to you in object code.\n")
    header_file.write("*\n")
    header_file.write("* If software source code is provided to you, modification and redistribution of the\n")
    header_file.write("* source code are permitted provided that the following conditions are met:\n")
    header_file.write("*\n")
    header_file.write("* *       any redistribution and use of the source code, including any resulting derivative\n")
    header_file.write("* works, are licensed by TI for use only with TI Devices.\n")
    header_file.write("*\n")
    header_file.write("* *       any redistribution and use of any object code compiled from the source code\n")
    header_file.write("* and any resulting derivative works, are licensed by TI for use only with TI Devices.\n")
    header_file.write("*\n")
    header_file.write("* Neither the name of Texas Instruments Incorporated nor the names of its suppliers\n")
    header_file.write("*\n")
    header_file.write("* may be used to endorse or promote products derived from this software without\n")
    header_file.write("* specific prior written permission.\n")
    header_file.write("*\n")
    header_file.write("* DISCLAIMER.\n")
    header_file.write("*\n")
    header_file.write("* THIS SOFTWARE IS PROVIDED BY TI AND TI'S LICENSORS \"AS IS\" AND ANY EXPRESS\n")
    header_file.write("* OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES\n")
    header_file.write("* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.\n")
    header_file.write("* IN NO EVENT SHALL TI AND TI'S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT,\n")
    header_file.write("* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,\n")
    header_file.write("* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,\n")
    header_file.write("* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY\n")
    header_file.write("* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE\n")
    header_file.write("* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED\n")
    header_file.write("* OF THE POSSIBILITY OF SUCH DAMAGE.\n")
    header_file.write("*\n")
    header_file.write("*/\n\n")
    header_file.write("/**\n")
    header_file.write(" *  \\file     safety_checkers_regcfg.h\n")
    header_file.write(" *\n")
    header_file.write(" *  \\brief    This file contains PM-RM safety checkers register configuration data.\n")
    header_file.write(" *\n")
    header_file.write(" */\n\n")
    header_file.write("#ifndef SAFETY_CHECKERS_REGCFG_H_\n")
    header_file.write("#define SAFETY_CHECKERS_REGCFG_H_\n\n")
    header_file.write("/* ========================================================================== */\n")
    header_file.write("/*                             Include Files                                  */\n")
    header_file.write("/* ========================================================================== */\n")
    header_file.write("#include <safety_checkers_soc.h>\n")
    header_file.write("#include <safety_checkers_tifs.h>\n\n")
    header_file.write("#ifdef __cplusplus\n")
    header_file.write("extern \"C\" {\n")
    header_file.write("#endif\n\n")
    header_file.write("/* ========================================================================== */\n")
    header_file.write("/*                           Macros & Typedefs                                */\n")
    header_file.write("/* ========================================================================== */\n\n")
    header_file.write("/* None */\n\n")
    header_file.write("/* ========================================================================== */\n")
    header_file.write("/*                         Structure Declarations                             */\n")
    header_file.write("/* ========================================================================== */\n\n")
    header_file.write("/* None */\n\n")
    header_file.write("/* ========================================================================== */\n")
    header_file.write("/*                  Internal/Private Function Declarations                    */\n")
    header_file.write("/* ========================================================================== */\n\n")
    header_file.write("/* None */\n\n")
    header_file.write("/* ========================================================================== */\n")
    header_file.write("/*                            Global Variables                                */\n")
    header_file.write("/* ========================================================================== */\n\n")

def end_header_banners(header_file):
    header_file.write("/* ========================================================================== */\n")
    header_file.write("/*                          Function Declarations                             */\n")
    header_file.write("/* ========================================================================== */\n\n")
    header_file.write("/* None */\n\n")
    header_file.write("/* ========================================================================== */\n")
    header_file.write("/*                       Static Function Definitions                          */\n")
    header_file.write("/* ========================================================================== */\n\n")
    header_file.write("/* None */\n\n")
    header_file.write("#ifdef __cplusplus\n")
    header_file.write("}\n")
    header_file.write("#endif\n\n")
    header_file.write("#endif /* #ifndef SAFETY_CHECKERS_REGCFG_H_ */\n")

def process_serialData(board_name):
    header_file_path, log_file_path = get_files_path(board_name)
    log_started = False

    try:
        if ser.isOpen():
            print("Serial port opened")

            # Create files if they don't exist
            os.makedirs(os.path.dirname(header_file_path), exist_ok=True)
            os.makedirs(os.path.dirname(log_file_path), exist_ok=True)

            # Open the header file and log file in write mode
            with open(header_file_path, "w") as header_file, open(log_file_path, "w") as log_file:

                start_header_banners(header_file) # Start of header file
            
                # Read data starts
                while True:
                    data = ser.readline()
                    if data:
                        print(data)
                        decoded_data = data.decode()
                        # Check if register config is received
                        if "static uintptr_t  pm_pscRegCfg[] = {" in decoded_data:
                            log_started = True

                        # Save logging data to log file
                        if not log_started:
                            log_file.write(decoded_data)

                        # Check if "finished" is received to break the loop
                        if "finished" in decoded_data:
                            log_file.write(decoded_data)
                            break

                        if log_started:
                            header_file.write(decoded_data.rstrip() + "\n")

                end_header_banners(header_file)  # End of header file
                print("Writing to the files are successful")
        else:
            print("Error in opening Serial port")
    except Exception as e:
        print("Error reading from serial port:", e)
    finally:
        # Close the serial port
        ser.close()

if __name__ == "__main__":
    args = sys.argv[1:]
    if len(args) == 1:
        board_name = args[0].lower()
        process_serialData(board_name)
    else:
        print("Usage: python script.py <board_name>")
        print("<board_name> : j721e_evm, j7200_evm, j721s2_evm, j784s4_evm")
