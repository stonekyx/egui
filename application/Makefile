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

$(DEFAULT): $(header) $(source) $(object)
	$(CC) -fPIC -shared $(object) -o application.so
	$(CP) application.so $(LIBRARY)/libapplication.so

%.o: %.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@ -I $(INCLUDE) -L $(LIBRARY)

$(CLEAN):
	$(RM) -f *.so *.o
