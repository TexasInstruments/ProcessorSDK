#
# This file is the makefile for building freertos posix support and is included by freertos.mak file.
#

SRCDIR += ${FREERTOS_LABS_INSTALL_PATH}/FreeRTOS-POSIX/FreeRTOS-Plus-POSIX/source
	
INCDIR += ${FREERTOS_LABS_INSTALL_PATH}/FreeRTOS-POSIX/include \
          ${FREERTOS_LABS_INSTALL_PATH}/FreeRTOS-POSIX/include/private \
          ${FREERTOS_LABS_INSTALL_PATH}/FreeRTOS-POSIX/include/FreeRTOS_POSIX \
          ${FREERTOS_LABS_INSTALL_PATH}/FreeRTOS-POSIX/FreeRTOS-Plus-POSIX/include \
          ${FREERTOS_LABS_INSTALL_PATH}/FreeRTOS-POSIX/FreeRTOS-Plus-POSIX/include/portable



# Common source files and CFLAGS across all platforms and cores
SRCS_COMMON += \
    FreeRTOS_POSIX_clock.c \
    FreeRTOS_POSIX_pthread_barrier.c \
    FreeRTOS_POSIX_pthread_cond.c \
    FreeRTOS_POSIX_pthread_mutex.c \
    FreeRTOS_POSIX_pthread.c \
    FreeRTOS_POSIX_sched.c \
    FreeRTOS_POSIX_semaphore.c \
    FreeRTOS_POSIX_unistd.c \
    FreeRTOS_POSIX_utils.c

# FreeRTOS tasks does not compile in CPP build. Skip it for CPP build
ifneq ($(CPLUSPLUS_BUILD), yes)
SRCS_COMMON += \
    FreeRTOS_POSIX_timer.c \
    FreeRTOS_POSIX_mqueue.c
endif


# Nothing beyond this point
