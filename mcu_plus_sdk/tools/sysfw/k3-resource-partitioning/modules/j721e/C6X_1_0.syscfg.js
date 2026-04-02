
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "C6x_1 Context 0 on Main island",
  "Security": "Secure",
  "displayName": "C6x_1 Secure host",
  "hostId": 27,
  "hostName": "C6X_1_0",
  "privId": 221
};
const modDef = createHostModule(hostInfo);
exports = modDef;
