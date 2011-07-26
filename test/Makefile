ut1_NAME := ut_midcutils
ut2_NAME := ut_midcstack
ut3_NAME := ut_midcqueue
ut4_NAME := ut_midc_pcre_gsub

ut1_C_SRCS := $(ut1_NAME).c
ut2_C_SRCS := $(ut2_NAME).c
ut3_C_SRCS := $(ut3_NAME).c
ut4_C_SRCS := $(ut4_NAME).c

ut1_OBJS := ${ut1_C_SRCS:.c=.o} ../midcutils.o
ut2_OBJS := ${ut2_C_SRCS:.c=.o} ../midcstack.o
ut3_OBJS := ${ut3_C_SRCS:.c=.o} ../midcqueue.o
ut4_OBJS := ${ut4_C_SRCS:.c=.o}

# /usr/local/include is needed for midcunit.h (unit test framework)
program_INCLUDE_DIRS := /usr/local/include ..
program_LIBRARY_DIRS :=
program_LIBRARIES :=

# these are the standard flags I compile with (for gcc)
# add -g if you want to compile with symbols for debugging
CFLAGS   += -Wall -O2 -W -pedantic -std=c99 -D_GNU_SOURCE
CPPFLAGS += $(foreach includedir,$(program_INCLUDE_DIRS),-I$(includedir))
LDFLAGS  += $(foreach librarydir,$(program_LIBRARY_DIRS),-L$(librarydir))
LDFLAGS  += $(foreach library,$(program_LIBRARIES),-l$(library))

.PHONY: all test tset clean distclean

#all: $(ut1_NAME) $(ut2_NAME) $(ut3_NAME) $(ut4_NAME)
all: $(ut1_NAME) $(ut2_NAME) $(ut3_NAME)

$(ut1_NAME): $(ut1_OBJS)

$(ut2_NAME): $(ut2_OBJS)

$(ut3_NAME): $(ut3_OBJS)

# bcs I mistype it all the time!
tset: test

test: $(ut1_NAME) $(ut2_NAME) $(ut3_NAME) run-tests

run-tests:
	@echo "------------------------"
	- ./$(ut1_NAME)
	@echo "------------------------"
	- ./$(ut2_NAME)
	@echo "------------------------"
	- ./$(ut3_NAME)

# *Note* only call this from the "test" target in libtool.mk
# in the parent directory after doing: make -f libtool.mk 
test_with_shared_lib: LDFLAGS := -L../.libs -lmidcutils 
test_with_shared_lib: LD_LIBRARY_PATH := ../.libs:.libs
test_with_shared_lib: ut1_OBJS := ${ut1_C_SRCS:.c=.o}
test_with_shared_lib: ut2_OBJS := ${ut2_C_SRCS:.c=.o}
test_with_shared_lib: ut3_OBJS := ${ut3_C_SRCS:.c=.o}
test_with_shared_lib: so_test1 so_test2 so_test3 run-tests

so_test1: ${ut1_C_SRCS:.c=.o} 
	$(LINK.c) $< -o $(ut1_NAME)

so_test2: ${ut2_C_SRCS:.c=.o}
	$(LINK.c) $< -o $(ut2_NAME)

so_test3: ${ut3_C_SRCS:.c=.o}
	$(LINK.c) $< -o $(ut3_NAME)

################

help:
	@ echo "== Targets =="
	@ egrep "^[^:. ]+:" [Mm]akefile | cut "-d:" -f1 

clean:
	@- $(RM) *.o

distclean: clean
	@- file * | grep ELF | cut "-d:" -f1 | xargs $(RM) 2>/dev/null; ls >/dev/null;

