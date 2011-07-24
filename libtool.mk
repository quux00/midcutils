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

# edit these if you want to install the shared library and headers somewhere else
HDR_INSTALL_DIR = /usr/local/include
LIB_INSTALL_DIR = /usr/local/lib

define compile_rule
	libtool --mode=compile $(COMPILE.c) -c $<
endef
define link_rule
	libtool --mode=link $(LINK.c) -o $@ $^ -rpath $(LIB_INSTALL_DIR)
endef

### targets

.PHONY: all test help clean distclean

all: $(library_LOBJS) lib${library_NAME}.la

%.lo: %.c
	$(call compile_rule)

lib${library_NAME}.la: $(library_LOBJS)
	$(call link_rule)

install: lib${library_NAME}.la
	libtool --mode=install install -c $< $(LIB_INSTALL_DIR)/$<
	install -m 644 -t $(HDR_INSTALL_DIR) $(library_HDRS)

###############################

help:
	@ echo "== Targets =="
	@ egrep "^[^: ]+:" libtool.mk | egrep -v "^\." | cut "-d:" -f1

clean:
	@- $(RM) -r $(library_OBJS) $(library_LOBJS) lib${library_NAME}.la .libs

distclean: clean
	@- $(RM) *.o *.la *.lo *.a *.lai
	@- file * | grep ELF | cut "-d:" -f1 | xargs $(RM) 2>/dev/null; ls >/dev/null;
