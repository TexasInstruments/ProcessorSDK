UTILS_DIR := ../utils

all: hostApp

hostApp: hostapp.c $(UTILS_DIR)/hostapp_utils.c
	gcc $^ -I$(UTILS_DIR) -o $@

.PHONY: clean
clean:
	rm -f *.o hostApp
