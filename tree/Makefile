# TODO: will automate the compiler at some point too
CC = g++

# needed compiler flags. will change this so that it works with clang too.
CFLAGS = -MMD -Wall -Wextra -I./include/ -I./src/

# automatation of source files
SRCS_DIR = src
SRCS = $(wildcard $(addsuffix /*.cpp, $(SRCS_DIR)))

# automation of object files
OBJDIR = obj
OBJS = $(addprefix $(OBJDIR)/, $(notdir $(SRCS:.cpp=.o)))

# automation of dependencies
DEPS = $(OBJS:.o=.d)
vpath %.cpp src builtins

# default make is debug
all: build

build: test

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -f test
	rm -rf $(OBJDIR)

test : $(OBJS)
	$(CC) $(CFLAGS) -o test $(OBJS) $(LIBS)

# compile every source file with its dependencies
obj/%.o : %.cpp | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# cli options
.PHONY: build clean

-include $(DEPS)
