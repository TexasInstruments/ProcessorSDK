
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "Cortex R5_0 context 2 on MAIN domain",
  "Security": "Secure",
  "displayName": "MAIN R5F0_1 Secure host",
  "hostId": 42,
  "hostName": "MAIN_0_R5_2",
  "privId": 214
};
const modDef = createHostModule(hostInfo);
exports = modDef;
