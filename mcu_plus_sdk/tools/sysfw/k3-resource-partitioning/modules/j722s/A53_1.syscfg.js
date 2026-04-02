
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "Cortex A53 context 1 on MAIN domain",
  "Security": "Secure",
  "displayName": "A53_1 Secure host",
  "hostId": 11,
  "hostName": "A53_1",
  "privId": 4
};
const modDef = createHostModule(hostInfo);
exports = modDef;
