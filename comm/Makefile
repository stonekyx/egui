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
egui_uds.h \
event_listener.h \
egui_comm.h \
message.h \
packet.h \
request.h \
respond.h

source = \
egui_uds.c \
event_listener.c \
request.c \
message.c \
respond.c \
egui_comm.c

object = \
egui_uds.o \
event_listener.o \
request.o \
message.o \
respond.o \
egui_comm.o

$(DEFAULT): $(header) $(source) $(object)
	$(CC) -fPIC -shared $(object) -o comm.so
	$(CP) comm.so $(LIBRARY)/libcomm.so

%.o: %.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@ -I $(INCLUDE)

$(CLEAN):
	$(RM) -f *.so *.o test_server test_client test_uds_server test_uds_client

