
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "C6x_0 context 1 on Main island",
  "Security": "Non Secure",
  "displayName": "C6x_0 NonSecure host",
  "hostId": 26,
  "hostName": "C6X_0_1",
  "privId": 220
};
const modDef = createHostModule(hostInfo);
exports = modDef;
