
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "ICSS Context 2 on Main island",
  "Security": "Non Secure",
  "displayName": "ICSSG2 host",
  "hostId": 52,
  "hostName": "ICSSG_2",
  "privId": 136
};
const modDef = createHostModule(hostInfo);
exports = modDef;
