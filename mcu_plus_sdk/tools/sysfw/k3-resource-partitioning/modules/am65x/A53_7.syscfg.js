
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "Cortex A53 context 7 on Main island",
  "Security": "Non Secure",
  "displayName": "A53 NonSecure host",
  "hostId": 17,
  "hostName": "A53_7",
  "privId": 1
};
const modDef = createHostModule(hostInfo);
exports = modDef;
