
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "Cortex R5_1 context 0 on MAIN domain",
  "Security": "Secure",
  "displayName": "MAIN R5F1_0 Secure host",
  "hostId": 45,
  "hostName": "MAIN_1_R5_0",
  "privId": 216
};
const modDef = createHostModule(hostInfo);
exports = modDef;
