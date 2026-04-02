
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "Cortex A72 context 1 on Main island",
  "Security": "Secure",
  "displayName": "A72 Secure host for OPTEE",
  "hostId": 11,
  "hostName": "A72_1",
  "privId": 1
};
const modDef = createHostModule(hostInfo);
exports = modDef;
