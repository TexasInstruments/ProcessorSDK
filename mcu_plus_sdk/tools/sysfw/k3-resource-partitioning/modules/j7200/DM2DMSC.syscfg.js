
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "DM to DMSC communication",
  "Security": "Secure",
  "displayName": "DM2DMSC",
  "hostId": 250,
  "hostName": "DM2DMSC"
};
const modDef = createHostModule(hostInfo);
exports = modDef;
