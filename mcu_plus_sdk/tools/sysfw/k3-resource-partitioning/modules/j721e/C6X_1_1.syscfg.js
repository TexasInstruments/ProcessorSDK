
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "C6x_1 context 1 on Main island",
  "Security": "Non Secure",
  "displayName": "C6x_1 NonSecure host",
  "hostId": 28,
  "hostName": "C6X_1_1",
  "privId": 221
};
const modDef = createHostModule(hostInfo);
exports = modDef;
