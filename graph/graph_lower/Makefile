CC = $(CROSS_COMPILE)gcc
CFLAGS = -std=iso9899:1990 -pedantic -Wall -g

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
../graph_lower.h

source = \
screen_init.c \
screen_exit.c \
screen_get_width.c \
screen_get_height.c \
screen_color.c \
screen_clear.c \
screen_clear_black.c \
screen_clear_white.c \
screen_set_pixel.c \
screen_set_h_line.c \
screen_get_pixel.c \
screen_set_v_line.c \
screen_set_x_line.c \
screen_set_area.c \
screen_cpy_area.c \
screen_flush.c



object = \
screen_init.o \
screen_exit.o \
screen_get_width.o \
screen_get_height.o \
screen_color.o \
screen_clear.o \
screen_clear_black.o \
screen_clear_white.o \
screen_set_pixel.o \
screen_set_h_line.o \
screen_get_pixel.o \
screen_set_v_line.o \
screen_set_x_line.o \
screen_set_area.o \
screen_cpy_area.o \
screen_flush.o




$(DEFAULT): $(header) $(source) $(object)

%.o: %.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@ -I $(INCLUDE)
	if ( ! grep -q "graph_lower/$@" ../objects.lst); then \
		echo graph_lower/$@ >> ../objects.lst; \
		fi

$(CLEAN):
	$(RM) -f $(object)
