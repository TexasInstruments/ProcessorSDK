
SRCDIR = . src
INCDIR = . src

# Common source files across all platforms and cores
SRCS_COMMON += pcie.c pcieinit.c

ifeq ($(SOC),$(filter $(SOC), am65xx))
    SRCDIR += src/v2
    INCDIR += src/v2
    SRCS_COMMON += pciev2.c pciev2_app.c pciev2_plconf.c pciev2_ep.c pciev2_rc.c pciev2_cfg.c
    PACKAGE_SRCS_COMMON += src/v2
else
    SRCDIR+= src/v1 src/v0
    INCDIR+= src/v1 src/v0
    SRCS_COMMON += pciev0.c pciev0_app.c pciev0_cfg.c pciev1.c pciev1_ticonf.c pciev1_plconf.c pciev1_ep.c pciev1_rc.c pciev1_cfg.c
    PACKAGE_SRCS_COMMON += src/v0 src/v1
endif

PACKAGE_SRCS_COMMON += makefile pcie.h pciever.h pcie_component.mk \
                      docs/ReleaseNotes_PCIE_LLD.pdf \
                      src/pcieloc.h src/pcie.c src/pcieinit.c \
                      build src/src_files_common.mk src/Module.xs \
                      config_mk.bld package.bld package.xdc package.xs pciever.h.xdt Settings.xdc.xdt
