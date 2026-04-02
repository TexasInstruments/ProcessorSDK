
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "Cortex R5_0 context 3 on MAIN domain",
  "Security": "Non Secure",
  "displayName": "MAIN R5F0_1 NonSecure host",
  "hostId": 43,
  "hostName": "MAIN_0_R5_3",
  "privId": 214
};
const modDef = createHostModule(hostInfo);
exports = modDef;
