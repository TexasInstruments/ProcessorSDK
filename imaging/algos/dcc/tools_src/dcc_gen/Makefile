# Name of the output executable
OUTPUT = dcc_gen_linux
# Adding include directory inc/
CFLAGS += -I"inc"

# Compiler?
CC = gcc
# Debugging?
#CFLAGS += -g
# Building under linux?
CFLAGS += -m32 -D __LINUX__
CFLAGS += -O2 -Wformat=0

# The following library can also be replaced by an already installed one.
# To install a supported library from the ubuntu repository use:

#sudo apt-get install libexpat1-dev:i386

# Choose whether to use the installed library or the one 
# included in the project:

LDFLAGS += -lexpat


CFILES  =   src/dcc.c		    \
	    src/tools.c             \
            src/dcc_header.c        \
	    src/typedef.c	    \
            src/general_data.c      \
            src/usecase.c           \
            src/write_header.c      \
	    src/write_dtp.c	    \
            src/write_parser.c

OBJECTS := $(CFILES:.c=.o)

all: $(OBJECTS)
	@echo Linking console executable: $(OUTPUT)
	@$(CC) $(CFLAGS) $(OBJECTS) -o $(OUTPUT) $(LDFLAGS)
	@rm -Rf src/*.o
.c.o:
	@echo Compiling: $<
	@$(CC) $(CFLAGS) -c $< -o $*.o
clean:
	@rm -Rfv $(OUTPUT) 2>/dev/null
	@rm -Rf src/*.o

