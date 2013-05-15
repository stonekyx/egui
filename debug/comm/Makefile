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
test_client.c \
test_uds_server.c \
test_uds_client.c 

object = \
test_server.o \
test_client.o \
test_uds_server.o \
test_uds_client.o 

$(DEFAULT): $(object)
	$(CC) test_server.o -ldata_structures -lcomm -L $(LIBRARY) -o test_server
	$(CC) test_client.o -ldata_structures -lcomm -L $(LIBRARY) -o test_client
	$(CC) test_uds_server.o -ldata_structures -lcomm -L $(LIBRARY) -o test_uds_server
	$(CC) test_uds_client.o -ldata_structures -lcomm -L $(LIBRARY) -o test_uds_client

%.o: %.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@ -I $(INCLUDE)

$(CLEAN):
	$(RM) -f test_server test_client test_uds_server test_uds_client *.o

