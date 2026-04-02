This windowed watchdog example consists of four usecases:

 (UC1) Permanent failure (non-windowed mode) - the watchdog is correctly serviced 20
       times and then servicing stops completely (with the watchdog in 100% window mode).
 (UC2) Permanent failure (windowed mode) - the watchdog is correctly serviced 20 times
       and then servicing stops completely (with the watchdog in 50% window mode).
 (UC3) Early key written - the watchdog is correctly serviced 20 times and then a key is
       written to the watchdog before the service window opens (with the watchdog in 50%
       window mode).
 (UC4) Late key written - the watchdog is correctly serviced 20 times and then a key is
       written to the watchdog after the service window closes (with the watchdog in 50%
       window mode).

Each usecase has both a Main domain (mcu2_0) binary for servicing the watchdog and an MCU
domain (mcu1_0) binary for detecting failures; both are combined into a single appimage
for booting via Secondary Bootloader (SBL). Each usecase is built using its own unique
main source files, which differ slightly in their usecase-based macros at the beginning
of each file, which correspond to ifdefs that change execution flow; these source files
are otherwise identical.

To build the example, cd to the pdk/packages/ti/diag/examples/wwdt_example_app directory
and run the following command:

source make_all_usecases.sh