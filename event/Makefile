CC = $(CROSS_COMPILE)gcc
CFLAGS = -g -std=iso9899:1990 -pedantic -Wall -g -O3 -mtune=native

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

header = event.h

source = keybd.c \
		 mouse.c 

object = keybd.o \
		 mouse.o 

$(DEFAULT): $(header) $(source) $(object)
	$(CC) -fPIC -shared $(object) -L $(LIBRARY) -lcomm -o event.so
	$(CP) event.so $(LIBRARY)/libevent.so
%.o: %.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@ -I $(INCLUDE)

$(CLEAN):
	$(RM) -f *.so *.o

