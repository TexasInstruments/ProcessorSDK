"""
Python script to extract or calculate memory addresses from soc json files of a J7 SoC and save them as CSV files
This script is used to generate addresses list only for J7 devices, as all the PLL and MMR offsets used in this file are specific to J7 devices.
"""

import json
import os
import csv

#SoC JSON files have different names than the SoC name passed from main.py
socNameMap = {
    "j722s": "j7aen_dsn",
    "j721e": "j7es",
    "j7200": "j7vcl",
    "j721s2": "j7_aep",
    "j784s4": "j7_ahp"
}

#PSC OFFSET CONSTANTS
OFFSET_MDSTAT = 0x800
OFFSET_PDSTAT = 0x200
PD_MMR_SIZE = 0x4
MD_MMR_SIZE = 0x4

# PLL OFFSET CONSTANTS
OFFSET_PLL_MMR_CFG = 0x00000008
OFFSET_PLL_CTRL = 0x00000020
OFFSET_PLL_FREQ_CTRL0 = 0x00000030
OFFSET_PLL_FREQ_CTRL1 = 0x00000034
OFFSET_PLL_DIV_CTRL = 0x00000038
OFFSET_PLL_HSDIV0_CTRL = 0x00000080

# PLL SIZE CONSTANTS
PLL_MMR_SIZE = 0x1000
PLL_HSDIV_MMR_SIZE = 0x4

# HFOSC CONSTANTS
J7_WKUP_CTRL_MMR = 0x43000000
J7_MAIN_DEVSTAT = 0x30

# PLL CTRL CONSTANTS
PLLCTRL_PLLCTRL = int(0x100)
PLLCTRL_PLLCTRL_PLLEN_BIT = 1 << 0
PLLCTRL_PLLCTRL_PLLENSRC_BIT = 1 << 5
PLLCTRL_PLLSTAT = int(0x13c)
PLLCTRL_PLLSTAT_LOCK = 1 << 1

def loadJsonFile(filePath):
    """
    Load JSON data from a file and return it as a Python object.

    If the file does not exist, print an error message and return None.
    If the file contains invalid JSON, print an error message and return None.

    Args:
        filePath: The path to the file to load.

    Returns: The loaded JSON data, or None on error.
    """
    try:
        with open(filePath, 'r') as file:
            return json.load(file)
    except FileNotFoundError:
        print(f"File not found: {filePath}")
        return None
    except json.JSONDecodeError:
        print(f"Error decoding JSON from file: {filePath}")
        return None

def createCsvFile(filePath, data):
    """
    Creates a CSV file with one address per row.

    Args:
        filePath (str): Path to the CSV file to be created.
        data (list[str]): Iterable containing address strings.

    Returns:
        bool: True if file was created successfully, False otherwise.
    """
    directory = os.path.dirname(filePath)
    try:
        # Create directory if it does not exist
        if directory and not os.path.exists(directory):
            os.makedirs(directory, exist_ok=True)
    except OSError as dirErr:
        print(f"Error creating directory {directory}: {dirErr}")
        return False

    try:
        with open(filePath, 'w', newline='', encoding='utf-8') as csvfile:
            writer = csv.writer(csvfile)
            for address in data:
                writer.writerow([address])
        return True
    except OSError as fileErr:
        print(f"Error creating or writing to file {filePath}: {fileErr}")
        return False
    except Exception as err:
        print(f"Unexpected error: {err}")
        return False

def getHsdivCnt(filePath):
    """
    Find the number of hsdivs for a PLL from soc.json file

    Args:
        filePath: The path to the soc.json file.

    Returns: Object containing number of hsdivs for each pll name
    """
    pllDomainNameMap ={
        "PLL0":"MAIN",
        "MCU_PLL0":"MCU",
        "WKUP_PLL0":"WKUP"
    }

    pllDataDict = loadJsonFile(filePath)
    pllData =dict()
    for ip in pllDataDict['ip_instances']:
        if "ip_name" in ip and ip["ip_name"] == "PLL":
            instName = ip["inst_name"]
            baseAddr = ip["regions"][0]["base"]
            pllData[instName]={}

            for reg in ip["regions"][0]["registers"]:
                regName = reg["name"]
                if regName.endswith("CFG"):
                    for bitfield in reg["bitfields"]:
                        if bitfield["name"] == "hsdiv_prsnt":
                            idx = reg["name"].split("_")[0][3:]
                            pllData[instName][idx] = bitfield["reset_value"]
    return pllData

def extractAddrFromJson(readFilePath, writeFilePath, isMuxData):
    """
    Extract register addresses from a JSON file and write them to a CSV file.

    Args:
        readFilePath (str): Path to the input JSON file.
        writeFilePath (str): Path to the output CSV file.
        isMuxData (bool): Flag indicating whether to add PLLCTRL addresses.
    """
    data = loadJsonFile(readFilePath)
    if data is None:
        print("Aborting: Failed to load JSON data.")
        return

    pllCtrlMux = [
        "clk_data_k3_pll_ctrl_wrap_wkup_0_sysclkout_clk",
        "clk_data_k3_pll_ctrl_wrap_main_0_sysclkout_clk",
        "clk_data_sam62_pll_ctrl_wrap_main_0_sysclkout_clk",
        "clk_data_sam62_pll_ctrl_wrap_mcu_0_sysclkout_clk"
        ]

    addressesSet = set()
    for key, value in data.items():
        regAddr = value.get("reg")
        if regAddr is None:
            print(f"Warning: Missing 'reg' key for entry '{key}'")
            continue
        addressesSet.add(regAddr)

        if isMuxData and key in pllCtrlMux:
            try:
                reg = int(regAddr, 16)
                regAddrPllCtrl = reg + PLLCTRL_PLLCTRL
                addressesSet.add(hex(regAddrPllCtrl))
                regAddrPllStat = reg + PLLCTRL_PLLSTAT
                addressesSet.add(hex(regAddrPllStat))
            except ValueError:
                print(f"Warning: Could not convert reg '{regAddr}' to int for entry '{key}'")

    addressesList = list(addressesSet)
    if not createCsvFile(writeFilePath, addressesList):
        print("Failed to write addresses to CSV file.")

def calcPscAddrFromJson(readFilePath, writeFilePath):
    """
    Reads a JSON file containing PSC data, calculates absolute addresses, and writes them to a CSV file.

    Args:
        readFilePath (str): Path to the input JSON file.
        writeFilePath (str): Path to the output CSV file.
    """
    data = loadJsonFile(readFilePath)
    if data is None:
        print("Aborting: Failed to load JSON data.")
        return

    addressesSet = set()
    for key, value in data.items():
        try:
            baseAddr = int(value["base_addr"], 16)
            pdIndex = int(value["pd_index"])
            lpscIndex = int(value["lpsc_index"])
        except KeyError as e:
            print(f"Warning: Missing key {e} in entry '{key}'")
            continue
        except ValueError as e:
            print(f"Warning: Value error for entry '{key}': {e}")
            continue

        try:
            pdRegAddr = baseAddr + pdIndex * PD_MMR_SIZE + OFFSET_PDSTAT
            mdRegAddr = baseAddr + lpscIndex * MD_MMR_SIZE + OFFSET_MDSTAT
            addressesSet.add(hex(pdRegAddr))
            addressesSet.add(hex(mdRegAddr))
        except Exception as e:
            print(f"Warning: Error calculating addresses for entry '{key}': {e}")

    addressesList = list(addressesSet)
    if not createCsvFile(writeFilePath, addressesList):
        print("Failed to write addresses to CSV file.")

def calcPllAddrFromJson(readFilePath, writeFilePath, socVal, autogenPath):
    """
    Reads a JSON file containing PLL data, calculates absolute addresses, and writes them to a CSV file.

    Args:
        readFilePath (str): Path to the input JSON file.
        writeFilePath (str): Path to the output CSV file.
    """
    data = loadJsonFile(readFilePath)
    if data is None:
        print("Aborting: Failed to load JSON data.")
        return

    socJsonPath = os.path.join(autogenPath, "src_input", "csl", socNameMap[socVal], "json", socNameMap[socVal].upper()+".json")
    hsdivCnt = getHsdivCnt(socJsonPath)

    addressesSet = set()
    for key, value in data.items():
        try:
            baseAddr = int(value["base"], 16)
            idx = int(value["idx"])
        except KeyError as e:
            print(f"Warning: Missing key {e} in entry '{key}'")
            continue
        except ValueError as e:
            print(f"Warning: Value error for entry '{key}': {e}")
            continue

        try:
            regAddrRefDiv = baseAddr + idx * PLL_MMR_SIZE + OFFSET_PLL_DIV_CTRL
            addressesSet.add(hex(regAddrRefDiv))

            regAddrFreqValInt = baseAddr + idx * PLL_MMR_SIZE + OFFSET_PLL_FREQ_CTRL0
            addressesSet.add(hex(regAddrFreqValInt))

            regAddrFreqValFraq = baseAddr + idx * PLL_MMR_SIZE + OFFSET_PLL_FREQ_CTRL1
            addressesSet.add(hex(regAddrFreqValFraq))

            regAddrPostDivEn = baseAddr + idx * PLL_MMR_SIZE + OFFSET_PLL_CTRL
            addressesSet.add(hex(regAddrPostDivEn))

            regAddrPostDiv = baseAddr + idx * PLL_MMR_SIZE + OFFSET_PLL_DIV_CTRL
            addressesSet.add(hex(regAddrPostDiv))

            regAddrPllEn = baseAddr + idx * PLL_MMR_SIZE + OFFSET_PLL_CTRL
            addressesSet.add(hex(regAddrPllEn))

            regAddrHsdivPrsnc = baseAddr + idx * PLL_MMR_SIZE + OFFSET_PLL_MMR_CFG
            addressesSet.add(hex(regAddrHsdivPrsnc))

            regAddrHfoscFreq = J7_WKUP_CTRL_MMR + J7_MAIN_DEVSTAT
            addressesSet.add(hex(regAddrHfoscFreq))

            name = "PLL0" if key.split("_")[-2] == "main" else ("WKUP_PLL0" if socVal == "j722s" else "MCU_PLL0")
            index = int(key.split("_")[-1])
            hsdivPrsnt = hsdivCnt[name][str(index)]

            id = 0
            while hsdivPrsnt:
                if hsdivPrsnt&1:
                    regAddrClkEn = baseAddr + idx * PLL_MMR_SIZE + OFFSET_PLL_HSDIV0_CTRL + PLL_HSDIV_MMR_SIZE * id
                    addressesSet.add(hex(regAddrClkEn))
                id += 1
                hsdivPrsnt = hsdivPrsnt >> 1

        except Exception as e:
            print(f"Warning: Error calculating addresses for entry '{key}': {e}")

    addressesList = list(addressesSet)
    if not createCsvFile(writeFilePath, addressesList):
        print("Failed to write addresses to CSV file.")

def createAddrFromSocData(socVal, autogenPath):
    """
    Given a SoC value (socVal, e.g. j722s, j784s4, etc) , constructs input and output paths for various SoC-related data files,
    then extracts and calculates register addresses, saving them to CSV files.

    Args:
        socVal (str): The name of the SoC data set (e.g., 'j722s', 'j721e', etc.)
    """
    # Determine the base directory (three levels up from this file)
    baseDir = os.path.dirname(os.path.dirname(os.path.abspath(os.getcwd())))
    readPath = os.path.join(baseDir, f'soc_data/{socVal}')
    writePath = os.path.join(baseDir, f'soc_data/{socVal}/mem_addr')

    # Ensure output directory exists
    try:
        os.makedirs(writePath, exist_ok=True)
    except OSError as e:
        print(f"Error creating output directory {writePath}: {e}")
        return

    # Extract and calculate addresses
    try:
        extractAddrFromJson(
            os.path.join(readPath, "soc_clk_data_div.json"),
            os.path.join(writePath, "soc_clk_data_div_addr.csv"),
            False
        )
    except Exception as e:
        print(f"Error processing soc_clk_data_div: {e}")

    try:
        extractAddrFromJson(
            os.path.join(readPath, "soc_clk_data_mux.json"),
            os.path.join(writePath, "soc_clk_data_mux_addr.csv"),
            True
        )
    except Exception as e:
        print(f"Error processing soc_clk_data_mux: {e}")

    try:
        calcPscAddrFromJson(
            os.path.join(readPath, "soc_psc_data.json"),
            os.path.join(writePath, "soc_psc_data_addr.csv")
        )
    except Exception as e:
        print(f"Error processing soc_psc_data: {e}")

    try:
        calcPllAddrFromJson(
            os.path.join(readPath, "soc_pll_data.json"),
            os.path.join(writePath, "soc_pll_data_addr.csv"),
            socVal,
            autogenPath
        )
    except Exception as e:
        print(f"Error processing soc_pll_data: {e}")

    print("Addresses Created")

createAddrFromSocData('j784s4', '/home/cgt/ti/sysfw/system-firmware-tools/system-firmware-autogen')