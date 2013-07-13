CC = $(CROSS_COMPILE)gcc
CFLAGS = -std=iso9899:1990 -pedantic -Wall -g

CD = cd
CP = cp
MV = mv
RM = rm
MAKE = make

INCLUDE = $(shell pwd)/../../../include
LIBRARY = $(shell pwd)/../../../library
DEP_LIBS = -lwidget -lapplication -lwidget -lclient_lib -lgeometry -lcomm -lconfig_parser -ldata_structures

DEFAULT = default
CLEAN = clean

.PHONY: $(DEFAULT) $(CLEAN)

header = \
calculator.h

source = \
calculator.c \
main.c

object = \
calculator.o \
main.o

$(DEFAULT): $(header) $(source) $(object)
	$(CC) $(CFLAGS) $(object) -o calculator -I $(INCLUDE) -L $(LIBRARY) $(DEP_LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@ -I $(INCLUDE)

$(CLEAN):
	$(RM) -f *.o calculator
