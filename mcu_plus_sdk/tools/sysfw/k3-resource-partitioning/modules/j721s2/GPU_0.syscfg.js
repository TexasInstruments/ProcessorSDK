
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "BXS context 0 on Main island",
  "Security": "Non Secure",
  "displayName": "GPU Host",
  "hostId": 30,
  "hostName": "GPU_0",
  "privId": 187
};
const modDef = createHostModule(hostInfo);
exports = modDef;
