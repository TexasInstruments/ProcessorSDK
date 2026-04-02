# Common source files across all platforms and cores

SRCDIR = . src src/v0
INCDIR = . src src/v0
SRCS_COMMON += GPIO_drv.c GPIO_v0.c


PACKAGE_SRCS_COMMON = makefile GPIO.h gpio_component.mk src/GPIO_osal.h \
              docs/ReleaseNotes_GPIO_LLD.pdf \
              src/GPIO_drv.c src/GPIO_drv_log.h \
              build src/src_files_common.mk \
              src/v0 src/v0/GPIO_v0.h src/v0/GPIO_v0.c test


