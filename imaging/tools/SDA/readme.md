# Sensor Driver Automation Tool

---

## 1. Introduction

This README file provides instructions for using the Sensor Driver Automation (SDA) tool for TDA4x RTOS SDK.
The SDA tool is a python script which generates the source code for your sensor driver in TDA4x RTOS SDK based on your input of [a sensor driver configuration file](#52-preparing-the-driver-configuration-file), [a sensor register initialization file](#51-preparing-the-sensor-initialization-file), and pre-prepared [sensor driver templates](#4-sensor-driver-templates).

## 2. Prerequisites

- Python 3.x
- Your camera/sensor module with a FPD-Link serializer and a FAKRA output connector, e.g., [D3RCM-IMX390-953](https://www.d3embedded.com/product/designcore-d3rcm-imx390-953-rugged-camera-module/)
- One TI TDA4x EVM, e.g., [J784S4XEVM](https://www.ti.com/tool/J784S4XEVM) or [J722SXH01EVM](https://www.ti.com/tool/J722SXH01EVM)
- A Fusion application Board (Fusion-1 or Fusion-2), e.g., [Fusion Application Board](https://www.ti.com/lit/pdf/spruii1)
    - Please refer to this [e2e FAQ](https://e2e.ti.com/support/processors-group/processors/f/processors-forum/1575823/faq-j722sxh01evm-how-do-i-use-the-fusion2-board-with-the-j722sxh01evm) for connecting J722S EVM, Fusion-2, and your camera. 
- TDA4 RTOS SDK, e.g., [Processor SDK RTOS J784S4](https://software-dl.ti.com/jacinto7/esd/processor-sdk-rtos-j784s4/latest/exports/docs/psdk_rtos/docs/user_guide/index.html), or [Processor SDK RTOS J722S](https://software-dl.ti.com/jacinto7/esd/processor-sdk-rtos-j722s/latest/exports/docs/psdk_rtos/docs/user_guide/index.html)

## 3. User Input

- A sensor register initialization file including all information needed for initializing your sensor over I2C (the information is typically provided by your sensor vendor).
- A driver configuration file for your camera/sensor module

## 4. Sensor Driver Templates <a id="4-sensor-driver-templates"></a>

Templates for popular ADAS sensors from Sony, OVT, and Onsemi are prepared and included in the SDA tool.
They work well for [the cameras](https://software-dl.ti.com/jacinto7/esd/processor-sdk-rtos-j722s/latest/exports/docs/imaging/imaging_release_notes.html#ImageSensor_Support_Linux_RTOS) supported by TDA4 RTOS SDK so far.

## 5. User Instructions

To use the SDA tool, please follow these steps below.

- Step-1: clone the [imaging repository](https://git.ti.com/cgit/processor-sdk/imaging/) or download the "imaging/tools/SDA/" folder manually, then navigate to that `SDA/` folder.
- [Step-2:](#51-preparing-the-sensor-initialization-file) Create a sensor initialization file, e.g., `SDA/input/sensor.txt` in the `SDA/input` directory.
- [Step-3:](#52-preparing-the-driver-configuration-file) Create a driver configuration file for your camera/sensor module, e.g., `SDA/input/config.txt` in the `SDA/input` directory.
- [Step-4:](#additional-instructions) Edit the configuration file to include the required settings and parameters for your camera/sensor module.
- Step-5: Run the script in `SDA/` folder using the command `python SDA.py -c input/config.txt`.
- Step-6: The script will generate the sensor driver source files under the `SDA/output` directory as specified in the driver configuration file.
- [Step-7:](#55-integration-of-the-new-sensor-driver-into-tda4x-rtos-sdk) Copy the generated sensor driver files into your TDA4x RTOS SDK `imaging/sensor_drv/` folder properly, apply some integration code changes, and test your new RTOS driver by streaming raw sensor video from your camera to TDA4 EVM.

### 5.1 Preparing the Sensor Initialization File <a id="51-preparing-the-sensor-initialization-file"></a>

The sensor initialization file includes information needed to initialize all sensor registers.
The initialization information is provided by the sensor vendor.

- Create a text file in the 3-column hex format of `sensor register address`, `sensor refister value`, and `I2C programming delay` to below.

```bash
0107                          01              00
4d5a                          1c              00
0308                          05              01
0409                          06              02
...
```
- The sensor initialization also decides most of the information needed later in the configuration file (such as sensor image width and height)

### 5.2 Preparing the Driver Configuration File <a id="52-preparing-the-driver-configuration-file"></a>

The configuration file should be in the 2-column format ("name" and "value") as in the IMX390 camera example below.
Please follow the comments at the end of each line carefully.

``` bash
#--------------------------------------------------------
# basic camera/sensor info

SENSOR_MACRO_IN_HEADER          SENSOR_SONY_IMX390_UB953_D3           # define this string as a macro in "iss_sensors.h" for SDK integration
SENSOR_NAME_UPPERCASE           IMX390                                # sensor name in upper case
SENSOR_NAME_LOWERCASE           imx390                                # sensor name in lower case
SENSOR_TEMPLATE_DIR             template_sony                         # driver template folder (available now for Sony, OVT, Onsemi)
SENSOR_OUTPUT_DIR               out_imx390                            # folder for saving generated driver code

# DCC ID must match ID used in tuning tool/tuning scripts
SENSOR_DCC_ID                   390                                   # DCC ID used for the camera/sensor/ISP tuning

#--------------------------------------------------------
# sensor config
SENSOR_RESOLUTION_WIDTH         1936                                  # number of pixels per row
SENSOR_RESOLUTION_HEIGHT        1096                                  # number of pixels per column
SENSOR_META_HEIGHT_BEFORE          0                                  # Metadata rows before a frame
SENSOR_META_HEIGHT_AFTER           4                                  # Metadata rows after a frame
SENSOR_RAW_MSB_LOCATION           11                                  # Raw pixel MSB location, e.g., 11 for 12-bit pixels
SENSOR_INIT_FILE              input/imx390_regs.txt                   # Sensor register address and value pairs used to program the sensor with above settings
SENSOR_USE_PCAL_D3                 0                                  # 0 - if no PCAL is used (D3 modules only); 1 - include PCAL code

# sensor stream on/off
SENSOR_STREAM_REG_ADDR          0x0000                                # Register address that controls stream on off on sensor
SENSOR_STREAM_ON_VALUE          0x00                                  # Value for SENSOR_STREAM_REG_ADDR to start streaming on sensor
SENSOR_STREAM_OFF_VALUE         0x01                                  # Value for SENSOR_STREAM_REG_ADDR to stop streaming on sensor

SENSOR_STREAM_ON_REG2           0                                     # OPTIONAL - Need a 2nd register write to start sensor streaming: 0 for no, 1 for yes
SENSOR_STREAM_OFF_REG2          0                                     # OPTIONAL - Need a 2nd register write to stop sensor streaming : 0 for no, 1 for yes
SENSOR_STREAM_REG2_ADDR         0x0000                                # OPTIONAL - Address of the 2nd sensor register
SENSOR_STREAM_ON_VAL2           0x00                                  # OPTIONAL - Value for SENSOR_STREAM_REG2_ADDR to start streaming
SENSOR_STREAM_OFF_VAL2          0x00                                  # OPTIONAL - Value for SENSOR_STREAM_REG2_ADDR to stop streaming

#--------------------------------------------------------
# serdes config
SENSOR_8_BIT_I2C_ADDRESS        0x34                                  # Physical 8-bit I2C address of sensor
SERIALIZER_8_BIT_I2C_ADDRESS    0x30                                  # Physical 8-bit I2C address of serializer
SERDES_USE_FPDLINK4                0                                  # Serializer type:     0 for FPD-Link 3, 1 for FPD-Link 4
SER_CFG_I2C_STRAP                  0                                  # Sensor I2C valtage:  0 for 3.3V, 1 for 1.8V

#--------------------------------------------------------
# serilizer GPIO settings
SER_LOCAL_GPIO_DATA             0x03                                  # GPIO output source register of serilizer: 4 LSBs for GPIO_OUT_SRC and 4 MSBs for GPIO_RMTEN
SER_GPIO_INPUT_CTRL             0xF0                                  # GPIO input control settings of serizizer: fixed to 0xF0 for most camera modules 

# clock divider setttings
# FPD-Link 3: "SENSOR_INPUT_CLK = 4000 * M / N / (1<<SER_HS_CLK_DIV)"
#          in the example below, 4000 * 1 / 37 / (1<<2) = 27.03 MHz
SER_HS_CLK_DIV                  2                                    # Divider for output clock:  0 for 1x, 1 for 2x, 2 for 4x, 3 for 8x
SER_DIV_M_VAL                   1                                    # M value for M/N Divider
SER_DIV_N_VAL                   37                                   # N value for M/N Divider

# FPD-Link 4: "SENSOR_INPUT_CLK = 7550 * M / N / (1<<3)"     (fix SER_HS_CLK_DIV to 3 for 8x)
#          in the example below, 7550 * 1 / 37 / (1<<3) = 25.51 MHz
#SER_HS_CLK_DIV                  3                                   # use 3 for FPDLink4
#SER_DIV_M_VAL                   1                                   # M value for M/N Divider
#SER_DIV_N_VAL                   37                                  # N value for M/N Divider
```

#### Additional Instructions <a id="additional-instructions"></a>

- `SENSOR_MACRO_IN_HEADER`

This is used for integrating your sensor driver into the sensor driver framework.
Please refer to this [line](https://git.ti.com/cgit/processor-sdk/imaging/tree/sensor_drv/include/iss_sensors.h?h=main#n161) for an example of IMX390 camera.

- `SENSOR_TEMPLATE_DIR`

Driver templates for popular ADAS sensors from Sony, OVT, and Onsemi are included in the tool under `template_sony`, `template_ovt`, and `template_onsemi` folders.

- `SENSOR_INIT_FILE`

This is the text file including all your sensor register initialization information.
It is in 3-column hex format of `sensor register address`, `sensor refister value`, and `I2C programming delay`. 
These settings shall be provided by your sensor vendor.

- `SENSOR_META_HEIGHT_BEFORE` and `SENSOR_META_HEIGHT_AFTER`

These values shall match the sensor register settings you used.

- `SENSOR_USE_PCAL_D3`

This value may be set to 1 for the D3 DesignCore&reg; Discovery cameras such as [IMX390](https://www.d3embedded.com/product-category/camera-modules/?fwp_platform=texas-instruments&fwp_series=discovery) and [IMX728](https://www.d3embedded.com/product-category/camera-modules/?fwp_platform=texas-instruments&fwp_interface=fpd-link-iv&fwp_sensor_model=sony-imx728-).

- `SENSOR_STREAM_REG_ADDR`, `SENSOR_STREAM_ON_VALUE`, `SENSOR_STREAM_OFF_VALUE`

These fields identify sensor register address and values for starting and stopping sensor streaming.
They shall be available in the sensor datasheet.

- `SENSOR_STREAM_ON_REG2`, `SENSOR_STREAM_ON_VAL2`, `SENSOR_STREAM_OFF_VAL2`, and `SENSOR_STREAM_OFF_REG2`

Some sensors may require writing to a second value to start and/or stop streaming. 

- `SER_CFG_I2C_STRAP`

This field is for the "I2C Strap Mode" setting of serializer (e.g., UB953).
Program this bit to choose the I2C voltage level between 3.3V and 1.8V.

- `SER_LOCAL_GPIO_DATA` and `SER_GPIO_INPUT_CTRL`

These values are for the GPIO register settings of the serializer.
Please refer to `LOCAL_GPIO_DATA` and `GPIO_INPUT_CTRL` registers in FPD-Link serializer [datasheet](https://www.ti.com/lit/ds/symlink/ds90ub953-q1.pdf) for details.

- `SER_HS_CLK_DIV`, `SER_DIV_M_VAL`, and `SER_DIV_N_VAL`

These values are for setting up the serializer correctly to provide an input clock signal for your sensor.
Please follow the equations in the comments of the configuration file to choose the right values for your camera/sensor module.

### 5.4 Output

The script will generate 5 sensor driver source/header files according to `SENSOR_NAME_LOWERCASE` in configuration file, e.g., 

- `imx390_sensor_config.h`
- `imx390_serdes_config.h`
- `imx390_gain_table.h`
- `iss_sensor_imx390.h`
- `iss_sensor_imx390.c`

These files will be placed in the directory specified by `SENSOR_OUTPUT_DIR` in the configuration file.

### 5.5 Integration of the New Sensor Driver into TDA4x RTOS SDK <a id="55-integration-of-the-new-sensor-driver-into-tda4x-rtos-sdk"></a>

After the sensor driver files are generated and copied to TDA4 RTOS SDK, some modification to `iss_sensors.h` and `iss_sensors.c` is necessary to integrate the new driver into the sensor driver framework.

1. Add driver C source files to `imaging/sensor_drv/src/concerto.mak` for top-level Makefiles to build driver into SDK.
    - Example: view the [concerto Makefile example](https://git.ti.com/cgit/processor-sdk/imaging/commit/sensor_drv/src/concerto.mak?h=main&id=6c8864dd92b1ca91e6d37aa7cd14fd2943eef9cf).
2. Register the driver handle with the sensor driver framework.
    - Call your driver's initialization function inside `IssSensor_Init()` in `imaging/sensor_drv/src/iss_sensors.c` to register the driver handle with the framework.
    - Driver initialization function is generated by the SDA and is in the format of `IssSensor_SENSOR_NAME_UPPERCASE_Init`, where `SENSOR_NAME_UPPERCASE` is from sensor configuration file.
        - Example: `status |= IssSensor_IMX390_Init();`
    - Add sensor init function signature to `imaging/sensor_drv/src/iss_sensor_priv.h`. See [example commit](https://git.ti.com/cgit/processor-sdk/imaging/commit/sensor_drv/src/iss_sensor_priv.h?h=main&id=7e3cc4030107247a885aa3530aa5346d6fa25a50).
3. Add driver macro name definition to `iss_sensors.h`.
    - The driver macro name used in `iss_sensors.h` should match `SENSOR_MACRO_IN_HEADER` from the SDA configuration.
    - The actual name used can be any alphanumeric string with 32 characters. TI usually uses `[IMAGE_SENSOR]-[SERIALIZER]-[CAM_MODULE_MANUFACTURER]`.
    - Example: `[#define SENSOR_SONY_IMX390_UB953_D3     "IMX390-UB953_D3"]` in `iss_sensors.h` under the `"Supports Sensor's name"` section
4. Test your new sensor driver with `app_heterogeneous` in vision apps of TDA4x SDK
    - Change the app in capture only mode after uncommenting [this line](https://git.ti.com/cgit/processor-sdk/vision_apps/tree/apps/utilities/app_heterogeneous/app_heterogeneous.c?h=main#n83)
    - Recompile your TDA4x SDK and prepare new SD card image for your TDA4x EVM following these instructions
        - [Quick Steps to build vision apps for selected configuration](https://software-dl.ti.com/jacinto7/esd/processor-sdk-rtos-jacinto7/latest/exports/docs/vision_apps/docs/user_guide/BUILD_INSTRUCTIONS.html#BUILD_DETAILED)
        - [Run vision apps on EVM in Linux+RTOS mode (via SD card boot)](https://software-dl.ti.com/jacinto7/esd/processor-sdk-rtos-jacinto7/latest/exports/docs/vision_apps/docs/user_guide/RUN_INSTRUCTIONS.html#run_steps_linux)
        - Stream 100 frames from your sensor and same one raw image on SD card with your camera connected to port-0 of your Fusion-board
            - `./app_heterogeneous 101 0 <your_sensor_driver_id>`
        - To determine your sensor ID run the app_heterogeneous app once to see the enumeration of available sensors. Example list below:
            ```
            ISS: Enumerating sensors ... !!!
            ISS: Enumerating sensors ... found 0 : IMX390-UB953_D3
            ISS: Enumerating sensors ... found 1 : IMX390-UB953-DISCOVERY-NARROW
            ISS: Enumerating sensors ... found 2 : IMX390-UB953-DISCOVERY-MID
            ISS: Enumerating sensors ... found 3 : IMX390-UB953-DISCOVERY-WIDE
            ISS: Enumerating sensors ... found 4 : IMX728-UB971_SONY
            ISS: Enumerating sensors ... found 5 : AR0233-UB953_MARS
            ISS: Enumerating sensors ... found 6 : AR0820-UB953_LI
            ISS: Enumerating sensors ... found 7 : UB9xxx_RAW12_TESTPATTERN
            ISS: Enumerating sensors ... found 8 : UB96x_UYVY_TESTPATTERN
            ISS: Enumerating sensors ... found 9 : GW_AR0233_UYVY
            ISS: Enumerating sensors ... found 10 : SENSOR_OX02F_UB953_OV
            ```
        - For more instructions see: [Heterogeneous App](https://software-dl.ti.com/jacinto7/esd/processor-sdk-rtos-jacinto7/latest/exports/docs/vision_apps/docs/user_guide/group_apps_utilities_app_heterogeneous.html)

### 6 Additional Steps for Running Single-Cam App with Your New Sensor Driver 
- Generate default DCC files for your driver.
    - For basic generation of header files, follow the default [XML generation tool](https://git.ti.com/cgit/processor-sdk/imaging/tree/tools/default_DCC_profile_gen/readme.txt).
    - Double check the information in input configuration matches with parameters used in the XML generation tool. In other words, width, height, DCC ID, sensor name etc, must match with that of the generated sensor driver params.
    - Navigate to the generated XMLs and run `generate_dcc.sh`.
        - Double check the following paths are correct: DCC_TOOL_PATH, DCC_BIN_PATH, OUT_PATH in `generate_dcc.sh`.
        - OUT_PATH must be `imaging/sensor_drv/include`.
- Add tuning header files to `imaging/utils/iss/src/app_iss_common_hlos.c`.
    - Tuning information needs to be passed to the VPAC and 2A kernels. `app_iss_common_hlos.c` abstracts the fetching of VISS, LDC, and 2A buffers and its sizes for each individual driver and DCC ID pair. 
    - Add generated DCC data from step 3 to `app_iss_common_hlos.c`, see [example commit](https://git.ti.com/cgit/processor-sdk/imaging/commit/utils/iss/src/app_iss_common.c?h=main&id=7e3cc4030107247a885aa3530aa5346d6fa25a50)
- Once the default DCC files are generated and tuning header files are added, please refer to the [Single Camera Application Documentation](https://software-dl.ti.com/jacinto7/esd/processor-sdk-rtos-jacinto7/latest/exports/docs/vision_apps/docs/user_guide/group_apps_basic_demos_app_single_cam.html) to run the demo.

## 7. Next Steps for Completing Your Sensor Driver

- Sensor Exposure Control
    - Implement the `SetAeParams` function for adjusting sensor exposure, e.g., [IMX728_SetAeParams](https://git.ti.com/cgit/processor-sdk/imaging/tree/sensor_drv/src/imx728/iss_sensor_imx728.c?h=main#n807).
        - This function varies from sensor to sensor and please refer to your sensor datasheet for more detail.
    - Implement the `GetExpPrgFxn` function for setting up AE, e.g., [IMX728_GetExpPrgFxn](https://git.ti.com/cgit/processor-sdk/imaging/tree/sensor_drv/src/imx728/iss_sensor_imx728.c?h=main#n1034) 
        - Please follow [this FAQ](https://e2e.ti.com/support/processors-group/processors/f/processors-forum/1262114/faq-setting-up-auto-exposure-ae-for-your-image-sensor-with-tda4-am6xa-isp-vpac) for more detail.
- Sensor White Balance (necessary if white balance has to be applied in sensor)
    - Properly set min max values for white balance gains in `GetWBPrgFxn` function, e.g., [AR0233_GetWBPrgFxn](https://git.ti.com/cgit/processor-sdk/imaging/tree/sensor_drv/src/ar0233/iss_sensor_ar0233.c?h=main#n724).
    - Map white balance values to sensor register values in `SetAwbParams` function, e.g., [AR0233_SetAwbParams](https://git.ti.com/cgit/processor-sdk/imaging/tree/sensor_drv/src/ar0233/iss_sensor_ar0233.c?h=main#n502).
- Sensor Probe Function (necessary if your sensor driver needs to support user probing)
    - Program the `Probe` function to read sensor identifying values. Return 0 if values match to indicate sensor was probed and matches driver known values, otherwise, return 0.
    - e.g., [IMX390_Probe](https://git.ti.com/cgit/processor-sdk/imaging/tree/sensor_drv/src/imx390/iss_sensor_imx390.c?h=main#n198)
- Set up VPAC tuning for your new camera/sensor module 
    - Please refer to [this page](https://e2e.ti.com/support/processors-group/processors/f/processors-forum/1553641/faq-tda4vm-how-to-use-auto-tune-in-the-python-tool-under-imaging-tools-of-tda4x-am6xa-sdk) for details.

## 8. Troubleshooting

If you encounter any issues, check the following:

- Make sure you have the correct Python version installed.
- Verify that your configuration file is correctly formatted.
- Check if the output directory is created.

For more information or to report issues, please go to [e2e.ti.com](https://e2e.ti.com).
