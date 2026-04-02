# SOC HW Specific data generator

This script generates SOC-specific data in `../soc_data/<soc>/*.json`.

The generated data includes:

- `socIPClk.json`: Contains all IPs and immediate clocks connected to them.
- `socClkData.json`: Contains all clocks and their type and data. There are three types of data:
    - `clk_data_div_reg`: Divider data
    - `socClkDataMux`: Mux data
    - `clk_data_pll_reg`: Pll data
    - `soc_clk_parent`: Parent of mux

## Usage

The script generates the SOC-specific data based on the `<soc>.json` file.

- `<soc>`: The System-on-Chip that the script should generate the data for. For example, `j721e`.
- `<autogen_path>`: The path to the Autogen repo. The path should be absolute. For example, `/home/user/system-firmware-autogen`.

For example, to generate the data for the j721e, the user should run:
`python3 main.py j721e /home/user/system-firmware-autogen`
