##########################################################################
#  There are two makefiles for midcutils: Makefile and libtool.mk
##########################################################################
#  
#  This one can compile each part of midcutils separately to .o files,
#  and run tests (in the test dir) linked to the .o files.
#  And it can make static library via the lib-static target (Unix only)
#  
##########################################################################
#  
#  The other one, libtool.mk, can create a shared library, run the
#  tests against the shared library, install the shared library and headers
#  and uninstall the shared library and headers.  It depends on libtool,
#  which you may need to install first if you don't already have it.
#  
##########################################################################
# 
#  === Usage of this Makefile ===
#  This makefile cannot install, only create .o and .a files and run
#  the unit tests (against the .o files).
#  
#  make all        - builds all the .o files
#  make test       - runs tests against the .o files
#                    You must have midcunit installed to run the unit tests.
#  make lib-static - creates libmidcutils.a in the current directory
#                    you will need to install it yourself and run
#                    ranlib on it
#  make clean      - removes .o and .a files
#  make distclean  - removes all binaries, including those created in
#                    the test dir
#  make help       - shows all the targets
#
##########################################################################
#  
#  === Note on running the unit tests === 
#  The unit tests use the midcunit unit test framework, which is just
#  one header file, midcunit.h. midcunit is available from: 
#  https://github.com/midpeter444/midcunit
#  
#  This makefile expects the header to installed in /usr/local/lib,
#  but you can change that by editing the program_INCLUDE_DIRS variable
#  in the Makefile in the test directory.
#
##########################################################################

##### basic compilation section #####
prg_NAME := midcutils
prg_SRCS := $(wildcard *.c)
prg_OBJS := ${prg_SRCS:.c=.o}

prg_INCLUDE_DIRS := .
prg_LIBRARY_DIRS :=
prg_LIBRARIES :=

# these are the standard flags I compile with (for gcc)
# add -g if you want to compile with symbols for debugging
CFLAGS+=-Wall -O2 -W -pedantic -std=c99 -D_GNU_SOURCE
CPPFLAGS += $(foreach includedir,$(prg_INCLUDE_DIRS),-I$(includedir))
LDFLAGS += $(foreach librarydir,$(prg_LIBRARY_DIRS),-L$(librarydir))
LDFLAGS += $(foreach library,$(prg_LIBRARIES),-l$(library))

.PHONY: all test clean distclean lib-static lib-shared

all: $(prg_OBJS)

$(prg_NAME): $(prg_OBJS)
	@ ls > /dev/null  # 'no-op' to prevent it from building an executable


###### create a static midcutil library (Unix) ######
midcutils.a: all
	$(AR) -crusv $@ $(prg_OBJS)

lib-static: midcutils.a


##### test #####
# make sure we can call $(MAKE)
ifeq ($(MAKE),)
    MAKE := make
endif

test: 
	@ $(MAKE) -C test test

##### misc helper targets #####

help:
	@ echo "== Targets =="
	@ egrep "^[^: ]+:" [Mm]akefile | egrep -v "^\." | cut "-d:" -f1 

clean:
	@- $(RM) *.o *.a

distclean: clean
	@- file * | grep ELF | cut "-d:" -f1 | xargs $(RM) 2>/dev/null; ls >/dev/null;
	@ $(MAKE) -C test distclean > /dev/null
