"use strict";

let common = system.getScript("/common");
let pinmux = system.getScript("/drivers/pinmux/pinmux");
let device = common.getDeviceName();
let soc = system.getScript(`/networking/soc/networking_${common.getSocName()}`);
//Get packet pool configuration script
const pktPoolScript = system.getScript("./enet_pkt_pool_config");
//Get ale configuration script
const aleScript = system.getScript("./enet_cpsw_ale_config");

const enet_cpsw_pinmux_config = {
    name: "pinmuxConfig",
    displayName: "Pinmux config",
	longDescription: "Configuration of pinmux for CPSW",
    collapsed:true,
    config: [

    ],
};

const enet_cpsw_cpdma_channel_config = {
    name: "cpdmaChConfig",
    displayName: "DMA channel config",
	longDescription: "Configuration of Tx/Rx DMA channels",
    collapsed:true,
    config: [

    ],
};

const enet_cpsw_system_config = {
    name: "cpswSystemConfig",
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




function getInterfaceNameList(inst) {
    let driverVer = soc.getDriverVer("enet_cpsw");
    let pinmux_instances;
    let pinmux_module;
    let pinmux_instance;
    let pinmux_config;

    pinmux_instances = inst.pinmux;
    pinmux_module = system.modules[`/networking/enet_cpsw/${driverVer}/enet_cpsw_${driverVer}_pinmux`];
    pinmux_instance = pinmux_instances[0];
    pinmux_config = pinmux_module.getInstanceConfig(pinmux_instance);

    return pinmux_module.getInterfaceNameList(pinmux_config);
}

function getPeripheralPinNames(inst)
{
    let driverVer = soc.getDriverVer("enet_cpsw");
    let pinmux_instances;
    let pinmux_module;
    let pinmux_instance;
    let pinmux_config;

    pinmux_instances = inst.pinmux;
    pinmux_module = system.modules[`/networking/enet_cpsw/${driverVer}/enet_cpsw_${driverVer}_pinmux`];
    pinmux_instance = pinmux_instances[0];
    pinmux_config = pinmux_module.getInstanceConfig(pinmux_instance);

    return pinmux_module.getPeripheralPinNames(pinmux_config);
}

let cpsw_input_clk_freq = 200000000;

const enet_config = [
    {
        clockIds        : [ "SOC_RcmPeripheralId_CPTS" ],
        clockFrequencies: [
            {
                moduleId: "SOC_RcmPeripheralId_CPTS",
                clkId   : "SOC_RcmPeripheralClockSource_SYS_CLK",
                clkRate : cpsw_input_clk_freq,
            },
        ],
    },
];

function getClockEnableIds(instance) {
    let instConfig = enet_config;
    return instConfig[0].clockIds;
}

function getClockFrequencies(inst) {

    let instConfig = enet_config;

    return instConfig[0].clockFrequencies;
}

function getDmaInterface(instance) {
    let cpswInstInfo = getCpswInstInfo(instance);
    return cpswInstInfo.dmaIf;
}

function getCpswInstInfo(instance) {
    const cpswInstInfoMap = new Map(
                               [
                                 ['awr294x',{enetType: 'ENET_CPSW_2G', numMacPorts: '1', instId: '0', dmaIf:'ENET_SOC_HOSTPORT_DMA_TYPE_CPDMA', macPortList:['ENET_MAC_PORT_1']}],
                                 ['am273x', {enetType: 'ENET_CPSW_2G', numMacPorts: '1', instId: '0', dmaIf:'ENET_SOC_HOSTPORT_DMA_TYPE_CPDMA', macPortList:['ENET_MAC_PORT_1']}],
                                 ['am263x',{enetType: 'ENET_CPSW_3G', numMacPorts: '2', instId: '0', dmaIf:'ENET_SOC_HOSTPORT_DMA_TYPE_CPDMA', macPortList:['ENET_MAC_PORT_1', 'ENET_MAC_PORT_2']}],
                                 ['am243x',{enetType: 'ENET_CPSW_3G', numMacPorts: '2', instId: '0', dmaIf:'ENET_SOC_HOSTPORT_DMA_TYPE_UDMA', macPortList:['ENET_MAC_PORT_1', 'ENET_MAC_PORT_2']}],
                                 ['am64x',{enetType: 'ENET_CPSW_3G', numMacPorts: '2', instId: '0', dmaIf:'ENET_SOC_HOSTPORT_DMA_TYPE_UDMA', macPortList:['ENET_MAC_PORT_1', 'ENET_MAC_PORT_2']}],
                               ],
                             );
    let instInfo =  cpswInstInfoMap.get(common.getSocName());
    instInfo.macPortList = instInfo.macPortList.filter(function(macPort, index,arr){
        let includeEntry = true;
        if ((macPort === 'ENET_MAC_PORT_1') && (instance.DisableMacPort1 === true))
        {
            includeEntry = false;
        }
        if ((macPort === 'ENET_MAC_PORT_2') && (instance.DisableMacPort2 === true))
        {
            includeEntry = false;
        }
        return includeEntry;
    });
    instInfo.numMacPorts = instInfo.macPortList.length;
    return instInfo;
}

function getBoardConfigTemplateInfo() {
    const boardConfigTemplate = new Map(
                               [
                                 ['am64x',{Cfile: "/networking/enet_cpsw/templates/am64x_am243x/enet_board_cfg.c.xdt",
                                  Header: "/networking/enet_cpsw/templates/am64x_am243x/enet_board_cfg.h.xdt"}],
                                 ['am243x',{Cfile: "/networking/enet_cpsw/templates/am64x_am243x/enet_board_cfg.c.xdt",
                                  Header: "/networking/enet_cpsw/templates/am64x_am243x/enet_board_cfg.h.xdt"}],
                                 ['awr294x',{Cfile: "/networking/enet_cpsw/templates/awr294x/enet_board_cfg.c.xdt",
                                  Header: "/networking/enet_cpsw/templates/awr294x/enet_board_cfg.h.xdt"}],
                                 ['am273x', {Cfile: "/networking/enet_cpsw/templates/am273x/enet_board_cfg.c.xdt",
                                 Header: "/networking/enet_cpsw/templates/am273x/enet_board_cfg.h.xdt"}],
                                 ['am263x',{Cfile: "/networking/enet_cpsw/templates/am263x/enet_board_cfg.c.xdt",
                                 Header: "/networking/enet_cpsw/templates/am263x/enet_board_cfg.h.xdt"}],
                               ],
                             );
    return boardConfigTemplate.get(common.getSocName());
}


function getSocConfigTemplateInfo() {
    const socConfigTemplate = new Map(
                               [
                                 ['am64x',{Cfile: "/networking/enet_cpsw/templates/am64x_am243x/enet_soc_cfg.c.xdt"}],
                                 ['am243x',{Cfile: "/networking/enet_cpsw/templates/am64x_am243x/enet_soc_cfg.c.xdt"}],
                                 ['awr294x',{Cfile: "/networking/enet_cpsw/templates/awr294x/enet_soc_cfg.c.xdt"}],
                                 ['am273x', {Cfile: "/networking/enet_cpsw/templates/am273x/enet_soc_cfg.c.xdt"}],
                                 ['am263x',{Cfile: "/networking/enet_cpsw/templates/am263x/enet_soc_cfg.c.xdt"}],
                               ],
                             );
    return socConfigTemplate.get(common.getSocName());
}

function getPhyMask(instance) {
    let cpswInstInfo = getCpswInstInfo(instance);
    let phyMask = '(' + '0';

    for (var i in cpswInstInfo.macPortList)
    {
        if (cpswInstInfo.macPortList[i] == 'ENET_MAC_PORT_1')
        {
            phyMask += ' | ' + '(1 << ' + instance.phyAddr1 + ' )';
        }
        if (cpswInstInfo.macPortList[i] == 'ENET_MAC_PORT_2')
        {
            phyMask += ' | ' + '(1 << ' + instance.phyAddr2 + ' )';
        }
    }
    phyMask += ')';
    return phyMask;
}

function getPacketsCount(instance, channelType) {
    let totalNumPackets = 0;
    let driverVer = soc.getDriverVer("enet_cpsw");
    let dma_ch_instances;
    let module_dma_ch;

    if (channelType === "TX")
    {
        dma_ch_instances = instance.txDmaChannel;
        module_dma_ch = system.modules[`/networking/enet_cpsw/${driverVer}/enet_cpsw_${driverVer}_tx_channel`];
    }
    else
    {
        dma_ch_instances = instance.rxDmaChannel;
        module_dma_ch = system.modules[`/networking/enet_cpsw/${driverVer}/enet_cpsw_${driverVer}_rx_channel`];
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
    let driverVer = soc.getDriverVer("enet_cpsw");
    let dma_ch_instances;
    let module_dma_ch;

    if (channelType === "TX")
    {
        dma_ch_instances = instance.txDmaChannel;
        module_dma_ch = system.modules[`/networking/enet_cpsw/${driverVer}/enet_cpsw_${driverVer}_tx_channel`];
    }
    else
    {
        dma_ch_instances = instance.rxDmaChannel;
        module_dma_ch = system.modules[`/networking/enet_cpsw/${driverVer}/enet_cpsw_${driverVer}_rx_channel`];
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

function getNumCpdmaDesc(instance) {
    const desc2PacketScalingFactor = 2;
    let cpdmaNumDesc = getRxPacketsCount(instance) + getTxPacketsCount(instance);
    
    cpdmaNumDesc *= desc2PacketScalingFactor;
    return  cpdmaNumDesc;
}

function getChannelConfig(instance, channelType, chTypeInstNum) {
    let driverVer = soc.getDriverVer("enet_cpsw");
    let dma_ch_instances;
    let module_dma_ch;

    if (channelType === "TX")
    {
        dma_ch_instances = instance.txDmaChannel;
        module_dma_ch = system.modules[`/networking/enet_cpsw/${driverVer}/enet_cpsw_${driverVer}_tx_channel`];
    }
    else
    {
        dma_ch_instances = instance.rxDmaChannel;
        module_dma_ch = system.modules[`/networking/enet_cpsw/${driverVer}/enet_cpsw_${driverVer}_rx_channel`];
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
    let driverVer = soc.getDriverVer("enet_cpsw");
    let module_dma_ch;

    if (channelType === "TX")
    {
        module_dma_ch = system.modules[`/networking/enet_cpsw/${driverVer}/enet_cpsw_${driverVer}_tx_channel`];
    }
    else
    {
        module_dma_ch = system.modules[`/networking/enet_cpsw/${driverVer}/enet_cpsw_${driverVer}_rx_channel`];
    }
    return (module_dma_ch.config.filter(o => o.name === 'PacketsCount'))[0].default;
}


function getCpuID() {
    return system.getScript(`/drivers/soc/drivers_${common.getSocName()}`).getCpuID();
}

function validate(instance, report) {
    pktPoolScript.validate(instance, report);
    aleScript.validate(instance, report);

}

function moduleInstances(instance) {

    let channelInstances = new Array();
    let driverVer = soc.getDriverVer("enet_cpsw");
    let maxCh     = 8;

    channelInstances.push({
        name: "txDmaChannel",
        displayName: "ENET tx dma channel",
        moduleName: `/networking/enet_cpsw/${driverVer}/enet_cpsw_${driverVer}_tx_channel`,
        useArray: true,
        minInstanceCount: 1,
        maxInstanceCount: maxCh,
        defaultInstanceCount: 1,
        collapsed:false,
        group: "cpdmaChConfig",
    });

    channelInstances.push({
        name: "rxDmaChannel",
        displayName: "ENET rx dma channel",
        moduleName: `/networking/enet_cpsw/${driverVer}/enet_cpsw_${driverVer}_rx_channel`,
        useArray: true,
        minInstanceCount: 1,
        maxInstanceCount: maxCh,
        defaultInstanceCount: 1,
        collapsed:false,
        group: "cpdmaChConfig",
    });

    channelInstances.push({
        name: "pinmux",
        displayName: "CPSW pinmux",
        moduleName: `/networking/enet_cpsw/${driverVer}/enet_cpsw_${driverVer}_pinmux`,
        useArray: true,
        minInstanceCount: 1,
        maxInstanceCount: 1,
        defaultInstanceCount: 1,
        collapsed:false,
        group: "pinmuxConfig",
    });

    return (channelInstances);
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

let enet_cpsw_module_name = "/networking/enet_cpsw/enet_cpsw";

let enet_cpsw_module = {

    displayName: "Enet (CPSW)",
    longDescription: "Driver for Common Port SWitch (CPSW). Support MAC, Switch and used in auto and industrial Ethernet to run TCP/IP, AVB etc. applications. TSN is supported via CPSW",
    templates: {
        "/drivers/pinmux/pinmux_config.c.xdt": {
            moduleName: enet_cpsw_module_name,
        },
        "/drivers/system/system_config.h.xdt": {
            driver_config: "/networking/enet_cpsw/templates/enet_cpsw_v3.h.xdt",
            moduleName: enet_cpsw_module_name,
        },
        "/drivers/system/power_clock_config.c.xdt": {
            moduleName: enet_cpsw_module_name,
        },
        "/board/board/board_config.h.xdt": {
            board_config: getBoardConfigTemplateInfo().Header,
            moduleName: enet_cpsw_module_name,
        },
        "/board/board/board_config.c.xdt": {
            board_config: getBoardConfigTemplateInfo().Cfile,
            moduleName: enet_cpsw_module_name,
        },
        "/networking/common/enet_config.c.xdt": {
            enet_mem_config: "/networking/enet_cpsw/templates/enet_app_memutils_cfg_cpdma.c.xdt",
            enet_syscfg_info: "/networking/enet_cpsw/templates/enet_app_syscfg_info.c.xdt",
            moduleName: enet_cpsw_module_name,
        },
        "/networking/common/enet_config.h.xdt": {
            enet_config: "/networking/enet_cpsw/templates/enet_syscfg.h.xdt",
            moduleName: enet_cpsw_module_name,
        },
        "/networking/common/enet_open.c.xdt": {
            enet_open: "/networking/enet_cpsw/templates/enet_init.c.xdt",
            enet_init_config: "/networking/enet_cpsw/templates/enet_app_cpsw_cfg.c.xdt",
            moduleName: enet_cpsw_module_name,
        },
        "/networking/common/enet_open.h.xdt": {
            enet_open: "/networking/enet_cpsw/templates/enet_init.h.xdt",
            moduleName: enet_cpsw_module_name,
        },
        "/networking/common/enet_soc.c.xdt": {
            enet_soc: getSocConfigTemplateInfo().Cfile,
            moduleName: enet_cpsw_module_name,
        },
    },
    defaultInstanceName: "CONFIG_ENET_CPSW",
    config: [
        {
            name: "phyAddr1",
            description: "Phy Address of the port 1. Value MUST be between 0 .. 31",
            displayName: "Phy Address",
            default: 0,
        },
        {
            name: "phyAddr2",
            description: "Phy Address of the port 2. Value MUST be between 0 .. 31",
            displayName: "Phy Address",
            default: 0,
        },
        {
            name: "MdioModeManual",
            description: "Flag to force MDIO manual Mode, where MDIO pins are manually controlled bypassing MDIO IP state machine",
            displayName: "Mdio Manual Mode Enable",
            default: true,
        },
        {
            name: "DisableMacPort1",
            description: "Flag to selectively disable MACport1. For CPSW3G both external mac ports are enabled by default. Application may selectively choose to disable some external ports",
            displayName: "Disable Mac Port1",
            default: false,
        },
        {
            name: "DisableMacPort2",
            description: "Flag to selectively disable MACport2. For CPSW3G both external mac ports are enabled by default. Application may selectively choose to disable some external ports",
            displayName: "Disable Mac Port2",
            default: false,
        },
        {
            name: "BoardType",
            description: "Board selection for AM263x",
            displayName: "BoardType",
            default: "am263x-cc",
            options: [
                {
                    name: "am263x-cc",
                },
                {
                    name: "am263x-lp",
                },
            ],
        },
        enet_cpsw_system_config,
        enet_cpsw_cpdma_channel_config,
        pktPoolScript.config,
        aleScript.config,
        enet_cpsw_pinmux_config,
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
    getInterfaceNameList,
    getPeripheralPinNames,
    getClockEnableIds,
    getClockFrequencies,
    getDmaInterface,
    getCpswInstInfo,
    getPhyMask,
    getBoardConfigTemplateInfo,
    getCpuID,
    getCpuInfo,
    getSocConfigTemplateInfo,
    getTxPacketsCount,
    getRxPacketsCount,
    getRxChannelCount,
    getTxChannelCount,
    getNumCpdmaDesc,
    getChannelConfig,
    getDefaultPacketCount,
    validate: validate,
};

exports = enet_cpsw_module;
