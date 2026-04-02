"use strict";

let common = system.getScript("/common");
let pinmux = system.getScript("/drivers/pinmux/pinmux");
let device = common.getDeviceName();
let soc = system.getScript(`/networking/soc/networking_${common.getSocName()}`);
//Get packet pool configuration script
const pktPoolScript = system.getScript("./enet_pkt_pool_config");

const enet_icssg_udma_channel_config = {
    name: "udmaChConfig",
    displayName: "DMA channel config",
	longDescription: "Configuration of Tx/Rx DMA channels",
    collapsed:true,
    config: [

    ],
};

const enet_icssg_system_config = {
    name: "icssgSystemConfig",
    displayName: "System integration config",
    longDescription: "System integration related configuration",
    collapsed:true,
    config: [
        {
            name: "McmEnable",
            description: "Flag to enable multi-client manager. Required for multi-core, multiple Enet client use cases",
            displayName: "Mcm Enable",
            default: false,
        },
        {
            name: "ExternalPhyMgmtEnable",
            description: "Flag to enable phy management in application. The enet driver internal phy functions including phy state machine is bypassed in this mode",
            displayName: "External Phy Management Enable",
            default: false,
        },
        {
            name: "AppLinkUpPortMask",
            description: "Application config to determine which macPorts should be polled for linkup to indicate link is up.Applicable in multi port scenario only",
            displayName: "AppLinkUpPortMask Config",
            default: "ANY_PORT",
            options: [
                {
                    name: "ALL_PORTS",
                },
                {
                    name: "ANY_PORT",
                },
            ],
        },
    ],
};

function getInterfaceName(inst, peripheralName)
{
    return `PRU_${inst.instance}_${peripheralName}`;
}

function getInterfacePinList(inst, peripheralName)
{
    let interfaceName = getInterfaceName(inst, peripheralName);
    let pinList = [];

    if(peripheralName=="IEP")
    {
        pinList.push("EDC_LATCH_IN0");
        pinList.push("EDC_SYNC_OUT0");
    }
    else
    {
        pinList = pinmux.getInterfacePinList(interfaceName);
    }

    return pinList;
}


function getPeripheralRequirements(inst, peripheralName, name)
{
    let interfaceName = getInterfaceName(inst, peripheralName);
    let pinList = getInterfacePinList(inst, peripheralName);
    let resources = [];

    if(name == undefined)
    {
        name = interfaceName;
    }
    else
    {
        name = getInterfaceName(inst, name);
    }

    for(let pin of pinList)
    {
        let pinResource = pinmux.getPinRequirements(interfaceName, pin);

        /* make all pins as "rx" and then override to make "rx" as false as needed  */
        pinmux.setConfigurableDefault( pinResource, "rx", true );

        resources.push( pinResource );
    }

    let peripheralRequirements = {
        name: name,
        displayName: name,
        interfaceName: interfaceName,
        resources: resources,
    };

    return peripheralRequirements;
}

function pinmuxRequirements(inst) {

    let mdio = getPeripheralRequirements(inst, "MDIO");
    let iep = getPeripheralRequirements(inst, "IEP");

    /* set default values for "rx" for different pins, based on use case */
    pinmux.setPeripheralPinConfigurableDefault( mdio, "MDC", "rx", false);
    pinmux.setPeripheralPinConfigurableDefault( iep, "EDC_SYNC_OUT0", "rx", false);

    if( inst.phyToMacInterfaceMode === "MII")
    {
        let mii_g_rt = getPeripheralRequirements(inst, "MII_G_RT");

        pinmux.setPeripheralPinConfigurableDefault( mii_g_rt, "MII0_TXD0", "rx", false);
        pinmux.setPeripheralPinConfigurableDefault( mii_g_rt, "MII0_TXD1", "rx", false);
        pinmux.setPeripheralPinConfigurableDefault( mii_g_rt, "MII0_TXD2", "rx", false);
        pinmux.setPeripheralPinConfigurableDefault( mii_g_rt, "MII0_TXD3", "rx", false);
        pinmux.setPeripheralPinConfigurableDefault( mii_g_rt, "MII0_TXEN", "rx", false);
        pinmux.setPeripheralPinConfigurableDefault( mii_g_rt, "MII1_TXD0", "rx", false);
        pinmux.setPeripheralPinConfigurableDefault( mii_g_rt, "MII1_TXD1", "rx", false);
        pinmux.setPeripheralPinConfigurableDefault( mii_g_rt, "MII1_TXD2", "rx", false);
        pinmux.setPeripheralPinConfigurableDefault( mii_g_rt, "MII1_TXD3", "rx", false);
        pinmux.setPeripheralPinConfigurableDefault( mii_g_rt, "MII1_TXEN", "rx", false);

        return [mdio, iep, mii_g_rt];
    }
    else
    {
        let rgmii1 = getPeripheralRequirements(inst, "RGMII", "RGMII1");
        let rgmii2 = getPeripheralRequirements(inst, "RGMII", "RGMII2");

        return [mdio, iep, rgmii1, rgmii2];
    }
}



function getInterfaceNameList(inst) {

    if(inst.phyToMacInterfaceMode === "MII")
    {
        return [
            getInterfaceName(inst, "MDIO"),
            getInterfaceName(inst, "IEP"),
            getInterfaceName(inst, "MII_G_RT" ),
        ];
    }
    else
    {
        return [
            getInterfaceName(inst, "MDIO"),
            getInterfaceName(inst, "IEP"),
            getInterfaceName(inst, "RGMII1" ),
            getInterfaceName(inst, "RGMII2" ),
        ];

    }
}

function getPeripheralPinNames(inst)
{
    let pinList = [];

    if(inst.phyToMacInterfaceMode === "MII")
    {
        pinList = pinList.concat( getInterfacePinList(inst, "MDIO"),
                        getInterfacePinList(inst, "IEP"),
                        getInterfacePinList(inst, "MII_G_RT" )
        );
    }
    else
    {
        pinList = pinList.concat( getInterfacePinList(inst, "MDIO"),
                        getInterfacePinList(inst, "IEP"),
                        getInterfacePinList(inst, "RGMII" )
        );

    }
    return pinList;
}

function getInstId(instance) {

    let matchInstId;

    const IcssgDualMacInstIdMap = [
        {mode:"DUAL MAC", instance:"ICSSG0", macPort:"ENET_MAC_PORT_1", instId:"0", enetType:"ENET_ICSSG_DUALMAC"},
        {mode:"DUAL MAC", instance:"ICSSG0", macPort:"ENET_MAC_PORT_2", instId:"1", enetType:"ENET_ICSSG_DUALMAC"},
        {mode:"DUAL MAC", instance:"ICSSG1", macPort:"ENET_MAC_PORT_1", instId:"2", enetType:"ENET_ICSSG_DUALMAC"},
        {mode:"DUAL MAC", instance:"ICSSG1", macPort:"ENET_MAC_PORT_2", instId:"3", enetType:"ENET_ICSSG_DUALMAC"},
    ];
    if(instance.mode == "DUAL MAC")
    {
        matchInstId = IcssgDualMacInstIdMap.find(element => element.instance === instance.instance
            && element.macPort === instance.dualMacPortSelected);
    }
    else
    {
        const IcssgSwtInstIdMap = [
                                        {mode:"SWITCH", instance:"ICSSG0",instId:"0", enetType:"ENET_ICSSG_SWITCH"},
                                        {mode:"SWITCH", instance:"ICSSG1",instId:"1", enetType:"ENET_ICSSG_SWITCH"},
                                ];
        matchInstId = IcssgSwtInstIdMap.find(element => element.instance === instance.instance)
    }

    return matchInstId;
}

function isDualMacIfEnabled(instance, icssgInst, macPort) {
    let isEnabled =  (instance.instance === icssgInst)
                     &&
                     (instance.dualMacPortSelected === macPort);
    return isEnabled;
}

function isSwitchIfEnabled(instance, icssgInst) {
    let isEnabled =  ((instance.instance === icssgInst));
    return isEnabled;
}

function isIcssgIfEnabled(instance, mode, icssgInst, macPort) {
    let isEnabled = false;
    if (instance.mode == mode) {
        if (mode === "DUAL MAC") {
            isEnabled = isDualMacIfEnabled(instance, icssgInst, macPort);
        }
        else {
            isEnabled = isSwitchIfEnabled(instance, icssgInst);
        }
    }
    return isEnabled;
}

function getMacPortInfo(instance) {
    let macPortInfo = {numMacPorts:0, macPortList:[]};
    if (instance.mode == "DUAL MAC") {
        macPortInfo.numMacPorts++;
        macPortInfo.macPortList.push(instance.dualMacPortSelected);
    }
    else
    {
        macPortInfo.numMacPorts++;
        macPortInfo.macPortList.push('ENET_MAC_PORT_1');

        macPortInfo.numMacPorts++;
        macPortInfo.macPortList.push('ENET_MAC_PORT_2');
    }
    return macPortInfo;
}

function getPhyMask(instance) {
    let macPortInfo = getMacPortInfo(instance);
    let phyMask = '(' + '0';

    for (var i in macPortInfo.macPortList)
    {
        if (macPortInfo.macPortList[i] == 'ENET_MAC_PORT_1')
        {
            phyMask += ' | ' + '( 1 << ' + instance.phyAddr1 + ')';
        }
        if (macPortInfo.macPortList[i] == 'ENET_MAC_PORT_2')
        {
            phyMask += ' | ' + '( 1 << ' + instance.phyAddr2 + ')';
        }
    }
    phyMask += ')';
    return phyMask;
}

function getPacketsCount(instance, channelType) {
    let totalNumPackets = 0;
    let dma_ch_instances;
    let module_dma_ch;

    if (channelType === "TX")
    {
        dma_ch_instances = instance.txDmaChannel;
        module_dma_ch = system.modules[`/networking/enet_icss/enet_icssg_tx_channel`];
    }
    else
    {
        dma_ch_instances = instance.rxDmaChannel;
        module_dma_ch = system.modules[`/networking/enet_icss/enet_icssg_rx_channel`];
    }

    for(let ch = 0; ch < dma_ch_instances.length; ch++) {
        let ch_instance = dma_ch_instances[ch];
        let ch_config = module_dma_ch.getInstanceConfig(ch_instance);
        totalNumPackets += ch_config.PacketsCount;
    }
    return totalNumPackets;
}

function getChannelCount(instance, channelType) {
    let totalNumChannels = 0;
    let dma_ch_instances;
    let module_dma_ch;

    if (channelType === "TX")
    {
        dma_ch_instances = instance.txDmaChannel;
        module_dma_ch = system.modules[`/networking/enet_icss/enet_icssg_tx_channel`];
    }
    else
    {
        dma_ch_instances = instance.rxDmaChannel;
        module_dma_ch = system.modules[`/networking/enet_icss/enet_icssg_rx_channel`];
    }

    for(let ch = 0; ch < dma_ch_instances.length; ch++) {
        let ch_instance = dma_ch_instances[ch];
        let ch_config = module_dma_ch.getInstanceConfig(ch_instance);
        totalNumChannels++;
    }
    return totalNumChannels;
}

function getTxPacketsCount(instance) {
    return getPacketsCount(instance, "TX");
}

function getRxPacketsCount(instance) {
    return getPacketsCount(instance, "RX");
}

function getTxChannelCount(instance) {
    return getChannelCount(instance, "TX");
}

function getRxChannelCount(instance) {
    return getChannelCount(instance, "RX");
}

function getChannelConfig(instance, channelType, chTypeInstNum) {
    let dma_ch_instances;
    let module_dma_ch;

    if (channelType === "TX")
    {
        dma_ch_instances = instance.txDmaChannel;
        module_dma_ch = system.modules[`/networking/enet_icss/enet_icssg_tx_channel`];
    }
    else
    {
        dma_ch_instances = instance.rxDmaChannel;
        module_dma_ch = system.modules[`/networking/enet_icss/enet_icssg_rx_channel`];
    }
    let channelCfgArray = new Array();


    for(let ch = 0; ch < dma_ch_instances.length; ch++) {
        let ch_instance = dma_ch_instances[ch];
        let ch_config = module_dma_ch.getInstanceConfig(ch_instance);
        channelCfgArray.push(ch_config);
    }
    return channelCfgArray[chTypeInstNum];
}

function getDefaultPacketCount(channelType) {
    let module_dma_ch;

    if (channelType === "TX")
    {
        module_dma_ch = system.modules[`/networking/enet_icss/enet_icssg_tx_channel`];
    }
    else
    {
        module_dma_ch = system.modules[`/networking/enet_icss/enet_icssg_rx_channel`];
    }
    return (module_dma_ch.config.filter(o => o.name === 'PacketsCount'))[0].default;
}


function getCpuID() {
    return system.getScript(`/drivers/soc/drivers_${common.getSocName()}`).getCpuID();
}


function getRxDefaultFlowCount(instance)
{
    let defaultFlowCount = 0;

    for (let chIdx = 0; chIdx < getRxChannelCount(instance); chIdx++)
    {
        defaultFlowCount += (getChannelConfig(instance, "RX", chIdx).useDefaultFlow === true) ? 1 : 0;
    }
    return defaultFlowCount;
    
    
}

function validate(instance, report) {
    pktPoolScript.validate(instance, report);
    if (instance.mode === "SWITCH")
    {
        if (getRxDefaultFlowCount(instance) != 2)
        {
            report.logError(`Both rx flow associated with switch should be designated as default rx flow`, instance, "rxDmaChannel");    
        }

        if ((getRxChannelCount(instance) != 2))
        {
            report.logError(`ICSS switch should have two rx flows`, instance, "rxDmaChannel");    
        }
        else
        {
            let rxCh0 = getChannelConfig(instance, "RX", 0);
            let rxCh1 = getChannelConfig(instance, "RX", 1);

            if ((rxCh0.chIdx ^ rxCh1.chIdx) != 1)
            {
                report.logError(`ICSS switch two rx flows should have chIdx of 0 and 1`, instance, "rxDmaChannel");    
            }
            if (rxCh1.AllocMacAddr === true)
            {
                report.logError(`Dont allocate mac address for ICSS switch second rx flow for rx channel 1`, instance, "rxDmaChannel");    
            }
        }
        
        
    }
    if (instance.mode === "DUAL MAC")
    {
        if (getRxDefaultFlowCount(instance) != 1)
        {
            report.logError(`Rx flow associated with dual mac instance should be designated as default rx flow`, instance, "rxDmaChannel");    
        }
    }

}

function moduleInstances(instance) {

    let channelInstances = new Array();
    let maxCh     = 4;

    channelInstances.push({
        name: "txDmaChannel",
        displayName: "ENET tx dma channel",
        moduleName: `/networking/enet_icss/enet_icssg_tx_channel`,
        useArray: true,
        minInstanceCount: 1,
        maxInstanceCount: maxCh,
        defaultInstanceCount: 1,
        collapsed:false,
        group: "udmaChConfig",
    });

    channelInstances.push({
        name: "rxDmaChannel",
        displayName: "ENET rx dma channel",
        moduleName: `/networking/enet_icss/enet_icssg_rx_channel`,
        useArray: true,
        minInstanceCount: 1,
        maxInstanceCount: maxCh,
        defaultInstanceCount: 1,
        collapsed:false,
        group: "udmaChConfig",
    });


    return (channelInstances);
}

function addSharedModuleInstances(inst) {
    let modInstances = new Array();

    modInstances.push({
        name: "icss",
        displayName: "PRU Configuration",
        moduleName: '/drivers/pruicss/pruicss',
        requiredArgs: {
            instance: inst.instance,
            coreClk: 250*1000000,
        },
    });

    modInstances.push({
        name: "udmaDrv",
        displayName: "Udma",
        moduleName: "/drivers/udma/udma",
        requiredArgs: {
            instance: "PKTDMA_0",
            $name: "CONFIG_UDMA_PKTDMA_0",
        },
    });

    return (modInstances);
}

function getCpuInfo() {
	const cpuInfo = new Map(
                               [
                                 ['CSL_CORE_ID_R5FSS0_0',{subsystem: "R5FSS",
                                  clusternum: "0", core: "0"}],
                                 ['CSL_CORE_ID_R5FSS0_1',{subsystem: "R5FSS",
                                  clusternum: "0", core: "1"}],
                                 ['CSL_CORE_ID_R5FSS1_0',{subsystem: "R5FSS",
                                  clusternum: "1", core: "0"}],
                                 ['CSL_CORE_ID_R5FSS1_1', {subsystem: "R5FSS",
                                  clusternum: "1", core: "1"}],
                               ],
                             );
	return cpuInfo.get(getCpuID());
}

let enet_icss_module_name = "/networking/enet_icss/enet_icss";

let enet_icss_module = {

    displayName: "Enet (ICSS)",
	longDescription: "Driver for Industrial Communication Subsystem - Gigabit (ICSSG) which is std. Ethernet similar to CPSW but TI recommendation is to use ICSSG for Industrial Ethernet use-cases",
    templates: {
        "/drivers/pinmux/pinmux_config.c.xdt": {
            moduleName: enet_icss_module_name,
        },
        "/drivers/system/system_config.h.xdt": {
            driver_config: "/networking/enet_icss/templates/enet_icss.h.xdt",
            moduleName: enet_icss_module_name,
        },
        "/board/board/board_config.h.xdt": {
            board_config: "/networking/enet_icss/templates/enet_board_cfg_am64x_am243x.h.xdt",
            moduleName: enet_icss_module_name,
        },
        "/board/board/board_config.c.xdt": {
            board_config: "/networking/enet_icss/templates/enet_board_cfg_am64x_am243x.c.xdt",
            moduleName: enet_icss_module_name,
        },
        "/networking/common/enet_config.c.xdt": {
            enet_mem_config: "/networking/enet_icss/templates/enet_app_memutils_cfg.c.xdt",
            enet_syscfg_info: "/networking/enet_icss/templates/enet_app_syscfg_info.c.xdt",
            moduleName: enet_icss_module_name,
        },
        "/networking/common/enet_config.h.xdt": {
            enet_config: "/networking/enet_icss/templates/enet_syscfg.h.xdt",
            moduleName: enet_icss_module_name,
        },
        "/networking/common/enet_open.c.xdt": {
            enet_open: "/networking/enet_icss/templates/enet_init.c.xdt",
            moduleName: enet_icss_module_name,
        },
        "/networking/common/enet_open.h.xdt": {
            enet_open: "/networking/enet_icss/templates/enet_init.h.xdt",
            moduleName: enet_icss_module_name,
        },
		"/networking/common/enet_soc.c.xdt": {
		enet_soc: "/networking/enet_icss/templates/enet_soc_cfg_am64x_am243x.c.xdt",
		moduleName: enet_icss_module_name,
        },
    },
    defaultInstanceName: "CONFIG_ENET_ICSS",
    config: [
        {
            name: "instance",
            displayName: "Instance",
            default: "ICSSG1",
            options: [
                {
                    name: "ICSSG0",
                },
                {
                    name: "ICSSG1",
                }
            ],
        },
        {
            name: "mode",
            displayName: "EMAC Mode",
            default: "SWITCH",
            options: [
                {
                    name: "SWITCH",
                },
                {
                    name: "DUAL MAC",
                },

            ],
            onChange: function (inst, ui) {
                /* Init delay applicable only for single master mode */
                if(inst.mode == "SWITCH") {
                    ui.dualMacPortSelected.hidden = true;
                }
                else {
                    ui.dualMacPortSelected.hidden = false;
                }
            },

        },
        {
            name: "phyToMacInterfaceMode",
            displayName: "MII/RGMII",
            default: "MII",
            options: [
                {
                    name: "MII",
                },
                {
                    name: "RGMII",
                },
            ],
        },
        {
            name: "phyAddr1",
            description: "Phy Address of the port in single/dual EMAC mode or Port 1 in Switch mode. Value MUST be between 0 .. 31",
            displayName: "Phy Address 1",
            default: 15,
        },
        {
            name: "phyAddr2",
            description: "Phy Address of the port in single/dual EMAC mode or Port 2 in Switch mode. Value MUST be between 0 .. 31",
            displayName: "Phy Address 2",
            default: 3,
        },
        {
            name: "dualMacPortSelected",
            displayName: "Dual-Mac Mode Port",
            default: 'ENET_MAC_PORT_1',
            hidden: true,
            options: [
                {
                    name: "ENET_MAC_PORT_1",
                },
                {
                    name: "ENET_MAC_PORT_2",
                },
            ],
            description: "Enabled MAC port in Dual mac mode",
        },
        {
            name: "QoS",
            description: "No of QoS level required",
            displayName: "QoS Level",
            default: 8,
            isInteger: true,
            range: [1, 8],
            displayFormat: "dec",
        },
        {
            name: "PremptionEnable",
            description: "Flag to enable premption",
            displayName: "Premption Enable",
            default: false,
        },
        {
            name: "GigabitSupportEnable",
            description: "Decides buffer pool allocation based on interface speed selected",
            displayName: "Gigabit Support",
            default: true,
        },
        enet_icssg_system_config,
        enet_icssg_udma_channel_config,
        pktPoolScript.config,
    ],
    moduleStatic: {
        modules: function(inst) {
            return [{
                name: "system_common",
                moduleName: "/system_common",
            }]
        },
    },
    moduleInstances: moduleInstances,
    sharedModuleInstances: addSharedModuleInstances,
    pinmuxRequirements,
    getInterfaceNameList,
    getPeripheralPinNames,
    getInstId,
    isIcssgIfEnabled,
    getMacPortInfo,
    getPhyMask,
    getCpuID,
    getCpuInfo,
    getTxPacketsCount,
    getRxPacketsCount,
    getRxChannelCount,
    getTxChannelCount,
    getChannelConfig,
    getDefaultPacketCount,
    validate: validate,
};

exports = enet_icss_module;
