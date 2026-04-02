
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "C7x_0 context 1 on Main island",
  "Security": "Non Secure",
  "displayName": "C7X core0 NonSecure host",
  "hostId": 21,
  "hostName": "C7X_0_1",
  "privId": 21
};
const modDef = createHostModule(hostInfo);
exports = modDef;
