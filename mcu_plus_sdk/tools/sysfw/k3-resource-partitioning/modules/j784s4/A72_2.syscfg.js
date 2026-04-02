
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "Cortex A72 context 2 on Main island",
  "Security": "Non Secure",
  "displayName": "A72 NonSecure host for Linux / VM1 / RTOS",
  "hostId": 12,
  "hostName": "A72_2"
};
const modDef = createHostModule(hostInfo);
exports = modDef;
