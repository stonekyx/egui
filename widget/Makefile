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
	$(CC) -fPIC -shared *.o -L $(LIBRARY) -o widget.so
	$(CP) widget.so $(LIBRARY)/libwidget.so	
	
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
	$(RM) -f widget.so
	$(RM) -f *.o

