
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "ICSS Context 1 on Main island",
  "Security": "Non Secure",
  "displayName": "ICSSG1 host",
  "hostId": 51,
  "hostName": "ICSSG_1",
  "privId": 136
};
const modDef = createHostModule(hostInfo);
exports = modDef;
