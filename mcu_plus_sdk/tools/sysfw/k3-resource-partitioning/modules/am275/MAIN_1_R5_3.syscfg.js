
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "Cortex R5_1 context 3 on MAIN domain",
  "Security": "Non Secure",
  "displayName": "MAIN R5F1_1 NonSecure host",
  "hostId": 48,
  "hostName": "MAIN_1_R5_3",
  "privId": 216
};
const modDef = createHostModule(hostInfo);
exports = modDef;
