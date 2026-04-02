
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "C7x_1 context 0 on MAIN domain",
  "Security": "Non Secure",
  "displayName": "C7X_1 NonSecure host",
  "hostId": 22,
  "hostName": "C7X_1_0",
  "privId": 33
};
const modDef = createHostModule(hostInfo);
exports = modDef;
