
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "Device Management and Security Control",
  "Security": "Secure",
  "displayName": "TIFS",
  "hostId": 0,
  "hostName": "TIFS"
};
const modDef = createHostModule(hostInfo);
exports = modDef;
