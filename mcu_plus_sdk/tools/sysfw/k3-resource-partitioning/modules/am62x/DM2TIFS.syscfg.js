
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "DM to TIFS communication",
  "Security": "Secure",
  "displayName": "DM2TIFS",
  "hostId": 250,
  "hostName": "DM2TIFS"
};
const modDef = createHostModule(hostInfo);
exports = modDef;
