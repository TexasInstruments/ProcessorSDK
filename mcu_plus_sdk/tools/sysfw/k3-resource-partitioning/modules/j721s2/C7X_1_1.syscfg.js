
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "C7x_1 context 1 on Main island",
  "Security": "Non Secure",
  "displayName": "C7X core1 NonSecure host",
  "hostId": 23,
  "hostName": "C7X_1_1",
  "privId": 26
};
const modDef = createHostModule(hostInfo);
exports = modDef;
