
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "GPU context 1 on MAIN domain",
  "Security": "Non Secure",
  "displayName": "GPU context 1 NonSecure host",
  "hostId": 32,
  "hostName": "GPU_1",
  "privId": 187
};
const modDef = createHostModule(hostInfo);
exports = modDef;
