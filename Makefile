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
pg1_NAME := midcutils
pg2_NAME := midcstack
pg3_NAME := midcqueue
pg4_NAME := midc_pcre_gsub

pg1_C_SRCS := $(pg1_NAME).c
pg2_C_SRCS := $(pg2_NAME).c
pg3_C_SRCS := $(pg3_NAME).c
pg4_C_SRCS := $(pg3_NAME).c $(pg1_C_SRCS)

pg1_OBJS := ${pg1_C_SRCS:.c=.o}
pg2_OBJS := ${pg2_C_SRCS:.c=.o}
pg3_OBJS := ${pg3_C_SRCS:.c=.o}
pg4_OBJS := ${pg4_C_SRCS:.c=.o}

program_INCLUDE_DIRS := .
program_LIBRARY_DIRS :=
program_LIBRARIES :=

# these are the standard flags I compile with (for gcc)
# add -g if you want to compile with symbols for debugging
CFLAGS+=-Wall -O2 -W -pedantic -std=c99 -D_GNU_SOURCE
CPPFLAGS += $(foreach includedir,$(program_INCLUDE_DIRS),-I$(includedir))
LDFLAGS += $(foreach librarydir,$(program_LIBRARY_DIRS),-L$(librarydir))
LDFLAGS += $(foreach library,$(program_LIBRARIES),-l$(library))

.PHONY: all test clean distclean lib-static lib-shared

# all: $(pg1_OBJS) $(pg2_OBJS) $(pg3_OBJS) $(pg4_OBJS)
all: $(pg1_OBJS) $(pg2_OBJS) $(pg3_OBJS)

$(pg1_NAME): $(pg1_OBJS)
	@ ls > /dev/null  # 'no-op' to prevent it from building an executable

$(pg2_NAME): $(pg2_OBJS)
	@ ls > /dev/null  # need a 'no-op' here to prevent it from
                    # building an executable -just want object file

$(pg3_NAME): $(pg3_OBJS)
	@ ls > /dev/null  # 'no-op' to prevent it from building an executable


###### create a static midcutil library (Unix) ######
midcutils.a: all
	$(AR) -crusv $@ $(pg1_OBJS) $(pg2_OBJS) $(pg3_OBJS)

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
