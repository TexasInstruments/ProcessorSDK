
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "C7x_0 context 0 on MAIN domain",
  "Security": "Non Secure",
  "displayName": "C7X_0 NonSecure host",
  "hostId": 20,
  "hostName": "C7X_0_0",
  "privId": 32
};
const modDef = createHostModule(hostInfo);
exports = modDef;
