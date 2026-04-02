
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "C7x_2 context 1 on Main island",
  "Security": "Non Secure",
  "displayName": "C7X core2 NonSecure host",
  "hostId": 25,
  "hostName": "C7X_2_1"
};
const modDef = createHostModule(hostInfo);
exports = modDef;
