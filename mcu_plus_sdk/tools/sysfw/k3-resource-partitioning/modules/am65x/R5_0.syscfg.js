
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "Cortex R5 Context 0 on MCU island",
  "Security": "Non Secure",
  "displayName": "R5 core0 NonSecure host",
  "hostId": 3,
  "hostName": "R5_0",
  "privId": 96
};
const modDef = createHostModule(hostInfo);
exports = modDef;
