noinst_PROGRAMS += debug/config_parser/main
debug_config_parser_main_SOURCES = debug/config_parser/main.c
debug_config_parser_main_LDADD = library/libevent.la library/libcomm.la\
	library/libconfig_parser.la\
	library/libdata_structures.la
