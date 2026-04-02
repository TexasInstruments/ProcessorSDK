
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "HSM Controller",
  "Security": "Secure",
  "displayName": "HSM Controller",
  "hostId": 253,
  "hostName": "HSM",
  "privId": 203
};
const modDef = createHostModule(hostInfo);
exports = modDef;
