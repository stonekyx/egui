CC = $(CROSS_COMPILE)gcc
CFLAGS = -std=iso9899:1990 -pedantic -Wall -g

CD = cd
CP = cp
MV = mv
RM = rm
MAKE = make

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

$(DEFAULT): $(header) $(source) $(object)
	$(CC) -fPIC -shared $(object) -o data_structures.so
	$(CP) data_structures.so $(LIBRARY)/libdata_structures.so

%.o: %.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@ -I $(INCLUDE)

$(CLEAN):
	$(RM) -f *.so *.o

