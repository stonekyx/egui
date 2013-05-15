CC = $(CROSS_COMPILE)gcc
CFLAGS = -std=iso9899:1990 -pedantic -Wall
AR = $(CROSS_COMPILE)ar

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

header = \
debug_graph.h \
debug_graph_lower.h \
debug_graph_engine.h

source = \
debug_graph.c \
debug_graph_lower.c \
debug_graph_engine.c

object = \
debug_graph.o \
debug_graph_lower.o \
debug_graph_engine.o

$(DEFAULT): $(object)
	$(CC) $(object) -ldata_structures -lgeometry -lgraph -L $(LIBRARY) -o debug_graph

%.o: %.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@ -I $(INCLUDE) 

$(CLEAN):
	$(RM) -f *.o

