CC = $(CROSS_COMPILE)gcc
CFLAGS = -std=iso9899:1990 -pedantic -Wall -g

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

header = \
application.h \
manipulate_window.h \
exec.h

source = \
application.c \
manipulate_window.c \
exec.c

object = \
application.o \
manipulate_window.o \
exec.o

SONAME = libapplication.so.1
REALNAME = libapplication.so.1.0.0
SIMPNAME = libapplication.so

$(DEFAULT): $(header) $(source) $(object)
	$(CC) -shared -Wl,-soname,$(SONAME) \
		-o $(LIBRARY)/$(REALNAME) $(object) -lc \
		-L$(LIBRARY) -lwidget -lclient_lib -lgeometry -lcomm -lconfig_parser -ldata_structures
	$(LN) -sf $(REALNAME) $(LIBRARY)/$(SONAME)
	$(LN) -sf $(SONAME) $(LIBRARY)/$(SIMPNAME)

%.o: %.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@ -I $(INCLUDE) -L $(LIBRARY)

$(CLEAN):
	$(RM) -f $(object) $(LIBRARY)/$(SONAME) $(LIBRARY)/$(REALNAME) $(LIBRARY)/$(SIMPNAME)
