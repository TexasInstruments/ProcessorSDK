UNIFLASH_HOST_BINDIR = bin
UNIFLASH_HOST_SRCDIR = source
CPPFLAG = -static

ifeq ($(OS),Windows_NT)
LOCAL_FLAG = -DWINDOWS
endif

all:  $(UNIFLASH_HOST_BINDIR)/ProcessorSDKSerialFlash

$(UNIFLASH_HOST_BINDIR)/.created:
	@mkdir -p $(UNIFLASH_HOST_BINDIR)
	@touch $(UNIFLASH_HOST_BINDIR)/.created

$(UNIFLASH_HOST_BINDIR)/ProcessorSDKSerialFlash.o: $(UNIFLASH_HOST_SRCDIR)/ProcessorSDKSerialFlash.cpp
	g++ $(LOCAL_FLAG) -o $(UNIFLASH_HOST_BINDIR)/ProcessorSDKSerialFlash.o -c $(UNIFLASH_HOST_SRCDIR)/ProcessorSDKSerialFlash.cpp

$(UNIFLASH_HOST_BINDIR)/main.o : $(UNIFLASH_HOST_SRCDIR)/main.cpp
	g++ $(LOCAL_FLAG) -o $(UNIFLASH_HOST_BINDIR)/main.o -c $(UNIFLASH_HOST_SRCDIR)/main.cpp

$(UNIFLASH_HOST_BINDIR)/ProcessorSDKSerialFlash: $(UNIFLASH_HOST_BINDIR)/.created $(UNIFLASH_HOST_BINDIR)/ProcessorSDKSerialFlash.o $(UNIFLASH_HOST_BINDIR)/main.o
	g++ $(CPPFLAG) -o $(UNIFLASH_HOST_BINDIR)/ProcessorSDKSerialFlash $(UNIFLASH_HOST_BINDIR)/ProcessorSDKSerialFlash.o $(UNIFLASH_HOST_BINDIR)/main.o

clean:
	rm -f $(UNIFLASH_HOST_BINDIR)/ProcessorSDKSerialFlash.o $(UNIFLASH_HOST_BINDIR)/main.o $(UNIFLASH_HOST_BINDIR)/ProcessorSDKSerialFlash


