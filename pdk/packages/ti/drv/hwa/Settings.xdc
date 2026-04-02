module Settings
{
    /*! This variable is to control the SoC type selection.
     * By default this variable is set to NULL.
     *
     * To use HWA for the selected device, add the following lines to config
     * file and set the deviceType correctly:
     *
     *      var hwaSettings = xdc.useModule ('ti.drv.hwa.Settings');
     *      hwaSettings.socType = "tpr12";
     *      hwaSettings.socType = "awr294x";
     *
     */
    metaonly config string socType = "";

    /*! This variable is to control the device library type selection.
     * By default this variable is set to release.
     *
     * To use the debug/release library, add the following lines to config
     * file and set the library profile accordingly:
     *
     *      var hwaSettings = xdc.useModule ('ti.drv.hwa.Settings');
     *      hwaSettings.libProfile = "debug";
     *
     */
    metaonly config string libProfile = "release";
}

