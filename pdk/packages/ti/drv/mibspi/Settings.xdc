module Settings
{
    /*! This variable is to control the SoC type selection.
     * By default this variable is set to NULL.
     *
     * To use MIBSPI for the selected device, add the following lines to config
     * file and set the deviceType correctly:
     *
     *      var mibspiSettings = xdc.useModule ('ti.drv.mibspi.Settings');
     *      mibspiSettings.socType = "tpr12";
     *      mibspiSettings.socType = "awr294x";
     *
     */
    metaonly config string socType = "";

    /*! This variable is to control the device library type selection.
     * By default this variable is set to release.
     *
     * To use the debug/release library, add the following lines to config
     * file and set the library profile accordingly:
     *
     *      var mibspiSettings = xdc.useModule ('ti.drv.mibspi.Settings');
     *      mibspiSettings.libProfile = "debug";
     *
     */
    metaonly config string libProfile = "release";
}

