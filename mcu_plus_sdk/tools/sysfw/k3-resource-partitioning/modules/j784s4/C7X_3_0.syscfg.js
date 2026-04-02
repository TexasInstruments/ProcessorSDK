
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "C7x_2 Context 0 on Main island",
  "Security": "Secure",
  "displayName": "C7X core3 Secure host",
  "hostId": 26,
  "hostName": "C7X_3_0"
};
const modDef = createHostModule(hostInfo);
exports = modDef;
