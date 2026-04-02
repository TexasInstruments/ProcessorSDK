module Settings
{
    /*! This variable is to control the SoC type selection.
     * By default this variable is set to NULL.
     *
     * To use canfd for the selected device, add the following lines to config
     * file and set the deviceType correctly:
     *
     *      var canfdSettings = xdc.useModule ('ti.drv.canfd.Settings');
     *      canfdSettings.socType = "tpr12";
     *      canfdSettings.socType = "awr294x";
     *
     */
    metaonly config string socType = "";

    /*! This variable is to control the device library type selection.
     * By default this variable is set to release.
     *
     * To use the debug/release library, add the following lines to config
     * file and set the library profile accordingly:
     *
     *      var canfdSettings = xdc.useModule ('ti.drv.canfd.Settings');
     *      canfdSettings.libProfile = "debug";
     *
     */
    metaonly config string libProfile = "release";
}

