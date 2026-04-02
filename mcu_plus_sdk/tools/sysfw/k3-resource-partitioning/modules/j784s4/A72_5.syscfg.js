
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "Cortex A72 context 5 on Main island",
  "Security": "Non Secure",
  "displayName": "A72 NonSecure host",
  "hostId": 15,
  "hostName": "A72_5"
};
const modDef = createHostModule(hostInfo);
exports = modDef;
