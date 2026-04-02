
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "Cortex R5 Context 3 on MCU island",
  "Security": "Secure",
  "displayName": "R5 core1 Secure host",
  "hostId": 6,
  "hostName": "R5_3",
  "privId": 97
};
const modDef = createHostModule(hostInfo);
exports = modDef;
