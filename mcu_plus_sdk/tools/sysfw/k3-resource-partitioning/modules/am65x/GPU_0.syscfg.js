
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "SGX544 Context 0 on Main island",
  "Security": "Non Secure",
  "displayName": "GPU host",
  "hostId": 30,
  "hostName": "GPU_0",
  "privId": 184
};
const modDef = createHostModule(hostInfo);
exports = modDef;
