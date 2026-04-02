
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "Cortex R5 context 1 on MCU island(Boot)",
  "Security": "Secure",
  "displayName": "MCU R5 core0 Secure host",
  "hostId": 4,
  "hostName": "MCU_0_R5_1",
  "privId": 96
};
const modDef = createHostModule(hostInfo);
exports = modDef;
