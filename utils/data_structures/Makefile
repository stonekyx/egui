CC = $(CROSS_COMPILE)gcc
CFLAGS = -std=iso9899:1990 -pedantic -Wall -g

CD = cd
CP = cp
MV = mv
RM = rm
MAKE = make
LN = ln

INCLUDE = $(shell pwd)/../../include
LIBRARY = $(shell pwd)/../../library

DEFAULT = default
CLEAN = clean

.PHONY: $(DEFAULT) $(CLEAN)

header =

source = \
vector.c \
list.c \
stack.c \
queue.c \
tree.c \
object.c

object = \
vector.o \
list.o \
stack.o \
queue.o \
tree.o \
object.o

SONAME = libdata_structures.so.1
REALNAME = libdata_structures.so.1.0.0
SIMPNAME = libdata_structures.so

$(DEFAULT): $(header) $(source) $(object)
	$(CC) -shared -Wl,-soname,$(SONAME) \
		-o $(LIBRARY)/$(REALNAME) $(object) -lc
	$(LN) -sf $(REALNAME) $(LIBRARY)/$(SONAME)
	$(LN) -sf $(SONAME) $(LIBRARY)/$(SIMPNAME)

%.o: %.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@ -I $(INCLUDE)

$(CLEAN):
	$(RM) -f $(object) $(LIBRARY)/$(SONAME) $(LIBRARY)/$(REALNAME) $(LIBRARY)/$(SIMPNAME)

