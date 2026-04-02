
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "Cortex R5_2 context 2 on Main island",
  "Security": "Non Secure",
  "displayName": "Main R5F2 core1 NonSecure host",
  "hostId": 47,
  "hostName": "MAIN_2_R5_2"
};
const modDef = createHostModule(hostInfo);
exports = modDef;
