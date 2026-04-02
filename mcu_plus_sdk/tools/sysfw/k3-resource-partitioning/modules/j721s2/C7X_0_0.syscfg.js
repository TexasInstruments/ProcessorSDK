
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "C7x_0 Context 0 on Main island",
  "Security": "Secure",
  "displayName": "C7X core0 Secure host",
  "hostId": 20,
  "hostName": "C7X_0_0",
  "privId": 21
};
const modDef = createHostModule(hostInfo);
exports = modDef;
