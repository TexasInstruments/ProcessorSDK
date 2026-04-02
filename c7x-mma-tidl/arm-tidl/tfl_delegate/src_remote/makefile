PLUGIN_CC ?= /home/subhajit/Works/IE-test/gcc-arm-9.2-2019.12-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-g++
#--sysroot /home/subhajit/Works/IE-test/rootfs
Q ?= @
CLIENT_CC ?= g++
BUILD ?= release

COMMON_SRCS := \
               tflite_delegate_messages.cpp
 
CLIENT_SRCS := \
             $(COMMON_SRCS) \
             tfl_delegate.cpp

PLUGIN_SRCS := \
             $(COMMON_SRCS) \
             tflite_delegate_registry.cpp

CFLAGS_CLIENT := -I. -I$(TIIE_DIR) -I$(TENSORFLOW_DIR) -fPIC
LDFLAGS_CLIENT := -lstdc++fs -L$(TIIE_DIR) -lti_inference_client

CFLAGS_PLUGIN := -I. -I$(TIIE_DIR) -I$(TENSORFLOW_DIR) -fPIC
LDFLAGS_PLUGIN :=

ifeq ($(BUILD),debug)
CFLAGS_CLIENT += -g
CFLAGS_PLUGIN += -g
LDFLAGS_CLIENT += -g
LDFLAGS_PLUGIN += -g
else
CFLAGS_CLIENT += -O2
CFLAGS_PLUGIN += -O2
endif


PLUGIN_OBJS := $(patsubst %.cpp, build/plugin/%.o, $(PLUGIN_SRCS))
CLIENT_OBJS := $(patsubst %.cpp, build/client/%.o, $(CLIENT_SRCS))

GEN_HEADERS := tflite_delegate_messages.h

build/plugin build/client: 
	$(Q)mkdir -p $@

%.h: %.yaml
	$(info [GEN] $@)
	$(Q)$(TIIE_DIR)/parse_messages.py $^ > $@

$(PLUGIN_OBJS) $(CLIENT_OBJS): $(GEN_HEADERS)

$(PLUGIN_OBJS): build/plugin

$(CLIENT_OBJS): build/client

build/client/%.o: %.cpp
	$(info [CLIENT CC] $@)
	$(Q)$(CLIENT_CC) $(CFLAGS_CLIENT) -c -o $@ $<

build/plugin/%.o: %.cpp
	$(info [PLUGIN CC] $@)
	$(Q)$(PLUGIN_CC) $(CFLAGS_PLUGIN) -c -o $@ $<

libtidl_tfl_delegate.so: $(CLIENT_OBJS)
	$(info [CLIENT LD] $@)
	$(Q)$(CLIENT_CC) -shared -rdynamic -o $@ $^ $(LDFLAGS_CLIENT)

tflite_delegate-ie-plugin.so: $(PLUGIN_OBJS)
	$(info [PLUGIN LD] $@)
	$(Q)$(PLUGIN_CC) -shared -rdynamic -o $@ $^ $(LDFLAGS_PLUGIN)

client: libtidl_tfl_delegate.so

plugin: tflite_delegate-ie-plugin.so

all: client plugin 

clean:
	$(Q)rm -Rf build *.so $(GEN_HEADERS)
