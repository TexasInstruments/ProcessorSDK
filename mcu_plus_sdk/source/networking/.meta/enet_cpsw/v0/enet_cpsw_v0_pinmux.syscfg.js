
let common = system.getScript("/common");
let pinmux = system.getScript("/drivers/pinmux/pinmux");
let device = common.getDeviceName();
let soc = system.getScript(`/networking/soc/networking_${common.getSocName()}`);

function getInstanceConfig(moduleInstance) {
    return {
        ...moduleInstance,
    };
};

function getPeripheralRequirements(inst, peripheralName)
{
    let interfaceName = getInterfaceName(inst, peripheralName);
    let resources = [];
    let pinResource = {};

    if(peripheralName == "RGMII")
    {
        pinResource = pinmux.getPinRequirements(interfaceName, "RD0", "RD0");
        pinmux.setConfigurableDefault( pinResource, "rx", true );
        resources.push( pinResource);
        pinResource = pinmux.getPinRequirements(interfaceName, "RD1", "RD1");
        pinmux.setConfigurableDefault( pinResource, "rx", true );
        resources.push( pinResource);
        pinResource = pinmux.getPinRequirements(interfaceName, "RD2", "RD2");
        pinmux.setConfigurableDefault( pinResource, "rx", true );
        resources.push( pinResource);
        pinResource = pinmux.getPinRequirements(interfaceName, "RD3", "RD3");
        pinmux.setConfigurableDefault( pinResource, "rx", true );
        resources.push( pinResource);
        pinResource = pinmux.getPinRequirements(interfaceName, "RX_CTL", "RX_CTL");
        pinmux.setConfigurableDefault( pinResource, "rx", true );
        resources.push( pinResource);
        pinResource = pinmux.getPinRequirements(interfaceName, "RXC", "RX_RXC");
        pinmux.setConfigurableDefault( pinResource, "rx", true );
        resources.push( pinResource);
        pinResource = pinmux.getPinRequirements(interfaceName, "TD0", "TD0");
        pinmux.setConfigurableDefault( pinResource, "rx", true );
        resources.push( pinResource);
        pinResource = pinmux.getPinRequirements(interfaceName, "TD1", "TD1");
        pinmux.setConfigurableDefault( pinResource, "rx", true );
        resources.push( pinResource);
        pinResource = pinmux.getPinRequirements(interfaceName, "TD2", "TD2");
        pinmux.setConfigurableDefault( pinResource, "rx", true );
        resources.push( pinResource);
        pinResource = pinmux.getPinRequirements(interfaceName, "TD3", "TD3");
        pinmux.setConfigurableDefault( pinResource, "rx", true );
        resources.push( pinResource);
        pinResource = pinmux.getPinRequirements(interfaceName, "TX_CTL", "TX_CTL");
        pinmux.setConfigurableDefault( pinResource, "rx", true );
        resources.push( pinResource);
        pinResource = pinmux.getPinRequirements(interfaceName, "TXC", "TX_RXC");
        pinmux.setConfigurableDefault( pinResource, "rx", true );
        resources.push( pinResource);
    }
    else if(peripheralName == "RMII1")
    {
        pinResource = pinmux.getPinRequirements(interfaceName, "RMII1_RXD0", "RXD0");
        pinmux.setConfigurableDefault( pinResource, "rx", true );
        resources.push( pinResource);
        pinResource = pinmux.getPinRequirements(interfaceName, "RMII1_RXD1", "RXD1");
        pinmux.setConfigurableDefault( pinResource, "rx", true );
        resources.push( pinResource);
        pinResource = pinmux.getPinRequirements(interfaceName, "RMII1_RX_ER", "RX_ER");
        pinmux.setConfigurableDefault( pinResource, "rx", true );
        resources.push( pinResource);
        pinResource = pinmux.getPinRequirements(interfaceName, "RMII1_TXD0", "TXD0");
        pinmux.setConfigurableDefault( pinResource, "rx", true );
        resources.push( pinResource);
        pinResource = pinmux.getPinRequirements(interfaceName, "RMII1_TXD1", "TXD1");
        pinmux.setConfigurableDefault( pinResource, "rx", true );
        resources.push( pinResource);
        pinResource = pinmux.getPinRequirements(interfaceName, "RMII1_CRS_DV", "CRS_DV");
        pinmux.setConfigurableDefault( pinResource, "rx", true );
        resources.push( pinResource);
    }
	else if(peripheralName == "RMII2")
    {
        pinResource = pinmux.getPinRequirements(interfaceName, "RMII2_RXD0", "RXD0");
        pinmux.setConfigurableDefault( pinResource, "rx", true );
        resources.push( pinResource);
        pinResource = pinmux.getPinRequirements(interfaceName, "RMII2_RXD1", "RXD1");
        pinmux.setConfigurableDefault( pinResource, "rx", true );
        resources.push( pinResource);
        pinResource = pinmux.getPinRequirements(interfaceName, "RMII2_RX_ER", "RX_ER");
        pinmux.setConfigurableDefault( pinResource, "rx", true );
        resources.push( pinResource);
        pinResource = pinmux.getPinRequirements(interfaceName, "RMII2_TXD0", "TXD0");
        pinmux.setConfigurableDefault( pinResource, "rx", true );
        resources.push( pinResource);
        pinResource = pinmux.getPinRequirements(interfaceName, "RMII2_TXD1", "TXD1");
        pinmux.setConfigurableDefault( pinResource, "rx", true );
        resources.push( pinResource);
        pinResource = pinmux.getPinRequirements(interfaceName, "RMII2_CRS_DV", "CRS_DV");
        pinmux.setConfigurableDefault( pinResource, "rx", true );
        resources.push( pinResource);
    }
    else
    {
        let pinList = getInterfacePinList(inst, interfaceName);
        for(let pin of pinList)
        {
            pinResource = pinmux.getPinRequirements(interfaceName, pin);

            /* make all pins as "rx" and then override to make "rx" as false as needed  */
            pinmux.setConfigurableDefault( pinResource, "rx", true );

            resources.push( pinResource );
        }
    }

    let peripheralRequirements = {
        name: interfaceName,
        displayName: interfaceName,
        interfaceName: interfaceName,
        resources: resources,
    };

    return peripheralRequirements;
}

function getInterfaceName(inst, peripheralName)
{
    return `${peripheralName}`;
}

function getInterfacePinList(inst, peripheralName)
{
    let interfaceName = getInterfaceName(inst, peripheralName);
    let pinList = [];

    pinList = pinmux.getInterfacePinList(interfaceName);

    return pinList;
}


function pinmuxRequirements(inst) {


    if( inst.phyToMacInterfaceMode === "RMII")
    {
        let rmii = getPeripheralRequirements(inst, "RMII");

        pinmux.setPeripheralPinConfigurableDefault( rmii, "RMII0_TXD0", "rx", false);
        pinmux.setPeripheralPinConfigurableDefault( rmii, "RMII0_TXD1", "rx", false);
        pinmux.setPeripheralPinConfigurableDefault( rmii, "RMII0_TXEN", "rx", false);
        pinmux.setPeripheralPinConfigurableDefault( rmii, "RMII1_TXD0", "rx", false);
        pinmux.setPeripheralPinConfigurableDefault( rmii, "RMII1_TXD1", "rx", false);
        pinmux.setPeripheralPinConfigurableDefault( rmii, "RMII1_TXEN", "rx", false);

        return [rmii];
    }
    else
    {
        let rgmii = getPeripheralRequirements(inst, "RGMII");

        return [rgmii];
    }
}

function getInterfaceNameList(inst) {

    if(inst.phyToMacInterfaceMode === "RMII")
    {
        return [
            getInterfaceName(inst, "RMII" ),
        ];
    }
    else
    {
        return [
            getInterfaceName(inst, "RGMII" ),
        ];

    }
}

function getPeripheralPinNames(inst)
{
    let pinList = [];

    if(inst.phyToMacInterfaceMode === "RMII")
    {
        pinList = getInterfacePinList(inst, "RMII" );
    }
    else
    {
        pinList = getInterfacePinList(inst, "RGMII" );

    }
    return pinList;
}


let enet_cpsw_pinmux_module = {
    displayName: "CPSW pinmux config",
    longDescription: `This configures CPSW module pinmux`,
    alwaysShowLongDescription: false,
    defaultInstanceName: "ENET_CPSW_PINMUX",
    config: [
        {    
            name: "phyToMacInterfaceMode",
            displayName: "RMII/RGMII",
            default: "RMII",
            options: [
                {
                    name: "RMII",
                },
                {
                    name: "RGMII",
                },
            ],
        },
    ],
    getInstanceConfig,
    pinmuxRequirements,
    getInterfaceNameList,
    getPeripheralPinNames,

};


exports = enet_cpsw_pinmux_module;
