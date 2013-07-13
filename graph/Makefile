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

$(DEFAULT):
	$(MAKE) --directory=graph_lower
	$(MAKE) --directory=graph_engine
	$(CC) -fPIC -shared *.o -o graph.so
	$(CP) graph.so $(LIBRARY)/libgraph.so

$(CLEAN):
	$(RM) -f *.so *.o


