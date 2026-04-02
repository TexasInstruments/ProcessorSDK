
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "Cortex R5_2 context 0 on Main island",
  "Security": "Non Secure",
  "displayName": "Main R5F2 core0 NonSecure host",
  "hostId": 45,
  "hostName": "MAIN_2_R5_0"
};
const modDef = createHostModule(hostInfo);
exports = modDef;
