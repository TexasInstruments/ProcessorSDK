
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "TI Foundational Security",
  "Security": "Secure",
  "displayName": "TI Foundational Security",
  "hostId": 0,
  "hostName": "TIFS"
};
const modDef = createHostModule(hostInfo);
exports = modDef;
