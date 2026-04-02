
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "Cortex A53 context 5 on Main island",
  "Security": "Non Secure",
  "displayName": "A53 NonSecure host",
  "hostId": 15,
  "hostName": "A53_5",
  "privId": 1
};
const modDef = createHostModule(hostInfo);
exports = modDef;
