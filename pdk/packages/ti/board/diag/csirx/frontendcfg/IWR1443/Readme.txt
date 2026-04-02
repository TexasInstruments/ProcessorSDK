The binaries are for configuration of IWR1443 front end to match the configuration
expected by the TPR12 CSIRX Diag app.

tpr12_csi_stream_tx.xer4f should be run from r4f on the FE device.

This binary is generated from mmWaveSDK release 3.4 with customizations specific to TPR12 EVM.

Follow below steps to build the mmWaveSDK to create the binaries
-----------------------------------------------------------------
1. Update the mmwave device type in setenv.bat
    $ cd <path_to_mmwave_sdk>/packages/scripts/windows
    Update the below line in the setenv.bat file
    set MMWAVE_SDK_DEVICE=iwr14xx
2. Execute the below commands to setup environmen variable
    $ setenv.bat
3. Execute the below commands to build the CSIStream tx application
    $ cd ..\..\ti\drivers\test\csi_stream
    $ gmake csiStreamTxClean
    $ gmake csiStreamTx
