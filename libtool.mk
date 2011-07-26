##########################################################################
#  There are two makefiles for midcutils: Makefile and libtool.mk
##########################################################################
#  
#  This one, libtool.mk, creates both a shared and static library, runs the
#  tests against the shared library, install the shared library and headers
#  and uninstall the shared library and headers.  It depends on libtool,
#  which you may need to install first if you don't already have it.
#  
##########################################################################
#
#  This other one, Makefile, can compile each part of midcutils separately
#  to .o files, and run tests (in the test dir) linked to the .o files.
#  And it can make static library via the lib-static target (Unix only).
# 
##########################################################################
# 
#  === Usage of this Makefile ===
#  This makefile depends on libtool and will, by default, install the
#  shared and static libraries in /usr/local/lib and the headers in
#  /usr/local/include.  You can change this by editing the variables:
#  LIB_INSTALL_DIR and HDR_INSTALL_DIR, respectively.
#  
#  make all       - libtool builds all the binaries, including the
#                   midcutils shared and static libraries
#  make test      - runs the unit tests against the shared library
#                   You must have midcunit installed to run the unit tests.
#  make install   - install the libraries and headers
#  make uninstall - uninstalls the libraries and headers
#  make distclean - removes all binaries, including those created in
#                   the test dir
#  make help      - shows all the targets
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

library_NAME  := midcutils
library_SRCS  := $(wildcard *.c)
library_OBJS  := ${library_SRCS:.c=.o}
library_LOBJS := ${library_SRCS:.c=.lo}
library_HDRS  := ${library_SRCS:.c=.h}
library_INCLUDE_DIRS := /usr/local/include
library_LIBRARY_DIRS := /usr/local/lib
library_LIBRARIES    :=
#library_LIBRARIES    := pcre  => in the future add this when get pcre_sub/gsub working

# change this if you don't want gcc, but on Ubuntu Linux, just using $(CC)
# doesn't work because libtool won't put in the -fPIC -DPIC switches unless
# it thinks it is gcc (even when cc is a link to gcc, sigh...)
CC        = gcc
CFLAGS   += -Wall -O2 -W -std=c99 -D_GNU_SOURCE -g
CPPFLAGS += $(foreach includedir,$(library_INCLUDE_DIRS),-I$(includedir))
LDFLAGS  += $(foreach librarydir,$(library_LIBRARY_DIRS),-L$(librarydir))
LDFLAGS  += $(foreach library,$(library_LIBRARIES),-l$(library))

ifeq ($(LIBTOOL),)
	LIBTOOL := libtool
endif

# edit these if you want to install the shared library and headers somewhere else
HDR_INSTALL_DIR := /usr/local/include
LIB_INSTALL_DIR := /usr/local/lib

define compile_rule
	$(LIBTOOL) --mode=compile $(COMPILE.c) -c $<
endef
define link_rule
	$(LIBTOOL) --mode=link $(LINK.c) -o $@ $^ -rpath $(LIB_INSTALL_DIR)
endef

### targets

.PHONY: all test help clean distclean

all: $(library_LOBJS) lib${library_NAME}.la

%.lo: %.c
	$(call compile_rule)

lib${library_NAME}.la: $(library_LOBJS)
	$(call link_rule)

install: lib${library_NAME}.la
	$(LIBTOOL) --mode=install install -c $< $(LIB_INSTALL_DIR)/$<
	install -m 644 -t $(HDR_INSTALL_DIR) $(library_HDRS)

uninstall: INSTALLED_HDRS += $(foreach includedir,$(library_HDRS),$(HDR_INSTALL_DIR)/$(includedir))
uninstall: 
	$(LIBTOOL) --mode=uninstall $(RM) "$(LIB_INSTALL_DIR)/libmidcutils.*" $(INSTALLED_HDRS)

############ Test #############

# make sure we can call $(MAKE)
ifeq ($(MAKE),)
	MAKE := make
endif

test:
	@ $(MAKE) -C test test_with_shared_lib

############ Misc #############

help:
	@ echo "== Targets =="
	@ egrep "^[^: ]+:" libtool.mk | egrep -v "^\." | cut "-d:" -f1

clean:
	@- $(RM) -r $(library_OBJS) $(library_LOBJS) lib${library_NAME}.la .libs

distclean: clean
	@- $(RM) *.o *.la *.lo *.a *.lai
	@- file * | grep ELF | cut "-d:" -f1 | xargs $(RM) 2>/dev/null; ls >/dev/null;
