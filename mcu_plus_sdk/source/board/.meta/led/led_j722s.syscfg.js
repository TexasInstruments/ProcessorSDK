
let common = system.getScript("/common");

let led_devices = [
    {
        name        : "GPIO",
        type        : "GPIO",
        i2cAddress  : 0x60,   /* Default address */
        ioIndex     : 0,
    },
    {
        name        : "Ioexp",
        type        : "I2C",
        i2cAddress  : 0x23,   /* Default address */
        ioIndex     : 16,
    },
];

function getConfigArr() {
    return led_devices;
}

exports = {
    getConfigArr,
};
