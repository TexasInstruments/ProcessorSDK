
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "Security Controller",
  "Security": "Secure",
  "displayName": "Security Controller",
  "hostId": 0,
  "hostName": "TIFS"
};
const modDef = createHostModule(hostInfo);
exports = modDef;
