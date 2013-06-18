CC = $(CROSS_COMPILE)gcc
CFLAGS = -std=iso9899:1990 -pedantic -Wall -g

DEFAULT = default
CLEAN = clean

.PHONY: $(DEFAULT) $(CLEAN)

$(DEFAULT):
	make --directory=data_structures
	make --directory=geometry
	make --directory=config_parser
	make --directory=color

$(CLEAN):
	make clean -C data_structures
	make clean -C geometry
	make clean -C config_parser
	make clean -C color

