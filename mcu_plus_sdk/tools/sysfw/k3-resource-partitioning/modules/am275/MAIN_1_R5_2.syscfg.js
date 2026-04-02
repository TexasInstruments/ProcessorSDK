
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "Cortex R5_1 context 2 on MAIN domain",
  "Security": "Secure",
  "displayName": "MAIN R5F1_1 Secure host",
  "hostId": 47,
  "hostName": "MAIN_1_R5_2",
  "privId": 216
};
const modDef = createHostModule(hostInfo);
exports = modDef;
