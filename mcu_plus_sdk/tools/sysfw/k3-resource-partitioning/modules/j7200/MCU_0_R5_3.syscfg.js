
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "Cortex R5 context 3 on MCU island",
  "Security": "Secure",
  "displayName": "MCU R5 core1 Secure host",
  "hostId": 6,
  "hostName": "MCU_0_R5_3",
  "privId": 97
};
const modDef = createHostModule(hostInfo);
exports = modDef;
