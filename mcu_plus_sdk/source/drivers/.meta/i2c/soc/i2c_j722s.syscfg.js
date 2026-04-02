
let common = system.getScript("/common");

let i2c_func_clk = 96000000;

const staticConfig_mcu = [

    {
        name: "I2C0",
        baseAddr: "CSL_I2C0_CFG_BASE",
        intNum: 193,
        eventId: 0,
        funcClk: i2c_func_clk,
        clockIds: [ "TISCI_DEV_I2C0" ],
    },
    {
        name: "I2C1",
        baseAddr: "CSL_I2C1_CFG_BASE",
        intNum: 194,
        eventId: 0,
        funcClk: i2c_func_clk,
        clockIds: [ "TISCI_DEV_I2C1" ],
    },
    {
        name: "I2C2",
        baseAddr: "CSL_I2C2_CFG_BASE",
        intNum: 195,
        eventId: 0,
        funcClk: i2c_func_clk,
        clockIds: [ "TISCI_DEV_I2C2" ],
    },
    {
        name: "I2C3",
        baseAddr: "CSL_I2C3_CFG_BASE",
        intNum: 196,
        eventId: 0,
        funcClk: i2c_func_clk,
        clockIds: [ "TISCI_DEV_I2C3" ],
    },
    {
        name: "I2C4",
        baseAddr: "CSL_I2C4_CFG_BASE",
        intNum: 239,
        eventId: 0,
        funcClk: i2c_func_clk,
        clockIds: [ "TISCI_DEV_I2C4" ],
    },
    {
        name: "MCU_I2C0",
        baseAddr: "CSL_MCU_I2C0_CFG_BASE",
        intNum: 197,
        eventId: 0,
        funcClk: i2c_func_clk,
        clockIds: [ "TISCI_DEV_MCU_I2C0" ],
    },
    {
        name: "WKUP_I2C0",
        baseAddr: "CSL_WKUP_I2C0_CFG_BASE",
        intNum: 190,
        eventId: 0,
        funcClk: i2c_func_clk,
        clockIds: [ "TISCI_DEV_WKUP_I2C0" ],
    },
];

const staticConfig_wkup = [
    {
        name: "I2C0",
        baseAddr: "CSL_I2C0_CFG_BASE",
        intNum: 193,
        eventId: 0,
        funcClk: i2c_func_clk,
        clockIds: [ "TISCI_DEV_I2C0" ],
    },
    {
        name: "I2C1",
        baseAddr: "CSL_I2C1_CFG_BASE",
        intNum: 194,
        eventId: 0,
        funcClk: i2c_func_clk,
        clockIds: [ "TISCI_DEV_I2C1" ],
    },
    {
        name: "I2C2",
        baseAddr: "CSL_I2C2_CFG_BASE",
        intNum: 195,
        eventId: 0,
        funcClk: i2c_func_clk,
        clockIds: [ "TISCI_DEV_I2C2" ],
    },
    {
        name: "I2C3",
        baseAddr: "CSL_I2C3_CFG_BASE",
        intNum: 196,
        eventId: 0,
        funcClk: i2c_func_clk,
        clockIds: [ "TISCI_DEV_I2C3" ],
    },
    {
        name: "MCU_I2C0",
        baseAddr: "CSL_MCU_I2C0_CFG_BASE",
        intNum: 197,
        eventId: 0,
        funcClk: i2c_func_clk,
        clockIds: [ "TISCI_DEV_MCU_I2C0" ],
    },
    {
        name: "WKUP_I2C0",
        baseAddr: "CSL_WKUP_I2C0_CFG_BASE",
        intNum: 190,
        eventId: 0,
        funcClk: i2c_func_clk,
        clockIds: [ "TISCI_DEV_WKUP_I2C0" ],
    },
];

const staticConfig_main = [

    {
        name: "I2C0",
        baseAddr: "CSL_I2C0_CFG_BASE",
        intNum: 193,
        eventId: 0,
        funcClk: i2c_func_clk,
        clockIds: [ "TISCI_DEV_I2C0" ],
    },
    {
        name: "I2C1",
        baseAddr: "CSL_I2C1_CFG_BASE",
        intNum: 194,
        eventId: 0,
        funcClk: i2c_func_clk,
        clockIds: [ "TISCI_DEV_I2C1" ],
    },
    {
        name: "I2C2",
        baseAddr: "CSL_I2C2_CFG_BASE",
        intNum: 195,
        eventId: 0,
        funcClk: i2c_func_clk,
        clockIds: [ "TISCI_DEV_I2C2" ],
    },
    {
        name: "I2C3",
        baseAddr: "CSL_I2C3_CFG_BASE",
        intNum: 196,
        eventId: 0,
        funcClk: i2c_func_clk,
        clockIds: [ "TISCI_DEV_I2C3" ],
    },
    {
        name: "I2C4",
        baseAddr: "CSL_I2C4_CFG_BASE",
        intNum: 239,
        eventId: 0,
        funcClk: i2c_func_clk,
        clockIds: [ "TISCI_DEV_I2C4" ],
    },
    {
        name: "MCU_I2C0",
        baseAddr: "CSL_MCU_I2C0_CFG_BASE",
        intNum: 197,
        eventId: 0,
        funcClk: i2c_func_clk,
        clockIds: [ "TISCI_DEV_MCU_I2C0" ],
    },
    {
        name: "WKUP_I2C0",
        baseAddr: "CSL_WKUP_I2C0_CFG_BASE",
        intNum: 190,
        eventId: 0,
        funcClk: i2c_func_clk,
        clockIds: [ "TISCI_DEV_WKUP_I2C0" ],
    },
];

const staticConfig_c7ss0 = [
    {
        name: "I2C0",
        baseAddr: "CSL_I2C0_CFG_BASE",
        eventId: 256 + (193 - 32),
        intNum: 0,
        funcClk: i2c_func_clk,
        clockIds: [ "TISCI_DEV_I2C0" ],
    },
    {
        name: "I2C1",
        baseAddr: "CSL_I2C1_CFG_BASE",
        eventId: 256 + (194 - 32),
        intNum: 0,
        funcClk: i2c_func_clk,
        clockIds: [ "TISCI_DEV_I2C1" ],
    },
    {
        name: "I2C2",
        baseAddr: "CSL_I2C2_CFG_BASE",
        eventId: 256 + (195 - 32),
        intNum: 0,
        funcClk: i2c_func_clk,
        clockIds: [ "TISCI_DEV_I2C2" ],
    },
    {
        name: "I2C3",
        baseAddr: "CSL_I2C3_CFG_BASE",
        eventId: 256 + (196 - 32),
        intNum: 0,
        funcClk: i2c_func_clk,
        clockIds: [ "TISCI_DEV_I2C3" ],
    },
    {
        name: "MCU_I2C0",
        baseAddr: "CSL_MCU_I2C0_CFG_BASE",
        eventId: 256 + (139 - 32),
        intNum: 0,
        funcClk: i2c_func_clk,
        clockIds: [ "TISCI_DEV_MCU_I2C0" ],
    },
    {
        name: "WKUP_I2C0",
        baseAddr: "CSL_WKUP_I2C0_CFG_BASE",
        eventId: 256 + (197 - 32),
        intNum: 0,
        funcClk: i2c_func_clk,
        clockIds: [ "TISCI_DEV_WKUP_I2C0" ],
    },
];

const staticConfig_c7ss1 = [
    {
        name: "I2C0",
        baseAddr: "CSL_I2C0_CFG_BASE",
        eventId: 256 + (193 - 32),
        intNum: 16,
        funcClk: i2c_func_clk,
        clockIds: [ "TISCI_DEV_I2C0" ],
    },
    {
        name: "I2C1",
        baseAddr: "CSL_I2C1_CFG_BASE",
        eventId: 256 + (194 - 32),
        intNum: 17,
        funcClk: i2c_func_clk,
        clockIds: [ "TISCI_DEV_I2C1" ],
    },
    {
        name: "I2C2",
        baseAddr: "CSL_I2C2_CFG_BASE",
        eventId: 256 + (195 - 32),
        intNum: 18,
        funcClk: i2c_func_clk,
        clockIds: [ "TISCI_DEV_I2C2" ],
    },
    {
        name: "I2C3",
        baseAddr: "CSL_I2C3_CFG_BASE",
        eventId: 256 + (196 - 32),
        intNum: 19,
        funcClk: i2c_func_clk,
        clockIds: [ "TISCI_DEV_I2C3" ],
    },
    {
        name: "MCU_I2C0",
        baseAddr: "CSL_MCU_I2C0_CFG_BASE",
        eventId: 256 + (139 - 32),
        intNum: 20,
        funcClk: i2c_func_clk,
        clockIds: [ "TISCI_DEV_MCU_I2C0" ],
    },
    {
        name: "WKUP_I2C0",
        baseAddr: "CSL_WKUP_I2C0_CFG_BASE",
        intNum: 256 + (197 - 32),
        eventId: 21,
        funcClk: i2c_func_clk,
        clockIds: [ "TISCI_DEV_WKUP_I2C0" ],
    },
];

function getStaticConfigArr() {

    let cpu = common.getSelfSysCfgCoreName();
    let staticConfigArr = staticConfig_mcu;

    if(cpu.match(/mcu-r5f*/))
    {
        staticConfigArr = staticConfig_mcu;
    }
    else if (cpu.match(/wkup-r5f*/))
    {
        staticConfigArr = staticConfig_wkup;
    }
    else if (cpu.match(/main-r5f*/))
    {
        staticConfigArr = staticConfig_main;
    }
    else if (cpu.match(/c75ss0*/))
    {
        staticConfigArr = staticConfig_c7ss0;
    }
    else if (cpu.match(/c75ss1*/))
    {
        staticConfigArr = staticConfig_c7ss1;
    }
    return staticConfigArr;
}

function getIsWkupDomainSupported() {
    let cpu = common.getSelfSysCfgCoreName();

    return true;
}

let soc = {
    getIsWkupDomainSupported,
    getStaticConfigArr,
};

exports = soc;

