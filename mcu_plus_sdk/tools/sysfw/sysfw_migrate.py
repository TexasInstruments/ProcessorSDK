import sys
import os
from shutil import copyfile
import glob

SOC                  = "j722s"
SOC_FOLDER           = "j722s"
ARCHIVE_FMT          = "tar"
SYSFW_REL_GIT_URL    = "ssh://bitbucket.itg.ti.com/sysfw/system-firmware-releases.git"
TIFS_GP_BIN_NAME     = f"ti-fs-firmware-{SOC}-gp.bin"
TIFS_HS_FS_BIN_NAME  = f"ti-fs-firmware-{SOC}-hs-fs-enc.bin"
TIFS_HS_FS_INNER_CERT_NAME = f"ti-fs-firmware-{SOC}-hs-fs-cert.bin"
TIFS_HS_BIN_NAME     = f"ti-fs-firmware-{SOC}-hs-enc.bin"
TIFS_HS_INNER_CERT_NAME = f"ti-fs-firmware-{SOC}-hs-cert.bin"

sciclient_path = os.path.realpath("../../source/drivers/sciclient/")

if(os.name=="nt"):
	print("[ERROR] Script currently not supported in windows")
	exit(1)

if(not os.path.isdir(sciclient_path)):
	print("[ERROR] This script is run from the wrong path !!!")
	exit(1)

if(len(sys.argv) < 2):
	print("[ERROR] Please provide the release tag")
	print("Usage: python3 sysfw_migrate.py <release-tag>")
	exit(1)

release_tag = sys.argv[1]

def get_git_file(remote_url, branch, filepath, strip_level):
	filebasename = os.path.basename(filepath)
	os.system(f"git archive --format={ARCHIVE_FMT} --remote={remote_url} {branch} {filepath} > {filebasename}.{ARCHIVE_FMT}")
	if(os.name=="posix"):
		os.system(f"tar -xf {filebasename}.{ARCHIVE_FMT} --strip-components={strip_level}")
	os.remove(f"{filebasename}.{ARCHIVE_FMT}")

def file_replace(filename, current_pattern, changed_pattern, lineNum=None):
	lines = None
	lineCount = 0
	with open(filename, "r") as f:
		lines = f.readlines()
		lineCount = len(lines)
	if(lineNum == None):
		for i in range(0, lineCount):
			lines[i] = lines[i].replace(current_pattern, changed_pattern)
	else:
		lines[lineNum] = lines[lineNum].replace(current_pattern, changed_pattern)
	with open(filename, "w") as f:
		f.writelines(lines)

# Clone SYSFW releases
os.mkdir("sysfw-rel")
os.chdir("sysfw-rel")

# Get sysfw bin and copy to sciclient directory
print(f"Getting the TIFS GP binary for {release_tag} ...")
get_git_file(SYSFW_REL_GIT_URL, release_tag, f"binaries/{TIFS_GP_BIN_NAME}", 1)
copyfile(TIFS_GP_BIN_NAME, sciclient_path+f"/soc/{SOC_FOLDER}/tifs.bin")
print("Done !!!\n")

print(f"Getting the TIFS HS-FS dummy-key (signed, encrypted) binary for {release_tag} ...")
get_git_file(SYSFW_REL_GIT_URL, release_tag, f"binaries/{TIFS_HS_FS_BIN_NAME}", 1)
copyfile(TIFS_HS_FS_BIN_NAME, sciclient_path+f"/soc/{SOC_FOLDER}/tifs-hs-fs-enc.bin")

print(f"Getting the inner certificate binary for TIFS HS-FS dummy-key binary for {release_tag} ...")
get_git_file(SYSFW_REL_GIT_URL, release_tag, f"binaries/{TIFS_HS_FS_INNER_CERT_NAME}", 1)
copyfile(TIFS_HS_FS_INNER_CERT_NAME, sciclient_path+f"/soc/{SOC_FOLDER}/tifs-hs-fs-enc-cert.bin")

#print(f"Getting the TIFS HS-SE production (signed, encrypted) binary for {release_tag} ...")
#get_git_file(SYSFW_REL_GIT_URL, release_tag, f"binaries/{TIFS_HS_BIN_NAME}", 1)
#copyfile(TIFS_HS_BIN_NAME, sciclient_path+f"/soc/{SOC_FOLDER}/tifs-hs-enc.bin")

#print(f"Getting the inner certificate binary for TIFS HS-SE production binary for {release_tag} ...")
#get_git_file(SYSFW_REL_GIT_URL, release_tag, f"binaries/{TIFS_HS_INNER_CERT_NAME}", 1)
#copyfile(TIFS_HS_INNER_CERT_NAME, sciclient_path+f"/soc/{SOC_FOLDER}/tifs-hs-enc-cert.bin")


print("Done !!!\n")

# In the case of HS, generate the encrypted signed HS binary and certificate. (ENC-KEY = tiMek.txt, SGN-KEY = tiMpk.pem)
# TODO

# Get the include files
print("Getting the include files ...")
get_git_file(SYSFW_REL_GIT_URL, release_tag, f"include/tisci", 1)
get_git_file(SYSFW_REL_GIT_URL, release_tag, f"include/{SOC}", 1)
print("Done !!!")

# Copy the tisci include files

print("Copying the include files to sciclient folder with necessary changes...")

tisci_gen_include_files = [ os.path.basename(i) for i in glob.glob("tisci/*.h") ]

for file in tisci_gen_include_files:
	copyfile("tisci/"+file, sciclient_path+"/include/tisci/"+file)
	file_replace(sciclient_path+"/include/tisci/"+file, "/* @} */", "/** @} */", -1)

tisci_pm_include_files = [ os.path.basename(i) for i in glob.glob("tisci/pm/*.h") ]

for file in tisci_pm_include_files:
	copyfile("tisci/pm/"+file, sciclient_path+"/include/tisci/pm/"+file)
	file_replace(sciclient_path+"/include/tisci/pm/"+file, "/* @} */", "/** @} */", -1)

tisci_rm_include_files = [ os.path.basename(i) for i in glob.glob("tisci/rm/*.h") ]

for file in tisci_rm_include_files:
	copyfile("tisci/rm/"+file, sciclient_path+"/include/tisci/rm/"+file)
	file_replace(sciclient_path+"/include/tisci/rm/"+file, "/* @} */", "/** @} */", -1)

tisci_security_include_files = [ os.path.basename(i) for i in glob.glob("tisci/security/*.h") ]

for file in tisci_security_include_files:
	copyfile("tisci/security/"+file, sciclient_path+"/include/tisci/security/"+file)
	file_replace(sciclient_path+"/include/tisci/security/"+file, "/* @} */", "/** @} */", -1)

tisci_soc_include_files = [ os.path.basename(i) for i in glob.glob(f"{SOC}/*.h") ]

for file in tisci_soc_include_files:
	copyfile(f"{SOC}/"+file, sciclient_path+f"/include/tisci/{SOC_FOLDER}/"+file)
	file_replace(sciclient_path+f"/include/tisci/{SOC_FOLDER}/"+file, "/* @} */", "/** @} */", -1)

print("Done !!!")

os.chdir("../")

if(os.name == "posix"):
	os.system("rm -rf sysfw-rel")

# To accommodate any changes in TISCI header files into the boardcfg, rebuild the boardcfg
if(os.name == "posix"):
	os.system("make -s -C boardcfg sciclient_boardcfg")
else:
	os.system("gmake -s -C boardcfg sciclient_boardcfg")














