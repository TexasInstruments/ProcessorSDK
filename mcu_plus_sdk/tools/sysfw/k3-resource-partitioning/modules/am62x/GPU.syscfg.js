
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "GPU context 0 on Main island",
  "Security": "Non Secure",
  "displayName": "GPU host",
  "hostId": 31,
  "hostName": "GPU",
  "privId": 187
};
const modDef = createHostModule(hostInfo);
exports = modDef;
