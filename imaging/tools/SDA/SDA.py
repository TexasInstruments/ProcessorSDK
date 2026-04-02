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

import sys
import argparse
import shlex
from create_sensor_driver import create_driver

def read_string_pairs(filename):
    """
    Reads in string pairs from a text file into a dictionary.

    Args:
        filename (str): The name of the text file to read from.

    Returns:
        dict: A dictionary where the keys are the first strings in each pair and the values are the second strings.
    """
    string_pairs = {}
    try:
        with open(filename, 'r') as file:
            for line in file:
                # Strip leading and trailing whitespace from the line
                line = line.strip()
                # Skip empty lines and comments
                if line and not line.startswith('#'):
                    # Split the line into tokens
                    tokens = shlex.split(line, comments=True)
                    # Check if the line contains exactly two tokens
                    if len(tokens) == 2:
                        # Add the token pair to the dictionary
                        string_pairs[tokens[0]] = tokens[1]
                    else:
                        print(f"Skipping invalid line: {line}")
    except FileNotFoundError:
        print(f"File '{filename}' not found.")
    return string_pairs

def main():
    # Parse command line arguments
    parser = argparse.ArgumentParser(description='Create a sensor driver from a configuration file')
    parser.add_argument('-c', '--config', default='config.txt', help='Path to the configuration file')
    args = parser.parse_args()

    # Read string pairs from file
    try:
        user_input = read_string_pairs(args.config)
    except Exception as e:
        print(f"Error reading string pairs: {e}")
        sys.exit(1)

    # Create driver
    try:
        create_driver(user_input)
    except Exception as e:
        print(f"Error creating driver: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()
