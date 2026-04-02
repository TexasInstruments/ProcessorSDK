
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "Cortex A72 context 6 on Main island",
  "Security": "Non Secure",
  "displayName": "A72 NonSecure host",
  "hostId": 16,
  "hostName": "A72_6"
};
const modDef = createHostModule(hostInfo);
exports = modDef;
