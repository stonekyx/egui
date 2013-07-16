CC = $(CROSS_COMPILE)gcc
CFLAGS = -g -std=iso9899:1990 -pedantic -Wall -g

CD = cd
CP = cp
MV = mv
RM = rm
MAKE = make
LN = ln

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

SONAME = libevent.so.1
REALNAME = libevent.so.1.0.0
SIMPNAME = libevent.so

$(DEFAULT): $(header) $(source) $(object)
	$(CC) -shared -Wl,-soname,$(SONAME) \
		$(object) -L $(LIBRARY) -lcomm -ldata_structures -lc \
		-o $(LIBRARY)/$(REALNAME)
	$(LN) -sf $(REALNAME) $(LIBRARY)/$(SONAME)
	$(LN) -sf $(SONAME) $(LIBRARY)/$(SIMPNAME)

%.o: %.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@ -I $(INCLUDE)

$(CLEAN):
	$(RM) -f $(object) $(LIBRARY)/$(SONAME) $(LIBRARY)/$(REALNAME) $(LIBRARY)/$(SIMPNAME)

