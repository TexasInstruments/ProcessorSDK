
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "Cortex R5 Context 1 on MCU island(Boot)",
  "Security": "Secure",
  "displayName": "R5 core0 Secure host",
  "hostId": 4,
  "hostName": "R5_1",
  "privId": 96
};
const modDef = createHostModule(hostInfo);
exports = modDef;
