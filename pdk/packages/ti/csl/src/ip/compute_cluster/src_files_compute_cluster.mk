
ifeq ($(SOC),$(filter $(SOC), j721e))
PACKAGE_SRCS_COMMON += cslr_compute_cluster.h src/ip/compute_cluster/V0 src/ip/compute_cluster/src_files_compute_cluster.mk
INCDIR += src/ip/compute_cluster/V0
endif
