
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "Cortex A72 context 4 on Main island",
  "Security": "Non Secure",
  "displayName": "A72 NonSecure host",
  "hostId": 14,
  "hostName": "A72_4",
  "privId": 1
};
const modDef = createHostModule(hostInfo);
exports = modDef;
