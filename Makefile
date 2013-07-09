CC = $(CROSS_COMPILE)gcc
CFLAGS = -std=iso9899:1990 -pedantic -Wall -g

LIBRARY = $(shell pwd)/library

DEFAULT = default
CLEAN = clean

.PHONY:  $(DEFAULT) $(CLEAN)

$(DEFAULT):
	make --directory=utils
	make --directory=comm
	make --directory=graph
	make --directory=event
	make --directory=client_lib
	make --directory=server_lib
	make --directory=widget
	make --directory=window_manager
	make --directory=application
	make --directory=debug

$(CLEAN):
	rm -rf $(LIBRARY)/*
	make clean -C utils
	make clean -C comm
	make clean -C graph
	make clean -C event
	make clean -C client_lib
	make clean -C server_lib
	make clean -C widget
	make clean -C window_manager
	make clean -C application
	make clean -C debug

