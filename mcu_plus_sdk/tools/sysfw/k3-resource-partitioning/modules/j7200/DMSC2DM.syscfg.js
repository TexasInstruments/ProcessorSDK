
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "DMSC to DM communication",
  "Security": "Non Secure",
  "displayName": "DMSC2DM",
  "hostId": 251,
  "hostName": "DMSC2DM"
};
const modDef = createHostModule(hostInfo);
exports = modDef;
