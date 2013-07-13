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

source = \
test_server.c \
test_client.c

object = \
test_server.o \
test_client.o

$(DEFAULT): $(object)
	$(CC) test_server.o -lserver_lib -lgraph -lgeometry -lcomm -ldata_structures -lcolor -L $(LIBRARY) -o test_server
	$(CC) test_client.o -lclient_lib -lgeometry -lcomm -ldata_structures -lcolor -L $(LIBRARY) -o test_client

%.o: %.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@ -I $(INCLUDE)

$(CLEAN):
	$(RM) -f test_server test_client *.o

