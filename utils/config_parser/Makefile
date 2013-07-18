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
config_parser.c

object = \
config_parser.o

SONAME = libconfig_parser.so.1
REALNAME = libconfig_parser.so.1.0.0
SIMPNAME = libconfig_parser.so

$(DEFAULT): $(header) $(source) $(object)
	$(CC) -shared -Wl,-soname,$(SONAME) \
		-o $(LIBRARY)/$(REALNAME) $(object) -lc \
		-L$(LIBRARY) -ldata_structures
	$(LN) -sf $(REALNAME) $(LIBRARY)/$(SONAME)
	$(LN) -sf $(SONAME) $(LIBRARY)/$(SIMPNAME)

%.o: %.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@ -I $(INCLUDE)

$(CLEAN):
	$(RM) -f $(object) $(LIBRARY)/$(SONAME) $(LIBRARY)/$(REALNAME) $(LIBRARY)/$(SIMPNAME)

