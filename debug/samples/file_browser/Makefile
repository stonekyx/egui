CC = $(CROSS_COMPILE)gcc
CFLAGS = -std=iso9899:1990 -pedantic -Wall -g

CD = cd
CP = cp
MV = mv
RM = rm
MAKE = make

INCLUDE = $(shell pwd)/../../../include
LIBRARY = $(shell pwd)/../../../library
DEP_LIBS = -ldata_structures -lcomm -lwidget -lapplication -lclient_lib -lconfig_parser -lgeometry 

DEFAULT = default
CLEAN = clean

header = \
directory_item.h \
directory_content.h \
my_widget.h

source = \
directory_content.c \
my_widget.c \
file_browser.c

object = \
directory_content.o \
my_widget.o \
file_browser.o

$(DEFAULT): $(header) $(source) $(object)
	$(CC) $(CFLAGS) $(object) -o file_browser -I $(INCLUDE) -L $(LIBRARY) $(DEP_LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@ -I $(INCLUDE)

$(CLEAN):
	$(RM) -f *.o file_browser
