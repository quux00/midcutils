pg1_NAME := midcutils
pg2_NAME := midcstack
pg3_NAME := midcqueue
pg4_NAME := midc_pcre_gsub

pg1_C_SRCS := $(pg1_NAME).c
pg2_C_SRCS := $(pg2_NAME).c $(pg1_C_SRCS)
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
CFLAGS+=-Wall -W -pedantic -std=c99 -D_GNU_SOURCE
CPPFLAGS += $(foreach includedir,$(program_INCLUDE_DIRS),-I$(includedir))
LDFLAGS += $(foreach librarydir,$(program_LIBRARY_DIRS),-L$(librarydir))
LDFLAGS += $(foreach library,$(program_LIBRARIES),-l$(library))

.PHONY: all test clean distclean

# all: $(pg1_OBJS) $(pg2_OBJS) $(pg3_OBJS) $(pg4_OBJS)
all: $(pg1_OBJS) $(pg3_OBJS)

$(pg3_NAME): $(pg3_OBJS)
	@ ls > /dev/null  # need a 'no-op' here to prevent it from
                    # building an executable -just want object file

################

test: 
  #~TODO: need to include or invoke the Makefile in the test dir ...

help:
	@ echo "== Targets =="
	@ egrep "^[^:. ]+:" [Mm]akefile | cut "-d:" -f1 

clean:
	@- $(RM) *.o

distclean: clean
	@- file * | grep ELF | cut "-d:" -f1 | xargs $(RM) 2>/dev/null; ls >/dev/null;
