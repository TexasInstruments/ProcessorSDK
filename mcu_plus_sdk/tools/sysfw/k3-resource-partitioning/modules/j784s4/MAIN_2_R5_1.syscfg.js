
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "Cortex R5_2 context 1 on Main island",
  "Security": "Secure",
  "displayName": "Main R5F2 core0 Secure host",
  "hostId": 46,
  "hostName": "MAIN_2_R5_1"
};
const modDef = createHostModule(hostInfo);
exports = modDef;
