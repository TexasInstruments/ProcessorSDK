# k3-respart-tool

Resource Partitioning tool for K3 devices. This tool is based on Texas
Instrument's SysConfig tool. It allows you to configure various system level
parameters and generate the data which can be fed into many software components.

Typical usage for this tool is for System integrators, where one would be  able
to partition **various resources** across different software components. These
resources includes DMA channels, rings, proxies, interrupts, etc. Apart from
this, the tool supports configuration of QoS (Quality of Service) and Firewall
parameters which helps in ensuring partitioning of **peripheral devices** across
different CPUs or virtual machines.

## Getting started

To start using this tool, follow these instructions:
* Download the SysConfig tool from [SysConfig release download link]
(https://www.ti.com/tool/download/SYSCONFIG)
* Download the file setup.exe or setup.run for Windows or Linux machine
respectively.
* If you have the **k3-resource-partitioning** packaged in the SDK, use this or
clone from TI internal [bitbucket project]
(https://bitbucket.itg.ti.com/projects/PSDKLA/repos/k3-resource-partitioning/browse)
* Open the SysConfig tool GUI from the desktop shortcut and click on the
**Import Software Product** button beside the **Software Product** dropdown menu.
* Navigate and select the **k3-resource-partitioning** folder.
* If not selected already, select the **Keystone3 Resource Partitioning Tool** option from the **Software Product** dropdown menu.
* Lastly, click on **Browse** button to open existing design for the platform
that you are interested in. Navigate to the out/ directory and you will find
baseline files for your platform. Use this as the starting point for any
customization.
* We do not recommend you to start from scratch, always load the baseline file
out/XYZ-platform-name.syscfg

For devices that are not yet officially supported by the latest SysConfig tool,
Resource Partitioning tool may provide patch files to support the device in 
SysConfig. Run the **sysconfig-deviceData-update** script 
`./scripts/sysconfig-deviceData-update.sh <soc> 
--sysconfig_dir=/path/to/sysconfig/installation` to patch the SysConfig tool.

## Troublshooting

* If you get an error like **No product with name "K3-Respart-Tool" and version
0.5 found**, Most likely you have missed the step to select the
**software product**. * If you are not able to see the HTML table and getting
error **ommitted HTML**, Most likely you may have missed / failed to run the
**setup script**. * If you see error **XYZ is not generated when configuration
errors exist**, It means your resource allocation has few errors. Please adjust
the allocation to fix the errors and then you can generate the output files.

## Usage

Once you have loaded the product in the SysConfig, and opened the baseline file,
you can accomplish following things.

### NAVSS Resource partitioning

This module allows to partition or allocate Navigator Subsystem (NAVSS)
resources across different hosts in the System. A **host** is any software
component which has a dedicated context for communication with
**System firmwarem** (SYSFW). A **Resource Management board config** file
describes how these resources are partitioned. This board config is passed by
the bootloader to System firmware as part of the boot up sequence. This module
allows you to automatically generate this file in an attempt to ease the NAVSS
resource partitioning.

In the left pane of GUI, you will see different hosts available and each host
describes the resources allocated to it. All the NAVSS resources are organized
in different **groups**, where user can specify the required **count** for each
resource. The tool automatically allocates the resources taaking into
consideration all the counts for all the hosts.

Apart from the resource allocation, the tool also has an option to configure
different **host capabilities**. Click on the **?** sign next to the host name
to read the detailed documentation for that host module.

#### Review resource allocation

At any point, user can review the current resource allocation done by the tool.
Select the pane **Resource Allocation Table** from the three dots shown at the
top right corner of the tool. Here, it presents an HTML table which shows the
exact range of resources allocated for each host. Each column describes the
resource ranges assigned for a certain host. Each row describes how different
host consume the give resource.

### Bandwidth Limiter

This module allows to generate the date required for configuring the various
rate limiters available for the SOC. Each rate limiter is expected to be
connected direcly to a single source vbusm interface that is being limited. The
limiter allows the user to impose bandwidth limits for reads or writes, as well
as limit the maximum outstanding transactions for reads or writes. These limits
are orthogonal to one another and can be programmed independently.

In the GUI, user should select a device and enable bandwidth and/or transaction
limits. Then the corresponding registers can be programmed with the required
limits. The tool will autogenerate a simple address-value pair data structure in
the **qos_data.c** output file. This can be used by any software (typically
bootloader) to program all the BW limiter settings

### QoS configuration

This module allows to generate the data required for configuring CBASS QoS
(Quality of Service) endpoints. QoS configuration includes two types of
parameters. Few parameters are used for tuning the performance of the DMA
transactions in the interconnect, and others are used for setting up the
IOMMU paths for the masters. For each device, there are multiple master ports
using which, DMA requests are made. Each device is capable of driving different
values of channel_id along with the DMA request. For every channel, a unique QoS
configuration can be programmed.

In the GUI, User should select a device, choose the endpoints, and select a list
of channels for which QoS should be programmed. Note that it is possible to add
multiple instances of QoS module with same device as long as the endpoints and
channels do not overlap. The tool auto generates a simple address-value pair
data structure in the **qos-config.c** output file. This can be used by any
software (typically bootloader) to program all the QoS settings.

### Firewall configuration

This module allows to generate the data required for Firewall configuration.
This is very much useful to ensure peripheral partitioning across CPU cores.
User can describe if a certain peripheral needs to be accessible ONLY from a
list of certain hosts. Each peripheral is protected by a set of firewalls. User
can select what all firewalls need to be programmed. Each firewall has different
numbers of regions availalbe. The region describes the address range where the
filter rules are applicable. Each region can be programmed to define access
rules for upto 3 permission slots. Multiple CPUs can have the same priv_id and
this would means one slot should be sufficient. The permission slot describes
if a transaction from a CPU cores with slected priv_id is allowed or not. You
can define this for different values of security, privilege level and
transaction types. e.g. Allow Secure writes, Secure reads, but do not allow Non
secure writes from A72 to MMC.

By default, the tool will automatically set the required start/end addresses to
be configured in the region, optionally allowing the user to define custom
region if required. Also, the tool allows to select a host_id and populates the
priv_id automatically. Using this data, it generates an array of data TISCI
message data structure that can be directly passed to SYSFW for firewall
configuration.

#### Split Resource Management Board Configuration

In order to reduce RM boardconfig size on TIFS data, user can filter some resources
those are not applicable for TIFS. The resource types those can be removed are 
present in **tifsResRem.json**. While generating **tifs-rm-cfg.c** tool will
skip all the resource that are mentioned in the **tifsResRem.json.

## Generate output files

This tool generates different files which has the RM board config data, QoS
configuration and Firewall configuration. Following table describes how to use
these files:

| Filename                            | Used by       | Output destination                   | Comments                                                                        |
|-------------------------------------|---------------|--------------------------------------|---------------------------------------------------------------------------------|
| rm-cfg.c                            | k3-image-gen  | soc/<soc>/<profile>/                 | e.g. - k3-image-gen/soc/j721e/evm/rm-cfg.c                                      |
| tifs-rm-cfg.c                       | k3-image-gen  | soc/<soc>/<profile>/                 | e.g. - k3-image-gen/soc/j721e/evm/tifs-rm-cfg.c                                 |
| sysfw_img_cfg.h                     | k3-image-gen  | soc/<soc>/<profile>                  | e.g. - k3-image-gen/soc/j721e/evm/sysfw_img_cfg.h                               |
|                                     |               |                                      |                                                                                 |
| <soc>-qos-config.c                  | U-boot        |                                      |                                                                                 |
| <soc>-qos-config.c                  | SBL           |                                      |                                                                                 |
|                                     |               |                                      |                                                                                 |
| <soc>-firewall-config.c             | U-boot        |                                      |                                                                                 |
| <soc>-firewall-config.c             | SBL           |                                      |                                                                                 |
|                                     |               |                                      |                                                                                 |
| sciclient_defaultBoardcfg.c         | PDK sciclient | packages/ti/drv/sciclient/soc/V<X>/  | e.g. - pdk/packages/ti/drv/sciclient/soc/V1/sciclient_defaultBoardcfg.c         |
| sciclient_defaultBoardcfg_rm.c      | PDK sciclient | packages/ti/drv/sciclient/soc/V<X>/  | e.g. - pdk/packages/ti/drv/sciclient/soc/V1/sciclient_defaultBoardcfg_rm.c      |
| sciclient_defaultBoardcfg_tifs_rm.c | PDK sciclient | packages/ti/drv/sciclient/soc/V<X>/  | e.g. - pdk/packages/ti/drv/sciclient/soc/V2/sciclient_defaultBoardcfg_tifs_rm.c |
   
## Developer notes

### directory structure
* **.metadata/product.json** - This file describes all the components that needs
to be loaded by the SysConfig tool. It also describes the supported platforms.
`product.json` describes the components and the components describe the list of
modules and templates that are applicable for the selected device.
* **modules** - This contains the UI module definition for different
configurables, their grouping, organization, and the Javascripts for handling
onChange events and validations.
* **templates** - This contains the xdt files which describe the output file
formats and small code snippets to generate the data using templates. It also
has few views to  describe the data in a more visual format like a Markdown
table or HTML table.
* **scripts** - These are the Javascripts for parsing different input data files
which generate the SoC specific JSON objects for the usage in the modules.
It also implements few utility functions which are frequently called by modules
and templates.
* **data** - This contains the JSON objects that the tool uses to populate the
UI items with SoC specific data. Many of these are auto generated using the
parsing scripts.
* **deviceData** - This contains Sysconfig patch files for devices that are
not yet officially supported in the Sysconfig Tool.

### Updating the device data

This tool needs lot of hardware and software data to present the GUI and to
allocate resources. Majority of this data is auto generated using SoC JSON files,
System firmware public documentation, etc. Most users do not need to update this
data. This is required only when adding support for a new SoC, migrating to new
SYSFW version or migrating to new SoC JSON, etc.

Make sure to add symbolic links to the appropriate repositories as shown below:
```
#  Do this ONLY if you want to update the data for SysConfig tool
dir=`git rev-parse --show-toplevel`
ln -s <path/to/system firmware repo> $dir/../system-firmware
ln -s <path/to/system firmware autogen repo> $dir/../system-firmware-autogen

```

There is a utility script `scripts/generate-data.sh [SOCNAME | all]` to generate
all the required JSON objects for selected SoC or for all of them.

### Procedure for Generating Data Files for a New SoC for the First Time

These are the initial steps to follow when starting from scratch with a new SoC.

* First, follow the instructions in the previous section on adding symbolic links
to the system-firmware and system-firmware-autogen repos. Make sure that the branches
in the original locations are pointing to the latest data for the SoC that you are
trying to add.

* Now edit the data-generation utility script `scripts/generate-data.sh` and insert a
new soc case under "gen_file()" for the new SoC common name that you are adding. Make
sure to add the correct SoC common name (instead of "j7xyz", in the below example) and
also add the proper path to the SoC design JSON file from the design team under the
git submodule connected under "src_input" in the system-firmware-autogen link you
already established.
For example:
```
    j7xyz)
    sysfw_soc=j7xyz
    soc_json=$autogen_repo/src_input/csl/j7xyz_dsn/json/J7XYZ.json
    ;;

```

* Make sure to also add the command "gen_files j7xyz" (replace "j7xyz" with your new SoC name)
in `scripts/generate-data.sh` for the new SoC in the section towards the bottom of the script
where similar commands are listed for all the other SoCs when the argument "all" is given at
script execution time.

* Next, go to your local SysConfig tool installation and pick out some random device
name from the list of devices to act as a stand-in for the real SoC name which will
be added later to the SysConfig tool. Make sure it does NOT match any of the other "soc"
names used currently in the `data/SOC.json` file. Copy-paste this name (e.g.,
F280013x, or some such) at the end of the list of names in the "devices" array list in
the file `.metadata/product.json`.

* Add a new SoC section in the file `data/SOC.json` that is copied from one of the other
similar SoC sections in the same file (one that has the most similar architecture).
For example:
```
    {
        "sRAMSize": 8,
        "sRamGranularity": 1,
        "sciClientPrefixReplacement": "TISCI_",
        "main_isolation_hostid": "TISCI_HOST_ID_WKUP_0_R5_0",
        "sciClientSocVersion": "V10",
        "shortName": "j7xyz",
        "soc": "F280013x",
        "wildCardPrivId": 195,
        "pdkUsage": true,
        "mcusdkUsage": true
    },
```
NOTE: replace the value of "sRAMSize" with the total size (in MBs) of MSMC memory on the
SoC (or 0, if no MSMC on the device), replace the value of "main_isolation_hostid" with
the ID of the Host to be allowed to send the main isolation SCI message on the SoC, replace
the "sciClientSoCVersion" value with the PDK folder version number for this SoC, replace
the "shortName" value with the official short name of the SoC (in all lower case), replace
the "soc" value with the name that you used already to add to the "devices" array in the
`.metadata/product.json` file above, and replace any other remaining values that might need
to be updated (uncommon).

* Now create a new SoC subfolder under the `data` folder that has the same "shortName" (e.g., j7xyz)
that you used in the `data/SOC.json` file above. Add a new file to the folder called `DeviceName.json`
and fill it with an empty array: `[]`

* Run the script to generate some initial data files for your new SoC, from the top level of the
repo, like this: `scripts/generate-data.sh j7xyz` (where "j7xyz" is the new SoC common name for
which you want to add the new resource files). If the script is using the proper file path to
your SoC JSON file (set above, earlier), then you should now have the following JSON files
under `data/j7xyz`: DeviceName.json, Firewall.json, Hosts.json, Qos.json and Resources.json.

* Manually add a new file to the `data/j7xyz` folder called BWlimiters.json and fill it with
an empty array: `[]` to start with. Populate this array with information for each rate limiter
available for the SOC. Refer to the file for j784s4 for the parameters expected.

* Now invoke the "gen_initial_user_json()" function in the script "generate-data.sh" in order
to generate a couple of other missing JSON files that we will need. If this is done correctly,
you should then have these additional JSON files in your `data/j7xyz` folder:
HostNames.json and ResDependencies.json.

* However, the file `DeviceName.json` will now have become corrupted (for our usage). So,
simply delete the newly updated `DeviceName.json` and re-create it with nothing but an
empty array in it again: `[]` 

* Now, edit the new file `HostNames.json` and replace all the "displayName" values (manually)
that currently just have the word "FILL" in them with meaningfull Host display names for each
"hostName" below that entry. NOTE: you can reuse any similar display names from one of the other
SoC data directories with a similar SoC architecture.  E.g., use "TI Foundational Security" for
the host "TIFS" and use "A53_0 Secure host" for the host "A53_0", etc.

* After replacing all the values marked "FILL" in the `HostNames.json` file with the actual host display
names, now run the utility script again (make sure it is returned to the normal usage with "gen_files"
function being used in it once again now): `scripts/generate-data.sh j7xyz`
After this, you should open up the file `Hosts.json` in the updated data folder for your new SoC and
you should see the displayName values inserted in each Host section, as new line items -- the names you
had previously entered in the other file `HostNames.json` before this.

* Now it is time to organize your resources into logical groups based on resource types. You will
start with the recently generated file `ResDependencies.json` and you will modify this to organize
your new SoC's resources into groups.
The best way to do this is to start with the layout of groupings in the `ResDependencies.json` file from
another similar SoC in one of the data/soc folders alongside your current SoC data folder.  So,
open up both of these files together and create similar group arrays using the same "groupName" entries
from the other (established) SoC with similar groups of resources.  You might have a few resources
extra (or a few missing) in the new SoC compared to the similar SoC. But, in general, the groupings
should be pretty much the same. You may have to do quite a bit of moving of lines to get into similar
type groupName arrays. This is okay as long as you don't lose any of the resource lines from the new
SoC's set of resources.
NOTE: you will need to add meaningful names not only for the "groupName" entries, but also for the
generic entries shown currently for each of the "utype" values. Replace these with human readable names
from the related SoC `ResDependencies.json` file. You will also need to copy the "copyFromUtype" lines
from the related SoC entries, if those lines exist in the other file. These are necessary to show that
certain resources need to be mapped together with related resources (like Rings with their related DMA
channel types).

* After this grouping is done, you will also have to manually update the Host restrictions on the
interrupt router and interrupt aggregator resources (VINTs, etc.) to represent the actual interrupt
connections that are allowed in the hardware.
See the section below, on "Updating ResDependencies.json", in order to go through this procedure.

* Next, after all manual updates have been made in your new SoC's `ResDependencies.json` file,
you will need to again run the utility script one more time: `scripts/generate-data.sh j7xyz`.
This ensures that all your final updates are translated properly into the output file:
`Resources.json`. Review this output file to ensure that it looks correct after you run the script.
Note that the `Resources.json` will also have many resource ranges (for each resource type) that were
not necessarily present in the `ResDependencies.json` file.

* Finally, you will need to create a new baseline version of the `out/j7xyz-evm.syscfg` file
to load into the SysConfig tool, as described towards the beginning of this README. It is recommended
to start from a copy of a related SoC's .syscfg file and then add (or remove) resource assignments
according to the new SoC's resources, to create a new baseline set of assignments for the new SoC.

### Updating ResDependencies.json  

ResDependencies.json file contains extra restrictions on the resource allocation.
For example all the interrupts may not be available for all the cores. this
information is added in this file. Basic structure of the file can be created
using the autogen script itself(by invoking the gen_initial_user_json function
in generate-data.sh)

To add interrupt related updates, look for interrupt_cfg.rst file in the folder
docs/public/5_soc_doc/<soc> in system-firmware repo. In that
file, search for tables having its name ending with "Output Destinations".
Each table represents one type of resource and its interrupt routing information.
These resources include CMP_EVENT_INTROUTER0, MAIN_GPIOMUX_INTROUTER0,
MCU_MCU_GPIOMUX_INTROUTER0, TIMESYNC_EVENT_INTROUTER0 etc. Ignore the 
TIMESYNC_EVENT_INTROUTER0 and CMP_EVENT_INTROUTER0 resource's table and look for
all other resource's table. Take one resource's table and look for the columns
"Destination Name" and "IR Output Index". One entry in the table represents that
the IR Output Index can be routed to the corresponding host. Select one host
from the "Destination Name" column and see which all "IR Output Index" can be
routed to the host. The IR Output Index might not be contiguous, but to avoid
further complexity, take the largest continuous range of output index that can
be assigned to the host. Similarly check for other hosts and find what ranges of
resource can be assigned to the host. Add this information as a "resRange" entry
with the following fields resStart: the starting index of resource, resCount:
number of resources that can be assigned,restrictHosts: the hosts that can be
assigned the particular range of resource for the corresponding resource, in
ResDependebcies.json file.
For example:
```
    "deviceName": "AM62X_DEV_WKUP_MCU_GPIOMUX_INTROUTER0",
    "subtypeName": "RESASG_SUBTYPE_IR_OUTPUT",
    "utype": "WKUP GPIO Interrupt Router",
    "resRange": [
        {
            "resStart": 6,
            "resCount": 4,
            "restrictHosts": [
                "A53_0",
                "A53_1",
                "A53_2",
                "A53_3"
            ]
        }
    ]
```
when assigning RESASG_SUBTYPE_IR_OUTPUT for A53, resource range from 6 to 4 nos
is a valid range. Adding this JSON entry will ensure that the tool can take care
of the custom range allocation

Similarly search for table's name ending with "Interrupt Destinations" in
interrupt_cfg.rst file mentioned in the above. These includes
DMASS0_INTAGGR_0 Interrupt Aggregator Virtual Interrupt Destinations,
SA3_SS0_INTAGGR_0 Interrupt Aggregator Virtual Interrupt Destinations etc.
These table represent resources related to  Virtual Interrupt. Do the same
procedure for these resources also as mentioned in the above paragraph.
