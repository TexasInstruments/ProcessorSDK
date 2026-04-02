
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "C7x_1 Context 0 on Main island",
  "Security": "Secure",
  "displayName": "C7X core1 Secure host",
  "hostId": 22,
  "hostName": "C7X_1_0",
  "privId": 26
};
const modDef = createHostModule(hostInfo);
exports = modDef;
