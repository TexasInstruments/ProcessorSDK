
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "Cortex R5_2 context 3 on MCU island",
  "Security": "Secure",
  "displayName": "Main R5F2 core1 Secure host",
  "hostId": 48,
  "hostName": "MAIN_2_R5_3"
};
const modDef = createHostModule(hostInfo);
exports = modDef;
