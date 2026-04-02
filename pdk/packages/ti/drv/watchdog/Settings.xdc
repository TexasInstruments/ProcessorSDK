module Settings
{
    /*! This variable is to control the SoC type selection.
     * By default this variable is set to NULL.
     *
     * To use WATCHDOG for the selected device, add the following lines to config
     * file and set the deviceType correctly:
     *
     *      var watchdogSettings = xdc.useModule ('ti.drv.watchdog.Settings');
     *      watchdogSettings.socType = "tpr12";
     *      watchdogSettings.socType = "awr294x";
     *
     */
    metaonly config string socType = "";

    /*! This variable is to control the device library type selection.
     * By default this variable is set to release.
     *
     * To use the debug/release library, add the following lines to config
     * file and set the library profile accordingly:
     *
     *      var watchdogSettings = xdc.useModule ('ti.drv.watchdog.Settings');
     *      watchdogSettings.libProfile = "debug";
     *
     */
    metaonly config string libProfile = "release";
}

