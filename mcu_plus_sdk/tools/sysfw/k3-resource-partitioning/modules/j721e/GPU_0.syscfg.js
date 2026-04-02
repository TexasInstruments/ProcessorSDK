
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "RGX context 0 on Main island",
  "Security": "Non Secure",
  "displayName": "GPU host",
  "hostId": 30,
  "hostName": "GPU_0",
  "privId": 187
};
const modDef = createHostModule(hostInfo);
exports = modDef;
