CC = $(CROSS_COMPILE)gcc
CFLAGS = -std=iso9899:1990 -pedantic -Wall -g

CD = cd
CP = cp
MV = mv
RM = rm
MAKE = make

INCLUDE = $(shell pwd)/../include
LIBRARY = $(shell pwd)/../library

DEFAULT = default
CLEAN = clean

.PHONY: $(DEFAULT) $(CLEAN)

header = \
server_lib.h

source = \
server_lib.c

object = \
server_lib.o

$(DEFAULT): $(header) $(source) $(object)
	$(CC) -fPIC -shared $(object) -L $(LIBRARY) -lcomm -ldata_structures -o server_lib.so
	$(CP) server_lib.so $(LIBRARY)/libserver_lib.so

%.o: %.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@ -I $(INCLUDE)

$(CLEAN):
	$(RM) -f *.so *.o
