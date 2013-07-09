CC = $(CROSS_COMPILE)gcc
CFLAGS = -g -Wall -W -pedantic

CP = cp
RM = rm
MAKE = make
AR = ar

LIBRARY = $(shell pwd)/../../library
INCLUDE = $(shell pwd)/../../include

all: color.o
	$(AR) cr libcolor.a color.o
	$(CP) -vf libcolor.a $(LIBRARY)/libcolor.a

color.o: color.c
	$(CC) $(CFLAGS) -fPIC -c color.c -I $(INCLUDE)

clean:
	$(RM) -vf *.o *.a
