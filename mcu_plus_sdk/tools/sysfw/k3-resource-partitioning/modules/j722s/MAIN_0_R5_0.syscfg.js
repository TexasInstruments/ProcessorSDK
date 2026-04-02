
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "Cortex R5_0 context 0 on MAIN domain",
  "Security": "Secure",
  "displayName": "MAIN R5F0 Secure host",
  "hostId": 37,
  "hostName": "MAIN_0_R5_0",
  "privId": 214
};
const modDef = createHostModule(hostInfo);
exports = modDef;
