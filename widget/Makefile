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

SONAME = libwidget.so.1
REALNAME = libwidget.so.1.0.0
SIMPNAME = libwidget.so

OBJECTS = widget/widget.o \
		  button/button.o \
		  label/label.o \
		  panel/panel.o \
		  scroll_bar/scroll_bar.o \
		  text_line/text_line.o \
		  image_view/image_view.o \
		  window/window.o \
		  window/paint_window_decoration.o \
		  dialog/dialog.o

.PHONY: $(DEFAULT) $(CLEAN)

$(DEFAULT):
	$(MAKE) --directory=widget
	$(MAKE) --directory=button
	#$(MAKE) --directory=icon
	$(MAKE) --directory=label
	$(MAKE) --directory=panel
	$(MAKE) --directory=scroll_bar
	$(MAKE) --directory=text_line
	$(MAKE) --directory=image_view
	$(MAKE) --directory=window
	$(MAKE) --directory=dialog
	$(CC) -shared -Wl,-soname,$(SONAME) \
		-o $(LIBRARY)/$(REALNAME) $(OBJECTS) -lc \
		-L$(LIBRARY) -lclient_lib -lgeometry -lcomm -lconfig_parser -ldata_structures
	$(LN) -sf $(REALNAME) $(LIBRARY)/$(SONAME)
	$(LN) -sf $(SONAME) $(LIBRARY)/$(SIMPNAME)

$(CLEAN):
	$(MAKE) clean -C widget
	$(MAKE) clean -C button
	#$(MAKE) clean -C icon
	$(MAKE) clean -C label
	$(MAKE) clean -C panel
	$(MAKE) clean -C scroll_bar
	$(MAKE) clean -C text_line
	$(MAKE) clean -C image_view
	$(MAKE) clean -C window
	$(MAKE) clean -C dialog
	$(RM) -f $(LIBRARY)/$(SONAME) $(LIBRARY)/$(REALNAME) $(LIBRARY)/$(SIMPNAME)

