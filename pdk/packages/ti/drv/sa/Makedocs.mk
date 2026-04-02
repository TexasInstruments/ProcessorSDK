# ==============================================================================
# File            : Makedocs.mk
# Description     : 
#
#   GNU makefile to generate SA LLD documentation from sources using Doxygen
#
# ==============================================================================

# The target for generating documents using doxygen is gendocs

# RELEASE_DOC=./docs/doxygen/release.chm

# releasenotes : $(RELEASE_DOC)

#$(RELEASE_DOC): ./docs/doxygen/release.h
#	-@echo generating Release Notes ...
#	if test ! -d ./docs/doxygen/html; then mkdir ./docs/doxygen/html; fi
#	cp $(TI_DOXYGEN_TEMPLATES)/*.* ./docs/doxygen/html
#	doxygen ../swtools/docs/doxygen/releaseDoxyfile
#	@xs -f ../swtools/release.js

SAAPI_DOC=./docs/doxygen/sa_lld_docs.chm

#release : $(AESAPI_DOC) $(AES_DOC)
release : $(SAAPI_DOC)

#gendocs $(AES_DOC): salld.h ./docs/doxygen/doxygen.h
#	-@echo generating SA LLD documentation ...
#	if test ! -d ./docs/doxygen/html; then mkdir ./docs/doxygen/html; fi
#	cp $(TI_DOXYGEN_TEMPLATES)/*.* ./docs/doxygen/html
#	doxygen.exe ./docs/doxygen/Doxyfile

genapidocs $(SAAPI_DOC): salld.h 
	-@echo generating SA LLD API documentation ...
	if test ! -d ./docs/doxygen/html; then mkdir ./docs/doxygen/html; fi
	#cp $(TI_DOXYGEN_TEMPLATES)/*.* ./docs/doxygen/html
	doxygen.exe ./docs/doxygen/APIDoxyfile
	-@echo ----------------------------
	-@echo SA LLD Eclipse Plugin Generation
	 xs xdc.tools.eclipsePluginGen -o . -x ./eclipse/sample.xml -c ./eclipse/toc_cdoc_sample.xml
	-@echo SA LLD Eclipse Plugin Generated
	-@echo ----------------------------


# End of Makedocs.mk
