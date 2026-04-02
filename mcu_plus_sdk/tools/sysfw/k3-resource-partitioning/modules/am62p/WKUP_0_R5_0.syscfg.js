
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "Cortex R5_0 context 0 on WKUP domain (BOOT)",
  "Security": "Secure",
  "displayName": "WKUP R5F0 Secure host",
  "hostId": 35,
  "hostName": "WKUP_0_R5_0",
  "privId": 212
};
const modDef = createHostModule(hostInfo);
exports = modDef;
