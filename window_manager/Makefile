CC = $(CROSS_COMPILE)gcc
CFLAGS = -std=iso9899:1990 -g -pedantic -Wall -lpthread -g

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
application_info.h \
cursor.h \
flush_in_z_order.h \
window_info.h \
window_info_iterator.h \
window_manager.h


source = \
application_info.c \
cursor.c \
flush_in_z_order.c \
main.c \
main_test.c \
window_info.c \
window_manager.c \
window_info_iterator.c \
window_manager_accept_handler.c \
window_manager_input_handler.c \
window_manager_set_request_handler.c

object = \
application_info.o \
cursor.o \
flush_in_z_order.o \
main.o \
window_info.o \
window_manager.o \
window_info_iterator.o \
window_manager_accept_handler.o \
window_manager_input_handler.o \
window_manager_set_request_handler.o

object_test = \
application_info.o \
cursor.o \
flush_in_z_order.o \
main_test.o \
window_info.o \
window_manager.o \
window_info_iterator.o \
window_manager_accept_handler.o \
window_manager_input_handler.o \
window_manager_set_request_handler.o

$(DEFAULT): $(header) $(source) $(object) $(object_test)
	$(CC) $(CFLAGS) $(object) -o server -I $(INCLUDE) -L $(LIBRARY) -ldata_structures -lgeometry -lcomm -lgraph -levent -lserver_lib -lconfig_parser -lcolor
	$(CC) $(CFLAGS) $(object_test) -o tserver -I $(INCLUDE) -L $(LIBRARY) -ldata_structures -lgeometry -lcomm -lgraph -levent -lserver_lib -lconfig_parser -lcolor

%.o: %.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@ -I $(INCLUDE)

$(CLEAN):
	$(RM) -f *.o server tserver *.out

