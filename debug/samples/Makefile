CC = $(CROSS_COMPILE)gcc
CFLAGS = -std=iso9899:1990 -g -pedantic -Wall -g

CD = cd
CP = cp
MV = mv
RM = rm
MAKE = make

INCLUDE = $(shell pwd)/../../include
LIBRARY = $(shell pwd)/../../library
DEP_LIBS = -lwidget -lapplication -lwidget -lclient_lib -lgeometry -lcomm -lconfig_parser -ldata_structures

DEFAULT = default
CLEAN = clean

.PHONY: $(DEFAULT) $(CLEAN)

header =

source = \
single_window.c \
multi_windows.c \
shutdown.c \
pop_up_window.c \
toplevel_child.c \
editerbasic.c \
#paint.c \
simple_desktop.c \
traversal.c \
icon.c \
panel.c

object = \
single_window.o \
multi_windows.o \
shutdown.o \
pop_up_window.o \
toplevel_child.o \
editerbasic.o \
#paint.o \
simple_desktop.o \
traversal.o \
icon.o \
panel.o

$(DEFAULT): $(header) $(source) $(object)
	$(CC) $(CFLAGS) multi_windows.o -o multi_windows -I $(INCLUDE) -L $(LIBRARY) $(DEP_LIBS)
	$(CC) $(CFLAGS) shutdown.o -o shutdown -I $(INCLUDE) -L $(LIBRARY) $(DEP_LIBS)
	$(CC) $(CFLAGS) pop_up_window.o -o pop_up_window -I $(INCLUDE) -L $(LIBRARY) $(DEP_LIBS)
	$(CC) $(CFLAGS) single_window.o -o single_window -I $(INCLUDE) -L $(LIBRARY) $(DEP_LIBS)
	$(CC) $(CFLAGS) toplevel_child.o -o toplevel_child -I $(INCLUDE) -L $(LIBRARY) $(DEP_LIBS)
	$(CC) $(CFLAGS) editerbasic.o -o editerbasic -I $(INCLUDE) -L $(LIBRARY) $(DEP_LIBS)
	$(MAKE) --directory=file_browser
	$(MAKE) --directory=calculator
	#$(CC) $(CFLAGS) traversal.o -o traversal -I $(INCLUDE) -L $(LIBRARY) $(DEP_LIBS)
	#$(CC) $(CFLAGS) paint.o -o paint -I $(INCLUDE) -L $(LIBRARY) $(DEP_LIBS)
# $(MAKE) --directory=vkeybd

%.o: %.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@ -I $(INCLUDE) -L $(INCLUDE)

$(CLEAN):
	$(RM) -f *.o paint shutdown widget label button text_line single_window multi_windows pop_up_window toplevel_child traversal icon editerbasic
	$(MAKE) clean -C file_browser
	$(MAKE) clean -C calculator
