CC = $(CROSS_COMPILE)gcc
CFLAGS = -std=iso9899:1990 -pedantic -Wall -g

DEFAULT = default
CLEAN = clean

.PHONY: $(DEFAULT) $(CLEAN)

$(DEFAULT):
	make --directory=config_parser
	make --directory=comm
	make --directory=event
	make --directory=lib
	make --directory=widget
	make --directory=samples

$(CLEAN):
	make clean -C config_parser
	make clean -C comm
	make clean -C event
	make clean -C lib
	make clean -C widget
	make clean -C samples
