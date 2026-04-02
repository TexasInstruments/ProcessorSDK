
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "Cortex A53 context 6 on Main island",
  "Security": "Non Secure",
  "displayName": "A53 NonSecure host",
  "hostId": 16,
  "hostName": "A53_6",
  "privId": 1
};
const modDef = createHostModule(hostInfo);
exports = modDef;
