
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "Cortex R5_1 context 1 on MAIN domain",
  "Security": "Non Secure",
  "displayName": "MAIN R5F1_0 NonSecure host",
  "hostId": 46,
  "hostName": "MAIN_1_R5_1",
  "privId": 216
};
const modDef = createHostModule(hostInfo);
exports = modDef;
