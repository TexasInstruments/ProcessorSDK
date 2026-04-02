#!/usr/bin/python3

"""
/*
 *
 * Copyright (c) 2025 Texas Instruments Incorporated
 *
 * All rights reserved not granted herein.
 *
 * Limited License.
 *
 * Texas Instruments Incorporated grants a world-wide, royalty-free, non-exclusive
 * license under copyrights and patents it now or hereafter owns or controls to make,
 * have made, use, import, offer to sell and sell ("Utilize") this software subject to the
 * terms herein.  With respect to the foregoing patent license, such license is granted
 * solely to the extent that any such patent is necessary to Utilize the software alone.
 * The patent license shall not apply to any combinations which include this software,
 * other than combinations with devices manufactured by or for TI ("TI Devices").
 * No hardware patent is licensed hereunder.
 *
 * Redistributions must preserve existing copyright notices and reproduce this license
 * (including the above copyright notice and the disclaimer and (if applicable) source
 * code license limitations below) in the documentation and/or other materials provided
 * with the distribution
 *
 * Redistribution and use in binary form, without modification, are permitted provided
 * that the following conditions are met:
 *
 * *       No reverse engineering, decompilation, or disassembly of this software is
 * permitted with respect to any software provided in binary form.
 *
 * *       any redistribution and use are licensed by TI for use only with TI Devices.
 *
 * *       Nothing shall obligate TI to provide you with source code for the software
 * licensed and provided to you in object code.
 *
 * If software source code is provided to you, modification and redistribution of the
 * source code are permitted provided that the following conditions are met:
 *
 * *       any redistribution and use of the source code, including any resulting derivative
 * works, are licensed by TI for use only with TI Devices.
 *
 * *       any redistribution and use of any object code compiled from the source code
 * and any resulting derivative works, are licensed by TI for use only with TI Devices.
 *
 * Neither the name of Texas Instruments Incorporated nor the names of its suppliers
 *
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * DISCLAIMER.
 *
 * THIS SOFTWARE IS PROVIDED BY TI AND TI'S LICENSORS "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL TI AND TI'S LICENSORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
 """

import os

# ------------------------------------------------------------------------------
# create driver files

# ------------------------------------------------------------------------------
# sensor_config.h
# - sensor regsiter file must be in 2 columns of hex values (address and value)
#   e.g.,
#     0107  01
#     4d5a  1c
#     ...
def update_sensor_config(user_input):
    """
    Updates the sensor configuration file based on the user input.

    Args:
        user_input (dict): A dictionary containing the user input.

    Returns:
        None.
    """
    templ = 'SENSOR_NAME_LOWERCASE_sensor_config.h'
    ftext = read_file(os.path.join(user_input['SENSOR_TEMPLATE_DIR'], templ))

    rep_list = ['SENSOR_NAME_UPPERCASE',
                'SENSOR_NAME_LOWERCASE',
                ]
    for k in rep_list:
        ftext = ftext.replace(k, user_input[k])

    import re
    rtext = read_file(user_input['SENSOR_INIT_FILE'])
    rtext = re.sub(r'(\w+)\s+(\w+)\s+(\w+)', r'    {0x\1, 0x\2, 0x\3},', rtext)

    ftext = ftext.replace('TO_FILL_WITH_SENSOR_SETTINGS_FROM_SENSOR_VENDOR', rtext)
    
    fout = templ.replace('SENSOR_NAME_LOWERCASE', user_input['SENSOR_NAME_LOWERCASE'])
    write_file(user_input['SENSOR_OUTPUT_DIR'], fout, ftext)
    return

# ------------------------------------------------------------------------------
# serdes_cofig.h
# - check TODO in template
def update_serdes_config(user_input):
    """
    Updates the serdes configuration file based on the user input.

    Args:
        user_input (dict): A dictionary containing the user input.

    Returns:
        None.
    """
    templ = 'SENSOR_NAME_LOWERCASE_serdes_config.h'
    ftext = read_file(os.path.join(user_input['SENSOR_TEMPLATE_DIR'], templ)) 

    rep_list = ['SENSOR_NAME_UPPERCASE',
                'SENSOR_NAME_LOWERCASE',
                'SENSOR_8_BIT_I2C_ADDRESS',
                'SERIALIZER_8_BIT_I2C_ADDRESS',
                ]
    for k in rep_list:
        ftext = ftext.replace(k, user_input[k])

    # ----------------------------------------
    # 3.3V or 1.8V
    val = int(user_input['SER_CFG_I2C_STRAP']) + 0x72
    ftext = ftext.replace('SER_CFG_I2C_STRAP', hex(val))

    # GPIO
    ftext = ftext.replace('SER_LOCAL_GPIO_DATA', user_input['SER_LOCAL_GPIO_DATA'])
    ftext = ftext.replace('SER_GPIO_INPUT_CTRL', user_input['SER_GPIO_INPUT_CTRL'])

    # clock and divider
    if 'SENSOR_INPUT_CLK' in user_input:
        # clk (MHz) / 1000 = M / N
        import math
        clk = int(user_input['SENSOR_INPUT_CLK'])
        hs_clk = 2  # 2 for 4x
        g = math.gcd(clk, 1000)
        m = clk // g
        n = 1000 // g

        ftext = ftext.replace('SER_HS_CLK_DIV_M', hex(hs_clk * 32 + m))
        ftext = ftext.replace('SER_DIV_N_VAL', hex(n))

    else:
        val = int(user_input['SER_HS_CLK_DIV']) * 32 + int(user_input['SER_DIV_M_VAL'])
        ftext = ftext.replace('SER_HS_CLK_DIV_M', hex(val))
        val = int(user_input['SER_DIV_N_VAL'])
        ftext = ftext.replace('SER_DIV_N_VAL', hex(val))

    # ----------------------------------------

    fout = templ.replace('SENSOR_NAME_LOWERCASE', user_input['SENSOR_NAME_LOWERCASE'])
    write_file(user_input['SENSOR_OUTPUT_DIR'], fout, ftext)
    return

# ------------------------------------------------------------------------------
# iss_sensor.h
# - check Gang_TODOs in template
def update_sensor_h(user_input):
    """
    Updates the content of the sensor header file.

    Args:
        user_input (dict): A dictionary containing the user input.

    Returns:
        None.
    """
    templ = 'iss_sensor_SENSOR_NAME_LOWERCASE.h'
    ftext = read_file(os.path.join(user_input['SENSOR_TEMPLATE_DIR'], templ))

    rep_list = ['SENSOR_NAME_UPPERCASE',
                'SENSOR_NAME_LOWERCASE',
                'SENSOR_RESOLUTION_WIDTH',
                'SENSOR_RESOLUTION_HEIGHT',
                'SENSOR_META_HEIGHT_BEFORE',
                'SENSOR_META_HEIGHT_AFTER',
                ]
    for k in rep_list:
        ftext = ftext.replace(k, user_input[k])


    fout = templ.replace('SENSOR_NAME_LOWERCASE', user_input['SENSOR_NAME_LOWERCASE'])
    write_file(user_input['SENSOR_OUTPUT_DIR'], fout, ftext)
    return

# ------------------------------------------------------------------------------
# iss_sensor.c
#   ? ub953_cfgScript( ) --> not a good name in senosr framework
#   ? fill in FPD3 or FPD4 settings according to user input
#   ? choose between Board_i2c16BitRegXX and Board_i2c8BitRegXX
#   ?
def update_sensor_c(user_input):
    """
    Updates the content of the sensor C file.

    Args:
        user_input (dict): A dictionary containing the user input.

    Returns:
        None.
    """
    templ = 'iss_sensor_SENSOR_NAME_LOWERCASE.c'
    ftext = read_file(os.path.join(user_input['SENSOR_TEMPLATE_DIR'], templ))

    rep_list = ['SENSOR_MACRO_IN_HEADER',
                'SENSOR_NAME_UPPERCASE',
                'SENSOR_NAME_LOWERCASE',
                'SENSOR_DCC_ID',
                'SENSOR_RAW_MSB_LOCATION',
                'SENSOR_STREAM_REG_ADDR',
                'SENSOR_STREAM_ON_VALUE',
                'SENSOR_STREAM_OFF_VALUE',
                'SENSOR_STREAM_ON_REG2',
                'SENSOR_STREAM_OFF_REG2',
                'SENSOR_STREAM_REG2_ADDR',
                'SENSOR_STREAM_ON_VAL2',
                'SENSOR_STREAM_OFF_VAL2',
                'SERDES_USE_FPDLINK4',
                'SENSOR_USE_PCAL_D3'
                ]
    for k in rep_list:
        if k in user_input:
            ftext = ftext.replace(k, user_input[k])

    fout = templ.replace('SENSOR_NAME_LOWERCASE', user_input['SENSOR_NAME_LOWERCASE'])
    write_file(user_input['SENSOR_OUTPUT_DIR'], fout, ftext)
    return

# ------------------------------------------------------------------------------
# gain_table.h
#   ? ox03f_gain_table_t --> gain_table_t according to sensor/user input
#     - OVT  {16b, 8b, 8b}
#     - ON   {?, ?}
#     - Sony {?, ?}
#     ? what else
#   ? how to create LUT automatically given sensor type?
#     - linear tyep
#     - log tyep
def update_gain_table(user_input):
    """
    Updates the content of the gain table header file.

    Args:
        user_input (dict): A dictionary containing the user input.

    Returns:
        None.
    """
    templ = 'SENSOR_NAME_LOWERCASE_gain_table.h'
    ftext = read_file(os.path.join(user_input['SENSOR_TEMPLATE_DIR'], templ))

    rep_list = ['SENSOR_NAME_UPPERCASE',
                'SENSOR_NAME_LOWERCASE',
                ]
    for k in rep_list:
        ftext = ftext.replace(k, user_input[k])

    fout = templ.replace('SENSOR_NAME_LOWERCASE', user_input['SENSOR_NAME_LOWERCASE'])
    write_file(user_input['SENSOR_OUTPUT_DIR'], fout, ftext)
    return



# ------------------------------------------------------------------------------
# helpers

def read_file(fname):
    """
    Reads the contents of a file.

    Args:
        fname (str): The name of the file to read.

    Returns:
        str: The contents of the file.
    """
    with open(fname, 'r') as file:
        return file.read()

def write_file(path, fname, fstr):
    """
    Writes the given string to a file.

    Args:
        fname (str): The name of the file to write to.
        fstr (str): The string to write to the file.

    Returns:
        int: The number of characters written to the file.
    """
    with open(os.path.join(path, fname), 'w') as file:
        return file.write(fstr)



# ------------------------------------------------------------------------------
import os

def create_output_directory(directory_path):
    """
    Creates the output directory if it does not exist.

    Parameters:
    - directory_path: The path to the output directory.

    Returns:
    - None
    """
    try:
        # Check if the directory exists
        if not os.path.exists(directory_path):
            # Create the directory
            os.makedirs(directory_path)
            print(f"Directory '{directory_path}' created successfully.")
        else:
            print(f"Directory '{directory_path}' already exists.")
    except OSError as e:
        print(f"Error creating directory: {e}")

def create_driver(user_input):
    """
    Creates a driver for a sensor based on the provided user input.

    Args:
        user_input (dict): A dictionary containing the user input.

    Returns:
        None.
    """

    # create output directory
    create_output_directory(user_input['SENSOR_OUTPUT_DIR'])

    # sensor_config.h
    update_sensor_config(user_input)

    # serdes_cofig.h
    update_serdes_config(user_input)

    # gain_table.h
    update_gain_table(user_input)

    # iss_sensor.h
    update_sensor_h(user_input)

    # iss_sensor.c
    update_sensor_c(user_input)
