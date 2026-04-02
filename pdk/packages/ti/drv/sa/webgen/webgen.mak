#
#  ======== Makefile ========
#  Makefile for SALLD web page 
#
#  Notes:
#
#  this will download SALLD package up in FDS format
#
#! Revisions History
#! =================
#! 04-Mar-2009  ada     Initial
#! 22-June-2011 ereui   Release 1.0.4.1
#! 14-Sept-2012 ereui   Release 1.0.5.2
#! 31-Oct-2012  eruei   Release 1.0.5.3
#! 27-Dec-2012  eruei   Release 1.0.5.4
#! 15-Feb-2013  abatni  Release 2.0.0.3
#! 27-Apr-2013  abatni  Release 2.0.0.5
#! 15-May-2013  abatni  Release 2.0.0.6
#! 22-July-2013 abatni  Release 2.0.1.2
#! 26-July-2013 abatni  Release 2.0.1.3
#! 29-July-2013 abatni  Release 2.0.1.4
#! 11-Sept-2013 abatni  Release 2.0.2.0

MK_NOISY=1
TllYPE=ENG
PRODUCT="SALLD"
VERSION=02_00_02_00
DVERSION="02.00.00"
RELEASE="02_00_00"
TITLE="$(PRODUCT) $(RELEASE)"
PRODUCT_NAME="$(PRODUCT)"
EMAIL=" "
PRODUCT_OR_COMPONENT=COMPONENT
DOWNLOAD_TEXT=""
URL=http://www.ti.com/tool/bioslinuxmcsdk
CCS_URL ='http://processors.wiki.ti.com/index.php/Download_CCS'
CCS_COMMENT='This is the TI Code Composer Studio IDE. It contains the IDE, CodeGen Tools, debugger and base components such as BIOS, IPC and so forth.'
BIOS_MCSDK_URL ='http://software-dl.ti.com/sdoemb/sdoemb_public_sw/bios_mcsdk/latest/index_FDS.html'
MCSDK_3_X_URL ='http://software-dl.ti.com/sdoemb/sdoemb_public_sw/mcsdk/latest/index_FDS.html'
WIN_CODEGEN_URL='http://software-dl-1.ti.com/dsps/forms/self_cert_export.html?prod_no=ti_cgt_c6000_7.2.6_setup_win32.exe&ref_url=http://software-dl.ti.com/dsps/dsps_registered_sw/sdo_ccstudio/codegen/C6000/7.2.4'
LINUX_CODEGEN_URL='http://software-dl-1.ti.com/dsps/forms/self_cert_export.html?prod_no=ti_cgt_c6000_7.2.6_setup_linux_x86.bin&ref_url=http://software-dl.ti.com/dsps/dsps_registered_sw/sdo_ccstudio/codegen/C6000/7.2.4'
FOLDER="http://www.ti.com/tool/bioslinuxmcsdk"
TI_CGT_URL="https://www-a.ti.com/downloads/sds_support/TICodegenerationTools/download.htm"


.all-files: .exports .wget .md5


VERSTRUNDER=$(shell echo $(VERSION) | sed -e 's/\./_/g' )

.wget:
	# cd exports; \
    # Note: place holder for future extension 

.exports:
	# rm -rf exports
	# mkdir exports
	rm -rf imports
	mkdir imports

.prod_links:

	$(call add-link,"setupwin32_salld_keystone2_$(VERSION).exe","setupwin32_salld_keystone2_$(VERSION).exe","SA LLD Package for Windows")
	$(call add-link,"setupLinux_salld_keystone2_$(VERSION).bin","setupLinux_salld_keystone2_$(VERSION).bin","SA LLD Package for Linux")
	$(call add-selfcert-link,"setupwin32_sa3gpp_keystone2_$(VERSION).exe","setupwin32_sa3gpp_keystone2_$(VERSION).exe","SA 3GPP Enabler Package for Windows")
	$(call add-selfcert-link,"setupLinux_sa3gpp_keystone2_$(VERSION).bin","setupLinux_sa3gpp_keystone2_$(VERSION).bin","SA 3GPP Enabler Package for Linux")
	$(call add-divider,"Dependencies")	
	$(call add-http-link,"${CCS_URL}","CCS v5.4.0.00091","${CCS_COMMENT}")
	$(call add-http-link,"${MCSDK_3_X_URL}","MCSDK 3.x Latest","MCSDK 3.x package")	
#	$(call add-http-link,"${WIN_CODEGEN_URL}","Code Gen 7.2.6 Windows","Codegen Tools for Windows")
#	$(call add-http-link,"${LINUX_CODEGEN_URL}","Code Gen 7.2.6 Linux","Codegen Tools for Linux")
	$(call add-http-link,"${TI_CGT_URL}","Code Gen 7.4.2","Codegen Tools")
	$(call add-divider,"Documentation")
	$(call add-link,"ReleaseNotes_SA_LLD.pdf","ReleaseNotes_SA_LLD.pdf","SA LLD Release Notes")
	$(call add-link,"SA_LLD_2_0_SoftwareManifest.pdf","SA_LLD_2_0_SoftwareManifest.pdf","SA LLD Software Manifest document")
	$(call add-link,"UserGuide_SA_LLD.pdf","UserGuide_SA_LLD.pdf","SA LLD User Guides")
	$(call add-link,"ReleaseNotes_SA_3GPP.pdf","ReleaseNotes_SA_3GPP.pdf","SA 3GPP Enabler Release Notes")
	$(call add-link,"SA_LLD_3GPP_2_0_SoftwareManifest.pdf","SA_LLD_3GPP_2_0_SoftwareManifest.pdf","SA LLD 3GPP Enabler Software Manifest document")
	$(call add-divider,"Checksums")
	$(call add-link,"md5sum.txt","MD5 Checksums")

.md5:
	@echo "Generating md5sum.txt"
	rm -f exports/md5sum.*
	@cd exports; ../genmd5 ../imports/md5sum.txt; mv ../imports/md5sum.txt .

.clean: my-clean

my-clean:
	rm -rf imports
	# rm -rf exports
	rm -f .wget
