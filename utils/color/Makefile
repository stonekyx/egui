CC = $(CROSS_COMPILE)gcc
CFLAGS = -g -Wall -W -pedantic

CP = cp
RM = rm
MAKE = make
AR = ar
LN = ln

LIBRARY = $(shell pwd)/../../library
INCLUDE = $(shell pwd)/../../include

OBJECTS = color.o

SONAME = libcolor.so.1
REALNAME = libcolor.so.1.0.0
SIMPNAME = libcolor.so

all: $(OBJECTS)
	$(CC) -shared -Wl,-soname,$(SONAME) \
		-o $(LIBRARY)/$(REALNAME) $(OBJECTS) -lc
	$(LN) -sf $(REALNAME) $(LIBRARY)/$(SONAME)
	$(LN) -sf $(SONAME) $(LIBRARY)/$(SIMPNAME)

color.o: color.c
	$(CC) $(CFLAGS) -fPIC -c color.c -I $(INCLUDE)

clean:
	$(RM) -f $(OBJECTS) $(LIBRARY)/$(SONAME) $(LIBRARY)/$(REALNAME) $(LIBRARY)/$(SIMPNAME)
