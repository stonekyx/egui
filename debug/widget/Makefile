CC = $(CROSS_COMPILE)gcc
CFLAGS = -std=iso9899:1990 -g -pedantic -Wall -g

CD = cd
CP = cp
MV = mv
RM = rm -f
MAKE = make

INCLUDE = $(shell pwd)/../../include
LIBRARY = $(shell pwd)/../../library
DEP_LIBS = -lwidget -lapplication -lwidget -lclient_lib -lgeometry -lcomm -lconfig_parser -ldata_structures

DEFAULT = default
CLEAN = clean

.PHONY: $(DEFAULT) $(CLEAN)

header =

source = \
test_widget.c \
test_label.c \
test_button.c \
test_scroll_bar.c \
test_text_line.c \
test_image_view.c \
#panel.c \
test_icon.c \

object = \
test_widget.o \
test_label.o \
test_button.o \
test_scroll_bar.o \
test_text_line.o \
test_image_view.o \
#panel.o \
test_icon.o \

$(DEFAULT): $(header) $(source) $(object)
	$(CC) $(CFLAGS) test_widget.o -o test_widget -I $(INCLUDE) -L $(LIBRARY) $(DEP_LIBS)
	$(CC) $(CFLAGS) test_label.o -o test_label -I $(INCLUDE) -L $(LIBRARY) $(DEP_LIBS)
	$(CC) $(CFLAGS) test_button.o -o test_button -I $(INCLUDE) -L $(LIBRARY) $(DEP_LIBS)
	$(CC) $(CFLAGS) test_text_line.o -o test_text_line -I $(INCLUDE) -L $(LIBRARY) $(DEP_LIBS)
	$(CC) $(CFLAGS) test_scroll_bar.o -o test_scroll_bar -I $(INCLUDE) -L $(LIBRARY) $(DEP_LIBS)
	$(CC) $(CFLAGS) test_image_view.o -o test_image_view -I $(INCLUDE) -L $(LIBRARY) $(DEP_LIBS)
	#$(CC) $(CFLAGS) test_icon.o -o test_icon -I $(INCLUDE) -L $(LIBRARY) $(DEP_LIBS)
# $(MAKE) --directory=vkeybd

%.o: %.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@ -I $(INCLUDE) -L $(INCLUDE)

$(CLEAN):
	$(RM) *.o test_widget test_label test_button test_scroll_bar test_text_line test_image_view
