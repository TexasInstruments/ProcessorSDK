
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "C7x context 1 on Main island",
  "Security": "Non Secure",
  "displayName": "C7X NonSecure host",
  "hostId": 21,
  "hostName": "C7X_1",
  "privId": 21
};
const modDef = createHostModule(hostInfo);
exports = modDef;
