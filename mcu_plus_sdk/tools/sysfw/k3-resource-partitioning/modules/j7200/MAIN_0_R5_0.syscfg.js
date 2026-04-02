
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "Cortex R5_0 context 0 on Main island",
  "Security": "Non Secure",
  "displayName": "Main R5F0 core0 NonSecure host",
  "hostId": 35,
  "hostName": "MAIN_0_R5_0",
  "privId": 212
};
const modDef = createHostModule(hostInfo);
exports = modDef;
