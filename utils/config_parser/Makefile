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
config_parser.c

object = \
config_parser.o

$(DEFAULT): $(header) $(source) $(object)
	$(CC) -fPIC -shared $(object) -o config_parser.so
	$(CP) config_parser.so $(LIBRARY)/libconfig_parser.so

%.o: %.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@ -I $(INCLUDE)

$(CLEAN):
	$(RM) -f *.so *.o

