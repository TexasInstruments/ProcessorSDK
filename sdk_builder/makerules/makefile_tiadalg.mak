#
# Utility makefile to build tiadalg libraries
#
# Edit this file to suit your specific build needs
#

tiadalg:
ifeq ($(SOC),j721e)
	$(MAKE) -C $(TIADALG_PATH) all -s
	$(MAKE) -C $(TIADALG_PATH) tiadalg_structure_from_motion_lib TARGET_CPU=$(C7X_TARGET) -s
endif
ifeq ($(SOC), $(filter $(SOC),j721s2 j784s4 j742s2))
	$(MAKE) -C $(TIADALG_PATH) TARGET_CPU=$(C7X_TARGET) SOC=$(SOC) all -s
endif

tiadalg_docs:
	$(MAKE) -C $(TIADALG_PATH) doxy_docs -s

tiadalg_clean:
	$(MAKE) -C $(TIADALG_PATH) clean -s

tiadalg_scrub:
	$(MAKE) -C $(TIADALG_PATH) scrub -s

.PHONY: tiadalg tiadalg_docs tiadalg_clean tiadalg_scrub
