CC = $(CROSS_COMPILE)gcc
CFLAGS = -std=iso9899:1990 -pedantic -Wall

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
text_line.h

source = \
text_line.c

object = \
text_line.o

$(DEFAULT): $(header) $(source) $(object)


%.o: %.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@ -I $(INCLUDE)

$(CLEAN):
	$(RM) -f $(object)
