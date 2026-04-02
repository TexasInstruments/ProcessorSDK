module Settings
{
    /*! This variable is to control the SoC type selection.
     * By default this variable is set to NULL.
     *
     * To use ADCBUF for the selected device, add the following lines to config
     * file and set the deviceType correctly:
     *
     *      var adcbufSettings = xdc.useModule ('ti.drv.adcbuf.Settings');
     *      adcbufSettings.socType = "awr294x";
     *
     */
    metaonly config string socType = "";

    /*! This variable is to control the device library type selection.
     * By default this variable is set to release.
     *
     * To use the debug/release library, add the following lines to config
     * file and set the library profile accordingly:
     *
     *      var adcbufSettings = xdc.useModule ('ti.drv.adcbuf.Settings');
     *      adcbufSettings.libProfile = "debug";
     *
     */
    metaonly config string libProfile = "release";
}

