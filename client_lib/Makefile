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
client_lib.h

source = \
client_lib.c

object = \
client_lib.o

$(DEFAULT): $(header) $(source) $(object)
	$(CC) -fPIC -shared $(object) -L $(LIBRARY) -o client_lib.so
	$(CP) client_lib.so $(LIBRARY)/libclient_lib.so

%.o: %.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@ -I $(INCLUDE)

$(CLEAN):
	$(RM) -f *.so *.o
