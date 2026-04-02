
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "Cortex R5 context 0 on MCU island",
  "Security": "Non Secure",
  "displayName": "MCU R5 core0 NonSecure host",
  "hostId": 3,
  "hostName": "MCU_0_R5_0",
  "privId": 96
};
const modDef = createHostModule(hostInfo);
exports = modDef;
