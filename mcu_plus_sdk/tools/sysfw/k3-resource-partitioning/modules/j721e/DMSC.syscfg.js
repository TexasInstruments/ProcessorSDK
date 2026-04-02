
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "Security Controller",
  "Security": "Secure",
  "displayName": "Device Management Security Controller",
  "hostId": 0,
  "hostName": "DMSC"
};
const modDef = createHostModule(hostInfo);
exports = modDef;
