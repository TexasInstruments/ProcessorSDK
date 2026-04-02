
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "Cortex R5_0 context 1 on WKUP domain",
  "Security": "Non Secure",
  "displayName": "WKUP R5F0 NonSecure host",
  "hostId": 36,
  "hostName": "WKUP_0_R5_1",
  "privId": 212
};
const modDef = createHostModule(hostInfo);
exports = modDef;
