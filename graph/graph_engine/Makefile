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
../graph_lower.h \
../graph_engine.h

source = \
engine_get_color_limit.c \
engine_graphics_device_init.c \
engine_graphics_device_exit.c \
engine_clear.c \
engine_clear_black.c \
engine_clear_white.c \
engine_draw_arrow.c \
engine_draw_bitmap.c \
engine_draw_img.c \
engine_draw_circle.c \
engine_draw_ellipse.c \
engine_draw_line.c \
engine_draw_point.c \
engine_draw_polygon.c \
engine_draw_rectangle.c \
engine_draw_x.c \
engine_fill_arrow.c \
engine_fill_circle.c \
engine_fill_ellipse.c \
engine_fill_rectangle.c \
engine_fill_x.c \
engine_show_text.c \
engine_get_font_width.c \
engine_get_font_height.c \
engine_get_area.c \
engine_get_color.c \
engine_set_font.c \
engine_set_area.c \
engine_set_color.c \
engine_cpy_area.c \
engine_update.c \
font_matrix/font_matrix_08.c \
font_matrix/font_matrix_10.c \
font_matrix/font_matrix_12.c \
font_matrix/font_matrix_14.c \
font_matrix/font_matrix_16.c \
font_matrix/font_matrix_20.c \
font_matrix/font_matrix_24.c \
font_matrix/font_matrix_28.c \
font_matrix/font_matrix_32.c \
font_matrix/font_matrix_40.c \
font_matrix/font_matrix_48.c \
font_matrix/font_matrix_56.c \
font_matrix/font_matrix_64.c

object = \
engine_get_color_limit.o \
engine_graphics_device_init.o \
engine_graphics_device_exit.o \
engine_clear.o \
engine_clear_black.o \
engine_clear_white.o \
engine_draw_arrow.o \
engine_draw_bitmap.o \
engine_draw_img.o \
engine_draw_circle.o \
engine_draw_ellipse.o \
engine_draw_line.o \
engine_draw_point.o \
engine_draw_polygon.o \
engine_draw_rectangle.o \
engine_draw_x.o \
engine_fill_arrow.o \
engine_fill_circle.o \
engine_fill_ellipse.o \
engine_fill_rectangle.o \
engine_fill_x.o \
engine_show_text.o \
engine_get_font_width.o \
engine_get_font_height.o \
engine_get_area.o \
engine_get_color.o \
engine_set_font.o \
engine_set_area.o \
engine_set_color.o \
engine_cpy_area.o \
engine_update.o \
font_matrix/font_matrix_08.o \
font_matrix/font_matrix_10.o \
font_matrix/font_matrix_12.o \
font_matrix/font_matrix_14.o \
font_matrix/font_matrix_16.o \
font_matrix/font_matrix_20.o \
font_matrix/font_matrix_24.o \
font_matrix/font_matrix_28.o \
font_matrix/font_matrix_32.o \
font_matrix/font_matrix_40.o \
font_matrix/font_matrix_48.o \
font_matrix/font_matrix_56.o \
font_matrix/font_matrix_64.o

$(DEFAULT): $(header) $(source) $(object)
	$(MV) $(object) ..

%.o: %.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@ -I $(INCLUDE)

$(CLEAN):
	$(RM) -f *.o font_matrix/*.o
