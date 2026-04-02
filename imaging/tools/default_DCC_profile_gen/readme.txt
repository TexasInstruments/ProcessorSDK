Usage for generating default tuning of your sensor:
1. Go to "scripts" folder
2. run "./generate_IMX390_cfg.sh" or "python3 ctt_def_xml_gen.py <sensor properties file>"

e.g. 
- python3 ctt_def_xml_gen.py ../configs/imx390_properties.txt
- or refer to "tune_IMX390_Disco_Def/" folder for default tuning of IMX390 Discovery Camera from D3

For other sensors, create a new script using (generate_IMX390_cfg.sh) for reference.

Default DCC XML files will be created in the folder PRJ_DIR as defined in the sensor properties file. 
Existing files in the folder with same file names will be overwritten. 


Usage for auto-tuning of your sensor:
- refer to "tune_IMX390_Disco_Raw/" and "tune_IMX390_RCM/" folders and follow the instructions in "imx390_cfg.txt"

Dependencies
1. Ubuntu 20.04 or 22.04
2. Python3 and numpy
3. OpenCV and Macbeth Chart Module for auto-tuning with raw sensor images

Note
The parameters like width, height, DCC ID, sensor name etc must match the values defined in sensor driver. 
 
 
