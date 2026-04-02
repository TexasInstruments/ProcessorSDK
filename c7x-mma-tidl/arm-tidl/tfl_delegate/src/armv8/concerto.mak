ifneq ($(TARGET_PLATFORM), PC)
include $(PRELUDE)

CSOURCES    := 
CPPSOURCES  :=

include $($(_MODULE)_SDIR)/../concerto_common.mak

include $(FINALE)
endif
