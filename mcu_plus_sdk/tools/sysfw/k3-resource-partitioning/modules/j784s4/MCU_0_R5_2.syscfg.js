
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "Cortex R5 context 2 on MCU island",
  "Security": "Non Secure",
  "displayName": "MCU R5F core1 NonSecure host",
  "hostId": 5,
  "hostName": "MCU_0_R5_2"
};
const modDef = createHostModule(hostInfo);
exports = modDef;
