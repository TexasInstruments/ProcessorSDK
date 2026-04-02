
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "GPU context 0 on MAIN domain",
  "Security": "Non Secure",
  "displayName": "GPU context 0 NonSecure host",
  "hostId": 31,
  "hostName": "GPU_0",
  "privId": 187
};
const modDef = createHostModule(hostInfo);
exports = modDef;
