
let common = system.getScript("/common");

let timerClockSourceConfig_mcu = {
    name: "clkSource",
    displayName: "Input Clock Source",
    default: "HFOSC0_CLKOUT",
    options: [
        {
            "name": "HFOSC0_CLKOUT",
        },
    ],
};

let timerClockSourceConfig_main = {
    name: "clkSource",
    displayName: "Input Clock Source",
    default: "HFOSC0_CLKOUT",
    options: [
        {
            "name": "HFOSC0_CLKOUT",
        },
    ],
};


let timerClockSourceConfig_wkup = {
    name: "clkSource",
    displayName: "Input Clock Source",
    default: "HFOSC0_CLKOUT",
    options: [
        {
            "name": "HFOSC0_CLKOUT",
        },
    ],
};


function getTimerClockSourceValue(instance) {
    let clkSelMuxValue = 0;

    switch(instance.clkSource) {
        default:
        case "HFOSC0_CLKOUT":
            clkSelMuxValue = 0x0;
            break;
    }
    return clkSelMuxValue;
}

function makeInstanceConfig() {
    let config = {};
    let staticConfigArr = getStaticConfigArr();
    let defaultInstanceIndex = getDefaultInstance();

    if(staticConfigArr.length == 0)
        return undefined;

    config.name = "instance";
    config.displayName = "Instance";
    config.description = "Select Instance";
    config.default = staticConfigArr[defaultInstanceIndex].name;
    config.options = [];

    for (let i = 0; i < staticConfigArr.length; i++) {
        let option = {};

        option.name = staticConfigArr[i].name;
        config.options.push(option);
    }

    return config;
}

function getDefaultInstance() {
    let cpu = common.getSelfSysCfgCoreName();
    let defaultInstanceMap = {
        "mcu-r5fss0-0":  0,
        "wkup-r5fss0-0": 0,
        "main-r5fss0-0": 0,
        "c75ss0-0": 0,
        "c75ss1-0": 0,
    }
    return defaultInstanceMap[cpu];
}

function getStaticConfigArr() {
    let cpu = common.getSelfSysCfgCoreName();
    let staticConfigArr;

    if(cpu.match(/mcu-r5fss0-0/)) {
        let staticConfig_mcu_r5f = [];

        for(let i=0; i<1; i++)
        {
            staticConfig_mcu_r5f.push(
                {
                    name: `MCU_TIMER${i}`,
                    timerBaseAddr: 0x04800000 + i*0x10000,
                    timerHwiIntNum: 28 + i,
                    timerInputPreScaler: 1,
                    clkSelMuxAddr: 0x04508060 + 4*i,
                    disableClkSourceConfig: false,
                    lockUnlockDomain: "SOC_DOMAIN_ID_MCU",
                    lockUnlockPartition: 2,
                }
            )
        }
        staticConfigArr = staticConfig_mcu_r5f;
    }
    else if(cpu.match(/wkup-r5fss0-0/)) {
        let staticConfig_wkup_r5f = [];

        for(let i=0; i<1; i++)
        {
            staticConfig_wkup_r5f.push(
                {
                    name: `WKUP_TIMER${i}`,
                    timerBaseAddr: 0x2B100000+ i*0x10000,
                    timerHwiIntNum: 138 + i,
                    timerInputPreScaler: 1,
                    clkSelMuxAddr: 0x430081B0 + 4*i,
                    disableClkSourceConfig: false,
                    lockUnlockDomain: "SOC_DOMAIN_ID_WKUP",
                    lockUnlockPartition: 2,
                }
            )
        }
        staticConfigArr = staticConfig_wkup_r5f;
    }
    else if (cpu.match(/main-r5fss0-0/)){
        let staticConfig_main_r5f = [];
        for(let i=0; i<1; i++)
        {
            staticConfig_main_r5f.push(
                {
                    name: `TIMER${i}`,
                    timerBaseAddr: 0x02400000 + i*0x10000,
                    timerHwiIntNum: 24 + i,
                    timerInputPreScaler: 1,
                    clkSelMuxAddr: 0x001081B0 + 4*i,
                    disableClkSourceConfig: false,
                    lockUnlockDomain: "SOC_DOMAIN_ID_MAIN",
                    lockUnlockPartition: 2,
                }
            )
        }
        staticConfigArr = staticConfig_main_r5f;
    }
    else if(cpu.match(/c75ss0-0/)){
        let staticConfig_c75x = [];

        for(let i=1; i<2; i++)
        {
            staticConfig_c75x.push(
                {
                    name: `TIMER${i}`,
                    timerBaseAddr: 0x02400000 + i*0x10000,
                    timerHwiIntNum: 8 + i,
                    eventId: (152 - 32) + 256 +  i, /* (256 - GIC SPI Intr start, ref: clec_spec am62a_soc_event_out_mapping)*/
                    timerInputPreScaler: 1,
                    clkSelMuxAddr: 0x001081B0 + 4*i,
                    disableClkSourceConfig: false,
                    lockUnlockDomain: "SOC_DOMAIN_ID_MAIN",
                    lockUnlockPartition: 2,
                }
            )
        }
        staticConfigArr = staticConfig_c75x;
    }
    else if(cpu.match(/c75ss1-0/)){
        let staticConfig_c75x = [];

        for(let i=2; i<3; i++)
        {
            staticConfig_c75x.push(
                {
                    name: `TIMER${i}`,
                    timerBaseAddr: 0x02400000 + i*0x10000,
                    timerHwiIntNum: 8 + i,
                    eventId: (152 - 32) + 256 +  i, /* (256 - GIC SPI Intr start, ref: clec_spec am62a_soc_event_out_mapping)*/
                    timerInputPreScaler: 1,
                    clkSelMuxAddr: 0x001081B0 + 4*i,
                    disableClkSourceConfig: false,
                    lockUnlockDomain: "SOC_DOMAIN_ID_MAIN",
                    lockUnlockPartition: 2,
                }
            )
        }
        staticConfigArr = staticConfig_c75x;
    }

    return staticConfigArr;
}

function getTimerClockSourceConfigArr() {
    let cpu = common.getSelfSysCfgCoreName();

    if(cpu.match(/mcu-r5fss0-0/)) {
        return timerClockSourceConfig_mcu;
    }
    if(cpu.match(/wkup-r5fss0-0/)) {
        return timerClockSourceConfig_wkup;
    }
    if(cpu.match(/main-r5fss0-0/) || cpu.match(/c75ss0-0/) || cpu.match(/c75ss1-0/)) {
        return timerClockSourceConfig_main;
    }
}

function getDefaultTimerClockSourceMhz() {
    let cpu = common.getSelfSysCfgCoreName();

    return 25000000;
}

exports = {
    getStaticConfigArr,
    getTimerClockSourceConfigArr,
    getTimerClockSourceValue,
    getDefaultTimerClockSourceMhz,
    makeInstanceConfig,
};
