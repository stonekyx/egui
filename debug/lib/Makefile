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
	$(CC) test_server.o -ldata_structures -lcomm -lgeometry -lgraph -lserver_lib -lcolor -L $(LIBRARY) -o test_server
	$(CC) test_client.o -ldata_structures -lcomm -lgeometry -lclient_lib -lcolor -L $(LIBRARY) -o test_client

%.o: %.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@ -I $(INCLUDE)

$(CLEAN):
	$(RM) -f test_server test_client *.o

