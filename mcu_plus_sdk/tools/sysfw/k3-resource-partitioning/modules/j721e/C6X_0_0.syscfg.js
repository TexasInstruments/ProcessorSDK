
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "C6x_0 Context 0 on Main island",
  "Security": "Secure",
  "displayName": "C6x_0 Secure host",
  "hostId": 25,
  "hostName": "C6X_0_0",
  "privId": 220
};
const modDef = createHostModule(hostInfo);
exports = modDef;
