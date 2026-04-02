
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "Cortex A72 context 7 on Main island",
  "Security": "Non Secure",
  "displayName": "A72 NonSecure host",
  "hostId": 17,
  "hostName": "A72_7"
};
const modDef = createHostModule(hostInfo);
exports = modDef;
