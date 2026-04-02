
const {createHostModule} = system.getScript("/modules/sysfwResPart.js");
const hostInfo = {
  "Description": "HSM Controller",
  "Security": "Secure",
  "displayName": "HSM",
  "hostId": 253,
  "hostName": "HSM",
  "privId": 204
};
const modDef = createHostModule(hostInfo);
exports = modDef;
