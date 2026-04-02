
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "Cortex A72 context 0 on Main island",
  "Security": "Secure",
  "displayName": "A72 Secure host for ATF",
  "hostId": 10,
  "hostName": "A72_0",
  "privId": 1
};
const modDef = createHostModule(hostInfo);
exports = modDef;
