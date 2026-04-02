
SRCDIR = . src
INCDIR = . src
# Common source files across all platforms and cores
SRCS_COMMON += diskio.c ff.c FATFS_drv.c

PACKAGE_SRCS_COMMON = makefile FATFS.h fatfs_component.mk src/FATFS_osal.h \
                      docs/ReleaseNotes_FATFS_LLD.pdf \
                      ff.h ffconf.h src/FATFS_drv_log.h \
                      diskio.h integer.h \
                      build src/src_files_common.mk src/Module.xs \
                      src/diskio.c src/ff.c src/FATFS_drv.c .gitignore \
                      config_mk.bld FATFSver.h FATFSver.h.xdt package.bld package.xdc package.xs Settings.xdc.xdt
