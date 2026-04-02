const driver = require("./driver.js");
const common = require("./common.js");
const clk_dev = require("./clk_dev.js");
const clk_div = require("./clk_div.js");
const clk_hfosc0 = require("./clk_hfosc0.js");
const clk_lfosc = require("./clk_lfosc.js");
const clk_fixed = require("./clk_fixed.js");
const clk_mux = require("./clk_mux.js");
const clk_pll = require("./clk_pll.js");
const clk_pllctrl = require("./clk_pllctrl.js");
const lpsc = require("./lpsc.js");
const pll = require("./pll.js");
const SocData = require("./soc_data.js");

module.exports = {
    driver,
    common,
    clk_dev,
    clk_div,
    clk_hfosc0,
    clk_fixed,
    clk_mux,
    clk_pll,
    clk_pllctrl,
    lpsc,
    pll,
    SocData,
};
