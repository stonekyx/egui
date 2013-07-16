CC = $(CROSS_COMPILE)gcc
CFLAGS = -std=iso9899:1990 -pedantic -Wall -g

CD = cd
CP = cp
MV = mv
RM = rm
MAKE = make
LN = ln

INCLUDE = $(shell pwd)/../include
LIBRARY = $(shell pwd)/../library

DEFAULT = default
CLEAN = clean

OBJLIST = objects.lst

SONAME = libgraph.so.1
REALNAME = libgraph.so.1.0.0
SIMPNAME = libgraph.so

.PHONY: $(DEFAULT) $(CLEAN)

$(DEFAULT):
	$(MAKE) --directory=graph_lower
	$(MAKE) --directory=graph_engine
	$(CC) -shared -Wl,-soname,$(SONAME) \
		-o $(LIBRARY)/$(REALNAME) `cat $(OBJLIST)` -lc
	$(LN) -sf $(REALNAME) $(LIBRARY)/$(SONAME)
	$(LN) -sf $(SONAME) $(LIBRARY)/$(SIMPNAME)

$(CLEAN):
	$(MAKE) --directory=graph_lower clean
	$(MAKE) --directory=graph_engine clean
	$(RM) -f $(LIBRARY)/$(SONAME) $(LIBRARY)/$(REALNAME) $(LIBRARY)/$(SIMPNAME) $(OBJLIST)


