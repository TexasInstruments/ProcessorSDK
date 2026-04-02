
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "Cortex R5_0 context 1 on MAIN domain",
  "Security": "Non Secure",
  "displayName": "MAIN R5F0 NonSecure host",
  "hostId": 38,
  "hostName": "MAIN_0_R5_1",
  "privId": 214
};
const modDef = createHostModule(hostInfo);
exports = modDef;
