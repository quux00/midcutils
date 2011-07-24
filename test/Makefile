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

program_INCLUDE_DIRS := $(HOME)/c/midcunit ..
program_LIBRARY_DIRS :=
program_LIBRARIES :=

# these are the standard flags I compile with (for gcc)
# add -g if you want to compile with symbols for debugging
CFLAGS+=-Wall -O2 -W -pedantic -std=c99 -D_GNU_SOURCE
CPPFLAGS += $(foreach includedir,$(program_INCLUDE_DIRS),-I$(includedir))
LDFLAGS += $(foreach librarydir,$(program_LIBRARY_DIRS),-L$(librarydir))
LDFLAGS += $(foreach library,$(program_LIBRARIES),-l$(library))

.PHONY: all test tset clean distclean

#all: $(ut1_NAME) $(ut2_NAME) $(ut3_NAME) $(ut4_NAME)
all: $(ut1_NAME) $(ut2_NAME) $(ut3_NAME)

$(ut1_NAME): $(ut1_OBJS)

$(ut2_NAME): $(ut2_OBJS)

$(ut3_NAME): $(ut3_OBJS)

# bcs I mistype it all the time!
tset: test

test: $(ut1_NAME) $(ut2_NAME) $(ut3_NAME)
	@echo "------------------------"
	- ./$(ut1_NAME)
	@echo "------------------------"
	- ./$(ut2_NAME)
	@echo "------------------------"
	- ./$(ut3_NAME)

################

help:
	@ echo "== Targets =="
	@ egrep "^[^:. ]+:" [Mm]akefile | cut "-d:" -f1 

clean:
	@- $(RM) *.o

distclean: clean
	@- file * | grep ELF | cut "-d:" -f1 | xargs $(RM) 2>/dev/null; ls >/dev/null;

