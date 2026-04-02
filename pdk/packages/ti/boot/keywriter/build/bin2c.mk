.PHONY : $(BIN2C_EXE)

$(KEYWRITER_APP_DIR)/main.c: soc/$(SOC)/tifs_keywriter.h

soc/$(SOC)/tifs_keywriter.h: tifs_bin/$(SOC)/ti-fs-keywriter.bin $(BIN2C_EXE)
	$(BIN2C_EXE) $(SIGNED_SYSFW_BIN) $(KEYWRITER_APP_DIR)/soc/$(SOC)/tifs_keywriter.h TIFS_KEYWRITER_BIN

$(BIN2C_EXE):
# Ensure the file is not in use before removing it
	if [ -f $(BIN2C_EXE) ]; then \
		while lsof $(BIN2C_EXE) >/dev/null 2>&1; do \
			echo "Waiting for $(BIN2C_EXE) to be released..."; \
			sleep 1; \
		done; \
	fi
	chmod +x $(BIN2C_EXE)
	gcc $(SCI_CLIENT_TOOLS_BIN2C_DIR)/bin2c.c -o $(BIN2C_EXE)
