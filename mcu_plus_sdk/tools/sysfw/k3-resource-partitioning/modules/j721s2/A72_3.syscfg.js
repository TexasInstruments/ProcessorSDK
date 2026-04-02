
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "Cortex A72 context 3 on Main island",
  "Security": "Non Secure",
  "displayName": "A72 NonSecure host for VM2",
  "hostId": 13,
  "hostName": "A72_3",
  "privId": 1
};
const modDef = createHostModule(hostInfo);
exports = modDef;
