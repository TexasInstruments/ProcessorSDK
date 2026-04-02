
# Getting Started with MCU PLUS SDK

## Supported HOST environments

- Windows 10 64bit
- Ubuntu 18.04 64bit

## Setup Environment

### Network proxies and tools

1. Refer below page to setup network proxy to work within TI network,
   - https://confluence.itg.ti.com/display/J7TDA4xSW/How+to+setup+TI+network+proxy
   - These are for linux, some of these work for cygwin in windows.

2. Refer below page to install repo tool
   - https://confluence.itg.ti.com/pages/viewpage.action?pageId=141003292

3. Setup Node.js proxy as below to build within TI network,
   - Create a file .npmrc in your OS HOME directory and add below lines to it
   - For Windows users this directory will be C:/Users/<user_name>
        ```
        proxy=http://webproxy.ext.ti.com:80
        https-proxy=http://webproxy.ext.ti.com:80
        https_proxy=http://webproxy.ext.ti.com:80
        ```
4. In case of Git Bash on Windows user will also need to set below in .gitconfig file:
    ```
    [http]
        proxy = http://webproxy.ext.ti.com:80

    [https]
        proxy = http://webproxy.ext.ti.com:80
    ```

### Option 1 - Using Repo Tool (Recommended for Linux users)
Initialize repo tool, git repositories and download all needed components.
1. Set up repo tool.

    ```
    curl https://storage.googleapis.com/git-repo-downloads/repo-1 > ~/bin/repo
    chmod a+x ~/bin/repo
    vi ~/.bashrc
    PATH=$HOME/bin:$PATH
    ```

2. This step does all the hard work to setup a working environment on the system.

    ```
    mkdir -p mcusdk
    cd mcusdk
    repo init -u ssh://git@bitbucket.itg.ti.com/processor-sdk-mcu/repo_manifests.git -m am62ax/mcusdk.xml -b sitara_mpu_master
    .repo/manifests/am62ax/sync.sh
    repo start dev --all
    ```
    Please note using am62ax repo manifest will work for am62x as well.

3. Install nodejs packages in MCU+ SDK folder
    ```
    cd mcu_plus_sdk
    npm ci

**Note** In case you get "SyntaxError: invalid syntax" during repo init, refer https://source.android.com/setup/develop#installing-repo

### Option 2 - Without Repo Tool (Manual Setup, recommended for Windows users)

**NOTE** DO NOT use cygwin for windows, use git for windwows instead, https://git-scm.com/download/win

1. Download and install CCS from here, https://www.ti.com/tool/download/CCSTUDIO
   - Install at default folder,
     - For windows, C:/ti
     - For Linux, $(HOME)/ti

2. Download and install SysConfig from below link
   - Windows: https://software-dl.ti.com/ccs/esd/sysconfig/sysconfig-1.12.1_2446-setup.exe
   - Linux:   https://software-dl.ti.com/ccs/esd/sysconfig/sysconfig-1.12.1_2446-setup.run
   - Install at default folder,
     - For windows, C:/ti
     - For Linux, $(HOME)/ti
   - Update the device data file for AM273x
     - delete the older device data file from SysConfig install dir (C:\ti\sysconfig_1.11.0\dist\deviceData\AM273x)
     - download location: http://bangsdowebsvr01.india.ti.com/PROCESSOR_SDK_RTOS_AUTOMOTIVE/swdownloads/mcu_plus_sdk/deviceData_am273x_sysconfig_1.11.0.zip
     - extract the zip file in deviceData folder of SysConfig install dir (C:\ti\sysconfig_1.11.0\dist\deviceData\)

3. Download and install GCC for Cortex A53 and ARM R5 from below link (only needed for AM64x developers)
   - For Windows: https://developer.arm.com/-/media/Files/downloads/gnu-a/9.2-2019.12/binrel/gcc-arm-9.2-2019.12-mingw-w64-i686-aarch64-none-elf.tar.xz
   - For Linux: https://developer.arm.com/-/media/Files/downloads/gnu-a/9.2-2019.12/binrel/gcc-arm-9.2-2019.12-x86_64-aarch64-none-elf.tar.xz
   - For Windows: https://developer.arm.com/-/media/Files/downloads/gnu-rm/7-2017q4/gcc-arm-none-eabi-7-2017-q4-major-win32.zip
   - For Linux: https://developer.arm.com/-/media/Files/downloads/gnu-rm/7-2017q4/gcc-arm-none-eabi-7-2017-q4-major-linux.tar.bz2
   - Install at default folder,
     - For windows, C:/ti
     - For Linux, $(HOME)/ti

4. Download and install Node.js v12.18.4 LTS
   - https://nodejs.org/en/
   - For windows,
     - Use the installer to download and install, install at default folder
   - For Linux, do below,
        ```
        curl -sL https://raw.githubusercontent.com/creationix/nvm/v0.35.3/install.sh -o install_nvm.sh
        bash install_nvm.sh
        rm -f install_nvm.sh
        source ~/.profile
        nvm install 12.18.4
        ```
   - Add Node.js binary folder to your OS path
   - Test Node.js installation by typing below on the command prompt
        ```
        node -v
        // => v12.18.4

        npm -v
        // => 6.14.6
        ```

5. Download and install doxygen,
   - For windows, tested with 1.8.20
     - Download doxygen for windows
       https://www.doxygen.nl/download.html
     - Install and add the install path, typically, C:/Program Files/doxygen/bin to your windows PATH
   - For linux, tested with Ubuntu 18.04 LTS
     - Download 1.8.15 version
       https://sourceforge.net/projects/doxygen/files/rel-1.8.15/
     - Download doxygen-1.8.xx.linux.bin.tar.gz
     - Untar in any location and add the doxygen-1.8.xx/bin folder to your path
   - Test by doing below on the command prompt
     ```
     doxygen -v
     // => 1.8.xx
     ```

6. Install openssl.
    - In windows,
      - Download and install OpenSSL for windows
        - Download from http://gnuwin32.sourceforge.net/packages/openssl.htm
        - Install to C:\GnuWin32
      - "Skip" the prompts that say it could not add entries to registtry. This is TI IT policy issue.
      - Add path OpenSSL (C:\GnuWin32\bin) to your "Path" variable in windows
    - In Linux,
      - Do below to install openssl
        ```
        sudo apt install openssl
        ```

7. Install Python3
    - Python scripts are used for below functionality in the SDK,
        - Flashing files to the flash on the EVM via UART.
        - Booting application on the EVM via UART
        - SYSFW boardcfg formatting and C header file generation for SYSFW
    - Flashing files is the most popular reason why you would need python, so its strongly
    recommended to install it.
    - In Windows,
        - Install python from, https://www.python.org/downloads/windows/
        - Confirm python is installed by typing below in a command prompt, make sure you see 3.x as the version
            ```
            python --version
            => Python 3.9.1
            ```
        - If above command fails, then add path to Python to your environment "Path" variable, by default python is installed at below
        path
            ```
            C:\Users\{username}\AppData\Local\Programs\Python\Python39
            ```
        - Check if the python package manager "pip" is installed, by default pip should be installed along with python.
            ```
            python -m pip --version
            => pip 21.0.1 from C:\Users\{username}\AppData\Local\Programs\Python\Python39\lib\site-packages\pip (python 3.9)
            ```
    - In Linux,
        - Do below in Linux Ubuntu shell to install python3
            ```
            sudo apt install python3
            ```
        - Check the python version by doing below
            ```
            python3 --version
            ```
        - Check if the python package manager "pip" is installed, by default pip should be installed along with python.
            ```
            pip3 --version
            ```
    - Install below additional packages via "pip" that are needed for the flashing tools.
        ```
        pip3 install pyserial xmodem tqdm --proxy=http://webproxy.ext.ti.com:80
        ```

8. Install Mono Runtime (Linux Only)
    - Mono runtime is required in Linux for creating bootloader images for application binaries.
    - If you haven't installed mono, you are likely to run into this error while building:
      ```
      /bin/sh: 1: mono: not found
      ```
    - Do below in Linux Ubuntu shell to install mono
      ```
      sudo apt install mono-runtime
      ```

9. Create a folder say "workarea" any where on your PC

10. Clone MCU+ SDK git repo into it
    ```
    # MCU+ SDK
    cd workarea
    git clone ssh://git@bitbucket.itg.ti.com/processor-sdk-mcu/mcu_plus_sdk.git
    # ARM CMSIS
    cd mcu_plus_sdk/source
    git clone ssh://git@bitbucket.itg.ti.com/processor-sdk-mcu/cmsis.git
    cd ../
    # FreeRTOS
    cd source/kernel/freertos
    git clone ssh://git@bitbucket.itg.ti.com/processor-sdk-mcu/freertos_posix.git FreeRTOS-POSIX/
    git clone ssh://git@bitbucket.itg.ti.com/processor-sdk-mcu/freertos_kernel.git FreeRTOS-Kernel/
    git clone ssh://git@bitbucket.itg.ti.com/processor-sdk-mcu/freertos_kernel.git FreeRTOS-Kernel-smp/
    cd FreeRTOS-Kernel-smp/
    git checkout -b smp V202110.00-SMP
    cd ../../../../
    # FreeRTOS-FAT
    cd source/fs/freertos_fat
    git clone ssh://git@bitbucket.itg.ti.com/processor-sdk-mcu/freertos_fat.git FreeRTOS-FAT/
    cd ../../../
    # Device Manager, VHWA, DMAUTILS for AM62x and AM62AX
    cd source/drivers
    git clone ssh://git@bitbucket.itg.ti.com/sitmpusw/device_manager.git
    cd device_manager/rm_pm_hal
    git clone ssh://git@bitbucket.itg.ti.com/sysfw/rm_pm_hal.git rm_pm_hal_src
    cd rm_pm_hal_src
    cd ../../../../../

    cd source/drivers
    git clone ssh://git@bitbucket.itg.ti.com/sitmpusw/dmautils.git
    git clone ssh://git@bitbucket.itg.ti.com/processor-sdk-vision/vhwa.git
    cd vhwa
    git checkout sitara_mpu_master
    cd ../../..

    ```

11. Install nodejs packages in MCU+ SDK folder
    ```
    cd mcu_plus_sdk
    npm ci
    ```
## Building with makefiles and using SysConfig

**NOTE** Use "gmake" in windows, add path to gmake present in CCS at C:\ti\ccsxxxx\ccs\utils\bin to your windows PATH
**NOTE** Unless mentioned otherwise, all below commands are invoked from root folder of the "mcu_plus_sdk" git repo.
**NOTE** Current supported device names are am62ax, am62x
         Pass one of these values to "DEVICE="

1. Do below to create makefiles, this step is optional since this is invoked as part of other steps as well,
    ```
    make gen-buildfiles DEVICE=am62x
    ```

2. Do below to see all granular build options
    ```
    make -s help DEVICE=am62x
    ```

3. Do below to build all libraries and ALL examples, valid PROFILE's are "release" or "debug"
    ```
    make -s -j4 clean DEVICE=am62x PROFILE=debug
    make -s -j4 all   DEVICE=am62x PROFILE=debug
    ```

4. Do below to launch SysConfig GUI with specific examples
    ```
    make -s help DEVICE=am62x

    # invoke the required example make command for syscfg GUI,
    gmake -C examples/hello_world/am62x-sk/m4fss0-0_nortos/ti-arm-clang syscfg-gui
    ```

5. Do below to check syscfg sanity test.
    ```
    make -s syscfg-tests DEVICE=am62x

    # Generate syscfg sanity test report by providing below make option,
    gmake syscfg-tests DEVICE=am62x > syscfg_test.txt
    ```
    - Make sure to fix all syscfg sanity test errors, the report is redirected to a file named “syscfg_test.txt”

## More help on building, loading, running, EVM setup, CCS setup, CCS projects, Flashing, SBL, ROV and much more.

Generate the documentation as mentioned in the below section and read more details in the user guide itself.

## Generate Documentation

- Goto mcu_plus_sdk and type below to build the documentation for the device of interest
    ```
    make docs DEVICE=am62x
    ```

- Browse API guide by opening below file for a DEVICE of interest
    ```
    README_FIRST_*.html
    ```

- Also note that code snippets added to “.internal\docs\api_guide\doxy_samples” is valid code that can compile.
  So make sure this code compiles, by fixing any errors found. Above command also compiles the code snippets.

- Make sure to fix all doxygen warnings, all the warning are redirected to a file named “.internal\docs\api_guide\doxy_warnings.txt”
  In ideal case, this file should be empty i.e no warnings from doxygen.

- So when one claims documentation is done, make sure you have run above and there are no compile errors and no doxygen warnings.
  Until then documentation is not done.

## Build Unit Tests

**NOTE**  CCS project spec generation is skipped for unit tests.
**NOTE**  See top level makefile for additional options related to “tests” like clean, help etc

- Units are located in "test/". These are compiled from the top level “makefile” by doing below
    ```
    make tests DEVICE=am62x
    ```

- For granular build options for unit tests, do below to see options for a specific device.
    ```
    cd test/
    make -s -f makefile.am62x help
    ```

## Code Coverage

- Build instrumented code
    ```
    make -s gen-buildfiles INSTRUMENTATION_MODE=enable
    make -s scrub
    make -s libs -j
    # build your application as usual like
    make -s -C examples/drivers/gpio/gpio_input_interrupt/am64x-evm/r5fss0-0_nortos/ti-arm-clang
    ```

- Run the generated binary

- After the execution ends, do below from CCS scripting console
    ```
    loadJSFile("C:\\ti\\mcu_plus_sdk\\.internal\\code_coverage\\clang_coverage_dump.js");
    ```
**NOTE** This points to the R5FSS0-0 core. Edit this in case the app is running from other cores

- This creates the profile count file (gpio_input_interrupt.release.out.cnt) in the binary folder

- Generate coverage report by providing below make option
    ```
    gmake -s -C examples/drivers/gpio/gpio_input_interrupt/am64x-evm/r5fss0-0_nortos/ti-arm-clang coverage
    ```

## Install SysConfig from git (OPTIONAL, NEEDED for ADVANCED USERS ONLY)

**NOTE** It is recommended to use the syscfg installer for now

**NOTE** This has steps for windows and linux, though on windows there could be some issues due to admin permissions needed for some tools.

- Do below steps to clone the SysConfig git repo, do this at same level as the `mcu_plus_sdk` repo
  ```
  # On linux
  git clone ssh://git@bitbucket.itg.ti.com/pmux/ti-pinmux-devtools-lx.git
  cd ti-pinmux-devtools-lx
  git checkout dev
  git submodule init
  git submodule update
  cd ..

  # On windows
  git clone ssh://git@bitbucket.itg.ti.com/pmux/ti-pinmux-devtools.git
  cd ti-pinmux-devtools
  git checkout dev
  git submodule init
  git submodule update
  cd ..

  # On windows or linux
  git clone ssh://git@bitbucket.itg.ti.com/pmux/ti-pinmux-tool.git
  cd ti-pinmux-tool
  # when testing a prototype branch,
  # switch to the branch by doing below,
  # git checkout --track origin/<branch name>
  # instead of below line
  git checkout dev
  git submodule init
  git submodule update
  cd ..

- Do below steps to build pinmux and device data
  ```
  # On linux
  cd ti-pinmux-tool
  source ./setenv.sh
  cd build/scripts/
  ./build_device_data.sh

  # On windows
  cd ti-pinmux-tool
  setenv.bat
  cd build/scripts/
  build_device_data.bat
