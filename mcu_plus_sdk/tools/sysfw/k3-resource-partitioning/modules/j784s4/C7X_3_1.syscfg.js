
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "C7x_3 context 1 on Main island",
  "Security": "Non Secure",
  "displayName": "C7X core3 NonSecure host",
  "hostId": 27,
  "hostName": "C7X_3_1"
};
const modDef = createHostModule(hostInfo);
exports = modDef;
